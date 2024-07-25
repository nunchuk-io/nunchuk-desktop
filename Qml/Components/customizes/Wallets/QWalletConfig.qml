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

QOnScreenContent {
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_437
    property var walletInfo: AppModel.walletInfo
    property string myRole: walletInfo.groupId !== "" ? walletInfo.myRole : ""
    property bool isAlias: (myRole === "KEYHOLDER" || myRole === "MASTER" || myRole === "ADMIN")
    extraHeader: Row {
        spacing: 8
        QBadge {
            text: STR.STR_QML_559.arg(walletInfo.walletM).arg(walletInfo.walletN)
            color: "#EAEAEA"
            font.weight: Font.Normal
            anchors.verticalCenter: parent.verticalCenter
        }
    }
    onCloseClicked: closeTo(NUNCHUCKTYPE.WALLET_TAB)
    content: Item {
        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            propagateComposedEvents: true
            onClicked: {
                wlname.typingFinish()
            }
        }
        Row {
            anchors.fill: parent
            spacing: 28
            Rectangle {
                width: 350
                height: 480
                color: "#F5F5F5"
                radius: 12
                Column {
                    anchors {
                        fill: parent
                        topMargin: 18
                        leftMargin: 18
                        bottomMargin: 18
                        rightMargin: 6
                    }
                    spacing: 8
                    Rectangle {
                        width: 326
                        height: wlname.childrenRect.height
                        color: "transparent"
                        QTextInputBoxTypeA {
                            id: wlname
                            width: 326
                            placeholderText: ""
                            text: AppModel.walletInfo.walletName
                            backgroundColor: "Transparent"
                            borderColor: "Transparent"
                            color: "#000000"
                            font.family: "Montserrat"
                            font.pixelSize: 16
                            anchors.horizontalCenter: parent.horizontalCenter
                            showEdit: wlname.enabled
                            enabled: {
                                if (AppModel.walletInfo.isGroupWallet) {
                                    return AppModel.walletInfo.myRole === "MASTER"
                                }
                                else {
                                    return true
                                }
                            }

                            onTypingFinished: {
                                if(currentText !== AppModel.walletInfo.walletName) {
                                    var infoObj = {
                                        "name"          : currentText,
                                        "description"   : ""
                                    };
                                    QMLHandle.sendEvent(EVT.EVT_WALLET_INFO_EDIT_NAME, infoObj)
                                }
                            }
                        }
                    }

                    Column {
                        width: 310
                        height: 300
                        spacing: 24
                        Item {
                            width: parent.width
                            height: 48
                            visible: walletInfo.aliasName === "" && isAlias
                            Row {
                                anchors{
                                    left: parent.left
                                    leftMargin: 0
                                    verticalCenter: parent.verticalCenter
                                }
                                spacing: 6
                                QLato {
                                    text: STR.STR_QML_1185
                                    font.weight: Font.Bold
                                }
                                QIconButton {
                                    iconSize: 24
                                    icon: "qrc:/Images/Images/right-arrow-dark.svg"
                                    onButtonClicked: {
                                        eFlow = eWALLET_SET_ALIAS
                                    }
                                    bgColor: "transparent"
                                }
                            }
                        }
                        Item {
                            width: parent.width
                            height: 48
                            visible: walletInfo.aliasName !== "" && isAlias
                            Row {
                                anchors{
                                    left: parent.left
                                    leftMargin: 0
                                    verticalCenter: parent.verticalCenter
                                }
                                spacing: 6
                                QLato {
                                    text: STR.STR_QML_1190
                                    font.weight: Font.Bold
                                }
                                QIconButton {
                                    iconSize: 24
                                    icon: "qrc:/Images/Images/right-arrow-dark.svg"
                                    onButtonClicked: {
                                        eFlow = eWALLET_CHANGE_ALIAS
                                    }
                                    bgColor: "transparent"
                                }
                            }
                        }
                        Item {
                            width: parent.width
                            height: 28
                            QLato {
                                anchors{
                                    left: parent.left
                                    leftMargin: 0
                                    verticalCenter: parent.verticalCenter
                                }
                                text: STR.STR_QML_436
                                font.weight: Font.Normal
                            }
                            QLato {
                                anchors{
                                    right: parent.right
                                    rightMargin: 0
                                    verticalCenter: parent.verticalCenter
                                }
                                text: walletInfo.walletEscrow ? STR.STR_QML_029 : STR.STR_QML_028
                                font.weight: Font.Bold
                            }
                        }
                        Item {
                            id: addresstype
                            width: parent.width
                            height: 28
                            readonly property var addressType_Value: [
                                STR.STR_QML_065,
                                STR.STR_QML_064,
                                STR.STR_QML_063,
                                STR.STR_QML_062,
                                STR.STR_QML_553
                            ]
                            QLato {
                                anchors{
                                    left: parent.left
                                    leftMargin: 0
                                    verticalCenter: parent.verticalCenter
                                }
                                text: STR.STR_QML_319
                                font.weight: Font.Normal
                            }
                            QLato {
                                anchors{
                                    right: parent.right
                                    rightMargin: 0
                                    verticalCenter: parent.verticalCenter
                                }
                                text: addresstype.addressType_Value[walletInfo.walletAddressType]
                                font.weight: Font.Bold
                            }
                        }
                    }
                }
            }
            Item {
                width: 350
                height: 480
                Column {
                    anchors.fill: parent
                    spacing: 8
                    QLato {
                        width: 97
                        height: 20
                        font.weight: Font.Bold
                        text: STR.STR_QML_289
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                    Rectangle {
                        width: 350
                        height: 452
                        border.width: 1
                        border.color: "#EAEAEA"
                        radius: 12
                        QListView {
                            id: signerlist
                            anchors.fill: parent
                            anchors.margins: 12
                            spacing: 16
                            model: walletInfo.walletSingleSignerAssigned
                            ScrollBar.vertical: ScrollBar { active: true }
                            delegate: QWalletKeyDelegate {
                                width: signerlist.width
                                signerName: model.singleSigner_name
                                signerXFP: model.singleSigner_masterFingerPrint
                                signerType: model.single_signer_type
                                deviceType: model.single_signer_devicetype
                                tagType: model.single_signer_tag
                                card_id: model.single_signer_device_cardid
                                accountIndex: model.single_signer_account_index
                                onViewPoliciesRequest: {
                                    var objectRequest = {
                                        "requestType": "viewPolicies",
                                        "requestData": AppModel.walletInfo.walletId
                                    }
                                    QMLHandle.sendEvent(EVT.EVT_WALLET_INFO_SIGNER_INFO_REQUEST, objectRequest)
                                }
                            }
                        }
                    }
                }
            }
        }
    }

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
            QContextMenu {
                id: othersContextMenu
                property string myRole: AppModel.walletInfo.groupId !== "" ? AppModel.walletInfo.myRole : ""
                property bool isAssisted: {

                    var isAss = AppModel.walletInfo.isAssistedWallet && parseFloat(AppModel.walletInfo.walletBalance) === 0
                    if (AppModel.walletInfo.isGroupWallet) {
                        return isAss && (AppModel.walletInfo.groupId !== "" ? myRole === "MASTER" : true)
                    }
                    else {
                        return isAss
                    }
                }
                property bool isCanDeleted: !AppModel.walletInfo.isAssistedWallet || isAssisted
                property bool isPrimaryOwner: (myRole === "MASTER" || myRole === "ADMIN") && (walletInfo.ownerMembers.length > 0)
                menuWidth: 300
                labels: {
                    var ls = [];
                    ls.push(STR.STR_QML_312)
                    ls.push(STR.STR_QML_198)
                    ls.push(STR.STR_QML_532)
                    ls.push(STR.STR_QML_686)
                    ls.push(STR.STR_QML_825)
                    if (isPrimaryOwner) {
                        ls.push(STR.STR_QML_1193)
                    }
                    ls.push(STR.STR_QML_1363)
                    if(isCanDeleted){
                        ls.push(STR.STR_QML_332)
                    }
                    return ls
                }
                icons:{
                    var ls = [];
                    ls.push("qrc:/Images/Images/settings-dark.svg")
                    ls.push("qrc:/Images/Images/download.png")
                    ls.push("qrc:/Images/Images/import_031F2B.png")
                    ls.push("qrc:/Images/Images/cached_24px.png")
                    ls.push("qrc:/Images/Images/settings-dark.svg")
                    if (isPrimaryOwner) {
                        ls.push("qrc:/Images/Images/account-member-dark.svg")
                    }
                    ls.push("qrc:/Images/Images/key-dark.svg")
                    if(isCanDeleted){
                        ls.push("qrc:/Images/Images/Delete.svg")
                    }
                    return ls
                }
                enables: [true, true, true , true, true, true, true]
                functions: {
                    var ls = [];
                    ls.push(function(){ QMLHandle.sendEvent(EVT.EVT_WALLET_INFO_UTXOS_REQUEST); })
                    ls.push(function(){ QMLHandle.sendEvent(EVT.EVT_WALLET_INFO_CHANGE_ADDRESS_REQUEST); })
                    ls.push(function(){ openfileDialog.open(); })
                    ls.push(function(){ forceRefresh.open(); })
                    ls.push(function(){ gaplimit.open(); })
                    if (isPrimaryOwner) {
                        ls.push(function(){ eFlow = eWALLET_PRIMARY_OWNER })
                    }
                    ls.push(function(){
                        if (!AppModel.walletInfo.isAssistedWallet) { // free acount
                            QMLHandle.sendEvent(EVT.EVT_REPLACE_KEYS_REQUEST, {isFirst: true})
                        } else {
                            _info.open()
                        }
                    })
                    if(isCanDeleted) {
                        ls.push(function(){
                            if (isAssisted) {
                                QMLHandle.sendEvent(EVT.EVT_WALLET_INFO_REMOVE, AppModel.walletInfo)
                            }
                            else {
                                modelConfirmDelete.open();
                            }
                        })
                    }
                    return ls
                }
                colors:{
                    var ls = [];
                    ls.push("#031F2B")
                    ls.push("#031F2B")
                    ls.push("#031F2B")
                    ls.push("#031F2B")
                    ls.push("#031F2B")
                    if (isPrimaryOwner) {
                        ls.push("#031F2B")
                    }
                    ls.push("#031F2B")
                    if(isCanDeleted){
                        ls.push("#CF4018")
                    }
                    return ls
                }
                onItemClicked: {
                    functions[index]()
                }
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
            QContextMenu {
                id: exportContextMenu
                menuWidth: 320
                labels: [
                    STR.STR_QML_324,
                    STR.STR_QML_325,
                    STR.STR_QML_326,
                    STR.STR_QML_327,
                    STR.STR_QML_328,
                    STR.STR_QML_1279,
                    STR.STR_QML_1280,
                    STR.STR_QML_674
                ]
                icons: [
                    "qrc:/Images/Images/description.png",
                    "qrc:/Images/Images/backup.png",
                    "qrc:/Images/Images/backup.png",
                    "qrc:/Images/Images/backup.png",
                    "qrc:/Images/Images/fileDownload.png",
                    "qrc:/Images/Images/exportqr.png",
                    "qrc:/Images/Images/exportqr.png",
                    "qrc:/Images/Images/fileDownload.png"
                ]
                onItemClicked: {
                    switch(index){
                    case 0: // Export Wallet BSMS File
                        exportwalletDialog.exportFormat = NUNCHUCKTYPE.DESCRIPTOR
                        exportwalletDialog.currentFile = StandardPaths.writableLocation(StandardPaths.DocumentsLocation) + "/"
                                + RoomWalletData.getValidFilename(AppModel.walletInfo.walletName)
                                + ".bsms"
                        exportwalletDialog.open()
                        break;
                    case 1: // Export Wallet Database
                        exportwalletDialog.exportFormat = NUNCHUCKTYPE.DB
                        exportwalletDialog.currentFile = StandardPaths.writableLocation(StandardPaths.DocumentsLocation) + "/"
                                + RoomWalletData.getValidFilename(AppModel.walletInfo.walletName)
                                + "-database.dat"
                        exportwalletDialog.open()
                        break;
                    case 2: // "Export Transaction History (CSV)"
                        exportwalletDialog.exportFormat = NUNCHUCKTYPE.TRANSACTION_CSV
                        exportwalletDialog.currentFile = StandardPaths.writableLocation(StandardPaths.DocumentsLocation) + "/"
                                + RoomWalletData.getValidFilename(AppModel.walletInfo.walletName)
                                + "-tx-history.csv"
                        exportwalletDialog.open()
                        break;
                    case 3: // "Export UTXOs (CSV)"
                        exportwalletDialog.exportFormat = NUNCHUCKTYPE.UTXO_CSV
                        exportwalletDialog.currentFile = StandardPaths.writableLocation(StandardPaths.DocumentsLocation) + "/"
                                + RoomWalletData.getValidFilename(AppModel.walletInfo.walletName)
                                + "-utxos.csv"
                        exportwalletDialog.open()
                        break;
                    case 4: // "Export To Coldcard"
                        exportwalletDialog.exportFormat = NUNCHUCKTYPE.COLDCARD
                        exportwalletDialog.currentFile = StandardPaths.writableLocation(StandardPaths.DocumentsLocation) + "/"
                                + RoomWalletData.getValidFilename(AppModel.walletInfo.walletName)
                                + "-Coldcard-Config.txt"
                        exportwalletDialog.open()
                        break;
                    case 5: // "Export as BC-UR2 QR (legacy)"
                        qrcodeExportResult.filename = RoomWalletData.getValidFilename(AppModel.walletInfo.walletName) + "_BCUR2_Legacy"
                        qrcodeExportResult.open()
                        var datalegacy = {
                            "qrtype": "BC-UR2-QR-Legacy"
                        }
                        QMLHandle.sendEvent(EVT.EVT_WALLET_INFO_EXPORT_QRCODE, datalegacy)
                        break;
                    case 6: // "Export as BC-UR2 QR"
                        qrcodeExportResult.filename = RoomWalletData.getValidFilename(AppModel.walletInfo.walletName) + "_BCUR2"
                        qrcodeExportResult.open()
                        var data_seedhammer = {
                            "qrtype": "BC-UR2-QR"
                        }
                        QMLHandle.sendEvent(EVT.EVT_WALLET_INFO_EXPORT_QRCODE, data_seedhammer)
                        break;

                    case 7: //"Export wallet to Bitbox"
                        var addrs = AppModel.walletInfo.walletunUsedAddressList;
                        if(addrs.length > 0){
                            displayAddressBusybox.addrToVerify = addrs[0]
                            AppModel.startDisplayAddress(AppModel.walletInfo.walletId, addrs[0])
                        }
                        break;
                    default:
                        break;
                    }
                }
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
            var wallet = AppModel.walletInfo;
            if(wallet.isSharedWallet){
                confirmDeleteWallet.open()
            }
            else{
                QMLHandle.sendEvent(EVT.EVT_WALLET_INFO_REMOVE, AppModel.walletInfo)
            }
        }
    }
    QPopupInfo{
        id:_info
        title: STR.STR_QML_339
        contentText: STR.STR_QML_1391
        usingMin: true
    }
}
