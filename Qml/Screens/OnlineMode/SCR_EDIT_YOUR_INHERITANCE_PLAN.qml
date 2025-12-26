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
import QtQuick.Controls 1.4
import QtGraphicalEffects 1.12
import QtQuick.Controls.Styles 1.4
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Chats"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../OnlineMode/Inheritances"
import "../OnlineMode/SetupWallets/TimeLocks"
import "../../../localization/STR_QML.js" as STR

QScreen {
    property var inheritancePlanInfo: ServiceSetting.walletInfo.inheritancePlanInfo
    property int actionPlan: inheritancePlanInfo.actionPlan
    readonly property var map_screens: [
        // {action_plan: ServiceType.IE_ACTIVATION_DATE,       screen_component: _edit_setup_an_activation_date},
        {action_plan: ServiceType.IE_ACTIVATION_DATE,       screen_component: _edit_setup_offchain_timelock},
        {action_plan: ServiceType.IE_LEAVE_MESSAGE,         screen_component: _edit_leave_a_message},
        {action_plan: ServiceType.IE_NOTIFICATION,          screen_component: _edit_notification_preferences},
        {action_plan: ServiceType.IE_BUFFER_PERIOD,         screen_component: _edit_setup_buffer_period},
    ]
    Loader {
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        sourceComponent: map_screens.find(function(e) {if (e.action_plan === actionPlan) return true; else return false}).screen_component
    }
    Component {
        id: _edit_setup_an_activation_date
        QEditInheritanceSetupAnActivationDate {
            planInfo: inheritancePlanInfo.planInfo
        }
    }
    Component {
        id: _edit_leave_a_message
        QEditInheritanceLeaveAMessage {
            planInfo: inheritancePlanInfo.planInfo
        }
    }
    Component {
        id: _edit_notification_preferences
        QEditInheritanceNotificationPreferences {
            planInfo: inheritancePlanInfo.planInfo
        }
    }

    Component {
        id: _edit_setup_buffer_period
        QEditInheritanceSetupBufferPeriod {
            planInfo: inheritancePlanInfo.planInfo
        }
    }
    Component {
        id: _edit_setup_offchain_timelock
        QSetupOffChainTimelockWallet {
            planInfo: inheritancePlanInfo.planInfo
        }
    }
}
