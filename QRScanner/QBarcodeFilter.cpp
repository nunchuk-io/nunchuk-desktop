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
#include "QBarcodeFilter.h"
#include <QImage>
#include <QtMultimedia/qvideoframe.h>
#include <QVideoFilterRunnable>
#include "QBarcodeDecoder.h"
#include "qUtils.h"
#include "QOutlog.h"

void processImage(QBarcodeDecoder *decoder, const QImage &image, ZXing::BarcodeFormats formats)
{
    decoder->process(image, formats);
}

QBarcodeFilterRunnable::QBarcodeFilterRunnable(QBarcodeFilter *filter) : m_filter{filter}
{

}

QVideoFrame QBarcodeFilterRunnable::run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags)
{
    Q_UNUSED(surfaceFormat);
    Q_UNUSED(flags);

    if (m_filter->getDecoder()->isDecoding()) {
        return *input;
    }
    if (m_filter->getImageFuture().isRunning()) {
        return *input;
    }
    const QImage croppedCapturedImage = m_filter->getDecoder()->videoFrameToImage(*input, m_filter->captureRect().toRect());
    m_filter->getImageFuture() = QtConcurrent::run(processImage,
                                                  m_filter->getDecoder(),
                                                  croppedCapturedImage,
                                                  m_filter->format());
    return *input;
}

QBarcodeFilter::QBarcodeFilter(QObject *parent)
    : QAbstractVideoFilter{parent}
    , m_decoder{new QBarcodeDecoder}
    , m_format{ZXing::BarcodeFormat::QRCode}
    , m_scanPercent{0}
{
    setScanPercent(0);
    setScanComplete(false);
    resetTags();
    connect(m_decoder, &QBarcodeDecoder::tagFound, this, &QBarcodeFilter::tagFound);
    connect(m_decoder, &QBarcodeDecoder::tagFound, this, &QBarcodeFilter::calculateTags);
}

QBarcodeFilter::~QBarcodeFilter()
{
    setScanPercent(0);
    setScanComplete(false);
    resetTags();
}

QVideoFilterRunnable *QBarcodeFilter::createFilterRunnable()
{
    return new QBarcodeFilterRunnable(this);
}

ZXing::BarcodeFormat QBarcodeFilter::format() const
{
    return m_format;
}

void QBarcodeFilter::calculateTags(const QString &tag)
{
    if(m_tags.contains(tag)){
        return;
    }
    m_tags.append(tag);
    nunchuk::AnalyzeQRResult ret = qUtils::AnalyzeQR(m_tags);
    setScanPercent(ret.estimated_percent_complete*100);
    setScanComplete(ret.is_complete);
    DBG_INFO << ret.expected_part_count << ret.processed_parts_count << ret.is_failure << ret.is_success;
}

bool QBarcodeFilter::scanComplete() const
{
    return m_scanComplete;
}

void QBarcodeFilter::setScanComplete(bool newScanComplete)
{
    if (m_scanComplete == newScanComplete)
        return;
    m_scanComplete = newScanComplete;
    emit scanCompleteChanged();
}

int QBarcodeFilter::scanPercent() const
{
    return m_scanPercent;
}

void QBarcodeFilter::setScanPercent(int newScanPercent)
{
    if (m_scanPercent == newScanPercent)
        return;
    m_scanPercent = newScanPercent;
    emit scanPercentChanged();
}

void QBarcodeFilter::resetTags()
{
    m_tags.clear();
}

QRectF QBarcodeFilter::captureRect() const
{
    return m_captureRect;
}

void QBarcodeFilter::setCaptureRect(const QRectF &captureRect)
{
    if (captureRect == m_captureRect) {
        return;
    }

    m_captureRect = captureRect;

    emit captureRectChanged(m_captureRect);
}

QBarcodeDecoder *QBarcodeFilter::getDecoder() const
{
    return m_decoder;
}

QFuture<void> QBarcodeFilter::getImageFuture() const
{
    return m_imageFuture;
}
