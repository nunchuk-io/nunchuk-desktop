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
import "../../origins"
import "../../customizes"
import "../../customizes/Chats"
import "../../customizes/Popups"
import "../../customizes/Wallets"
import "../../../../localization/STR_QML.js" as STR

QPopupOverlayScreen {
    id: _infoPopup
    QScreenStateFlow {
        id: stateScreen
    }

    readonly property var map_flow: [
        {screen: "flexible-multisig",         screen_component: flexible_multisig},
        {screen: "expanding-multisig",        screen_component: expanding_multisig},
        {screen: "decaying-multisig",         screen_component: decaying_multisig},
        {screen: "zen-hodl",                  screen_component: zen_hodl},
        {screen: "enter-mini-script",         screen_component: enter_mini_script},
        {screen: "miniscript-add-existing-key",screen_component: miniscript_add_existing_key},
    ]

    content: {
        var itemScreen = map_flow.find(function(e) {if (e.screen === stateScreen.screenFlow) return true; else return false})
        if (itemScreen) {
            return itemScreen.screen_component
        } else {
            _infoPopup.close()
            return null
        }
    }

    function selectedTemplate(templateName) {
        newWalletInfo.walletType = NUNCHUCKTYPE.MINISCRIPT;
        if (templateName === "expanding-multisig") {
            newWalletInfo.walletN = 2;
            newWalletInfo.walletM = 2;
            newWalletInfo.newWalletN = 3;
            newWalletInfo.newWalletM = 2;            
        } else if (templateName === "decaying-multisig") {
            newWalletInfo.walletN = 3;
            newWalletInfo.walletM = 2;
            newWalletInfo.newWalletN = 3;
            newWalletInfo.newWalletM = 1;            
        } else if (templateName === "flexible-multisig") {
            newWalletInfo.walletN = 3;
            newWalletInfo.walletM = 2;
            newWalletInfo.newWalletN = 3;
            newWalletInfo.newWalletM = 3;            
        } else if (templateName === "zen-hodl") {
            newWalletInfo.walletN = 2;
            newWalletInfo.walletM = 2;
            newWalletInfo.newWalletN = 2;
            newWalletInfo.newWalletM = 2;            
        }
        stateScreen.setScreenFlow(templateName)
        _infoPopup.open()
    }

    function enterCustom(option) {
        _taprootWarning.count = 0;
        newWalletInfo.customizeMiniscript = ""
        if (option === "enter-miniscript") {
            _infoPopup.switchEnterMiniScript()
        } else {
            openfileDialog.open();
        }
    }

    function switchEnterMiniScript() {
        stateScreen.setScreenFlow("enter-mini-script")
        if (!_infoPopup.opened) {
            _infoPopup.open()
        }
    }
    Component {
        id: expanding_multisig
        QWalletTemplateMultisig {
            label.text: STR.STR_QML_1806
            templateMiniscript: "expanding-multisig"
            onCloseClicked: _infoPopup.close()
            onPrevClicked: stateScreen.backScreen()
        }
    }

    Component {
        id: decaying_multisig
        QWalletTemplateMultisig {
            label.text: STR.STR_QML_1808
            templateMiniscript: "decaying-multisig"
            onCloseClicked: _infoPopup.close()
            onPrevClicked: stateScreen.backScreen()
        }
    }

    Component {
        id: flexible_multisig
        QWalletTemplateMultisig {
            label.text: STR.STR_QML_1810
            templateMiniscript: "flexible-multisig"
            onCloseClicked: _infoPopup.close()
            onPrevClicked: stateScreen.backScreen()
        }
    }

    Component {
        id: zen_hodl
        QWalletTemplateMultisig {
            label.text: STR.STR_QML_1885
            templateMiniscript: "zen-hodl"
            onCloseClicked: _infoPopup.close()
            onPrevClicked: stateScreen.backScreen()
        }
    }

    Component {
        id: enter_mini_script
        QWalletEnterMiniscript {
            miniscript: newWalletInfo.customizeMiniscript
            onCloseClicked: _infoPopup.close()
            onPrevClicked: stateScreen.backScreen()
            onNextClicked: {
                _taprootWarning.count += 1;

                var isValidMiniscript = newWalletInfo.isValidMiniscriptTemplate(newWalletInfo.customizeMiniscript)
                var isValidTapscript  = newWalletInfo.isValidTapscriptTemplate(newWalletInfo.customizeMiniscript)
                var isTaprootWallet   = newWalletInfo.walletAddressType === NUNCHUCKTYPE.TAPROOT

                if (!isValidMiniscript && isValidTapscript && (_taprootWarning.count < 2) && !isTaprootWallet){
                    _taprootWarning.open()                   
                    return
                }
                else if (newWalletInfo.enterCustomMiniscript(newWalletInfo.customizeMiniscript)) {
                    if (newWalletInfo.configureWallet()) {
                       _infoPopup.close()
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
