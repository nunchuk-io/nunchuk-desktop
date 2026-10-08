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
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0
import HMIEVENTS 1.0
import EWARNING 1.0
import QRCodeItem 1.0
import "../../customizes"
import "../../origins"
import "../../customizes/Texts"
import "../../customizes/Buttons"

Rectangle {
    width: 321
    height: 56
    property bool isCurrentItem: false
    property string contactname: "name"
    property string contactAvt: ""
    color: isCurrentItem ? Qt.rgba(255, 255, 255, 0.3) : "transparent"
    Rectangle {
        width: 4
        height: parent.height
        anchors.left: parent.left
        visible: isCurrentItem
        color: "#FDD95C"
    }
    Item {
        anchors {
            fill: parent
            topMargin: 10
            bottomMargin: 10
            rightMargin: 16
            leftMargin: 16
        }
        Row {
            anchors.fill: parent
            spacing: 8
            QAvatar {
                id: avatar
                width: 36
                height: 36
                userCount: 1
                avatarUrl: contactAvt
                username: contactname
                displayStatus: false
                anchors.verticalCenter: parent.verticalCenter
            }
            QText{
                id: contacttext
                width: 240
                font.family: "Lato"
                font.pixelSize: 16
                color: "#FFFFFF"
                font.weight: Font.Bold
                elide: Text.ElideRight
                text: contactname
                anchors.verticalCenter: avatar.verticalCenter
            }
        }
    }

    signal itemLeftClicked()
    signal itemRightClicked()
    signal itemDoubleClicked()

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        onClicked: {
            if (mouse.button === Qt.RightButton){ itemRightClicked() }
            else{ itemLeftClicked() }
        }
        onDoubleClicked: {itemDoubleClicked() }
    }
}
