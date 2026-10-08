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
Column {
    width: 321
    spacing: 8
    property string contactname: "name"
    property string contactmail: "mail"
    signal itemIgnoreClicked()
    signal itemAcceptClicked()
    Item {
        width: parent.width - 20
        height: 56
        Row {
            anchors.fill: parent
            spacing: 8
            Item {
                width: 8
                height: parent.height
            }
            QAvatar {
                id: avatar
                width: 36
                height: 36
                username: contactname
                displayStatus: false
                anchors.verticalCenter: parent.verticalCenter
            }
            Column {
                anchors.verticalCenter: parent.verticalCenter
                spacing: 8
                QText{
                    width: 230
                    font.family: "Lato"
                    font.pixelSize: 16
                    color: "#FFFFFF"
                    font.weight: Font.Bold
                    elide: Text.ElideRight
                    text: contactname
                }
                QText{
                    width: 240
                    font.family: "Lato"
                    font.pixelSize: 12
                    color: "#FFFFFF"
                    elide: Text.ElideMiddle
                    text: "(" + contactmail + ")"
                }
            }
        }
    }
    Row {
        spacing: 8
        Item {
            width: avatar.width
            height: 36
        }
        QTextButton {
            width: 69
            height: 36
            label.text: STR.STR_QML_467
            label.font.pixelSize: 12
            label.color: "#FFFFFF"
            type: eTypeE
            onButtonClicked: { itemIgnoreClicked() }
        }
        QTextButton {
            width: 69
            height: 36
            label.text: STR.STR_QML_468
            label.font.pixelSize: 12
            type: eTypeC
            onButtonClicked: { itemAcceptClicked() }
        }
    }
}
