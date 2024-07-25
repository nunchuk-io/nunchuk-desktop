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


#ifndef STATE_ID_SCR_HOME_H
#define STATE_ID_SCR_HOME_H

#include <QObject>
#include <QVariant>
#include "ViewsDefines.h"
#include "ViewsEnums.h"

void SCR_HOME_Entry(QVariant msg);
void SCR_HOME_Exit(QVariant msg);
void EVT_HOME_WALLET_SELECTED_HANDLER(QVariant msg);
void EVT_HOME_WALLET_COPY_ADDRESS_HANDLER(QVariant msg);
void EVT_HOME_SETTING_REQUEST_HANDLER(QVariant msg);
void EVT_HOME_DISPLAY_ADDRESS_HANDLER(QVariant msg);
void EVT_HOME_IMPORT_PSBT_HANDLER(QVariant msg);
void EVT_HOME_EXPORT_BSMS_HANDLER(QVariant msg);
void EVT_HOME_ADD_WALLET_REQUEST_HANDLER(QVariant msg);
void EVT_HOME_MASTER_SIGNER_INFO_REQUEST_HANDLER(QVariant msg);
void EVT_HOME_SEND_REQUEST_HANDLER(QVariant msg);
void EVT_HOME_RECEIVE_REQUEST_HANDLER(QVariant msg);
void EVT_HOME_TRANSACTION_HISTORY_REQUEST_HANDLER(QVariant msg);
void EVT_HOME_WALLET_INFO_REQUEST_HANDLER(QVariant msg);
void EVT_HOME_TRANSACTION_INFO_REQUEST_HANDLER(QVariant msg);
void EVT_HOME_REMOTE_SIGNER_INFO_REQUEST_HANDLER(QVariant msg);
void EVT_HOME_ADD_NEW_SIGNER_REQUEST_HANDLER(QVariant msg);
void EVT_HOME_COLDCARD_NFC_SIGNER_INFO_REQUEST_HANDLER(QVariant msg);
void EVT_ASK_HARDWARE_REQ_HANDLER(QVariant msg);
void EVT_EXIST_HARDWARE_REQ_HANDLER(QVariant msg);
void EVT_SHOW_GROUP_WALLET_CONFIG_REQUEST_HANDLER(QVariant msg);
void EVT_KEY_HEALTH_CHECK_STATUS_REQUEST_HANDLER(QVariant msg);
void EVT_RECURRING_PAYMENTS_REQUEST_HANDLER(QVariant msg);
void EVT_ADD_HARDWARE_REQUEST_HANDLER(QVariant msg);
void EVT_ADD_SOFTWARE_SIGNER_RESULT_HANDLER(QVariant msg);
void EVT_REPLACE_KEYS_REQUEST_HANDLER(QVariant msg);

#endif // STATE_ID_SCR_HOME_H
