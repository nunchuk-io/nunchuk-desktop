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
#include "STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION.h"
#include "QEventProcessor.h"
#include "Models/AppModel.h"
#include "Models/SingleSignerModel.h"
#include "Models/WalletModel.h"
#include "bridgeifaces.h"
#include "Chats/ClientController.h"
#include "Chats/QNunchukRoomModel.h"
#include "Chats/matrixbrigde.h"
#include "localization/STR_CPP.h"

void SCR_SHARED_WL_DEVICE_REGISTRATION_Entry(QVariant msg) {
    AppModel::instance()->setQrExported(QStringList());
}

void SCR_SHARED_WL_DEVICE_REGISTRATION_Exit(QVariant msg) {

}

void EVT_SHARED_WALLET_EXPORT_COLDCARD_HANDLER(QVariant msg) {
    QString file_path = qUtils::QGetFilePath(msg.toString());
    if(CLIENT_CURRENTROOM->roomWallet() && (file_path != "")){
        nunchuk::ExportFormat format = nunchuk::ExportFormat::COLDCARD;
        if(CLIENT_CURRENTROOM->roomWallet()->walletInfo()){
            int type = CLIENT_CURRENTROOM->roomWallet()->walletInfo()->walletType();
            format = (int)nunchuk::WalletType::MINISCRIPT == type ? nunchuk::ExportFormat::DESCRIPTOR_EXTERNAL_INTERNAL : nunchuk::ExportFormat::COLDCARD;
        }
        bool ret = bridge::nunchukExportWallet(CLIENT_CURRENTROOM->roomWallet()->get_wallet_id(),
                                               file_path,
                                               format);
        DBG_INFO << file_path << ret;
    }
}

void EVT_SHARED_WALLET_EXPORT_QRCODE_HANDLER(QVariant msg) {
    QString wallet_id = msg.toString();
    AppModel::instance()->setQrExported(QStringList());
    QWarningMessage msgwarning;
#if 0
    QStringList qrtags = bridge::nunchukExportCoboWallet(wallet_id, msgwarning);
#endif
    QStringList qrtags = bridge::nunchukExportKeystoneWallet(wallet_id, msgwarning);
    if((int)EWARNING::WarningType::NONE_MSG == msgwarning.type()){
        AppModel::instance()->setQrExported(qrtags);
    }
    else{
        AppModel::instance()->showToast(msgwarning.code(), msgwarning.what(), (EWARNING::WarningType)msgwarning.type());
    }
}

void EVT_SHARED_WALLET_SKIP_REGISTRATION_HANDLER(QVariant msg) {

}

void EVT_SHARED_WL_DEVICE_REGISTRAION_BACK_HANDLER(QVariant msg) {

}
