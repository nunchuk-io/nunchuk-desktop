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
import Features.Wallets.ViewModels 1.0
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Chats"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../Components/customizes/QRCodes"
import "../../../../localization/STR_QML.js" as STR

QOnScreenContentTypeB {
    id: root
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: ""
    extraHeader: Item {}
    onCloseClicked: {
        reqCloseTo(NUNCHUCKTYPE.CURRENT_TAB)
        vm.close();
    }
    function reqCloseTo(tab) {
        if (typeof(closeTo) === "function")
            closeTo(tab);
    }
    property var walletInfo: AppModel.walletInfo
    Item {
        anchors {
            left: parent.left
            leftMargin: 36
            top: parent.top
            topMargin: 36
        }

        width: 728
        height: 551
        Column {
            width: parent.width
            spacing: 24
            Rectangle {
                width: 96;height: 96;
                radius: 48
                color: "#F5F5F5"
                QIcon {
                    iconSize: 60
                    anchors.centerIn: parent
                    source: "qrc:/Images/Images/Wallet-registration.svg"
                }
            }
            QLato {
                width: 550
                height: 40
                text: STR.STR_QML_1647
                font.pixelSize: 32
                font.weight: Font.Medium
                verticalAlignment: Text.AlignVCenter
            }
            QLato {
                width: 550
                text: STR.STR_QML_1648
                verticalAlignment: Text.AlignVCenter
                lineHeightMode: Text.FixedHeight
                lineHeight: 28
                wrapMode: Text.WordWrap
            }
            QLato {
                width: 550
                text: STR.STR_QML_2064
                verticalAlignment: Text.AlignVCenter
                lineHeightMode: Text.FixedHeight
                lineHeight: 28
                wrapMode: Text.WordWrap
                visible: vm.event == RegisterWalletOnHardwareViewModel.WithdrawBitcoin
            }
            QWarningBgMulti {
                width: 528
                visible: vm.event == RegisterWalletOnHardwareViewModel.WithdrawBitcoin
                height: 64
                icon: ""
                txt.text: STR.STR_QML_2063
            }
        }
        
    }
    onPrevClicked: {
        reqCloseTo(NUNCHUCKTYPE.CURRENT_TAB)
        vm.back()
    }
    bottomRight: Row {
        spacing: 12
        QTextButton {
            width: 138
            height: 48
            label.text: STR.STR_QML_079
            label.font.pixelSize: 16
            type: eTypeB
            onButtonClicked: {
                reqCloseTo(NUNCHUCKTYPE.CURRENT_TAB)
                vm.cancel()
            }
        }
        QButtonLargeTail {
            id: exportwallet
            width: 211
            height: 48
            type: eTypeE
            label: STR.STR_QML_1649
            optionVisible: exportContextMenu.visible
            layoutDirection: Qt.RightToLeft
            onButtonClicked: {
                exportContextMenu.x = 20
                exportContextMenu.y = 20 - exportContextMenu.height
                exportContextMenu.open()
            }

            QMultiContextMenu {
                id: exportContextMenu
                menuWidth: 300
                subMenuWidth: 300
                property var exportConfigurationMenu: [
                    // {
                    //     visible: true,
                    //     label: STR.STR_QML_324, // As BSMS
                    //     icon: "",
                    //     iconRight: "",
                    //     color: "#031F2B",
                    //     enable: true,
                    //     subMenu: null,
                    //     action: function() {
                    //         // Export Wallet BSMS File
                    //         exportContextMenu.close()
                    //         exportDialog.action = function(){
                    //             walletInfo.requestExportWalletViaBSMS(exportDialog.currentFile)
                    //         }
                    //         exportDialog.currentFile = StandardPaths.writableLocation(StandardPaths.DocumentsLocation) + "/"
                    //                 + RoomWalletData.getValidFilename(walletInfo.walletName)
                    //                 + ".bsms"
                    //         exportDialog.open()
                    //     }
                    // },
                    {
                        visible: true,
                        label: STR.STR_QML_1869, // As descriptor
                        icon: "",
                        iconRight: "",
                        color: "#031F2B",
                        enable: true,
                        subMenu: null,
                        action: function() {
                            exportContextMenu.close()
                            exportDialog.action = function(){
                                walletInfo.requestViaDescriptor(exportDialog.currentFile)
                            }
                            exportDialog.currentFile = StandardPaths.writableLocation(StandardPaths.DocumentsLocation) + "/"
                                    + RoomWalletData.getValidFilename(walletInfo.walletName)
                                    + ".txt"
                            exportDialog.open()                            
                        }
                    },
                    {
                        visible: true,
                        label: STR.STR_QML_1747, // As BBQR
                        icon: "",
                        iconRight: "",
                        color: "#031F2B",
                        enable: true,
                        subMenu: null,
                        action: function() {
                            // "Export via BBQR"
                            exportContextMenu.close()
                            qrcodeExportResult.filename = RoomWalletData.getValidFilename(walletInfo.walletName) + "_BBQR"
                            qrcodeExportResult.open()
                            walletInfo.requestExportWalletViaQRBBQRColdcard()
                        }
                    },
                    {
                        visible: true,
                        label: STR.STR_QML_1750, // As BC-UR2 QR (legacy)
                        icon: "",
                        iconRight: "",
                        color: "#031F2B",
                        enable: true,
                        subMenu: null,
                        action: function() {
                            // "Export as BC-UR2 QR (legacy)"
                            exportContextMenu.close()
                            qrcodeExportResult.filename = RoomWalletData.getValidFilename(walletInfo.walletName) + "_BCUR2_Legacy"
                            qrcodeExportResult.open()
                            walletInfo.requestExportWalletViaQRBCUR2Legacy()
                        }
                    },
                    {
                        visible: true,
                        label: STR.STR_QML_1751, // As BC-UR2 QR
                        icon: "",
                        iconRight: "",
                        color: "#031F2B",
                        enable: true,
                        subMenu: null,
                        action: function() {
                            // "Export as BC-UR2 QR"
                            exportContextMenu.close()
                            qrcodeExportResult.filename = RoomWalletData.getValidFilename(walletInfo.walletName) + "_BCUR2"
                            qrcodeExportResult.open()
                            walletInfo.requestExportWalletViaQRBCUR2()
                        }
                    },
                    {
                        visible: true,
                        label: STR.STR_QML_1748, // To COLDCARD
                        icon: "",
                        iconRight: "",
                        color: "#031F2B",
                        enable: true,
                        subMenu: null,
                        action: function() {
                            // "Export To Coldcard"
                            exportContextMenu.close()
                            exportDialog.action = function(){
                                walletInfo.requestExportWalletViaCOLDCARD(exportDialog.currentFile)
                            }
                            exportDialog.currentFile = StandardPaths.writableLocation(StandardPaths.DocumentsLocation) + "/"
                                    + RoomWalletData.getValidFilename(walletInfo.walletName)
                                    + "-Coldcard-Config.txt"
                            exportDialog.open()
                        }
                    },
                    {
                        visible: true,
                        label: STR.STR_QML_1749, // To BitBox
                        icon: "",
                        iconRight: "",
                        color: "#031F2B",
                        enable: true,
                        subMenu: null,
                        action: function() {
                            //"Export wallet to Bitbox"
                            exportContextMenu.close()
                            var addrs = walletInfo.walletunUsedAddressList;
                            if(addrs.length > 0){
                                displayAddressBusybox.addrToVerify = addrs[0]
                                AppModel.startDisplayAddress(walletInfo.walletId, addrs[0])
                                walletInfo.needRegistered = true
                            }
                        }
                    }
                ]
                property var exportTransactionMenu: [
                    {
                        visible: true,
                        label: STR.STR_QML_1713,
                        icon: "",
                        iconRight: "",
                        color: "#031F2B",
                        enable: true,
                        subMenu: null,
                        action: function() { // "Export PDF invoices"
                            var currentDate = new Date();
                            var monthNames = ["JAN", "FEB", "MAR", "APR", "MAY", "JUN",
                                              "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"];
                            var day = currentDate.getDate();
                            var month = monthNames[currentDate.getMonth()];
                            var year = currentDate.getFullYear();
                            var today = day + '' + month + '' + year;
                            exportDialog.action = function(){
                                walletInfo.requestExportTransactionViaPDF(exportDialog.currentFile)
                            }
                            exportContextMenu.close()
                            exportDialog.currentFile = StandardPaths.writableLocation(StandardPaths.DocumentsLocation) + "/"
                                                            + RoomWalletData.getValidFilename(walletInfo.walletName)
                                                            + "_transaction_history_"
                                                            + today
                                                            + ".pdf"
                            exportDialog.open()
                        }
                    },
                    {
                        visible: true,
                        label: STR.STR_QML_1714,
                        icon: "",
                        iconRight: "",
                        color: "#031F2B",
                        enable: true,
                        subMenu: null,
                        action: function(){
                            var currentDate = new Date();
                            var monthNames = ["JAN", "FEB", "MAR", "APR", "MAY", "JUN",
                                              "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"];
                            var day = currentDate.getDate();
                            var month = monthNames[currentDate.getMonth()];
                            var year = currentDate.getFullYear();
                            var today = day + '' + month + '' + year;
                            exportDialog.action = function(){
                                walletInfo.requestExportTransactionViaCSV(exportDialog.currentFile)
                            }
                            exportContextMenu.close()
                            exportDialog.currentFile = StandardPaths.writableLocation(StandardPaths.DocumentsLocation) + "/"
                                                            + RoomWalletData.getValidFilename(walletInfo.walletName)
                                                            + "_transaction_history_"
                                                            + today
                                                            + ".csv"
                            exportDialog.open()
                        }
                    },
                ]
                mapMenu: [
                    {
                        visible: true,
                        label: STR.STR_QML_347,
                        icon: "qrc:/Images/Images/save-backup-dark.svg",
                        iconRight: "qrc:/Images/Images/right-arrow-dark.svg",
                        color: "#031F2B",
                        enable: true,
                        subMenu: exportConfigurationMenu,
                        action: function(){

                        }
                    },
                    {
                        visible: true,
                        label: STR.STR_QML_325,
                        icon: "qrc:/Images/Images/ExportFile.svg",
                        iconRight: "",
                        color: "#031F2B",
                        enable: true,
                        subMenu: null,
                        action: function(){
                            // Export Wallet Database
                            exportDialog.action = function(){
                                walletInfo.requestExportWalletViaDB(exportDialog.currentFile)
                            }
                            exportContextMenu.close()
                            exportDialog.currentFile = StandardPaths.writableLocation(StandardPaths.DocumentsLocation)
                                    + "/"
                                    + RoomWalletData.getValidFilename(walletInfo.walletName)
                                    + "-database.dat"
                            exportDialog.open()
                        }
                    },
                    {
                        visible: true,
                        label: STR.STR_QML_326,
                        icon: "qrc:/Images/Images/ExportFile.svg",
                        iconRight: "qrc:/Images/Images/right-arrow-dark.svg",
                        color: "#031F2B",
                        enable: false,
                        subMenu: exportTransactionMenu,
                        action: function(){

                        }
                    },
                    {
                        visible: true,
                        label: STR.STR_QML_327,
                        icon: "qrc:/Images/Images/ExportFile.svg",
                        iconRight: "",
                        color: "#031F2B",
                        enable: false,
                        subMenu: null,
                        action: function(){
                            // "Export UTXOs (CSV)"
                            exportDialog.action = function(){
                                walletInfo.requestExportUtxoCSV(exportDialog.currentFile)
                            }
                            exportContextMenu.close()
                            exportDialog.currentFile = StandardPaths.writableLocation(StandardPaths.DocumentsLocation) + "/"
                                    + RoomWalletData.getValidFilename(walletInfo.walletName)
                                    + "-utxos.csv"
                            exportDialog.open()
                        }
                    }
                ]
            }
        }
    }
    FileDialog {
        id: exportDialog
        property var action
        fileMode: FileDialog.SaveFile
        onAccepted: {
            if (action) {
                action()
            }
        }
    }
    QQrExportResultPDF {
        id: qrcodeExportResult
        model: AppModel.qrExported
    }
    RegisterWalletOnHardwareViewModel {
        id: vm
        Component.onCompleted: {
            vm.attachContext(vmContext)
            FlowManager.currentFlow.bind(vm)
            if (vm.event == RegisterWalletOnHardwareViewModel.WithdrawBitcoin) {
                root.walletInfo = vm.walletInfo
            }            
        }
    }
}
