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


#ifndef STATE_ID_SCR_TRANSACTION_INFO_H
#define STATE_ID_SCR_TRANSACTION_INFO_H

#include <QObject>
#include <QVariant>
#include "ViewsDefines.h"
#include "ViewsEnums.h"

void SCR_TRANSACTION_INFO_Entry(QVariant msg);
void SCR_TRANSACTION_INFO_Exit(QVariant msg);
void EVT_TRANSACTION_SIGN_REQUEST_HANDLER(QVariant msg);
void EVT_TRANSACTION_EXPORT_REQUEST_HANDLER(QVariant msg);
void EVT_TRANSACTION_EXPORT_QRCODE_HANDLER(QVariant msg);
void EVT_TRANSACTION_IMPORT_REQUEST_HANDLER(QVariant msg);
void EVT_TRANSACTION_IMPORT_QRCODE_HANDLER(QVariant msg);
void EVT_TRANSACTION_BROADCAST_REQUEST_HANDLER(QVariant msg);
void EVT_TRANSACTION_SET_MEMO_REQUEST_HANDLER(QVariant msg);
void EVT_TRANSACTION_SCAN_DEVICE_REQUEST_HANDLER(QVariant msg);
void EVT_TRANSACTION_REMOVE_REQUEST_HANDLER(QVariant msg);
void EVT_TRANSACTION_SEND_PIN_REQUEST_HANDLER(QVariant msg);
void EVT_TRANSACTION_PROMT_PIN_REQUEST_HANDLER(QVariant msg);
void EVT_TRANSACTION_VERIFY_ADDRESS_HANDLER(QVariant msg);
void EVT_TRANSACTION_SEND_PASSPHRASE_HANDLER(QVariant msg);
void EVT_TRANSACTION_CANCEL_REQUEST_HANDLER(QVariant msg);
void EVT_TRANSACTION_INFO_BACK_REQUEST_HANDLER(QVariant msg);
void EVT_TRANSACTION_INFO_CANCEL_PENDING_TRANSACTION_REQUEST_HANDLER(QVariant msg);
void EVT_TRANSACTION_REPLACE_BY_FEE_REQUEST_HANDLER(QVariant msg);
void EVT_TRANSACTION_INFO_BACK_UTXO_INFO_HANDLE(QVariant msg);

#endif // STATE_ID_SCR_TRANSACTION_INFO_H
