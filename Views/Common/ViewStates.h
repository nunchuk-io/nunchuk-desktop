/************************************************************************* 
* This file is part of the Nunchuk software (https://nunchuk.io/)        * 
* Copyright (C) 2020-2022 Enigmo                                         * 
* Copyright (C) 2022 Nunchuk                                             * 
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


#ifndef VIEWSTATES_H
#define VIEWSTATES_H

#include "ViewsDefines.h"
#include "ViewsEnums.h"
#include "STATE_ID_ROOT.h"
#include "STATE_ID_SCR_ADD_HARDWARE.h"
#include "STATE_ID_SCR_ADD_HARDWARE_ASK.h"
#include "STATE_ID_SCR_ADD_HARDWARE_EXIST.h"
#include "STATE_ID_SCR_ADD_HARDWARE_SIGNER.h"
#include "STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET.h"
#include "STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT.h"
#include "STATE_ID_SCR_ADD_NEW_SIGNER.h"
#include "STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET.h"
#include "STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER.h"
#include "STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT.h"
#include "STATE_ID_SCR_ADD_WALLET.h"
#include "STATE_ID_SCR_ADD_WALLET_CONFIRMATION.h"
#include "STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION.h"
#include "STATE_ID_SCR_APP_SETTINGS.h"
#include "STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET.h"
#include "STATE_ID_SCR_BACKUP_SHARED_WALLET.h"
#include "STATE_ID_SCR_CHANGE_EMAIL.h"
#include "STATE_ID_SCR_CHANGE_PASSWORD.h"
#include "STATE_ID_SCR_CONFIGURE_SHARED_WALLET.h"
#include "STATE_ID_SCR_CONSOLIDATE_OUTPUT.h"
#include "STATE_ID_SCR_CREATE_ACCOUNT.h"
#include "STATE_ID_SCR_CREATE_NEW_SEED.h"
#include "STATE_ID_SCR_CREATE_SHARED_WALLET.h"
#include "STATE_ID_SCR_CREATE_TRANSACTION.h"
#include "STATE_ID_SCR_DASHBOARD_ALERT_INFO.h"
#include "STATE_ID_SCR_DASHBOARD_ALERT_SUCCESS.h"
#include "STATE_ID_SCR_DUMMY_TRANSACTION_INFO.h"
#include "STATE_ID_SCR_EDIT_YOUR_INHERITANCE_PLAN.h"
#include "STATE_ID_SCR_ENTER_BACKUP_PASSWORD.h"
#include "STATE_ID_SCR_HEALTH_CHECK_STARTING.h"
#include "STATE_ID_SCR_HOME.h"
#include "STATE_ID_SCR_HOME_ONLINE.h"
#include "STATE_ID_SCR_INHERITANCE_CONFIRM_TRANSACTION.h"
#include "STATE_ID_SCR_INHERITANCE_TRANSACTION_DETAILS.h"
#include "STATE_ID_SCR_INHERITANCE_WITHDRAW_BALANCE.h"
#include "STATE_ID_SCR_INPUT_PASSPHRASE.h"
#include "STATE_ID_SCR_INPUT_PIN.h"
#include "STATE_ID_SCR_KEY_HEALTH_CHECK_STATUS.h"
#include "STATE_ID_SCR_KEY_RECOVERY.h"
#include "STATE_ID_SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION.h"
#include "STATE_ID_SCR_LOCKDOWN_ANSER_SECURITY_QUESTION.h"
#include "STATE_ID_SCR_LOCKDOWN_SUCCESS.h"
#include "STATE_ID_SCR_LOGIN_ONLINE.h"
#include "STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY.h"
#include "STATE_ID_SCR_MASTER_SIGNER_INFO.h"
#include "STATE_ID_SCR_ONBOARDING.h"
#include "STATE_ID_SCR_ONLINE_ADD_CONTACTS.h"
#include "STATE_ID_SCR_PENDING_REQUEST.h"
#include "STATE_ID_SCR_PRIMARY_KEY_ACCOUNT.h"
#include "STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION.h"
#include "STATE_ID_SCR_RECEIVE.h"
#include "STATE_ID_SCR_RECOVER_SHARED_WALLET.h"
#include "STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER.h"
#include "STATE_ID_SCR_RECURRING_PAYMENTS.h"
#include "STATE_ID_SCR_REENTER_YOUR_PASSWORD.h"
#include "STATE_ID_SCR_REMOTE_SIGNER_INFO.h"
#include "STATE_ID_SCR_REPLACE_KEYS.h"
#include "STATE_ID_SCR_REPLACE_PRIMARY_KEY.h"
#include "STATE_ID_SCR_REPLACE_SELECT_KEY.h"
#include "STATE_ID_SCR_REVIEW_SHARED_WALLET.h"
#include "STATE_ID_SCR_SELECT_SERVER.h"
#include "STATE_ID_SCR_SELECT_WALLET_CO_SIGN_POLICE.h"
#include "STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD.h"
#include "STATE_ID_SCR_SEND.h"
#include "STATE_ID_SCR_SERVICE_SETTINGS.h"
#include "STATE_ID_SCR_SETUP_ANSWER_SECURITY_QUESTION.h"
#include "STATE_ID_SCR_SETUP_SECURITY_QUESTION.h"
#include "STATE_ID_SCR_SHARED_WALLET_CONFIGURE.h"
#include "STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION.h"
#include "STATE_ID_SCR_SHARE_YOUR_SECRETS.h"
#include "STATE_ID_SCR_SIGN_IN.h"
#include "STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY.h"
#include "STATE_ID_SCR_SIGN_IN_MANUALLY.h"
#include "STATE_ID_SCR_SIGN_IN_VIA_XPUB.h"
#include "STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION.h"
#include "STATE_ID_SCR_STARTING_CREATE_SIGNER.h"
#include "STATE_ID_SCR_TAPSIGNER_RECOVERED_SUCCESS.h"
#include "STATE_ID_SCR_TRANSACTION_HISTORY.h"
#include "STATE_ID_SCR_TRANSACTION_INFO.h"
#include "STATE_ID_SCR_UNLOCK_DB.h"
#include "STATE_ID_SCR_UPDATE_PROFILE.h"
#include "STATE_ID_SCR_UTXOS.h"
#include "STATE_ID_SCR_UTXO_OUTPUT.h"
#include "STATE_ID_SCR_WALLET_CHANGE_ADDRESSES.h"
#include "STATE_ID_SCR_WALLET_INFO.h"
#include "STATE_ID_TOAST_MESSAGE_DISPLAY.h"

#endif // VIEWSTATES_H
