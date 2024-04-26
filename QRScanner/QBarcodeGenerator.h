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
#ifndef QBARCODEGENERATOR_H
#define QBARCODEGENERATOR_H

#include <QQuickPaintedItem>
#include <QDir>
#include <QImage>
#include <qqml.h>
#include <QQuickItem>
#include <QObject>
#include <QPainter>

class QBarcodeGenerator : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QString  textInput       READ textInput      WRITE setTextInput      NOTIFY textInputChanged)
    Q_PROPERTY(int      borderWitdh     READ borderWitdh    WRITE setBorderWitdh    NOTIFY borderWitdhChanged)
public:
    QBarcodeGenerator();
    ~QBarcodeGenerator();
    QString textInput() const;
    int borderWitdh() const;
protected:
    void paint(QPainter *painter);
private:
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

#endif // QBARCODEGENERATOR_H
