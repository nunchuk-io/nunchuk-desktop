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
#ifndef QBARCODEFILTER_H
#define QBARCODEFILTER_H

#include <QAbstractVideoFilter>
#include <QtConcurrent/QtConcurrent>
#include <qqml.h>

#include "QBarcodeDecoder.h"
#include "BarcodeFormat.h"

void processImage(QBarcodeDecoder *decoder, const QImage &image, ZXing::BarcodeFormats formats);

class QBarcodeFilter : public QAbstractVideoFilter
{
    Q_OBJECT
    Q_PROPERTY(QRectF   captureRect     READ captureRect WRITE setCaptureRect   NOTIFY captureRectChanged)
    Q_PROPERTY(int      scanPercent     READ scanPercent                        NOTIFY scanPercentChanged)
    Q_PROPERTY(bool     scanComplete    READ scanComplete                       NOTIFY scanCompleteChanged)

public:
    explicit QBarcodeFilter(QObject *parent = nullptr);
    virtual ~QBarcodeFilter();

    QRectF captureRect() const;
    void setCaptureRect(const QRectF &captureRect);

    QBarcodeDecoder *getDecoder() const;
    QFuture<void> getImageFuture() const;
    QVideoFilterRunnable *createFilterRunnable() override;
    ZXing::BarcodeFormat format() const;

    int scanPercent() const;
    void setScanPercent(int newScanPercent);
    void resetTags();

    bool scanComplete() const;
    void setScanComplete(bool newScanComplete);

signals:
    void captureRectChanged(const QRectF &captureRect);
    void tagFound(const QString &tag);
    void scanPercentChanged();
    void scanCompleteChanged();

private slots:
    void calculateTags(const QString &tag);

private:
    QRectF          m_captureRect;
    QBarcodeDecoder *m_decoder;
    QFuture<void>   m_imageFuture;
    ZXing::BarcodeFormat m_format;
    int             m_scanPercent;
    bool            m_scanComplete;
    QStringList     m_tags;
};

class QBarcodeFilterRunnable : public QVideoFilterRunnable
{
public:
    QBarcodeFilterRunnable(QBarcodeFilter *filter);

    /*!
     * \fn QVideoFrame run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, QVideoFilterRunnable::RunFlags flags) override
     * \brief Run method in order to asynchronously process the input video frame.
     * \param QVideoFrame *input - a pointer to frame of video data.
     * \param const QVideoSurfaceFormat &surfaceFormat - the stream format of a video presentation surface.
     * \param QVideoFilterRunnable::RunFlags flags - typedef for QFlags<RunFlag>.
     */
    QVideoFrame run(QVideoFrame *    input,
                    const QVideoSurfaceFormat      &surfaceFormat,
                    QVideoFilterRunnable::RunFlags flags) override;

private:
    QBarcodeFilter *m_filter;
};

#endif // QBARCODEFILTER_H
