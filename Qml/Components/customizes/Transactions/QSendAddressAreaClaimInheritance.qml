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
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Chats"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

Rectangle {
    width: 350
    height: 480
    radius: 12
    border.color: "#EAEAEA"
    color: "#FFFFFF"
    layer.enabled: true
    layer.effect: OpacityMask {
        maskSource: Rectangle {
            width: 350
            height: 480
            radius: 12
        }
    }
    property string myRole: ""
    property var transactionInfo
    signal addrToVerify(var addr)
    signal newMemoNotify(var newMemo)
    Flickable {
        anchors.fill: parent
        flickableDirection: Flickable.VerticalFlick
        clip: true
        interactive: contentHeight > height
        contentHeight: contentDisp.height
        ScrollBar.vertical: ScrollBar { active: true }
        Column {
            id: contentDisp
            width: parent.width
            spacing: 12
            anchors.horizontalCenter: parent.horizontalCenter
            QSendToAddressBlock {
                id: sendToAddress
                useMouseArea: true
                anchors.horizontalCenter: parent.horizontalCenter
                onAddressClicked: {
                    addrToVerify(addr)
                }
            }

            QTransactionNoteBlock {
                id: transationNote
                onMemoNotify: {
                    newMemoNotify(newMemo)
                }
            }

            QSignaturesBlock {
                id: signatures
                visible: (transactionInfo.status === NUNCHUCKTYPE.PENDING_SIGNATURES)
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }
    }
}
