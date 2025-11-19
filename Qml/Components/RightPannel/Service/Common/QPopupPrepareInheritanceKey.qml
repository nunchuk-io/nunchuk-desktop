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
import "./../../../origins"
import "./../../../customizes"
import "./../../../customizes/Buttons"
import "./../../../customizes/Popups"
import "./../../../customizes/services"
import "./../../../customizes/Texts"
import "../../../RightPannel/Service/Common"
import "../../../../../localization/STR_QML.js" as STR

QPopupOverlayScreen {
    id: _infoPopup
    signal nextClicked()
    QScreenStateFlow {
        id: stateScreen
    }

    property var map_flow: [
        { screen: "your-plan-requires-two-inheritance-keys",  screen_component: your_plan_requires_two_inheritance_keys },
        { screen: "prepare-inheritance-key",                  screen_component: prepare_inheritance_key },
        { screen: "recover-inheritance-key",                  screen_component: recover_inheritance_key },
        { screen: "restore-seed-phrase-to-hardware-device",   screen_component: restore_seed_phrase_to_hardware_device },
        { screen: "your-plan-requires-two-inheritance-keys-added-one",  screen_component: your_plan_requires_two_inheritance_keys_added_one },
    ]

    content: {
        var itemScreen = map_flow.find(function(e) {if (e.screen === stateScreen.screenFlow) return true; else return false})
        if (itemScreen) {
            return prepare_inheritance_key //itemScreen.screen_component
        } else {
            _infoPopup.close()
            return null
        }
    }

    function onChainClaim() {
        stateScreen.setScreenFlow("your-plan-requires-two-inheritance-keys")
        _infoPopup.open()
    }

    Component {
        id: your_plan_requires_two_inheritance_keys
        QYourPlanRequiresTwoInheritanceKeys { 
            onCloseClicked: _infoPopup.close()
            onPrevClicked: _infoPopup.close()
            onNextClicked: {
                stateScreen.setScreenFlow("prepare-inheritance-key")
            }
        }
    }

    Component {
        id: prepare_inheritance_key
        QPrepareInheritanceKey { }
    }

    Component {
        id: recover_inheritance_key
        QRecoverInheritanceKey { }
    }

    Component {
        id: restore_seed_phrase_to_hardware_device
        QRestoreSeedPhraseToHardwareDevice { }
    }

    Component {
        id: your_plan_requires_two_inheritance_keys_added_one
        QYourPlanRequiresTwoInheritanceKeysAddedOne { 
            onCloseClicked: _infoPopup.close()
            onPrevClicked: _infoPopup.close()
            onNextClicked: {
                stateScreen.setScreenFlow("prepare-inheritance-key")
            }
        }
    }
}
