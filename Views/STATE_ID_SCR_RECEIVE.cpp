/**************************************************************************
 * This file is part of the Nunchuk software (https://nunchuk.io/)        *
 * Copyright (C) 2020-2022 Enigmo								          *
 * Copyright (C) 2022 Nunchuk								              *
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
#include "STATE_ID_SCR_RECEIVE.h"
#include "QEventProcessor.h"
#include "Models/AppModel.h"
#include "Models/SingleSignerModel.h"
#include "Models/WalletModel.h"
#include "bridgeifaces.h"
#include <QClipboard>

void SCR_RECEIVE_Entry(QVariant msg) {
    if(AppModel::instance()->walletInfo()){
        QString wallet_id = AppModel::instance()->walletInfo()->walletId();
        AppModel::instance()->startGetUsedAddresses(wallet_id);
        AppModel::instance()->startGetUnusedAddresses(wallet_id);
    }
}

void SCR_RECEIVE_Exit(QVariant msg) {
    AppModel::instance()->setAddressBalance(0);
}

void EVT_BTN_GEN_NEW_ADDRESS_HANDLER(QVariant msg) {
    if(AppModel::instance()->walletInfo()){
        QString wallet_id = AppModel::instance()->walletInfo()->walletId();
        if(AppModel::instance()->walletInfo()->unUsedAddressList().count() < MAX_UNUSED_ADDR){
            bridge::nunchukGenNewAddresses(wallet_id, false);
            AppModel::instance()->startGetUsedAddresses(wallet_id);
            AppModel::instance()->startGetUnusedAddresses(wallet_id);
        }
    }
}

void EVT_RECEIVE_BACK_REQUEST_HANDLER(QVariant msg) {

}

void EVT_RECEIVE_COPY_ADDRESS_HANDLER(QVariant msg) {
    if(!msg.toString().isEmpty()){
        qApp->clipboard()->setText(msg.toString());
    }
}

void EVT_RECEIVE_SET_QUICK_RECEIVE_ADDRESS_HANDLER(QVariant msg) {
    // Need handle set quick receive address here
    if(AppModel::instance()->walletInfo()){
        AppModel::instance()->walletInfo()->setAddress(msg.toString());
    }
}


void EVT_RECEIVE_DISPLAY_ADDRESS_HANDLER(QVariant msg) {
    if(AppModel::instance()->walletInfo()){
        AppModel::instance()->startDisplayAddress(AppModel::instance()->walletInfo()->walletId(),
                                                  msg.toString());
    }
}

void EVT_RECEIVE_ADDRESS_BALANCE_HANDLER(QVariant msg) {
    if(AppModel::instance()->walletInfo()){
        qint64 amount = bridge::nunchukGetAddressBalance(AppModel::instance()->walletInfo()->walletId(), msg.toString());
        AppModel::instance()->setAddressBalance(amount);
    }
}
