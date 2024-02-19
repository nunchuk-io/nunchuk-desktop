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
#include "QRCodeItem.h"
#include "QZXing.h"

#if 0 // nayuki
QSharedPointer<QRGenerator> QRCodeItem::m_QRgen = QSharedPointer<QRGenerator>(NULL);
#endif

QRCodeItem::QRCodeItem() : m_borderWitdh(0)
{
#if 0 // nayuki
    if (m_QRgen.data() == NULL) {
        m_QRgen = QSharedPointer<QRGenerator>(new QRGenerator());
    }
#endif
    connect(this, &QQuickPaintedItem::widthChanged, this, &QRCodeItem::slotUpdate);
    connect(this, &QQuickPaintedItem::heightChanged, this, &QRCodeItem::slotUpdate);
}

QRCodeItem::~QRCodeItem()
{

}

QString QRCodeItem::textInput() const
{
    return m_textInput;
}

int QRCodeItem::borderWitdh() const
{
    return m_borderWitdh;
}

void QRCodeItem::paint(QPainter *painter)
{
    if(NULL != painter &&  !m_textInput.isEmpty() ){
        QImage qrImage = QZXing::encodeData(m_textInput, QZXing::EncoderFormat::EncoderFormat_QR_CODE, QSize(this->width(), this->height()));
#if 0 // nayuki
//        QImage qrImage = m_QRgen.data()->qrImage(m_textInput, QSize(this->width(), this->height()), m_borderWitdh);
#endif
        painter->drawImage(QRectF(0, 0, qrImage.width(), qrImage.height()), qrImage);
    }
}

void QRCodeItem::setTextInput(QString arg)
{
    if (m_textInput != arg){
        m_textInput = arg;
        QQuickPaintedItem::update();
        emit textInputChanged(arg);
    }
}

void QRCodeItem::setBorderWitdh(int arg)
{
    if ((m_borderWitdh != arg) && (arg > 0)) {
        m_borderWitdh = arg;
        QQuickPaintedItem::update();
        emit borderWitdhChanged(arg);
    }
}

void QRCodeItem::slotUpdate()
{
    QQuickPaintedItem::update();
}
