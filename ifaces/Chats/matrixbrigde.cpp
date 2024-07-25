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
#include "matrixbrigde.h"
#include "matrixlistener.h"
#include "nunchuckiface.h"
#include "Chats/ClientController.h"
#include "AppSetting.h"
#include "QEventProcessor.h"
#include "ViewsEnums.h"

QJsonObject matrixbrigde::stringToJson(const QString &content)
{
    QJsonObject obj;
    QJsonDocument doc = QJsonDocument::fromJson(content.toUtf8());
    if(doc.isNull())
    {
        DBG_INFO << "JSON IS NULL";
    }
    else{
        if(doc.isObject())
        {
            obj = doc.object();
        }
        else{
            DBG_INFO << "JSON IS NOT OBJECT";
        }
    }
    return obj;
}

QNunchukMatrixEvent matrixbrigde::InitWallet(const QString &room_id,
                                             const QString &name,
                                             int m,
                                             int n,
                                             ENUNCHUCK::AddressType address_type,
                                             bool is_escrow,
                                             const QString &description,
                                             QWarningMessage &msg)
{
    nunchuk::NunchukMatrixEvent e = matrixifaces::instance()->InitWallet(room_id.toStdString(),
                                                                         name.toStdString(),
                                                                         m,
                                                                         n,
                                                                         (nunchuk::AddressType)address_type,
                                                                         is_escrow,
                                                                         description.toStdString(),
                                                                         msg);
    return QNunchukMatrixEvent(e);
}

QNunchukMatrixEvent matrixbrigde::JoinWallet(const QString &room_id,
                                             const QSingleSignerPtr &signer,
                                             QWarningMessage &msg)
{
    if(signer){
        nunchuk::NunchukMatrixEvent e = matrixifaces::instance()->JoinWallet(room_id.toStdString(),
                                                                             signer->originSingleSigner(),
                                                                             msg);
        return QNunchukMatrixEvent(e);
    }
    return QNunchukMatrixEvent();
}

QNunchukMatrixEvent matrixbrigde::LeaveWallet(const QString &room_id,
                                              const QString &join_id,
                                              const QString &reason,
                                              QWarningMessage &msg)
{
    nunchuk::NunchukMatrixEvent e = matrixifaces::instance()->LeaveWallet(room_id.toStdString(),
                                                                          join_id.toStdString(),
                                                                          reason.toStdString(),
                                                                          msg);
    return QNunchukMatrixEvent(e);
}

QNunchukMatrixEvent matrixbrigde::CancelWallet(const QString &room_id,
                                               const QString &reason,
                                               QWarningMessage &msg)
{
    nunchuk::NunchukMatrixEvent e = matrixifaces::instance()->CancelWallet(room_id.toStdString(),
                                                                           reason.toStdString(),
                                                                           msg);
    return QNunchukMatrixEvent(e);
}

QNunchukMatrixEvent matrixbrigde::CreateWallet(const QString &room_id,
                                               QWarningMessage &msg)
{
    nunchuk::NunchukMatrixEvent e = matrixifaces::instance()->CreateWallet( nunchukiface::instance()->nunchukinstance(),
                                                                            room_id.toStdString(),
                                                                            msg);
    return QNunchukMatrixEvent(e);
}

QNunchukMatrixEvent matrixbrigde::DeleteWallet(const QString &room_id, QWarningMessage &msg)
{
    nunchuk::NunchukMatrixEvent e = matrixifaces::instance()->DeleteWallet( nunchukiface::instance()->nunchukinstance(),
                                                                            room_id.toStdString(),
                                                                            msg);
    return QNunchukMatrixEvent(e);
}

