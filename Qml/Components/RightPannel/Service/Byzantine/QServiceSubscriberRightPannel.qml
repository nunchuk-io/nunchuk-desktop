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
import "../Common"
import "../Byzantine" as B
import "../HoneyBadger-IronHand" as HBIR

Item {
    readonly property var option_map: [
        {screen:_EMERGENCY_LOCKDOWN,                    screen_component: _emergency_lockdown       },
        {screen:_KEY_RECOVERY,                          screen_component: _key_recovery             },
        {screen:_VIEW_INHERITANCE_PLANING,              screen_component: _view_inheritance_planing },
        {screen:_CLAIM_AN_INHERITANCE,                  screen_component: _claim_an_inheritance     },
        {screen:_PLATFORM_KEY_CO_SIGNING_POLICIES,      screen_component: _platform_key_co_signing_policies_B },
        {screen:_GET_ADDITIONAL_WALLETS,                screen_component: "" },
        {screen:_REPLACE_KEY_IN_AN_ASSISTED_WALLET,     screen_component: "" },
        {screen:_ORDER_NEW_HARDWARE,                    screen_component: "" },
        {screen:_MANAGE_SUBSCRIPTION,                   screen_component: "" },
    ]
    property var itemOption: option_map.find(function(e) {if (e.screen === ServiceSetting.optionIndex) return true; else return false})

    function platform_key_co_signing_policies() {
        if (ServiceSetting.walletInfo === null) {
            return _platform_key_co_signing_policies_B
        } else {
            return (ServiceSetting.walletInfo.isHoneyBadger || ServiceSetting.walletInfo.isIronHand) ? _platform_key_co_signing_policies_HBIR : _platform_key_co_signing_policies_B
        }
    }

    Loader {
        anchors.fill: parent
        sourceComponent: if(ServiceSetting.optionIndex !== _PLATFORM_KEY_CO_SIGNING_POLICIES) {
                             itemOption.screen_component
                         }
                         else {
                             platform_key_co_signing_policies()
                         }
    }
    Component {
        id: _claim_an_inheritance
        QServiceClaimAnInheritance {
        }
    }
    Component {
        id: _emergency_lockdown
        QServiceEmergencyLockdown {
        }
    }
    Component {
        id: _key_recovery
        QServiceKeyRecovery {
        }
    }
    Component {
        id: _view_inheritance_planing
        QServiceViewInheritancePlaning {
        }
    }
    Component {
        id: _platform_key_co_signing_policies_B
        B.QServicePlatformKeyCoSigningPolicies {
        }
    }
    Component {
        id: _platform_key_co_signing_policies_HBIR
        HBIR.QServicePlatformKeyCoSigningPolicies {
        }
    }
}
