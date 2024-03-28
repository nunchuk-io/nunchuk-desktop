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
#include "STATE_ID_SCR_RECOVER_SHARED_WALLET.h"
#include "QEventProcessor.h"
#include "Models/AppModel.h"
#include "Models/SingleSignerModel.h"
#include "Models/WalletModel.h"
#include "bridgeifaces.h"
#include "Chats/ClientController.h"
#include "Chats/QNunchukRoomModel.h"
#include "Chats/matrixbrigde.h"
#include "localization/STR_CPP.h"

void SCR_RECOVER_SHARED_WALLET_Entry(QVariant msg) {
    Q_UNUSED(msg);
}

void SCR_RECOVER_SHARED_WALLET_Exit(QVariant msg) {
    Q_UNUSED(msg);
}

void EVT_IMPORT_BSMS_SHARED_WALLET_HANDLER(QVariant msg) {
    QVariantMap maps = msg.toMap();
    bool importFile = maps["passFile"].toBool();
    static QString file_path;
    if(importFile){
        file_path = maps["filePath"].toString();
        file_path = qUtils::QGetFilePath(file_path);
    }
    else{
        if(CLIENT_CURRENTROOM){
            QRoomWalletPtr wl = CLIENT_CURRENTROOM->roomWalletPtr();
            if(!wl.isNull()){
                QWarningMessage msgwarning;
                matrixbrigde::ImportWallet(CLIENT_CURRENTROOM->id(),
                                           wl->walletName(),
                                           wl->walletDescription(),
                                           file_path,
                                           msgwarning);
                if(msgwarning.type() == (int)EWARNING::WarningType::NONE_MSG){
                    QEventProcessor::instance()->sendEvent(E::EVT_IMPORT_SHARED_WALLET_SUCCEED);
                }
                else{
                    AppModel::instance()->showToast(msgwarning.code(), msgwarning.what(), EWARNING::WarningType::EXCEPTION_MSG);
                }
            }
        }
    }
}

void EVT_IMPORT_QR_SHARED_WALLET_HANDLER(QVariant msg) {
    QEventProcessor::instance()->sendEvent(E::EVT_IMPORT_SHARED_WALLET_SUCCEED);
}

void EVT_IMPORT_SHARED_WALLET_SUCCEED_HANDLER(QVariant msg) {
    Q_UNUSED(msg);
}



void EVT_RECOVER_SHARED_WALLET_BACK_HANDLER(QVariant msg) {

}


