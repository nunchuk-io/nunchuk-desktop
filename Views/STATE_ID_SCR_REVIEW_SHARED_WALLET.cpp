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
#include "STATE_ID_SCR_REVIEW_SHARED_WALLET.h"
#include "QQuickViewer.h"
#include "Models/AppModel.h"
#include "Models/SingleSignerModel.h"
#include "Models/WalletModel.h"
#include "bridgeifaces.h"
#include "Chats/ClientController.h"
#include "Chats/QNunchukRoomModel.h"
#include "Chats/matrixbrigde.h"

void SCR_REVIEW_SHARED_WALLET_Entry(QVariant msg) {

}

void SCR_REVIEW_SHARED_WALLET_Exit(QVariant msg) {

}

void EVT_REVIEW_SHARED_WALLET_BACK_HANDLER(QVariant msg) {

}

void EVT_REVIEW_SHARED_WALLET_INVITE_SIGNER_HANDLER(QVariant msg) {
    /////// initial wallet
    if(CLIENT_CURRENTROOM){
        QRoomWalletPtr wl = CLIENT_CURRENTROOM->roomWalletPtr();
        if(!wl.isNull()){
            QWarningMessage msgwarning;
            matrixbrigde::InitWallet(CLIENT_CURRENTROOM->id(),
                                     wl.data()->walletName(),
                                     wl.data()->walletM(),
                                     wl.data()->walletN(),
                                     (ENUNCHUCK::AddressType)wl.data()->walletAddressType().toUInt(),
                                     wl.data()->walletEscrow(),
                                     wl.data()->walletDescription(),
                                     msgwarning);
        }
    }
}

