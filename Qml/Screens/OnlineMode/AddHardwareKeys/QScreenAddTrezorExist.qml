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

    QInputtingIndex { id: inputtingIndex }
    readonly property var map_flow: [
        {screen: "eSCREEN_LIST_KEYS",    screen_component: _reuseListKeys},
        {screen: "eSCREEN_INPUT_INDEX",  screen_component: _accountSettingIndex},
        {screen: "eSCREEN_LOADING",screen_component: loadingState},
        {screen: "eSCREEN_REFRESH_DEVICE",  screen_component: _scanDevices},
        {screen: "eSCREEN_SUCCESS",       screen_component: addKeysuccess},
    ]

    Loader {
        width: popupWidth
        height: popupHeight
        sourceComponent: {
            var itemScreen = map_flow.find(function(e) { return e.screen === stateScreen.screenFlow })
            return itemScreen ? itemScreen.screen_component : _reuseListKeys
        }
    }

    Component.onCompleted: {
        var currentSigner = SignerManagement.currentSigner
        inputtingIndex.device_name = currentSigner.name
        inputtingIndex.device_type = currentSigner.type
        inputtingIndex.device_tag = currentSigner.tag
        inputtingIndex.device_xfp = currentSigner.xfp
        inputtingIndex.device_bip32_path = currentSigner.derivation_path
        inputtingIndex.current_index = currentSigner.account_index
        inputtingIndex.new_index = -1
    }

    // Screen: list existing keys
    Component {
        id: _reuseListKeys
        QOnScreenContentTypeB {
            id:_content
            width: popupWidth
            height: popupHeight
            anchors.centerIn: parent
            label.text: STR.STR_QML_901
            extraHeader: Item {}
            onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
            content: QAddAnExistingKey {
                notice: STR.STR_QML_915
            }
            onPrevClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
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
                        var isNormalFlow = SignerManagement.currentSigner.wallet_type !== "MINISCRIPT"
                        if (isNormalFlow) {
                            stateScreen.setScreenFlow("eSCREEN_INPUT_INDEX")
                        } else {
                            var onlyUseForClaim = SignerManagement.currentSigner.onlyUseForClaim !== undefined && SignerManagement.currentSigner.onlyUseForClaim
                            if (onlyUseForClaim) {
                                ServiceSetting.servicesTag.requestDownloadWalletWithIndexAsync(inputtingIndex.device_xfp)
                            } else {
                                draftWallet.requestAddOrReplacementBothIndicesIfPossibleAsync(inputtingIndex.device_xfp)
                            }
                        }
                    }
                }
            }
        }
    }

    // Screen: account/index input
    Component {
        id: _accountSettingIndex
        QOnScreenContentTypeA {
            id: accountIndexSetting
            width: popupWidth
            height: popupHeight
            anchors.centerIn: parent
            label.text: STR.STR_QML_1046
            onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
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
                                QLato {
                                    width: 146
                                    height: 20
                                    text: inputtingIndex.device_name
                                    color: "#031F2B"
                                    font.weight: Font.Normal
                                    font.pixelSize: 16
                                }
                                QLato {
                                    width: 146
                                    height: 20
                                    text: "XFP: " + inputtingIndex.device_xfp
                                    color: "#595959"
                                    font.weight: Font.Normal
                                    font.capitalization: Font.AllUppercase
                                    font.pixelSize: 12
                                }
                            }
                        }
                    }
                    QLato {
                        width: 540
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
                        QLato {
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
                        QLato {
                            visible: newIndexInputted.textInputted !== ""
                            text: "BIP32 path: " + ((AppSetting.primaryServer === NUNCHUCKTYPE.MAIN) ? qsTr("m/48h/0h/%1h/2h").arg(newIndexInputted.textInputted !== "" ? newIndexInputted.textInputted : "0")
                                                                                                     : qsTr("m/48h/1h/%1h/2h").arg(newIndexInputted.textInputted !== "" ? newIndexInputted.textInputted : "0"))
                        }
                    }
                }
            }
            onPrevClicked: { stateScreen.setScreenFlow("eSCREEN_LIST_KEYS") }
            bottomRight: QTextButton {
                width: 120
                height: 48
                label.text: STR.STR_QML_265
                label.font.pixelSize: 16
                type: eTypeE
                enabled: inputtingIndex.new_index !== -1
                onButtonClicked: {
                    stateScreen.setScreenFlow("eSCREEN_LOADING")
                    draftWallet.requestAddOrReplacementWithIndexAsync(inputtingIndex.device_xfp, inputtingIndex.new_index)
                }
            }
        }
    }

    // Screen: scan devices
    Component {
        id: _scanDevices
        QOnScreenContentTypeA {
            id: scanDevices
            width: popupWidth
            height: popupHeight
            anchors.centerIn: parent
            label.text: STR.STR_QML_814
            onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
            content: QAddKeyRefreshDevices {
                title: STR.STR_QML_830
                state_id: EVT.STATE_ID_SCR_ADD_HARDWARE_EXIST
            }
            onPrevClicked: { 
                stateScreen.backScreen()
                if (stateScreen.screenFlow === "eSCREEN_LOADING") {
                    stateScreen.backScreen()
                }
            }
            bottomRight: QTextButton {
                width: 120
                height: 48
                label.text: STR.STR_QML_265
                label.font.pixelSize: 16
                type: eTypeE
                enabled: scanDevices.contentItem.isEnable()
                onButtonClicked: {
                    if (scanDevices.contentItem.selected_xfp === inputtingIndex.device_xfp) {
                        stateScreen.setScreenFlow("eSCREEN_LOADING")
                        draftWallet.requestAddOrReplacementWithIndexAsync(inputtingIndex.device_xfp, inputtingIndex.new_index)
                    }
                    else
                    {
                        AppModel.showToast(-1, STR.STR_QML_1169, EWARNING.ERROR_MSG);
                    }
                }
            }
        }
    }

    // Loading state (no change in UX)
    Component {
        id: loadingState
        QScreenAddKeyBusyLoadingState {
            busyTitle: STR.STR_QML_831
        }
    }

    // Result screen (no change in UX)
    Component {
        id: addKeysuccess
        QScreenAddKeyResult {
            isSuccess: true
            resultTitle: STR.STR_QML_832
            resultSubtitle: STR.STR_QML_828
            onDoneClicked: doneAddHardwareKey()
        }
    }

    // Events mapping to ScreenState
    Connections {
        target: draftWallet
        onAddHardwareAlert: {
            stateScreen.setScreenFlow("eSCREEN_REFRESH_DEVICE")
        }
    }
}
