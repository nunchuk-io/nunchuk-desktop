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
import "../customizes/Texts"
import "../customizes/Buttons"
import "../../../localization/STR_QML.js" as STR

Rectangle {
    property bool transactionisReceiveTx: true
    property string transactiondestinationList: ""
    property var transactiontxid: ""
    property int transactionstatus: 0
    property string transactionMemo: ""
    property string transactionAmount: "0"
    property string transactiontotalCurrency: "0"
    property string transactionDate: "--/--/---- ##:## ##"
    property bool transactionIsRbf: false
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
        color: "#EAEAEA"
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
                    text: transactiondestinationList
                    font.family: "Lato"
                    font.pixelSize: 16
                    color: "#031F2B"
                    elide: Text.ElideMiddle
                    font.weight: Font.DemiBold
                }
            }
        }
        Item{
            id: idstate
            width: statusWidth
            height: parent.height
            Column {
                anchors.left: idstate.left
                anchors.leftMargin: 16
                anchors.verticalCenter: parent.verticalCenter
                spacing: 4
                Rectangle {
                    id: indicator
                    width: 120
                    height: 24
                    radius: 20
                    color: GlobalData.transactionColor(transactionstatus)
                    QText {
                        font.pixelSize: 10
                        font.bold: true
                        font.family: "Lato"
                        color: "#031F2B"
                        anchors.centerIn: parent
                        text: GlobalData.transactionStatus(transactionstatus, confirmation)
                    }
                }
                Rectangle {
                    border.width: 1
                    border.color: "#DEDEDE"
                    width: 37
                    height: 16
                    radius: 20
                    visible: transactionIsRbf
                    QLato {
                        text: "RBF"
                        font.weight: Font.Normal
                        font.pixelSize: 12
                        anchors.centerIn: parent
                    }
                }
            }

            QImage {
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                source: "qrc:/Images/Images/Signed.png"
                visible: (transactionstatus === NUNCHUCKTYPE.PENDING_SIGNATURES)
            }
        }
        Item{
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
        Item{
            width: memoWidth
            height: parent.height
            Column{
                anchors.verticalCenter: parent.verticalCenter
                spacing: 4
                QText {
                    font.family: "Lato"
                    font.pixelSize: 14
                    color:  "#595959"
                    text: "Memo"
                    verticalAlignment: Text.AlignVCenter
                }
                QText {
                    id: memo
                    width: memoWidth
                    font.family: "Lato"
                    font.pixelSize: 14
                    color:  (transactionstatus === NUNCHUCKTYPE.REPLACED) || (transactionstatus === NUNCHUCKTYPE.NETWORK_REJECTED)  ? "#9CAEB8" : "#031F2B"
                    text: transactionMemo
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }
            }
        }
        Item{
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
