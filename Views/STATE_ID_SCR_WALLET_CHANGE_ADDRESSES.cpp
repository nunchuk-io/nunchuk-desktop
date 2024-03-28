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
#include "STATE_ID_SCR_WALLET_CHANGE_ADDRESSES.h"
#include "QEventProcessor.h"
#include "Models/AppModel.h"
#include "bridgeifaces.h"
#include <QClipboard>

void SCR_WALLET_CHANGE_ADDRESSES_Entry(QVariant msg) {
    if(AppModel::instance()->walletInfo()){
        QString wallet_id = AppModel::instance()->walletInfo()->id();
        AppModel::instance()->startGetUsedAddresses(wallet_id);
        AppModel::instance()->startGetUnusedAddresses(wallet_id);
    }
}

void SCR_WALLET_CHANGE_ADDRESSES_Exit(QVariant msg) {

}

void EVT_WALLET_CHANGE_ADDRESSES_GEN_NEW_ADDRESS_HANDLER(QVariant msg) {
    if(AppModel::instance()->walletInfo()){
        QString wallet_id = AppModel::instance()->walletInfo()->id();
        if(AppModel::instance()->walletInfo()->unUsedChangeddAddressList().count() < MAX_UNUSED_ADDR){
            bridge::nunchukGenNewAddresses(wallet_id, true);
            AppModel::instance()->startGetUsedAddresses(wallet_id);
            AppModel::instance()->startGetUnusedAddresses(wallet_id);
        }
    }
}

void EVT_WALLET_CHANGE_ADDRESS_BACK_REQUEST_HANDLER(QVariant msg) {

}

void EVT_WALLET_CHANGE_ADDRESSES_COPY_HANDLER(QVariant msg) {
    if(!msg.toString().isEmpty()){
        qApp->clipboard()->setText(msg.toString());
    }
}

void EVT_CHANGE_ADDRESS_BACK_SHARED_WALLET_HANDLER(QVariant msg) {

}
