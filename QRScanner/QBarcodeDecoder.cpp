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
#include "QBarcodeDecoder.h"
#include "QOutlog.h"
#include "ResultPoint.h"
#include <QDebug>
#include <QImage>
#include <QtMultimedia/qvideoframe.h>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOffscreenSurface>
#include <iostream>
#include <ReadBarcode.h>
#include <exception>
#include <QScopeGuard>
#include <QDateTime>
#include <QPainter>
#include <QElapsedTimer>
#include <algorithm>

namespace ZXing {
namespace Qt {
using ZXing::ReaderOptions;
using ZXing::BarcodeFormat;
using ZXing::BarcodeFormats;
using ZXing::Binarizer;

template <typename T, typename _ = decltype(ToString(T()))>
QDebug operator << (QDebug dbg, const T& v)
{
    return dbg.noquote() << QString::fromStdString(ToString(v));
}

class Result : private ZXing::Result
{
public:
    explicit Result(ZXing::Result&& r) : ZXing::Result(std::move(r)){ }
    using ZXing::Result::format;
    using ZXing::Result::isValid;
    inline QString text() const {
        return QString::fromStdString(ZXing::Result::text());
    }
};

Result ReadBarcode(const QImage& img, const ReaderOptions& options = { })
{
    auto ImgFmtFromQImg = [](const QImage& img){
        switch (img.format()) {
        case QImage::Format_ARGB32:
        case QImage::Format_RGB32:
#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
            return ImageFormat::RGBA;
#else
            return ImageFormat::XRGB;
#endif
        case QImage::Format_RGB888:
            return ImageFormat::RGB;
        case QImage::Format_RGBX8888:
        case QImage::Format_RGBA8888:
            return ImageFormat::RGBA;
        case QImage::Format_Grayscale8:
            return ImageFormat::Lum;
        default: return ImageFormat::None;
        }
    };

    auto exec = [&](const QImage& img){
        return Result(ZXing::ReadBarcode({ img.bits(), img.width(), img.height(), ImgFmtFromQImg(img) }, options));
    };

    return ImgFmtFromQImg(img) == ImageFormat::None ? exec(img.convertToFormat(QImage::Format_RGBX8888)) : exec(img);
}
} // Qt namespace
} // ZXing namespace

using namespace ZXing::Qt;

std::ostream& operator << (std::ostream& os, const std::vector<ZXing::ResultPoint>& points)
{
    for (const auto& p : points) {
        os << int(p.x() + .5f) << "x" << int(p.y() + .5f) << " ";
    }

    return os;
}

QBarcodeDecoder::QBarcodeDecoder(QObject *parent) : QObject(parent)
{

}

void QBarcodeDecoder::setIsDecoding(bool isDecoding)
{
    if (m_isDecoding.exchange(isDecoding) == isDecoding) {
        return;
    }
    emit isDecodingChanged(isDecoding);
}

bool QBarcodeDecoder::  isDecoding() const
{
    return m_isDecoding;
}

void QBarcodeDecoder::process(const QImage& capturedImage, ZXing::BarcodeFormats formats)
{
    auto decodeGuard = qScopeGuard([=, this](){ setIsDecoding(false); });
    setIsDecoding(true);

    if (capturedImage.isNull())
        return;

    const auto base = ReaderOptions()
        .setFormats(formats)
        .setTryHarder(true)
        .setTryRotate(true)
        .setTryDownscale(true)
        .setIsPure(false);

    try {
        QElapsedTimer timer;
        timer.start();
        const QImage work = prepareImage(capturedImage);

        auto result = ReadBarcode(work, ReaderOptions(base)
            .setBinarizer(Binarizer::LocalAverage));
        int pass = 1;

        if (!result.isValid()) {
            result = ReadBarcode(work, ReaderOptions(base)
                .setBinarizer(Binarizer::GlobalHistogram)
                .setTryInvert(true));
            pass = 2;
        }

        const bool runHeavyPasses = !result.isValid() && (m_missCount.fetch_add(1) % HEAVY_PASS_INTERVAL == 0);

        if (runHeavyPasses) {
            const QImage stretched = stretchContrast(work);
            result = ReadBarcode(stretched, ReaderOptions(base)
                .setBinarizer(Binarizer::LocalAverage)
                .setTryInvert(true));
            pass = 3;
        }

        if (result.isValid()) {
            if (pass != 1) {
                DBG_INFO << "QR decoded pass" << pass << "size" << work.width() << "x" << work.height()
                         << "in" << timer.elapsed() << "ms";
            }
            emit tagFound(result.text());
        }
    }
    catch (std::exception& e) {
        emit errorOccured("ZXing exception: " + QString::fromLocal8Bit(e.what()));
    }
}

