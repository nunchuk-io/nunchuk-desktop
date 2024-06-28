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


#ifndef STATE_ID_ROOT_H
#define STATE_ID_ROOT_H

#include <QObject>
#include <QVariant>
#include "ViewsDefines.h"
#include "ViewsEnums.h"

void ROOT_Entry(QVariant msg);
void ROOT_Exit(QVariant msg);
void EVT_STARTING_APPLICATION_LOCALMODE_HANDLER(QVariant msg);
void EVT_STARTING_APPLICATION_ONLINEMODE_HANDLER(QVariant msg);
void EVT_STARTING_APPLICATION_ONLINE_HWL_HANDLER(QVariant msg);
void EVT_SETTING_ACCOUNT_CHANGE_PASSWORD_HANDLER(QVariant msg);
void EVT_ONS_CLOSE_ALL_REQUEST_HANDLER(QVariant msg);
void EVT_ROOT_SIGN_IN_PRIMARY_KEY_REQUEST_HANDLER(QVariant msg);
void EVT_ROOT_CREATE_PRIMARY_KEY_REQUEST_HANDLER(QVariant msg);
void EVT_GOTO_HOME_WALLET_TAB_HANDLER(QVariant msg);
void EVT_ONS_CLOSE_REQUEST_HANDLER(QVariant msg);
void EVT_LOGIN_DB_REQUEST_HANDLER(QVariant msg);
void EVT_ROOT_PROMT_PIN_HANDLER(QVariant msg);
void EVT_ROOT_PROMT_PASSPHRASE_HANDLER(QVariant msg);
void EVT_LOGIN_MATRIX_REQUEST_HANDLER(QVariant msg);
void EVT_GOTO_HOME_CHAT_TAB_HANDLER(QVariant msg);
void EVT_ONLINE_ONS_CLOSE_REQUEST_HANDLER(QVariant msg);
void EVT_SHOW_TOAST_MESSAGE_HANDLER(QVariant msg);
void EVT_ROOT_UPDATE_PROFILE_HANDLER(QVariant msg);
void EVT_SHOW_CREATE_ACCOUNT_REQUEST_HANDLER(QVariant msg);
void EVT_GOTO_APP_SETTINGS_TAB_HANDLER(QVariant msg);
void EVT_SIGN_IN_REQUEST_HANDLER(QVariant msg);
void EVT_ROOT_ENTRY_PRIMARY_KEY_REQUEST_HANDLER(QVariant msg);
void EVT_LOGIN_WITH_SOFTWARE_KEY_REQUEST_HANDLER(QVariant msg);
void EVT_GOTO_SERVICE_SETTING_TAB_HANDLER(QVariant msg);
void EVT_CLOSE_TO_SERVICE_SETTINGS_REQUEST_HANDLER(QVariant msg);
void EVT_NUNCHUK_LOGIN_SUCCEEDED_HANDLER(QVariant msg);
void EVT_HEALTH_CHECK_STARTING_REQUEST_HANDLER(QVariant msg);
void EVT_DUMMY_TRANSACTION_INFO_REQUEST_HANDLER(QVariant msg);
void EVT_DASHBOARD_ALERT_SUCCESS_REQUEST_HANDLER(QVariant msg);
void EVT_SHARE_YOUR_SECRET_REQUEST_HANDLER(QVariant msg);
void EVT_REENTER_YOUR_PASSWORD_REQUEST_HANDLER(QVariant msg);
void EVT_ONBOARDING_REQUEST_HANDLER(QVariant msg);
void EVT_REPLACE_SELECT_KEY_REQUEST_HANDLER(QVariant msg);

#endif // STATE_ID_ROOT_H
