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
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Chats"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../Components/customizes/QRCodes"
import "../../Components/customizes/Popups"
import "../../../localization/STR_QML.js" as STR

QScreen {
    id:_screen
    property bool firstEnable: true
    QOnScreenContent {
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        label.text: STR.STR_QML_021
        onCloseClicked: {
            QMLHandle.sendEvent(EVT.EVT_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_ADD_WALLET)
        }
        Row {
            id: step
            width: 644
            height: 31
            spacing: 8
            anchors {
                left: parent.left
                leftMargin: 40
                top: parent.top
                topMargin: 78
            }
            QAddStep {
                step: 1
                stepName: STR.STR_QML_022
                currentStep: 1
                anchors.verticalCenter: parent.verticalCenter
            }
            Rectangle {
                width: 30
                height: 2
                radius: 4
                color: "#839096"
                anchors.verticalCenter: parent.verticalCenter
            }
            QAddStep {
                step: 2
                stepName: STR.STR_QML_023
                currentStep: 1
                anchors.verticalCenter: parent.verticalCenter
            }
            Rectangle {
                width: 30
                height: 2
                radius: 4
                color: "#839096"
                anchors.verticalCenter: parent.verticalCenter
            }
            QAddStep {
                step: 3
                stepName: STR.STR_QML_024
                currentStep: 1
                anchors.verticalCenter: parent.verticalCenter
            }
        }
        QText {
            text: STR.STR_QML_022
            font.weight: Font.Bold
            font.pixelSize: 16
            color: "#031F2B"
            anchors {
                left: parent.left
                leftMargin: 40
                top: step.bottom
                topMargin: 32
            }
        }
        Row {
            anchors {
                left: parent.left
                leftMargin: 40
                top: parent.top
                topMargin: 195
            }
            spacing: 32
            QTextInputBox {
                id: signerName
                width: 269
                heightMin: 56
                mode: eEDIT_MODE
                placeholder.text: STR.STR_QML_025
                maximumLength: 42
                textOutput: AppModel.newWalletInfo.walletName
                color: Qt.rgba(255, 255, 255, 0.5)
                rightPading: 50
                border.color: "#C9DEF1"
                radius: 4
                anchors.verticalCenter: parent.verticalCenter
            }

            QTextInputBox {
                id: signerDescription
                width: 419
                heightMin: 56
                mode: eEDIT_MODE
                placeholder.text: STR.STR_QML_026
                textOutput: AppModel.newWalletInfo.walletDescription
                color: Qt. rgba(255, 255, 255, 0.5)
                border.color: "#C9DEF1"
                rightPading: 50
                maximumLength: 76
                radius: 4
                anchors.verticalCenter: parent.verticalCenter
            }
        }
        QText {
            text: STR.STR_QML_027
            font.weight: Font.Bold
            font.pixelSize: 14
            color: "#031F2B"
            anchors {
                left: parent.left
                leftMargin: 40
                top: parent.top
                topMargin: 283
            }
        }
        FocusScope {
            anchors {
                left: parent.left
                leftMargin: 40
                top: parent.top
                topMargin: 312
            }
            Item {
                id: standardWallet
                focus: !AppModel.newWalletInfo.walletEscrow
                width: 130
                height: 24
                Image {
                    id:icostandard
                    source: standardWallet.focus ? "qrc:/Images/Images/radio-selected-dark.svg" : "qrc:/Images/Images/radio-dark.svg"
                }
                QText {
                    text: STR.STR_QML_028
                    font.pixelSize: 14
                    color: "#031F2B"
                    anchors {
                        left: icostandard.right
                        leftMargin: 8
                        verticalCenter: icostandard.verticalCenter
                    }
                }
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    onClicked: standardWallet.focus = true
                }
            }
            Item {
                id: escrowWallet
                focus: AppModel.newWalletInfo.walletEscrow
                height: 24
                anchors {
                    left: standardWallet.right
                    leftMargin: 42
                    verticalCenter: standardWallet.verticalCenter
                }
                Row {
                    Item {
                        width: 212
                        height: 24
                        Image {
                            id:icoescrow
                            source: escrowWallet.focus ? "qrc:/Images/Images/radio-selected-dark.svg" : "qrc:/Images/Images/radio-dark.svg"
                        }
                        QText {
                            id: textEscrow
                            text: STR.STR_QML_029
                            font.pixelSize: 14
                            color: "#031F2B"
                            anchors {
                                left: icoescrow.right
                                leftMargin: 8
                                verticalCenter: icoescrow.verticalCenter
                            }
                        }
                        MouseArea {
                            anchors.fill: parent
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            onClicked: escrowWallet.focus = true
                        }
                    }
                    QTooltip {
                        toolTip: STR.STR_QML_030
                    }
                }
            }
        }
        Rectangle {
            width: 720
            height: 1
            color: "#C9DEF1"
            anchors {
                horizontalCenter: parent.horizontalCenter
                top: parent.top
                topMargin: 368
            }
        }
        Item {
            id: expandOption
            property bool expanded: false
            width: 492
            height: 24
            anchors {
                left: parent.left
                leftMargin: 40
                top: parent.top
                topMargin: 394
            }
            Row {
                Image {
                    id: expandIcon
                    source: expandOption.expanded ? "qrc:/Images/Images/expand_less_24px.png" : "qrc:/Images/Images/expand_more_24px.png"
                }
                QText {
                    text: STR.STR_QML_031
                    font.weight: Font.Bold
                    font.pixelSize: 14
                    color: "#031F2B"
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                onClicked: expandOption.expanded = !expandOption.expanded
            }
        }
        Column {
            visible: expandOption.expanded
            focus: true
            spacing: 8
            anchors {
                left: parent.left
                leftMargin: 68
                top: parent.top
                topMargin: 425
            }
            Repeater {
                id: addressTypeSelection
                property int typeSeleted: AppModel.newWalletInfo.walletAddressType
                readonly property var typeValue: [
                    NUNCHUCKTYPE.NATIVE_SEGWIT,
                    NUNCHUCKTYPE.NESTED_SEGWIT,
                    NUNCHUCKTYPE.LEGACY,
                    NUNCHUCKTYPE.TAPROOT
                ]
                readonly property var typeText: [
                    STR.STR_QML_032,
                    STR.STR_QML_033,
                    STR.STR_QML_034,
                    STR.STR_QML_553
                ]
                model: typeValue.length
                Item {
                    width: 576
                    height: 24
                    Row {
                        id: walletType
                        anchors.fill: parent
                        spacing: 8
                        QIcon {
                            iconSize: 24
                            id:icowalletType
                            source: (addressTypeSelection.typeSeleted === addressTypeSelection.typeValue[index]) ? "qrc:/Images/Images/radio-selected-dark.svg" :
                                                                                                                   "qrc:/Images/Images/radio-dark.svg"
                        }
                        QText {
                            text: addressTypeSelection.typeText[index]
                            font.pixelSize: 14
                            color: "#031F2B"
                            anchors.verticalCenter: icowalletType.verticalCenter
                        }
                    }
                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        onClicked: { addressTypeSelection.typeSeleted = addressTypeSelection.typeValue[index] }
                    }
                }
            }
        }
        QTextButton {
            width: 120
            height: 48
            label.text: STR.STR_QML_035
            label.font.pixelSize: 16
            type: eTypeB
            anchors {
                left: parent.left
                leftMargin: 32
                bottom: parent.bottom
                bottomMargin: 32
            }
            onButtonClicked: {
                QMLHandle.sendEvent(EVT.EVT_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_ADD_WALLET)
            }
        }
        QButtonLargeTail {
            id: importwallet
            width: 209
            height: 48
            anchors {
                left: parent.left
                leftMargin: 335
                bottom: parent.bottom
                bottomMargin: 32
            }
            label: STR.STR_QML_036
            type: eSECONDARY
            optionVisible: optionMenu.visible
            onButtonClicked: {
                optionMenu.x = 20
                optionMenu.y = 20 - optionMenu.height
                optionMenu.open()
            }
            QContextMenu {
                id: optionMenu
                menuWidth: 350
                labels: [
                    STR.STR_QML_037,
                    STR.STR_QML_038,
                    STR.STR_QML_040,
                    STR.STR_QML_041
                ]
                icons: [
                    "qrc:/Images/Images/import.png",
                    "qrc:/Images/Images/QRCodeScan.png",
                    "qrc:/Images/Images/import.png",
                    "qrc:/Images/Images/download.png"
                ]
                onItemClicked: {
                    switch(index){
                    case 0:
                        fileDialog.iType = Popup_t.IMPORT_WALLET_DESCRIPTOR
                        fileDialog.open()
                        break;
                    case 1:
                        fileDialog.iType = Popup_t.IMPORT_WALLET_QRCODE
                        qrscaner.open()
                        break;
                    case 2:
                        fileDialog.iType = Popup_t.IMPORT_WALLET_CONFIGFILE
                        fileDialog.open()
                        break;
                    case 3:
                        fileDialog.iType = Popup_t.IMPORT_WALLET_DB
                        fileDialog.open()
                        break;
                    default:
                        break;
                    }
                }
            }
        }

        QTextButton {
            width: 200
            height: 48
            label.text: STR.STR_QML_042
            label.font.pixelSize: 16
            type: eTypeE
            enabled: (signerName.textOutput !== "") || firstEnable
            anchors {
                right: parent.right
                rightMargin: 40
                bottom: parent.bottom
                bottomMargin: 32
            }
            onButtonClicked: {
                firstEnable = false
                if(signerName.textOutput !== ""){
                    var newSignerObj = { "walletNameInputted"    : signerName.textOutput,
                                         "walletDescription"     : signerDescription.textOutput ,
                                         "walletEscrow"          : escrowWallet.focus ,
                                         "addressType"           : addressTypeSelection.typeSeleted };
                    QMLHandle.sendEvent(EVT.EVT_ADD_WALLET_SIGNER_CONFIGURATION_REQUEST, newSignerObj)
                }
                else{
                    _warning.open()
                }
            }
        }
        QPopupToast{
            id:_warning
            x:36
            y:520
            warningType:EWARNING.WARNING_MSG
            warningExplain:STR.STR_QML_587
        }
    }

    QQrImportScanner {
        id: qrscaner
        onTagFound: {
            if(AppModel.parseQRWallet(signerName.textOutput, signerDescription.textOutput, qrscaner.tags)){
                qrscaner.close()
                var importData = {  "qrTags"        : qrscaner.tags,
                                    "importType"    : fileDialog.iType,
                                    "walletname"    : signerName.textOutput,
                                    "walletdescription"    : signerDescription.textOutput};
                QMLHandle.sendEvent(EVT.EVT_ADD_WALLET_IMPORT, importData)
            }
        }
    }
    FileDialog {
        id: fileDialog
        property int iType: Popup_t.IMPORT_WALLET_DB
        fileMode: FileDialog.OpenFile
        onAccepted: {
            var importData = {  "filePath"      : fileDialog.file,
                                "importType"    : fileDialog.iType,
                                "walletname"    : signerName.textOutput,
                                "walletdescription"    : signerDescription.textOutput};
            QMLHandle.sendEvent(EVT.EVT_ADD_WALLET_IMPORT, importData)
        }
    }
}
