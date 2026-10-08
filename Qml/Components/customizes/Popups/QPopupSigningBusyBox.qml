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
import QtQuick.Controls 2.3
import QtGraphicalEffects 1.12
import Qt.labs.platform 1.1
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import "../../origins"
import "../../customizes"
import "../../customizes/Texts"
import "../../customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

Popup {
    id: signingBusyBox
    property int signerType: -1
    modal: true
    focus: true
    background: Item{}
    Rectangle {
        id: signingBusyBoxMask
        width: 300
        height: 196
        radius: 24
        color: "#FFFFFF"
        anchors.centerIn: parent
        layer.enabled: true
        layer.effect: OpacityMask {
            maskSource: Rectangle {
                width: 300
                height: 196
                radius: 24
            }
        }
        Column {
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
                text: STR.STR_QML_297
            }
            QText {
                width: 252
                anchors.horizontalCenter: parent.horizontalCenter
                font.family: "Lato"
                font.pixelSize: 16
                text: signingBusyBox.signerType === NUNCHUCKTYPE.HARDWARE ? STR.STR_QML_298 : STR.STR_QML_122
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignHCenter
            }
        }
    }
    DropShadow {
        anchors.fill: signingBusyBoxMask
        horizontalOffset: 3
        verticalOffset: 5
        spread: 0
        radius: 8
        samples: 30
        color: "#aa000000"
        source: signingBusyBoxMask
    }
}
