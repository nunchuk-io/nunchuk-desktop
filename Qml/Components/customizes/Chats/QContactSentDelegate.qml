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
    property string contactemail: "email"
    property string contactname: "email"

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
        Item {
            width: 300
            height: 36
            QText{
                id: email
                font.family: "Lato"
                font.pixelSize: 16
                color: "#000000"
                font.weight: Font.Normal
                width: 230
                text: contactemail
                elide: Text.ElideRight
                anchors.verticalCenter: parent.verticalCenter
            }
            Rectangle {
                width: 58
                height: 16
                radius: 16
                color: "#D0E2FF"
                anchors.left: email.right
                anchors.leftMargin: 8
                anchors.verticalCenter: parent.verticalCenter
                QText{
                    font.family: "Lato"
                    font.pixelSize: 10
                    color: "#031F2B"
                    font.weight: Font.ExtraBold
                    text: STR.STR_QML_469
                    anchors.centerIn: parent
                }
            }
        }        
        QTextButton {
            width: 137
            height: 36
            label.text: STR.STR_QML_470
            label.font.pixelSize: 12
            label.color: "#FFFFFF"
            type: eTypeB
            onButtonClicked: { itemIgnoreClicked() }
        }
    }
}
