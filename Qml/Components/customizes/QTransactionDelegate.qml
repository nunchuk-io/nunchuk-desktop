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
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import "../origins"
import "../../../localization/STR_QML.js" as STR

Rectangle {
    property bool transactionisReceiveTx: true
    property var transactiondestinationList: ""
    property var transactiontxid: ""
    property int transactionstatus: 0
    property string transactionMemo: ""
    property string transactionAmount: "0"
    property string transactiontotalCurrency: "0"
    property string transactionDate: "--/--/---- ##:## ##"
    property int confirmation: 1

    property int timeWidth: 123
    property int statusWidth: 166
    property int memoWidth: 178
    property int amountWidth: 178
    property int addressWidth: 160

    color: btnMouse.containsMouse ?"#C9DEF1" : "transparent"

    Rectangle {
        width: parent.width
        height: 1
        color: "#C9DEF1"
        anchors.bottom: parent.bottom
    }

    Row {
        height: parent.height
        Item{
            width: addressWidth
            height: parent.height
            Column{
                anchors.verticalCenter: parent.verticalCenter
                spacing: 4
                QText {
                    width: addressWidth
                    text: transactionisReceiveTx ? STR.STR_QML_626_ : STR.STR_QML_626
                    font.family: "Lato"
                    font.pixelSize: 12
                    color: "#595959"
                    font.weight: Font.Normal
                }
                QText {
                    width: addressWidth
                    text: {
                        if(transactiondestinationList){
                            var address = transactiondestinationList.reciever
                            var textL = address.substring(0,6)
                            var textR = address.substring(address.length - 4,address.length)
                            return textL + "..." + textR
                        }
                        return ""
                    }
                    font.family: "Lato"
                    font.pixelSize: 16
                    color: "#031F2B"
                    elide: Text.ElideMiddle
                    font.weight: Font.DemiBold
                }
            }
        }

        Item {
            id: idstate
            width: statusWidth
            height: parent.height
            Rectangle {
                id: indicator
                width: 4
                height: 24
                anchors.left: idstate.left
                anchors.leftMargin: 16
                anchors.verticalCenter: parent.verticalCenter
                radius: 1
                color: {
                    if(transactionstatus === NUNCHUCKTYPE.PENDING_SIGNATURES){ return "#E02247" }
                    else if(transactionstatus === NUNCHUCKTYPE.READY_TO_BROADCAST){ return "#FF7A00" }
                    else if(transactionstatus === NUNCHUCKTYPE.NETWORK_REJECTED){ return "#031F2B" }
                    else if(transactionstatus === NUNCHUCKTYPE.PENDING_CONFIRMATION){ return "#FF7A00" }
                    else if(transactionstatus === NUNCHUCKTYPE.CONFIRMED){ return "#35ABEE" }
                    else { return "#031F2B" }
                }
            }

            QText {
                font.pixelSize: 12
                font.family: "Lato"
                color: "#323E4A"
                anchors.left: indicator.right
                anchors.leftMargin: 8
                anchors.verticalCenter: indicator.verticalCenter
                text: {
                    if(transactionstatus === NUNCHUCKTYPE.PENDING_SIGNATURES){ return STR.STR_QML_283 }
                    else if(transactionstatus === NUNCHUCKTYPE.READY_TO_BROADCAST){ return STR.STR_QML_284 }
                    else if(transactionstatus === NUNCHUCKTYPE.NETWORK_REJECTED){ return STR.STR_QML_285 }
                    else if(transactionstatus === NUNCHUCKTYPE.PENDING_CONFIRMATION){ return STR.STR_QML_286 }
                    else if(transactionstatus === NUNCHUCKTYPE.CONFIRMED){ return (qsTr("%1 %2").arg(confirmation).arg(STR.STR_QML_287)) }
                    else { return STR.STR_QML_456 }
                }
            }

            QImage {
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                source: "qrc:/Images/Images/Signed.png"
                visible: (transactionstatus === NUNCHUCKTYPE.PENDING_SIGNATURES)
            }
        }
        Item {
            id: blocktime
            width: timeWidth
            height: parent.height
            QText {
                anchors {
                    fill: parent
                    leftMargin: 16
                    rightMargin: 16
                    topMargin: 3
                    bottomMargin: 3
                }
                font.family: "Lato"
                font.pixelSize: 12
                color: (transactionstatus === NUNCHUCKTYPE.REPLACED) || (transactionstatus === NUNCHUCKTYPE.NETWORK_REJECTED) ? "#9CAEB8" : "#323E4A"
                text: transactionDate
                wrapMode: Text.WordWrap
                verticalAlignment: Text.AlignVCenter
            }
        }
        Item {
            width: memoWidth
            height: parent.height
            QText {
                id: memo
                anchors {
                    fill: parent
                    leftMargin: 16
                    rightMargin: 16
                    topMargin: 9
                    bottomMargin: 9
                }
                font.family: "Lato"
                font.pixelSize: 14
                color:  (transactionstatus === NUNCHUCKTYPE.REPLACED) || (transactionstatus === NUNCHUCKTYPE.NETWORK_REJECTED)  ? "#9CAEB8" : "#031F2B"
                text: transactionMemo
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }
        }

        Item {
            width: amountWidth
            height: parent.height
            Column{
                anchors.verticalCenter: parent.verticalCenter
                spacing: 4
                QText {
                    id: amountBTC
                    width: amountWidth
                    font.family: "Lato"
                    font.pixelSize: 14
                    color:  (transactionstatus === NUNCHUCKTYPE.REPLACED) || (transactionstatus === NUNCHUCKTYPE.NETWORK_REJECTED) ? "#9CAEB8" : "#031F2B"
                    text: {
                        var sign = (transactionisReceiveTx ? "" : "-")
                        return qsTr("%1 %2 %3").arg(sign).arg(transactionAmount).arg(RoomWalletData.unitValue)
                    }
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight
                    elide: Text.ElideRight
                }
                QText {
                    id: amountCurrency
                    width: amountWidth
                    font.family: "Lato"
                    font.pixelSize: 14
                    color:  (transactionstatus === NUNCHUCKTYPE.REPLACED) || (transactionstatus === NUNCHUCKTYPE.NETWORK_REJECTED) ? "#9CAEB8" : "#031F2B"
                    text: transactiontotalCurrency + " " + AppSetting.currency
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight
                    elide: Text.ElideRight
                }
            }
        }
    }

    signal buttonClicked()
    MouseArea {
        id: btnMouse
        hoverEnabled: true
        anchors.fill: parent
        onClicked: buttonClicked()
    }
}
