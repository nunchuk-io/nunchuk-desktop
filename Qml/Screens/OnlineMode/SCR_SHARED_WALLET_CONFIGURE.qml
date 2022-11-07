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
import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.12
import Qt.labs.platform 1.1
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Chats"
import "../../../localization/STR_QML.js" as STR
QScreen {
    id: roots
    popupWidth: 800
    popupHeight: 700

    QOnScreenContent {
        id: contenCenter
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        label.text: STR.STR_QML_437
        label.font.pixelSize: 32
        label.font.weight: Font.Medium
        onCloseClicked: {
            QMLHandle.sendEvent(EVT.EVT_ONLINE_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_SHARED_WALLET_CONFIGURE)
        }
        extraHeader: Row {
            id: configureHeader
            spacing: 4
            Rectangle {
                width: creatingState.width + 20
                height: 24
                radius: 20
                color: RoomWalletData.roomWalletCreated ? "#A7F0BA" : RoomWalletData.fullSigners ? "#FDEBD2" : "#FFD7D9"
                QText {
                    id: creatingState
                    anchors.centerIn: parent
                    text: RoomWalletData.roomWalletCreated ? STR.STR_QML_376 : RoomWalletData.fullSigners ? STR.STR_QML_377 : STR.STR_QML_378
                    font.family: "Lato"
                    font.pixelSize: 16
                    color: "#031F2B"
                    font.weight: Font.Medium
                }
            }
            Rectangle {
                width: creatingState.width + 20
                height: 24
                radius: 20
                color: "#EAEAEA"
                QText {
                    id: signerConfig
                    anchors.centerIn: parent
                    text: qsTr("%1 %2").arg(RoomWalletData.multisigConfiguration).arg(STR.STR_QML_069)
                    font.family: "Lato"
                    font.pixelSize: 16
                    color: "#031F2B"
                    font.weight: Font.Medium
                }
            }
            Rectangle {
                width: 80
                height: 24
                radius: 20
                color: "#EAEAEA"
                Row {
                    spacing: 4
                    anchors.centerIn: parent
                    QImage {
                        width: 16
                        height: 16
                        anchors.verticalCenter: parent.verticalCenter
                        source: "qrc:/Images/Images/OnlineMode/Joint wallet_031F2B.png"
                    }
                    QText {
                        anchors.verticalCenter: parent.verticalCenter
                        text: STR.STR_QML_438
                        font.family: "Lato"
                        font.pixelSize: 16
                        color: "#031F2B"
                        font.weight: Font.Medium
                    }
                }
            }
        }
        Row {
            anchors {
                horizontalCenter: parent.horizontalCenter
                top: parent.top
                topMargin: 100
            }
            spacing: 28
            Rectangle {
                width: 350
                height: 480
                radius: 12
                color: "#F5F5F5"
                Column {
                    anchors.fill: parent
                    anchors {
                        topMargin: 24
                        bottomMargin: 24
                        rightMargin: 16
                        leftMargin: 16
                    }
                    spacing: 24

                    QTextInputBoxTypeA {
                        id: wlname
                        width: parent.width
                        height: Math.max(80, contentHeight + topPadding + bottomPadding)
                        placeholderText: STR.STR_QML_317
                        text: RoomWalletData.walletName
                        backgroundColor: "Transparent"
                        borderColor: "Transparent"
                        color: "#000000"
                        font.family: "Montserrat"
                        font.pixelSize: 32
                        font.weight: Font.Medium
                        enabled: false // Temporary disable untill support it
                    }

                    Item {
                        width: parent.width
                        height: childrenRect.height
                        QText {
                            anchors.left: parent.left
                            text: STR.STR_QML_027
                            font.family: "Lato"
                            font.pixelSize: 16
                            color: "#031F2B"
                        }
                        QText {
                            anchors.right: parent.right
                            text: RoomWalletData.walletType
                            font.family: "Lato"
                            font.pixelSize: 16
                            font.weight: Font.Bold
                            horizontalAlignment: Text.AlignRight
                            color: "#031F2B"
                        }
                    }

                    Item {
                        width: parent.width
                        height: childrenRect.height
                        QText {
                            anchors.left: parent.left
                            text: STR.STR_QML_436
                            font.family: "Lato"
                            font.pixelSize: 16
                            color: "#031F2B"
                        }
                        QText {
                            anchors.right: parent.right
                            text: RoomWalletData.addressType
                            font.family: "Lato"
                            font.pixelSize: 16
                            font.weight: Font.Bold
                            horizontalAlignment: Text.AlignRight
                            color: "#031F2B"
                        }
                    }
                }
            }
            Column {
                width: 350
                height: 480
                spacing: 8
                Row {
                    width: parent.width
                    height: 20
                    spacing: 8
                    QText {
                        text: STR.STR_QML_289
                        color: "#031F2B"
                        font.family: "Lato"
                        font.weight: Font.Bold
                        font.pixelSize: 18
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    QImage {
                        width: 16
                        height: 16
                        anchors.verticalCenter: parent.verticalCenter
                        source: RoomWalletData.fullSigners ? "qrc:/Images/Images/OnlineMode/check_circle_24px.png" :
                                                             "qrc:/Images/Images/OnlineMode/pending_actions-24px.png"
                    }
                    QText {
                        text: RoomWalletData.fullSigners ? STR.STR_QML_439 : (RoomWalletData.pendingSigners <= 1 ? STR.STR_QML_440_: STR.STR_QML_440.arg(RoomWalletData.pendingSigners))
                        color: "#595959"
                        font.family: "Lato"
                        font.pixelSize: 16
                        font.weight: Font.Medium
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
                Rectangle {
                    width: parent.width
                    height: 452
                    radius: 12
                    color: "#FFFFFF"
                    border.color: "#EAEAEA"
                    QListView {
                        id: signerAssigned
                        anchors.fill: parent
                        anchors.margins: 16
                        model: RoomWalletData.roomWalletReady ? RoomWalletData.roomWallet.walletSigners : 0
                        clip: true
                        delegate: QWalletSignerDelegate {
                            width: signerAssigned.width
                            height: 62
                            signername : model.signer_name
                            signerxfp: model.signer_xfp
                            isLocaluser: model.signer_is_localuser
                            signerType: model.signer_type
                            isPrimaryKey: model.signer_primary_key
                            onItemClicked: {RoomWalletData.currentRoom.leaveWallet(model.signer_xfp)}
                        }
                        section
                        {
                            property: "signer_is_localuser"
                            criteria: ViewSection.FullString
                            delegate: Item {
                                id: sectionItem
                                width: signerAssigned.width
                                height: isLocalUser ? 1 : 16
                                property bool isLocalUser: section === "true"
                                Rectangle {
                                    anchors.centerIn: parent
                                    width: parent.width
                                    height: 1
                                    visible: !sectionItem.isLocalUser
                                    color: "#EAEAEA"
                                }
                            }
                        }
                        ScrollBar.vertical: ScrollBar { active: true }
                    }
                }
            }
        }
        QButtonTextLink {
            height: 48
            displayIcon: false
            label: STR.STR_QML_441
            fontPixelSize: 16
            textColor: ["#CF4018", "#839096", "#35ABEE"]
            visible: !RoomWalletData.roomWalletCreated
            enabled: visible
            anchors {
                left: parent.left
                leftMargin: 40
                bottom: parent.bottom
                bottomMargin: 36
            }
            onButtonClicked: {confirmCancelWallet.open()}
        }
        Loader {
            id: buttonControl
            anchors {
                right: parent.right
                rightMargin: 36
                bottom: parent.bottom
                bottomMargin: 36
            }
            sourceComponent: RoomWalletData.roomWalletCreated ? backupWalletBtns : RoomWalletData.fullSigners ? (RoomWalletData.isCreator ? finalizeBtn : closeBtn) : addSignerBtn
        }
        Component {
            id: closeBtn
            QTextButton {
                width: 133
                height: 48
                label.text: STR.STR_QML_139
                label.font.pixelSize: 16
                type: eTypeE
                onButtonClicked: {
                    QMLHandle.sendEvent(EVT.EVT_ONLINE_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_SHARED_WALLET_CONFIGURE)
                }
            }
        }
        Component {
            id: finalizeBtn
            QTextButton {
                width: 133
                height: 48
                label.text: STR.STR_QML_442
                label.font.pixelSize: 16
                type: eTypeE
                onButtonClicked: {
                    if(RoomWalletData.currentRoom) {RoomWalletData.currentRoom.finalizeWallet()}
                    QMLHandle.sendEvent(EVT.EVT_ONLINE_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_SHARED_WALLET_CONFIGURE)
                }
            }
        }
        Component {
            id: addSignerBtn
            QTextButton {
                width: 230
                height: 48
                label.text: STR.STR_QML_443
                label.font.pixelSize: 16
                type: eTypeE
                onButtonClicked: {
                    QMLHandle.sendEvent(EVT.EVT_SHARED_WALLET_ADD_SIGNER_REQUEST)
                }
            }
        }
        Component {
            id: backupWalletBtns
            Row {
                spacing: 12
                QButtonLargeTail {
                    width: 170
                    height: 48
                    label: STR.STR_QML_444
                    type: eSECONDARY
                    optionVisible: optionMenu.visible
                    onButtonClicked: {
                        optionMenu.popup()
                    }
                    QContextMenu {
                        id: optionMenu
                        labels: [
                            STR.STR_QML_312,
                            STR.STR_QML_198
//                          STR.STR_QML_332
                        ]
                        onItemClicked: {
                            switch(index){
                            case 0:
                                QMLHandle.sendEvent(EVT.EVT_SHARED_WALLET_UTXO_OUTPUT)
                                break;
                            case 1:
                                QMLHandle.sendEvent(EVT.EVT_SHARED_WALLET_CHANGE_ADDRESS)
                                break;
                            case 2:
                            default:
                                modelConfirmDelete.open()
                                break;
                            }
                        }
                    }
                }
                QTextButton {
                    width: 80
                    height: 48
                    label.text: STR.STR_QML_323
                    label.font.pixelSize: 16
                    type: eTypeB
                    onButtonClicked: {
                        exportMenu.popup()
                    }
                    QContextMenu {
                        id: exportMenu
                        labels: [
                            STR.STR_QML_445,
                            STR.STR_QML_446
                        ]
                        icons: [
                            "qrc:/Images/Images/OnlineMode/Export.png",
                            "qrc:/Images/Images/OnlineMode/QRCodeScan.png"
                        ]
                        onItemClicked: {
                            switch(index){
                            case 0:
                                exportwalletDialog.exportFormat = NUNCHUCKTYPE.COLDCARD
                                exportwalletDialog.currentFile = StandardPaths.writableLocation(StandardPaths.DocumentsLocation) + "/"
                                        + RoomWalletData.getValidFilename(RoomWalletData.walletName)
                                        + "-Coldcard-Config.txt"
                                exportwalletDialog.open()
                                break;
                            case 1:
                            default:
                                qrcodeExportResult.open()
                                QMLHandle.sendEvent(EVT.EVT_SHARED_WALLET_CONFIGURE_EXPORT_QRCODE, RoomWalletData.walletId)
                                break;
                            }
                        }
                    }
                }
            }
        }
        QQrExportResult {
            id: qrcodeExportResult
            model: AppModel.qrExported
        }
        QConfirmDeletingTypeA {
            id: modelConfirmDelete
            modalWidth: 488
            modalHeight: 317
            label: STR.STR_QML_334
            content: STR.STR_QML_335
            onDeleteRequest: { }
        }
    }
    QConfirmYesNoPopup {
        id: confirmCancelWallet
        onConfirmNo: close()
        onConfirmYes: {
            close()
            if(RoomWalletData.currentRoom) {RoomWalletData.currentRoom.cancelWallet()}
            QMLHandle.sendEvent(EVT.EVT_ONLINE_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_SHARED_WALLET_CONFIGURE)
        }
    }
    FileDialog {
        id: exportwalletDialog
        property int exportFormat: -1
        fileMode: FileDialog.SaveFile
        onAccepted: {
            if(exportFormat === NUNCHUCKTYPE.COLDCARD){
                QMLHandle.sendEvent(EVT.EVT_SHARED_WALLET_CONFIGURE_EXPORT_COLDCARD, exportwalletDialog.currentFile)
            }
        }
    }
}
