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
import "../../../../Screens/OnlineMode/SetupWallets"
import "../../../../Screens/OnlineMode/SetupWallets/TimeLocks"
import "../../../../Screens/OnlineMode/AddHardwareKeys"
import "../../../../../localization/STR_QML.js" as STR

QPopupOverlayScreen {
    id: _infoPopup
    signal nextClicked()
    property var claimData
    property bool isKeyHolderLimited: false
    property bool isMiniscript: true
    QScreenStateFlow {
        id: stateScreen
    }

    property var map_flow: [
        { screen: "your-plan-requires-two-inheritance-keys",  screen_component: your_plan_requires_two_inheritance_keys },
        { screen: "prepare-inheritance-key",                  screen_component: prepare_inheritance_key },
        { screen: "recover-inheritance-key",                  screen_component: recover_inheritance_key },
        { screen: "restore-seed-phrase-to-hardware-device",   screen_component: restore_seed_phrase_to_hardware_device },
        { screen: "your-plan-requires-two-inheritance-keys-added-one",  screen_component: your_plan_requires_two_inheritance_keys_added_one },
        { screen: "recover-an-existing-seed",               screen_component: _recoverAnExistingSeed },
        { screen: "eSCREEN_CLAIM_INHERITANCE_PLAN_RESULT_ERROR", screen_component: _resultClaimInheritancePlan},
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

    function onChainClaimSecond(claimData) {
        _infoPopup.claimData = claimData
        stateScreen.setScreenFlow("your-plan-requires-two-inheritance-keys-added-one")
        _infoPopup.open()
    }

    function onChainClaim(claimData) {
        _infoPopup.claimData = claimData
        if (claimData.inheritance_key_count == 1) {
            stateScreen.setScreenFlow("prepare-inheritance-key")
        } else {
            stateScreen.setScreenFlow("your-plan-requires-two-inheritance-keys")
        }
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
        QPrepareInheritanceKey { 
            onCloseClicked: _infoPopup.close()
            onPrevClicked: stateScreen.backScreen()
            onNextClicked: {
                if (key_option === "hardware-device") {
                    _hardwareAddKey.isInheritance = true
                    _hardwareAddKey.open()
                } else {
                    stateScreen.setScreenFlow("recover-inheritance-key")
                }
            }
        }
    }

    Component {
        id: recover_inheritance_key
        QRecoverInheritanceKey { 
            onCloseClicked: _infoPopup.close()
            onPrevClicked: stateScreen.backScreen()
            onNextClicked: {
                if (key_option === "hardware-device") {
                    stateScreen.setScreenFlow("restore-seed-phrase-to-hardware-device")
                } else {
                    // Recover via seed phrase backup not yet supported
                    stateScreen.setScreenFlow("recover-an-existing-seed")
                }                
            }
        }
    }

    Component {
        id: restore_seed_phrase_to_hardware_device
        QRestoreSeedPhraseToHardwareDevice {
            onCloseClicked: _infoPopup.close()
            onPrevClicked: stateScreen.backScreen()
            onNextClicked: {
                _hardwareAddKey.isInheritance = true
                _hardwareAddKey.open()
            }
        }
    }

    Component {
        id: your_plan_requires_two_inheritance_keys_added_one
        QYourPlanRequiresTwoInheritanceKeysAddedOne { 
            onCloseClicked: _infoPopup.close()
            onPrevClicked: stateScreen.backScreen()
            onNextClicked: {
                stateScreen.setScreenFlow("prepare-inheritance-key")
            }
        }
    }

    Component {
        id: _recoverAnExistingSeed
        QRecoverAnExistingSeed {            
            onCloseClicked: _infoPopup.close()
            onPrevClicked: stateScreen.backScreen()
        }
    }

    QPopupHardwareAddKey {
        id: _hardwareAddKey
        titleText: STR.STR_QML_2050 + "\n\n" + STR.STR_QML_2051
        supportWarning: false
        isKeyHolderLimited: _infoPopup.isKeyHolderLimited
        isMiniscript: _infoPopup.isMiniscript
        onNextClicked: {
            _hardwareAddKey.close()
            _checkFirmware.hadwareTag = hardware
            _checkFirmware.open()
        }
    }

    QPopupCheckYourFirmware {
        id: _checkFirmware
        onNextClicked: {
            _infoPopup.close()
            _checkFirmware.close()
            ServiceSetting.requestSignerInitialClaimSetup(hadwareTag)
            ServiceSetting.requestStartAddHardwareKey(hadwareTag)
        }
    }

     Component {
        id: _resultClaimInheritancePlan
        QScreenAddKeyResult {
            isSuccess: false
            resultTitle: STR.STR_QML_2045
            resultSubtitle: STR.STR_QML_2046
            onCloseClicked: _infoPopup.close()
            bottomRight: Row {
                spacing: 12
                QTextButton {
                    width: 120
                    height: 48
                    label.text: STR.STR_QML_097
                    label.font.pixelSize: 16
                    type: eTypeE
                    onButtonClicked: {
                        _infoPopup.close()
                    }
                }
            }
        }
    }
    QPopupInfoTwoButtons {
        id: _info
        signal yesClicked()
        title: STR.STR_QML_661
        labels: [STR.STR_QML_433,STR.STR_QML_432]
        funcs: [
            function() { yesClicked() },
            function() {}
        ]
    }
    Connections {
        target: AppModel
        onNotifySignerExist: {
            showPopupInfo(isSoftware, fingerPrint)
        }
    }
    function showPopupInfo(isSoftware, fingerPrint){
        if (isSoftware) {
            _info.contentText = STR.STR_QML_1283.arg(fingerPrint.toUpperCase())
            _info.contentTextTwo = STR.STR_QML_1284
            _info.open()
        }
        else {
            _info.contentText = STR.STR_QML_1283.arg(fingerPrint.toUpperCase())
            _info.contentTextTwo = ""
            _info.open()
        }
    }

    Connections {
        target: ServiceSetting.servicesTag
        onResultClaimInheritanceAlert: {
            if (result === "CLAIM_INHERITANCE_SECOND_KEY") {
                _infoPopup.onChainClaimSecond(claimData)
            } else if (result === "CLAIM_INHERITANCE_PLAN_RESULT_SUCCESS") {
                _infoPopup.close()
            } else if (result === "eSCREEN_CLAIM_INHERITANCE_PLAN_RESULT_ERROR") {
                stateScreen.setScreenFlow("eSCREEN_CLAIM_INHERITANCE_PLAN_RESULT_ERROR")
            }
        }
    }
}
