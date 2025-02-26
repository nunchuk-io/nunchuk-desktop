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
#include "STATE_ID_SCR_ADD_WALLET.h"
#include "QEventProcessor.h"
#include "Models/AppModel.h"
#include "Models/SingleSignerModel.h"
#include "Models/WalletModel.h"
#include "Premiums/GroupSandboxModel.h"
#include "bridgeifaces.h"
#include "localization/STR_CPP.h"

void SCR_ADD_WALLET_Entry(QVariant msg) {
    AppModel::instance()->resetSignersChecked();
}

void SCR_ADD_WALLET_Exit(QVariant msg) {

}

void EVT_ADD_WALLET_IMPORT_HANDLER(QVariant msg) {
    int importType = msg.toMap().value("importType").toInt();
    QString walletname = msg.toMap().value("walletname").toString();
    QString walletdescription = msg.toMap().value("walletdescription").toString();
    QWalletPtr walletImported = NULL;
    QWarningMessage importmsg;

    if ((int)POPUP::WalletImportType::IMPORT_WALLET_QRCODE == importType) {
        QEventProcessor::instance()->sendEvent(E::EVT_ADD_WALLET_IMPORT_SUCCEED);
        AppModel::instance()->showToast(0, STR_CPP_067, EWARNING::WarningType::SUCCESS_MSG);
    }
    else {
        QString fileSelected = msg.toMap().value("filePath").toString();
        QString file_path = qUtils::QGetFilePath(fileSelected);
        if(file_path != ""){
            if((int)POPUP::WalletImportType::IMPORT_WALLET_DB == importType){
                walletImported = bridge::nunchukImportWallet(file_path, importmsg);
                if(walletImported){
                    walletImported.data()->setCreationMode((int)Wallet::CreationMode::CREATE_BY_IMPORT_DB);
                }
            }
            else if((int)POPUP::WalletImportType::IMPORT_WALLET_DESCRIPTOR == importType){
                walletImported = bridge::nunchukImportWalletDescriptor(file_path, walletname, walletdescription, importmsg);
                if(walletImported){
                    walletImported.data()->setCreationMode((int)Wallet::CreationMode::CREATE_BY_IMPORT_DESCRIPTOR);
                }
            }
            else{
                walletImported = bridge::nunchukImportWalletDescriptor(file_path, walletname, walletdescription, importmsg);
                if(walletImported){
                    walletImported.data()->setCreationMode((int)Wallet::CreationMode::CREATE_BY_IMPORT_CONFIGURATION);
                }
            }
            if(walletImported && importmsg.type() == (int)EWARNING::WarningType::NONE_MSG){
                AppModel::instance()->walletList()->addWallet(walletImported);
                AppModel::instance()->resetSignersChecked();
                AppModel::instance()->walletList()->requestSort(WalletListModel::WalletRoles::wallet_createDate_Role, Qt::AscendingOrder);
                int index = AppModel::instance()->walletList()->getWalletIndexById(walletImported.data()->walletId());
                if(-1 != index){
                    AppModel::instance()->setWalletListCurrentIndex(index);
                }
                QEventProcessor::instance()->sendEvent(E::EVT_ADD_WALLET_IMPORT_SUCCEED);
                AppModel::instance()->showToast(0, STR_CPP_067, EWARNING::WarningType::SUCCESS_MSG);
            }
            else{
                AppModel::instance()->showToast(importmsg.code(), importmsg.what(), (EWARNING::WarningType)importmsg.type());
            }
        }
    }
}

void EVT_ADD_WALLET_BACK_REQUEST_HANDLER(QVariant msg) {

}

void EVT_ADD_WALLET_SIGNER_CONFIGURATION_REQUEST_HANDLER(QVariant msg) {
    QMap<QString, QVariant> maps = msg.toMap();
    QString walletNameInputted = maps["walletNameInputted"].toString();
    QString walletDescription  = maps["walletDescription"].toString();
    int  addressType = maps["addressType"].toInt();
    if(auto nw = AppModel::instance()->newWalletInfo()) {
        nw->setWalletName(walletNameInputted);
        nw->setWalletDescription(walletDescription);
        nw->setWalletAddressType(addressType);
    }
}

void EVT_ADD_WALLET_IMPORT_SUCCEED_HANDLER(QVariant msg) {

}
