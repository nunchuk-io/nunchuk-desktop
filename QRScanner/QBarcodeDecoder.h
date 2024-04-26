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
#ifndef QBARCODEDECODER_H
#define QBARCODEDECODER_H

#include <QObject>
#include <QVideoFrame>

#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include "BarcodeFormat.h"

// Default camera resolution width/height
#define DEFAULT_RES_W 1080
#define DEFAULT_RES_H 1920

class QBarcodeDecoder : public QObject
{
    Q_OBJECT

public:
    explicit QBarcodeDecoder(QObject *parent = nullptr);
    void clean();
    bool isDecoding() const;
    QString captured() const;
    QImage videoFrameToImage(const QVideoFrame &videoFrame, const QRect &captureRect) const;
    QImage smoothTransformation(const QImage &image) const;

public slots:
    void process(const QImage& capturedImage, ZXing::BarcodeFormats formats);

private:
#ifdef QR_DFS_IMPROVE
    QImage deepFocusSharpen(const QImage &inputImage) const;
    QImage applyGaussianBlur(const QImage &image) const;
    QImage createUnsharpMask(const QImage &original, const QImage &blurred) const;
    QImage adjustContrast(const QImage &image) const;
    QImage addMaskToOriginal(const QImage &original, const QImage &mask) const;
#endif

signals:
    void isDecodingChanged(bool isDecoding);
    void errorOccured(const QString& errorString);
    void tagFound(const QString &tag);
private:
    bool m_isDecoding = false;
    void setIsDecoding(bool isDecoding);
};

#endif // QBARCODEDECODER_H