QNunchukMatrixEvent matrixbrigde::InitTransaction(const QString& room_id,
                                                  const QMap<QString, qint64> outputs,
                                                  const QString& memo,
                                                  const QUTXOListModelPtr inputs,
                                                  const int fee_rate,
                                                  bool subtract_fee_from_amount,
                                                  QWarningMessage& msg)
{
    std::map<std::string, nunchuk::Amount> out;
    for (int i = 0; i < outputs.keys().count(); i++) {
        out[outputs.keys().at(i).toStdString()] = outputs[outputs.keys().at(i)];
    }
    std::vector<nunchuk::UnspentOutput> in;
    if(inputs){
        for (int j = 0; j < inputs.data()->rowCount(); j++) {
            QUTXOPtr it = inputs.data()->getUTXOByIndex(j);
            if(it){
                nunchuk::UnspentOutput utxo;
                utxo.set_txid(it.data()->txid().toStdString());
                utxo.set_vout(it.data()->vout());
                utxo.set_address(it.data()->address().toStdString());
                utxo.set_amount(it.data()->amountSats());
                utxo.set_height(it.data()->height());
                in.push_back(utxo);
            }
        }
    }
    nunchuk::NunchukMatrixEvent e = matrixifaces::instance()->InitTransaction(nunchukiface::instance()->nunchukinstance(),
                                                                              room_id.toStdString(),
                                                                              out,
                                                                              memo.toStdString(),
                                                                              in,
                                                                              fee_rate,
                                                                              subtract_fee_from_amount,
                                                                              msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        return QNunchukMatrixEvent(e);
    }
    else{
        return QNunchukMatrixEvent();
    }
}

QNunchukMatrixEvent matrixbrigde::SignTransaction(const QString &room_id,
                                                   const QString &init_event_id,
                                                   const QDevicePtr &device,
                                                  QWarningMessage &msg)
{
    if(device){
        nunchuk::Device dv(device.data()->type().toStdString(),
                           device.data()->path().toStdString(),
                           device.data()->model().toStdString(),
                           device.data()->masterFingerPrint().toStdString(),
                           device.data()->needsPassPhraseSent(),
                           device.data()->needsPinSent());
        nunchuk::NunchukMatrixEvent e = matrixifaces::instance()->SignTransaction(room_id.toStdString(),
                                                                                  nunchukiface::instance()->nunchukinstance(),
                                                                                  init_event_id.toStdString(),
                                                                                  dv,
                                                                                  msg);
        emit AppModel::instance()->finishedSigningTransaction();
        return QNunchukMatrixEvent(e);
    }
    emit AppModel::instance()->finishedSigningTransaction();
    return QNunchukMatrixEvent();
}


QNunchukMatrixEvent matrixbrigde::SignAirgapTransaction(const QString &init_event_id,
                                                        const QString &master_fingerprint,
                                                        QWarningMessage &msg)
{
    nunchuk::NunchukMatrixEvent e = matrixifaces::instance()->SignAirgapTransaction(nunchukiface::instance()->nunchukinstance(),
                                                                                    init_event_id.toStdString(),
                                                                                    master_fingerprint.toStdString(),
                                                                                    msg);
    return QNunchukMatrixEvent(e);
}

QNunchukMatrixEvent matrixbrigde::RejectTransaction(const QString &room_id,
                                                    const QString &init_event_id,
                                                    const QString &reason,
                                                    QWarningMessage &msg)
{
    nunchuk::NunchukMatrixEvent e = matrixifaces::instance()->RejectTransaction(room_id.toStdString(),
                                                                                init_event_id.toStdString(),
                                                                                reason.toStdString(),
                                                                                msg);
    return QNunchukMatrixEvent(e);
}

QNunchukMatrixEvent matrixbrigde::CancelTransaction(const QString &room_id,
                                                    const QString &init_event_id,
                                                    const QString &reason,
                                                    QWarningMessage &msg)
{
    nunchuk::NunchukMatrixEvent e = matrixifaces::instance()->CancelTransaction(room_id.toStdString(),
                                                                                init_event_id.toStdString(),
                                                                                reason.toStdString(),
                                                                                msg);
    return QNunchukMatrixEvent(e);
}

QNunchukMatrixEvent matrixbrigde::BroadcastTransaction(const QString &room_id,
                                                       const QString& init_event_id,
                                                       QWarningMessage &msg)
{
    nunchuk::NunchukMatrixEvent e = matrixifaces::instance()->BroadcastTransaction(room_id.toStdString(),
                                                                                   nunchukiface::instance()->nunchukinstance(),
                                                                                   init_event_id.toStdString(),
                                                                                   msg);
    return QNunchukMatrixEvent(e);
}

