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
import "../../Components/customizes/Wallets"
import "../OnlineMode/ReplaceKeyFrees"
import "../OnlineMode/AddHardwareKeys"
import "../../../localization/STR_QML.js" as STR

QScreen {
    property var inputtingIndex: QtObject {
        property string device_name: ""
        property int signer_type: -1
        property string device_type: ""
        property string device_tag: ""
        property string device_xfp : ""
        property string device_bip32_path : ""
        property int    current_index: -1
        property int    new_index: -1
    }
    property var walletInfo: AppModel.newWalletInfo
    property var replaceFlow: walletInfo.replaceFlow
    readonly property var map_screens: [
        {screen_name: "replace-key-info",         screen_component: replace_key_info    },
        {screen_name: "replace-select-keys",      screen_component: replace_select_keys },
        {screen_name: "replace-existing-key",     screen_component: replace_existing_key},
        {screen_name: "customize-key-account",    screen_component: customize_key_account},
        {screen_name: "congratulation-done",      screen_component: congratulation_done  },
        {screen_name: "replace-transfer-funds",   screen_component: replace_transfer_funds},
        {screen_name: "replace-before-you-begin", screen_component: replace_before_you_begin},
        {screen_name: "replace-add-signer",       screen_component: addSigner()},
        {screen_name: "replace-AddColdcardViaAFile", screen_component: replace_AddColdcardViaAFile},
    ]
    function setReplaceFlow(flow) {
        var _input = {
            type: "switch-screen-flow",
            screen_name: flow
        }
        QMLHandle.sendEvent(EVT.EVT_REPLACE_KEYS_ENTER, _input)
    }
    function selectReplaceKey(xfp, index) {
        var _input = {
            type: "select-replace-key",
            fingerXfp: xfp,
            indexXfp: index
        }
        QMLHandle.sendEvent(EVT.EVT_REPLACE_KEYS_ENTER, _input)
    }
    readonly property int eADD_REFRESH_DEVICE: 1
    readonly property int eADD_LOADING: 2
    readonly property int eADD_SUCCESSFULLY: 3
    readonly property int eADD_ERROR: 4
    function addSigner() {
        switch(AppModel.addSignerWizard) {
        case eADD_REFRESH_DEVICE: return replace_add_signer
        case eADD_LOADING: return replace_add_signer_loading
        case eADD_SUCCESSFULLY: return null
        case eADD_ERROR: return null
        default: return null
        }
    }

    Loader {
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        sourceComponent: map_screens.find(function(e) {if (e.screen_name === replaceFlow) return true; else return false}).screen_component
    }

    Component {
        id: replace_key_info
        QReplaceKeysInfo {

        }
    }
    Component {
        id: replace_select_keys
        QReplaceSelectKeys {

        }
    }
    Component {
        id: replace_existing_key
        QReplaceExistingKey {

        }
    }
    Component {
        id: customize_key_account
        QReplaceCustomizeKeyAccount {

        }
    }
    Component {
        id: congratulation_done
        QReplaceCongratulationANewWalletCreated {

        }
    }
    Component {
        id: replace_transfer_funds
        QReplaceTransferFunds {

        }
    }
    Component {
        id: replace_before_you_begin
        QReplaceBeforeYouBegin {

        }
    }
    Component {
        id: replace_add_signer_loading
        QReplaceAddSignerLoading {

        }
    }
    Component {
        id: replace_add_signer
        QReplaceAddSigner {

        }
    }
    Component {
        id: replace_AddColdcardViaAFile
        QScreenAddColdcardViaAFile {
            id: via_file
            onPrevClicked: {
                setReplaceFlow("replace-add-signer")
            }
            function addDeviceViaImportFile() {
                var masterSignerObj = {
                    "type"       : "import-coldcard-via-file",
                    "file_path"  : via_file.file,
                    "new_index"  : inputtingIndex.new_index
                };
                QMLHandle.sendEvent(EVT.EVT_REPLACE_KEYS_ENTER, masterSignerObj)
            }
        }
    }
}
