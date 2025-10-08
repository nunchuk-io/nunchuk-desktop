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
// Qt imports
import QtQuick 2.4
import QtQuick.Controls 2.3
import Qt.labs.platform 1.1
import QtGraphicalEffects 1.12

// Application-specific imports
import "../../../../localization/STR_QML.js" as STR
import "../../customizes"
import "../../customizes/Buttons"
import "../../customizes/Popups"
import "../../customizes/Signers"
import "../../customizes/Texts"
import "../../origins"

// Third-party or additional module imports
import DataPool 1.0
import EWARNING 1.0
import HMIEVENTS 1.0
import NUNCHUCKTYPE 1.0

Loader {
    property var newWalletInfo: AppModel.newWalletInfo
    property string flow_screen: newWalletInfo.screenFlow
    readonly property var map_flow: [
        {screen: "flexible-multisig",         screen_component: flexible_multisig},
        {screen: "expanding-multisig",        screen_component: expanding_multisig},
        {screen: "decaying-multisig",         screen_component: decaying_multisig},
        {screen: "zen-hodl",                  screen_component: zen_hodl},
        {screen: "enter-mini-script",         screen_component: enter_mini_script},
        {screen: "setup-mini-script",         screen_component: setup_mini_script},
        {screen: "review-wallet-miniscript",  screen_component: review_wallet_miniscript},
        {screen: "miniscript-add-existing-key",screen_component: miniscript_add_existing_key},
    ]
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    sourceComponent: {
        var itemScreen = map_flow.find(function(e) {if (e.screen === flow_screen) return true; else return false})
        if (itemScreen) {
            return itemScreen.screen_component
        } else {
            return miniscript_Introduction
        }
    }
    function switchEnterMiniScript() {
        newWalletInfo.screenFlow = "enter-mini-script";
    }
    function switchSetupMiniScript() {
        newWalletInfo.screenFlow = "setup-mini-script";
    }
    function switchDraftWalletReviewMiniscript() {
        newWalletInfo.screenFlow = "review-wallet-miniscript";
    }

    Component {
        id: miniscript_Introduction
        QWalletMiniscriptIntroduction {
            onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
            onPrevClicked: QMLHandle.sendEvent(EVT.EVT_ADD_WALLET_SIGNER_CONFIGURATION_BACK)

            onSelectMiniscriptTemplate: (templateName) => {
                newWalletInfo.clearTimeMiniscript(templateName);
                if (templateName === "expanding-multisig") {
                    newWalletInfo.walletM = 2;
                    newWalletInfo.walletN = 2;
                    newWalletInfo.newWalletM = 2;
                    newWalletInfo.newWalletN = 3;
                } else if (templateName === "decaying-multisig") {
                    newWalletInfo.walletM = 2;
                    newWalletInfo.walletN = 3;
                    newWalletInfo.newWalletM = 1;
                    newWalletInfo.newWalletN = 3;
                } else if (templateName === "flexible-multisig") {
                    newWalletInfo.walletM = 2;
                    newWalletInfo.walletN = 3;
                    newWalletInfo.newWalletM = 3;
                    newWalletInfo.newWalletN = 3;
                } else if (templateName === "zen-hodl") {
                    newWalletInfo.walletM = 1;
                    newWalletInfo.walletN = 1;
                    newWalletInfo.newWalletM = 1;
                    newWalletInfo.newWalletN = 1;
                }

                newWalletInfo.screenFlow = templateName;
            }
            onEnterCustomMiniscript: (option) => {
                _taprootWarning.count = 0;
                newWalletInfo.customizeMiniscript = ""
                if (option === "enter-miniscript") {
                    switchEnterMiniScript()
                } else {
                    openfileDialog.open();
                }
            }
        }
    }

    Component {
        id: expanding_multisig
        QWalletTemplateMultisig {
            label.text: STR.STR_QML_1806
            templateMiniscript: "expanding-multisig"
            onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
            onPrevClicked: newWalletInfo.backScreen()
        }
    }

    Component {
        id: decaying_multisig
        QWalletTemplateMultisig {
            label.text: STR.STR_QML_1808
            templateMiniscript: "decaying-multisig"
            onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
            onPrevClicked: newWalletInfo.backScreen()
        }
    }

    Component {
        id: flexible_multisig
        QWalletTemplateMultisig {
            label.text: STR.STR_QML_1810
            templateMiniscript: "flexible-multisig"
            onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
            onPrevClicked: newWalletInfo.backScreen()
        }
    }

    Component {
        id: zen_hodl
        QWalletTemplateMultisig {
            label.text: STR.STR_QML_1885
            templateMiniscript: "zen-hodl"
            onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
            onPrevClicked: newWalletInfo.backScreen()
        }
    }

    Component {
        id: enter_mini_script
        QWalletEnterMiniscript {
            miniscript: newWalletInfo.customizeMiniscript
            onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
            onPrevClicked: newWalletInfo.backScreen()
            onNextClicked: {
                _taprootWarning.count += 1;
                if (newWalletInfo.isTaprootType(newWalletInfo.customizeMiniscript) && 
                    _taprootWarning.count < 2 &&
                    newWalletInfo.walletAddressType !== NUNCHUCKTYPE.TAPROOT) {
                    _taprootWarning.open()                   
                    return
                } else if (newWalletInfo.enterCustomMiniscript(newWalletInfo.customizeMiniscript)) {
                    if (newWalletInfo.configureWallet()) {
                       switchSetupMiniScript()
                       return
                    }
                }
                _taprootWarning.count = 0;
            }
            onMiniscriptEntered: (miniscript) => {
                newWalletInfo.customizeMiniscript = miniscript;
            }
        }
    }

    Component {
        id: setup_mini_script
        QSetupMiniscriptWallet {
            onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
            onPrevClicked: newWalletInfo.backScreen()
            onNextClicked: {
                switchDraftWalletReviewMiniscript()
            }
        }
    }
    Component {
        id: review_wallet_miniscript
        QDraftWalletReviewMiniscript {
            onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
            onPrevClicked: newWalletInfo.backScreen()
            onNextClicked: {
                AppModel.startCreateMiniscriptWallet();
                closeTo(NUNCHUCKTYPE.CURRENT_TAB)
            }
        }
    }

    Component {
        id: miniscript_add_existing_key
        QWalletMiniscriptAddAnExistingKey {
        }
    }

    FileDialog {
        id: openfileDialog
        fileMode: FileDialog.OpenFile
        onAccepted: {
            if(newWalletInfo.importMiniscriptFile(openfileDialog.file)) {
                switchEnterMiniScript()
            }
        }
    }

    QPopupInfoTwoButtons {
        id: _taprootWarning
        property int count: 0
        isVertical: false
        title: STR.STR_QML_661
        contentText: STR.STR_QML_1859
        labels: [STR.STR_QML_432,STR.STR_QML_097]
        funcs: [
            function() {  },
            function() { 
                newWalletInfo.requestChangeWalletTypeToTaproot()
            }
        ]
    }
}