QRoomWalletPtr matrixbrigde::ReloadRoomWallet( QNunchukRoom * const room)
{
    if(!room || !room->room() || HasRoomWallet(room->id()) == false){
        return NULL;
    }
    QWarningMessage msg;
    QRoomWalletPtr ret = GetRoomWallet(room->id(), msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type() && ret){
        if(ret.data()->get_cancel_event_id() != ""){
            return NULL;
        }
        if(ret.data()->get_init_event_id() == "" || ret.data()->get_init_event_id().isEmpty()){
            return NULL;
        }
        if(!ret.data()->walletSigners()){
            ret.data()->setWalletSigners(QWalletSignersModelPtr(new QWalletSignersModel()));
        }
        QWarningMessage msggetevt;
        QNunchukMatrixEvent evt = GetEvent(room->id(),ret.data()->get_init_event_id(), msggetevt);
        if((int)EWARNING::WarningType::NONE_MSG == msggetevt.type()){
            QJsonObject jsonObj = stringToJson(evt.get_content());
            ret.data()->updateWalletInfo(jsonObj);
        }
        QStringList userIds;
        userIds.reserve(room->room()->users().size());
        for (auto u : room->room()->users()){
            userIds.append(u->id());
        }
        QJsonObject json_content_object = stringToJson(ret.data()->get_json_content());
        QJsonObject joins = json_content_object["joins"].toObject();
        QStringList joins_Id = joins.keys();
        for (QString userId : joins_Id) {
            DBG_INFO << userId;
            QJsonArray joiner = joins[userId].toArray();
            for (auto v : joiner) {
                QJsonObject it = v.toObject();
                QJsonObject signerInfo;
                signerInfo["xfp"] = it["master_fingerprint"].toString();
                signerInfo["xpub"] = "";
                signerInfo["bip32path"] = it["derivation_path"].toString();
                signerInfo["joid_id"] = it["join_event_id"].toString();
                signerInfo["signer_type"] = (int)qUtils::GetSignerType(it["signer_type"].toString());
                signerInfo["is_localuser"] = room->checkIsLocalUser(userId);
                signerInfo["name"] = userId;//roomuser.email;
                ret.data()->updateSignerInfo(signerInfo);
            }
            if(userIds.contains(userId)){
                userIds.removeAll(userId);
            }
        }
        for (QString userId : userIds) {
            QJsonObject signerInfo;
            signerInfo["xfp"] = "";
            signerInfo["xpub"] = "";
            signerInfo["bip32path"] = "";
            signerInfo["joid_id"] = "";
            signerInfo["signer_type"] = (int)qUtils::GetSignerType("HARDWARE");
            signerInfo["is_localuser"] = room->checkIsLocalUser(userId);
            signerInfo["name"] = userId;//roomuser.email;
            ret.data()->updateSignerInfo(signerInfo);
        }
        room->setWalletImport(room->createWalletFromJson(json_content_object));
        // Check created wallet
        QString wallet_id = ret.data()->get_wallet_id();
        if(wallet_id != ""){
            if(AppModel::instance()->walletList()){
                if(!AppModel::instance()->walletList()->containsId(wallet_id)){
                    QWalletPtr wl = bridge::nunchukGetWallet(wallet_id);
                    if(wl){
                        wl.data()->setIsSharedWallet(true);
                        wl.data()->setRoomId(room->id());
                        wl.data()->setInitEventId(ret.data()->get_init_event_id());
                        wl.data()->setName(ret.data()->walletName());
                        AppModel::instance()->walletList()->addWallet(wl);
                        AppModel::instance()->requestSyncWalletDb(wallet_id);
                        ret.data()->setWalletInfo(wl);
                    }
                }
                else{
                    AppModel::instance()->walletList()->updateSharedWalletById(wallet_id, room->id(), ret.data()->get_init_event_id(), ret.data()->walletName());
                    if(AppModel::instance()->walletInfo() && (0 == QString::compare(AppModel::instance()->walletInfo()->id(), wallet_id, Qt::CaseInsensitive))){
                        AppModel::instance()->walletInfo()->setIsSharedWallet(true);
                    }
                    QWalletPtr realWl = AppModel::instance()->walletList()->getWalletById(wallet_id);
                    ret.data()->setWalletInfo(realWl);
                }
                for (SignerAssigned signer : ret.data()->walletSigners()->fullList()) {
                    AppModel::instance()->walletList()->updateSignerOfRoomWallet(wallet_id, signer);
                }
            }
        }
        return ret;
    }
    else{
        return NULL;
    }
}

