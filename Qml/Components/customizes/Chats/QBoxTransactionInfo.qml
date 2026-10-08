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
import NUNCHUCKTYPE 1.0
import "../../origins"
import "../../customizes"
import "../../customizes/Chats"
import "../../customizes/Texts"
import "../../customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

Item {
    id: lastTransactionInfo
    width: 180
    height: 90
    property QtObject pinTransaction: RoomWalletData.currentRoom ? RoomWalletData.currentRoom.pinTransaction : null
    property QtObject txObject: pinTransaction ? pinTransaction.info : null
    property int    tx_status: txObject ? txObject.status : -1
    property int    confirmations: txObject ? Math.max(0, (AppModel.blockHeight - txObject.height) + 1)  : 0
    property string tx_id: txObject ? txObject.txid : ""
    property string to_addr: txObject ? txObject.destinationList.reciever : ""
    property string tx_amount: txObject ? txObject.total + RoomWalletData.unitValue : ""
    visible: pinTransaction ? true : false
    Rectangle {
        id: bgLastTransaction
        width: parent.width - 4
        height: parent.height - 4
        anchors.centerIn: parent
        color: "#FFFFFF"
        visible: false
        radius: 8
    }
    DropShadow {
        anchors.fill: bgLastTransaction
        verticalOffset: 2
        cached: true
        radius: 8
        samples: 16
        color: Qt.rgba(0, 0, 0, 0.15)
        source: bgLastTransaction
        Row {
            anchors.fill: parent
            anchors.margins: 8
            spacing: 8
            QIcon {
                id: icoLastTransaction
                iconSize: 18
                source: "qrc:/Images/Images/PendingTransaction.png"
            }
            Column {
                spacing: 8
                Rectangle {
                    height: 24
                    width: tx_currentState.width + 20
                    radius: 20                       
                    color: GlobalData.transactionColor(lastTransactionInfo.tx_status)
                    QText {
                        id: tx_currentState
                        font.family: "Lato"
                        font.pixelSize: 10
                        text:  GlobalData.transactionStatus(lastTransactionInfo.tx_status, lastTransactionInfo.confirmations)
                        anchors.centerIn: parent
                        color: "#031F2B"
                        font.weight: Font.Bold
                    }
                }
                Row {
                    id: tx_configinfo
                    spacing: 0
                    layoutDirection: Qt.LeftToRight
                    QText {
                        id: txTitle
                        text: STR.STR_QML_491
                        font.family: "Lato"
                        font.pixelSize: 12
                        color: "#031F2B"
                    }
                    QText {
                        width: 124 - txTitle.paintedWidth
                        text: lastTransactionInfo.to_addr
                        font.family: "Lato"
                        font.pixelSize: 12
                        color: "#031F2B"
                        elide: Text.ElideMiddle
                        font.weight: Font.Bold
                    }
                }
                QText {
                    id: tx_sharedwlname
                    font.family: "Lato"
                    font.pixelSize: 12
                    text: lastTransactionInfo.tx_amount
                    color: "#031F2B"
                    font.weight: Font.Bold
                }
            }
        }
    }
    MouseArea {
        id: lastTransactionInfoMouse
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        enabled: lastTransactionInfo.visible
        onClicked: {
            QMLHandle.sendEvent(EVT.EVT_HOME_ONLINE_TRANSACTION_INFO_REQUEST, lastTransactionInfo.pinTransaction)
        }
    }
}
