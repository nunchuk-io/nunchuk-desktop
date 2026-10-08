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
import "../../../Components/customizes/Wallets"
import "../../OnlineMode/SetupWallets"
import "../../OnlineMode/SetupWallets/TimeLocks"
import "../../OnlineMode/AddHardwareKeys"
import "../../OnlineMode/ServerKeys"
import "../../OnlineMode/Healths"
import "../../OnlineMode/Inheritances"
import "../../OnlineMode/RecurringPayments"
import "../../../Components/customizes/services"
import "../../../../localization/STR_QML.js" as STR

Loader {
    property int flow: AppModel.walletInfo.flow
    property var walletInfo: AppModel.walletInfo
    property string config_flow: GroupWallet.dashboardInfo.configFlow
    readonly property var map_flow: [
        {flow_action: AlertType.GROUP_WALLET_PENDING,        screen_component: _group_wallet_creation_pending},
        {flow_action: AlertType.WALLET_PENDING,              screen_component: _user_wallet_creation_pending},
        {flow_action: AlertType.GROUP_WALLET_SETUP,          screen_component: walletSetup},
        {flow_action: AlertType.UPDATE_SERVER_KEY,           screen_component: _platform_key_policy_change},
        {flow_action: AlertType.HEALTH_CHECK_PENDING,        screen_component: _health_check_pending},
        {flow_action: AlertType.HEALTH_CHECK_REQUEST,        screen_component: _health_check_request},
        {flow_action: AlertType.UPDATE_INHERITANCE_PLAN,     screen_component: _update_inheritance_plan},
        {flow_action: AlertType.CANCEL_INHERITANCE_PLAN,     screen_component: _cancel_inheritance_plan},
        {flow_action: AlertType.CREATE_INHERITANCE_PLAN,     screen_component: _create_inheritance_plan},
        {flow_action: AlertType.REQUEST_INHERITANCE_PLANNING,screen_component: _inheritance_planning_request},
        {flow_action: AlertType.KEY_RECOVERY_APPROVED,       screen_component: enter_backup_password},
        {flow_action: AlertType.CREATE_INHERITANCE_PLAN_SUCCESS,screen_component: _inheritance_plan_has_been_setup},
        {flow_action: AlertType.RECURRING_PAYMENT_REQUEST,   screen_component:_recurring_payment_request},
        {flow_action: AlertType.RECURRING_PAYMENT_CANCELATION_PENDING,   screen_component:_cancel_payment_request},
        {flow_action: AlertType.KEY_RECOVERY_REQUEST,        screen_component: _request_key_recovery_for},
        {flow_action: AlertType.UPDATE_SECURITY_QUESTIONS,   screen_component: _update_security_questions},
        {flow_action: AlertType.MANAGE_GROUP_CHAT_HISTORY,   screen_component: _manageGroupChatHistory},
        {flow_action: AlertType.CHANGE_EMAIL_REQUEST,        screen_component: _change_email_request},
        {flow_action: AlertType.HEALTH_CHECK_REMINDER,       screen_component: _healthCheckReminderAlert},
        {flow_action: AlertType.KEY_REPLACEMENT_PENDING,     screen_component: _key_replacement_pending},
        {flow_action: AlertType.BACKUP_WALLET,               screen_component: bsms_file_success},
        {flow_action: AlertType.CHANGE_TIMELOCK_TYPE,        screen_component: _user_wallet_creation_pending},
        {flow_action: AlertType.KEY_TIMELOCK_UPDATE_PENDING, screen_component: replace_or_change_timelock},
    ]
    readonly property var map_screens: [
        {screen_name: "register-gapped-device",         screen_component: _register_gapped_device},
        {screen_name: "register-COLDCARD",              screen_component: _register_COLDCARD},
        {screen_name: "register-BitBox",                screen_component: _register_BitBox},
        {screen_name: "register-Done",                  screen_component: _accessing_wallet_configuration},
        {screen_name: "accessing-wallet-configuration", screen_component: _accessing_wallet_configuration},
        {screen_name: "register-claim",                 screen_component: _claim_your_key},
        {screen_name: "register-wallet-hardware",       screen_component: register_wallet_hardware},
    ]

    function walletSetup() {
        return map_screens.find(function(e) {if (e.screen_name === config_flow) return true; else return false}).screen_component
    }

    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    sourceComponent: {
        var item = map_flow.find(function(e) {if (e.flow_action === flow) return true; else return false}).screen_component
        return flow === AlertType.GROUP_WALLET_SETUP ? item() : item
    }

    Component {
        id: _group_wallet_creation_pending
        QWalletCreationPendingOnchain {

        }
    }
    Component {
        id: _user_wallet_creation_pending
        QWalletCreationPendingOnchain {
            isRead: true
        }
    }
    Component {
        id: _register_gapped_device
        QRegisterAirGappedDevice {}
    }
    Component {
        id: _register_COLDCARD
        QRegisterCOLDCARD {}
    }
    Component {
        id: _register_BitBox
        QRegisterBitbox {}
    }
    Component {
        id: _claim_your_key
        QClaimYourKey {}
    }
    Component {
        id: _accessing_wallet_configuration
        QAccessingWalletConfiguration {}
    }
    Component {
        id: _platform_key_policy_change
        QPlatformKeyPolicyChange {}
    }
    Component {
        id: _health_check_pending
        QHealthCheckPending {}
    }
    Component {
        id: _health_check_request
        QHealthCheckRequested {}
    }
    Component {
        id: _healthCheckReminderAlert
        QHealthCheckReminderAlert {}
    }

    Component {
        id: _update_inheritance_plan
        QInheritancePlanInfo {
            description: STR.STR_QML_1036
            descriptionHb: STR.STR_QML_1215
            planInfo: AppModel.walletInfo.inheritancePlanInfo.planInfoUpdate
        }
    }

    Component {
        id: _cancel_inheritance_plan
        QInheritancePlanInfo {
            description: STR.STR_QML_1034
            descriptionHb: STR.STR_QML_1214
            planIsCancel: true
        }
    }

    Component {
        id: _create_inheritance_plan
        QInheritancePlanInfo {
            description: STR.STR_QML_1035
            planInfo: AppModel.walletInfo.inheritancePlanInfo.planInfoNew
        }
    }

    Component {
        id: _inheritance_planning_request
        QInheritancePlanningRequest {
        }
    }
    Component {
        id: enter_backup_password
        QEnterBackupPassword {
            onPrevClicked: closeTo(NUNCHUCKTYPE.SERVICE_TAB)
            onNextClicked: {
                var _input = {
                    type: "enter-backup-password",
                    password: backup_password.textInputted
                }
                QMLHandle.sendEvent(EVT.EVT_DASHBOARD_ALERT_INFO_ENTER, _input)
            }
        }
    }

    Component {
        id: _inheritance_plan_has_been_setup
        QInheritancePlanHasBeenSetup {

        }
    }

    Component {
        id: _recurring_payment_request
        QRecurringPaymentRequest {

        }
    }

    Component {
        id: _cancel_payment_request
        QRecurringPaymentCancelationPending {

        }
    }

    Component {
        id: _request_key_recovery_for
        QRequestedKeyRecoveryFor {

        }
    }

    Component {
        id: _update_security_questions
        QSecurityQuestionsUpdate {

        }
    }

    Component {
        id: _manageGroupChatHistory
        QManageGroupChatHistory {

        }
    }

    Component {
        id: _change_email_request
        QChangeEmailRequest {

        }
    }

    Component {
        id: _key_replacement_pending
        QKeyReplacementInProgress {

        }
    }

    Component {
        id: bsms_file_success
        QSaveYourWalletBSMSFile {
            onNextClicked: {
                savefileDialog.currentFile = StandardPaths.writableLocation(StandardPaths.DocumentsLocation) + "/"
                            + RoomWalletData.getValidFilename(walletInfo.walletName)
                            + ".bsms"
                savefileDialog.open()
            }
        }
    }

    Component {
        id: register_wallet_hardware
        QRegisterWalletOnHardware {
        }
    }

    Component {
        id: replace_or_change_timelock
        QWalletReplaceKeyOrChangeTimelock {

        }
    }

    FileDialog {
        id: savefileDialog
        fileMode: FileDialog.SaveFile
        onAccepted: {
            walletInfo.requestExportWalletViaBSMS(savefileDialog.currentFile)
            GroupWallet.dashboardInfo.markRead()
            GroupWallet.dashboardInfo.dismissAlert()
            closeTo(NUNCHUCKTYPE.CURRENT_TAB)
        }
    }
}
