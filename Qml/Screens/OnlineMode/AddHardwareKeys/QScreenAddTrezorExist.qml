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
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Chats"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../Components/customizes/Signers"
import "../../../../localization/STR_QML.js" as STR

QScreenAdd {
    anchors.fill: parent
    readonly property int eREUSE_LIST_KEYS: 0
    readonly property int eREUSE_INPUT_INDEX: 1
    readonly property int eREUSE_LOADING_STATE: 2
    readonly property int eREUSE_SCAN_DEVICE: 3
    readonly property int eREUSE_RESULT: 4
    property int eADD_STEP: eREUSE_LIST_KEYS
    Component.onCompleted: {
        inputtingIndex.device_name = ""
        inputtingIndex.device_type = ""
        inputtingIndex.device_tag = ""
        inputtingIndex.device_xfp = ""
        inputtingIndex.device_bip32_path = ""
    }

    QOnScreenContentTypeB {
        id:_content
        visible: eADD_STEP === eREUSE_LIST_KEYS
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        label.text: STR.STR_QML_901
        extraHeader: Item {}
        onCloseClicked: closeTo(NUNCHUCKTYPE.WALLET_TAB)
        content: QAddAnExistingKey {
            notice: STR.STR_QML_915
        }
        onPrevClicked: closeTo(NUNCHUCKTYPE.WALLET_TAB)
        bottomRight: Row {
            spacing: 12
            QTextButton {
                width: 215
                height: 48
                label.text: STR.STR_QML_900
                label.font.pixelSize: 16
                type: eTypeB
                onButtonClicked: {
                    QMLHandle.sendEvent(EVT.EVT_ADD_NEW_HARDWARE_REQUEST)
                }
            }
            QTextButton {
                width: 150
                height: 48
                label.text: STR.STR_QML_899
                label.font.pixelSize: 16
                type: eTypeE
                enabled: _content.contentItem.fingerPrint !== ""
                onButtonClicked: {
                    inputtingIndex.current_index = draftWallet.reuseKeyGetCurrentIndex(_content.contentItem.fingerPrint)
                    inputtingIndex.device_name = _content.contentItem.key_name
                    inputtingIndex.device_type = _content.contentItem.device_type
                    inputtingIndex.device_tag = _content.contentItem.tag
                    inputtingIndex.device_xfp = _content.contentItem.fingerPrint
                    inputtingIndex.device_bip32_path = draftWallet.bip32path(inputtingIndex.device_xfp, inputtingIndex.current_index)
                    eADD_STEP = eREUSE_INPUT_INDEX
                }
            }
        }
    }
    QOnScreenContentTypeA {
        id: inputtingIndex
        property string device_name: ""
        property string device_type: ""
        property string device_tag: ""
        property string device_xfp : ""
        property string device_bip32_path : ""
        property int    current_index: -1
        property int    new_index: -1

        visible: eADD_STEP == eREUSE_INPUT_INDEX
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        label.text: STR.STR_QML_1046
        onCloseClicked: closeTo(NUNCHUCKTYPE.WALLET_TAB)
        content: Item {
            Column {
                spacing: 32
                Item {
                    width: parent.width
                    height: 64
                    Row {
                        anchors.fill: parent
                        anchors.margins: 8
                        spacing: 12
                        Rectangle {
                            width: 48
                            height: 48
                            radius: width
                            color: "#F5F5F5"
                            QSignerDarkIcon {
                                iconSize: 24
                                anchors.centerIn: parent
                                device_type: inputtingIndex.device_type
                                type: NUNCHUCKTYPE.HARDWARE
                                tag: inputtingIndex.device_tag
                            }
                        }
                        Column {
                            spacing: 4
                            anchors.verticalCenter: parent.verticalCenter
                            QText {
                                width: 146
                                height: 20
                                text: inputtingIndex.device_name
                                color: "#031F2B"
                                font.weight: Font.Normal
                                font.family: "Lato"
                                font.pixelSize: 16
                            }
                            QText {
                                width: 146
                                height: 20
                                text: "XFP: " + inputtingIndex.device_xfp
                                color: "#595959"
                                font.weight: Font.Normal
                                font.capitalization: Font.AllUppercase
                                font.family: "Lato"
                                font.pixelSize: 12
                            }
                        }
                    }
                }
                QText {
                    width: 540
                    font.family: "Lato"
                    font.pixelSize: 16
                    lineHeight: 28
                    lineHeightMode: Text.FixedHeight
                    wrapMode: Text.WordWrap
                    text: STR.STR_QML_1171
                }
                Column {
                    spacing: 4
                    QTextInputBoxTypeB {
                        label: STR.STR_QML_1172
                        boxWidth: 350
                        boxHeight: 48
                        isValid: true
                        enabled: false
                        textInputted: (inputtingIndex.current_index >= 0) ? inputtingIndex.current_index : ""
                    }
                    QText {
                        visible: (inputtingIndex.current_index >= 0)
                        text: "BIP32 path: " + inputtingIndex.device_bip32_path
                    }
                }
                Column {
                    spacing: 4
                    QTextInputBoxTypeB {
                        id: newIndexInputted
                        label: STR.STR_QML_1173
                        boxWidth: 350
                        boxHeight: 48
                        isValid: true
                        validator:  RegExpValidator {regExp: /^[0-9][0-9]*$/ }
                        textInputted: inputtingIndex.current_index === -1 ? 0 : ""
                        onTextInputtedChanged: {
                            if(textInputted !== ""){
                                inputtingIndex.new_index = textInputted
                            }
                            else{
                                inputtingIndex.new_index = -1
                            }
                        }
                    }
                    QText {
                        visible: newIndexInputted.textInputted !== ""
                        text: "BIP32 path: " + ((AppSetting.primaryServer === NUNCHUCKTYPE.MAIN) ? qsTr("m/48h/0h/%1h/2h").arg(newIndexInputted.textInputted !== "" ? newIndexInputted.textInputted : "0")
                                                                                                 : qsTr("m/48h/1h/%1h/2h").arg(newIndexInputted.textInputted !== "" ? newIndexInputted.textInputted : "0"))
                    }
                }
            }
        }
        onPrevClicked: { eADD_STEP = eREUSE_LIST_KEYS }
        bottomRight: QTextButton {
            width: 120
            height: 48
            label.text: STR.STR_QML_265
            label.font.pixelSize: 16
            type: eTypeE
            enabled: inputtingIndex.new_index !== -1
            onButtonClicked: {
                eADD_STEP = eREUSE_LOADING_STATE
                draftWallet.reuseKeyFromMasterSigner(inputtingIndex.device_xfp, inputtingIndex.new_index)
            }
        }
    }
    QOnScreenContentTypeA {
        id: scanDevices
        visible: eADD_STEP === eREUSE_SCAN_DEVICE
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        label.text: STR.STR_QML_814
        onCloseClicked: closeTo(NUNCHUCKTYPE.WALLET_TAB)
        content: QAddKeyRefreshDevices {
            title: STR.STR_QML_830
            state_id: EVT.STATE_ID_SCR_ADD_HARDWARE_EXIST
        }
        onPrevClicked: { eADD_STEP = eREUSE_INPUT_INDEX }
        bottomRight: QTextButton {
            width: 120
            height: 48
            label.text: STR.STR_QML_265
            label.font.pixelSize: 16
            type: eTypeE
            enabled: scanDevices.contentItem.isEnable()
            onButtonClicked: {
                if (scanDevices.contentItem.selected_xfp === inputtingIndex.device_xfp) {
                    eADD_STEP = eREUSE_LOADING_STATE
                    draftWallet.reuseKeyFromMasterSigner(inputtingIndex.device_xfp, inputtingIndex.new_index)
                }
                else
                {
                    AppModel.showToast(-1, STR.STR_QML_1169, EWARNING.ERROR_MSG);
                }
            }
        }
    }
    QOnScreenContent {
        id: loadingState
        visible: eADD_STEP === eREUSE_LOADING_STATE
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        enableHeader: false
        content: Item {
            Column {
                width: 400
                height: 56
                anchors.centerIn: parent
                spacing: 16
                QBusyIndicator {
                    width: 70
                    height: 70
                    anchors.horizontalCenter: parent.horizontalCenter
                }
                QLato{
                    font.weight: Font.Bold
                    font.pixelSize: 20
                    text: STR.STR_QML_831
                    anchors.horizontalCenter: parent.horizontalCenter
                    horizontalAlignment: Text.AlignHCenter
                }
            }
        }
    }
    QOnScreenContent {
        id: addKeysuccess
        visible: eADD_STEP === eREUSE_RESULT
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        label.text: ""
        onCloseClicked: closeTo(NUNCHUCKTYPE.WALLET_TAB)
        Column {
            anchors.fill: parent
            anchors.margins: 36
            spacing: 24
            Rectangle {
                width: 96;height: 96;
                radius: 48
                color: "#A7F0BA"
                QIcon {
                    iconSize: 60
                    anchors.centerIn: parent
                    source: "qrc:/Images/Images/check-dark.svg"
                }
            }
            QLato {
                width: parent.width
                height: 40
                text: STR.STR_QML_832
                font.pixelSize: 32
                font.weight: Font.DemiBold
                verticalAlignment: Text.AlignVCenter
            }
            QLato {
                width: parent.width
                height: 28
                text: STR.STR_QML_828
                verticalAlignment: Text.AlignVCenter
                lineHeightMode: Text.FixedHeight
                lineHeight: 28
                wrapMode: Text.WordWrap
            }
        }
        bottomRight: Row {
            spacing: 12
            QTextButton {
                width: 120
                height: 48
                label.text: STR.STR_QML_777
                label.font.pixelSize: 16
                type: eTypeE
                onButtonClicked: doneAddHardwareKey()
            }
        }
    }

    Connections {
        target: draftWallet
        onReuseKeyGetSignerResult : {
            console.log("onReuseKeyGetSignerResult", result)
            if(result == 1){
                eADD_STEP = eREUSE_RESULT
            }
            else{
                eADD_STEP = eREUSE_SCAN_DEVICE
            }
        }
        onAddHardwareAlert: {
            eADD_STEP = eREUSE_SCAN_DEVICE
        }
    }
}