QRoomWalletPtr matrixbrigde::GetRoomWallet(const QString &room_id,
                                           QWarningMessage &msg)
{
    nunchuk::RoomWallet ret = matrixifaces::instance()->GetRoomWallet(room_id.toStdString(),
                                                                      msg);
    return QRoomWalletPtr( new QRoomWallet(ret));
}

QRoomTransactionModelPtr matrixbrigde::GetPendingTransactions(const QString &room_id)
{
    QWarningMessage msg;
    QRoomTransactionModelPtr ret = QRoomTransactionModelPtr(new QRoomTransactionModel()) ;
    std::vector<nunchuk::RoomTransaction> results = matrixifaces::instance()->GetPendingTransactions(room_id.toStdString(), msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        foreach (nunchuk::RoomTransaction room_tx, results) {
            QWarningMessage txWarning;
            nunchuk::Transaction tx = bridge::nunchukGetOriginTransaction(QString::fromStdString(room_tx.get_wallet_id()),
                                                                          QString::fromStdString(room_tx.get_tx_id()),
                                                                          txWarning);
            if((int)EWARNING::WarningType::NONE_MSG == txWarning.type()){
                QTransactionPtr rawtx = bridge::convertTransaction(tx, QString::fromStdString(room_tx.get_wallet_id()));
                if(rawtx){
                    rawtx.data()->setRoomId(room_id);
                    rawtx.data()->setInitEventId(QString::fromStdString(room_tx.get_init_event_id()));
                    QRoomTransactionPtr target = QRoomTransactionPtr(new QRoomTransaction(room_tx));
                    target.data()->setTransaction(rawtx);
                    int status = (target.data()->transaction()) ? target.data()->transaction()->status() : (int)ENUNCHUCK::TransactionStatus::NETWORK_REJECTED;
                    if(status == (int)ENUNCHUCK::TransactionStatus::PENDING_SIGNATURES
                            || status == (int)ENUNCHUCK::TransactionStatus::READY_TO_BROADCAST
                            || status == (int)ENUNCHUCK::TransactionStatus::PENDING_CONFIRMATION)
                    {
                        ret.data()->addTransaction(target);
                    }
                }
            }
        }
    }
    return ret;
}

void matrixbrigde::ConsumeEvent(const QString& room_id,const QNunchukMatrixEvent &event)
{
    QWarningMessage msg;
    matrixifaces::instance()->ConsumeEvent(room_id.toStdString(),
                                           nunchukiface::instance()->nunchukinstance(),
                                           event.nunchukEvent(),
                                           msg);
}

