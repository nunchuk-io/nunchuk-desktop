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
#include "STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET.h"
#include "QEventProcessor.h"
#include "Models/AppModel.h"
#include "Models/SingleSignerModel.h"
#include "Models/WalletModel.h"
#include "bridgeifaces.h"
#include "Chats/ClientController.h"
#include "Chats/QNunchukRoomModel.h"
#include "Chats/matrixbrigde.h"


void SCR_ASSIGN_SIGNER_TO_SHARED_WALLET_Entry(QVariant msg) {
    // Update checked state
    if(AppModel::instance()->masterSignerList()){
        AppModel::instance()->masterSignerList()->resetUserChecked();
        AppModel::instance()->masterSignerList()->resetMasterSignerNeedXpub();
    }
    if(AppModel::instance()->remoteSignerList()){
        AppModel::instance()->remoteSignerList()->resetUserChecked();
    }
    if(CLIENT_CURRENTROOM && CLIENT_CURRENTROOM->roomWallet() && CLIENT_CURRENTROOM->roomWallet()->walletSigners()){
        QStringList xfps = CLIENT_CURRENTROOM->roomWallet()->walletSigners()->xfplist();
        for (QString xfp : xfps) {
            if(AppModel::instance()->masterSignerList()){
                AppModel::instance()->masterSignerList()->setUserCheckedByFingerprint(true, xfp);
            }
            if(AppModel::instance()->remoteSignerList()){
                AppModel::instance()->remoteSignerList()->setUserCheckedByFingerprint(true, xfp);
            }
        }
    }
}

void SCR_ASSIGN_SIGNER_TO_SHARED_WALLET_Exit(QVariant msg) {
    // Reset checked state
    if(AppModel::instance()->masterSignerList()){
        AppModel::instance()->masterSignerList()->resetUserChecked();
    }
    if(AppModel::instance()->remoteSignerList()){
        AppModel::instance()->remoteSignerList()->resetUserChecked();
    }
}

void EVT_ASSIGN_SIGNER_TO_SHARED_REQUEST_HANDLER(QVariant msg) {
    DBG_INFO;
}

void EVT_ASSIGN_SIGNER_TO_SHARED_WALLET_BACK_REVIEW_WALLET_HANDLER(QVariant msg) {

}

void EVT_ASSIGN_SIGNER_TO_SHARED_BACK_WALLET_CONFIG_HANDLER(QVariant msg) {

}


