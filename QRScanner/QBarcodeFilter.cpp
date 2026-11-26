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
#include <QRegularExpression>
#include <QThread>
#include <QScopeGuard>
#include "QOutlog.h"
#include <string>
#include <vector>
#include "nunchuk.h"

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

    static const int kMaxConcurrentDecodes = qBound(1, QThread::idealThreadCount() / 2, 2);
    if (m_processing->fetchAndAddAcquire(1) >= kMaxConcurrentDecodes) {
        m_processing->fetchAndSubRelease(1);
        return;
    }

    QVideoFrame copy(frame);
    if (!copy.map(QVideoFrame::ReadOnly)) {
        m_processing->fetchAndSubRelease(1);
        return;
    }
    QImage image = copy.toImage();
    copy.unmap();

    if (image.isNull()) {
        m_processing->fetchAndSubRelease(1);
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
        auto release = qScopeGuard([processing]() { processing->fetchAndSubRelease(1); });
        try {
            decoder->process(image, ZXing::BarcodeFormat::QRCode);
        } catch (...) {
            DBG_INFO << "QR decode threw unknown exception";
        }
    });
}

// ── Decoder relay ─────────────────────────────────────────────────────────────
// Called on the main thread (QueuedConnection from worker thread).
namespace {
bool parseMultipart(const QString &tag, QString &key, int &index, int &total, bool &isPsbtUr)
{
    static const QRegularExpression urRe(QStringLiteral("^ur:([a-z0-9\\-]+)/(\\d+)-(\\d+)/"),
                                         QRegularExpression::CaseInsensitiveOption);
    static const QRegularExpression bbqrRe(QStringLiteral("^B\\$[0-9A-Z]{2}([0-9A-Z]{2})([0-9A-Z]{2})"),
                                           QRegularExpression::CaseInsensitiveOption);
    static const QRegularExpression urV1Re(QStringLiteral("^ur:([a-z0-9\\-]+)/(\\d+)of(\\d+)/"),
                                           QRegularExpression::CaseInsensitiveOption);
    isPsbtUr = false;
    auto m = urRe.match(tag);
    if (!m.hasMatch()) {
        m = urV1Re.match(tag);
    }
    if (m.hasMatch()) {
        index = m.captured(2).toInt();
        total = m.captured(3).toInt();
        isPsbtUr = m.captured(1).compare(QStringLiteral("crypto-psbt"), Qt::CaseInsensitive) == 0;
        key = QStringLiteral("ur:") + m.captured(1).toLower() + QString::number(total);
        return total > 1;
    }
    m = bbqrRe.match(tag);
    if (m.hasMatch()) {
        bool ok1 = false, ok2 = false;
        total = m.captured(1).toInt(&ok1, 36);
        index = m.captured(2).toInt(&ok2, 36);
        key = QStringLiteral("bbqr") + QString::number(total);
        return ok1 && ok2 && total > 1;
    }
    return false;
}
}

void QBarcodeFilter::onDecoderTagFound(const QString &tag)
{
    QString key;
    int index = 0;
    int total = 0;
    int percent = 100;
    bool complete = true;
    bool isPsbtUr = false;
    if (parseMultipart(tag, key, index, total, isPsbtUr)) {
        if (key != m_multipartKey) {
            m_multipartKey = key;
            m_multipartTotal = total;
            m_parts.clear();
            m_multipartTags.clear();
        }
        m_parts.insert(index);
        if (m_multipartTags.contains(tag)) {
            return;
        }
        m_multipartTags.append(tag);
        DBG_INFO << "QR part" << index << "/" << m_multipartTotal << "collected" << m_multipartTags.size();
        const int got = static_cast<int>(m_parts.size());
        complete = got >= m_multipartTotal;
        if (complete && isPsbtUr) {
            bool decoded = false;
            try {
                std::vector<std::string> in;
                for (const QString &t : m_multipartTags) {
                    in.push_back(t.toStdString());
                }
                decoded = !nunchuk::Utils::ParseKeystoneTransaction(in).empty();
            } catch (...) {
                decoded = false;
            }
            bool allPureParts = true;
            for (int i = 1; i <= m_multipartTotal; ++i) {
                if (!m_parts.contains(i)) {
                    allPureParts = false;
                    break;
                }
            }
            if (!decoded) {
                DBG_INFO << "QR psbt not decodable yet, pure parts complete:" << allPureParts << "tags:" << m_multipartTags.size();
            }
            complete = decoded || allPureParts || m_multipartTags.size() >= m_multipartTotal * 3;
        }
        percent = complete ? 100 : qBound(0, got * 100 / m_multipartTotal, 99);
    } else if (tag != m_lastSingleTag) {
        m_lastSingleTag = tag;
        DBG_INFO << "QR single tag, length:" << tag.size() << "prefix:" << tag.left(48);
    }
    if (m_scanPercent != percent) {
        m_scanPercent = percent;
        emit scanPercentChanged();
    }
    if (m_scanComplete != complete) {
        m_scanComplete = complete;
        emit scanCompleteChanged();
    }
    emit tagFound(tag);
}
