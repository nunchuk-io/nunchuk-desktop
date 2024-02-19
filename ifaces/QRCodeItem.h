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

#ifndef QRCODEITEM_H
#define QRCODEITEM_H

#include <QtCore>
#include <QQuickPaintedItem>
#include <QRect>
#include <QBrush>
#include <QPainter>

#if 0 // nayuki
#include "QRGenerator.h"
#include <QSharedPointer>
#endif

class QRCodeItem : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QString  textInput       READ textInput      WRITE setTextInput      NOTIFY textInputChanged)
    Q_PROPERTY(int      borderWitdh     READ borderWitdh    WRITE setBorderWitdh    NOTIFY borderWitdhChanged)
public:
    QRCodeItem();
    ~QRCodeItem();
    QString textInput() const;
    int borderWitdh() const;
protected:
    void paint(QPainter *painter);
private:
#if 0 // nayuki
    static QSharedPointer<QRGenerator> m_QRgen;
#endif

    QString m_textInput;
    int m_borderWitdh;
signals:
    void textInputChanged(QString arg);
    void borderWitdhChanged(int arg);

public:
    void setTextInput(QString arg);
    void setBorderWitdh(int arg);
    void slotUpdate();
};
#endif // QRCODEITEM_H
