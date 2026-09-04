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
    if (m_isDecoding == isDecoding) {
        return;
    }

    m_isDecoding = isDecoding;

    emit isDecodingChanged(m_isDecoding);
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

    // Base options shared by all passes.
    // tryHarder + tryRotate cover most orientation/damage cases.
    const auto base = ReaderOptions()
        .setFormats(formats)
        .setTryHarder(true)
        .setTryRotate(true)
        .setIsPure(false);

    try {
        // Normalize resolution and pixel format once — reused across passes.
        const QImage work = prepareImage(capturedImage);

        // ── Pass 1: LocalAverage binarizer ──────────────────────────────────
        // Best for normal lighting and reasonable contrast. Fast path.
        auto result = ReadBarcode(work, ReaderOptions(base)
            .setBinarizer(Binarizer::LocalAverage));

        // ── Pass 2: GlobalHistogram binarizer ───────────────────────────────
        // Better for low-contrast QR codes, faded prints, glossy surfaces,
        // or scenes with very uneven ambient lighting.
        if (!result.isValid()) {
            result = ReadBarcode(work, ReaderOptions(base)
                .setBinarizer(Binarizer::GlobalHistogram));
        }

        // ── Pass 3: Grayscale + LocalAverage ────────────────────────────────
        // Strips colour information that can mislead binarization on images
        // with reflections, coloured backgrounds, or strong colour casts.
        if (!result.isValid()) {
            const QImage gray = work.convertToFormat(QImage::Format_Grayscale8);
            result = ReadBarcode(gray, ReaderOptions(base)
                .setBinarizer(Binarizer::LocalAverage));
        }

        if (result.isValid())
            emit tagFound(result.text());
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

// ── prepareImage ─────────────────────────────────────────────────────────────
// Normalises the image before handing it to ZXing:
//   1. Downscales to at most MAX_DECODE_DIM on the long edge.
//      ZXing complexity is O(area); a 4× downscale cuts time by 16×.
//      640 px is enough for QR version 40 (177×177 modules → ~3.6 px/module).
//   2. Converts to RGBX8888 if not already in a ZXing-native format, so
//      ZXing's internal conversion is a no-op.
QImage QBarcodeDecoder::prepareImage(const QImage &image) const
{
    if (image.isNull())
        return image;

    constexpr int MAX_DECODE_DIM = 640;

    QImage result = (image.width() > MAX_DECODE_DIM || image.height() > MAX_DECODE_DIM)
        ? image.scaled(MAX_DECODE_DIM, MAX_DECODE_DIM,
                       Qt::KeepAspectRatio, Qt::SmoothTransformation)
        : image;

    // Ensure pixel format is one ZXing handles natively (avoids internal copy).
    const auto fmt = result.format();
    if (fmt != QImage::Format_Grayscale8 &&
        fmt != QImage::Format_RGB888     &&
        fmt != QImage::Format_RGBX8888   &&
        fmt != QImage::Format_RGBA8888)
    {
        result = result.convertToFormat(QImage::Format_RGBX8888);
    }

    return result;
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