void matrixbrigde::makeMatrixInstance(const QString &account,
                                      const QString &device_id,
                                      QWarningMessage &msg)
{
    AppSetting::instance()->setGroupSetting(account);
    qUtils::SetChain((nunchuk::Chain)AppSetting::instance()->primaryServer());
    nunchuk::AppSettings setting;

    // Chain setting
    setting.set_chain((nunchuk::Chain)AppSetting::instance()->primaryServer());

    // mainnet sever
    std::vector<std::string> mainnetServer;
    mainnetServer.push_back(AppSetting::instance()->mainnetServer().toStdString());
    mainnetServer.push_back(AppSetting::instance()->secondaryServer().toStdString());
    setting.set_mainnet_servers(mainnetServer);

    // Testnet server
    std::vector<std::string> testnetServer;
    testnetServer.push_back(AppSetting::instance()->testnetServer().toStdString());
    testnetServer.push_back(AppSetting::instance()->secondaryServer().toStdString());
    setting.set_testnet_servers(testnetServer);

    // Signet server
    std::vector<std::string> signetServer;
    signetServer.push_back(AppSetting::instance()->signetServer().toStdString());
    signetServer.push_back(AppSetting::instance()->secondaryServer().toStdString());
    setting.set_signet_servers(signetServer);

    // hwi path
    setting.set_hwi_path(bridge::hwiPath().toStdString());

    //  certificate file
    QString certPath = "";
    if(AppSetting::instance()->enableCertificateFile()){
        certPath = AppSetting::instance()->certificateFile();
    }
    setting.set_certificate_file(certPath.toStdString());

    // Storage path
    setting.set_storage_path(AppSetting::instance()->storagePath().toStdString());

    setting.enable_proxy(AppSetting::instance()->enableTorProxy());
    setting.set_proxy_host(AppSetting::instance()->torProxyAddress().toStdString());
    setting.set_proxy_port(AppSetting::instance()->torProxyPort());
    setting.set_proxy_username(AppSetting::instance()->torProxyName().toStdString());
    setting.set_proxy_password(AppSetting::instance()->torProxyPassword().toStdString());

    // Core RPC
    if(AppSetting::instance()->enableCoreRPC()){
        setting.set_backend_type(nunchuk::BackendType::CORERPC);
    }
    else{
        setting.set_backend_type(nunchuk::BackendType::ELECTRUM);
    }
    setting.set_corerpc_host(AppSetting::instance()->coreRPCAddress().toStdString());
    setting.set_corerpc_port(AppSetting::instance()->coreRPCPort());
    setting.set_corerpc_username(AppSetting::instance()->coreRPCName().toStdString());
    setting.set_corerpc_password(AppSetting::instance()->coreRPCPassword().toStdString());

    matrixifaces::instance()->makeMatrixInstance(setting,
                                                 "",
                                                 account.toStdString(),
                                                 device_id.toStdString(),
                                                 msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        matrixbrigde::EnableGenerateReceiveEvent();
    }
}

QNunchukMatrixEvent matrixbrigde::GetEvent(const QString& room_id,const QString &event_id, QWarningMessage &msg)
{
    nunchuk::NunchukMatrixEvent ret = matrixifaces::instance()->GetEvent(room_id.toStdString(),
                                                                         event_id.toStdString(),
                                                                         msg);
    return QNunchukMatrixEvent(ret);
}

QList<QRoomWalletPtr> matrixbrigde::GetAllRoomWallets(QWarningMessage &msg)
{
    QList<QRoomWalletPtr> ret;
    std::vector<nunchuk::RoomWallet> wallets = matrixifaces::instance()->GetAllRoomWallets(msg);
    for(nunchuk::RoomWallet wallet : wallets){
        if(wallet.get_wallet_id() != "" && wallet.get_finalize_event_id() != "")
        {
            QString room_id = QString::fromStdString(wallet.get_room_id());
            if(CLIENT_INSTANCE->rooms() && CLIENT_INSTANCE->rooms()->containsRoomId(room_id)){
                QNunchukRoomPtr room = CLIENT_INSTANCE->rooms()->getRoomById(room_id);
                QRoomWalletPtr roomWallet = QRoomWalletPtr( new QRoomWallet(wallet));
                QWarningMessage msggetevt;
                QNunchukMatrixEvent evt = GetEvent(roomWallet.data()->get_room_id(),
                                                   roomWallet.data()->get_init_event_id(),
                                                   msggetevt);
                if((int)EWARNING::WarningType::NONE_MSG == msggetevt.type()){
                    // Wallet info
                    QJsonObject jsonObj = stringToJson(evt.get_content());
                    roomWallet.data()->updateWalletInfo(jsonObj);
                    //Signer info
                    QStringList userIds;
                    userIds.reserve(room->room()->users().size());
                    for (auto u : room->room()->users()){
                        userIds.append(u->id());
                    }
                    QJsonObject json_content_object = stringToJson(roomWallet.data()->get_json_content());
                    QJsonObject joins = json_content_object["joins"].toObject();
                    QStringList joins_Id = joins.keys();
                    for (QString userId : joins_Id) {
                        QJsonArray joiner = joins[userId].toArray();
                        for (auto v : joiner) {
                            QJsonObject it = v.toObject();
                            QJsonObject signerInfo;
                            signerInfo["xfp"] = it["master_fingerprint"].toString();
                            signerInfo["xpub"] = "";
                            signerInfo["bip32path"] = it["derivation_path"].toString();
                            signerInfo["joid_id"] = it["join_event_id"].toString();
                            signerInfo["signer_type"] = (int)qUtils::GetSignerType(it["signer_type"].toString());
                            signerInfo["is_localuser"] = room->checkIsLocalUser(userId);
                            signerInfo["name"] = userId;//roomuser.email;
                            roomWallet.data()->updateSignerInfo(signerInfo);
                        }
                        if(userIds.contains(userId)){
                            userIds.removeAll(userId);
                        }
                    }
                    for (QString userId : userIds) {
                        QJsonObject signerInfo;
                        signerInfo["xfp"] = "";
                        signerInfo["xpub"] = "";
                        signerInfo["bip32path"] = "";
                        signerInfo["joid_id"] = "";
                        signerInfo["signer_type"] = (int)qUtils::GetSignerType("HARDWARE");
                        signerInfo["is_localuser"] = room->checkIsLocalUser(userId);
                        signerInfo["name"] = userId;//roomuser.email;
                        roomWallet.data()->updateSignerInfo(signerInfo);
                    }
                }
                ret.append(roomWallet);
            }
        }
    }
    return ret;
}

