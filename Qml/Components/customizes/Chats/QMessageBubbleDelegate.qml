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
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.12
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import "../../origins"
import "../../customizes"
import "../../customizes/Chats"
import "../../customizes/Texts"
import "../../customizes/Buttons"

Item {
    id: bubleChatDelegate
    implicitHeight: rowContent.height

    property alias  layoutDirection : rowContent.layoutDirection
    property alias  isReceive       : infoText.isReceive
    property alias  messageContent  : infoText.messageContent
    property alias  sendername      : sendername.text

    Row {
        id: rowContent
        width: parent.width
        spacing: 12
        Rectangle {
            width: 36
            height: 36
            radius: 36
            border.color: "#DEDEDE"
            anchors.bottom: parent.bottom
            visible: isReceive
            QImage {
                width: 18
                height: 18
                anchors.centerIn: parent
                source: "qrc:/Images/Images/DefautAvatar.png"
            }
        }
        Column {
            spacing: 4
            QLato {
                id: sendername
                font.pixelSize: 12
                color: "#757575"
                visible: isReceive
            }
            QConversationText {
                id: infoText
                maxWidth: (rowContent.width - 48)*2/3
            }
            QLato {
                id: status
                font.pixelSize: 12
                color: "#757575"
                text: "Delivered"
                anchors.right: parent.right
                visible: !isReceive
            }
        }
    }
}
