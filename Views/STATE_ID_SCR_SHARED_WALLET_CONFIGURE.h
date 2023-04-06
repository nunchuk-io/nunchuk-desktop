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


#ifndef STATE_ID_SCR_SHARED_WALLET_CONFIGURE_H
#define STATE_ID_SCR_SHARED_WALLET_CONFIGURE_H

#include <QObject>
#include <QVariant>
#include "ViewsDefines.h"
#include "ViewsEnums.h"

void SCR_SHARED_WALLET_CONFIGURE_Entry(QVariant msg);
void SCR_SHARED_WALLET_CONFIGURE_Exit(QVariant msg);
void EVT_SHARED_WALLET_FINALIZE_WALLET_HANDLER(QVariant msg);
void EVT_SHARED_WALLET_CONFIGURE_CANCEL_HANDLER(QVariant msg);
void EVT_SHARED_WALLET_CONFIGURE_EXPORT_COLDCARD_HANDLER(QVariant msg);
void EVT_SHARED_WALLET_CONFIGURE_EXPORT_QRCODE_HANDLER(QVariant msg);
void EVT_SHARED_WALLET_CONFIGURE_DELETE_SWL_HANDLER(QVariant msg);
void EVT_SHARED_WALLET_CONFIGURE_EXPORT_BSMS_HANDLER(QVariant msg);
void EVT_SHARED_WALLET_CONFIGURE_BACK_HANDLER(QVariant msg);
void EVT_SHARED_WALLET_ADD_SIGNER_REQUEST_HANDLER(QVariant msg);
void EVT_SHARED_WALLET_UTXO_OUTPUT_HANDLER(QVariant msg);
void EVT_SHARED_WALLET_CHANGE_ADDRESS_HANDLER(QVariant msg);

#endif // STATE_ID_SCR_SHARED_WALLET_CONFIGURE_H
