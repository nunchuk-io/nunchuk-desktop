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
#include "STATE_ID_SCR_HOME_ONLINE.h"
#include "QEventProcessor.h"
#include "Models/AppModel.h"
#include "Models/SingleSignerModel.h"
#include "Models/WalletModel.h"
#include "bridgeifaces.h"
#include "Servers/Draco.h"
#include "Chats/ClientController.h"
#include "Chats/QNunchukRoomModel.h"
#include "Chats/matrixbrigde.h"
#include "localization/STR_CPP.h"

void SCR_HOME_ONLINE_Entry(QVariant msg) {
    AppModel::instance()->setTabIndex((int)ENUNCHUCK::TabSelection::CHAT_TAB);
    if(CLIENT_INSTANCE->rooms()){
        CLIENT_INSTANCE->rooms()->startCountdown();
    }
}

void SCR_HOME_ONLINE_Exit(QVariant msg) {

}

void EVT_HOME_ONLINE_ADD_SIGNER_HANDLER(QVariant msg) {

}

void EVT_HOME_ONLINE_CREATE_CHAT_ROOM_HANDLER(QVariant msg) {

}

void EVT_HOME_ONLINE_ADD_CONTACT_HANDLER(QVariant msg) {

}

void EVT_HOME_ONLINE_CREATE_DIRECT_CHAT_HANDLER(QVariant msg) {

}

void EVT_HOME_ONLINE_DELETE_DIRECT_ROOM_HANDLER(QVariant msg) {

}

void EVT_HOME_ONLINE_DELETE_ROOM_HANDLER(QVariant msg) {

}

void EVT_HOME_ONLINE_SEND_CHAT_CONTENT_HANDLER(QVariant msg) {
    CLIENT_INSTANCE->sendMessage(msg.toString());
}

void EVT_HOME_SHOW_ALL_PENDING_CONTACT_HANDLER(QVariant msg) {

}

void EVT_HOME_ONLINE_CREATE_SHARED_WALLET_HANDLER(QVariant msg) {
    int signerCount = (AppModel::instance()->masterSignerList() ? AppModel::instance()->masterSignerList()->rowCount() : 0)
            + (AppModel::instance()->remoteSignerList() ? AppModel::instance()->remoteSignerList()->rowCount() : 0);
    if(signerCount > 0){
        if(CLIENT_CURRENTROOM){
            CLIENT_CURRENTROOM->setRoomWallet(QRoomWalletPtr(new QRoomWallet()));
        }
        QEventProcessor::instance()->sendEvent(E::EVT_HOME_ONLINE_SIGNER_AVAILABLE_FOR_SWL);
    }
    else{
        QEventProcessor::instance()->sendEvent(E::EVT_HOME_ONLINE_SIGNER_UNAVAILABLE_FOR_SWL);
    }
}

void EVT_HOME_BACKUP_SHARED_WALLET_HANDLER(QVariant msg) {

}

void EVT_HOME_ONLINE_CANCEL_SHARED_WL_HANDLER(QVariant msg) {
    // CANCEL WALLET
    QString roomid = msg.toString();
    QWarningMessage execption;
    matrixbrigde::CancelWallet(roomid, STR_CPP_010, execption);
    if((int)EWARNING::WarningType::NONE_MSG == execption.type()){
        DBG_INFO << "CANCEL SUCCEED " << roomid;
    }
}

void EVT_HOME_ONLINE_SIGNER_AVAILABLE_FOR_SWL_HANDLER(QVariant msg) {

}

void EVT_HOME_ONLINE_SIGNER_UNAVAILABLE_FOR_SWL_HANDLER(QVariant msg) {

}

void EVT_HOME_SHARED_WALLET_CONFIGURE_HANDLER(QVariant msg) {

}

void EVT_HOME_SHARED_WL_SEND_REQUEST_HANDLER(QVariant msg) {
    if(msg.isValid()){
        if(CLIENT_INSTANCE->rooms() == nullptr) return;
        QString room_id = CLIENT_INSTANCE->getRoomIdByWalletId(msg.toString());
        int index = CLIENT_INSTANCE->rooms()->getIndex(room_id);
        if(index >= 0){
            CLIENT_INSTANCE->rooms()->setCurrentIndex(index);
        }
    }
    if(CLIENT_CURRENTROOM && CLIENT_CURRENTROOM->roomWallet() && CLIENT_CURRENTROOM->roomWallet()->walletInfo() && AppModel::instance()->walletList()){
        QString wallet_id = CLIENT_CURRENTROOM->roomWallet()->get_wallet_id();
        int index = AppModel::instance()->walletList()->getWalletIndexById(wallet_id);
        if(index != -1){
            AppModel::instance()->setWalletListCurrentIndex(index);
        }
    }

    if(AppModel::instance()->walletInfo()){
        QUTXOListModelPtr utxos = bridge::nunchukGetUnspentOutputs(AppModel::instance()->walletInfo()->id());
        if(utxos){
            AppModel::instance()->setUtxoList(utxos);
        }
    }
}

void EVT_HOME_ONLINE_TRANSACTION_INFO_REQUEST_HANDLER(QVariant msg) {
    QRoomTransaction* tx = msg.value<QRoomTransaction*>() ;
    if(tx){
        if(tx->transaction()){
            tx->transaction()->setInitEventId(tx->get_init_event_id());
        }
        AppModel::instance()->setTransactionInfo(tx->transactionPtr());
    }
}

void EVT_HOME_ONLINE_CANCEL_TRANSACTION_HANDLER(QVariant msg) {
    QRoomTransaction* tx = msg.value<QRoomTransaction*>() ;
    if(tx){
        QWarningMessage warningmsg;
        matrixbrigde::CancelTransaction(tx->transaction()->roomId(),
                                        tx->get_init_event_id(),
                                        STR_CPP_077,
                                        warningmsg);
        if((int)EWARNING::WarningType::NONE_MSG == warningmsg.type()){
            if(tx->transaction()){
                QNunchukRoom* room = CLIENT_INSTANCE->GetRoomById(tx->transaction()->roomId());
                if(room){
                    room->startGetPendingTxs();
                }
                AppModel::instance()->requestSyncWalletDb(tx->transaction()->walletId());
            }
        }
        else{
            AppModel::instance()->showToast(warningmsg.code(), warningmsg.what(), (EWARNING::WarningType)warningmsg.type());
        }
    }
}

void EVT_HOME_ONLINE_SERVICE_SUPPORT_REQ_HANDLER(QVariant msg)
{
    CLIENT_INSTANCE->createSupportRoom();
}