QImage QBarcodeDecoder::videoFrameToImage(const QVideoFrame &videoFrame, const QRect &captureRect) const
{
    if (!videoFrame.isValid()) {
        return QImage();
    }

    QVideoFrame frame(videoFrame);

    if (!frame.map(QVideoFrame::ReadOnly)) {
        return QImage();
    }

    QImage image = frame.toImage();

    frame.unmap();

    if (image.isNull()) {
        return QImage();
    }

    if (image.format() != QImage::Format_ARGB32) {
        image = image.convertToFormat(QImage::Format_ARGB32);
    }

    return image.copy(captureRect);
}

QImage QBarcodeDecoder::prepareImage(const QImage &image) const
{
    if (image.isNull())
        return image;

    QImage gray = image.format() == QImage::Format_Grayscale8 ? image : image.convertToFormat(QImage::Format_Grayscale8);
    if (gray.width() > MAX_DECODE_DIM || gray.height() > MAX_DECODE_DIM) {
        gray = gray.scaled(MAX_DECODE_DIM, MAX_DECODE_DIM, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }
    return gray;
}

QImage QBarcodeDecoder::stretchContrast(const QImage &gray) const
{
    if (gray.isNull() || gray.format() != QImage::Format_Grayscale8)
        return gray;

    int hist[256] = {0};
    const int w = gray.width();
    const int h = gray.height();
    for (int y = 0; y < h; ++y) {
        const uchar *line = gray.constScanLine(y);
        for (int x = 0; x < w; ++x)
            ++hist[line[x]];
    }

    const qint64 total = static_cast<qint64>(w) * h;
    const qint64 cut = total / 100;
    int lo = 0, hi = 255;
    qint64 acc = 0;
    for (int i = 0; i < 256; ++i) {
        acc += hist[i];
        if (acc > cut) { lo = i; break; }
    }
    acc = 0;
    for (int i = 255; i >= 0; --i) {
        acc += hist[i];
        if (acc > cut) { hi = i; break; }
    }
    if (hi - lo < 16)
        return gray;

    uchar lut[256];
    for (int i = 0; i < 256; ++i)
        lut[i] = static_cast<uchar>(qBound(0, (i - lo) * 255 / (hi - lo), 255));

    QImage out(gray.size(), QImage::Format_Grayscale8);
    for (int y = 0; y < h; ++y) {
        const uchar *src = gray.constScanLine(y);
        uchar *dst = out.scanLine(y);
        for (int x = 0; x < w; ++x)
            dst[x] = lut[src[x]];
    }
    return out;
}

// ── smoothTransformation ──────────────────────────────────────────────────────
// Legacy entry-point kept for source compatibility.
// Without QR_DFS_IMPROVE the old code scaled to the same size (no-op).
// Now delegates to prepareImage so callers get the same normalisation.
QImage QBarcodeDecoder::smoothTransformation(const QImage &image) const
{
    if (image.isNull())
        return QImage();

#ifdef QR_DFS_IMPROVE
    QImage result = deepFocusSharpen(image);
#ifdef QR_DFS_IMPROVE_TEST
    QString fileName = QString("/Users/barontong/Desktop/qra/%1.png")
        .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss-zzz"));
    result.save(fileName);
#endif
    return result;
#else
    return prepareImage(image);
#endif
}

#ifdef QR_DFS_IMPROVE
QImage QBarcodeDecoder::deepFocusSharpen(const QImage &inputImage) const {
    QImage origin = inputImage.scaled(inputImage.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QImage grayscaleImage = origin.convertToFormat(QImage::Format_Grayscale8);
    QImage blurredImage = applyGaussianBlur(grayscaleImage);
    QImage mask = createUnsharpMask(grayscaleImage, blurredImage);
    mask = adjustContrast(mask);
    QImage sharpenedImage = addMaskToOriginal(origin, mask);
    return sharpenedImage;
}

QImage QBarcodeDecoder::applyGaussianBlur(const QImage &image) const {
    // Convert QImage to grayscale for simplicity
    QImage grayscaleImage = image.convertToFormat(QImage::Format_Grayscale8);

    // Gaussian kernel for blurring
    float kernel[5][5] = {
        {0.0030, 0.0133, 0.0219, 0.0133, 0.0030},
        {0.0133, 0.0596, 0.0983, 0.0596, 0.0133},
        {0.0219, 0.0983, 0.1621, 0.0983, 0.0219},
        {0.0133, 0.0596, 0.0983, 0.0596, 0.0133},
        {0.0030, 0.0133, 0.0219, 0.0133, 0.0030}
    };

    // Apply Gaussian blur to the grayscale image
    QImage blurredImage(grayscaleImage.size(), QImage::Format_Grayscale8);
    for (int y = 2; y < grayscaleImage.height() - 2; ++y) {
        for (int x = 2; x < grayscaleImage.width() - 2; ++x) {
            float sum = 0.0;
            for (int j = -2; j <= 2; ++j) {
                for (int i = -2; i <= 2; ++i) {
                    sum += qRed(grayscaleImage.pixel(x + i, y + j)) * kernel[j + 2][i + 2];
                }
            }
            blurredImage.setPixel(x, y, qRgb(sum, sum, sum));
        }
    }

    return blurredImage;
}


QImage QBarcodeDecoder::createUnsharpMask(const QImage &original, const QImage &blurred) const {
    // Subtract blurred image from the original to create a mask (unsharp mask)
    QImage mask(original.size(), QImage::Format_Grayscale8);
    for (int y = 0; y < original.height(); ++y) {
        for (int x = 0; x < original.width(); ++x) {
            int newValue = qBound(0, qGray(original.pixel(x, y)) - qGray(blurred.pixel(x, y)), 255);
            mask.setPixel(x, y, qRgb(newValue, newValue, newValue));
        }
    }
    return mask;
}

QImage QBarcodeDecoder::adjustContrast(const QImage &image) const {
    // Adjust the contrast of the input image
    // Here, I'll provide a simple adjustment by multiplying pixel values
    QImage adjustedImage = image;
    for (int y = 0; y < adjustedImage.height(); ++y) {
        for (int x = 0; x < adjustedImage.width(); ++x) {
            int newValue = qBound(0, qGray(adjustedImage.pixel(x, y)) * 2, 255); // Adjust contrast by multiplying pixel values
            adjustedImage.setPixel(x, y, qRgb(newValue, newValue, newValue));
        }
    }
    return adjustedImage;
}

QImage QBarcodeDecoder::addMaskToOriginal(const QImage &original, const QImage &mask) const {
    // Add the adjusted mask to the original image to sharpen it
    QImage sharpenedImage = original;
    for (int y = 0; y < sharpenedImage.height(); ++y) {
        for (int x = 0; x < sharpenedImage.width(); ++x) {
            int newValue = qBound(0, qGray(original.pixel(x, y)) + qGray(mask.pixel(x, y)), 255);
            sharpenedImage.setPixel(x, y, qRgb(newValue, newValue, newValue));
        }
    }
    return sharpenedImage;
}
#endif
