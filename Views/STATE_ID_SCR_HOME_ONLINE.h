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


#ifndef STATE_ID_SCR_HOME_ONLINE_H
#define STATE_ID_SCR_HOME_ONLINE_H

#include <QObject>
#include <QVariant>
#include "ViewsDefines.h"
#include "ViewsEnums.h"

void SCR_HOME_ONLINE_Entry(QVariant msg);
void SCR_HOME_ONLINE_Exit(QVariant msg);
void EVT_HOME_ONLINE_ADD_SIGNER_HANDLER(QVariant msg);
void EVT_HOME_ONLINE_CREATE_CHAT_ROOM_HANDLER(QVariant msg);
void EVT_HOME_ONLINE_CREATE_DIRECT_CHAT_HANDLER(QVariant msg);
void EVT_HOME_ONLINE_DELETE_DIRECT_ROOM_HANDLER(QVariant msg);
void EVT_HOME_ONLINE_DELETE_ROOM_HANDLER(QVariant msg);
void EVT_HOME_ONLINE_SEND_CHAT_CONTENT_HANDLER(QVariant msg);
void EVT_HOME_ONLINE_CANCEL_SHARED_WL_HANDLER(QVariant msg);
void EVT_HOME_ONLINE_CREATE_SHARED_WALLET_HANDLER(QVariant msg);
void EVT_HOME_ONLINE_CANCEL_TRANSACTION_HANDLER(QVariant msg);
void EVT_HOME_ONLINE_SERVICE_SUPPORT_REQ_HANDLER(QVariant msg);
void EVT_HOME_ONLINE_ADD_CONTACT_HANDLER(QVariant msg);
void EVT_HOME_SHOW_ALL_PENDING_CONTACT_HANDLER(QVariant msg);
void EVT_HOME_ONLINE_SIGNER_AVAILABLE_FOR_SWL_HANDLER(QVariant msg);
void EVT_HOME_SHARED_WALLET_CONFIGURE_HANDLER(QVariant msg);
void EVT_HOME_BACKUP_SHARED_WALLET_HANDLER(QVariant msg);
void EVT_HOME_ONLINE_SIGNER_UNAVAILABLE_FOR_SWL_HANDLER(QVariant msg);
void EVT_HOME_SHARED_WL_SEND_REQUEST_HANDLER(QVariant msg);
void EVT_HOME_ONLINE_TRANSACTION_INFO_REQUEST_HANDLER(QVariant msg);

#endif // STATE_ID_SCR_HOME_ONLINE_H
