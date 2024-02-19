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


#include "STATE_ID_SCR_INHERITANCE_WITHDRAW_BALANCE.h"
#include "localization/STR_CPP.h"
#include "Models/AppModel.h"
#include "Servers/Draco.h"
#include "ServiceSetting.h"
#include "Premiums/QWalletServicesTag.h"

void SCR_INHERITANCE_WITHDRAW_BALANCE_Entry(QVariant msg) {

}

void SCR_INHERITANCE_WITHDRAW_BALANCE_Exit(QVariant msg) {

}

void EVT_INHERITANCE_CREATE_DRAFT_TX_REQUEST_HANDLER(QVariant msg)
{
    QString to_wallet_id = msg.toString();
    DBG_INFO << to_wallet_id;
    ServiceSetting::instance()->servicesTagPtr()->setInheritanceAddress(to_wallet_id);
    ServiceSetting::instance()->servicesTagPtr()->inheritanceCreateDraftTransaction();
}

void EVT_INHERITANCE_WITHDRAW_BALANCE_BACK_HANDLER(QVariant msg) {

}

void EVT_INHERITANCE_CONFIRM_TRANSACTION_REQUEST_HANDLER(QVariant msg) {

}

void EVT_INHERITANCE_NEW_TRANSACTION_REQ_HANDLER(QVariant msg)
{
    DBG_INFO << msg;
    QString destinationAddress = msg.toString();
    ServiceSetting::instance()->servicesTagPtr()->setInheritanceAddressNewTransaction(destinationAddress);
    ServiceSetting::instance()->servicesTagPtr()->inheritanceCreateDraftTransaction();
}
