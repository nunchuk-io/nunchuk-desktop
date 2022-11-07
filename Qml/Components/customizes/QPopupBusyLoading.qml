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
import QtQuick.Controls 1.4
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.12
import HMIEVENTS 1.0
import EWARNING 1.0
import QRCodeItem 1.0
import DataPool 1.0
import "../origins"
import "../customizes"

Popup {
    id: createBusy
    width: parent.width
    height: parent.height
    modal: true
    focus: true
    property string warningText1: ""
    property string warningText2: ""
    background: Item{}
    Rectangle {
        id: boxmask
        width: 300
        height: _column.implicitHeight + 24*2
        radius: 24
        color: "#FFFFFF"
        anchors.centerIn: parent
        layer.enabled: true
        layer.effect: OpacityMask {
            maskSource: Rectangle {
                width: boxmask.width
                height: boxmask.height
                radius: 24
            }
        }
        Column {
            id:_column
            spacing: 12
            anchors.centerIn: parent
            QBusyIndicator {
                width: 70
                height: 70
                anchors.horizontalCenter: parent.horizontalCenter
            }
            QText {
                anchors.horizontalCenter: parent.horizontalCenter
                font.family: "Lato"
                font.pixelSize: 16
                font.weight: Font.Bold
                text: warningText1
                visible: warningText1 !== ""
            }
            QText {
                width: 252
                anchors.horizontalCenter: parent.horizontalCenter
                font.family: "Lato"
                font.pixelSize: 16
                text: warningText2
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignHCenter
                visible: warningText2 !== ""
            }
        }
    }
    DropShadow {
        anchors.fill: boxmask
        horizontalOffset: 3
        verticalOffset: 5
        spread: 0
        radius: 8
        samples: 30
        color: "#aa000000"
        source: boxmask
    }
}
