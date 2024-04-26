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
#include "QBarcodeGenerator.h"
#include <QStandardPaths>
#include "MultiFormatWriter.h"
#include <QDateTime>
#include "QOutlog.h"
#include "BitMatrix.h"

QBarcodeGenerator::QBarcodeGenerator() : m_borderWitdh(0)
{
    connect(this, &QQuickPaintedItem::widthChanged, this, &QBarcodeGenerator::slotUpdate);
    connect(this, &QQuickPaintedItem::heightChanged, this, &QBarcodeGenerator::slotUpdate);
}

QBarcodeGenerator::~QBarcodeGenerator()
{

}

QString QBarcodeGenerator::textInput() const
{
    return m_textInput;
}

int QBarcodeGenerator::borderWitdh() const
{
    return m_borderWitdh;
}

void QBarcodeGenerator::paint(QPainter *painter)
{
    if(NULL != painter &&  !m_textInput.isEmpty() ){
        int qr_border = 0;//this->borderWitdh();
        // variable with min size 200
        int qr_width = fmax(200, this->width());
        int qr_height = fmax(200, this->height());
        // Check if size is not eaual
        if (qr_width != qr_height){
            qr_width = qr_height = fmin(qr_width, qr_height);
        }
        ZXing::MultiFormatWriter writer = ZXing::MultiFormatWriter(ZXing::BarcodeFormat::QRCode)
                                              .setEncoding(ZXing::CharacterSet::UTF8)
                                              .setMargin(qr_border)
                                              .setEccLevel(0);
        auto bitmap = ZXing::ToMatrix<uint8_t>(writer.encode(m_textInput.toStdString(), qr_width, qr_height ));
        QImage qrImage = QImage(bitmap.data(),
                                qr_width,
                                qr_height,
                                QImage::Format::Format_Grayscale8);
        // Check if size is different
        if (qrImage.width() != this->width() || qrImage.height() != this->height()){
            qrImage = qrImage.scaled(this->width(), this->height());
        }
        painter->drawImage(0, 0, qrImage);
    }
}

void QBarcodeGenerator::setTextInput(QString arg)
{
    if (m_textInput != arg){
        m_textInput = arg;
        update();
        emit textInputChanged(arg);
    }
}

void QBarcodeGenerator::setBorderWitdh(int arg)
{
    if (m_borderWitdh != arg) {
        m_borderWitdh = arg;
        update();
        emit borderWitdhChanged(arg);
    }
}

void QBarcodeGenerator::slotUpdate()
{
    update();
}
