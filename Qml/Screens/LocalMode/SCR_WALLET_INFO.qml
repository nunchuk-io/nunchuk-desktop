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
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Chats"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../../localization/STR_QML.js" as STR
QScreen {
    QOnScreenContent {
        id: contenCenter
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
            visible: listSignerAssigned.isContainColdCard && (AppModel.walletInfo.walletN > 1)
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
                            QMLHandle.sendEvent(EVT.EVT_WALLET_INFO_EDIT_NAME, wlname.textOutput)
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
                            QMLHandle.sendEvent(EVT.EVT_WALLET_INFO_EDIT_DESCRIPTION, wldescription.textOutput)
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
            property bool isContainColdCard: false
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
                property int    signerType: model.single_signer_type
                Component.onCompleted: { if(single_signer_isColdcard) listSignerAssigned.isContainColdCard = true }
                Rectangle {
                    id: rect
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: parent.width - 4
                    height: columnContent.height + 16
                    border.color: single_signer_isColdcard ? "#F6D65D" : "transparent"
                    color: itemMouse.containsMouse ? Qt.rgba(255, 255, 255, 0.7):  Qt.rgba(255, 255, 255)
                    radius: 4
                    layer.enabled: true
                    layer.effect: DropShadow {
                        source: rect
                        verticalOffset: 3
                        radius: 8
                        samples: 16
                        color: "#80000000"
                    }
                    Rectangle {
                        width: 4
                        height: columnContent.height
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        anchors.leftMargin: 16
                        color: itemMouse.containsMouse ? "#F6D65D" : "#C9DEF1"
                    }
                    Column {
                        id: columnContent
                        width: parent.width - 40
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        anchors.leftMargin: 30
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
                            Rectangle {
                                width: _txt.paintedWidth + 8*2
                                height: 21
                                color: "#FDD95C"
                                visible: model.single_signer_primary_key
                                radius: 4
                                anchors{
                                    verticalCenter: parent.verticalCenter
                                    right: _type.left
                                    rightMargin: 4
                                }
                                QText {
                                    id:_txt
                                    text: STR.STR_QML_641
                                    font.family: "Lato"
                                    font.weight: Font.Bold
                                    font.pixelSize: 10
                                    anchors.centerIn: parent
                                    color: "#031F2B"
                                }
                            }
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
                                    text: single_signer_isColdcard ? "COLDCARD" :  GlobalData.signerNames(delegateAssignSigners.signerType)
                                    font.family: "Lato"
                                    font.weight: Font.Bold
                                    font.pixelSize: 10
                                    anchors.centerIn: parent
                                    color: "#031F2B"
                                }
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
                MouseArea {
                    id: itemMouse
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    onClicked: {
                        QMLHandle.sendEvent(EVT.EVT_WALLET_INFO_SIGNER_INFO_REQUEST, index)
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
                menuWidth: 400
                labels: [
                    STR.STR_QML_324,
                    STR.STR_QML_325,
                    STR.STR_QML_326,
                    STR.STR_QML_327,
                    STR.STR_QML_328,
                    STR.STR_QML_329,
                    STR.STR_QML_330,
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
                    case 5: // "Export as QR Keystone"
                        qrcodeExportResult.open()
                        QMLHandle.sendEvent(EVT.EVT_WALLET_INFO_EXPORT_QRCODE, "keystone")
                        break;
                    case 6: //"Export as QR Passport"
                        qrcodeExportResult.open()
                        QMLHandle.sendEvent(EVT.EVT_WALLET_INFO_EXPORT_QRCODE, "passport")
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
                menuWidth: 280
                labels: {
                    var ls = [];
                    ls.push(STR.STR_QML_312)
                    ls.push(STR.STR_QML_198)
                    if(!AppModel.walletInfo.isAssistedWallet){
                        ls.push(STR.STR_QML_332)
                    }
                    ls.push(STR.STR_QML_532)
                    if(ClientController.user.isPremiumUser){
                        ls.push(STR.STR_QML_686)
//                        ls.push(STR.STR_QML_825)
                    }
                    return ls
                }
                icons:{
                    var ls = [];
                    ls.push("qrc:/Images/Images/settings-dark.svg")
                    ls.push("qrc:/Images/Images/download.png")
                    if(!AppModel.walletInfo.isAssistedWallet){
                        ls.push("qrc:/Images/Images/Delete.png")
                    }
                    ls.push("qrc:/Images/Images/import_031F2B.png")
                    if(ClientController.user.isPremiumUser){
                        ls.push("qrc:/Images/Images/cached_24px.png")
                        ls.push("qrc:/Images/Images/settings-dark.svg")
                    }
                    return ls
                }
                enables: [true, true, true , true, true, true]
                functions: {
                    var ls = [];
                    ls.push(function(){ QMLHandle.sendEvent(EVT.EVT_WALLET_INFO_UTXOS_REQUEST); })
                    ls.push(function(){ QMLHandle.sendEvent(EVT.EVT_WALLET_INFO_CHANGE_ADDRESS_REQUEST); })
                    if(!AppModel.walletInfo.isAssistedWallet){
                        ls.push(function(){ modelConfirmDelete.open(); })
                    }
                    ls.push(function(){ openfileDialog.open(); })
                    if(ClientController.user.isPremiumUser){
                        ls.push(function(){ forceRefresh.open(); })
                        ls.push(function(){ gaplimit.open(); })
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
    QConfirmYesNoPopup {
        id: confirmDeleteWallet
        title: STR.STR_QML_661
        contentText: STR.STR_QML_662
        onConfirmNo: close()
        onConfirmYes: {
            close()
            QMLHandle.sendEvent(EVT.EVT_WALLET_INFO_REMOVE, AppModel.walletInfo)
        }
    }
    QQrExportResult {
        id: qrcodeExportResult
        model: AppModel.qrExported
    }
    FileDialog {
        id: exportwalletDialog
        property int exportFormat: -1
        fileMode: FileDialog.SaveFile
        onAccepted: {
            if(exportFormat === NUNCHUCKTYPE.DB){
                QMLHandle.sendEvent(EVT.EVT_WALLET_INFO_EXPORT_DB, exportwalletDialog.currentFile)
            }
            else if(exportFormat === NUNCHUCKTYPE.DESCRIPTOR){
                QMLHandle.sendEvent(EVT.EVT_WALLET_INFO_EXPORT_DESCRIPTOR, exportwalletDialog.currentFile)
            }
            else if(exportFormat === NUNCHUCKTYPE.COLDCARD){
                QMLHandle.sendEvent(EVT.EVT_WALLET_INFO_EXPORT_COLDCARD, exportwalletDialog.currentFile)
            }
//            else if(exportFormat === NUNCHUCKTYPE.QRCODE){
//                QMLHandle.sendEvent(EVT.EVT_WALLET_INFO_EXPORT_QRCODE, exportwalletDialog.currentFile)
//                qrcodeExportResult.open()
//            }
            else if(exportFormat === NUNCHUCKTYPE.TRANSACTION_CSV){
                var csvTxObj = { "csv_type"     : 1,
                                 "file_path"    : exportwalletDialog.currentFile};
                QMLHandle.sendEvent(EVT.EVT_WALLET_INFO_EXPORT_CSV, csvTxObj)
            }
            else if(exportFormat === NUNCHUCKTYPE.UTXO_CSV){
                var csvUtxoObj = { "csv_type"     : -1,
                                   "file_path"    : exportwalletDialog.currentFile};
                QMLHandle.sendEvent(EVT.EVT_WALLET_INFO_EXPORT_CSV, csvUtxoObj)
            }
            else{}
        }
    }
    FileDialog {
        id: openfileDialog
        fileMode: FileDialog.OpenFile
        onAccepted: {
            QMLHandle.sendEvent(EVT.EVT_WALLET_INFO_IMPORT_PSBT, openfileDialog.file)
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
    /*=================Force refresh (advanced)=================*/
    QConfirmYesNoPopup {
        id: forceRefresh
        title: STR.STR_QML_334
        contentText: STR.STR_QML_689
        onConfirmNo: close()
        onConfirmYes: {
            close()
            forceRefreshBusybox.open()
            QMLHandle.sendEvent(EVT.EVT_WALLET_INFO_REFRESH_WALLET_REQUEST,AppModel.walletInfo.walletId);
        }
    }
    Popup {
        id: forceRefreshBusybox
        width: parent.width
        height: parent.height
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape
        background: Item{}
        property string addrToVerify: ""
        Rectangle {
            id: forceRefreshMask
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
                    text: STR.STR_QML_122
                }
            }
        }
        DropShadow {
            anchors.fill: forceRefreshMask
            horizontalOffset: 3
            verticalOffset: 5
            spread: 0
            radius: 8
            samples: 30
            color: "#aa000000"
            source: forceRefreshMask
        }
        onOpened: {
            forceRefreshTimer.start()
        }
        Timer {
            id: forceRefreshTimer
            interval: 5000
            repeat: false
            onTriggered: {
                stop()
                forceRefreshBusybox.close()
                _warning.open()
            }
        }
    }
    QPopupToast{
        id:_warning
        x:contenCenter.x + 36
        y:contenCenter.y + 520
        warningType:EWARNING.SUCCESS_MSG
        warningExplain:STR.STR_QML_690
    }

    Popup {
        id: gaplimit
        width: parent.width
        height: parent.height
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape
        background: Item{}
        signal confirmSave()
        signal confirmCancel()
        Rectangle {
            id: boxmask
            width: 300
            height: 250
            radius: 24
            color: "#FFFFFF"
            anchors.centerIn: parent
            layer.enabled: true
            layer.effect: OpacityMask {
                maskSource: Rectangle {
                    width: boxmask.width
                    height: boxmask.height
                    radius: boxmask.radius
                }
            }
            Column {
                spacing: 12
                anchors.fill: parent
                anchors.margins: 24
                QText {
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.family: "Lato"
                    font.pixelSize: 16
                    font.weight: Font.Bold
                    text: "Gap limit"
                }
                QText {
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.family: "Lato"
                    font.pixelSize: 16
                    text: "Max: 100"
                }
                Item { height: 1; width: parent.width }
                Item {
                    width: 252
                    height: 78
                    QTextInputBoxTypeB {
                        id: inputGapLimit
                        label: ""
                        boxWidth: 252
                        boxHeight: 48
                        isValid: textInputted != ""
                        errorText: "Dummy"
                        showError: true
                    }
                }
                Row {
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 12
                    QTextButton {
                        width: 120
                        height: 36
                        label.text: "Cancel"
                        label.font.pixelSize: 12
                        type: eTypeB
                        onButtonClicked: {
                            gaplimit.close()
                        }
                    }
                    QTextButton {
                        width: 120
                        height: 36
                        label.text: "Save"
                        label.font.pixelSize: 12
                        type: eTypeE
                        onButtonClicked: {

                        }
                    }
                }
            }
        }
        DropShadow {
            anchors.fill: boxmask
            horizontalOffset: 3
            verticalOffset: 5
            spread: 0
            radius: 8
            samples: 30
            color: "#aa000000"
            source: boxmask
        }
    }

    /*==========================================================*/
    Connections {
        target: AppModel
        onStartDisplayAddress: {
            if(isOnTop) displayAddressBusybox.open()
            else displayAddressBusybox.close()
        }
        onFinishedDisplayAddress: {
            displayAddressBusybox.close()
        }
    }
}

