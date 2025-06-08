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
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Chats"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../Components/customizes/Signers"
import "../../Components/customizes/Wallets"
import "../LocalMode/Onboarding"
import "../../../localization/STR_QML.js" as STR

QScreen {
    property string screen_state: OnBoarding.screenFlow
    readonly property var map_screens: [
        {screen_name: "onboarding",                 screen_component: _onboarding},
        {screen_name: "unassistedWallet",           screen_component: _unassistedWallet},
        {screen_name: "assistedWallet",             screen_component: _assistedWallet},
        {screen_name: "hotWallet",                  screen_component: _hotWallet},
        {screen_name: "getMore",                    screen_component: _getMore},
        {screen_name: "addAWallet",                 screen_component: _addAWallet},
        {screen_name: "recoverHotWallet",           screen_component: _recoverHotWallet},
        {screen_name: "haveAnAdvisor",              screen_component: _haveAnAdvisor},
        {screen_name: "dontHaveAnAdvisor",          screen_component: _dontHaveAnAdvisor},
        {screen_name: "seedPhrase",                 screen_component: _seedPhrase},
        {screen_name: "confirmSeedPhrase",          screen_component: _confirmSeedPhrase},
        {screen_name: "claimAddAWallet",            screen_component: _claimAddAWallet},
        {screen_name: "updateWalletName",           screen_component: _updateWalletName}
    ]

    Loader {
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        sourceComponent: map_screens.find(function(e) {if (e.screen_name === screen_state) return true; else return false}).screen_component
    }
    Component {
        id: _onboarding
        QOnboarding {}
    }
    Component {
        id: _unassistedWallet
        QUnassistedWallet {}
    }
    Component {
        id: _assistedWallet
        QAssistedWallet {}
    }
    Component {
        id: _hotWallet
        QHotWallet {}
    }
    Component {
        id: _getMore
        QGetMoreOutOfNunchukWithAnAssistedWallet {}
    }
    Component {
        id: _addAWallet
        QAddAWallet {
            hasWallet: true
        }
    }
    Component {
        id: _claimAddAWallet
        QAddAWallet {
            hasWallet: false
        }
    }
    Component {
        id: _recoverHotWallet
        QRecoverHotWallet {}
    }
    Component {
        id: _haveAnAdvisor
        QHaveAnAdvisor {}
    }
    Component {
        id: _dontHaveAnAdvisor
        QDontHaveAnAdvisor {}
    }
    Component {
        id: _seedPhrase
        QSeedPhrase {}
    }
    Component {
        id: _confirmSeedPhrase
        QConfirmSeedPhrase {}
    }
    Component {
        id: _updateWalletName
        QRecoverWalletNameEdit {
            onNextClicked: {
                var _input = {
                    type: "update-new-wallet-name",
                    new_wallet_name: newWalletName,
                }
                QMLHandle.sendEvent(EVT.EVT_ONBOARDING_ACTION_REQUEST, _input)
            }
        }
    }
}
