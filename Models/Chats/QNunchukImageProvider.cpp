/**************************************************************************
 * This file is part of the Nunchuk software (https://nunchuk.io/)        *
 * Copyright (C) 2020-2022 Enigmo								          *
 * Copyright (C) 2022 Nunchuk								              *
 *                                                                        *
 * This program is free software; you can redistribute it and/or          *
 * modify it under the terms of the GNU General Public License            *
 * as published by the Free Software Foundation; either version 3         *
 * of the License, or (at your option) any later version.                 *
 *                                                                        *
 * This program is distributed in the hope that it will be useful,        *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 * GNU General Public License for more details.                           *
 *                                                                        *
 * You should have received a copy of the GNU General Public License      *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
 *                                                                        *
 **************************************************************************/
#include "QNunchukImageProvider.h"
#include "QOutlog.h"

ThumbnailResponse::ThumbnailResponse(Connection *c, QString id, QSize size)
    : c(c), mediaId(std::move(id)), requestedSize(size)
{
    if (!c)
        errorStr = tr("No connection to perform image request");
    else if (mediaId.count('/') != 1)
        errorStr =
                tr("Media id '%1' doesn't follow server/mediaId pattern")
                .arg(mediaId);
    else if (requestedSize.isEmpty()) {
        qDebug() << "ThumbnailResponse: returning an empty image for"
                 << mediaId << "due to empty" << requestedSize;
        image = {requestedSize, QImage::Format_Invalid};
    }
    if (!errorStr.isEmpty() || requestedSize.isEmpty()) {
        emit finished();
        return;
    }
    // We are good to go
    qDebug().nospace() << "ThumbnailResponse: requesting " << mediaId
                       << ", " << size;
    errorStr = tr("Image request is pending");
    // Execute a request on the main thread asynchronously
    moveToThread(c->thread());
    QMetaObject::invokeMethod(this, &ThumbnailResponse::startRequest);
}

void ThumbnailResponse::startRequest()
{
    Q_ASSERT(QThread::currentThread() == c->thread());
    job = c->getThumbnail(mediaId, requestedSize);
    // Connect to any possible outcome including abandonment
    // to make sure the QML thread is not left stuck forever.
    connect(job, &BaseJob::finished, this, &ThumbnailResponse::prepareResult);
}

void ThumbnailResponse::prepareResult()
{
    Q_ASSERT(QThread::currentThread() == job->thread());
    Q_ASSERT(job->error() != BaseJob::Pending);
    {
        QWriteLocker _(&lock);
        if (job->error() == BaseJob::Success)
        {
            image = job->thumbnail();
            errorStr.clear();
            qDebug().nospace() << "ThumbnailResponse: image ready for "
                               << mediaId << ", " << image.size();
        } else if (job->error() == BaseJob::Abandoned) {
            errorStr = tr("Image request has been cancelled");
            qDebug() << "ThumbnailResponse: cancelled for" << mediaId;
        } else {
            errorStr = job->errorString();
            qWarning() << "ThumbnailResponse: no valid image for"
                       << mediaId << "-" << errorStr;
        }
    }
    job = nullptr;
    emit finished();
}

void ThumbnailResponse::doCancel()
{
    if (job){
        Q_ASSERT(QThread::currentThread() == job->thread());
        job->abandon();
    }
}

QQuickTextureFactory *ThumbnailResponse::textureFactory() const
{
    QReadLocker _(&lock);
    return QQuickTextureFactory::textureFactoryForImage(image);
}

QString ThumbnailResponse::errorString() const
{
    QReadLocker _(&lock);
    return errorStr;
}

void ThumbnailResponse::cancel()
{
    // Flip from QML thread to the main thread
    QMetaObject::invokeMethod(this, &ThumbnailResponse::doCancel );
}

QNunchukImageProvider::QNunchukImageProvider()
{

}

QQuickImageResponse *QNunchukImageProvider::requestImageResponse(const QString &id, const QSize &requestedSize)
{
    auto size = requestedSize;
    size.setWidth(qMin(300,requestedSize.width()));
    size.setHeight(qMin(300,requestedSize.height()));
    // Force integer overflow if the value is -1 - may cause issues when
    // screens resolution becomes 100K+ each dimension :-D
    QString tmpid = id;
    if(id.contains("mxc://")){
        tmpid = tmpid.remove("mxc://");
    }
    else{
        tmpid = id;
    }
    if (size.width() == -1)
        size.setWidth(ushort(-1));
    if (size.height() == -1)
        size.setHeight(ushort(-1));
    return new ThumbnailResponse(m_connection.load(), tmpid, size);
}

void QNunchukImageProvider::setConnection(Connection *c)
{
    m_connection.store(c);
}
