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
import QtQuick 2.12
import QRCodeItem 1.0
import QtGraphicalEffects 1.0

Rectangle {
    property int iconSize: 48
    property string address: "address"
    property int offset: 8
    width: iconSize
    height: iconSize
    radius: offset
    color: "#F5F5F5"
    border.width: 1
    border.color: "#DEDEDE"
    QRCodeItem {
        id: qrCode
        anchors.centerIn: parent
        width: iconSize - 2*offset
        height: iconSize - 2*offset
        borderWitdh: 0
        textInput: address
    }
    layer.enabled: true
    layer.effect: DropShadow {
        horizontalOffset: 3
        verticalOffset: 3
        radius: 8.0
        samples: 17
        color: "#80000000"
    }
}
