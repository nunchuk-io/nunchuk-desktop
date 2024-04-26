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
    // This will set the "isDecoding" to false automatically
    auto decodeGuard = qScopeGuard([=, this](){setIsDecoding(false);});
    setIsDecoding(true);
    const auto readerOptions = ReaderOptions()
                                   .setFormats(formats)
                                   .setTryHarder(true)
                                   .setTryRotate(true)
                                   .setIsPure(false)
                                   .setBinarizer(Binarizer::LocalAverage);

    try{
        QImage improvedImage = smoothTransformation(capturedImage);
        auto result = ReadBarcode(improvedImage, readerOptions);
        if (!result.isValid()) {
            result = ReadBarcode(improvedImage, readerOptions);
            if (!result.isValid()) {
                result = ReadBarcode(improvedImage, readerOptions);
            }
        }
        if(result.isValid()){
            emit tagFound(result.text());
        }
    }
    catch(std::exception& e) {
        emit errorOccured("ZXing exception: " + QString::fromLocal8Bit(e.what()));
    }
}

QImage QBarcodeDecoder::videoFrameToImage(const QVideoFrame &videoFrame, const QRect &captureRect) const
{
    auto handleType = videoFrame.handleType();
    if (handleType == QAbstractVideoBuffer::NoHandle) {
        QImage image = videoFrame.image();
        if (image.isNull()) {
            return QImage();
        }
        if (image.format() != QImage::Format_ARGB32) {
            image = image.convertToFormat(QImage::Format_ARGB32);
        }
        return image.copy(captureRect);
    }

    if (handleType == QAbstractVideoBuffer::GLTextureHandle) {
        QImage image(videoFrame.width(), videoFrame.height(), QImage::Format_ARGB32);
        GLuint textureId = static_cast<GLuint>(videoFrame.handle().toInt());
        QOpenGLContext *ctx = QOpenGLContext::currentContext();
        QOpenGLFunctions *f = ctx->functions();
        GLuint fbo;
        f->glGenFramebuffers(1, &fbo);
        GLint prevFbo;
        f->glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prevFbo);
        f->glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        f->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);
        f->glReadPixels(0, 0, videoFrame.width(), videoFrame.height(), GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
        f->glBindFramebuffer(GL_FRAMEBUFFER, static_cast<GLuint>( prevFbo ) );
        f->glDeleteFramebuffers(1,&fbo);
        return image.rgbSwapped().copy(captureRect);
    }
    return QImage();
}

QImage QBarcodeDecoder::smoothTransformation(const QImage &image) const
{
    // "Maximum image width/height is 65535" error will be thrown if the image is too large
    if (image.isNull()) {
        return QImage();
    }
#ifdef QR_DFS_IMPROVE
    QImage result = deepFocusSharpen(image);
#ifdef QR_DFS_IMPROVE_TEST
    QString fileName = QString("/Users/barontong/Desktop/qra/%1.png").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss-zzz"));
    result.save(fileName);
#endif

#else
    QImage result = image.scaled(image.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
#endif
    return result;
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
