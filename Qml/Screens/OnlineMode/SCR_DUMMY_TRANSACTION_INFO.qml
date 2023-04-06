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
            color: "#FFD7D9"
            QText {
                id: indicatorStatusTop
                anchors.centerIn: parent
                font.pixelSize: 16
                font.weight: Font.Medium
                font.family: "Lato"
                color: "#031F2B"
                text: STR.STR_QML_283
            }
        }
        onCloseClicked: {
            QMLHandle.sendEvent(EVT.EVT_ONLINE_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_DUMMY_TRANSACTION_INFO)
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
                                                QMLHandle.sendEvent(EVT.EVT_DUMMY_TRANSACTION_VERIFY_ADDRESS, displayAddressBusybox.addrToVerify)
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
                                            text: qsTr("$%1 %2").arg(model.destination_amount_currency).arg(AppSetting.currency)
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
                                        text: qsTr("$%1 %2").arg(AppModel.transactionInfo.feeCurrency).arg(AppSetting.currency)
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
                                        text: qsTr("$%1 %2").arg(AppModel.transactionInfo.totalCurrency).arg(AppSetting.currency)
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
                                        text: qsTr("$%1 %2").arg(AppModel.transactionInfo.change.amountCurrency).arg(AppSetting.currency)
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
                                        QMLHandle.sendEvent(EVT.EVT_DUMMY_TRANSACTION_SET_MEMO_REQUEST, newMemo)
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
                                width: 16
                                height: 16
                                source: "qrc:/Images/Images/OnlineMode/pending_actions-24px.png"
                                anchors.verticalCenter: parent.verticalCenter
                            }
                            QText {
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
                sourceComponent: btnPendingSignatures
            }
            Loader {
                id: bottomLeftButtons
                anchors.left: parent.left
                anchors.leftMargin: 36
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 36
                sourceComponent: cancelTransactionBtn
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
                                        text: qsTr("$%1 %2").arg(model.destination_amount_currency).arg(AppSetting.currency)
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
                                        QMLHandle.sendEvent(EVT.EVT_DUMMY_TRANSACTION_SET_MEMO_REQUEST, currentText)
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
                tx_status: AppModel.transactionInfo.status
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
                    QMLHandle.sendEvent(EVT.EVT_DUMMY_TRANSACTION_SCAN_DEVICE_REQUEST)
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
        QTextButton {
            width: 200
            height: 48
            label.text: STR.STR_QML_427
            label.font.pixelSize: 16
            type: eTypeB
            onButtonClicked: {
                QMLHandle.sendEvent(EVT.EVT_DUMMY_TRANSACTION_SIGN_CONFIRM_REQUEST)
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
                QMLHandle.sendEvent(EVT.EVT_DUMMY_TRANSACTION_INFO_BACK)
            }
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
            if(timerSigningTx.fingerPrint !== ""){ QMLHandle.sendEvent(EVT.EVT_DUMMY_TRANSACTION_SIGN_REQUEST, timerSigningTx.fingerPrint)}
        }
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
