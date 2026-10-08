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
import DataPool 1.0
import NUNCHUCKTYPE 1.0
import Features.Transactions.ViewModels 1.0
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Chats"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../Components/customizes/Transactions"
import "../../Components/customizes/Popups"
import "../../Components/customizes/QRCodes"

QOnScreenContentTypeA {
    id:_content
    property var transactionInfo: vm.transactionInfo
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_785
    readonly property int confirmations: Math.max(0, (AppModel.blockHeight - transactionInfo.height) + 1)
    extraHeader: QBadge {
        height: 24
        color: GlobalData.transactionColor(transactionInfo.status)
        text: GlobalData.transactionStatus(transactionInfo.status, confirmations)
    }
    onCloseClicked: vm.close()
    content: QSendTransaction {
        walletInfo: vm.walletInfo
        transactionInfo: _content.transactionInfo
        myRole: vm.walletInfo.myRole
        pendingSignature: vm.transactionInfo.pendingSignatures
        isDummy: vm.transactionInfo.isClaimTx || vm.transactionInfo.isDummyTx
        onAddrToVerify: {
            displayAddressBusybox.addrToVerify = addr
            vm.verifyAddress(addr)
        }
        onNewMemoNotify: {
            vm.setMemo(newMemo)
        }
        signerEvents: {
            "sign": function(signer) { 
                signingBusyBox.signerType = signer.single_signer_type
                signingBusyBox.open()
                timerSigningTx.fingerPrint = signer.singleSigner_masterFingerPrint
                timerSigningTx.restart()
            },
            "scan": function(signer) { 
                vm.scanDevice()
            },
            "exportQr": function(signer) { 
                requestExportViaQR()
            },
            "exportBBQR": function(signer) { 
                requestExportViaBBQR()
            },
            "exportFile": function(signer) { 
                requestExportPSBT()
            },
            "importQr": function(signer) { 
                qrcodeImport.open()
            },
            "importFile": function(signer) { 
                openfileDialog.open()
            }
        }
    }
    bottomLeft: Item {}
    bottomRight: Row {
        spacing: 12
        QButtonLargeTail {
            id: advancedBtn
            width: 240
            height: 48
            type: eSECONDARY
            label: STR.STR_QML_299
            optionVisible: exportContextMenu.visible
            onButtonClicked: {
                exportContextMenu.x = 20
                exportContextMenu.y = 20 - exportContextMenu.height
                exportContextMenu.open()
            }                
            QMultiContextMenu {
                id: exportContextMenu
                menuWidth: 300
                subMenuWidth: 300
                property var exportMessage: [
                    {
                        visible: true,
                        label: STR.STR_QML_114,
                        icon: "qrc:/Images/Images/ExportFile.svg",
                        iconRight: "",
                        color: "#031F2B",
                        enable: true,
                        subMenu: null,
                        action: function() {
                            exportContextMenu.close()
                            requestExportViaQR()
                        }
                    },
                    {
                        visible: true,
                        label: STR.STR_QML_1531,
                        icon: "qrc:/Images/Images/ExportFile.svg",
                        iconRight: "",
                        color: "#031F2B",
                        enable: true,
                        subMenu: null,
                        action: function() {
                            exportContextMenu.close()
                            requestExportViaBBQR()
                        }
                    },
                    {
                        visible: true,
                        label: STR.STR_QML_2097,
                        icon: "qrc:/Images/Images/ExportFile.svg",
                        iconRight: "",
                        color: "#031F2B",
                        enable: true,
                        subMenu: null,
                        action: function() {
                            exportContextMenu.close()
                            requestExportPSBT()
                        }
                    }
                ]                 
                property var importMessage: [
                    {
                        visible: true,
                        label: STR.STR_QML_302,
                        icon: "qrc:/Images/Images/importFile.svg",
                        iconRight: "",
                        color: "#031F2B",
                        enable: true,
                        subMenu: null,
                        action: function() {
                            exportContextMenu.close()
                            qrcodeImport.open()
                        }
                    },
                    {
                        visible: true,
                        label: STR.STR_QML_677,
                        icon: "qrc:/Images/Images/importFile.svg",
                        iconRight: "",
                        color: "#031F2B",
                        enable: true,
                        subMenu: null,
                        action: function() {
                            exportContextMenu.close()
                            openfileDialog.open()
                        }
                    }
                ]
                mapMenu: [
                    {
                        visible: true,
                        label: STR.STR_QML_294,
                        icon: "qrc:/Images/Images/ExportFile.svg",
                        iconRight: "qrc:/Images/Images/right-arrow-dark.svg",
                        color: "#031F2B",
                        enable: true,
                        subMenu: exportMessage,
                        action: function() {
                        }
                    },
                    {
                        visible: true,
                        label: STR.STR_QML_252,
                        icon: "qrc:/Images/Images/importFile.svg",
                        iconRight: "qrc:/Images/Images/right-arrow-dark.svg",
                        color: "#031F2B",
                        enable: true,
                        subMenu: importMessage,
                        action: function() {
                        }
                    }
                ]
            }
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
                vm.txBroadcast()
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
                vm.txSignRequest(timerSigningTx.fingerPrint)
            }
        }
    }
    QQrImportScanner {
        id: qrcodeImport
        onTagFound: {
            if(qrcodeImport.complete) {
                vm.txSignImportQr(qrcodeImport.tags)
                qrcodeImport.close();
            }
        }
    }
    QQrExportResultPDF {
        id: qrcodeExportResult
        model: vm.qrExported
    }
    FileDialog {
        id: openfileDialog
        fileMode: FileDialog.OpenFile
        onAccepted: {
            vm.txSignImportFile(openfileDialog.file)
        }
    }
    FileDialog {
        id: savefileDialog
        fileMode: FileDialog.SaveFile
        onAccepted: {
            vm.txSignExportFile(savefileDialog.currentFile)
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
                
            },
            function() {
                vm.close()
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
        vm.txSignExportQr()
        qrcodeExportResult.filename = "Transaction_" + transactionInfo.txid
        qrcodeExportResult.open()
    }
    function requestExportViaBBQR() {
        vm.txSignExportBBQr()
        qrcodeExportResult.filename = "Transaction_bbqr_" + transactionInfo.txid
        qrcodeExportResult.open()
    }

    TransactionDetailsViewModel {
        id: vm
    }
    Connections {
        target: vm
        onFinishedSigningTransaction: {
            signingBusyBox.close()
            signingBusyBox.signerType = -1
        }
    }
}
