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


#ifndef STATE_ID_SCR_APP_SETTINGS_H
#define STATE_ID_SCR_APP_SETTINGS_H

#include <QObject>
#include <QVariant>
#include "ViewsDefines.h"
#include "ViewsEnums.h"

void SCR_APP_SETTING_Entry(QVariant msg);
void SCR_APP_SETTING_Exit(QVariant msg);
void EVT_APP_SETTING_DELETE_ACCOUNT_REQUEST_HANDLER(QVariant msg);
void EVT_APP_SETTING_CHANGE_PASSPHRASE_HANDLER(QVariant msg);
void EVT_APP_SETTING_REQUEST_RESTART_HANDLER(QVariant msg);
void EVT_APP_SETTING_REQUEST_RESCAN_HANDLER(QVariant msg);
void EVT_APP_SETTING_DELETE_PRIMARY_KEY_ACCOUNT_REQUEST_HANDLER(QVariant msg);
void EVT_APP_SETTING_BACK_REQUEST_HANDLER(QVariant msg);
void EVT_APP_SETTING_BACK_TO_ONLINE_MODE_HANDLER(QVariant msg);
void EVT_SHOW_REPLACE_PRIMARY_KEY_REQUEST_HANDLER(QVariant msg);

#endif // STATE_ID_SCR_APP_SETTINGS_H
