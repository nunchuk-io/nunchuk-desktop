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


#ifndef STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT_H
#define STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT_H

#include <QObject>
#include <QVariant>
#include "ViewsDefines.h"
#include "ViewsEnums.h"

void SCR_REMOTE_SIGNER_RESULT_Entry(QVariant msg);
void SCR_REMOTE_SIGNER_RESULT_Exit(QVariant msg);
void EVT_REMOTE_SIGNER_RESULT_HEALTH_CHECK_HANDLER(QVariant msg);
void EVT_REMOTE_SIGNER_RESULT_EDIT_NAME_HANDLER(QVariant msg);
void EVT_REMOTE_SIGNER_RESULT_IMPORT_SIGNATURE_HANDLER(QVariant msg);
void EVT_REMOTE_SIGNER_RESULT_EXPORT_MESSAGE_HANDLER(QVariant msg);
void EVT_REMOTE_SIGNER_RESULT_GET_XPUBS_HANDLER(QVariant msg);
void EVT_REMOTE_SIGNER_RESULT_DELETE_REQUEST_HANDLER(QVariant msg);
void EVT_REMOTE_SIGNER_RESULT_CONFIRM_ADD_TO_WALLET_SIGNER_CONFIG_HANDLER(QVariant msg);

#endif // STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT_H
