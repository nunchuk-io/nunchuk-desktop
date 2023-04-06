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

#ifndef QNUNCHUKIMAGEPROVIDER_H
#define QNUNCHUKIMAGEPROVIDER_H
#include <QObject>
#include <QAtomicPointer>
#include <QQuickImageProvider>
#include <QtCore/QReadWriteLock>
#include <QtCore/QThread>
#include <room.h>
#include <connection.h>
#include <ssosession.h>
#include <user.h>
#include <uriresolver.h>
#include <jobs/mediathumbnailjob.h>

using namespace Quotient;
using Quotient::BaseJob;

class ThumbnailResponse : public QQuickImageResponse
{
    Q_OBJECT
public:
    ThumbnailResponse(Connection* c, QString id, QSize size);
    ~ThumbnailResponse() override = default;

private slots:
    // All these run in the main thread, not QML thread
    void startRequest();
    void prepareResult();
    void doCancel();

private:
    Connection* c;
    const QString mediaId;
    const QSize requestedSize;
    Quotient::MediaThumbnailJob* job = nullptr;
    QImage image;
    QString errorStr;
    mutable QReadWriteLock lock; // Guards ONLY these two above
    // The following overrides run in QML thread
    QQuickTextureFactory *textureFactory() const override;
    QString errorString() const override;
    void cancel() override;
};

class QNunchukImageProvider : public QQuickAsyncImageProvider
{
public:
    QNunchukImageProvider();
    QQuickImageResponse* requestImageResponse(const QString& id, const QSize& requestedSize) override;
    void setConnection(Quotient::Connection* c);
private:
    QAtomicPointer<Quotient::Connection> m_connection;
    Q_DISABLE_COPY(QNunchukImageProvider)
};

#endif // QNUNCHUKIMAGEPROVIDER_H
