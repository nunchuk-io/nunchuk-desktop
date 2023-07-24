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


#include "STATE_ID_SCR_ADD_LEDGER_EXIST.h"
#include "AppModel.h"
#include "Chats/QUserWallets.h"

void SCR_ADD_LEDGER_EXIST_Entry(QVariant msg) {

}

void SCR_ADD_LEDGER_EXIST_Exit(QVariant msg) {

}

void EVT_ADD_EXIST_LEDGER_REQUEST_HANDLER(QVariant msg) {
    DBG_INFO << msg;
    QMasterSignerPtr signer = AppModel::instance()->masterSignerList()->getMasterSignerByXfp(msg.toString());
    AppModel::instance()->setMasterSignerInfo(signer);
    QUserWallets::instance()->addKeyRequested();
}

void EVT_ADD_NEW_LEDGER_REQUEST_HANDLER(QVariant msg) {

}

