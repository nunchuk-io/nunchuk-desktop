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

#include <QObject>
#include <QRectF>
#include <QAtomicInt>
#include <QPointer>
#include <QSharedPointer>
#include <QVideoSink>
#include <QVideoFrame>
#include <QtConcurrent>
#include <QVariant>

#include "QBarcodeDecoder.h"
#include "BarcodeFormat.h"

class QBarcodeFilter : public QObject
{
    Q_OBJECT

    // Qt6: videoSink is owned by this object; QML / CaptureSession reads it
    Q_PROPERTY(QVideoSink* videoSink  READ videoSink  CONSTANT)
    // Declarative binding to a QML VideoOutput item.
    // Setting this property wires the VideoOutput's internal QVideoSink to the
    // barcode decoder automatically — no connectToVideoOutput() call needed.
    Q_PROPERTY(QObject* videoOutput   READ videoOutput WRITE setVideoOutput NOTIFY videoOutputChanged)
    // Crop rect in camera-source coordinates (set from QML via VideoOutput.mapRectToSource)
    Q_PROPERTY(QRectF captureRect     READ captureRect     WRITE setCaptureRect NOTIFY captureRectChanged)
    // Scan progress: 0..100 (100 = tag found).  Consumed by QQrScanner / QQrImportScanner.
    Q_PROPERTY(int  scanPercent       READ scanPercent      NOTIFY scanPercentChanged)
    Q_PROPERTY(bool scanComplete      READ scanComplete     NOTIFY scanCompleteChanged)

public:
    explicit QBarcodeFilter(QObject *parent = nullptr);

    QVideoSink* videoSink();

    QObject* videoOutput() const;
    void     setVideoOutput(QObject* videoOutputItem);

    QRectF captureRect() const;
    void   setCaptureRect(const QRectF &rect);

    int  scanPercent() const;
    bool scanComplete() const;

signals:
    void tagFound(const QString &tag);
    void videoOutputChanged();
    void captureRectChanged();
    void scanPercentChanged();
    void scanCompleteChanged();

private slots:
    void processFrame(const QVideoFrame &frame);
    void onDecoderTagFound(const QString &tag);  // relay from decoder (may arrive on worker thread)

private:
    QVideoSink*              m_videoSink;
    QPointer<QVideoSink>    m_connectedSink;       // QPointer: auto-nulls if VideoOutput destroyed first
    QObject*                m_videoOutput = nullptr;

    // Both decoder and processing flag are shared with QtConcurrent lambdas.
    // This prevents use-after-free when QBarcodeFilter is destroyed while a
    // worker is still running: the lambda holds its own shared-ptr copy, keeping
    // both objects alive until the task finishes.
    QSharedPointer<QBarcodeDecoder> m_decoder;
    QSharedPointer<QAtomicInt>      m_processing;

    QRectF m_captureRect;
    int    m_scanPercent  = 0;
    bool   m_scanComplete = false;
};

#endif // QBARCODEFILTER_H