QRoomTransactionPtr matrixbrigde::GetRoomTransaction(const QString &room_id,const QString &init_event_id, QWarningMessage &msg)
{
    nunchuk::RoomTransaction tx = matrixifaces::instance()->GetRoomTransaction(room_id.toStdString(),
                                                                               init_event_id.toStdString(),
                                                                               msg);
    DBG_INFO << QString::fromStdString(tx.get_room_id());
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        QRoomTransactionPtr ret = QRoomTransactionPtr(new QRoomTransaction(tx));
        QTransactionPtr rawtx = bridge::nunchukGetTransaction(QString::fromStdString(tx.get_wallet_id()),
                                                              QString::fromStdString(tx.get_tx_id()));
        if(rawtx){
            rawtx.data()->setRoomId(QString::fromStdString(tx.get_room_id()));
            rawtx.data()->setWalletId(QString::fromStdString(tx.get_wallet_id()));
            rawtx.data()->setInitEventId(QString::fromStdString(tx.get_init_event_id()));
        }
        ret.data()->setTransaction(rawtx);
        return ret;
    }
    else{
        return NULL;
    }
}

nunchuk::RoomTransaction matrixbrigde::GetOriginRoomTransaction(const QString &room_id,const QString &init_event_id, QWarningMessage &msg)
{
    return matrixifaces::instance()->GetRoomTransaction(room_id.toStdString(),
                                                        init_event_id.toStdString(),
                                                        msg);
}

nunchuk::RoomTransaction matrixbrigde::GetOriginPendingTransaction(const QString &room_id, const QString &tx_id, QWarningMessage &msg)
{
    std::vector<nunchuk::RoomTransaction> results = matrixifaces::instance()->GetPendingTransactions(room_id.toStdString(), msg);
    foreach (nunchuk::RoomTransaction tx, results) {
        if(0 == tx.get_tx_id().compare(tx_id.toStdString())){
            return tx;
        }
    }
    return {};
}

QString matrixbrigde::GetTransactionId(const QString &room_id,const QString &init_event_id, QWarningMessage &msg)
{
    std::string ret = matrixifaces::instance()->GetTransactionId(room_id.toStdString(),
                                                                 init_event_id.toStdString(),
                                                                 msg);
    return QString::fromStdString(ret);
}

void matrixbrigde::ConsumeSyncEvent(const QString& room_id,const QNunchukMatrixEvent &event)
{
    QWarningMessage msg;
    matrixifaces::instance()->ConsumeSyncEvent(room_id.toStdString(),nunchukiface::instance()->nunchukinstance(),
                                               event.nunchukEvent(),
                                               msg);
}

void matrixbrigde::RegisterAutoBackup(const QString &sync_room_id,
                                      const QString &access_token)
{
    QWarningMessage msg;
    matrixifaces::instance()->RegisterAutoBackup(nunchukiface::instance()->nunchukinstance(),
                                               sync_room_id.toStdString(),
                                               access_token.toStdString(),
                                               msg);
}

