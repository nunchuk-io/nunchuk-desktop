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
    property var transactionInfo: AppModel.transactionInfo
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_785
    extraHeader: QBadge {
        color: GlobalData.transactionColor(transactionInfo.status)
        text: GlobalData.transactionStatus(transactionInfo.status, confirmations)
    }
    onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
    content: QSendTransaction {
        walletInfo: ServiceSetting.walletInfo
        transactionInfo: AppModel.transactionInfo
        myRole: ServiceSetting.walletInfo.myRole
        isDummy: true
        onAddrToVerify: {
            displayAddressBusybox.addrToVerify = addr
            var _address = {
                type: "address-to-verify",
                address: addr
            }
            QMLHandle.sendEvent(EVT.EVT_INHERITANCE_TRANSACTION_DETAILS_ACTION, _address)
        }
        onNewMemoNotify: {
            var _memo = {
                type: "memo-notify",
                memo: newMemo
            }
            QMLHandle.sendEvent(EVT.EVT_INHERITANCE_TRANSACTION_DETAILS_ACTION, _memo)
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
            QMLHandle.sendEvent(EVT.EVT_INHERITANCE_TRANSACTION_DETAILS_ACTION, _scanDevice )
        }
        onKeyExportRequest: {
            requestExportPSBT()
        }
        onKeyImportRequest: {
            openfileDialog.open()
        }        
    }
    bottomLeft: Item {}
    bottomRight: Row {
        spacing: 12
        QBtnExportImport {
            id: advancedBtn
            funcs: [
                function() {
                    requestExportPSBT()
                },
                function() {
                    requestExportViaQR()
                },
                function() {
                    requestExportViaBBQR()
                },
                function() {
                    openfileDialog.open()
                },
                function() {
                    qrcodeImport.open()
                }
            ]
        }
        QTextButton {
            id: startbroatcast
            width: 180
            height: 48
            label.text: STR.STR_QML_497
            label.font.pixelSize: 16
            type: eTypeE
            enabled: transactionInfo.timelockedUntil.enabledBroadcastTxBtn
            visible: transactionInfo.status === NUNCHUCKTYPE.READY_TO_BROADCAST
            onButtonClicked: {
                var _boardcast = {
                    type: "tx-broadcast",
                }
                QMLHandle.sendEvent(EVT.EVT_INHERITANCE_TRANSACTION_DETAILS_ACTION, _boardcast)
            }
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

    Timer {
        id: timerSigningTx
        property string fingerPrint : ""
        interval: 500
        onTriggered: {
            if(timerSigningTx.fingerPrint !== "") {
                var sign = {
                    type: "claim-tx-sign",
                    xfp: timerSigningTx.fingerPrint
                }
                QMLHandle.sendEvent(EVT.EVT_INHERITANCE_TRANSACTION_DETAILS_ACTION, sign)
            }
        }
    }
    QQrImportScanner {
        id: qrcodeImport
        onTagFound: {
            if(qrcodeImport.complete){
                var _input = {
                    type: "claim-tx-import-qr",
                    tags: qrcodeImport.tags
                }
                qrcodeImport.close();
                QMLHandle.sendEvent(EVT.EVT_INHERITANCE_TRANSACTION_DETAILS_ACTION, _input)
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
            var _input = {
                type: "claim-tx-import",
                file: openfileDialog.file
            }
            QMLHandle.sendEvent(EVT.EVT_INHERITANCE_TRANSACTION_DETAILS_ACTION, _input)
        }
    }
    FileDialog {
        id: savefileDialog
        fileMode: FileDialog.SaveFile
        onAccepted: {
            var _input = {
                type: "claim-tx-export",
                file: savefileDialog.currentFile
            }
            QMLHandle.sendEvent(EVT.EVT_INHERITANCE_TRANSACTION_DETAILS_ACTION, _input)
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
                QMLHandle.sendEvent(EVT.EVT_INHERITANCE_TRANSACTION_DETAILS_ACTION, {type: "register-wallet"})
            },
            function() {
                closeTo(NUNCHUCKTYPE.CURRENT_TAB)
            }
        ]
    }

    Connections {
        target: AppModel
        function onStartDisplayAddress(wallet_id, address) {
            if(isOnTop) displayAddressBusybox.open()
            else displayAddressBusybox.close()
        }
        function onFinishedDisplayAddress(result) {
            displayAddressBusybox.close()
        }
        function onFinishedSigningTransaction() {
            signingBusyBox.close()
            signingBusyBox.signerType = -1
        }
    }
    Connections {
        target: dummyTx
        onRegisterWalletWithDevice: {
            _warning.open()
        }
    }
    function requestExportPSBT() {
        savefileDialog.currentFile = StandardPaths.writableLocation(StandardPaths.DocumentsLocation) + "/"
                + RoomWalletData.getValidFilename(transactionInfo.txid)
                + ".psbt"
        savefileDialog.open()
    }
    function requestExportViaQR() {
        var _input = {
            type: "claim-tx-export-qr",
        }
        QMLHandle.sendEvent(EVT.EVT_INHERITANCE_TRANSACTION_DETAILS_ACTION, _input)
        qrcodeExportResult.filename = "Transaction_" + transactionInfo.txid
        qrcodeExportResult.open()
    }
    function requestExportViaBBQR() {
        var _input = {
            type: "claim-tx-export-bbqr",
        }
        QMLHandle.sendEvent(EVT.EVT_INHERITANCE_TRANSACTION_DETAILS_ACTION, _input)
        qrcodeExportResult.filename = "Transaction_bbqr_" + transactionInfo.txid
        qrcodeExportResult.open()
    }
}
