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
#include "STATE_ID_SCR_HOME.h"
#include "QQuickViewer.h"
#include "Models/AppModel.h"
#include "Models/SingleSignerModel.h"
#include "Models/WalletModel.h"
#include "bridgeifaces.h"
#include "Draco.h"
#include <QClipboard>
#include "localization/STR_CPP.h"
#include "ifaces/Chats/matrixbrigde.h"

void SCR_HOME_Entry(QVariant msg) {
    AppModel::instance()->setTabIndex((int)ENUNCHUCK::TabSelection::WALLET_TAB);
}

void SCR_HOME_Exit(QVariant msg) {

}

void EVT_HOME_WALLET_SELECTED_HANDLER(QVariant msg) {
    if(msg.toInt() >= 0){
        AppModel::instance()->setWalletListCurrentIndex(msg.toInt());
    }
}

void EVT_HOME_ADD_WALLET_REQUEST_HANDLER(QVariant msg) {
    QWalletPtr newWallet(new Wallet());
    newWallet.data()->setCapableCreate(false);
    AppModel::instance()->setNewWalletInfo(newWallet);
    AppModel::instance()->resetSignersChecked();
    AppModel::instance()->setSingleSignerInfo(QSingleSignerPtr(new QSingleSigner()));
    QQuickViewer::instance()->setCurrentFlow((int)ENUNCHUCK::IN_FLOW::FLOW_ADD_WALLET);
}

void EVT_HOME_MASTER_SIGNER_INFO_REQUEST_HANDLER(QVariant msg) {
    AppModel::instance()->setMasterSignerInfo(msg.toInt());
    AppModel::instance()->setWalletsUsingSigner(AppModel::instance()->walletList()->walletListByMasterSigner(AppModel::instance()->masterSignerInfo()->id()));
}

void EVT_HOME_REMOTE_SIGNER_INFO_REQUEST_HANDLER(QVariant msg) {
    QSingleSignerPtr it = AppModel::instance()->remoteSignerList()->getSingleSignerByIndex(msg.toInt());
    if(it) {
        AppModel::instance()->setSingleSignerInfo(it);
        AppModel::instance()->setWalletsUsingSigner(AppModel::instance()->walletList()->walletListByFingerPrint(it.data()->masterFingerPrint()));
    }
}

void EVT_HOME_SEND_REQUEST_HANDLER(QVariant msg) {
    if(AppModel::instance()->walletInfo()){
        QUTXOListModelPtr utxos = bridge::nunchukGetUnspentOutputs(AppModel::instance()->walletInfo()->id());
        if(utxos){
            AppModel::instance()->setUtxoList(utxos);
        }
    }
}

void EVT_HOME_RECEIVE_REQUEST_HANDLER(QVariant msg) {

}

void EVT_HOME_TRANSACTION_HISTORY_REQUEST_HANDLER(QVariant msg) {

}

void EVT_HOME_WALLET_INFO_REQUEST_HANDLER(QVariant msg) {

}

void EVT_HOME_WALLET_COPY_ADDRESS_HANDLER(QVariant msg) {
    if(!msg.toString().isEmpty()){
        qApp->clipboard()->setText(msg.toString());
    }
}

void EVT_HOME_TRANSACTION_INFO_REQUEST_HANDLER(QVariant msg) {
    if(msg.toString() != "" && AppModel::instance()->walletInfo() && AppModel::instance()->walletInfo()->transactionHistory()){
        QTransactionPtr it = AppModel::instance()->walletInfo()->transactionHistory()->getTransactionByTxid(msg.toString());
        if(it){
            if(CLIENT_INSTANCE->isNunchukLoggedIn() && CLIENT_INSTANCE->isMatrixLoggedIn() && CLIENT_INSTANCE->rooms()){
                QString room_id = CLIENT_INSTANCE->rooms()->getRoomIdByWalletId(AppModel::instance()->walletInfo()->id());
                if(room_id != ""){
                    QWarningMessage tmsg;
                    std::vector<nunchuk::RoomTransaction> results = matrixifaces::instance()->GetPendingTransactions(room_id.toStdString(), tmsg);
                    foreach (nunchuk::RoomTransaction tx, results) {
                        if(0 == QString::compare(QString::fromStdString(tx.get_tx_id()), msg.toString(), Qt::CaseInsensitive)){
                            it->setInitEventId(QString::fromStdString(tx.get_init_event_id()));
                        }
                    }
                }
            }

            QMasterSignerListModelPtr mastersigners = bridge::nunchukGetMasterSigners();
            if(mastersigners){
                for (QMasterSignerPtr master : mastersigners->fullList()) {
                    it.data()->singleSignersAssigned()->updateSignerIsLocalAndReadyToSign(master);
                }
            }
            QJsonObject data = Draco::instance()->assistedWalletGetTx(AppModel::instance()->walletInfo()->id(),it->txid());
            it->setServerKeyMessage(data);
        }
        AppModel::instance()->setTransactionInfo(it);
    }
}

