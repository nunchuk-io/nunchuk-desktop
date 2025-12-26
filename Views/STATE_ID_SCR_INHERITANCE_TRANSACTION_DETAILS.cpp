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


#include "STATE_ID_SCR_INHERITANCE_TRANSACTION_DETAILS.h"
#include "AppModel.h"
#include "Transactions/TransactionHelper.h"

void SCR_INHERITANCE_TRANSACTION_DETAILS_Entry(QVariant msg) {

}

void SCR_INHERITANCE_TRANSACTION_DETAILS_Exit(QVariant msg) {

}

void EVT_INHERITANCE_TRANSACTION_DETAILS_BACK_HANDLER(QVariant msg) {

}

void EVT_INHERITANCE_TRANSACTION_DETAILS_ACTION_HANDLER(QVariant msg) {
    QMap<QString, QVariant> maps = msg.toMap();
    QString type = maps["type"].toString();
    DBG_INFO << type;
    if (type == "address-to-verify") {
        auto address = maps["address"].toString();
        helper::transaction::verifyAddress(address);
    }
    else if (type == "memo-notify") {
        auto memo = maps["memo"].toString();
        helper::transaction::setMemo(memo);
    }
    else if (type == "scan-device") {
        helper::transaction::scanDevice(msg);
    }
    else if (type == "claim-tx-sign") {
        auto xfp = maps["xfp"].toString();
        helper::transaction::signRequest(xfp);
    }
    else if (type == "claim-tx-import-qr") {
        helper::transaction::importQr(msg);
    }
    else if (type == "claim-tx-import") {
        auto file = maps["file"].toString();
        helper::transaction::importFile(msg);
    }
    else if (type == "claim-tx-export") {
        helper::transaction::exportFile(msg);
    }
    else if (type == "register-wallet") {
        // helper::transaction::registerWallet(msg);
    }
    else if (type == "force-sync-claim-tx") {
        // helper::transaction::forceSyncWalletDb(msg);
    }
    else if (type == "claim-tx-export-qr") {
        helper::transaction::exportQr(msg);
    }
    else if (type == "claim-tx-export-bbqr") {
        // helper::transaction::exportBbqr(msg);
    }
    else if (type == "tx-broadcast") {
        helper::transaction::broadcast(msg);
    }
}
