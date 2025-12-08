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
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Chats"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../Components/customizes/Signers"
import "../../../Components/customizes/Popups"
import "../SetupWallets"
import "../../../../localization/STR_QML.js" as STR

QScreenAdd {
    anchors.fill: parent
    QInputtingIndex { id: inputtingIndex }
    readonly property var map_flow: [
        {screen: "eSCREEN_LIST_KEYS",                    screen_component: _reuseListKeys},
        {screen: "eSCREEN_INPUT_INDEX",                  screen_component: _accountSettingIndex},
        {screen: "eSCREEN_LOADING",                screen_component: loadingState},
        {screen: "eSCREEN_REFRESH_DEVICE",                  screen_component: _scanDevices},
        {screen: "eSCREEN_SUCCESS",                       screen_component: addKeysuccess},
        {screen: "eSCREEN_VIA_FILE",                     screen_component: importViaFile},
        {screen: "_ASK_PASSPHRASE",                     screen_component: _passPhraseSelect},
        {screen: "_IMPORTANT_NOTICE",                   screen_component: _importantNotice},
        {screen: "_BACKUP_PASSPHRASE",                  screen_component: _passPhraseBackup},
        {screen: "_BACKUP_COLDCARD",                    screen_component: _backupCOLDCARD},
        {screen: "eSCREEN_CLAIM_INHERITANCE_PLAN_RESULT_ERROR", screen_component: _resultClaimInheritancePlan},
    ]
    Loader {
        width: popupWidth
        height: popupHeight
        sourceComponent: {
            var itemScreen = map_flow.find(function(e) {if (e.screen == stateScreen.screenFlow) return true; else return false})
            if (itemScreen) {
                return itemScreen.screen_component
            } else {
                return _reuseListKeys
            }
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

    Component {
        id: _passPhraseSelect
        QSelectPassPhraseQuestion {
            onRequestBack: {
                closeTo(NUNCHUCKTYPE.CURRENT_TAB)
            }
            onRequestNext: {
                if (option === "not-have-a-passphrase") {
                    stateScreen.setScreenFlow("eSCREEN_INPUT_INDEX")
                } else {
                    stateScreen.setScreenFlow("_IMPORTANT_NOTICE")
                }
            }
        }
    }
    Component {
        id: _importantNotice
        QImportantNoticeAboutPassphrase {
            onRequestBack: stateScreen.backScreen()
            onRequestNext: {
                stateScreen.setScreenFlow("_BACKUP_PASSPHRASE")
            }
            onRequestWithout: {
                var alert = GroupWallet.dashboardInfo.alert
                var can_replace = alert.payload.can_replace
                if (can_replace) {
                    GroupWallet.dashboardInfo.requestShowReplacementKey();
                } else {
                    GroupWallet.dashboardInfo.requestShowLetAddYourKeys();
                }
            }
        }
    }
    Component {
        id: _passPhraseBackup
        QPassphraseBackupReminder {
            onRequestBack: stateScreen.backScreen()
            onRequestNext: {
                stateScreen.setScreenFlow("eSCREEN_INPUT_INDEX")
            }
        }
    }
    Component {
        id: _backupCOLDCARD
        QBackupCOLDCARD {
            inputFingerPrint: inputtingIndex.device_xfp
            onPrevClicked: {
                stateScreen.setScreenFlow("eSCREEN_SUCCESS")
            }

        }
    }
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
                notice: STR.STR_QML_916
            }
            onPrevClicked: stateScreen.backScreen()
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
                        inputtingIndex.signer_type = _content.contentItem.type
                        inputtingIndex.device_tag = _content.contentItem.tag
                        inputtingIndex.device_xfp = _content.contentItem.fingerPrint
                        inputtingIndex.device_bip32_path = draftWallet.bip32path(inputtingIndex.device_xfp, inputtingIndex.current_index)                        
                        var isNormalFlow = SignerManagement.currentSigner.wallet_type !== "MINISCRIPT"
                        if (isNormalFlow) {
                            stateScreen.setScreenFlow("_ASK_PASSPHRASE")
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
    Component {
        id: _accountSettingIndex
        QOnScreenContentTypeA {
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
                                    type: inputtingIndex.signer_type
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

            onPrevClicked: stateScreen.backScreen()
            bottomRight: QTextButton {
                width: 120
                height: 48
                label.text: STR.STR_QML_265
                label.font.pixelSize: 16
                type: eTypeE
                enabled: inputtingIndex.new_index !== -1
                onButtonClicked: {
                    if(inputtingIndex.signer_type == NUNCHUCKTYPE.COLDCARD_NFC) {
                        if (inputtingIndex.new_index == inputtingIndex.current_index) {
                            stateScreen.setScreenFlow("eSCREEN_LOADING")
                            draftWallet.requestAddOrReplacementWithIndexAsync(inputtingIndex.device_xfp, inputtingIndex.new_index)
                        }
                        else {
                            _info.open()
                        }
                    }
                    else {
                        stateScreen.setScreenFlow("eSCREEN_LOADING")
                        draftWallet.requestAddOrReplacementWithIndexAsync(inputtingIndex.device_xfp, inputtingIndex.new_index)
                    }
                }
            }
            QPopupInfoTwoButtons {
                id: _info
                property string link:"https://nunchuk.io/"
                title: STR.STR_QML_339
                contentText: STR.STR_QML_1166
                labels: [STR.STR_QML_035,STR.STR_QML_341]
                isVertical: false
                funcs: [
                    function() {},
                    function() {
                        GroupWallet.dashboardInfo.requestShowLetAddYourKeys();
                    }
                ]
            }
        }
    }
    Component {
        id: _scanDevices
        QOnScreenContentTypeA {
            id: scanDevices
            width: popupWidth
            height: popupHeight
            anchors.centerIn: parent
            label.text: STR.STR_QML_904
            onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
            content: QAddKeyRefreshDevices {
                title: STR.STR_QML_911
                state_id: EVT.STATE_ID_SCR_ADD_HARDWARE_EXIST
            }
            onPrevClicked: { 
                stateScreen.backScreen()
                if (stateScreen.screenFlow === "eSCREEN_LOADING") {
                    stateScreen.backScreen()
                }
            }
            bottomRight: Row {
                spacing: 12
                QIconTextButton {
                    width: 244
                    height: 48
                    label: STR.STR_QML_1050
                    icons: ["importFile.svg", "importFile.svg", "importFile.svg","importFile.svg"]
                    fontPixelSize: 16
                    iconSize: 16
                    type: eTypeB
                    onButtonClicked: {
                        stateScreen.setScreenFlow("eSCREEN_VIA_FILE")
                    }
                }
                QTextButton {
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
    }
    Component {
        id: loadingState
        QScreenAddKeyBusyLoadingState {
            busyTitle: STR.STR_QML_912
        }
    }
    Component {
        id: addKeysuccess
        QScreenAddKeyResult {
            isSuccess: true
            resultTitle: STR.STR_QML_913
            resultSubtitle: STR.STR_QML_828
            onDoneClicked: doneAddHardwareKey()
        }
    }

    Component {
        id: _resultClaimInheritancePlan
        QScreenAddKeyResult {
            isSuccess: false
            resultTitle: STR.STR_QML_2045
            resultSubtitle: STR.STR_QML_2046
            bottomRight: Row {
                spacing: 12
                QTextButton {
                    width: 120
                    height: 48
                    label.text: STR.STR_QML_097
                    label.font.pixelSize: 16
                    type: eTypeE
                    onButtonClicked: {
                        closeTo(NUNCHUCKTYPE.CURRENT_TAB)
                    }
                }
            }
        }
    }
    function doneOrTryAgainAddColdcardKey(isSuccess) {
        var isNormalFlow = SignerManagement.currentSigner.wallet_type !== "MINISCRIPT"
        if (isNormalFlow) {
            if (isSuccess) {
                var is_inheritance = GroupWallet.dashboardInfo.isInheritance()
                if (is_inheritance) {
                    stateScreen.setScreenFlow("_BACKUP_COLDCARD")
                } else {
                    AppModel.showToast(0, STR.STR_QML_1392, EWARNING.SUCCESS_MSG);
                    closeTo(NUNCHUCKTYPE.CURRENT_TAB)
                }
            } else {
                GroupWallet.refresh()
                GroupWallet.dashboardInfo.requestShowLetAddYourKeys();
            }
        } else {
            var xfp = SignerManagement.currentSigner.xfp
            if (GroupWallet.dashboardInfo.enoughKeyAdded(xfp)) {
                AppModel.showToast(0, STR.STR_QML_1392, EWARNING.SUCCESS_MSG);
                closeTo(NUNCHUCKTYPE.CURRENT_TAB)
            } else {
                GroupWallet.refresh()
                GroupWallet.dashboardInfo.requestShowLetAddYourKeys();
            }
        }
    }
    Component {
        id: importViaFile
        QScreenAddColdcardViaAFile
        {
            id: via_file
            onPrevClicked: stateScreen.backScreen()
            function addDeviceViaImportFile() {
                var xfp = draftWallet.reuseKeyXfp(via_file.file)
                if (inputtingIndex.device_xfp === xfp) {
                    draftWallet.requestImportFileAddOrReplacementWithIndexAsync(via_file.file, inputtingIndex.new_index)
                }
                else
                {
                    AppModel.showToast(-1, STR.STR_QML_1169, EWARNING.ERROR_MSG);
                }
            }
        }
    }
}
