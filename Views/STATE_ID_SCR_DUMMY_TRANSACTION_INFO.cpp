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

#include "STATE_ID_SCR_DUMMY_TRANSACTION_INFO.h"
#include "Chats/QUserWallets.h"

void SCR_DUMMY_TRANSACTION_INFO_Entry(QVariant msg) {

}

void SCR_DUMMY_TRANSACTION_INFO_Exit(QVariant msg) {

}

void EVT_DUMMY_TRANSACTION_INFO_BACK_HANDLER(QVariant msg) {

}


void EVT_DUMMY_TRANSACTION_LOCKDOWN_SUCCEEDED_REQUEST_HANDLER(QVariant msg) {

}

void EVT_DUMMY_TRANSACTION_SIGN_REQUEST_HANDLER(QVariant msg) {
    QString xfp = msg.toString();
    QUserWallets::instance()->signDummyTx(xfp);
}

void EVT_DUMMY_TRANSACTION_SIGN_CONFIRM_REQUEST_HANDLER(QVariant msg) {
    if (QUserWallets::instance()->checkDummyTx()) {
        if (QUserWallets::instance()->lockdownBySignDummyTx()) {
            QQuickViewer::instance()->sendEvent(E::EVT_DUMMY_TRANSACTION_LOCKDOWN_SUCCEEDED_REQUEST);
        }
    }
}

void EVT_DUMMY_TRANSACTION_VERIFY_ADDRESS_HANDLER(QVariant msg)
{
    AppModel::instance()->startDisplayAddress(QString::fromStdString(QUserWallets::instance()->getWallet().get_id()),
                                              msg.toString());
}

void EVT_DUMMY_TRANSACTION_SET_MEMO_REQUEST_HANDLER(QVariant msg)
{
    QString memo = msg.toString();
    if(AppModel::instance()->transactionInfo() && memo != ""){
        QString tx_id = AppModel::instance()->transactionInfo()->txid();
        QString wallet_id = AppModel::instance()->transactionInfo()->walletId();
        bridge::nunchukUpdateTransactionMemo(wallet_id,
                                             tx_id,
                                             memo);
    }
}

void EVT_DUMMY_TRANSACTION_SCAN_DEVICE_REQUEST_HANDLER(QVariant msg)
{
    AppModel::instance()->startScanDevices(E::STATE_ID_SCR_TRANSACTION_INFO);
}
