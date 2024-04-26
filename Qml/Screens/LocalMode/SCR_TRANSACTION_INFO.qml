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
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../Components/customizes/Transactions"
import "../../Components/customizes/Popups"
import "../../Components/customizes/QRCodes"
import "../../../localization/STR_QML.js" as STR

QScreen {
    readonly property int  confirmations: Math.max(0, (AppModel.chainTip - AppModel.transactionInfo.height)+1)
    readonly property bool needShowRBF:   !AppModel.walletInfo.isSharedWallet

    QOnScreenContent {
        id: contenCenter
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        label.text: STR.STR_QML_282
        label.font.pixelSize: 32
        label.font.weight: Font.Medium
        extraHeader: Row {
            spacing: 4
            QBadge {
                color: {
                    if(AppModel.transactionInfo.status === NUNCHUCKTYPE.PENDING_SIGNATURES){ return "#FFD7D9" }
                    else if(AppModel.transactionInfo.status === NUNCHUCKTYPE.READY_TO_BROADCAST){ return "#FDEBD2" }
                    else if(AppModel.transactionInfo.status === NUNCHUCKTYPE.PENDING_CONFIRMATION){ return "#E8DAFF" }
                    else if(AppModel.transactionInfo.status === NUNCHUCKTYPE.CONFIRMED){ return "#D0E2FF" }
                    else if(AppModel.transactionInfo.status === NUNCHUCKTYPE.NETWORK_REJECTED){ return "#CF4018" }
                    else if(AppModel.transactionInfo.status === NUNCHUCKTYPE.REPLACED){ return "#EAEAEA" }
                    else{ return "#FFD7D9" }
                }
                text:  {
                    if(AppModel.transactionInfo.status === NUNCHUCKTYPE.PENDING_SIGNATURES){ return STR.STR_QML_283 }
                    else if(AppModel.transactionInfo.status === NUNCHUCKTYPE.READY_TO_BROADCAST){ return STR.STR_QML_284 }
                    else if(AppModel.transactionInfo.status === NUNCHUCKTYPE.NETWORK_REJECTED){ return STR.STR_QML_285 }
                    else if(AppModel.transactionInfo.status === NUNCHUCKTYPE.PENDING_CONFIRMATION){ return STR.STR_QML_286 }
                    else if(AppModel.transactionInfo.status === NUNCHUCKTYPE.CONFIRMED){ return qsTr("%1 %2").arg(confirmations).arg(STR.STR_QML_287) }
                    else if(AppModel.transactionInfo.status === NUNCHUCKTYPE.REPLACED){ return STR.STR_QML_288 }
                    else{ return STR.STR_QML_082 }
                }
                font.weight: Font.Normal
                fontSize: 12
            }
            Rectangle {
                border.width: 1
                border.color: "#DEDEDE"
                width: 106
                height: 24
                radius: 20
                visible: AppModel.transactionInfo.replaceTxid !== ""
                QLato {
                    text: STR.STR_QML_293
                    font.weight: Font.Normal
                    font.pixelSize: 12
                    anchors.centerIn: parent
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
        content: AppModel.transactionInfo.isReceiveTx ? txReceived : txSend
        bottomLeft: {
            if (AppModel.transactionInfo.isReceiveTx) {
                return receiveTxBtnLeft
            }
            else {
                if(AppModel.transactionInfo.status === NUNCHUCKTYPE.PENDING_CONFIRMATION){ return btnPendingConfirmationLeft }
                else { return null }
            }
        }
        bottomRight: {
            if (!AppModel.transactionInfo.isReceiveTx) {
                if(AppModel.transactionInfo.status === NUNCHUCKTYPE.PENDING_SIGNATURES){ return btnPendingSignatures }
                else if(AppModel.transactionInfo.status === NUNCHUCKTYPE.READY_TO_BROADCAST){ return btnReadyToBroadcast }
                else if(AppModel.transactionInfo.status === NUNCHUCKTYPE.PENDING_CONFIRMATION){ return btnPendingConfirmationRight }
                else if(AppModel.transactionInfo.status === NUNCHUCKTYPE.CONFIRMED){ return btnConfirmed }
                else { return btnReplacedRejected }
            }
            else {
                return receiveTxBtnRight
            }
        }
    }
    Component {
        id: txSend
        QSendTransaction {
            transactionInfo: AppModel.transactionInfo
            myRole: AppModel.walletInfo.myRole
            onAddrToVerify: {
                displayAddressBusybox.addrToVerify = addr
                QMLHandle.sendEvent(EVT.EVT_TRANSACTION_VERIFY_ADDRESS, addr)
            }
            onNewMemoNotify: {
                QMLHandle.sendEvent(EVT.EVT_TRANSACTION_SET_MEMO_REQUEST, newMemo)
            }
            onKeySignRequest: {
                if(signer.single_signer_type === NUNCHUCKTYPE.SOFTWARE){
                    _confirm.open()
                    _confirm.signerType = signer.single_signer_type
                    _confirm.fingerPrint = signer.singleSigner_masterFingerPrint
                }
                else{
                    signingBusyBox.signerType = signer.single_signer_type
                    signingBusyBox.open()
                    timerSigningTx.fingerPrint = signer.singleSigner_masterFingerPrint
                    timerSigningTx.restart()
                }
            }
            onKeyScanRequest: {
                QMLHandle.sendEvent(EVT.EVT_TRANSACTION_SCAN_DEVICE_REQUEST)
            }
            onKeyExportRequest: {
                savefileDialog.currentFile = StandardPaths.writableLocation(StandardPaths.DocumentsLocation) + "/"
                        + RoomWalletData.getValidFilename(transactionInfo.txid)
                        + ".psbt"
                savefileDialog.open()
            }
            onKeyImportRequest: {
                openfileDialog.open()
            }
        }
    }
    Component {
        id: txReceived
        QReceivedTransaction {
            transactionInfo: AppModel.transactionInfo
            onNewMemoNotify: {
                QMLHandle.sendEvent(EVT.EVT_TRANSACTION_SET_MEMO_REQUEST, newMemo)
            }
        }
    }
    Component {
        id: receiveTxBtnLeft
        Item {
            width: 257
            height: 48
            QBtnOptions {
                id: optionsBtn
                enableRequestSignature : false
                enableScheduleBroadcast: false
                enableCancelTransaction: (AppModel.transactionInfo.status === NUNCHUCKTYPE.REPLACED) || (AppModel.transactionInfo.status === NUNCHUCKTYPE.NETWORK_REJECTED)
                isAssisedWallet: AppModel.walletInfo.isAssistedWallet
                isSharedWallet:  AppModel.walletInfo.isSharedWallet
                funcs: [
                    function(){ // Request signature

                    },
                    function(){ // Schedule broadcast

                    },
                    function(){ // Copy id
                        AppModel.transactionInfo.copyTransactionID()
                    },
                    function(){ // Cancel tx
                        QMLHandle.sendEvent(EVT.EVT_TRANSACTION_REMOVE_REQUEST)
                    },
                ]
            }
        }
    }
    Component {
        id: receiveTxBtnRight
        Item {
            width: 257
            height: 48
            Row {
                anchors.right: parent.right
                width: 257
                height: 24
                spacing: 8
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
        }
    }

    // Buttons
    Component {
        id: btnPendingSignatures
        Row {
            height: 48
            spacing: 16
            readonly property string myRole: AppModel.walletInfo.myRole
            QBtnOptions {
                id: optionsBtn
                readonly property bool allowMoreOption:    (myRole !== "")
                                                        && (myRole !== "OBSERVER")
                                                        && (myRole !== "KEYHOLDER_LIMITED")
                enableRequestSignature : allowMoreOption
                enableScheduleBroadcast: false
                enableCancelTransaction: AppModel.walletInfo.isGroupWallet ? allowMoreOption : true
                isAssisedWallet: AppModel.walletInfo.isAssistedWallet
                isSharedWallet:  AppModel.walletInfo.isSharedWallet
                visible: myRole !== "OBSERVER"
                funcs: [
                    function(){ // Request signature
                        groupMembers.open()
                    },
                    function(){ // Schedule broadcast

                    },
                    function(){ // Copy id
                        AppModel.transactionInfo.copyTransactionID()
                    },
                    function(){ // Cancel tx
                        if(isSharedWallet){
                            QMLHandle.sendEvent(EVT.EVT_TRANSACTION_CANCEL_REQUEST)
                        }
                        else{
                            QMLHandle.sendEvent(EVT.EVT_TRANSACTION_REMOVE_REQUEST)
                        }
                    },
                ]
            }
            QBtnExportImport {
                id: advancedBtn
                visible: myRole !== "OBSERVER"
                funcs: [
                    function() {
                        // Export via file [.psbt]
                        savefileDialog.currentFile = StandardPaths.writableLocation(StandardPaths.DocumentsLocation) + "/"
                                + RoomWalletData.getValidFilename(AppModel.transactionInfo.txid)
                                + ".psbt"
                        savefileDialog.open()
                    },
                    function() {
                        // Export via QR
                        requestExportViaQR()
                    },
                    function() {
                        // Import via file [.psbt]
                        openfileDialog.open()
                    },
                    function() {
                        // Import via QR
                        qrcodeImport.open()
                    }
                ]
            }
        }
    }
    Component {
        id: btnReadyToBroadcast
        Row {
            height: 48
            spacing: 16
            QBtnOptions {
                id: optionsBtn
                enableRequestSignature : false
                enableScheduleBroadcast: false
                enableCancelTransaction: true
                isAssisedWallet: AppModel.walletInfo.isAssistedWallet
                isSharedWallet:  AppModel.walletInfo.isSharedWallet
                funcs: [
                    function(){ // Request signature

                    },
                    function(){ // Schedule broadcast

                    },
                    function(){ // Copy id
                        AppModel.transactionInfo.copyTransactionID()
                    },
                    function(){ // Cancel tx
                        if(isSharedWallet){
                            QMLHandle.sendEvent(EVT.EVT_TRANSACTION_CANCEL_REQUEST)
                        }
                        else{
                            QMLHandle.sendEvent(EVT.EVT_TRANSACTION_REMOVE_REQUEST)
                        }
                    },
                ]
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
                        STR.STR_QML_114,
                    ]
                    icons: [
                        "qrc:/Images/Images/ExportFile.svg",
                        "qrc:/Images/Images/OnlineMode/QRCodeScan.png",
                    ]
                    onItemClicked: {
                        switch(index){
                        case 0: // Export via file [.psbt]
                            savefileDialog.currentFile = StandardPaths.writableLocation(StandardPaths.DocumentsLocation) + "/"
                                    + RoomWalletData.getValidFilename(AppModel.transactionInfo.txid)
                                    + ".psbt"
                            savefileDialog.open()
                            break;
                        case 1: // Export via QR
                            requestExportViaQR()
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
                label.font.pixelSize: 16
                type: eTypeE
                onButtonClicked: {
                    QMLHandle.sendEvent(EVT.EVT_TRANSACTION_BROADCAST_REQUEST)
                }
            }
        }
    }

    Component {
        id: btnPendingConfirmationLeft
        Row {
            height: 48
            spacing: 16
            QButtonTextLink {
                width: 170
                height: 36
                displayIcon: false
                label: STR.STR_QML_296
                textColor: ["#CF4018", "#CF4018", "#CF4018"]
                fontPixelSize: 16
                onButtonClicked: {
                    QMLHandle.sendEvent(EVT.EVT_TRANSACTION_REMOVE_REQUEST)
                }
            }
        }
    }

    Component {
        id: btnPendingConfirmationRight
        Row {
            height: 48
            spacing: 16
            QTextButton {
                id: replaceByFeeButton
                width: 141
                height: 48
                label.text: STR.STR_QML_293
                label.font.pixelSize: 16
                type: eTypeB
                visible: needShowRBF
                onButtonClicked: {
                    QMLHandle.sendEvent(EVT.EVT_TRANSACTION_REPLACE_BY_FEE_REQUEST)
                }
            }
            QTextButton {
                id: viewBlockstreamBtn
                width: 235
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
            QBtnOptions {
                id: optionsBtn
                enableRequestSignature : false
                enableScheduleBroadcast: false
                enableCancelTransaction: true
                isAssisedWallet: AppModel.walletInfo.isAssistedWallet
                isSharedWallet:  AppModel.walletInfo.isSharedWallet
                funcs: [
                    function(){ // Request signature

                    },
                    function(){ // Schedule broadcast

                    },
                    function(){ // Copy id
                        AppModel.transactionInfo.copyTransactionID()
                    },
                    function(){ // Cancel tx
                        QMLHandle.sendEvent(EVT.EVT_TRANSACTION_REMOVE_REQUEST)
                    },
                ]
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
                        STR.STR_QML_114,
                        STR.STR_QML_691,
                    ]
                    icons: [
                        "qrc:/Images/Images/ExportFile.svg",
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
                        case 1: // Export via QR
                            requestExportViaQR()
                            break;
                        case 2: // Copy transaction ID
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
    QQrImportScanner {
        id: qrcodeImport
        onTagFound: {
            if(qrcodeImport.complete){
                AppModel.transactionInfo.parseQRTransaction(qrcodeImport.tags)
                qrcodeImport.close();
                QMLHandle.sendEvent(EVT.EVT_TRANSACTION_IMPORT_QRCODE)
            }
        }
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
    QPopupDisplayAddressBusyBox {
        id: displayAddressBusybox
        width: parent.width
        height: parent.height
    }
    QPopupSigningBusyBox {
        id: signingBusyBox
        width: parent.width
        height: parent.height
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
    QGroupMembers {
        id: groupMembers
        onRequestSignature: {
            AppModel.transactionInfo.requestSignatures(membership_id)
        }
    }

    function requestExportViaQR(){
        QMLHandle.sendEvent(EVT.EVT_TRANSACTION_EXPORT_QRCODE)
        qrcodeExportResult.open()
    }
    function urlActiveLink(){
        var activeLink = ""
        switch(AppSetting.primaryServer){
        case NUNCHUCKTYPE.MAIN:
            activeLink = EXPLORER_MAINNET + AppModel.transactionInfo.txid
            break;
        case NUNCHUCKTYPE.TESTNET:
            activeLink = EXPLORER_TESTNET + AppModel.transactionInfo.txid
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
