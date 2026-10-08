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
import "../../../../localization/STR_QML.js" as STR

Rectangle {
    width: 504
    height: 36
    radius: 36
    property string contactname: "name"
    property string contactemail: "email"

    signal itemAcceptClicked()
    signal itemIgnoreClicked()

    Row {
        anchors.fill: parent
        spacing: 8
        QAvatar {
            id: avatar
            width: 36
            height: 36
            username: contactname
            displayStatus: false
        }
        QText{
            id: name
            width: 302 - email.width
            font.family: "Lato"
            font.pixelSize: 16
            color: "#000000"
            font.weight: Font.Bold
            elide: Text.ElideRight
            text: contactname
            anchors.verticalCenter: avatar.verticalCenter
        }
        QText{
            id: email
            font.family: "Lato"
            font.pixelSize: 12
            color: "#000000"
            font.weight: Font.Normal
            text: "(" + contactemail + ")"
            anchors.verticalCenter: avatar.verticalCenter
        }

        QText{
            id: ignore
            width: 50
            font.family: "Lato"
            font.pixelSize: 12
            color: "#000000"
            font.weight: ignoreMouse.containsMouse ?  Font.ExtraBold : Font.DemiBold
            text: STR.STR_QML_467
            horizontalAlignment: Text.AlignHCenter
            anchors.verticalCenter: avatar.verticalCenter
            font.underline: ignoreMouse.containsMouse
            MouseArea {
                id: ignoreMouse
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                onClicked: {itemIgnoreClicked()}
            }
        }

        QTextButton {
            width: 73
            height: 36
            label.text: STR.STR_QML_468
            label.font.pixelSize: 12
            type: eTypeB
            onButtonClicked: {itemAcceptClicked()}
        }
    }
}
