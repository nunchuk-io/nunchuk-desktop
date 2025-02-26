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
    label.text: STR.STR_QML_278
    extraHeader: QText {
        height: 21
        width: 550
        elide: Text.ElideRight
        text: "(" + wlname.textOutput + ")"
        color: "#031F2B"
        font.family: "Montserrat"
        font.weight: Font.DemiBold
        font.pixelSize: 14
    }
    onCloseClicked: {
        wlname.applyNameChanged()
        wldescription.applyDescriptionChanged()
        QMLHandle.sendEvent(EVT.EVT_WALLET_INFO_BACK_REQUEST)
    }
    QNotification {
        id: notification
        width: 718
        height: notification.visible ? 70 : 0
        wrapMode: Text.WordWrap
        labelWidth: 620
        anchors.horizontalCenter: parent.horizontalCenter
        visible: AppModel.walletInfo.containsColdcard && (AppModel.walletInfo.walletN > 1)
        label: STR.STR_QML_316
        currentStatus: EWARNING.WARNING_MSG
        onCloseRequest: notification.visible = false
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: parent.top
            topMargin: 81
        }
    }
    Flickable {
        width: 344
        height: exportwallet.y - y - 10
        flickableDirection: Flickable.VerticalFlick
        clip: true
        interactive: height < contentHeight
        contentHeight: contentWallets.height
        ScrollBar.vertical: ScrollBar { active: true }
        anchors {
            left: parent.left
            leftMargin: 40
            top: notification.bottom
            topMargin: 25
        }
        Column {
            id: contentWallets
            spacing: 8
            QTextInputBox {
                id: wlname
                width: 344
                heightMin: 54
                mode: ePREVIEW_MODE
                color: "Transparent"
                placeholder.text: STR.STR_QML_317
                textOutput: AppModel.walletInfo.walletName
                onTypingFinished: textOutput = Qt.binding(function(){return currentText;})
                onEditingFinished: wlname.applyNameChanged()
                function applyNameChanged(){
                    if(wlname.textOutput !== AppModel.walletInfo.walletName && wlname.textOutput!== ""){
                        var infoObj = {
                            "name"          : wlname.textOutput,
                            "description"   : wldescription.textOutput
                        };
                        QMLHandle.sendEvent(EVT.EVT_WALLET_INFO_EDIT_NAME, infoObj)
                    }
                }
            }
            QTextInputBox {
                id: wldescription
                width: 344
                heightMin: 54
                mode: ePREVIEW_MODE
                placeholder.text: STR.STR_QML_318
                color: textOutput === "" ? Qt.rgba(201, 222, 241, 0.5) : "Transparent"
                textOutput: AppModel.walletInfo.walletDescription
                isEditing: true
                onTypingFinished: textOutput = Qt.binding(function(){return currentText;})
                onEditingFinished: wldescription.applyDescriptionChanged()
                function applyDescriptionChanged(){
                    if(wldescription.textOutput !== AppModel.walletInfo.walletDescription){
                        var infoObj = {
                            "name"          : wlname.textOutput,
                            "description"   : wldescription.textOutput
                        };

                        QMLHandle.sendEvent(EVT.EVT_WALLET_INFO_EDIT_DESCRIPTION, infoObj)
                    }
                }
            }
            QTextInputBox {
                id: wltype
                width: 344
                heightMin: 54
                mode: eREADONLY_MODE
                color: "Transparent"
                placeholder.text: STR.STR_QML_319
                textOutput: AppModel.walletInfo.walletEscrow ? STR.STR_QML_029 : STR.STR_QML_028
            }
            QTextInputBox {
                id: addresstype
                readonly property var addressType_Value: [
                    STR.STR_QML_065,
                    STR.STR_QML_064,
                    STR.STR_QML_063,
                    STR.STR_QML_062,
                    STR.STR_QML_553
                ]
                width: 344
                heightMin: 54
                mode: eREADONLY_MODE
                color: "Transparent"
                placeholder.text: STR.STR_QML_066
                textOutput: addresstype.addressType_Value[AppModel.walletInfo.walletAddressType]
            }
            QTextInputBox {
                id: createdate
                width: 344
                heightMin: 54
                mode: eREADONLY_MODE
                color: "Transparent"
                placeholder.text: STR.STR_QML_067
                textOutput: AppModel.walletInfo.walletCreateDate
            }
        }
    }
    Row {
        id: rightTit
        spacing: 8
        anchors {
            left: parent.left
            leftMargin: 416
            top: notification.bottom
            topMargin: 25
        }
        QText {
            text: STR.STR_QML_320
            font.family: "Montserrat"
            font.weight: Font.DemiBold
            color: "#031F2B"
            font.pixelSize: 14
            verticalAlignment: Text.AlignBottom
        }
        QText {
            text: AppModel.walletInfo.walletM
            font.family: "Lato"
            font.weight: Font.Bold
            color: "#031F2B"
            font.pixelSize: 16
            verticalAlignment: Text.AlignBottom
        }
        Rectangle {
            width: 1
            height: 16
            color: "#839096"
        }
        QText {
            text: AppModel.walletInfo.walletN
            font.family: "Lato"
            font.weight: Font.Bold
            color: "#839096"
            font.pixelSize: 16
            verticalAlignment: Text.AlignBottom
        }
    }
    QListView {
        id: listSignerAssigned
        width: 355
        height: 283
        model: AppModel.walletInfo.walletSingleSignerAssigned
        spacing: 8
        clip: true
        anchors {
            right: parent.right
            rightMargin: 40
            top: rightTit.bottom
            topMargin: 25
        }
        ScrollBar.vertical: ScrollBar { active: true }
        delegate: Item {
            id: delegateAssignSigners
            width: 344
            height: rect.height + 4
            Rectangle {
                id: rect
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width - 4
                height: 110
                border.color: single_signer_isColdcard ? "#F6D65D" : "gray"
                color: itemMouse.containsMouse ? Qt.rgba(255, 255, 255, 0.7):  Qt.rgba(255, 255, 255)
                radius: 4
                layer.enabled: true
                layer.effect: DropShadow {
                    source: rect
                    verticalOffset: 3
                    radius: 8
                    samples: 32
                    color: "#80000000"
                }

                Row {
                    anchors.fill: parent
                    anchors.margins: 8
                    spacing: 8
                    Rectangle {
                        width: 4
                        height: parent.height
                        color: itemMouse.containsMouse ? "#F6D65D" : "#C9DEF1"
                    }
                    Item {
                        width: 220
                        height: parent.height
                        Column {
                            anchors.fill: parent
                            spacing: 8
                            Item {
                                width: parent.width
                                height: 21
                                QText {
                                    id: signername
                                    width: parent.width - (signerTypeText.width + 30)
                                    height: 21
                                    text: model.singleSigner_name
                                    font.pixelSize: 14
                                    font.weight: Font.DemiBold
                                    font.family: "Montserrat"
                                    color: "#031F2B"
                                    anchors.verticalCenter: parent.verticalCenter
                                    elide: Text.ElideRight
                                }
                            }
                            QText {
                                id: signerXFP
                                width: parent.width
                                height: 16
                                text: qsTr("%1 %2").arg(STR.STR_QML_321).arg(model.singleSigner_masterFingerPrint)
                                font.capitalization: Font.AllUppercase
                                font.family: "Lato"
                                font.pixelSize: 12
                                color: "#031F2B"
                            }
                            QText {
                                id: derivation
                                width: parent.width
                                height: 16
                                text: qsTr("%1 %2").arg(STR.STR_QML_150).arg(model.singleSigner_derivationPath)
                                font.family: "Lato"
                                font.pixelSize: 12
                                color: "#839096"
                            }
                            QText {
                                id: healthCheckDate
                                width: parent.width
                                height: 16
                                text: qsTr("%1 %2").arg(STR.STR_QML_322).arg(model.singleSigner_lastHealthCheck)
                                font.family: "Lato"
                                font.pixelSize: 12
                                color: "#839096"
                            }
                        }
                    }
                    Item {
                        width: 80
                        height: parent.height
                        Column {
                            anchors.fill: parent
                            spacing: 8
                            Item {
                                width: parent.width
                                height: 21
                                Rectangle {
                                    id:_type
                                    width: signerTypeText.width + 10
                                    height: signerTypeText.height + 10
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.right: parent.right
                                    color: single_signer_isColdcard ? "#F6D65D" : "#C9DEF1"
                                    radius: 20
                                    visible: signerTypeText.text !== ""
                                    QText {
                                        id: signerTypeText
                                        text: GlobalData.signers(model.single_signer_type)
                                        font.family: "Lato"
                                        font.weight: Font.Bold
                                        font.pixelSize: 10
                                        anchors.centerIn: parent
                                        color: "#031F2B"
                                    }
                                }
                            }
                            Item {
                                width: parent.width
                                height: 16
                                visible: model.single_signer_primary_key
                                Rectangle {
                                    width: primarytxt.paintedWidth + 8*2
                                    height: 21
                                    color: "#FDD95C"
                                    radius: 20
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.right: parent.right
                                    QText {
                                        id: primarytxt
                                        text: STR.STR_QML_641
                                        font.family: "Lato"
                                        font.weight: Font.Bold
                                        font.pixelSize: 10
                                        anchors.centerIn: parent
                                        color: "#031F2B"
                                    }
                                }
                            }
                            Item {
                                width: parent.width
                                height: 16
                                visible: (single_signer_account_index > 0) && (model.single_signer_type !== NUNCHUCKTYPE.SERVER)
                                Rectangle {
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.right: parent.right
                                    height: parent.height
                                    width: accText.width + 16
                                    color: "#EAEAEA"
                                    radius: 20
                                    QText {
                                        id: accText
                                        text: qsTr("Acct %1").arg(single_signer_account_index)
                                        font.family: "Lato"
                                        font.weight: Font.Bold
                                        font.pixelSize: 10
                                        anchors.centerIn: parent
                                        color: "#031F2B"
                                    }
                                }
                            }
                            Item {
                                width: parent.width
                                height: 16
                            }
                        }
                    }
                }
            }
            MouseArea {
                id: itemMouse
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                onClicked: {
                    var objectRequest = {
                        "requestType": "signerInfo",
                        "requestData": index
                    }
                    QMLHandle.sendEvent(EVT.EVT_WALLET_INFO_SIGNER_INFO_REQUEST, objectRequest)
                }
            }
        }
    }
    QButtonLargeTail {
        id: exportwallet
        width: 198
        height: 48
        anchors {
            right: parent.right
            rightMargin: 36
            bottom: parent.bottom
            bottomMargin: 59
        }
        label: STR.STR_QML_323
        type: ePRIMARY
        optionVisible: exportContextMenu.visible
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
                STR.STR_QML_1279,
                STR.STR_QML_1280,
                STR.STR_QML_328,
                STR.STR_QML_674
            ]
            icons: [
                "qrc:/Images/Images/description.png",
                "qrc:/Images/Images/Backup.png",
                "qrc:/Images/Images/Backup.png",
                "qrc:/Images/Images/Backup.png",
                "qrc:/Images/Images/exportqr.png",
                "qrc:/Images/Images/exportqr.png",
                "qrc:/Images/Images/fileDownload.png",
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
                case 4: // "Export as BC-UR2 QR (legacy)"
                    qrcodeExportResult.filename = RoomWalletData.getValidFilename(AppModel.walletInfo.walletName) + "_BCUR2_Legacy"
                    qrcodeExportResult.open()
                    var datalegacy = {
                        "qrtype": "BC-UR2-QR-Legacy"
                    }
                    QMLHandle.sendEvent(EVT.EVT_WALLET_INFO_EXPORT_QRCODE, datalegacy)
                    break;
                case 5: // "Export as BC-UR2 QR"
                    qrcodeExportResult.filename = RoomWalletData.getValidFilename(AppModel.walletInfo.walletName) + "_BCUR2"
                    qrcodeExportResult.open()
                    var data_seedhammer = {
                        "qrtype": "BC-UR2-QR"
                    }
                    QMLHandle.sendEvent(EVT.EVT_WALLET_INFO_EXPORT_QRCODE, data_seedhammer)
                    break;
                case 6: // "Export To Coldcard"
                    exportwalletDialog.exportFormat = NUNCHUCKTYPE.COLDCARD
                    exportwalletDialog.currentFile = StandardPaths.writableLocation(StandardPaths.DocumentsLocation) + "/"
                            + RoomWalletData.getValidFilename(AppModel.walletInfo.walletName)
                            + "-Coldcard-Config.txt"
                    exportwalletDialog.open()
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
    QButtonLargeTail {
        id: otheraction
        width: 200
        height: 48
        anchors {
            right: parent.right
            rightMargin: 250
            bottom: parent.bottom
            bottomMargin: 59
        }
        label: STR.STR_QML_331
        type: eSECONDARY
        optionVisible: othersContextMenu.visible
        onButtonClicked: {
            othersContextMenu.x = 20
            othersContextMenu.y = 20 - othersContextMenu.height
            othersContextMenu.open()
        }
        QContextMenu {
            id: othersContextMenu
            property string myRole: AppModel.walletInfo.myRole
            property bool isAssisted: {
                if (AppModel.walletInfo.isAssistedWallet && parseFloat(AppModel.walletInfo.walletBalance) === 0) {
                    if (AppModel.walletInfo.isAssistedWallet) {
                        return true
                    }
                    else {
                        return (AppModel.walletInfo.groupId !== "" ? myRole === "MASTER" : true)
                    }
                }
                return false

            }
            property bool isCanDeleted: !AppModel.walletInfo.isAssistedWallet || isAssisted
            menuWidth: 300
            labels: {
                var ls = [];
                ls.push(STR.STR_QML_312)
                ls.push(STR.STR_QML_198)
                ls.push(STR.STR_QML_532)
                ls.push(STR.STR_QML_686)
                ls.push(STR.STR_QML_825)
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
                if(isCanDeleted){
                    ls.push("qrc:/Images/Images/Delete.svg")
                }
                return ls
            }
            enables: [
                (myRole !== "FACILITATOR_ADMIN"),
                (myRole !== "FACILITATOR_ADMIN"),
                (myRole !== "FACILITATOR_ADMIN"),
                true,
                (myRole !== "FACILITATOR_ADMIN"),
                (myRole !== "FACILITATOR_ADMIN")
            ]
            visibles: [
                (myRole !== "FACILITATOR_ADMIN"),
                (myRole !== "FACILITATOR_ADMIN"),
                (myRole !== "FACILITATOR_ADMIN"),
                true,
                (myRole !== "FACILITATOR_ADMIN"),
                (myRole !== "FACILITATOR_ADMIN")
            ]

            functions: {
                var ls = [];
                ls.push(function(){ QMLHandle.sendEvent(EVT.EVT_WALLET_INFO_UTXOS_REQUEST); })
                ls.push(function(){ QMLHandle.sendEvent(EVT.EVT_WALLET_INFO_CHANGE_ADDRESS_REQUEST); })
                ls.push(function(){ openfileDialog.open(); })
                ls.push(function(){ forceRefresh.open(); })
                ls.push(function(){ gaplimit.open(); })
                if(isCanDeleted){
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
    QText {
        text: STR.STR_QML_333
        anchors {
            right: parent.right
            rightMargin: 40
            bottom: parent.bottom
            bottomMargin: 40
        }
        font.family: "Lato"
        font.pixelSize: 12
        color: "#323E4A"
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
}
