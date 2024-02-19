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
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

Popup {
    id: displayAddressBusybox
    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape
    background: Item{}
    property string addrToVerify: ""
    Rectangle {
        id: displayAddressMask
        width: 500
        height: 250
        radius: 8
        color: "#FFFFFF"
        anchors.centerIn: parent
        layer.enabled: true
        layer.effect: OpacityMask {
            maskSource: Rectangle {
                width: 500
                height: 250
                radius: 8
            }
        }
        QCloseButton {
            anchors {
                right: parent.right
                rightMargin: 16
                top: parent.top
                topMargin: 16
            }
            onClicked: {displayAddressBusybox.close()  ; displayAddressBusybox.addrToVerify = ""}
        }
        Column {
            spacing: 16
            anchors.centerIn: parent
            QBusyIndicator {
                width: 70
                height: 70
                anchors.horizontalCenter: parent.horizontalCenter
            }
            QText {
                anchors.horizontalCenter: parent.horizontalCenter
                font.family: "Lato"
                font.pixelSize: 14
                font.weight: Font.Bold
                text: STR.STR_QML_008
            }
            Rectangle {
                width: 450
                height: 60
                color: Qt.rgba(0, 0, 0, 0.1)
                anchors.horizontalCenter: parent.horizontalCenter
                QText {
                    anchors.fill: parent
                    anchors.leftMargin: 5
                    anchors.rightMargin: 5
                    anchors.topMargin: 5
                    anchors.bottomMargin: 5
                    wrapMode: Text.WrapAnywhere
                    horizontalAlignment: Text.AlignHCenter
                    text: displayAddressBusybox.addrToVerify
                    anchors.centerIn: parent
                    font.pixelSize: 18
                    color: "#323E4A"
                }
            }
        }
    }
    DropShadow {
        anchors.fill: displayAddressMask
        horizontalOffset: 3
        verticalOffset: 5
        spread: 0
        radius: 8
        samples: 30
        color: "#aa000000"
        source: displayAddressMask
    }
}
