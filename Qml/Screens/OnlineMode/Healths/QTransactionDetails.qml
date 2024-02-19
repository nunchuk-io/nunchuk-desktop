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
import DataPool 1.0
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Chats"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../Components/customizes/Transactions"
import "../../../Components/customizes/Popups"
import "../../../Components/customizes/QRCodes"
import "../../../../localization/STR_QML.js" as STR

QOnScreenContentTypeA {
    id:_content
    property int flow: AppModel.walletInfo.flow
    property var dummyTx: AppModel.walletInfo.dummyTx
    property var transactionInfo: dummyTx ? dummyTx.transactionInfo : null
    function closeTransaction() {
        QMLHandle.sendEvent(EVT.EVT_DUMMY_TRANSACTION_INFO_BACK)
    }

    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_785
    extraHeader: QBadge {
        color: "#FFD7D9"
        text: STR.STR_QML_283
    }
    onCloseClicked: closeTransaction()
    content: QSendTransaction {
        isDummy: true
        transactionInfo: _content.transactionInfo
        pendingSignatue: dummyTx ? dummyTx.pending_signatures : 1
        onAddrToVerify: {
            displayAddressBusybox.addrToVerify = addr
            var _address = {
                type: "address-to-verify",
                address: addr
            }
            QMLHandle.sendEvent(EVT.EVT_DUMMY_TRANSACTION_ACTION_ENTER_REQUEST, _address)
        }
        onNewMemoNotify: {
            var _memo = {
                type: "memo-notify",
                memo: newMemo
            }
            QMLHandle.sendEvent(EVT.EVT_DUMMY_TRANSACTION_ACTION_ENTER_REQUEST, _memo)
        }
        onKeySignRequest: {
            signingBusyBox.signerType = signer.single_signer_type
            signingBusyBox.open()
            timerSigningTx.fingerPrint = signer.singleSigner_masterFingerPrint
            timerSigningTx.restart()
        }
        onKeyScanRequest: {
            var _scanDevice = {
                type: "scan-device",
            }
            QMLHandle.sendEvent(EVT.EVT_DUMMY_TRANSACTION_ACTION_ENTER_REQUEST, _scanDevice )
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
    bottomLeft: Item {}
    bottomRight: QBtnExportImport {
        id: advancedBtn
        funcs: [
        function() {
            savefileDialog.currentFile = StandardPaths.writableLocation(StandardPaths.DocumentsLocation) + "/"
                    + RoomWalletData.getValidFilename(transactionInfo.txid)
                    + ".psbt"
            savefileDialog.open()
        },
        function() {
            requestExportViaQR()
        },
        function() {
            openfileDialog.open()
        },
        function() {
            transactionInfo.setScan_percent(0)
            qrcodeImport.open()
        }
        ]
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

    Timer {
        id: timerSigningTx
        property string fingerPrint : ""
        interval: 500
        onTriggered: {
            if(timerSigningTx.fingerPrint !== "") {
                var sign = {
                    type: "dummy-tx-sign",
                    xfp: timerSigningTx.fingerPrint
                }
                QMLHandle.sendEvent(EVT.EVT_DUMMY_TRANSACTION_ACTION_ENTER_REQUEST, sign)
            }
        }
    }
    QQrImportScanner {
        id: qrcodeImport
        onTagFound: {
            if(transactionInfo.parseQRSuccess(qrcodeImport.tags)){
                var _input = {
                    type: "dummy-tx-import-qr",
                    tags: qrcodeImport.tags
                }
                qrcodeImport.close();
                QMLHandle.sendEvent(EVT.EVT_DUMMY_TRANSACTION_ACTION_ENTER_REQUEST, _input)
            }
        }
        percent: transactionInfo.scan_percent
    }
    QQrExportResult {
        id: qrcodeExportResult
        model: AppModel.qrExported
    }
    FileDialog {
        id: openfileDialog
        fileMode: FileDialog.OpenFile
        onAccepted: {
            var _input = {
                type: "dummy-tx-import",
                file: openfileDialog.file
            }
            QMLHandle.sendEvent(EVT.EVT_DUMMY_TRANSACTION_ACTION_ENTER_REQUEST, _input)
        }
    }
    FileDialog {
        id: savefileDialog
        fileMode: FileDialog.SaveFile
        onAccepted: {
            var _input = {
                type: "dummy-tx-export",
                file: savefileDialog.currentFile
            }
            QMLHandle.sendEvent(EVT.EVT_DUMMY_TRANSACTION_ACTION_ENTER_REQUEST, _input)
        }
    }

    QPopupInfoThreeButtons {
        id: _warning
        property string link:"https://nunchuk.io/"
        title: STR.STR_QML_339
        contentText: STR.STR_QML_1006
        labels: [STR.STR_QML_1004, STR.STR_QML_1005, STR.STR_QML_079]
        funcs: [
            function() {
                signingBusyBox.open()
                timerSigningTx.restart()
            },
            function() {
                QMLHandle.sendEvent(EVT.EVT_DUMMY_TRANSACTION_ACTION_ENTER_REQUEST, {type: "register-wallet"})
            },
            function() {
                closeTransaction()
            }
        ]
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
    Connections {
        target: dummyTx
        onTransactionSignedSuccess: {
            if (flow === AlertType.GROUP_WALLET_SETUP) {
                QMLHandle.sendEvent(EVT.EVT_DUMMY_TRANSACTION_ACTION_ENTER_REQUEST, {type: "accessing-wallet-configuration"})
            }
        }
        onRegisterWalletWithDevice: {
            _warning.open()
        }
    }
    function requestExportViaQR(){
        var _input = {
            type: "dummy-tx-export-qr",
        }
        QMLHandle.sendEvent(EVT.EVT_DUMMY_TRANSACTION_ACTION_ENTER_REQUEST, _input)
        qrcodeExportResult.open()
    }
}
