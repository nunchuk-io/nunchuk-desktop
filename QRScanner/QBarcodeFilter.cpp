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

QBarcodeFilter::QBarcodeFilter(QObject *parent)
    : QObject(parent)
    , m_videoSink(new QVideoSink(this))
    , m_decoder(QSharedPointer<QBarcodeDecoder>::create())   // no parent — lifetime managed by shared ptr
    , m_processing(QSharedPointer<QAtomicInt>::create(0))
{
    // videoFrameChanged is emitted on the camera pipeline thread;
    // Qt::DirectConnection keeps latency low — processFrame must be re-entrant-safe.
    connect(m_videoSink, &QVideoSink::videoFrameChanged,
            this, &QBarcodeFilter::processFrame,
            Qt::DirectConnection);

    // tagFound is emitted from a QtConcurrent worker thread.
    // Qt::AutoConnection → QueuedConnection across threads, so onDecoderTagFound
    // always runs on the main thread where m_scanComplete / m_scanPercent live.
    // When QBarcodeFilter is destroyed, QObject::~QObject disconnects this
    // connection — any late tagFound emission from a still-running worker is
    // safely discarded (empty connection list, no crash).
    connect(m_decoder.data(), &QBarcodeDecoder::tagFound,
            this, &QBarcodeFilter::onDecoderTagFound,
            Qt::AutoConnection);
}

// ── videoSink ────────────────────────────────────────────────────────────────
QVideoSink* QBarcodeFilter::videoSink()
{
    return m_videoSink;
}

// ── videoOutput property ──────────────────────────────────────────────────────
// Declarative binding to a QML VideoOutput item.
// Reads VideoOutput.videoSink via QObject::property() and connects its
// videoFrameChanged signal to processFrame.  Disconnects from the previous
// sink when the binding changes (supports dynamic re-binding).
QObject* QBarcodeFilter::videoOutput() const
{
    return m_videoOutput;
}

void QBarcodeFilter::setVideoOutput(QObject* videoOutputItem)
{
    if (m_videoOutput == videoOutputItem)
        return;

    // Disconnect from previous VideoOutput's sink
    if (m_connectedSink) {
        disconnect(m_connectedSink, &QVideoSink::videoFrameChanged,
                   this,            &QBarcodeFilter::processFrame);
        m_connectedSink = nullptr;
    }

    m_videoOutput = videoOutputItem;

    if (videoOutputItem) {
        QVariant sinkVariant = videoOutputItem->property("videoSink");
        QVideoSink* sink = sinkVariant.value<QVideoSink*>();
        if (sink) {
            m_connectedSink = sink;
            connect(sink, &QVideoSink::videoFrameChanged,
                    this,  &QBarcodeFilter::processFrame,
                    Qt::DirectConnection);
        } else {
            qWarning("QBarcodeFilter::setVideoOutput: could not obtain QVideoSink from VideoOutput");
        }
    }

    emit videoOutputChanged();
}

// ── captureRect ───────────────────────────────────────────────────────────────
QRectF QBarcodeFilter::captureRect() const
{
    return m_captureRect;
}

void QBarcodeFilter::setCaptureRect(const QRectF &rect)
{
    if (m_captureRect == rect)
        return;
    m_captureRect = rect;
    emit captureRectChanged();
}

// ── scanPercent / scanComplete ────────────────────────────────────────────────
int QBarcodeFilter::scanPercent() const
{
    return m_scanPercent;
}

bool QBarcodeFilter::scanComplete() const
{
    return m_scanComplete;
}

// ── Frame processing ──────────────────────────────────────────────────────────
void QBarcodeFilter::processFrame(const QVideoFrame &frame)
{
    if (!frame.isValid())
        return;

    // Debounce: drop frames while a decode is already running.
    // testAndSetAcquire(expected, newValue): returns true only if the swap succeeded.
    if (!m_processing->testAndSetAcquire(0, 1))
        return;

    QVideoFrame copy(frame);
    if (!copy.map(QVideoFrame::ReadOnly)) {
        m_processing->storeRelease(0);
        return;
    }
    QImage image = copy.toImage();
    copy.unmap();

    if (image.isNull()) {
        m_processing->storeRelease(0);
        return;
    }

    // Crop to the QML-supplied capture rect (centre box) when valid.
    // captureRect is in source/camera pixel coordinates (set via
    // VideoOutput.mapRectToSource in QQrScanner.qml).
    const QRect cropRect = m_captureRect.toRect();
    if (!cropRect.isEmpty() && image.rect().intersects(cropRect)) {
        image = image.copy(image.rect().intersected(cropRect));
    }

    // Run decode on a worker thread.
    // IMPORTANT: capture shared-ptr copies, NOT `this`.  QBarcodeFilter may be
    // destroyed (Loader teardown) while this lambda is still running.  The shared
    // ptrs extend the lifetimes of decoder and processing flag independently of
    // QBarcodeFilter's own lifetime, preventing use-after-free crashes.
    auto decoder    = m_decoder;
    auto processing = m_processing;
    QtConcurrent::run([decoder, processing, image]() {
        decoder->process(image, ZXing::BarcodeFormat::QRCode);
        processing->storeRelease(0);
    });
}

// ── Decoder relay ─────────────────────────────────────────────────────────────
// Called on the main thread (QueuedConnection from worker thread).
void QBarcodeFilter::onDecoderTagFound(const QString &tag)
{
    if (!m_scanComplete) {
        m_scanComplete = true;
        emit scanCompleteChanged();
    }
    if (m_scanPercent != 100) {
        m_scanPercent = 100;
        emit scanPercentChanged();
    }
    emit tagFound(tag);
}
