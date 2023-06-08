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
import QtQuick 2.4
import QtGraphicalEffects 1.0
import "../../customizes/Texts"

QBaseButton {
    id: idRootStyle1
    // type enum
    readonly property int eFILLED           : eTypeJ
    readonly property int eOUTLINE_NORMAL   : eTypeK
    readonly property int eOUTLINE_DARK     : eTypeL
    type: eFILLED

    property string label: "Value"
    property int fontPixelSize: 14
    property var fontWeight: Font.Bold
    property string fontFamily: "Lato"

    QBackgroundState {
        id:bg
        anchors.fill: parent
    }
    Row {
        anchors.centerIn: idRootStyle1
        spacing: 9
        Item {
            id: plus
            width: 16
            height: 16
            anchors.verticalCenter: parent.verticalCenter
            Rectangle {
                width: 10
                height: 2
                anchors.centerIn: parent
                color: text.color
            }
            Rectangle {
                width: 2
                height: 10
                color: text.color
                anchors.centerIn: parent
            }
        }
        QLatoState {
            id: text
            height: idRootStyle1.height
            anchors.verticalCenter: plus.verticalCenter
            verticalAlignment: Text.AlignVCenter
            font.weight: fontWeight
            font.pixelSize: fontPixelSize
            text: label
        }
    }

    Rectangle {
        id: clickbg
        anchors.fill: parent
        radius: bg.radius
        color: type == eOUTLINE_DARK ? Qt.rgba(255, 255, 255, 0.1) : Qt.rgba(0, 0, 0, 0.1)
        visible: btnMouse.pressed
    }
}
