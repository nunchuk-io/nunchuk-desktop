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
import "../customizes/Popups"
import "../../../localization/STR_QML.js" as STR

Rectangle {
    id: rootDelegate
    property bool transactionisReceiveTx: true
    property bool walletIsByzantineGuardian: false
    property string transactiondestinationList: ""
    property string transactiontxid: ""
    property int transactionstatus: 0
    property string transactionMemo: ""
    property string transactionAmount: "0"
    property string transactiontotalCurrency: "0"
    property string transactionDate: "--/--/---- ##:## ##"
    property bool transactionIsRbf: false
    property int confirmation: 1
    property bool isFacilitatorAdmin: false
    property int timeWidth: 123
    property int statusWidth: 166
    property int memoWidth: 178
    property int amountWidth: 178
    property int addressWidth: 160
    property var parentList

    enabled: !isFacilitatorAdmin
    color: btnMouse.containsMouse ?"#C9DEF1" : "transparent"

    signal buttonClicked()

    Rectangle {
        width: parent.width
        height: 1
        color: "#EAEAEA"
        anchors.bottom: parent.bottom
    }
    MouseArea {
        id: btnMouse
        hoverEnabled: true
        anchors.fill: parent
        onClicked: buttonClicked()
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
                    text: isFacilitatorAdmin ? "••••••" : transactiondestinationList
                    font.family: "Lato"
                    font.pixelSize: 16
                    color: "#031F2B"
                    elide: Text.ElideMiddle
                    font.weight: Font.DemiBold
                }
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    buttonClicked()
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
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    buttonClicked()
                }
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
                font.pixelSize: 16
                color: (transactionstatus === NUNCHUCKTYPE.REPLACED) || (transactionstatus === NUNCHUCKTYPE.NETWORK_REJECTED) ? "#9CAEB8" : "#323E4A"
                text: transactionDate
                wrapMode: Text.WordWrap
                verticalAlignment: Text.AlignVCenter
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    buttonClicked()
                }
            }
        }
        Item{
            width: memoWidth
            height: parent.height
            MouseArea {
                id: mouseMemoMove
                anchors.fill: parent
                hoverEnabled: memo.text !== "" && memo.truncated
                enabled: memo.text !== "" && memo.truncated
                onEntered: {
                    calloutMemo.visible = true
                    var delegateHeight = rootDelegate.height
                    var mouseOnList     = mouseMemoMove.mapToItem(parentList, mouseX, mouseY)
                    var mouseOnDelegate = mouseMemoMove.mapToItem(rootDelegate, mouseX, mouseY)
                    var mousePositionY = mouseOnList.y
                    calloutMemo.setCalloutPosition(mouseOnDelegate.x, mouseOnDelegate.y, mousePositionY)
                }
                onPositionChanged: {
                    calloutMemo.visible = true
                    var delegateHeight = rootDelegate.height
                    var mouseOnList     = mouseMemoMove.mapToItem(parentList, mouseX, mouseY)
                    var mouseOnDelegate = mouseMemoMove.mapToItem(rootDelegate, mouseX, mouseY)
                    var mousePositionY = mouseOnList.y
                    calloutMemo.setCalloutPosition(mouseOnDelegate.x, mouseOnDelegate.y, mousePositionY)
                }
                onExited: {
                    calloutMemo.visible = false
                }
                onClicked : {
                    buttonClicked()
                }
            }
            Column{
                anchors.verticalCenter: parent.verticalCenter
                spacing: 4
                QText {
                    font.family: "Lato"
                    font.pixelSize: 12
                    color:  "#595959"
                    text: "Memo"
                    verticalAlignment: Text.AlignVCenter
                }
                QText {
                    id: memo
                    width: memoWidth
                    height: paintedHeight
                    font.family: "Lato"
                    font.pixelSize: 16
                    color:  (transactionstatus === NUNCHUCKTYPE.REPLACED) || (transactionstatus === NUNCHUCKTYPE.NETWORK_REJECTED)  ? "#9CAEB8" : "#031F2B"
                    text: memo.convertToClickableLinks(transactionMemo)
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                    onLinkActivated: {
                        Qt.openUrlExternally(link)
                    }
                    function convertToClickableLinks(text) {
                        // Regular expression to match URLs (simplified)
                        var urlRegex = /(https?:\/\/[^\s]+)/g;
                        // Replace URLs in the text with clickable <a> tags
                        return text.replace(urlRegex, "<a href='$1'>$1</a>");
                    }
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
                    font.pixelSize: 16
                    color:  (transactionstatus === NUNCHUCKTYPE.REPLACED)
                            || (transactionstatus === NUNCHUCKTYPE.NETWORK_REJECTED) ? "#9CAEB8"
                                                                                     : (transactionstatus === NUNCHUCKTYPE.CONFIRMED) ? "#1C652D" : "#031F2B"
                    text: {
                        if(isFacilitatorAdmin){
                            return "••••••"
                        }
                        else {
                            var sign = (transactionisReceiveTx ? "" : "-")
                            return qsTr("%1 %2 %3").arg(sign).arg(transactionAmount).arg(RoomWalletData.unitValue)
                        }
                    }
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight
                    elide: Text.ElideRight
                }
                QText {
                    id: amountCurrency
                    width: amountWidth
                    font.family: "Lato"
                    font.pixelSize: 12
                    color:  (transactionstatus === NUNCHUCKTYPE.REPLACED) || (transactionstatus === NUNCHUCKTYPE.NETWORK_REJECTED) ? "#9CAEB8" : "#031F2B"
                    text: isFacilitatorAdmin ? "••••••" : (transactiontotalCurrency + " " + AppSetting.currency)
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight
                    elide: Text.ElideRight
                    visible: !walletIsByzantineGuardian
                }
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    buttonClicked()
                }
            }
        }
    }
    QCalloutShape {
        id: calloutMemo
        width: 340
        visible: false
        contentText: transactionMemo
    }
}
