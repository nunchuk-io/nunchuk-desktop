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
import "../../Components/customizes/services"
import "../OnlineMode/SetupWallets"
import "../OnlineMode/AddHardwareKeys"
import "../OnlineMode/ServerKeys"
import "../OnlineMode/Healths"
import "../OnlineMode/RecurringPayments"
import "../../../localization/STR_QML.js" as STR

QScreen {
    property int flow: AppModel.walletInfo.flow
    property string config_flow: GroupWallet.dashboardInfo.configFlow
    readonly property var map_flow: [
        {flow_action: AlertType.GROUP_WALLET_SETUP,                   screen_component: _health_check_procedure},
        {flow_action: AlertType.HEALTH_CHECK_STATUS,                  screen_component: _health_check_status},
        {flow_action: AlertType.UPDATE_SERVER_KEY,                    screen_component: _signature_require_one},
        {flow_action: AlertType.HEALTH_CHECK_REQUEST,                 screen_component: _health_check_procedure},
        {flow_action: AlertType.HEALTH_CHECK_PENDING,                 screen_component: _health_check_procedure},
        {flow_action: AlertType.SERVICE_TAG_POLICY_UPDATE,            screen_component: _signature_require_one},
        {flow_action: AlertType.SERVICE_TAG_INHERITANCE_PLAN_CREATE,  screen_component: _signature_require_one},
        {flow_action: AlertType.SERVICE_TAG_INHERITANCE_PLAN_UPDATE,  screen_component: _signature_require_one},
        {flow_action: AlertType.SERVICE_TAG_INHERITANCE_PLAN_CANCEL,  screen_component: _signature_require_one},

        {flow_action: AlertType.REQUEST_INHERITANCE_PLANNING,         screen_component: _signature_require_two},
        {flow_action: AlertType.CREATE_INHERITANCE_PLAN,              screen_component: _signature_require_one},
        {flow_action: AlertType.UPDATE_INHERITANCE_PLAN,              screen_component: _signature_require_one},
        {flow_action: AlertType.CANCEL_INHERITANCE_PLAN,              screen_component: _signature_require_one},
        {flow_action: AlertType.KEY_RECOVERY_REQUEST,                 screen_component: _key_recovery_requested},

        {flow_action: AlertType.CREATE_RECURRING_PAYMENT,             screen_component: _authorize_recurring_payment},
        {flow_action: AlertType.RECURRING_PAYMENT_REQUEST,            screen_component: _signature_require_three},

        {flow_action: AlertType.CANCEL_RECURRING_PAYMENT,             screen_component: _cancel_recurring_payment},
        {flow_action: AlertType.RECURRING_PAYMENT_CANCELATION_PENDING,screen_component: _cancel_recurring_payment},
        {flow_action: AlertType.SERVICE_TAG_UPDATE_SECURITY_QUESTION, screen_component: _signature_require_one},
    ]
    readonly property var map_screens: [
        {screen_name: "health-check-procedure",         screen_component: _health_check_procedure},
        {screen_name: "health-check-request-sent",      screen_component: _health_check_request_sent},
    ]
    property var _health_check_status: map_screens.find(function(e) {if (e.screen_name === config_flow) return true; else return false}).screen_component

    Loader {
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        sourceComponent: map_flow.find(function(e) {if (e.flow_action === flow) return true; else return false}).screen_component
    }
    Component {
        id: _signature_require_one
        QSignaturesRequired {
            description_top: STR.STR_QML_1012
            description_bottom: STR.STR_QML_1013
        }
    }

    Component {
        id: _signature_require_two
        QSignaturesRequired {
            description_top: STR.STR_QML_1038
            description_bottom: STR.STR_QML_1013
        }
    }

    Component {
        id: _signature_require_three
        QSignaturesRequired {
            description_top: STR.STR_QML_1041
            description_bottom: STR.STR_QML_1013
        }
    }

    Component {
        id: _authorize_recurring_payment
        QAuthorizeRecurringPayment {
        }
    }

    Component {
        id: _cancel_recurring_payment
        QCancelRecurringPayment {
            title: STR.STR_QML_1146
        }
    }

    Component {
        id: _signature_require_four
        QSignaturesRequired {
            description_top: STR.STR_QML_1039
            description_bottom: STR.STR_QML_1040
        }
    }

    Component {
        id: _health_check_procedure
        QHealthCheckProcedure {}
    }
    Component {
        id: _health_check_request_sent
        QHealthCheckRequestSent {}
    }
    Component {
        id: _key_recovery_requested
        QKeyRecoveryRequested {}
    }
}
