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

#ifndef QRGENERATOR_H
#define QRGENERATOR_H

#include <QObject>
#include <QImage>
#include <QSize>
#include <QPainter>
#include "contrib/nayuki/QrCode.hpp"

using QRGeneratoren::QrCode;

class QRGenerator
{
public:
    QRGenerator();
    ~QRGenerator();
    QImage qrImage(QString text, QSize _size = QSize(115, 120), int _border=6);

private:
    QImage generate(QString text, QSize _size, int _border);
};

#endif // QRGENERATOR_H
