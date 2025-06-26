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
    id: configWindow
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_437
    property var walletInfo: AppModel.walletInfo
    property string myRole: walletInfo.groupId !== "" ? walletInfo.myRole : ""
    property bool isAlias: (myRole === "KEYHOLDER" || myRole === "MASTER" || myRole === "ADMIN")
    property bool isHotWallet: walletInfo.keyNeedBackup
    extraHeader: Item {}
    onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
    content: Item {
        anchors {
            top: parent.top
            topMargin: 8
        }
        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            propagateComposedEvents: true
            onClicked: {
                configDetails.editFinished()
            }
        }
        Row {
            height: parent.height
            spacing: 24
            QWalletConfigDetails {
                id: configDetails

            }
            QWalletConfigKeys {

            }
        }
    }
    bottomLeft: Item{}
    bottomRight: Row{
        spacing: 12
        QButtonLargeTail {
            id: otheraction
            width: 102
            height: 48
            label: STR.STR_QML_1094
            type: eSECONDARY
            optionVisible: othersContextMenu.visible
            layoutDirection: Qt.RightToLeft
            onButtonClicked: {
                othersContextMenu.x = 20
                othersContextMenu.y = 20 - othersContextMenu.height
                othersContextMenu.open()
            }
            QMultiContextMenu {
                id: othersContextMenu
                property string myRole: walletInfo.myRole
                property bool isAssisted: {
                    var isAss = walletInfo.isAssistedWallet && parseFloat(walletInfo.walletBalance) === 0
                    if (walletInfo.isGroupWallet) {
                        return isAss && (walletInfo.groupId !== "" ? myRole === "MASTER" : true)
                    }
                    else {
                        return isAss
                    }
                }
                property bool isCanDeleted: !walletInfo.isAssistedWallet || isAssisted
                property bool isPrimaryOwner: (myRole === "MASTER" || myRole === "ADMIN") && (walletInfo.ownerMembers.length > 0)
                menuWidth: 300
                property var exportMenu: [
                    {
                        visible: true,
                        label: STR.STR_QML_1483,
                        icon: "",
                        iconRight: "",
                        color: "#031F2B",
                        enable: true,
                        subMenu: null,
                        action: function() {
                            exportDialog.action = function(){
                                walletInfo.requestExportCoinControlData(exportDialog.currentFile)
                            }
                            exportDialog.currentFile = StandardPaths.writableLocation(StandardPaths.DocumentsLocation) + "/"
                                    + RoomWalletData.getValidFilename(walletInfo.walletName)
                                    + "_labels.json"
                            exportDialog.open()
                            othersContextMenu.close()
                        }
                    },
                    {
                        visible: true,
                        label: STR.STR_QML_1484,
                        icon: "",
                        iconRight: "",
                        color: "#031F2B",
                        enable: true,
                        subMenu: null,
                        action: function(){
                            _confirm.action = function(){
                                exportDialog.action = function(){
                                    walletInfo.requestExportBIP329(exportDialog.currentFile)
                                }
                                exportDialog.currentFile = StandardPaths.writableLocation(StandardPaths.DocumentsLocation) + "/"
                                        + RoomWalletData.getValidFilename(walletInfo.walletName)
                                        + "_BIP329labels.json"
                                exportDialog.open()
                                othersContextMenu.close()
                            }
                            _confirm.contentText = STR.STR_QML_1485
                            _confirm.open()
                        }
                    },
                ]
                property var importMenu: [
                    {
                        visible: true,
                        label: STR.STR_QML_1483,
                        icon: "",
                        iconRight: "",
                        color: "#031F2B",
                        enable: true,
                        subMenu: null,
                        action: function() {
                            _confirm.action = function(){
                                importDialog.action = function(){
                                    walletInfo.requestImportCoinControlData(importDialog.currentFile)
                                }
                                importDialog.open()
                                othersContextMenu.close()
                            }
                            _confirm.contentText = STR.STR_QML_1515
                            _confirm.open()
                        }
                    },
                    {
                        visible: true,
                        label: STR.STR_QML_1484,
                        icon: "",
                        iconRight: "",
                        color: "#031F2B",
                        enable: true,
                        subMenu: null,
                        action: function(){
                            _confirm.action = function(){
                                importDialog.action = function(){
                                    walletInfo.requestImportBIP329(importDialog.currentFile)
                                }
                                importDialog.open()
                                othersContextMenu.close()
                            }
                            _confirm.contentText = STR.STR_QML_1515
                            _confirm.open()
                        }
                    },
                ]
                mapMenu: [
                    {
                        visible: myRole !== "FACILITATOR_ADMIN",
                        label: STR.STR_QML_532,
                        icon: "qrc:/Images/Images/importFile.svg",
                        iconRight: "",
                        color: "#031F2B",
                        enable: myRole !== "FACILITATOR_ADMIN",
                        subMenu: null,
                        action: function(){ openfileDialog.open(); }
                    },
                    {
                        visible: true,
                        label: STR.STR_QML_1481,
                        icon: "qrc:/Images/Images/ExportFile.svg",
                        iconRight: "qrc:/Images/Images/right-arrow-dark.svg",
                        color: "#031F2B",
                        enable: true,
                        subMenu: exportMenu,
                        action: function(){ console.warn("export") }
                    },
                    {
                        visible: true,
                        label: STR.STR_QML_1482,
                        icon: "qrc:/Images/Images/importFile.svg",
                        iconRight: "qrc:/Images/Images/right-arrow-dark.svg",
                        color: "#031F2B",
                        enable: true,
                        subMenu: importMenu,
                        action: function(){ console.warn("import") }
                    },
                    {
                        visible: myRole !== "FACILITATOR_ADMIN",
                        label: STR.STR_QML_312,
                        icon: "qrc:/Images/Images/settings-dark.svg",
                        iconRight: "",
                        color: "#031F2B",
                        enable: myRole !== "FACILITATOR_ADMIN",
                        subMenu: null,
                        action: function(){ QMLHandle.sendEvent(EVT.EVT_WALLET_INFO_UTXOS_REQUEST); }
                    },
                    {
                        visible: myRole !== "FACILITATOR_ADMIN",
                        label: STR.STR_QML_198,
                        icon: "qrc:/Images/Images/received-dark.svg",
                        iconRight: "",
                        color: "#031F2B",
                        enable: myRole !== "FACILITATOR_ADMIN",
                        subMenu: null,
                        action: function(){ QMLHandle.sendEvent(EVT.EVT_WALLET_INFO_CHANGE_ADDRESS_REQUEST); }
                    },
                    {
                        visible: true,
                        label: STR.STR_QML_686,
                        icon: "qrc:/Images/Images/cached_24px.png",
                        iconRight: "",
                        color: "#031F2B",
                        enable: true,
                        subMenu: null,
                        action: function(){ forceRefresh.open(); }
                    },
                    {
                        visible: myRole !== "FACILITATOR_ADMIN",
                        label: STR.STR_QML_825,
                        icon: "qrc:/Images/Images/settings-dark.svg",
                        iconRight: "",
                        color: "#031F2B",
                        enable: myRole !== "FACILITATOR_ADMIN",
                        subMenu: null,
                        action: function(){ gaplimit.open(); }
                    },
                    {
                        visible: isPrimaryOwner,
                        label: STR.STR_QML_1193,
                        icon: "qrc:/Images/Images/account-member-dark.svg",
                        iconRight: "",
                        color: "#031F2B",
                        enable: true,
                        subMenu: null,
                        action: function(){ eFlow = eWALLET_PRIMARY_OWNER }
                    },
                    {
                        visible: myRole !== "FACILITATOR_ADMIN",
                        label: STR.STR_QML_1363,
                        icon: "qrc:/Images/Images/key-dark.svg",
                        iconRight: "",
                        color: "#031F2B",
                        enable: myRole !== "FACILITATOR_ADMIN",
                        subMenu: null,
                        action: function(){
                            if (!walletInfo.isAssistedWallet) { // non assisted wallet acount
                                QMLHandle.sendEvent(EVT.EVT_REPLACE_KEYS_REQUEST, {isFirst: true})
                            } else {
                                _info.open()
                            }
                        }
                    },
                    {
                        visible: !walletInfo.isAssistedWallet,
                        label: (walletInfo.isArchived ? STR.STR_QML_1728 : STR.STR_QML_1727),
                        icon: (walletInfo.isArchived ? "qrc:/Images/Images/Archived1.png" : "qrc:/Images/Images/Archived2.png"),
                        iconRight: "",
                        color: "#031F2B",
                        enable: !walletInfo.isAssistedWallet,
                        subMenu: null,
                        action: function(){
                            othersContextMenu.close()
                            walletInfo.isArchived = !walletInfo.isArchived
                        }
                    },
                    {
                        visible: isCanDeleted,
                        label: STR.STR_QML_332,
                        icon: "qrc:/Images/Images/Delete.svg",
                        iconRight: "",
                        color: "#CF4018",
                        enable: true,
                        subMenu: null,
                        action: function(){
                            if (isAssisted) {
                                QMLHandle.sendEvent(EVT.EVT_WALLET_INFO_REMOVE, walletInfo)
                            } else if (isHotWallet) {
                                if(walletInfo.allowBackup()) {
                                    _infoHotWallet.open();
                                } else {
                                    modelConfirmDelete.open();
                                }                                
                            } else {
                                modelConfirmDelete.open();
                            }
                        }
                    }
                ]
            }
        }
        QButtonLargeTail {
            id: exportwallet
            width: 111
            height: 48
            label: STR.STR_QML_323
            type: eSECONDARY
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
                    {
                        visible: true,
                        label: STR.STR_QML_324, // As BSMS
                        icon: "",
                        iconRight: "",
                        color: "#031F2B",
                        enable: true,
                        subMenu: null,
                        action: function() {
                            // Export Wallet BSMS File
                            exportContextMenu.close()
                            exportDialog.action = function(){
                                walletInfo.requestExportWalletViaBSMS(exportDialog.currentFile)
                            }
                            exportDialog.currentFile = StandardPaths.writableLocation(StandardPaths.DocumentsLocation) + "/"
                                    + RoomWalletData.getValidFilename(walletInfo.walletName)
                                    + ".bsms"
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
                            exportContextMenu.close()
                            // "Export via BBQR"
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
                            exportContextMenu.close()
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
                            exportContextMenu.close()
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
                        enable: true,
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
                        enable: true,
                        subMenu: null,
                        action: function(){
                            // "Export UTXOs (CSV)"
                            exportDialog.action = function(){
                                walletInfo.requestExportUtxoCSV(exportDialog.currentFile)
                            }
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

    QConfirmDeletingTypeA {
        id: modelConfirmDelete
        modalWidth: 488
        modalHeight: 317
        label: STR.STR_QML_334
        content: STR.STR_QML_335
        onDeleteRequest: {
            if(walletInfo.isSharedWallet){
                confirmDeleteWallet.open()
            }
            else{
                QMLHandle.sendEvent(EVT.EVT_WALLET_INFO_REMOVE, walletInfo)
            }
        }
    }
    QPopupInfo{
        id:_info
        title: STR.STR_QML_339
        contentText: STR.STR_QML_1391
        usingMin: true
    }
    QPopupInfo{
        id:_infoHotWallet
        title: STR.STR_QML_339
        contentText: STR.STR_QML_1770
        usingMin: true
    }

    QConfirmYesNoPopup {
        id: _confirm
        title: STR.STR_QML_334
        property var action
        onConfirmNo: close()
        onConfirmYes: {
            close()
            if(action) {
                action()
            }
        }
    }

    FileDialog {
        id: importDialog
        property var action
        fileMode: FileDialog.OpenFile
        onAccepted: {
            if (action) {
                action()
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
}
