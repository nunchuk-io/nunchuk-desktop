import "../../../localization/STR_QML.js" as STR
import "../../Components/customizes"
import "../../Components/customizes/Buttons"
import "../../Components/customizes/Chats"
import "../../Components/customizes/Popups"
import "../../Components/customizes/QRCodes"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Transactions"
import "../../Components/origins"
import DataPool 1.0
import EWARNING 1.0
import HMIEVENTS 1.0
import NUNCHUCKTYPE 1.0
import QRCodeItem 1.0
import Qt.labs.platform 1.1
import QtGraphicalEffects 1.12
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

QScreen {
    id: screenTxInfoRoot

    property var    walletInfo: AppModel.walletInfo
    property var    transactionInfo: AppModel.transactionInfo
    property bool   isMiniscript: (walletInfo.walletType === NUNCHUCKTYPE.MINISCRIPT)
    readonly property int confirmations: Math.max(0, (AppModel.blockHeight - AppModel.transactionInfo.height) + 1)
    readonly property bool needShowRBF: !AppModel.walletInfo.isSharedWallet
    property var    timelockedUntil: AppModel.transactionInfo.timelockedUntil

    function requestExportPSBT() {
        savefileDialog.currentFile = StandardPaths.writableLocation(StandardPaths.DocumentsLocation) + "/" + RoomWalletData.getValidFilename(AppModel.transactionInfo.txid) + ".psbt";
        savefileDialog.open();
    }

    function requestExportViaQR() {
        qrcodeExportResult.filename = "Transaction_" + AppModel.transactionInfo.txid;
        qrcodeExportResult.open();
        var datalegacy = {
            "qrtype": "QR-transaction"
        };
        QMLHandle.sendEvent(EVT.EVT_TRANSACTION_EXPORT_QRCODE, datalegacy);
    }

    function requestExportViaBBQR() {
        qrcodeExportResult.filename = "Transaction_bbqr_" + AppModel.transactionInfo.txid;
        qrcodeExportResult.open();
        var datalegacy = {
            "qrtype": "BBQR-transaction"
        };
        QMLHandle.sendEvent(EVT.EVT_TRANSACTION_EXPORT_QRCODE, datalegacy);
    }

    function urlActiveLink() {
        var activeLink = "";
        switch (AppSetting.primaryServer) {
        case NUNCHUCKTYPE.MAIN:
            activeLink = EXPLORER_MAINNET + AppModel.transactionInfo.txid;
            break;
        case NUNCHUCKTYPE.TESTNET:
            activeLink = EXPLORER_TESTNET + AppModel.transactionInfo.txid;
            break;
        case NUNCHUCKTYPE.SIGNET:
            activeLink = EXPLORER_SIGNNET + AppModel.transactionInfo.txid;
            break;
        default:
            break;
        }
        return activeLink;
    }

    function requestCopyTransactionID() {
        var to_addr = AppModel.transactionInfo ? AppModel.transactionInfo.destinationList.reciever : "";
        ClientController.copyMessage(to_addr);
    }

    QOnScreenContent {
        id: contenCenter

        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        label.text: STR.STR_QML_785
        label.font.pixelSize: 32
        label.font.weight: Font.Medium
        onCloseClicked: {
            if (NUNCHUCKTYPE.CHAT_TAB === AppModel.tabIndex) {
                QMLHandle.sendEvent(EVT.EVT_ONLINE_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_TRANSACTION_INFO);
            } else {
                if (QMLHandle.onsRequester() === EVT.STATE_ID_SCR_TRANSACTION_HISTORY)
                    QMLHandle.sendEvent(EVT.EVT_TRANSACTION_INFO_BACK_REQUEST);
                else if (QMLHandle.onsRequester() === EVT.STATE_ID_SCR_UTXO_OUTPUT)
                    QMLHandle.sendEvent(EVT.EVT_TRANSACTION_INFO_BACK_UTXO_INFO);
                else
                    QMLHandle.sendEvent(EVT.EVT_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_TRANSACTION_INFO);
            }
        }
        content: AppModel.transactionInfo.isReceiveTx ? txReceived : txSend
        bottomLeft: {
            if (AppModel.transactionInfo.isReceiveTx)
                return receiveTxBtnLeft;
            else
                return null;
        }
        bottomRight: {
            if (!AppModel.transactionInfo.isReceiveTx) {
                if (AppModel.transactionInfo.status === NUNCHUCKTYPE.PENDING_SIGNATURES)
                    return btnPendingSignatures;
                else if (AppModel.transactionInfo.status === NUNCHUCKTYPE.READY_TO_BROADCAST)
                    return btnReadyToBroadcast;
                else if (AppModel.transactionInfo.status === NUNCHUCKTYPE.PENDING_CONFIRMATION)
                    return btnPendingConfirmation;
                else if (AppModel.transactionInfo.status === NUNCHUCKTYPE.CONFIRMED)
                    return btnConfirmed;
                else
                    return btnReplacedRejected;
            } else {
                return receiveTxBtnRight;
            }
        }

        extraHeader: Row {
            spacing: 4
            QBadge {
                color: {
                    if(walletInfo.walletType === NUNCHUCKTYPE.MINISCRIPT && AppModel.transactionInfo.timelockedUntil && AppModel.transactionInfo.timelockedUntil.hasLocked) {
                        return "#FFEAF9"
                    }
                    else {
                        return GlobalData.transactionColor(AppModel.transactionInfo.status)
                    }
                }
                text:  {
                    if(walletInfo.walletType === NUNCHUCKTYPE.MINISCRIPT && AppModel.transactionInfo.timelockedUntil && AppModel.transactionInfo.timelockedUntil.hasLocked) {
                        return STR.STR_QML_1875
                    }
                    else {
                        return GlobalData.transactionStatus(AppModel.transactionInfo.status, confirmations)
                    }
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
    }

    Component {
        id: txSend
        QSendTransaction {
            transactionInfo: AppModel.transactionInfo
            myRole: AppModel.walletInfo.myRole
            onAddrToVerify: {
                displayAddressBusybox.addrToVerify = addr;
                QMLHandle.sendEvent(EVT.EVT_TRANSACTION_VERIFY_ADDRESS, addr);
            }
            onNewMemoNotify: {
                QMLHandle.sendEvent(EVT.EVT_TRANSACTION_SET_MEMO_REQUEST, newMemo);
            }
            onKeySignRequest: {
                signingBusyBox.signerType = signer.single_signer_type;
                signingBusyBox.open();
                timerSigningTx.fingerPrint = signer.singleSigner_masterFingerPrint;
                timerSigningTx.restart();
            }
            onKeyScanRequest: {
                QMLHandle.sendEvent(EVT.EVT_TRANSACTION_SCAN_DEVICE_REQUEST);
            }
            onKeyExportRequest: {
                requestExportPSBT();
            }
            onKeyImportRequest: {
                openfileDialog.open();
            }
            onKeyEnterPreImageInput: {
                enterPreimage.clearText()
                enterPreimage.hashData = hashData
                enterPreimage.typeNode = typeNode                
                enterPreimage.open()
            }
        }

    }

    Component {
        id: txReceived

        QReceivedTransaction {
            transactionInfo: AppModel.transactionInfo
            onNewMemoNotify: {
                QMLHandle.sendEvent(EVT.EVT_TRANSACTION_SET_MEMO_REQUEST, newMemo);
            }
        }

    }
    // Buttons Receive

    Component {
        id: receiveTxBtnLeft

        Item {
            width: 257
            height: 48

            QBtnOptions {
                // Request signature
                // Schedule broadcast
                // Cancel tx

                id: optionsBtn

                enableRequestSignature: false
                enableScheduleBroadcast: false
                enableCancelTransaction: false
                enableShowInvoice: true
                funcs: [function() {
                }, function() {
                }, function() {
                    // Copy id
                    AppModel.transactionInfo.copyTransactionID();
                }, function() {
                }, function() {
                    // show invoice
                    invoice.open();
                }]
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
                    font.capitalization: Font.AllUppercase
                    font.family: "Montserrat"
                    font.pixelSize: 12
                    font.weight: Font.DemiBold
                    color: mouseExplorerBlockchain3.containsMouse ? "#35ABEE" : "#031F2B"
                    wrapMode: Text.WordWrap

                    MouseArea {
                        id: mouseExplorerBlockchain3

                        hoverEnabled: true
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: {
                            Qt.openUrlExternally(urlActiveLink());
                        }
                    }

                }

            }

        }

    }
    // Button Send

    Component {
        id: btnPendingSignatures

        Row {
            readonly property string myRole: AppModel.walletInfo.myRole

            height: 48
            spacing: 16

            QBtnMore {
                id: moreOptions

                property bool allowRole: (myRole !== "") && (myRole !== "OBSERVER") && (myRole !== "KEYHOLDER_LIMITED")

                width: 102
                height: 48
                enableRequestSignature: allowRole
                enableScheduleBroadcast: false
                enableCancelTransaction: AppModel.walletInfo.isGroupWallet ? allowRole : true
                enableExportTransaction: true
                enableImportTransaction: true
                isAssisedWallet: AppModel.walletInfo.isAssistedWallet
                isSharedWallet: AppModel.walletInfo.isSharedWallet
                onExportToPSBT: {
                    requestExportPSBT();
                }
                onExportToQR: {
                    requestExportViaQR();
                }
                onExportToBBQR: {
                    requestExportViaBBQR();
                }
                onImportViaPSBT: {
                    openfileDialog.open();
                }
                onImportViaQR: {
                    qrcodeImport.open();
                }
                onShowInvoice: {
                    invoice.open();
                }
                onRequestSignature: {
                    groupMembers.open();
                }
                onCancelTransaction: {
                    if (isSharedWallet)
                        QMLHandle.sendEvent(EVT.EVT_TRANSACTION_CANCEL_REQUEST);
                    else
                        QMLHandle.sendEvent(EVT.EVT_TRANSACTION_REMOVE_REQUEST);
                }
            }

        }

    }

    Component {
        id: btnReadyToBroadcast
        Row {
            height: 48
            spacing: 16
            QBtnMore {
                id: moreOptions
                width: 102
                height: 48
                enableCancelTransaction: true
                enableExportTransaction: true
                isAssisedWallet: AppModel.walletInfo.isAssistedWallet
                isSharedWallet: AppModel.walletInfo.isSharedWallet
                onExportToPSBT: {
                    requestExportPSBT();
                }
                onExportToQR: {
                    requestExportViaQR();
                }
                onExportToBBQR: {
                    requestExportViaBBQR();
                }
                onShowInvoice: {
                    invoice.open();
                }
                onCancelTransaction: {
                    if (isSharedWallet)
                        QMLHandle.sendEvent(EVT.EVT_TRANSACTION_CANCEL_REQUEST);
                    else
                        QMLHandle.sendEvent(EVT.EVT_TRANSACTION_REMOVE_REQUEST);
                }
            }
            QTextButton {
                id: startbroatcast
                width: 180
                height: 48
                label.text: STR.STR_QML_497
                label.font.pixelSize: 16
                type: eTypeE
                enabled: isMiniscript ? timelockedUntil.enabledBroadcastTxBtn : true
                onButtonClicked: {
                    QMLHandle.sendEvent(EVT.EVT_TRANSACTION_BROADCAST_REQUEST);
                }
            }
        }
    }

    Component {
        id: btnPendingConfirmation

        Row {
            height: 48
            spacing: 16

            QBtnMore {
                id: moreOptions

                width: 102
                height: 48
                enableExportTransaction: true
                enableCancelTransaction: true
                onExportToPSBT: {
                    requestExportPSBT();
                }
                onExportToQR: {
                    requestExportViaQR();
                }
                onExportToBBQR: {
                    requestExportViaBBQR();
                }
                onShowInvoice: {
                    invoice.open();
                }
                onCancelTransaction: {
                    QMLHandle.sendEvent(EVT.EVT_TRANSACTION_REMOVE_REQUEST);
                }
            }

            QTextButton {
                id: replaceByFeeButton

                width: 141
                height: 48
                label.text: STR.STR_QML_293
                label.font.pixelSize: 16
                type: eTypeB
                visible: needShowRBF
                onButtonClicked: {
                    QMLHandle.sendEvent(EVT.EVT_TRANSACTION_REPLACE_BY_FEE_REQUEST);
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
                    Qt.openUrlExternally(urlActiveLink());
                }
            }

            QTextButton {
                id: manageCoin

                width: 123
                height: 48
                label.text: STR.STR_QML_1514
                label.font.pixelSize: 16
                type: eTypeB
                onButtonClicked: {
                    var input = {
                        "type": "manage-coin-details"
                    };
                    AppModel.walletInfo.requestSyncSelectCoinForMakeTransaction(input);
                }
            }

        }

    }

    Component {
        id: btnConfirmed

        Row {
            height: 48
            spacing: 16

            QBtnMore {
                id: moreOptions

                width: 102
                height: 48
                enableExportTransaction: true
                onExportToPSBT: {
                    requestExportPSBT();
                }
                onExportToQR: {
                    requestExportViaQR();
                }
                onExportToBBQR: {
                    requestExportViaBBQR();
                }
                onShowInvoice: {
                    invoice.open();
                }
            }

            QTextButton {
                id: viewBlockstreamBtn

                width: 247
                height: 48
                label.text: STR.STR_QML_291
                label.font.pixelSize: 16
                type: eTypeB
                onButtonClicked: {
                    Qt.openUrlExternally(urlActiveLink());
                }
            }

            QTextButton {
                id: manageCoin

                width: 123
                height: 48
                label.text: STR.STR_QML_1514
                label.font.pixelSize: 16
                type: eTypeB
                onButtonClicked: {
                    var input = {
                        "type": "manage-coin-details"
                    };
                    AppModel.walletInfo.requestSyncSelectCoinForMakeTransaction(input);
                }
            }

        }

    }

    Component {
        id: btnReplacedRejected

        Row {
            height: 48
            spacing: 16

            QBtnMore {
                id: moreOptions

                width: 102
                height: 48
                enableCancelTransaction: true
                enableExportTransaction: true
                isAssisedWallet: AppModel.walletInfo.isAssistedWallet
                isSharedWallet: AppModel.walletInfo.isSharedWallet
                onExportToPSBT: {
                    requestExportPSBT();
                }
                onExportToQR: {
                    requestExportViaQR();
                }
                onExportToBBQR: {
                    requestExportViaBBQR();
                }
                onCancelTransaction: {
                    if (isSharedWallet)
                        QMLHandle.sendEvent(EVT.EVT_TRANSACTION_CANCEL_REQUEST);
                    else
                        QMLHandle.sendEvent(EVT.EVT_TRANSACTION_REMOVE_REQUEST);
                }
                onShowInvoice: {
                    invoice.open();
                }
            }

        }

    }

    Component {
        id: btnPendingConfirmationRight

        QTextButton {
            id: replaceByFeeButton

            width: 141
            height: 48
            label.text: STR.STR_QML_296
            label.font.pixelSize: 16
            type: eTypeB
            onButtonClicked: {
            }
        }

    }

    QQrImportScanner {
        id: qrcodeImport

        onTagFound: {
            if (qrcodeImport.complete) {
                AppModel.transactionInfo.parseQRTransaction(qrcodeImport.tags);
                qrcodeImport.close();
                QMLHandle.sendEvent(EVT.EVT_TRANSACTION_IMPORT_QRCODE);
            }
        }
    }

    QQrExportResultPDF {
        id: qrcodeExportResult

        model: AppModel.qrExported
    }

    FileDialog {
        id: openfileDialog

        fileMode: FileDialog.OpenFile
        onAccepted: {
            QMLHandle.sendEvent(EVT.EVT_TRANSACTION_IMPORT_REQUEST, openfileDialog.file);
        }
    }

    FileDialog {
        id: savefileDialog

        fileMode: FileDialog.SaveFile
        onAccepted: {
            QMLHandle.sendEvent(EVT.EVT_TRANSACTION_EXPORT_REQUEST, savefileDialog.currentFile);
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

    QPopupEnterPreimageInHex {
        id: enterPreimage
        onEnterText: {
            transactionInfo.enterPreimageInput(enterPreimage.hashData, str, enterPreimage.typeNode)
        }
    }

    Connections {
        function onStartDisplayAddress(wallet_id, address) {
            if (isOnTop)
                displayAddressBusybox.open();
            else
                displayAddressBusybox.close();
        }

        function onFinishedDisplayAddress(result) {
            displayAddressBusybox.close();
        }

        function onFinishedSigningTransaction() {
            signingBusyBox.close();
            signingBusyBox.signerType = -1;
        }

        target: AppModel
    }

    Timer {
        id: timerSigningTx

        property string fingerPrint: ""

        interval: 500
        onTriggered: {
            if (timerSigningTx.fingerPrint !== "")
                QMLHandle.sendEvent(EVT.EVT_TRANSACTION_SIGN_REQUEST, timerSigningTx.fingerPrint);

        }
    }

    QGroupMembers {
        id: groupMembers
        onRequestSignature: {
            AppModel.transactionInfo.requestSignatures(membership_id);
        }
    }

    QTransactionInvoicePopup {
        id: invoice

        width: parent.width
        height: parent.height
        txid: AppModel.transactionInfo.txid
        total: AppModel.transactionInfo.total
        subtotal: AppModel.transactionInfo.subtotal
        blocktime: AppModel.transactionInfo.blocktime
        fee: AppModel.transactionInfo.fee
        memo: AppModel.transactionInfo.memo
        hasChange: AppModel.transactionInfo.hasChange
        changeAmount: AppModel.transactionInfo.change.amount
        changeAddress: AppModel.transactionInfo.change.address
        isRecieveTx: AppModel.transactionInfo.isReceiveTx
        destinations: AppModel.transactionInfo.destinationList
    }

}