void EVT_HOME_SETTING_REQUEST_HANDLER(QVariant msg) {

}

void EVT_HOME_DISPLAY_ADDRESS_HANDLER(QVariant msg) {
    if(AppModel::instance()->walletInfo()){
        AppModel::instance()->startDisplayAddress(AppModel::instance()->walletInfo()->id(),
                                                  msg.toString());
    }
}

void EVT_HOME_ADD_NEW_SIGNER_REQUEST_HANDLER(QVariant msg) {
    QQuickViewer::instance()->setCurrentFlow((int)ENUNCHUCK::IN_FLOW::FLOW_ADD_SIGNER);
}


void EVT_HOME_IMPORT_PSBT_HANDLER(QVariant msg) {
    QString file_path = qUtils::QGetFilePath(msg.toString());
    if (file_path != "" && AppModel::instance()->walletInfo()){
        QString wallet_id = AppModel::instance()->walletInfo()->id();
        QWarningMessage msgwarning;
        QTransactionPtr trans = bridge::nunchukImportTransaction(wallet_id, file_path, msgwarning);
        if((int)EWARNING::WarningType::NONE_MSG == msgwarning.type()){
            if(trans){
                AppModel::instance()->setTransactionInfo(trans);
                if(AppModel::instance()->walletInfo()){
                    AppModel::instance()->startGetTransactionHistory(wallet_id);
                    AppModel::instance()->startGetUsedAddresses(wallet_id);
                    AppModel::instance()->startGetUnusedAddresses(wallet_id);
                }
                AppModel::instance()->showToast(0,
                                                STR_CPP_091,
                                                EWARNING::WarningType::SUCCESS_MSG,
                                                STR_CPP_091);
            }
        }
        else{
            AppModel::instance()->showToast(msgwarning.code(),
                                            msgwarning.what(),
                                            (EWARNING::WarningType)msgwarning.type(),
                                            STR_CPP_092);
        }
    }
}

void EVT_HOME_EXPORT_BSMS_HANDLER(QVariant msg) {
    QString file_path = qUtils::QGetFilePath(msg.toString());
    if(AppModel::instance()->walletInfo() && (file_path != "")){
        bool ret = bridge::nunchukExportWallet(AppModel::instance()->walletInfo()->id(),
                                                file_path,
                                                nunchuk::ExportFormat::BSMS);
        DBG_INFO << file_path << ret;
    }
}

void EVT_HOME_COLDCARD_NFC_SIGNER_INFO_REQUEST_HANDLER(QVariant msg) {
    QSingleSignerPtr it = AppModel::instance()->remoteSignerList()->getSingleSignerByIndex(msg.toInt());
    if (it) {
        QMasterSignerPtr signer = QMasterSignerPtr(new QMasterSigner());
        signer.data()->setId(it->masterSignerId());
        signer.data()->setName(it->name());
        signer.data()->setSignerType((int)it.data()->signerType());
        signer.data()->setFingerPrint(it.data()->masterFingerPrint());
        signer.data()->setDeviceType(it.data()->devicetype());
        AppModel::instance()->setSingleSignerInfo(it);
        AppModel::instance()->setMasterSignerInfo(signer);
        AppModel::instance()->setWalletsUsingSigner(AppModel::instance()->walletList()->walletListByMasterSigner(AppModel::instance()->masterSignerInfo()->id()));
    }
}