void matrixbrigde::EnableGenerateReceiveEvent()
{
    QWarningMessage msg;
    matrixifaces::instance()->EnableGenerateReceiveEvent(nunchukiface::instance()->nunchukinstance(), msg);
}

bool matrixbrigde::HasRoomWallet(const QString &room_id)
{
    QWarningMessage msg;
    return matrixifaces::instance()->HasRoomWallet(room_id.toStdString(), msg);
}

void matrixbrigde::UploadFileCallback(const QString &json_info, const QString &file_url)
{
    matrixifaces::instance()->UploadFileCallback(json_info.toStdString(),
                                                 file_url.toStdString());
}

void matrixbrigde::DownloadFileCallback(const QString &json_info, const std::vector<unsigned char> &file_data)
{
    matrixifaces::instance()->DownloadFileCallback(nunchukiface::instance()->nunchukinstance(),
                                                   json_info.toStdString(),
                                                   file_data,
                                                   DownloadFileProgress);
}

void matrixbrigde::EnableAutoBackup(bool state)
{
    QWarningMessage msg;
    matrixifaces::instance()->EnableAutoBackup(state, msg);
}

void matrixbrigde::Backup()
{
    QWarningMessage msg;
    matrixifaces::instance()->Backup(nunchukiface::instance()->nunchukinstance(), msg);
}

QNunchukMatrixEvent matrixbrigde::ImportWallet(const QString &room_id,
                                               const QString &name,
                                               const QString &description,
                                               const QString& filepath,
                                               QWarningMessage &msg)
{
    DBG_INFO << room_id << name << description << filepath;
    nunchuk::NunchukMatrixEvent e;
    QFile sourceFile(filepath);
    if(sourceFile.open(QIODevice::ReadOnly)){
        QString descs = sourceFile.readAll();
        sourceFile.close();
        nunchuk::Wallet wallet = qUtils::ParseWalletDescriptor(descs,msg);
        e = matrixifaces::instance()->ImportWallet(room_id.toStdString(),
                                                 name.toStdString(),
                                                 wallet.get_m(),
                                                 wallet.get_n(),
                                                 (nunchuk::AddressType)wallet.get_address_type(),
                                                 wallet.is_escrow(),
                                                 description.toStdString(),
                                                 wallet.get_signers(),
                                                 msg);

    }else{
        msg.setWarningMessage(-1, "", EWARNING::WarningType::EXCEPTION_MSG);
    }
    return QNunchukMatrixEvent(e);
    // De call ham nay em call event EVT_IMPORT_SHARED_WALLET_REQUEST va
    // Read bsms file here to get content

    // Then, call qUtils::ParseWalletDescriptor return a wallet
    // Check msg
    // If false ==> Show toast error message
    // If OK call matrixbrigde::InitWallet with data from wallet return from ParseWalletDescriptor
    // check message
    // False false ==> show toast
    // If ok, send event EVT_IMPOR_SHARED_WALLET_SUCCEED
}

QNunchukMatrixEvent matrixbrigde::ImportWalletQR(const QString &room_id,
                                                 const QString &name,
                                                 const QString &description,
                                                 nunchuk::Chain chain,
                                                 const QStringList qrtags,
                                                 QWarningMessage& msg)
{
    DBG_INFO << room_id << name << qrtags;
    nunchuk::NunchukMatrixEvent e;
    QStringList in = qrtags;
    in.removeDuplicates();
    if(in.isEmpty()){
        msg.setWarningMessage(-1, "", EWARNING::WarningType::EXCEPTION_MSG);
        return QNunchukMatrixEvent();
    }
    nunchuk::Wallet wallet = qUtils::ParseKeystoneWallet(chain,in,msg);
    if (msg.type() == (int)EWARNING::WarningType::NONE_MSG) {
        e = matrixifaces::instance()->ImportWallet(room_id.toStdString(),
            name.toStdString(),
            wallet.get_m(),
            wallet.get_n(),
            (nunchuk::AddressType)wallet.get_address_type(),
            wallet.is_escrow(),
            description.toStdString(),
            wallet.get_signers(),
            msg);
        if (msg.type() == (int)EWARNING::WarningType::NONE_MSG) {
            return QNunchukMatrixEvent(e);
        }
    }
    return QNunchukMatrixEvent();
}
