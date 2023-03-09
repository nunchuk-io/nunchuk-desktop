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
import Qt.labs.platform 1.1
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import QRCodeItem 1.0
import DataPool 1.0
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Chats"
import "../../../localization/STR_QML.js" as STR

QScreen {
    readonly property int confirmations: Math.max(0, (AppModel.chainTip - AppModel.transactionInfo.height)+1)
    readonly property bool needShowCancel: {
        if((AppModel.transactionInfo.status === NUNCHUCKTYPE.PENDING_SIGNATURES) || (AppModel.transactionInfo.status === NUNCHUCKTYPE.READY_TO_BROADCAST)){
            return (AppModel.transactionInfo.roomId !== "")
        } else{ return false }
    }
    readonly property bool needShowRBF: !AppModel.walletInfo.isSharedWallet

    QOnScreenContent {
        id: contenCenter
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        label.text: STR.STR_QML_282
        label.font.pixelSize: 32
        label.font.weight: Font.Medium
        extraHeader: Rectangle {
            width: indicatorStatusTop.width + 20
            height: indicatorStatusTop.height + 10
            radius: 20
            color: {
                if(AppModel.transactionInfo.status === NUNCHUCKTYPE.PENDING_SIGNATURES){ return "#FFD7D9" }
                else if(AppModel.transactionInfo.status === NUNCHUCKTYPE.READY_TO_BROADCAST){ return "#FDEBD2" }
                else if(AppModel.transactionInfo.status === NUNCHUCKTYPE.PENDING_CONFIRMATION){ return "#E8DAFF" }
                else if(AppModel.transactionInfo.status === NUNCHUCKTYPE.CONFIRMED){ return "#D0E2FF" }
                else if(AppModel.transactionInfo.status === NUNCHUCKTYPE.NETWORK_REJECTED){ return "#CF4018" }
                else if(AppModel.transactionInfo.status === NUNCHUCKTYPE.REPLACED){ return "#EAEAEA" }
                else{ return "#FFD7D9" }
            }
            QText {
                id: indicatorStatusTop
                anchors.centerIn: parent
                font.pixelSize: 16
                font.weight: Font.Medium
                font.family: "Lato"
                color: "#031F2B"
                text:  {
                    if(AppModel.transactionInfo.status === NUNCHUCKTYPE.PENDING_SIGNATURES){ return STR.STR_QML_283 }
                    else if(AppModel.transactionInfo.status === NUNCHUCKTYPE.READY_TO_BROADCAST){ return STR.STR_QML_284 }
                    else if(AppModel.transactionInfo.status === NUNCHUCKTYPE.NETWORK_REJECTED){ return STR.STR_QML_285 }
                    else if(AppModel.transactionInfo.status === NUNCHUCKTYPE.PENDING_CONFIRMATION){ return STR.STR_QML_286 }
                    else if(AppModel.transactionInfo.status === NUNCHUCKTYPE.CONFIRMED){ return qsTr("%1 %2").arg(confirmations).arg(STR.STR_QML_287) }
                    else if(AppModel.transactionInfo.status === NUNCHUCKTYPE.REPLACED){ return STR.STR_QML_288 }
                    else{ return STR.STR_QML_082 }
                }
            }
        }
        onCloseClicked: {
            if(NUNCHUCKTYPE.CHAT_TAB === AppModel.tabIndex){
                QMLHandle.sendEvent(EVT.EVT_ONLINE_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_TRANSACTION_INFO)
            }
            else{
                if(QMLHandle.onsRequester() === EVT.STATE_ID_SCR_TRANSACTION_HISTORY){
                    QMLHandle.sendEvent(EVT.EVT_TRANSACTION_INFO_BACK_REQUEST)
                }
                else if(QMLHandle.onsRequester() === EVT.STATE_ID_SCR_UTXO_OUTPUT){
                    QMLHandle.sendEvent(EVT.EVT_TRANSACTION_INFO_BACK_UTXO_INFO)
                }
                else{
                    QMLHandle.sendEvent(EVT.EVT_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_TRANSACTION_INFO)
                }
            }
        }
        Loader {
            anchors.fill: parent
            sourceComponent: AppModel.transactionInfo.isReceiveTx ? txReceived : txSend
        }
    }
    Component {
        id: txSend
        Item {
            Row {
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: 100
                spacing: 30
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
                            Rectangle {
                                width: parent.width
                                height: 32
                                color: "#F5F5F5"
                                QText {
                                    text: STR.STR_QML_213
                                    font.family: "Lato"
                                    font.weight: Font.Bold
                                    font.pixelSize: 12
                                    color: "#323E4A"
                                    anchors {
                                        left: parent.left
                                        leftMargin: 16
                                        verticalCenter: parent.verticalCenter
                                    }
                                }
                                QText {
                                    text: STR.STR_QML_214
                                    font.family: "Lato"
                                    font.weight: Font.Bold
                                    font.pixelSize: 12
                                    color: "#323E4A"
                                    anchors {
                                        right: parent.right
                                        rightMargin: 16
                                        verticalCenter: parent.verticalCenter
                                    }
                                }
                            }
                            // Destination infomation
                            Repeater {
                                model: AppModel.transactionInfo.destinationList
                                width: parent.width
                                Row {
                                    width: parent.width - 24
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    spacing: 12
                                    QText {
                                        width: 192
                                        lineHeightMode: Text.FixedHeight
                                        lineHeight: 20
                                        wrapMode: Text.WrapAnywhere
                                        font.pixelSize: 16
                                        font.weight: Font.Bold
                                        color: "#031F2B"
                                        font.family: "Lato"
                                        text: destination_address
                                        MouseArea {
                                            id: btnMouseVerify
                                            hoverEnabled: true
                                            cursorShape: Qt.PointingHandCursor
                                            anchors.fill: parent
                                            onClicked: {
                                                displayAddressBusybox.addrToVerify = destination_address
                                                QMLHandle.sendEvent(EVT.EVT_TRANSACTION_VERIFY_ADDRESS, displayAddressBusybox.addrToVerify)
                                            }
                                        }
                                    }
                                    Column {
                                        width: 122
                                        spacing: 4
                                        QText {
                                            width: parent.width
                                            lineHeightMode: Text.FixedHeight
                                            lineHeight: 20
                                            wrapMode: Text.WrapAnywhere
                                            font.pixelSize: 16
                                            font.weight: Font.Bold
                                            color: "#031F2B"
                                            font.family: "Lato"
                                            text: destination_amount + RoomWalletData.unitValue
                                            horizontalAlignment: Text.AlignRight
                                        }
                                        QText {
                                            width: parent.width
                                            lineHeightMode: Text.FixedHeight
                                            lineHeight: 16
                                            wrapMode: Text.WrapAnywhere
                                            font.pixelSize: 12
                                            color: "#031F2B"
                                            font.family: "Lato"
                                            text: qsTr("$%1 USD").arg(model.destination_amount_usd)
                                            horizontalAlignment: Text.AlignRight
                                        }
                                    }
                                }
                            }
                            Rectangle { width: parent.width; height: 1; color: "#F5F5F5"}
                            Row {
                                visible: changeinfo.visible
                                width: parent.width - 24
                                anchors.horizontalCenter: parent.horizontalCenter
                                spacing: 12
                                QText {
                                    width: 192
                                    lineHeightMode: Text.FixedHeight
                                    lineHeight: 20
                                    wrapMode: Text.WrapAnywhere
                                    font.pixelSize: 16
                                    font.weight: Font.Bold
                                    color: "#031F2B"
                                    font.family: "Lato"
                                    text: STR.STR_QML_215
                                }
                                Column {
                                    width: 122
                                    spacing: 4
                                    QText {
                                        width: parent.width
                                        lineHeightMode: Text.FixedHeight
                                        lineHeight: 20
                                        wrapMode: Text.WrapAnywhere
                                        font.pixelSize: 16
                                        font.weight: Font.Bold
                                        color: "#031F2B"
                                        font.family: "Lato"
                                        text: AppModel.transactionInfo.fee + RoomWalletData.unitValue
                                        horizontalAlignment: Text.AlignRight
                                    }
                                    QText {
                                        width: parent.width
                                        lineHeightMode: Text.FixedHeight
                                        lineHeight: 16
                                        wrapMode: Text.WrapAnywhere
                                        font.pixelSize: 12
                                        color: "#031F2B"
                                        font.family: "Lato"
                                        text: qsTr("$%1 USD").arg(AppModel.transactionInfo.feeUSD)
                                        horizontalAlignment: Text.AlignRight
                                    }
                                }
                            }
                            Row {
                                visible: changeinfo.visible
                                width: parent.width - 24
                                anchors.horizontalCenter: parent.horizontalCenter
                                spacing: 12
                                QText {
                                    width: 192
                                    lineHeightMode: Text.FixedHeight
                                    lineHeight: 20
                                    wrapMode: Text.WrapAnywhere
                                    font.pixelSize: 16
                                    font.weight: Font.Bold
                                    color: "#031F2B"
                                    font.family: "Lato"
                                    text: STR.STR_QML_216
                                }
                                Column {
                                    width: 122
                                    spacing: 4
                                    QText {
                                        width: parent.width
                                        lineHeightMode: Text.FixedHeight
                                        lineHeight: 20
                                        wrapMode: Text.WrapAnywhere
                                        font.pixelSize: 16
                                        font.weight: Font.Bold
                                        color: "#031F2B"
                                        font.family: "Lato"
                                        text: AppModel.transactionInfo.total + RoomWalletData.unitValue
                                        horizontalAlignment: Text.AlignRight
                                    }
                                    QText {
                                        width: parent.width
                                        lineHeightMode: Text.FixedHeight
                                        lineHeight: 16
                                        wrapMode: Text.WrapAnywhere
                                        font.pixelSize: 12
                                        color: "#031F2B"
                                        font.family: "Lato"
                                        text: qsTr("$%1 USD").arg(AppModel.transactionInfo.totalUSD)
                                        horizontalAlignment: Text.AlignRight
                                    }
                                }
                            }
                            Rectangle {
                                id: changeinfo
                                width: parent.width
                                visible: AppModel.transactionInfo.hasChange
                                height: changeinfo.visible ? 32 : 0
                                color: "#F5F5F5"
                                QText {
                                    text: STR.STR_QML_217
                                    font.family: "Lato"
                                    font.weight: Font.Bold
                                    font.pixelSize: 12
                                    color: "#323E4A"
                                    anchors {
                                        left: parent.left
                                        leftMargin: 16
                                        verticalCenter: parent.verticalCenter
                                    }
                                }
                                QText {
                                    text: STR.STR_QML_214
                                    font.family: "Lato"
                                    font.weight: Font.Bold
                                    font.pixelSize: 12
                                    color: "#323E4A"
                                    anchors {
                                        right: parent.right
                                        rightMargin: 16
                                        verticalCenter: parent.verticalCenter
                                    }
                                }
                            }
                            Row {
                                visible: changeinfo.visible
                                width: parent.width - 24
                                anchors.horizontalCenter: parent.horizontalCenter
                                spacing: 12
                                QText {
                                    width: 192
                                    lineHeightMode: Text.FixedHeight
                                    lineHeight: 20
                                    wrapMode: Text.WrapAnywhere
                                    font.pixelSize: 16
                                    font.weight: Font.Bold
                                    color: "#031F2B"
                                    font.family: "Lato"
                                    text: AppModel.transactionInfo.change.address
                                }
                                Column {
                                    width: 122
                                    spacing: 4
                                    QText {
                                        width: parent.width
                                        lineHeightMode: Text.FixedHeight
                                        lineHeight: 20
                                        wrapMode: Text.WrapAnywhere
                                        font.pixelSize: 16
                                        font.weight: Font.Bold
                                        color: "#031F2B"
                                        font.family: "Lato"
                                        text: AppModel.transactionInfo.change.amount + RoomWalletData.unitValue
                                        horizontalAlignment: Text.AlignRight
                                    }
                                    QText {
                                        width: parent.width
                                        lineHeightMode: Text.FixedHeight
                                        lineHeight: 16
                                        wrapMode: Text.WrapAnywhere
                                        font.pixelSize: 12
                                        color: "#031F2B"
                                        font.family: "Lato"
                                        text: qsTr("$%1 USD").arg(AppModel.transactionInfo.change.amountUSD)
                                        horizontalAlignment: Text.AlignRight
                                    }
                                }
                            }
                            Rectangle {
                                width: parent.width
                                visible: AppModel.transactionInfo.hasChange
                                height: 32
                                color: "#F5F5F5"
                                QText {
                                    text: STR.STR_QML_218
                                    font.family: "Lato"
                                    font.weight: Font.Bold
                                    font.pixelSize: 12
                                    color: "#323E4A"
                                    anchors {
                                        left: parent.left
                                        leftMargin: 16
                                        verticalCenter: parent.verticalCenter
                                    }
                                }
                            }
                            QTextInputBoxTypeA {
                                width: parent.width - 24
                                placeholderText: "--"
                                text: AppModel.transactionInfo.memo
                                backgroundColor: "Transparent"
                                borderColor: "Transparent"
                                color: "#000000"
                                font.family: "Montserrat"
                                font.pixelSize: 16
                                anchors.horizontalCenter: parent.horizontalCenter
                                onTypingFinished: {
                                    var newMemo = currentText
                                    if(newMemo === ""){newMemo = "--"}
                                    if(newMemo !== AppModel.transactionInfo.memo){
                                        QMLHandle.sendEvent(EVT.EVT_TRANSACTION_SET_MEMO_REQUEST, newMemo)
                                    }
                                }
                            }
                            Rectangle {
                                width: parent.width
                                visible: AppModel.transactionInfo.hasChange
                                height: 32
                                color: "#F5F5F5"
                                QText {
                                    text: STR.STR_QML_219
                                    font.family: "Lato"
                                    font.weight: Font.Bold
                                    font.pixelSize: 12
                                    color: "#323E4A"
                                    anchors {
                                        left: parent.left
                                        leftMargin: 16
                                        verticalCenter: parent.verticalCenter
                                    }
                                }
                            }
                            Item {
                                width: parent.width
                                height: 32
                                QText {
                                    id: numbersignaturepending
                                    text: AppModel.transactionInfo.numberSigned + "/" + AppModel.transactionInfo.m
                                    font.pixelSize: 16
                                    color: "#031F2B"
                                    font.family: "Lato"
                                    font.weight: Font.Bold
                                    anchors {
                                        left: parent.left
                                        leftMargin: 16
                                        verticalCenter: parent.verticalCenter
                                    }
                                    width: 346
                                    height: 32
                                    wrapMode: Text.WordWrap
                                }
                                Row {
                                    spacing: 8
                                    height: 32
                                    visible: (AppModel.transactionInfo.status === NUNCHUCKTYPE.PENDING_SIGNATURES)
                                    anchors {
                                        left: parent.left
                                        leftMargin: 70
                                        verticalCenter: numbersignaturepending.verticalCenter
                                    }
                                    Rectangle {
                                        id: indicatorStatus
                                        width: 4
                                        height: 24
                                        radius: 1
                                        color: "#E02247"
                                        anchors.verticalCenter: parent.verticalCenter
                                    }
                                    QText {
                                        text: STR.STR_QML_220.arg(Math.max(0, (AppModel.transactionInfo.m - AppModel.transactionInfo.numberSigned)))
                                        font.pixelSize: 12
                                        font.family: "Lato"
                                        color: indicatorStatus.color
                                        width: 346
                                        height: 32
                                        anchors.verticalCenter: parent.verticalCenter
                                    }
                                }
                            }
                        }
                    }
                }
                Item {
                    width: 350
                    height: 480
                    Column {
                        width: parent.width
                        spacing: 8
                        Row {
                            spacing: 8
                            height: 20
                            QText {
                                color: "#031F2B"
                                font.family: "Lato"
                                font.pixelSize: 16
                                font.weight: Font.Bold
                                text: STR.STR_QML_289
                                anchors.verticalCenter: parent.verticalCenter
                            }
                            QImage {
                                visible: (AppModel.transactionInfo.status === NUNCHUCKTYPE.PENDING_SIGNATURES)
                                width: 16
                                height: 16
                                source: "qrc:/Images/Images/OnlineMode/pending_actions-24px.png"
                                anchors.verticalCenter: parent.verticalCenter
                            }
                            QText {
                                visible: (AppModel.transactionInfo.status === NUNCHUCKTYPE.PENDING_SIGNATURES)
                                color: "#595959"
                                font.family: "Lato"
                                font.pixelSize: 12
                                text: STR.STR_QML_220.arg(Math.max(0, (AppModel.transactionInfo.m - AppModel.transactionInfo.numberSigned)))
                                anchors.verticalCenter: parent.verticalCenter
                            }
                        }

                        Rectangle {
                            width: 350
                            height: 451
                            radius: 12
                            border.color: "#EAEAEA"
                            color: "#FFFFFF"
                            layer.enabled: true
                            layer.effect: OpacityMask {
                                maskSource: Rectangle {
                                    width: 350
                                    height: 451
                                    radius: 12
                                }
                            }
                            Loader {
                                anchors.fill: parent
                                anchors.margins: 12
                                sourceComponent: transactionAssignedSigners
                            }
                        }
                    }
                }
            }
            Loader {
                id: buttons
                anchors.right: parent.right
                anchors.rightMargin: 36
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 36
                sourceComponent: {
                    if(AppModel.transactionInfo.status === NUNCHUCKTYPE.PENDING_SIGNATURES){ return btnPendingSignatures }
                    else if(AppModel.transactionInfo.status === NUNCHUCKTYPE.READY_TO_BROADCAST){ return btnReadyToBroadcast }
                    else if(AppModel.transactionInfo.status === NUNCHUCKTYPE.PENDING_CONFIRMATION){ return btnPendingConfirmation }
                    else if(AppModel.transactionInfo.status === NUNCHUCKTYPE.CONFIRMED){ return btnConfirmed }
                    else { return btnReplacedRejected }
                }
            }
            Loader {
                id: bottomLeftButtons
                anchors.left: parent.left
                anchors.leftMargin: 36
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 36
                visible: needShowCancel
                sourceComponent:  needShowCancel ?  cancelTransactionBtn : null
            }
        }
    }
    Component {
        id: txReceived
        Item {
            Rectangle {
                id: contentarea
                width: 728
                height: 480
                anchors.top: parent.top
                anchors.topMargin: 84
                anchors.horizontalCenter: parent.horizontalCenter
                radius: 12
                color: "#FFFFFF"
                border.color: "#EAEAEA"
                layer.enabled: true
                layer.effect: OpacityMask {
                    maskSource: Rectangle {
                        width: contentarea.width
                        height: contentarea.height
                        radius: 12
                    }
                }
                Flickable {
                    id: flickcontent
                    anchors.fill: parent
                    flickableDirection: Flickable.VerticalFlick
                    clip: true
                    interactive: contentHeight > flickcontent.height
                    contentHeight: contentDisp.height
                    ScrollBar.vertical: ScrollBar { active: true }
                    Column {
                        id: contentDisp
                        width: parent.width
                        anchors.horizontalCenter: parent.horizontalCenter
                        spacing: 5
                        Rectangle {
                            width: parent.width - 2
                            height: 32
                            anchors.horizontalCenter: parent.horizontalCenter
                            color: "#F5F5F5"
                            QText {
                                text: STR.STR_QML_290
                                font.family: "Lato"
                                font.weight: Font.Bold
                                font.pixelSize: 12
                                color: "#323E4A"
                                anchors {
                                    left: parent.left
                                    leftMargin: 20
                                    verticalCenter: parent.verticalCenter
                                }
                            }
                            QText {
                                text: STR.STR_QML_214
                                font.family: "Lato"
                                font.weight: Font.Bold
                                font.pixelSize: 12
                                color: "#323E4A"
                                anchors {
                                    right: parent.right
                                    rightMargin: 20
                                    verticalCenter: parent.verticalCenter
                                }
                            }
                        }
                        // Destination infomation
                        Repeater {
                            model: AppModel.transactionInfo.destinationList
                            width: parent.width
                            Item {
                                width: parent.width
                                height: 40
                                QText {
                                    width: parent.width*2/3 - 20
                                    text: destination_address
                                    wrapMode: Text.WrapAnywhere
                                    font.pixelSize: 12
                                    color: "#031F2B"
                                    font.family: "Lato"
                                    anchors {
                                        left: parent.left
                                        leftMargin: 20
                                        verticalCenter: parent.verticalCenter
                                    }
                                }
                                Column {
                                    anchors {
                                        right: parent.right
                                        rightMargin: 20
                                        verticalCenter: parent.verticalCenter
                                    }
                                    QText {
                                        width: parent.width*1/3 - 20
                                        height: 16
                                        text: destination_amount + RoomWalletData.unitValue
                                        horizontalAlignment: Text.AlignRight
                                        font.pixelSize: 16
                                        font.weight: Font.Bold
                                        color: "#031F2B"
                                        font.family: "Lato"
                                    }
                                    QText {
                                        width: parent.width*1/3 - 20
                                        height: 16
                                        text: qsTr("$%1 USD").arg(model.destination_amount_usd)
                                        horizontalAlignment: Text.AlignRight
                                        font.pixelSize: 12
                                        color: "#031F2B"
                                        font.family: "Lato"
                                    }
                                }
                            }
                        }
                        Rectangle {
                            width: parent.width
                            height: 1
                            anchors.right: parent.right
                            color: "#C9DEF1"
                        }
                        Item {
                            width: parent.width
                            height: memodisplay.height
                            clip: true
                            QTextInputBox {
                                id: memodisplay
                                textOutput: AppModel.transactionInfo.memo
                                placeholder.text: STR.STR_QML_218
                                color: "transparent"
                                width: parent.width
                                heightMin: 64
                                botomLineVisible: false
                                mode: ePREVIEW_MODE
                                onTypingFinished: {
                                    if(currentText !== AppModel.transactionInfo.memo && currentText !== ""){
                                        QMLHandle.sendEvent(EVT.EVT_TRANSACTION_SET_MEMO_REQUEST, currentText)
                                    }
                                }
                            }
                        }
                    }
                }
            }
            Row {
                width: 257
                height: 24
                spacing: 8
                anchors.top: contentarea.bottom
                anchors.topMargin: 14
                anchors.right: contentarea.right
                visible: (AppModel.transactionInfo.status !== NUNCHUCKTYPE.REPLACED) && (AppModel.transactionInfo.status !== NUNCHUCKTYPE.NETWORK_REJECTED)
                QImage {
                    source: "qrc:/Images/Images/link.png"
                    anchors.verticalCenter: parent.verticalCenter
                }
                QText {
                    id: tit
                    height: 42
                    anchors.verticalCenter: parent.verticalCenter
                    text: STR.STR_QML_291
                    font.capitalization :  Font.AllUppercase
                    font.family: "Montserrat"
                    font.pixelSize: 12
                    font.weight: Font.DemiBold
                    color: mouseExplorerBlockchain3.containsMouse ? "#35ABEE":"#031F2B"
                    wrapMode: Text.WordWrap
                    MouseArea {
                        id: mouseExplorerBlockchain3
                        hoverEnabled: true
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: {
                            Qt.openUrlExternally(urlActiveLink())
                        }
                    }
                }
            }
            QTextButton {
                id: removebtn
                width: 200
                height: 48
                anchors {
                    bottom: parent.bottom
                    bottomMargin: 40
                    right: parent.right
                    rightMargin: 40
                }
                visible: needShowCancel ? false : (AppModel.transactionInfo.status === NUNCHUCKTYPE.REPLACED) || (AppModel.transactionInfo.status === NUNCHUCKTYPE.NETWORK_REJECTED)
                enabled: visible
                label.text: STR.STR_QML_292
                label.font.pixelSize: 16
                type: eTypeB
                onButtonClicked: {
                    QMLHandle.sendEvent(EVT.EVT_TRANSACTION_REMOVE_REQUEST)
                }
            }
        }
    }
    Component {
        id: transactionAssignedSigners
        QListView {
            id: signerlist
            clip: true
            spacing: 16
            ScrollBar.vertical: ScrollBar { active: true }
            model: AppModel.transactionInfo.singleSignersAssigned
            delegate: QTransactionSignerDelegate {
                width: signerlist.width
                alreadySigned: model.single_signer_signed_status
                signername: model.singleSigner_name
                signerxfp: model.singleSigner_masterFingerPrint
                signerType: model.single_signer_type
                signerReadyToSign: model.single_signer_readyToSign
                isLocaluser: model.single_signer_is_local
                devicetype: model.single_signer_devicetype
                card_id: model.single_signer_device_cardid
                serverkeyMessage: AppModel.transactionInfo.serverKeyMessage
                onSignRequest: {
                    if(model.single_signer_type === NUNCHUCKTYPE.SOFTWARE){
                        _confirm.open()
                        _confirm.signerType = model.single_signer_type
                        _confirm.fingerPrint = model.singleSigner_masterFingerPrint
                    }
                    else{
                        signingBusyBox.signerType = model.single_signer_type
                        signingBusyBox.open()
                        timerSigningTx.fingerPrint = model.singleSigner_masterFingerPrint
                        timerSigningTx.restart()
                    }
                }
                onScanRequest: {
                    QMLHandle.sendEvent(EVT.EVT_TRANSACTION_SCAN_DEVICE_REQUEST)
                }
            }
            section
            {
                property: "single_signer_is_local"
                criteria: ViewSection.FullString
                delegate: Item {
                    id: sectionItem
                    property bool isLocalUser: section === "true"
                    width: signerlist.width
                    height: isLocalUser ? 0 : 16
                    Rectangle {
                        anchors.centerIn: parent
                        width: parent.width
                        height: 1
                        visible: !sectionItem.isLocalUser
                        color: "#EAEAEA"
                    }
                }
            }
        }
    }
    // Buttons
    Component {
        id: btnPendingSignatures
        Row {
            height: 48
            spacing: 16
            QTextButton {
                id: removebtn
                width: 200
                height: 48
                enabled: visible
                label.text: STR.STR_QML_292
                label.font.pixelSize: 16
                type: eTypeB
                visible: !needShowCancel
                onButtonClicked: {
                    QMLHandle.sendEvent(EVT.EVT_TRANSACTION_REMOVE_REQUEST)
                }
            }
            QButtonLargeTail {
                id: advancedBtn
                width: 220
                height: 48
                type: eSECONDARY
                label: STR.STR_QML_299
                optionVisible: imExContextMenu.visible
                onButtonClicked: {
                    imExContextMenu.x = 20
                    imExContextMenu.y = 20 - imExContextMenu.height
                    imExContextMenu.open()
                }
                QContextMenu {
                    id: imExContextMenu
                    menuWidth: 320
                    labels: [
                        STR.STR_QML_300,
                        STR.STR_QML_115,
                        STR.STR_QML_116,
                        STR.STR_QML_301,
                        STR.STR_QML_302,
                        STR.STR_QML_303
                    ]
                    icons: [
                        "qrc:/Images/Images/download_031F2B.png",
                        "qrc:/Images/Images/OnlineMode/QRCodeScan.png",
                        "qrc:/Images/Images/OnlineMode/QRCodeScan.png",
                        "qrc:/Images/Images/import_031F2B.png",
                        "qrc:/Images/Images/OnlineMode/QRCodeScan.png",
                        "qrc:/Images/Images/OnlineMode/QRCodeScan.png"
                    ]
                    onItemClicked: {
                        switch(index){
                        case 0: // Export via file [.psbt]
                            savefileDialog.currentFile = StandardPaths.writableLocation(StandardPaths.DocumentsLocation) + "/"
                                    + RoomWalletData.getValidFilename(AppModel.transactionInfo.txid)
                                    + ".psbt"
                            savefileDialog.open()
                            break;
                        case 1: // Export via QR Keystone
                            requestExportQRKeyStone()
                            break;
                        case 2: "Export via QR Passport"
                            requestExportQRPassport()
                            break;
                        case 3: // Import via file [.psbt]
                            openfileDialog.open()
                            break;
                        case 4: // Import via QR Keystone
                            requestImportQRKeyStone()
                            break;
                        case 5: // Import via QR Passport
                            requestImportQRPassport()
                            break;
                        default:
                            break;
                        }
                    }
                }
            }
        }
    }
    Component {
        id: btnReadyToBroadcast
        Row {
            height: 48
            spacing: 16
            QTextButton {
                id: removebtn
                width: 200
                height: 48
                enabled: visible
                label.text: STR.STR_QML_292
                visible: !needShowCancel
                label.font.pixelSize: 16
                type: eTypeB
                onButtonClicked: {
                    QMLHandle.sendEvent(EVT.EVT_TRANSACTION_REMOVE_REQUEST)
                }
            }
            QButtonLargeTail {
                id: advancedBtn
                width: 220
                height: 48
                type: eSECONDARY
                label: STR.STR_QML_294
                optionVisible: imExContextMenu.visible
                onButtonClicked: {
                    imExContextMenu.x = 20
                    imExContextMenu.y = 20 - imExContextMenu.height
                    imExContextMenu.open()
                }
                QContextMenu {
                    id: imExContextMenu
                    menuWidth: 320
                    labels: [
                        STR.STR_QML_300,
                        STR.STR_QML_115,
                        STR.STR_QML_116,
                    ]
                    icons: [
                        "qrc:/Images/Images/download_031F2B.png",
                        "qrc:/Images/Images/OnlineMode/QRCodeScan.png",
                        "qrc:/Images/Images/OnlineMode/QRCodeScan.png"
                    ]
                    onItemClicked: {
                        switch(index){
                        case 0: // Export via file [.psbt]
                            savefileDialog.currentFile = StandardPaths.writableLocation(StandardPaths.DocumentsLocation) + "/"
                                    + RoomWalletData.getValidFilename(AppModel.transactionInfo.txid)
                                    + ".psbt"
                            savefileDialog.open()
                            break;
                        case 1: // Export via QR Keystone
                            requestExportQRKeyStone()
                            break;
                        case 2: // Export via QR Passport
                            requestExportQRPassport()
                            break;
                        default:
                            break;
                        }
                    }
                }
            }
            QTextButton {
                id: startbroatcast
                width: 180
                height: 48
                label.text: STR.STR_QML_295
//                visible: (NUNCHUCKTYPE.CHAT_TAB === AppModel.tabIndex || AppModel.transactionInfo.initEventId !== "") ? AppModel.transactionInfo.createByMe : true
                label.font.pixelSize: 16
                type: eTypeE
                onButtonClicked: {
                    QMLHandle.sendEvent(EVT.EVT_TRANSACTION_BROADCAST_REQUEST)
                }
            }
        }
    }
    Component {
        id: btnPendingConfirmation
        Row {
            height: 48
            spacing: 16
            QTextButton {
                id: viewBlockstreamBtn
                width: 247
                height: 48
                label.text: STR.STR_QML_291
                label.font.pixelSize: 16
                type: eTypeB
                onButtonClicked: {
                    Qt.openUrlExternally(urlActiveLink())
                }
            }
            QTextButton {
                id: replaceByFeeButton
                width: 210
                height: 48
                label.text: STR.STR_QML_293
                label.font.pixelSize: 16
                type: eTypeB
                visible: needShowRBF
                onButtonClicked: {
                    QMLHandle.sendEvent(EVT.EVT_TRANSACTION_REPLACE_BY_FEE_REQUEST)
                }
            }
            QButtonLargeTail {
                id: advancedBtn
                width: 220
                height: 48
                type: eSECONDARY
                label: STR.STR_QML_294
                optionVisible: imExContextMenu.visible
                onButtonClicked: {
                    imExContextMenu.x = 20
                    imExContextMenu.y = 20 - imExContextMenu.height
                    imExContextMenu.open()
                }
                QContextMenu {
                    id: imExContextMenu
                    menuWidth: 320
                    labels: [
                        STR.STR_QML_300,
                        STR.STR_QML_115,
                        STR.STR_QML_116,
                    ]
                    icons: [
                        "qrc:/Images/Images/download_031F2B.png",
                        "qrc:/Images/Images/OnlineMode/QRCodeScan.png",
                        "qrc:/Images/Images/OnlineMode/QRCodeScan.png"
                    ]
                    onItemClicked: {
                        switch(index){
                        case 0: // Export via file [.psbt]
                            savefileDialog.currentFile = StandardPaths.writableLocation(StandardPaths.DocumentsLocation) + "/"
                                    + RoomWalletData.getValidFilename(AppModel.transactionInfo.txid)
                                    + ".psbt"
                            savefileDialog.open()
                            break;
                        case 1: // Export via QR Keystone
                            requestExportQRKeyStone()
                            break;
                        case 1: // Export via QR Passport
                            requestExportQRPassport()
                            break;
                        default:
                            break;
                        }
                    }
                }
            }
        }
    }
    Component {
        id: btnConfirmed
        Row {
            height: 48
            spacing: 16
            QTextButton {
                id: viewBlockstreamBtn
                width: 247
                height: 48
                label.text: STR.STR_QML_291
                label.font.pixelSize: 16
                type: eTypeB
                onButtonClicked: {
                    Qt.openUrlExternally(urlActiveLink())
                }
            }
        }
    }
    Component {
        id: btnReplacedRejected
        Row {
            height: 48
            spacing: 16
            QTextButton {
                id: removebtn
                width: 200
                height: 48
                enabled: visible
                label.text: STR.STR_QML_292
                label.font.pixelSize: 16
                visible: !needShowCancel
                type: eTypeB
                onButtonClicked: {
                    QMLHandle.sendEvent(EVT.EVT_TRANSACTION_REMOVE_REQUEST)
                }
            }
            QButtonLargeTail {
                id: advancedBtn
                width: 220
                height: 48
                type: eSECONDARY
                label: STR.STR_QML_294
                optionVisible: imExContextMenu.visible
                onButtonClicked: {
                    imExContextMenu.x = 20
                    imExContextMenu.y = 20 - imExContextMenu.height
                    imExContextMenu.open()
                }
                QContextMenu {
                    id: imExContextMenu
                    menuWidth: 320
                    labels: [
                        STR.STR_QML_300,
                        STR.STR_QML_115,
                        STR.STR_QML_116,
                        STR.STR_QML_691,
                    ]
                    icons: [
                        "qrc:/Images/Images/download_031F2B.png",
                        "qrc:/Images/Images/OnlineMode/QRCodeScan.png",
                        "qrc:/Images/Images/OnlineMode/QRCodeScan.png",
                        "qrc:/Images/Images/copy-dark.svg"
                    ]
                    onItemClicked: {
                        switch(index){
                        case 0: // Export via file [.psbt]
                            savefileDialog.currentFile = StandardPaths.writableLocation(StandardPaths.DocumentsLocation) + "/"
                                    + RoomWalletData.getValidFilename(AppModel.transactionInfo.txid)
                                    + ".psbt"
                            savefileDialog.open()
                            break;
                        case 1: // Export via QR Keystone
                            requestExportQRKeyStone()
                            break;
                        case 2: // Export via QR Passport
                            requestExportQRPassport()
                            break;
                        case 3: // Copy transaction ID
                            requestCopyTransactionID()
                            break;
                        default:
                            break;
                        }
                    }
                }
            }
        }
    }
    Component {
        id: cancelTransactionBtn
        QButtonTextLink {
            height: 48
            label: STR.STR_QML_296
            fontPixelSize: 16
            textColor: ["#CF4018", "#839096", "#35ABEE"]
            displayIcon: false
            onButtonClicked: {
                QMLHandle.sendEvent(EVT.EVT_TRANSACTION_CANCEL_REQUEST)
            }
        }
    }
    QQrImportScanner {
        id: qrcodeImport
        property int qrType: Popup_t.IMPORT_WALLET_QRCODE_KEYSTONE
        onTagFound: {
            if(qrType === Popup_t.IMPORT_WALLET_QRCODE_KEYSTONE){
                if(AppModel.parseKeystoneTransaction(qrcodeImport.tags)){
                    qrcodeImport.close();
                    QMLHandle.sendEvent(EVT.EVT_TRANSACTION_IMPORT_QRCODE)
                }
            }
            else if(qrType === Popup_t.IMPORT_WALLET_QRCODE_PASSPORT){
                if(AppModel.parsePassportTransaction(qrcodeImport.tags)){
                    qrcodeImport.close();
                    QMLHandle.sendEvent(EVT.EVT_TRANSACTION_IMPORT_QRCODE)
                }
            }
            else{
                return;
            }
        }
        onClosed: qrcodeImport.qrType = Popup_t.IMPORT_WALLET_QRCODE_KEYSTONE
    }
    QQrExportResult {
        id: qrcodeExportResult
        model: AppModel.qrExported
    }
    FileDialog {
        id: openfileDialog
        fileMode: FileDialog.OpenFile
        onAccepted: {
            QMLHandle.sendEvent(EVT.EVT_TRANSACTION_IMPORT_REQUEST, openfileDialog.file)
        }
    }
    FileDialog {
        id: savefileDialog
        fileMode: FileDialog.SaveFile
        onAccepted: {
            QMLHandle.sendEvent(EVT.EVT_TRANSACTION_EXPORT_REQUEST, savefileDialog.currentFile)
        }
    }
    Popup {
        id: displayAddressBusybox
        width: parent.width
        height: parent.height
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape
        background: Item{}
        property string addrToVerify: ""
        Rectangle {
            id: displayAddressMask
            width: 500
            height: 250
            radius: 8
            color: "#FFFFFF"
            anchors.centerIn: parent
            layer.enabled: true
            layer.effect: OpacityMask {
                maskSource: Rectangle {
                    width: 500
                    height: 250
                    radius: 8
                }
            }
            QCloseButton {
                anchors {
                    right: parent.right
                    rightMargin: 16
                    top: parent.top
                    topMargin: 16
                }
                onClicked: {displayAddressBusybox.close()  ; displayAddressBusybox.addrToVerify = ""}
            }
            Column {
                spacing: 16
                anchors.centerIn: parent
                QBusyIndicator {
                    width: 70
                    height: 70
                    anchors.horizontalCenter: parent.horizontalCenter
                }
                QText {
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.family: "Lato"
                    font.pixelSize: 14
                    font.weight: Font.Bold
                    text: STR.STR_QML_008
                }
                Rectangle {
                    width: 450
                    height: 60
                    color: Qt.rgba(0, 0, 0, 0.1)
                    anchors.horizontalCenter: parent.horizontalCenter
                    QText {
                        anchors.fill: parent
                        anchors.leftMargin: 5
                        anchors.rightMargin: 5
                        anchors.topMargin: 5
                        anchors.bottomMargin: 5
                        wrapMode: Text.WrapAnywhere
                        horizontalAlignment: Text.AlignHCenter
                        text: displayAddressBusybox.addrToVerify
                        anchors.centerIn: parent
                        font.pixelSize: 18
                        color: "#323E4A"
                    }
                }
            }
        }
        DropShadow {
            anchors.fill: displayAddressMask
            horizontalOffset: 3
            verticalOffset: 5
            spread: 0
            radius: 8
            samples: 30
            color: "#aa000000"
            source: displayAddressMask
        }
    }
    Popup {
        id: signingBusyBox
        property int signerType: -1
        width: parent.width
        height: parent.height
        modal: true
        focus: true
        background: Item{}
        Rectangle {
            id: signingBusyBoxMask
            width: 300
            height: 196
            radius: 24
            color: "#FFFFFF"
            anchors.centerIn: parent
            layer.enabled: true
            layer.effect: OpacityMask {
                maskSource: Rectangle {
                    width: 300
                    height: 196
                    radius: 24
                }
            }
            Column {
                spacing: 12
                anchors.centerIn: parent
                QBusyIndicator {
                    width: 70
                    height: 70
                    anchors.horizontalCenter: parent.horizontalCenter
                }
                QText {
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.family: "Lato"
                    font.pixelSize: 16
                    font.weight: Font.Bold
                    text: STR.STR_QML_297
                }
                QText {
                    width: 252
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.family: "Lato"
                    font.pixelSize: 16
                    text: signingBusyBox.signerType === NUNCHUCKTYPE.HARDWARE ? STR.STR_QML_298 : STR.STR_QML_122
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignHCenter
                }
            }
        }
        DropShadow {
            anchors.fill: signingBusyBoxMask
            horizontalOffset: 3
            verticalOffset: 5
            spread: 0
            radius: 8
            samples: 30
            color: "#aa000000"
            source: signingBusyBoxMask
        }
    }
    QConfirmYesNoPopup{
        id:_confirm
        property int signerType: -1
        property string fingerPrint : ""
        contentText:STR.STR_QML_687
        onConfirmNo: close()
        onConfirmYes: {
            close()
            signingBusyBox.signerType = _confirm.signerType
            signingBusyBox.open()
            timerSigningTx.fingerPrint = _confirm.fingerPrint
            timerSigningTx.restart()
        }
    }
    Connections {
        target: AppModel
        onStartDisplayAddress: {
            if(isOnTop) displayAddressBusybox.open()
            else displayAddressBusybox.close()
        }
        onFinishedDisplayAddress: {
            displayAddressBusybox.close()
        }
        onFinishedSigningTransaction : {signingBusyBox.close(); signingBusyBox.signerType = -1}
    }
    Timer {
        id: timerSigningTx
        property string fingerPrint : ""
        interval: 500
        onTriggered: {
            if(timerSigningTx.fingerPrint !== ""){ QMLHandle.sendEvent(EVT.EVT_TRANSACTION_SIGN_REQUEST, timerSigningTx.fingerPrint)}
        }
    }
    function requestExportQRKeyStone(){
        QMLHandle.sendEvent(EVT.EVT_TRANSACTION_EXPORT_QRCODE, "keystone")
        qrcodeExportResult.open()
    }
    function requestExportQRPassport(){
        QMLHandle.sendEvent(EVT.EVT_TRANSACTION_EXPORT_QRCODE, "passport")
        qrcodeExportResult.open()
    }
    function requestImportQRKeyStone(){
        qrcodeImport.qrType = Popup_t.IMPORT_WALLET_QRCODE_KEYSTONE
        qrcodeImport.open()
    }
    function requestImportQRPassport(){
        qrcodeImport.qrType = Popup_t.IMPORT_WALLET_QRCODE_PASSPORT
        qrcodeImport.open()
    }
    function urlActiveLink(){
        var activeLink = ""
        switch(AppSetting.primaryServer){
        case NUNCHUCKTYPE.MAIN:
            activeLink = BLOCKSTREAM_MAINNET + AppModel.transactionInfo.txid
            break;
        case NUNCHUCKTYPE.TESTNET:
            activeLink = BLOCKSTREAM_TESTNET + AppModel.transactionInfo.txid
            break;
        case NUNCHUCKTYPE.SIGNET:
            activeLink = AppSetting.signetStream + AppModel.transactionInfo.txid
            break
        default: break
        }
        return activeLink
    }
    function requestCopyTransactionID(){
        var to_addr = AppModel.transactionInfo ? AppModel.transactionInfo.destinationList.reciever : ""
        ClientController.copyMessage(to_addr)
    }
}
