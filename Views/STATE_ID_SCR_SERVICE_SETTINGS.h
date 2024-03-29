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


#ifndef STATE_ID_SCR_SERVICE_SETTINGS_H
#define STATE_ID_SCR_SERVICE_SETTINGS_H

#include <QObject>
#include <QVariant>
#include "ViewsDefines.h"
#include "ViewsEnums.h"

void SCR_SERVICE_SETTINGS_Entry(QVariant msg);
void SCR_SERVICE_SETTINGS_Exit(QVariant msg);
void EVT_CLAIM_INHERITANCE_CHECK_REQUEST_HANDLER(QVariant msg);
void EVT_CO_SIGNING_SERVER_KEY_UPDATE_REQUEST_HANDLER(QVariant msg);
void EVT_CO_SIGNING_SERVER_KEY_UPDATE_SUCCEED_HANDLER(QVariant msg);
void EVT_SERVICE_SELECT_WALLET_REQUEST_HANDLER(QVariant msg);
void EVT_INHERITANCE_PLAN_FINALIZE_REQUEST_HANDLER(QVariant msg);
void EVT_SERVICE_SUPPORT_REQUEST_HANDLER(QVariant msg);
void EVT_INHERITANCE_WITHDRAW_BALANCE_REQUEST_HANDLER(QVariant msg);
void EVT_EDIT_YOUR_INHERITANCE_PLAN_REQUEST_HANLDER(QVariant msg);
void EVT_KEY_RECOVERY_REQUEST_HANDLER(QVariant msg);
void EVT_SELECT_YOUR_LOCKDOWN_PERIOD_REQUEST_HANDLER(QVariant msg);
void EVT_WALLET_CO_SIGN_POLICE_REQUEST_HANDLER(QVariant msg);
void EVT_LOCKDOWN_SUCCESS_REQUEST_HANDLER(QVariant msg);
void EVT_SETUP_SECURITY_QUESTION_REQUEST_HANDLER(QVariant msg);

#endif // STATE_ID_SCR_SERVICE_SETTINGS_H
