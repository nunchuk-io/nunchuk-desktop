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


#include "STATE_ID_SCR_INHERITANCE_CONFIRM_TRANSACTION.h"
#include "Models/AppModel.h"
#include "bridgeifaces.h"
#include "localization/STR_CPP.h"
#include "Chats/QUserWallets.h"

void SCR_INHERITANCE_CONFIRM_TRANSACTION_Entry(QVariant msg) {

}

void SCR_INHERITANCE_CONFIRM_TRANSACTION_Exit(QVariant msg) {

}

void EVT_INHERITANCE_TRANSACTION_DETAILS_REQUEST_HANDLER(QVariant msg) {
    QUserWallets::instance()->inheritanceSignTransaction();
}

void EVT_INHERITANCE_CONFIRM_TRANSACTION_BACK_HANDLER(QVariant msg) {

}

void EVT_INHERITANCE_CREATE_DRAFT_TX_FEE_REQ_HANDLER(QVariant msg)
{
    bool subtractFromFeeAmout = msg.toMap().value("subtractFromFeeAmout").toBool();
    int feeRate = msg.toMap().value("feeRate").toDouble()*1000; // Convert sats/Byte to sats/kB
    bool manualFee = msg.toMap().value("manualFee").toBool();
    bool manualOutput = msg.toMap().value("manualOutput").toBool();
    if(!manualFee) feeRate = 1000;//default value
    DBG_INFO << "subtract:" << subtractFromFeeAmout << "| manual Output:" << manualOutput << "| manual Fee:" << manualFee << "| free rate:" << feeRate;
    QUserWallets::instance()->inheritanceCreateDraftTransaction(feeRate);
}
