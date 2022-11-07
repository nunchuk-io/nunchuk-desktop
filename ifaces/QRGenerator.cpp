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
#include "QRGenerator.h"
#include <QDebug>
QRGenerator::QRGenerator(){}

QRGenerator::~QRGenerator(){}

QImage QRGenerator::qrImage(QString text, QSize _size, int _border){
    return generate(text, _size, _border);
}

QImage QRGenerator::generate(QString text, QSize _size, int _border)
{
    QImage img_ret;
    const int border = _border;
    const QrCode qr = QrCode::encodeText(text.toStdString().c_str(), QrCode::Ecc::LOW);
    int size = qr.getSize();
    QImage img(size, size, QImage::Format_RGB888);
    img.fill(Qt::white);
    if(size > 0){
        for (int _y = 0; _y < size; _y++) {
            for (int _x = 0; _x < size; _x++) {
                if(0 != qr.getModule(_x, _y)){
                    img.setPixelColor(_x, _y, QColor(Qt::black));
                }
            }
        }
    }
    img_ret = img.scaled(_size.width() - 2*border, _size.height() - 2*border, Qt::KeepAspectRatioByExpanding);
    QImage imgResult(_size.width(), _size.height(), QImage::Format_ARGB32);
    imgResult.fill(Qt::white);
    QPainter painter(&imgResult);
    QPoint destPos = QPoint(border, border);
    painter.drawImage(destPos, img_ret);
    painter.end();
    return imgResult;
}
