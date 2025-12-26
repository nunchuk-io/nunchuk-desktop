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
#include "QNunchukRoomModel.h"
#include "connection.h"
#include "uriresolver.h"
#include "csapi/joining.h"
#include "csapi/leaving.h"
#include "csapi/room_send.h"
#include "events/roomevent.h"
#include "events/reactionevent.h"
#include "events/redactionevent.h"
#include "events/simplestateevents.h"
#include "events/roommessageevent.h"
#include <functional>
#include "QOutlog.h"
#include "ClientController.h"
#include "Chats/matrixbrigde.h"
#include "ifaces/bridgeifaces.h"
#include "AppModel.h"
#include "ViewsEnums.h"
#include "Servers/Draco.h"
#include "localization/STR_CPP.h"
#include <QTextDocumentFragment>
#include "Premiums/QUserWallets.h"
#include "Premiums/QGroupWallets.h"
#include "Premiums/QInheritancePlan.h"
#include "Premiums/QWalletServicesTag.h"
#include "Premiums/QGroupWalletDummyTx.h"
#include "QThreadForwarder.h"

QNunchukRoom::QNunchukRoom(Room *r):
    m_room(r),
    m_conversation(QConversationModelPtr(new QConversationModel(r))),
    m_lasttimestamp(0),
    m_users(QNunchukRoomUsersPtr(new QNunchukRoomUsers(r))),
    m_roomWallet(NULL),
    m_lastMessage(""),
    m_pendingTxs(QRoomTransactionModelPtr(new QRoomTransactionModel())),
    m_txs(QRoomTransactionModelPtr(new QRoomTransactionModel())),
    m_downloaded(false),
    m_pinTransaction(nullptr),
    m_IsEncrypted(false),
    m_maxLifeTime(-1)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
    QQmlEngine::setObjectOwnership(m_users.data(), QQmlEngine::CppOwnership);
    m_typingNames.clear();

    if(m_room){
        QQmlEngine::setObjectOwnership(m_room, QQmlEngine::CppOwnership);
    }
    qmlRegisterType<FileTransferInfo>();
    qRegisterMetaType<FileTransferInfo>();
    connect(AppSetting::instance(), &AppSetting::enableColabChanged, this, &QNunchukRoom::isIgnoredCollabWalletChanged);
}

QNunchukRoom::~QNunchukRoom()
{

}

bool QNunchukRoom::isIgnoredCollabWallet() const
{
    bool isByzantineAccount = CLIENT_INSTANCE->isByzantine();
    bool isFinneyAccount = CLIENT_INSTANCE->isFinney();
    bool ret {false};
    if (isByzantineAccount || isFinneyAccount) {
        ret = true;
    }
    else {
        ret = !AppSetting::instance()->enableColab();
    }
    return ret;
}

bool QNunchukRoom::isNunchukByzantineRoom() const
{
    if(m_room){
        return m_room->currentState().contains(NUNCHUK_ROOM_BYZANTINE);
    }
    return false;
}

bool QNunchukRoom::isServerNoticeRoom() const
{
    return (m_room ? m_room->isServerNoticeRoom() : false);
}

bool QNunchukRoom::isNunchukSyncRoom() const
{
    return m_room ? (m_room->tagNames().contains(NUNCHUK_ROOM_SYNC)) : false;
}

bool QNunchukRoom::isSupportRoom() const
{
    QString tagname = (int)ENUNCHUCK::Chain::MAIN == (int)AppSetting::instance()->primaryServer() ?  NUNCHUK_ROOM_SUPPORT : NUNCHUK_ROOM_SUPPORTTESTNET;
    return m_room ? (m_room->tagNames().contains(tagname)) : false;
}

bool QNunchukRoom::isDirectChat() const
{
    return m_room ? m_room->isDirectChat() : false;
}

bool QNunchukRoom::isByzantineRoom() const
{
    bool ret = (m_room ? m_room->currentState().contains(NUNCHUK_ROOM_BYZANTINE) : false );
    DBG_INFO << ret;
    return ret;
}

QString QNunchukRoom::byzantineRoomGroupId()
{
    if(m_room && m_room->currentState().contains(NUNCHUK_ROOM_BYZANTINE)){
        QJsonObject content = m_room->currentState().contentJson(NUNCHUK_ROOM_BYZANTINE);
        DBG_INFO << content;
        return content["group_id"].toString();
    }
    return "";
}

QString QNunchukRoom::localUserName() const
{
    if(m_room) return m_room->localUser()->displayname(m_room);
    else return "";
}

QString QNunchukRoom::id() const
{
    if(m_room) return m_room->id();
    else return "";
}

QStringList QNunchukRoom::aliases() const
{
    if(m_room) return m_room->aliases();
    else return QStringList();
}

QString QNunchukRoom::status() const
{
    return "";
}

int QNunchukRoom::userCount() const
{
    if(m_room){
        return m_room->users().count();
    }
    else{
        return 0;
    }
}

QStringList QNunchukRoom::userNames()
{
    QStringList ret;
    ret.clear();
    if(m_room){
        ret = m_room->memberNames();//safeMemberNames();
    }
    ret.count();
    return ret;
}

QStringList QNunchukRoom::talkersName()
{
    QStringList ret;
    ret.clear();
    if(m_room){
        QString local_id = room()->localUser()->id();
        for (int i = 0; i < m_room->users().count(); ++i) {
            if(m_room->users().at(i)){
                QString user_id = m_room->users().at(i)->id();
                if(isDirectChat() && qUtils::strCompare(user_id, local_id)){
                    continue;
                }
                else{
                    ret.append(m_room->users().at(i)->name(m_room));
                }
            }
        }
    }
    return ret;
}

QStringList QNunchukRoom::talkersAvatar()
{
    QStringList ret;
    ret.clear();
    if(m_room){
        QString local_id = room()->localUser()->id();
        for (int i = 0; i < m_room->users().count(); ++i) {
            if(m_room->users().at(i)){
                QString user_id = m_room->users().at(i)->id();
                if(isDirectChat() && qUtils::strCompare(user_id, local_id)){
                    continue;
                }
                else{
                    ret.append(m_room->users().at(i)->avatarMediaId(m_room));
                }
            }
        }
    }
    return ret;
}

QString QNunchukRoom::roomAvatar()
{
    if(m_room && userCount() == 2){
        int targetId = userNames().indexOf(m_room->localUser()->name()) == 0 ? 1 : 0 ;
        return (targetId != -1) ? m_room->users().at(targetId)->avatarMediaId() : "";
    }
    else{
        return "";
    }
}

QString QNunchukRoom::roomName()
{
    if(m_room){
        if(isServerNoticeRoom() ) {
            return "Nunchuk Service";
        }
        else if(isNunchukSyncRoom()){
            return "Nunchuk Sync";
        }
        else if(isSupportRoom()){
            return "Support";
        }
        else if(isNunchukByzantineRoom()){
            return m_room->name() != "" ? m_room->name() : m_room->displayName();
        }
        else if(isDirectChat()){
            return m_room->name() != "" ? m_room->name() : m_room->displayName() != "" ? m_room->displayName() : userNames().join(", ");
        }
        else{
            if(userCount() == 2){
                int targetId = userNames().indexOf(m_room->localUser()->name()) == 0 ? 1 : 0 ;
                return (targetId != -1) ? userNames().at(targetId) : "Unknown";
            }
            else{
                return m_room->name() != "" ? m_room->name() : m_room->displayName() != "" ? m_room->displayName() : userNames().join(", ");
            }
        }
    }
    else{
        return STR_CPP_002;
    }
}

void QNunchukRoom::setRoomName(const QString &name)
{
    if(m_room){
        m_room->setName(name);
        emit roomNameChanged();
    }
}

Room *QNunchukRoom::room() const
{
    return m_room;
}

JoinState QNunchukRoom::roomJoinState()
{
    if(m_room) {
        return m_room->joinState();
    }
    return JoinState::Leave;
}

int QNunchukRoom::unreadCount() const
{
    if(m_room){
        return m_room->unreadCount() > 0 ? m_room->unreadCount() : 0;
    }
    return 0;
}

QString QNunchukRoom::postEvent(const QString& eventType, const QJsonObject& content)
{
    QString txnId = "";
    RoomEvent* evt = NULL;
    if(qUtils::strCompare(eventType, NUNCHUK_EVENT_WALLET)){
        evt = new QNunchukWalletEvent(eventType, content);
    }
    else if(qUtils::strCompare(eventType, NUNCHUK_EVENT_TRANSACTION)){
        evt = new QNunchukTransactionEvent(eventType, content);
    }
    else if(qUtils::strCompare(eventType, NUNCHUK_EVENT_SYNC)){
        evt = new QNunchukSyncEvent(eventType, content);
    }
    else if(qUtils::strCompare(eventType, NUNCHUK_EVENT_EXCEPTION)){
        evt = new QNunchukExceptionEvent(eventType, content);
    }
    else {}
    if(m_room && evt){
        if(isNunchukSyncRoom() && !AppSetting::instance()->enableMultiDeviceSync()){
            return "";
        }
        txnId = m_room->postEvent(evt);
        if(validatePendingEvent(txnId)){
            return txnId;
        }
    }
    return "";
}

QString QNunchukRoom::postJson(const QString &matrixType, const QJsonObject &content)
{
    QString txnId = "";
    if(m_room){
        txnId = postJson(matrixType, content);
        if(validatePendingEvent(txnId)){
            DBG_INFO << "POST JSON SUCCEED";
        }
    }
    return "";
}

void QNunchukRoom::slotFinishedDownloadTransaction(nunchuk::RoomTransaction room_tx, nunchuk::Transaction tx, const Conversation &cons)
{
    QString wallet_id = QString::fromStdString(room_tx.get_wallet_id());
    QRoomTransactionPtr target = QRoomTransactionPtr(new QRoomTransaction(room_tx));
    if(tx.get_txid() != ""){
        QTransactionPtr rawtx = bridge::convertTransaction(tx, wallet_id);
        if(rawtx){
            rawtx.data()->setRoomId(id());
            rawtx.data()->setInitEventId(QString::fromStdString(room_tx.get_init_event_id()));
            target.data()->setTransaction(rawtx);
            if(conversation()){
                conversation()->updateTransaction(cons, target);
                setPinTransaction(conversation()->pinTransaction());
            }
        }
    }
    if(!isDownloaded()){
        startGetPendingTxs();
    }
    if(AppModel::instance()->transactionInfo() && target.data()->transaction() && (qUtils::strCompare(QString::fromStdString(room_tx.get_init_event_id()), target.data()->transaction()->initEventId())))
    {
        AppModel::instance()->setTransactionInfo(target.data()->transactionPtr());
    }
}

void QNunchukRoom::slotFinishFinalizeWallet(QString what, int type, int code)
{
    if(type != (int)EWARNING::WarningType::NONE_MSG){
        AppModel::instance()->showToast(code, what, (EWARNING::WarningType)type);
    }
}

void QNunchukRoom::slotFinishCancelWallet(QString what, int type, int code)
{
    if(type != (int)EWARNING::WarningType::NONE_MSG){
        AppModel::instance()->showToast(code, what, (EWARNING::WarningType)type);
    }
}

void QNunchukRoom::slotFinishedGetPendingTxs(QRoomTransactionModelPtr txs)
{
    setPendingTxs(txs->clone());
}

void QNunchukRoom::slotUpdateInitEventId(const Conversation cons)
{
    if(conversation()){
        conversation()->updateInitEventId(cons);
    }
}

void QNunchukRoom::sendMessage(const QString &message)
{
    if(m_room && conversation() && message != ""){
        QString txnId = m_room->postPlainText(message);
        if(validatePendingEvent(txnId)){
            Conversation cons;
            cons.sendByMe = true;
            cons.sender   = m_room->localUser()->displayname(room()) != "" ? m_room->localUser()->displayname(room()) : m_room->localUser()->id();
            cons.receiver =  m_room->localUser()->displayname(room());
            cons.timestamp = QDateTime::currentDateTime().toTime_t();
            cons.message = Quotient::prettyPrint(message);
            cons.messageType = (int)ENUNCHUCK::ROOM_EVT::PLAIN_TEXT;
            cons.txnId = txnId;
            cons.visible = isValidMessageTime(cons);
            conversation()->addMessage(cons);
            conversation()->requestSortByTimeAscending();
            setLastMessage(cons);
            setLasttimestamp(cons);
        }
        else{
            DBG_INFO << "SEND FALSE";
        }
    }
}

void QNunchukRoom::sendReaction(const QString &react)
{
    if(m_room){
        const auto targetEvtId = m_room->messageEvents().back()->id();
        const auto txnId = m_room->postReaction(targetEvtId, react);
        if(validatePendingEvent(txnId)){
            DBG_INFO << "SEND OK";
        }
        else{
            DBG_INFO << "SEND FALSE";
        }
    }
}

void QNunchukRoom::sendFile(const QString& description, const QString localFile)
{
    if(m_room){
        QString filepath = qUtils::QGetFilePath(localFile);
        if(filepath == "") return;
        int file_mimeType = (int)ENUNCHUCK::ROOM_EVT::FILE_OTHER;

        QMimeDatabase db;
        QMimeType mime = db.mimeTypeForFile(filepath);
        QString file_caption = QFileInfo(localFile).fileName();
        if (mime.name().startsWith("image/")) {
            file_mimeType = (int)ENUNCHUCK::ROOM_EVT::FILE_IMAGE;
        } else if (mime.name().startsWith("video/")) {
            file_mimeType = (int)ENUNCHUCK::ROOM_EVT::FILE_VIDEO;
        } else {
            file_mimeType = (int)ENUNCHUCK::ROOM_EVT::FILE_OTHER;
        }
        auto txnId = m_room->postFile(description.isEmpty()
                                                 ? QUrl(filepath).fileName()
                                                 : description,
                                             QUrl::fromLocalFile(filepath));
        QObject::connect(m_room, &Room::fileTransferCompleted,
                         [=](QString id, QUrl /*localFile*/, QUrl /*mxcUrl*/) {
            if (id == txnId) {
                DBG_INFO << "fileTransferCompleted";
            }
        });
        QObject::connect(m_room, &Room::fileTransferFailed, [=](QString id, QString /*error*/) {
            if (id == txnId) {
                DBG_INFO << "fileTransferFailed";
            }
        });
        QObject::connect( m_room, &Room::fileTransferProgress, [=](QString id, qint64 progress, qint64 total) {
            if (id == txnId) {
                DBG_INFO << "fileTransferProgress:" << progress << total;
            }
        });

        if(validatePendingEvent(txnId)){
            Conversation cons;
            cons.sendByMe = true;
            cons.sender   = m_room->localUser()->displayname(room()) != "" ? m_room->localUser()->displayname(room()) : m_room->localUser()->id();
            cons.receiver =  m_room->localUser()->displayname(room());
            cons.timestamp = QDateTime::currentDateTime().toTime_t();
            cons.messageType = file_mimeType;
            cons.file_path =  QUrl::fromLocalFile(filepath).toString();
            cons.txnId = txnId;
            cons.visible = isValidMessageTime(cons);
            if(file_mimeType == (int)ENUNCHUCK::ROOM_EVT::FILE_OTHER){
                if(description != ""){
                    QString messageInput = QString("%1 \n %2").arg(file_caption).arg(description);
                    cons.message = Quotient::prettyPrint(messageInput);
                }
                else{
                    cons.message = Quotient::prettyPrint(file_caption);
                }
            }
            else{
                cons.message = Quotient::prettyPrint(description);
            }
            conversation()->addMessage(cons);
            conversation()->requestSortByTimeAscending();
            setLastMessage(cons);
            setLasttimestamp(cons);
        }
    }
}

void QNunchukRoom::inviteToRoom(const QString &memberId)
{
    if(m_room){
        m_room->inviteToRoom(memberId);
    }
}

void QNunchukRoom::kickMember(const QString& memberId)
{
    if(m_room){
        QString kickreason = QString("%1 %2").arg(STR_CPP_005).arg(localUserName());
        m_room->kickMember(memberId, kickreason);
    }
}

void QNunchukRoom::banMember(const QString &userId)
{
    if(m_room){
        QString banreason = QString("%1 %2").arg(STR_CPP_005).arg(localUserName());
        m_room->ban(userId, banreason);
    }
}

void QNunchukRoom::getMoreContents(const int limit)
{
    if(m_room){
        m_room->getPreviousContent(limit);
    }
}

bool QNunchukRoom::joinWalletWithMasterSigner(const QString &id, bool &needXpub)
{
    bool ret = false;
    if(roomWallet()){
        ENUNCHUCK::WalletType walletType = roomWallet()->walletEscrow() ?  ENUNCHUCK::WalletType::ESCROW  :
                                                                           roomWallet()->walletN() > 1 ? ENUNCHUCK::WalletType::MULTI_SIG :
                                                                                                         ENUNCHUCK::WalletType::SINGLE_SIG;
        ENUNCHUCK::AddressType addressType = (ENUNCHUCK::AddressType)roomWallet()->walletAddressType().toInt();
        QMasterSignerPtr it = AppModel::instance()->masterSignerList()->getMasterSignerById(id);
        QWarningMessage msgWarning;
        QSingleSignerPtr signer = bridge::nunchukGetAvailableSignerFromMasterSigner(it, walletType, addressType, msgWarning);
        if(signer && msgWarning.type() == (int)EWARNING::WarningType::NONE_MSG){
            msgWarning.resetWarningMessage();
            matrixbrigde::JoinWallet(this->id(), signer, msgWarning);
            if((int)EWARNING::WarningType::NONE_MSG == msgWarning.type()){
                ret = true;
            }
            else{
                AppModel::instance()->showToast(msgWarning.code(), msgWarning.what(), (EWARNING::WarningType)msgWarning.type());
            }
        }
        else{
            needXpub = true;
            AppModel::instance()->showToast(msgWarning.code(), msgWarning.what(), (EWARNING::WarningType)msgWarning.type());
        }
    }
    return ret;
}

bool QNunchukRoom::joinWalletWithSingleSigner(const QString &xfp)
{
    bool ret = false;
    QSingleSignerPtr signer = AppModel::instance()->remoteSignerList()->getSingleSignerByFingerPrint(xfp);
    if(signer){
        QWarningMessage msgWarning;
        matrixbrigde::JoinWallet(id(), signer, msgWarning);
        if((int)EWARNING::WarningType::NONE_MSG == msgWarning.type()){
            ret = true;
        }
        else{
            AppModel::instance()->showToast(msgWarning.code(), msgWarning.what(), (EWARNING::WarningType)msgWarning.type());
        }
    }
    return ret;
}

bool QNunchukRoom::joinWalletUseSignerFromWalletImport(const QString &signer_id, const QString &xfp)
{
    bool ret = false;
    nunchuk::SingleSigner signer;
    for(nunchuk::SingleSigner s : m_walletImport.get_signers()){
        if((s.get_master_signer_id() != "" && (s.get_master_signer_id() == signer_id.toStdString() || s.get_master_signer_id() == xfp.toStdString()))
          || (s.get_master_fingerprint() != "" && (s.get_master_fingerprint() == signer_id.toStdString() || s.get_master_fingerprint() == xfp.toStdString()))){
            signer = s;
            break;
        }
    }
    QSingleSignerPtr signerPtr = QSingleSignerPtr(new QSingleSigner(signer));
    if(signerPtr && bridge::nunchukHasSinger(signer)){
        QWarningMessage msgWarning;
        matrixbrigde::JoinWallet(id(), signerPtr, msgWarning);
        if((int)EWARNING::WarningType::NONE_MSG == msgWarning.type()){
            ret = true;
        }
        else{
            AppModel::instance()->showToast(msgWarning.code(), msgWarning.what(), (EWARNING::WarningType)msgWarning.type());
        }
    }
    return ret;
}

void QNunchukRoom::updateCancelWallet(const QString &init_event_id)
{
    if(conversation()){
        conversation()->updateCancelWallet(init_event_id);
    }
}

void QNunchukRoom::updateCancelTransaction(const Conversation cons)
{
    if(conversation()){
        conversation()->updateCancelTransaction(cons);
    }
}

bool QNunchukRoom::isDownloaded() const
{
    return m_downloaded;
}

bool QNunchukRoom::extractNunchukEvent(const QString &matrixType, const QString &init_event_id, const QJsonObject &json, Conversation &cons)
{
    QString msgtype = json["msgtype"].toString();
    DBG_INFO << "FIXME" << msgtype << json;
    if(qUtils::strCompare(matrixType, NUNCHUK_EVENT_WALLET)){
        QJsonObject body = json["body"].toObject();
        cons.matrixType = NUNCHUK_EVENT_WALLET;
        if(qUtils::strCompare(msgtype, NUNCHUK_MSG_WALLET_INIT)){
            cons.init_event_id = init_event_id;
            cons.init_event_json = json;
            bool isCanceled = roomWallet() ? !(qUtils::strCompare(cons.init_event_id, roomWallet()->get_init_event_id())) : true;
            cons.messageType = isCanceled  ? (int)ENUNCHUCK::ROOM_EVT::WALLET_PAST : (int)ENUNCHUCK::ROOM_EVT::WALLET_INIT;
            cons.message = QString(QJsonDocument(body).toJson(QJsonDocument::Compact));
        }
        else if(qUtils::strCompare(msgtype, NUNCHUK_MSG_WALLET_JOIN)){
            QString xfp = body["key"].toString().split('/')[0].remove('[');
            QJsonObject init_event = body["io.nunchuk.relates_to"].toObject()["init_event"].toObject();
            QString wallet_name = init_event["content"].toObject()["body"].toObject()["name"].toString();
            QString init_event_id = init_event["event_id"].toString();
            cons.message = STR_CPP_011.arg(xfp).arg(wallet_name);
            cons.init_event_id = init_event_id;
            cons.messageType = (int)ENUNCHUCK::ROOM_EVT::WALLET_JOIN;
        }
        else if(qUtils::strCompare(msgtype, NUNCHUK_MSG_WALLET_LEAVE)){
            QJsonObject io_nunchuk_relates_to = body["io.nunchuk.relates_to"].toObject();
            QString join_event_id = io_nunchuk_relates_to["join_event_id"].toString();
            QJsonObject init_event = io_nunchuk_relates_to["init_event"].toObject();
            QString wallet_name = init_event["content"].toObject()["body"].toObject()["name"].toString();
            QString init_event_id = init_event["event_id"].toString();
            QString xfp = "";
            QWarningMessage joinmsg;
            QNunchukMatrixEvent nunJoinEvent = matrixbrigde::GetEvent(id(), join_event_id, joinmsg);
            if((int)EWARNING::WarningType::NONE_MSG == joinmsg.type()){
                QJsonObject joinjson = matrixbrigde::stringToJson(nunJoinEvent.get_content());
                xfp = joinjson["body"].toObject()["key"].toString().split('/')[0].remove('[');
            }
            cons.message = STR_CPP_012.arg(xfp).arg(wallet_name);
            cons.init_event_id = init_event_id;
            cons.messageType = (int)ENUNCHUCK::ROOM_EVT::WALLET_LEAVE;
        }
        else if(qUtils::strCompare(msgtype, NUNCHUK_MSG_WALLET_READY)){
            QJsonObject init_event = body["io.nunchuk.relates_to"].toObject()["init_event"].toObject();
            QString wallet_name = init_event["content"].toObject()["body"].toObject()["name"].toString();
            QString init_event_id = init_event["event_id"].toString();
            cons.message = STR_CPP_013.arg(wallet_name);
            cons.init_event_id = init_event_id;
            cons.messageType = (int)ENUNCHUCK::ROOM_EVT::WALLET_READY;
        }
        else if(qUtils::strCompare(msgtype, NUNCHUK_MSG_WALLET_CREATE)){
            QJsonObject init_event = body["io.nunchuk.relates_to"].toObject()["init_event"].toObject();
            QString wallet_name = init_event["content"].toObject()["body"].toObject()["name"].toString();
            QString init_event_id = init_event["event_id"].toString();
            cons.message = STR_CPP_014.arg(wallet_name);
            cons.init_event_id = init_event_id;
            cons.messageType = (int)ENUNCHUCK::ROOM_EVT::WALLET_CREATE;
        }
        else if(qUtils::strCompare(msgtype, NUNCHUK_MSG_WALLET_CANCEL)){
            QJsonObject init_event = body["io.nunchuk.relates_to"].toObject()["init_event"].toObject();
            QString wallet_name = init_event["content"].toObject()["body"].toObject()["name"].toString();
            QString init_event_id = init_event["event_id"].toString();
            cons.message = STR_CPP_015.arg(wallet_name);
            cons.init_event_id = init_event_id;
            cons.messageType = (int)ENUNCHUCK::ROOM_EVT::WALLET_CANCEL;
        }
        else {
            return false;
        }
        if(roomWallet()){
            bool isCreator = conversation()->isWalletCreator(roomWallet()->get_init_event_id());
            roomWallet()->setIsCreator(isCreator);
        }
    }
    else if(qUtils::strCompare(matrixType, NUNCHUK_EVENT_TRANSACTION)){
        DBG_INFO << "FIXME TRANSACTON" << msgtype << json;

        QJsonObject body = json["body"].toObject();
        cons.matrixType = NUNCHUK_EVENT_TRANSACTION;
        if(qUtils::strCompare(msgtype, NUNCHUK_MSG_TX_INIT)){
            cons.init_event_id = init_event_id;
            cons.message = QString(QJsonDocument(body).toJson(QJsonDocument::Compact));
            cons.messageType = (int)ENUNCHUCK::ROOM_EVT::TX_INIT;
        }
        else if(qUtils::strCompare(msgtype, NUNCHUK_MSG_TX_SIGN)){
            QString xfp = body["master_fingerprint"].toString();
            QJsonObject init_event = body["io.nunchuk.relates_to"].toObject()["init_event"].toObject();
            cons.message = STR_CPP_016.arg(xfp);
            cons.messageType = (int)ENUNCHUCK::ROOM_EVT::TX_SIGN;
            cons.init_event_id = init_event["event_id"].toString();
            cons.init_event_json = json;
        }
        else if(qUtils::strCompare(msgtype, NUNCHUK_MSG_TX_BROADCAST)){
            QJsonObject init_event = body["io.nunchuk.relates_to"].toObject()["init_event"].toObject();
            cons.message = STR_CPP_017;
            cons.messageType = (int)ENUNCHUCK::ROOM_EVT::TX_BROADCAST;
            cons.init_event_id = init_event["event_id"].toString();
            cons.init_event_json = json;
        }
        else if(qUtils::strCompare(msgtype, NUNCHUK_MSG_TX_READY)){
            QJsonObject init_event = body["io.nunchuk.relates_to"].toObject()["init_event"].toObject();
            cons.message = STR_CPP_018;
            cons.messageType = (int)ENUNCHUCK::ROOM_EVT::TX_READY;
            cons.init_event_id = init_event["event_id"].toString();
            cons.init_event_json = json;
        }
        else if(qUtils::strCompare(msgtype, NUNCHUK_MSG_TX_CANCEL)){
            QJsonObject init_event = body["io.nunchuk.relates_to"].toObject()["init_event"].toObject();
            cons.message = STR_CPP_019;
            cons.messageType = (int)ENUNCHUCK::ROOM_EVT::TX_CANCEL;
            cons.init_event_id = init_event["event_id"].toString();
            cons.init_event_json = json;
        }
        else if(qUtils::strCompare(msgtype, NUNCHUK_MSG_TX_RECEIVE)){
            if(matrixbrigde::HasRoomWallet(id()) == false) return false;
            QWarningMessage roomTxWarning;
            QString tx_id = matrixbrigde::GetTransactionId(id(), init_event_id, roomTxWarning);
            if((int)EWARNING::WarningType::NONE_MSG == roomTxWarning.type() && tx_id != ""){
                cons.init_event_id = tx_id;
                cons.message = STR_CPP_020;
                cons.messageType = (int)ENUNCHUCK::ROOM_EVT::TX_RECEIVE;
                cons.init_event_json = json;
            }
        }
        else {
            return false;
        }
        downloadTransactionThread(cons, id());
    }
    else if(qUtils::strCompare(matrixType, NUNCHUK_EVENT_EXCEPTION)){
        QJsonObject body = json["body"].toObject();
        cons.matrixType = NUNCHUK_EVENT_EXCEPTION;
        if(qUtils::strCompare(msgtype, NUNCHUK_EVENT_EXCEPTION)){
            cons.init_event_id = init_event_id;
            cons.init_event_json = json;
            cons.messageType = (int)ENUNCHUCK::ROOM_EVT::EXCEPTION;
            cons.message = QString(QJsonDocument(body).toJson(QJsonDocument::Compact));
        }
    }
    else {
        return false;
    }
    return true;
}

nunchuk::Wallet QNunchukRoom::walletImport() const
{
    return m_walletImport;
}

void QNunchukRoom::setWalletImport(const nunchuk::Wallet &walletImport)
{
    m_walletImport = walletImport;
}

int QNunchukRoom::roomType()
{
    if(isServerNoticeRoom() ) {
        return (int)ENUNCHUCK::RoomType::SERVICE_ROOM;
    }
    else if(isNunchukSyncRoom()){
        return (int)ENUNCHUCK::RoomType::NUNSYNC_ROOM;
    }
    else if(isSupportRoom()){
        return (int)ENUNCHUCK::RoomType::SUPPORT_ROOM;
    }
    else{
        return (int)ENUNCHUCK::RoomType::GRPCHAT_ROOM;
    }
}

void QNunchukRoom::synchonizesUserData()
{
    for (auto e = m_room->messageEvents().rbegin(); e != m_room->messageEvents().rend(); ++e){
        nunchukConsumeSyncEvent(**e);
    }
}

bool QNunchukRoom::isValidMessageTime(const Conversation cons)
{
    if(messageMaxLifeTime() <= 0){
        return true;
    }
    else{
        QDateTime message_time_utc = QDateTime::fromTime_t(cons.timestamp);
        QDateTime time_now_utc = QDateTime::currentDateTimeUtc(); // FIXME get from server ? worst case is local time incorrect
        // message age
        qint64 message_age = time_now_utc.toMSecsSinceEpoch() - message_time_utc.toMSecsSinceEpoch();
        DBG_INFO << "Retention" << roomName() << cons.message << cons.messageType
                 << message_time_utc.toString("yyyy-MM-dd hh:mm:ss")
                 << time_now_utc.toString("yyyy-MM-dd hh:mm:ss")
                 << message_age
                 << messageMaxLifeTime();
        // compare with max life time
        return message_age <= messageMaxLifeTime();
    }
}

qint64 QNunchukRoom::messageMaxLifeTime() const
{
    return m_maxLifeTime;
}

void QNunchukRoom::setMessageMaxLifeTime(qint64 value)
{
    DBG_INFO << "Retention" << roomName() << value;
    m_maxLifeTime = value;
    if(conversation()){
        conversation()->setMaxLifeTime(m_maxLifeTime);
    }
}

void QNunchukRoom::startCountdown()
{
    if(conversation() && isByzantineRoom()){
        conversation()->startCountdown();
    }
}

void QNunchukRoom::stopCountdown()
{
    if(conversation()){
        conversation()->stopCountdown();
    }
}

void QNunchukRoom::activateRetention(qint64 max_lifetime)
{
    if(m_room){
        QJsonObject content;
        content["max_lifetime"] = max_lifetime;
        m_room->setState(NUNCHUK_ROOM_RETENTION, "", content);
    }
}

void QNunchukRoom::downloadTransactionThread(Conversation cons, const QString &roomid)
{
    if(roomWallet() && roomWallet()->get_wallet_id() != ""){
        if(cons.init_event_id != ""){ // FIXME FOR CHECK DUP RECIEVED TX EVT
            QtConcurrent::run([this, cons, roomid]() {
                if(cons.messageType == (int)ENUNCHUCK::ROOM_EVT::TX_RECEIVE){
                    QString wallet_id = roomWallet() ? roomWallet()->get_wallet_id() : "";
                    QString tx_id = cons.init_event_id;
                    QWarningMessage txWarning;
                    nunchuk::Transaction tx = bridge::nunchukGetOriginTransaction(wallet_id,
                                                                                  tx_id,
                                                                                  txWarning);
                    nunchuk::RoomTransaction room_tx;
                    room_tx.set_wallet_id(wallet_id.toStdString());
                    room_tx.set_tx_id(tx_id.toStdString());
                    room_tx.set_room_id(id().toStdString());
                    room_tx.set_tx(tx);
                    room_tx.set_init_event_id(tx_id.toStdString());
                    emit signalFinishedDownloadTransaction(room_tx, tx, cons);
                }
                else{

                    DBG_INFO << "FIXME TRANSACTON: init_event:>" << cons.init_event_id;
                    QWarningMessage roomTxWarning;
                    nunchuk::RoomTransaction room_tx = matrixbrigde::GetOriginRoomTransaction(roomid,
                                                                                              cons.init_event_id,
                                                                                              roomTxWarning);
#if 0 //FIXME ==> HANDLE CANCEL
                    if(cons.messageType == (int)ENUNCHUCK::ROOM_EVT::TX_CANCEL){
                        QWarningMessage msggetevt;
                        QNunchukMatrixEvent evt = matrixbrigde::GetEvent(roomid, cons.init_event_id, msggetevt);
                        DBG_INFO << "FIXME"
                                 << evt.get_content();
                    }
#endif
                    if((int)EWARNING::WarningType::NONE_MSG == roomTxWarning.type() && room_tx.get_wallet_id() != ""){

                        DBG_INFO << "FIXME TRANSACTON: get_wallet_id:>" << room_tx.get_wallet_id() << "get_tx_id:" << room_tx.get_tx_id();
                        QWarningMessage txWarning;
                        room_tx.set_room_id(id().toStdString());
                        nunchuk::Transaction tx = bridge::nunchukGetOriginTransaction(QString::fromStdString(room_tx.get_wallet_id()),
                                                                                      QString::fromStdString(room_tx.get_tx_id()),
                                                                                      txWarning);
                        if((int)EWARNING::WarningType::NONE_MSG == txWarning.type() && tx.get_txid() != ""){
                            emit signalFinishedDownloadTransaction(room_tx, tx, cons);
                        }
                    }
                }
            });
        }
        AppModel::instance()->requestSyncWalletDb(roomWallet()->get_wallet_id());
    }
}

bool QNunchukRoom::leaveWallet(const QString &xfp)
{
    bool ret = false;
    if(roomWallet() && roomWallet()->walletSigners()){
        QString join_id = roomWallet()->walletSigners()->getJoinEventId(xfp);
        if("" != join_id){
            QWarningMessage msgWarning;
            matrixbrigde::LeaveWallet(id(), join_id, STR_CPP_008, msgWarning);
            if((int)EWARNING::WarningType::NONE_MSG == msgWarning.type()){
                ret = true;
            }
            else{
                AppModel::instance()->showToast(msgWarning.code(), msgWarning.what(), (EWARNING::WarningType)msgWarning.type());
            }
        }
    }
    return ret;
}

bool QNunchukRoom::getXpub(const QString &id)
{
    QWarningMessage warningmsg;
    bridge::nunchukCacheMasterSignerXPub(id, warningmsg);
    if((int)EWARNING::WarningType::NONE_MSG == warningmsg.type()){
        AppModel::instance()->masterSignerList()->updateMasterSignerNeedXpubById(id, false);
        return true;
    }
    else{
        AppModel::instance()->masterSignerList()->updateMasterSignerNeedXpubById(id, true);
        AppModel::instance()->showToast(warningmsg.code(), warningmsg.what(), (EWARNING::WarningType)warningmsg.type());
        return false;
    }
}

void QNunchukRoom::finalizeWallet()
{
    emit signalStartFinalizeWallet();
    QtConcurrent::run([this]() {
        QWarningMessage msgwarning;
        matrixbrigde::CreateWallet(id(), msgwarning);
        emit signalFinishFinalizeWallet(msgwarning.what(), (int)msgwarning.type(), msgwarning.code());
    });
}

void QNunchukRoom::cancelWallet()
{
    emit signalStartCancelWallet();
    QtConcurrent::run([this]() {
        QWarningMessage msgwarning;
        matrixbrigde::CancelWallet(id(), STR_CPP_010, msgwarning);
        emit signalFinishCancelWallet(msgwarning.what(), (int)msgwarning.type(), msgwarning.code());
    });
}

void QNunchukRoom::downloadFile(const QString &eventId, const QUrl &localFilename)
{
    if(m_room){
        DBG_INFO << eventId;
        m_room->downloadFile(eventId, localFilename);
    }
}

void QNunchukRoom::setTags(const QString &newtag)
{
    if(m_room){
        m_room->addTag(newtag);
    }
}

void QNunchukRoom::joinAndLeaveWallet()
{
    if(roomWallet() && roomWallet()->walletSigners()){
        QStringList origin_xpfs = roomWallet()->walletSigners()->xfplist();
        QStringList join_master_ids;
        QStringList join_remote_xfps;
        QStringList leave_xfps;
        join_master_ids.clear();
        join_remote_xfps.clear();
        leave_xfps.clear();
        if(AppModel::instance()->masterSignerList()){
            for (QMasterSignerPtr master : AppModel::instance()->masterSignerList()->fullList()) {
                QString xfp = master.data()->fingerPrint().toUpper();
                if(master.data()->checked()){
                    if(origin_xpfs.contains(xfp)){ continue; }
                    else{
                        join_master_ids.append(master.data()->id());
                    }
                }
                else{
                    if(origin_xpfs.contains(xfp)){
                        leave_xfps.append(xfp);
                    }
                    else{ continue; }
                }
            }
        }
        if(AppModel::instance()->remoteSignerList()){
            for (QSingleSignerPtr remote : AppModel::instance()->remoteSignerList()->fullList()) {
                QString xfp = remote.data()->masterFingerPrint().toUpper();
                if(remote.data()->checked()){
                    if(origin_xpfs.contains(xfp)){ continue; }
                    else{
                        join_remote_xfps.append(xfp);
                    }
                }
                else{
                    if(origin_xpfs.contains(xfp)){
                        leave_xfps.append(xfp);
                    }
                    else{ continue; }
                }
            }
        }
        bool anyFailure = false;
        // LEAVE
        DBG_INFO << "LEAVE :" << leave_xfps;
        for (QString leave_xfp : leave_xfps) {
            bool ret = leaveWallet(leave_xfp);
            if(!ret){
                anyFailure = true;
                if(AppModel::instance()->masterSignerList()){
                    AppModel::instance()->masterSignerList()->setUserCheckedByFingerprint(true, leave_xfp);
                }
                if(AppModel::instance()->remoteSignerList()){
                    AppModel::instance()->remoteSignerList()->setUserCheckedByFingerprint(true, leave_xfp);
                }
            }
        }
        DBG_INFO << "JOIN MASTER :" << join_master_ids;
        AppModel::instance()->masterSignerList()->resetMasterSignerNeedXpub();
        for (QString join_master_id : join_master_ids) {
            bool needXpub = false;
            bool ret = false;
            if(m_walletImport.get_signers().size() > 0){
                ret = joinWalletUseSignerFromWalletImport(join_master_id,"");
            }else{
                ret = joinWalletWithMasterSigner(join_master_id, needXpub);
            }
            if(!ret && AppModel::instance()->masterSignerList()){
                anyFailure = true;
                AppModel::instance()->masterSignerList()->setUserCheckedById(false, join_master_id);
                if(needXpub){
                    AppModel::instance()->masterSignerList()->updateMasterSignerNeedXpubById(join_master_id, true);
                }
            }
        }
        DBG_INFO << "JOIN REMOTE :" << join_remote_xfps;
        for (QString join_remote_xfp : join_remote_xfps) {
            bool ret = false;
            if(m_walletImport.get_signers().size() > 0){
                ret = joinWalletUseSignerFromWalletImport("",join_remote_xfp);
            }else{
                ret = joinWalletWithSingleSigner(join_remote_xfp);
            }
            if(!ret && AppModel::instance()->remoteSignerList()){
                anyFailure = true;
                AppModel::instance()->remoteSignerList()->setUserCheckedByFingerprint(false, join_remote_xfp);
            }
        }
        if(!anyFailure){
            QEventProcessor::instance()->sendEvent(E::EVT_ONLINE_ONS_CLOSE_REQUEST, E::STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET);
        }
    }
    else{
        QEventProcessor::instance()->sendEvent(E::EVT_ONLINE_ONS_CLOSE_REQUEST, E::STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET);
    }
}

void QNunchukRoom::setDisplayed(bool displayed)
{
    if(m_room){
        m_room->setDisplayed(displayed);
    }
}

void QNunchukRoom::markAllMessagesAsRead(){
    if(m_room){
        m_room->markAllMessagesAsRead();
    }
}

void QNunchukRoom::markFiveMessagesAsRead()
{
    if(m_room && m_conversation)
    {
        int max = 10;
        int index = 0;
        for(int i = m_conversation->count() - m_room->unreadCount(); i < m_conversation->count(); i++){
            if(m_room->isValidIndex(i)){
                index = i;
                max --;
                if(max == 0) break;
            }
        }
        if(m_room->isValidIndex(index)){
            m_room->setLastDisplayedEvent(index);
            m_room->markMessagesAsRead(m_room->lastDisplayedEventId());
        }
        if(m_room->unreadCount() < 10){
            m_room->markAllMessagesAsRead();
        }
    }

}

void QNunchukRoom::downloadHistorical()
{
    DBG_INFO << "Room[" << roomName() << "], Tags[" << m_room->tagNames() << "]";
    if(!m_room) return;
    else{
        if(isServerNoticeRoom()){
            //FIXME - DEBUG
//            for (auto e = m_room->messageEvents().rbegin(); e != m_room->messageEvents().rend(); ++e){
//                nunchukNoticeEvent(**e); // FIXME
//            }
        }
        else if(isNunchukSyncRoom()){
            QtConcurrent::run([this]() {
                if(CLIENT_INSTANCE->isNunchukLoggedIn() && CLIENT_INSTANCE->isMatrixLoggedIn()){
                    matrixbrigde::RegisterAutoBackup(id(), CLIENT_INSTANCE->accessToken());
                }
            });
        }
        else{
            setRoomWallet(matrixbrigde::ReloadRoomWallet(this));
            QtConcurrent::run([=,this]() {
                if(conversation()){
                    conversation()->clear();
                    for (auto e = m_room->messageEvents().begin(); e != m_room->messageEvents().end(); ++e){
                        nunchukConsumeEvent(**e);
                    }
                    if(!roomWallet()) {
                        Conversation init;
                        init.timestamp = -100;
                        init.messageType = (int)ENUNCHUCK::ROOM_EVT::INITIALIZE;
                        conversation()->addHistoryMessage(init);
                    }
                    for (auto it = m_room->messageEvents().rbegin(); it != m_room->messageEvents().rend(); ++it){
                        Conversation cons = createConversation(**it);
                        if(cons.messageType != (int)ENUNCHUCK::ROOM_EVT::INVALID){
                            conversation()->addHistoryMessage(cons);
                        }
                    }
                    conversation()->requestSortByTimeAscending(false);
                    setLastMessage(conversation()->lastMessage());
                    setLasttimestamp(conversation()->lastTime());
                    if(roomWallet()){
                        AppModel::instance()->requestSyncWalletDb(roomWallet()->get_wallet_id());
                        bool isCreator = conversation()->isWalletCreator(roomWallet()->get_init_event_id());
                        roomWallet()->setIsCreator(isCreator);
                    }
                }
                startGetPendingTxs();
                m_downloaded = true;
            });
        }
    }
}

void QNunchukRoom::connectRoomSignals()
{
    if(m_room){
        m_downloaded = false;
        connect(m_room, &Room::notificationCountChanged, this, &QNunchukRoom::notificationCountChanged);
        connect(m_room, &Room::highlightCountChanged, this, &QNunchukRoom::highlightCountChanged);
        connect(m_room, &Room::namesChanged, this, &QNunchukRoom::roomNameChanged);
        connect(m_room, &Room::pendingEventAboutToMerge, this, &QNunchukRoom::pendingEventAboutToMerge);
        connect(m_room, &Room::pendingEventChanged, this, &QNunchukRoom::pendingEventChanged);
        connect(m_room, &Room::messageSent, this, &QNunchukRoom::messageSent);
        connect(m_room, &Room::aboutToAddNewMessages, this, &QNunchukRoom::aboutToAddNewMessages);
        connect(m_room, &Room::addedMessages, this, &QNunchukRoom::addedMessages);
        connect(m_room, &Room::addedMessages, this, &QNunchukRoom::allHisLoadedChanged);
        connect(m_room, &Room::aboutToAddHistoricalMessages, this, &QNunchukRoom::aboutToAddHistoricalMessages);
        connect(m_room, &Room::unreadMessagesChanged, this, &QNunchukRoom::unreadMessagesChanged);
        connect(m_room, &Room::userAdded, this, &QNunchukRoom::usersChanged);
        connect(m_room, &Room::userAdded, this, &QNunchukRoom::roomNameChanged);
        connect(m_room, &Room::userAdded, this, &QNunchukRoom::userCountChanged);
        connect(m_room, &Room::userRemoved, this, &QNunchukRoom::usersChanged);
        connect(m_room, &Room::userRemoved, this, &QNunchukRoom::roomNameChanged);
        connect(m_room, &Room::userRemoved, this, &QNunchukRoom::userCountChanged);
        connect(m_room, &Room::typingChanged, this, &QNunchukRoom::typingChanged);
        connect(this, &QNunchukRoom::signalFinishedDownloadTransaction, this, &QNunchukRoom::slotFinishedDownloadTransaction);
        connect(this, &QNunchukRoom::signalFinishFinalizeWallet, this, &QNunchukRoom::slotFinishFinalizeWallet);
        connect(this, &QNunchukRoom::signalFinishCancelWallet, this, &QNunchukRoom::slotFinishCancelWallet);
        connect(this, &QNunchukRoom::signalFinishedGetPendingTxs, this, &QNunchukRoom::slotFinishedGetPendingTxs);
        downloadHistorical();
    }
}

void QNunchukRoom::connectRoomServiceSignals()
{
    if(m_room){
        connect(this,   &QNunchukRoom::noticeService,   ClientController::instance(),   &ClientController::refreshContacts);
        connect(m_room, &Room::addedMessages,           ClientController::instance(),   &ClientController::refreshContacts);
        connect(m_room, &Room::addedMessages,           this,                           &QNunchukRoom::addedMessages);
    }
}

bool QNunchukRoom::checkIsLocalUser(const QString userID)
{
    QString localUserId = room()->localUser()->id();
    localUserId.remove("@");
    localUserId.remove("nunchuk_io_");
    localUserId.remove(":nunchuk.io");
    QString senderUserId = userID;
    senderUserId.remove("@");
    senderUserId.remove("nunchuk_io_");
    senderUserId.remove(":nunchuk.io");
    return (qUtils::strCompare(senderUserId, localUserId));
}

DracoUser QNunchukRoom::getNunchukMemberUser(const QString &input)
{
    QString id_input = input;
    id_input.remove("@");
    id_input.remove("nunchuk_io_");
    id_input.remove(":nunchuk.io");
    for(DracoUser user : m_nunchukMembers){
        QString my_id = user.id;
        my_id.remove("@");
        my_id.remove("nunchuk_io_");
        my_id.remove(":nunchuk.io");
        if(qUtils::strCompare(my_id, id_input)){
            return user;
        }
    }
    DracoUser tmp;
    tmp.id = input;
    tmp.name = input;
    tmp.email = input;
    tmp.gender = input;
    tmp.avatar = input;
    tmp.status = input;
    tmp.status_text = input;
    tmp.chat_id = input;
    tmp.username = input;
    tmp.login_type = input;
    return tmp;
}

void QNunchukRoom::setNunchukMembers(const QList<DracoUser> &nunchukMembers)
{
    m_nunchukMembers = nunchukMembers;
}

QRoomTransactionModel *QNunchukRoom::pendingTxs() const
{
    return m_pendingTxs.data();
}

QRoomTransactionModelPtr QNunchukRoom::pendingTxsPtr() const
{
    return m_pendingTxs;
}

void QNunchukRoom::setPendingTxs(const QRoomTransactionModelPtr &pendingTxs)
{
    m_pendingTxs = pendingTxs;
    emit pendingTxsChanged();
}

QRoomTransactionModel* QNunchukRoom::txs() const
{
    return m_txs.data();
}

QRoomTransactionModelPtr QNunchukRoom::txsPtr() const
{
    return m_txs;
}

void QNunchukRoom::setTxs(const QRoomTransactionModelPtr &txs)
{
    m_txs = txs;
}

void QNunchukRoom::transactionChanged(const QString &tx_id, const int status, const int height)
{
    if(conversation()){
        conversation()->transactionChanged(tx_id, status, height);
    }
    startGetPendingTxs();
}

void QNunchukRoom::updateTransactionMemo(const QString &tx_id, const QString &memo)
{
    if(conversation()){
        conversation()->updateTransactionMemo(tx_id, memo);
    }
}

void QNunchukRoom::startGetPendingTxs()
{
    if(matrixbrigde::HasRoomWallet(id())){
        QtConcurrent::run([this]() {
            QRoomTransactionModelPtr ret = matrixbrigde::GetPendingTransactions(id());
            emit signalFinishedGetPendingTxs(ret);
        });
    }
}

QRoomWallet *QNunchukRoom::roomWallet() const
{
    return m_roomWallet.data();
}

QRoomWalletPtr QNunchukRoom::roomWalletPtr() const
{
    return m_roomWallet;
}

void QNunchukRoom::setRoomWallet(const QRoomWalletPtr &roomWallet)
{
    if(m_roomWallet != roomWallet){
        m_roomWallet = roomWallet;
        emit roomWalletChanged();
    }
}

QString QNunchukRoom::lastMessage() const
{
    return m_lastMessage;
}

void QNunchukRoom::setLastMessage(const Conversation &cons)
{
    QString lastmsg;
    QString picname = cons.sendByMe ? "You" : cons.sender;
    if(cons.messageType == (int)ENUNCHUCK::ROOM_EVT::PLAIN_TEXT){
        if(qUtils::strCompare(cons.matrixType, NUNCHUK_ROOM_ENCRYPTED)){
            m_IsEncrypted = true;
        }
        else{
            m_IsEncrypted = false;
        }
    }
    lastmsg = QString("<b>%1</b>: %2").arg(picname).arg(QTextDocumentFragment::fromHtml(cons.message).toPlainText().isEmpty() ? "sent an attachment": cons.message );
    if(m_lastMessage != lastmsg){
        m_lastMessage = lastmsg;
        emit lastMessageChanged();
    }
}

QString QNunchukRoom::lasttimestamp() const
{
    QDateTime today = QDateTime::currentDateTime();
    QDateTime day = QDateTime::fromTime_t(m_lasttimestamp);
    if(today.date().year() == day.date().year()){
        qint64 numberDay = day.daysTo(today);
        if(numberDay == 0){
            return day.toString("hh:mm AP");
        }
        else if(numberDay <= 7 && numberDay > 0){
            if(today.date().dayOfWeek() > day.date().dayOfWeek()){
                return day.toString("dddd");
            }else{
                return day.toString("MMMM dd");
            }
        }
        else{
            return day.toString("MMMM dd");
        }
    }
    else{
        return day.toString("MMMM dd, yyyy");
    }
}

time_t QNunchukRoom::lasttimestamp_timet() const
{
    return m_lasttimestamp;
}

void QNunchukRoom::setLasttimestamp(const Conversation &cons)
{
    time_t lasttimestamp = cons.timestamp;
    if(QDateTime::fromTime_t(m_lasttimestamp) < QDateTime::fromTime_t(lasttimestamp)){
        m_lasttimestamp = lasttimestamp;
        emit lasttimestampChanged();
    }
}

QNunchukRoomUsers *QNunchukRoom::users() const
{
    return m_users.data();
}

QNunchukRoomUsersPtr QNunchukRoom::usersPtr() const
{
    return m_users;
}

bool QNunchukRoom::HasContact(const QString &id)
{
    return users()->HasContact(id) && roomWallet();
}

void QNunchukRoom::setUsers(const QNunchukRoomUsersPtr &data)
{
    m_users = data;
    emit usersChanged();
}

bool QNunchukRoom::validatePendingEvent(const QString &txnId)
{
    if(m_room){
        auto it = m_room->findPendingEvent(txnId);
        return it != m_room->pendingEvents().end()
                && it->deliveryStatus() == EventStatus::Submitted
                && (*it)->transactionId() == txnId;
    }
    else{
        return false;
    }
}

void QNunchukRoom::highlightCountChanged()
{
    if (m_room && m_room->displayed() && !m_room->hasUnreadMessages()) {
        m_room->resetNotificationCount();
        m_room->resetHighlightCount();
    }
}

void QNunchukRoom::notificationCountChanged()
{
    if(m_room && m_room->displayed() && !m_room->hasUnreadMessages()) {
        m_room->resetNotificationCount();
        m_room->resetHighlightCount();
    }
}

void QNunchukRoom::pendingEventAboutToMerge(RoomEvent *serverEvent, int pendingEventIndex)
{
    DBG_INFO;
}

void QNunchukRoom::pendingEventChanged(int pendingEventIndex)
{
//    DBG_INFO << pendingEventIndex;
}

void QNunchukRoom::messageSent(QString txnId, QString eventId)
{
    if(conversation()){
        conversation()->messageSent(txnId, eventId);
    }
}

void QNunchukRoom::aboutToAddNewMessages(RoomEventsRange events)
{
//    DBG_INFO << " new event(s) in room " << m_room->name() << events.size() << m_room->pendingEvents().size();
}

void QNunchukRoom::addedMessages(int fromIndex, int toIndex)
{
    DBG_INFO << fromIndex << toIndex << isServerNoticeRoom();
    if(isServerNoticeRoom()){
        emit noticeService();
        QtConcurrent::run([=, this]() {
            for (auto e = m_room->messageEvents().rbegin(); e != m_room->messageEvents().rend(); ++e){
                QThreadForwarder::instance()->forwardInQueuedConnection([fromIndex, toIndex, this, e = std::move(e)](){
                    if(fromIndex <= e->index() &&  toIndex >= e->index()){
                        const RoomEvent* lastEvent = e->get();
                        nunchukNoticeEvent(*lastEvent);
                    }
                });
            }
        });
    }
    else if(isNunchukSyncRoom()){
        QtConcurrent::run([=, this]() {
            for (auto e = m_room->messageEvents().rbegin(); e != m_room->messageEvents().rend(); ++e){
                QThreadForwarder::instance()->forwardInQueuedConnection([fromIndex, toIndex, this, e = std::move(e)](){
                    if(fromIndex <= e->index() &&  toIndex >= e->index()){
                        const RoomEvent* lastEvent = e->get();
                        nunchukConsumeSyncEvent(*lastEvent);
                    }
                });
            }
        });
    }
    else{
        receiveMessage(fromIndex, toIndex);
    }
}

void QNunchukRoom::aboutToAddHistoricalMessages(RoomEventsRange events)
{
}

void QNunchukRoom::unreadMessagesChanged(Room *room)
{
    emit unreadCountChanged();
}

void QNunchukRoom::typingChanged()
{
    QStringList usersTypingName;
    usersTypingName.clear();
    if (!m_room || m_room->usersTyping().isEmpty())  {
        setTypingNames("");
        return;
    }
    const auto& usersTyping = m_room->usersTyping();
    int MaxNamesToShow = 3;
    int SampleSizeForHud = 2;
    usersTypingName.reserve(MaxNamesToShow);
    const auto endIt = usersTyping.size() > MaxNamesToShow
            ? usersTyping.cbegin() + SampleSizeForHud
            : usersTyping.cend();
    for (auto it = usersTyping.cbegin(); it != endIt; ++it)
        usersTypingName << m_room->safeMemberName((*it)->id());

    if (usersTyping.size() > MaxNamesToShow) {
        usersTypingName.push_back( tr("%L1 more").arg(usersTyping.size() - SampleSizeForHud));
    }

    if(usersTypingName.count() > 1){
        setTypingNames(QString("%1 are typing ...").arg(usersTypingName.join(", ")));
    }
    else if(usersTypingName.count() == 1){
        setTypingNames(QString("%1 is typing ...").arg(usersTypingName.at(0)));
    }
    else{
        setTypingNames("");
    }
}

void QNunchukRoom::roomWalletCreated(const QString &roomId)
{
    DBG_INFO << roomId;
}

bool QNunchukRoom::extractNunchukEvent(const RoomEvent &evt, Conversation &cons)
{
    return extractNunchukEvent(evt.matrixType(),evt.id(),evt.contentJson(),cons);
}

void QNunchukRoom::eventToConversation(const RoomEvent& evt, Conversation &result, Qt::TextFormat format)
{
    if(m_room){
        using namespace Quotient;
        const bool prettyPrint = (format == Qt::RichText);
        bool    hasFileContent = false;
        int     file_mimeType  = (int)ENUNCHUCK::ROOM_EVT::FILE_OTHER;

        const auto message = switchOnType( evt,
        [&](const RoomMessageEvent& e){
            using namespace MessageEventContent;
            if (e.hasFileContent()) {
                auto fileCaption = prettyPrint ? Quotient::prettyPrint(e.plainBody()) : e.plainBody();
                hasFileContent = true;
                auto filename = e.content()->fileInfo()->originalName.toHtmlEscaped();
                if(e.msgtype() == RoomMessageEvent::MsgType::Image){
                    file_mimeType  = (int)ENUNCHUCK::ROOM_EVT::FILE_IMAGE;
                    fileCaption = fileCaption.remove(filename);
                }
                else if(e.msgtype() == RoomMessageEvent::MsgType::Video){
                    file_mimeType  = (int)ENUNCHUCK::ROOM_EVT::FILE_VIDEO;
                    fileCaption = fileCaption.remove(filename);
                }
                else{
                    file_mimeType  = (int)ENUNCHUCK::ROOM_EVT::FILE_OTHER;
                }
                return fileCaption;
            }
            else{
                QString plainBody;
                // 1. prettyPrint/HTML
                if (prettyPrint && e.mimeType().name() != "text/plain") {
                    auto htmlBody = static_cast<const TextContent*>(e.content())->body;
                    htmlBody.replace(utils::userPillRegExp, "<b>\\1</b>");
                    htmlBody.replace(utils::strikethroughRegExp, "<s>\\1</s>");
                    return htmlBody;
                }
                // 2. prettyPrint/text 3. plainText/HTML 4. plainText/text
                if (e.content() && e.mimeType().name() == "text/plain") {  // 2/4
                    plainBody = static_cast<const TextContent*>(e.content())->body;
                }
                else {
                    plainBody = e.plainBody();
                }
                if (prettyPrint) {
                    return Quotient::prettyPrint(plainBody);
                }
                return plainBody;
            }
        },
        [=](const RoomMemberEvent& e) {
            // FIXME: Rewind to the name that was at the time of this event
            auto subjectName = m_room->user(e.userId())->displayname(room());
            QString content = "";
            // The below code assumes senderName output in AuthorRole
            switch (e.membership()) {
            case Membership::Invite:
            {
                if (e.repeatsState()){
                    content = STR_CPP_021.arg(subjectName);
                    return content;
                }
            }
            case Membership::Join:
            {
                if (e.repeatsState()){
                    content = STR_CPP_022;
                    return content;
                }
                if (!e.prevContent() || e.membership() != e.prevContent()->membership) {
                    content = e.membership() == Membership::Invite ? STR_CPP_023.arg(subjectName) : STR_CPP_024;
                    return content;
                }
                QString text{};
                if (e.isRename()) {
                    if (e.displayName().isEmpty())
                        text = STR_CPP_025;
                    else
                        text = STR_CPP_026.arg(e.displayName().toHtmlEscaped());
                }
                if (e.isAvatarUpdate()) {
                    if (!text.isEmpty())
                        text += STR_CPP_027;
                    if (e.avatarUrl().isEmpty())
                        text += STR_CPP_028;
                    else if (!e.prevContent()->avatarUrl)
                        text += STR_CPP_029;
                    else
                        text += STR_CPP_030;
                }
                content = text;
                return content;
            }
            case Membership::Leave:
            {
                if (e.prevContent() &&e.prevContent()->membership == Membership::Invite) {
                    content = (e.senderId() != e.userId()) ? STR_CPP_031.arg(subjectName)
                                                           : STR_CPP_032;
                    return content;
                }
                if (e.prevContent() && e.prevContent()->membership == Membership::Ban) {
                    content = (e.senderId() != e.userId()) ? STR_CPP_033.arg(subjectName)
                                                           : STR_CPP_034;
                    return content;
                }
                QString ret = (e.senderId() != e.userId()) ? STR_CPP_035.arg(subjectName, e.contentJson()["reason"_ls].toString().toHtmlEscaped()) : STR_CPP_036;
                content = ret;
                if(m_room && isSupportRoom()){
                    DBG_INFO << content;
                    int member_size = m_room->users().size();
                    if(member_size < 2){
                        emit roomNeedTobeLeaved(id());
                    }
                }
                return content;
            }
            case Membership::Ban:
            {
                QString ret = (e.senderId() != e.userId())
                        ? STR_CPP_037
                          .arg(subjectName, e.contentJson()["reason"_ls]
                          .toString()
                          .toHtmlEscaped())
                        : STR_CPP_038;
                content = ret;
                return content;
            }
            case Membership::Knock:
            {
                content = STR_CPP_039;
                return content;
            }
            default:;
            }
            content = STR_CPP_040;
            return content;
        },
        [=](const RoomCanonicalAliasEvent& e) {
            QString content = (e.alias().isEmpty()) ? STR_CPP_042 : STR_CPP_043.arg(e.alias());
            return content;
        },
        [=](const RoomNameEvent& e) {
            QString content = (e.name().isEmpty()) ? STR_CPP_044 : STR_CPP_045.arg(e.name().toHtmlEscaped());
            return content;
        },
        [=](const RoomTopicEvent& e) {
            QString content = (e.topic().isEmpty()) ? STR_CPP_046 :
                                                      STR_CPP_047.arg(prettyPrint ? Quotient::prettyPrint(e.topic())
                                                                                  : e.topic());
            return content;
        },
        [=](const RoomAvatarEvent& e) {
            QString content = STR_CPP_048;
            return content;
        },
        [=](const EncryptedEvent& e) {
            QString content = STR_CPP_099;
            return content;
        },
        [=](const EncryptionEvent& e) {
            QString content = STR_CPP_049;
            return content;
        },
        [=](const RoomCreateEvent& e) {
            QString content = (e.isUpgrade() ? STR_CPP_050 : STR_CPP_051).arg(e.version().isEmpty() ? "1" : e.version().toHtmlEscaped());
            return content;
        },
        [=] (const RoomTombstoneEvent& e) {
            QString content = STR_CPP_052.arg(e.serverMessage().toHtmlEscaped());
            return content;
        },
        [=](const StateEventBase& e) {
            // A small hack for state events from TWIM bot
            QString content = e.stateKey() == "twim" ? tr("updated the database", "TWIM bot updated the database")
                                                     : e.stateKey().isEmpty() ? tr("updated %1 state", "%1 - Matrix event type").arg(e.matrixType())
                                                                              : tr("updated %1 state for %2","%1 - Matrix event type, %2 - state key").arg(e.matrixType(), e.stateKey().toHtmlEscaped());
            return content;
        },
        tr("Unknown event")
        );

        result.message = message;
        if(evt.isStateEvent()){
            result.messageType = (int)ENUNCHUCK::ROOM_EVT::STATE_EVT;
        }
        else {
            if(hasFileContent){
                result.messageType = file_mimeType;
            }
            else{
                result.messageType = (int)ENUNCHUCK::ROOM_EVT::PLAIN_TEXT;
            }
        }
    }
}

void QNunchukRoom::receiveMessage(int fromIndex, int toIndex)
{
    if(conversation()){
        int pos = 0;
        bool nameOrAvatarChanged = false;
        for (auto rit = m_room->messageEvents().rbegin(); rit!= m_room->messageEvents().rend(); ++rit){
            if(fromIndex <= rit->index() &&  toIndex >= rit->index()){
                const RoomEvent* lastEvent = rit->get();
                //check null
                if(!lastEvent){ continue; }
                User* sender = m_room->user(lastEvent->senderId());
                QString nameDisplay = sender->displayname(room()) != "" ? sender->displayname(room()) : sender->id();
                QString avatar = sender->avatarMediaId(room());
                Conversation oldCons = conversation()->getConversation(lastEvent->senderId());
                nameOrAvatarChanged = oldCons.sender.localeAwareCompare(nameDisplay) != 0 || oldCons.avatar.localeAwareCompare(avatar) != 0;
                nunchukConsumeEvent(*lastEvent);
                if(lastEvent->matrixType() == NUNCHUK_EVENT_WALLET){
                    setRoomWallet(matrixbrigde::ReloadRoomWallet(this));
                }
                Conversation cons = createConversation(*lastEvent);
                if(cons.messageType != (int)ENUNCHUCK::ROOM_EVT::INVALID){
                    if(toIndex < m_room->maxTimelineIndex() ){
                        conversation()->insertMessage(pos, cons);
                    }
                    else{
                        conversation()->addMessage(cons);
                    }
                    pos++;
                    if(cons.messageType == (int)ENUNCHUCK::ROOM_EVT::WALLET_CANCEL){
                        updateCancelWallet(cons.init_event_id);
                    }
                    if(cons.messageType == (int)ENUNCHUCK::ROOM_EVT::TX_CANCEL){
                        updateCancelTransaction(cons);
                    }
                }
            }
        }
        if(nameOrAvatarChanged){
            if(users()){
                users()->refresh();
            }
            emit roomNameChanged();
        }
        conversation()->requestSortByTimeAscending();
        setLastMessage(conversation()->lastMessage());
        setLasttimestamp(conversation()->lastTime());
    }
}

Conversation QNunchukRoom::createConversation(const RoomEvent &evt)
{
    User* sender = m_room->user(evt.senderId());
    Conversation cons;
    cons.isStateEvent = evt.isStateEvent();
    cons.sendByMe   = (sender == m_room->localUser());
    cons.sender     = sender->displayname(room()) != "" ? sender->displayname(room()) : sender->id();
    cons.avatar     = sender->avatarMediaId(room());
    cons.receiver   = m_room->localUser()->id();
    cons.timestamp  = evt.originTimestamp().toTime_t();
    cons.senderId   = evt.senderId();
    cons.evtId = evt.id();
    cons.txnId = evt.transactionId();
    cons.visible = isValidMessageTime(cons);
    QString matrixType = evt.matrixType();
    cons.matrixType = matrixType;
    if((qUtils::strCompare(matrixType, NUNCHUK_ROOM_MESSAGE))
            || (qUtils::strCompare(matrixType, NUNCHUK_ROOM_MEMBER))
            || (qUtils::strCompare(matrixType, NUNCHUK_ROOM_NAME))
            || (qUtils::strCompare(matrixType, NUNCHUK_ROOM_ENCRYPTED))
            || (qUtils::strCompare(matrixType, NUNCHUK_ROOM_ENCRYPTION)))
    {
        eventToConversation(evt, cons, Qt::RichText);
    }
    else if((qUtils::strCompare(matrixType, NUNCHUK_EVENT_WALLET)) ||
             (qUtils::strCompare(matrixType, NUNCHUK_EVENT_TRANSACTION)) ||
             (qUtils::strCompare(matrixType, NUNCHUK_EVENT_EXCEPTION)))
    {
        bool ret = extractNunchukEvent(evt, cons);
        if(!ret){
            cons.messageType = (int)ENUNCHUCK::ROOM_EVT::INVALID;
        }
    }
    else{
        return cons;
    }
    return cons;
}

void QNunchukRoom::nunchukConsumeEvent(const RoomEvent &evt)
{
    QString matrixType = evt.matrixType();
    if((qUtils::strCompare(NUNCHUK_EVENT_WALLET, matrixType)) || (qUtils::strCompare(NUNCHUK_EVENT_TRANSACTION, matrixType))){
        QNunchukMatrixEvent e;
        e.set_event_id(evt.id());
        e.set_type(matrixType);
        e.set_content(QString(QJsonDocument(evt.contentJson()).toJson(QJsonDocument::Compact)));
        e.set_room_id(m_room->id());
        e.set_sender(evt.senderId());
        e.set_ts(evt.originTimestamp().toTime_t());
        matrixbrigde::ConsumeEvent(m_room->id(), e);
    }
}

void QNunchukRoom::nunchukConsumeSyncEvent(const RoomEvent &evt)
{
    if(CLIENT_INSTANCE->isNunchukLoggedIn() && CLIENT_INSTANCE->isMatrixLoggedIn() && AppSetting::instance()->enableMultiDeviceSync()){
        QString matrixType = evt.matrixType();
        if(qUtils::strCompare(NUNCHUK_EVENT_SYNC, matrixType)){
            QNunchukMatrixEvent e;
            e.set_event_id(evt.id());
            e.set_type(matrixType);
            e.set_content(QString(QJsonDocument(evt.contentJson()).toJson(QJsonDocument::Compact)));
            e.set_room_id(m_room->id());
            e.set_sender(evt.senderId());
            e.set_ts(evt.originTimestamp().toTime_t());
            matrixbrigde::ConsumeSyncEvent(m_room->id(),e);
        }
    }
}

void QNunchukRoom::nunchukNoticeEvent(const RoomEvent &evt)
{
    if (CLIENT_INSTANCE->isNunchukLoggedIn() && CLIENT_INSTANCE->isMatrixLoggedIn()) {
        QString matrixType = evt.matrixType();
        if(qUtils::strCompare(matrixType, NUNCHUK_ROOM_MESSAGE))
        {
            auto w = AppModel::instance()->walletInfoPtr();
            auto dashboard = QGroupWallets::instance()->dashboardInfoPtr();
            if (dashboard.isNull()) {
                dashboard = w ? w->dashboard() : dashboard;
            }            
            QString msgtype = evt.contentJson()["msgtype"].toString();
            DBG_INFO << msgtype;
            if (dashboard) {
                dashboard->GetAlertsInfo();
            }
            if (msgtype.contains("io.nunchuk.custom.draft_wallet", Qt::CaseInsensitive))
            {
                QGroupWallets::instance()->GetAllGroups();
                QUserWallets::instance()->GetListAllRequestAddKey();
                if (dashboard) {
                    dashboard->GetDraftWalletInfo();
                }
            }
            else if (msgtype.contains("io.nunchuk.custom.wallet_created", Qt::CaseInsensitive) ||
                     msgtype.contains("io.nunchuk.custom.wallet_name_changed", Qt::CaseInsensitive) ||
                     msgtype.contains("io.nunchuk.custom.key_name_changed", Qt::CaseInsensitive) ||
                     msgtype.contains("io.nunchuk.custom.key_updated", Qt::CaseInsensitive))
            {
                AppModel::instance()->requestCreateUserWallets();
                QList<uint> states = QEventProcessor::instance()->getCurrentStates();
                if(msgtype.contains("io.nunchuk.custom.wallet_created", Qt::CaseInsensitive) && 
                    !states.isEmpty() && states.last() == (uint)E::STATE_ID_SCR_DASHBOARD_ALERT_INFO) {
                    QEventProcessor::instance()->sendEvent(E::EVT_ONS_CLOSE_ALL_REQUEST);
                    QUserWallets::instance()->GetListAllRequestAddKey();
                }
            }
            else if(msgtype.contains("io.nunchuk.custom.transaction", Qt::CaseInsensitive))
            {
                QJsonObject content = evt.fullJson()["content"].toObject();
                DBG_INFO << content;
                QString wallet_id = content["wallet_local_id"].toString();
                if (wallet_id != "") {
                    if(msgtype.contains("io.nunchuk.custom.transaction_canceled") || msgtype.contains("io.nunchuk.custom.transaction_batch_created")){
                        AppModel::instance()->startSyncWalletDb(wallet_id);
                    }
                    else {
                        QString tx_id = content["transaction_id"].toString();
                        QWarningMessage msg;
                        nunchuk::Transaction tx = bridge::nunchukGetOriginTransaction(wallet_id, tx_id, msg);
                        if((int)EWARNING::WarningType::NONE_MSG == msg.type())
                        {
                            if(AppModel::instance()->walletList()){
                                QWalletPtr wallet = AppModel::instance()->walletList()->getWalletById(wallet_id);
                                if(wallet && wallet.data()->isAssistedWallet()){
                                    QTransactionPtr trans = wallet.data()->SyncAssistedTxs(tx);
                                    if (trans) {
                                        QJsonObject data = wallet.data()->GetServerKeyInfo(trans->txid());
                                        if(!data.isEmpty()){
                                            trans->setServerKeyMessage(data);
                                        }
                                        DBG_INFO << trans.data()->memo();
                                        wallet.data()->transactionHistory()->updateTransaction(trans->txid(), trans);
                                        if(AppModel::instance()->transactionInfo()){
                                            QString current_tx_wallet_id = AppModel::instance()->transactionInfo()->walletId();
                                            QString current_tx_id        = AppModel::instance()->transactionInfo()->txid();
                                            if(qUtils::strCompare(wallet_id, current_tx_wallet_id) && qUtils::strCompare(tx_id, current_tx_id)){
                                                AppModel::instance()->setTransactionInfo(trans);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        else {
                            AppModel::instance()->startSyncWalletDb(wallet_id);
                        }
                    }
                }
            }
            else if (msgtype.contains("io.nunchuk.custom.group", Qt::CaseInsensitive))
            {
                if (dashboard) {
                    dashboard->GetHealthCheckInfo();
                }
                QGroupWallets::instance()->GetAllGroups();

                if (msgtype.contains("io.nunchuk.custom.group_wallet_created", Qt::CaseInsensitive) ||
                    msgtype.contains("io.nunchuk.custom.group_wallet_deleted", Qt::CaseInsensitive) ||
                    msgtype.contains("io.nunchuk.custom.group_wallet_name_changed", Qt::CaseInsensitive) ||
                    msgtype.contains("io.nunchuk.custom.group_membership_request_created", Qt::CaseInsensitive))
                {
                    AppModel::instance()->requestCreateUserWallets();
                }
            }
            else if (   msgtype.contains("io.nunchuk.custom.dummy_transaction", Qt::CaseInsensitive)
                     || msgtype.contains("io.nunchuk.custom.server_key_change", Qt::CaseInsensitive)
                     || msgtype.contains("io.nunchuk.custom.wallet_inheritance", Qt::CaseInsensitive)
                     || msgtype.contains("io.nunchuk.custom.health_check", Qt::CaseInsensitive)
                     || msgtype.contains("io.nunchuk.custom.server_key", Qt::CaseInsensitive)
                     || msgtype.contains("io.nunchuk.custom.recurring_payment", Qt::CaseInsensitive)
                     || msgtype.contains("io.nunchuk.custom.key_recovery", Qt::CaseInsensitive)
                     || msgtype.contains("io.nunchuk.custom.health_check_reminder_updated", Qt::CaseInsensitive)
                     || msgtype.contains("io.nunchuk.custom.health_check_reminder", Qt::CaseInsensitive)
                     || msgtype.contains("io.nunchuk.custom.health_check_skipped", Qt::CaseInsensitive))
            {
                if (dashboard) {
                    dashboard->GetHealthCheckInfo();
                    if (auto plan = dashboard->inheritancePlanPtr()) {
                        plan->GetInheritancePlan();
                    }
                }
                if (msgtype.contains("io.nunchuk.custom.wallet_inheritance_updated", Qt::CaseInsensitive)
                 || msgtype.contains("io.nunchuk.custom.health_check_completed", Qt::CaseInsensitive)
                 || msgtype.contains("io.nunchuk.custom.server_key_updated", Qt::CaseInsensitive)
                 || msgtype.contains("io.nunchuk.custom.recurring_payment_approved", Qt::CaseInsensitive)
                 || msgtype.contains("io.nunchuk.custom.key_recovery_approved", Qt::CaseInsensitive))
                {
                    QJsonObject content = evt.fullJson()["content"].toObject();
                    QString dummy_transaction_id     = content["dummy_transaction_id"].toString();
                    if (dashboard) {
                        if (auto dummy = dashboard->groupDummyTxPtr()) {
                            if (qUtils::strCompare(dummy_transaction_id, dummy->tx_id())) {
                                QList<uint> states = QEventProcessor::instance()->getCurrentStates();
                                if(!states.isEmpty() && states.last() == (uint)E::STATE_ID_SCR_DUMMY_TRANSACTION_INFO) {
                                    QEventProcessor::instance()->sendEvent(E::EVT_ONS_CLOSE_ALL_REQUEST);
                                }
                            }
                        }
                    }
                }
            }
            else if(msgtype.contains("io.nunchuk.custom.group_wallet_primary_owner_updated", Qt::CaseInsensitive)){
                // TBD
            }
            else if(msgtype.contains("io.nunchuk.custom.group_wallet_alias", Qt::CaseInsensitive)){
                // TBD
            }
            else if(msgtype.contains("io.nunchuk.custom.saved_address_updated", Qt::CaseInsensitive)){
                emit AppSetting::instance()->favoriteAddressesChanged();
            }
            else if(msgtype.contains("io.nunchuk.custom.wallet_key_replacement_canceled", Qt::CaseInsensitive)
                       || msgtype.contains("io.nunchuk.custom.wallet_key_replacement_reset", Qt::CaseInsensitive))
            {
                QList<uint> states = QEventProcessor::instance()->getCurrentStates();
                if(!states.isEmpty() && states.last() == (uint)E::STATE_ID_SCR_DASHBOARD_ALERT_INFO)
                {
                    QEventProcessor::instance()->sendEvent(E::EVT_ONS_CLOSE_ALL_REQUEST);
                }
            }
            else if(msgtype.contains("io.nunchuk.custom.wallet_key_replacement_pending", Qt::CaseInsensitive))
            {

            }
            else if(msgtype.contains("io.nunchuk.custom.wallet_key_replacement_completed", Qt::CaseInsensitive)
                       || msgtype.contains("io.nunchuk.custom.wallet_replaced", Qt::CaseInsensitive))
            {
                QList<uint> states = QEventProcessor::instance()->getCurrentStates();
                if(msgtype.contains("io.nunchuk.custom.wallet_replaced", Qt::CaseInsensitive)
                    && !states.isEmpty()
                    && states.last() == (uint)E::STATE_ID_SCR_DASHBOARD_ALERT_INFO)
                {
                    QEventProcessor::instance()->sendEvent(E::EVT_ONS_CLOSE_ALL_REQUEST);
                }

                if (dashboard) {
                    dashboard->GetWalletInfo();
                    dashboard->GetKeyReplacementStatus();
                    if(dashboard->isReplaced()){
                        dashboard->setShowDashBoard(false);
                        AppModel::instance()->requestCreateUserWallets();
                    }
                    if (auto walletList = AppModel::instance()->walletListPtr()) {
                        walletList->refresh();
                    }
                }
            }
            else if(msgtype.contains("io.nunchuk.custom.wallet_replacement_timelock_set", Qt::CaseInsensitive)){
                if (dashboard && dashboard->showDashBoard()) {
                    dashboard->GetKeyReplacementStatus();
                }
            }
            else if(msgtype.contains("io.nunchuk.custom.claiming_wallet_created", Qt::CaseInsensitive)
                       || msgtype.contains("io.nunchuk.custom.claiming_wallet_deleted", Qt::CaseInsensitive)
                       || msgtype.contains("io.nunchuk.custom.claiming_wallet_updated", Qt::CaseInsensitive))
            {
                AppModel::instance()->requestCreateUserWallets();
            }
            else {
                // for honey badger
                if (dashboard) {
                    dashboard->GetHealthCheckInfo();
                }
            }
        }
    }
}

QConversationModel *QNunchukRoom::conversation() const
{
    return m_conversation.data();
}

QConversationModelPtr QNunchukRoom::conversationPtr() const
{
    return m_conversation;
}

void QNunchukRoom::setConversation(const QConversationModelPtr &data)
{
    m_conversation = data;
    emit conversationChanged();
}

QString QNunchukRoom::typingNames()
{
    return m_typingNames;
}

void QNunchukRoom::setTypingNames(const QString names)
{
    m_typingNames = names;
    emit typingNamesChanged();
}

bool QNunchukRoom::allHisLoaded() const
{
    if(m_room){
        return m_room->allHistoryLoaded();
    }
    return false;
}

QRoomTransaction *QNunchukRoom::pinTransaction()
{
    return m_pinTransaction;
}

void QNunchukRoom::setPinTransaction(QRoomTransactionPtr tran)
{
    m_pinTransaction = tran.data();
    emit pinTransactionChanged();
}

nunchuk::Wallet QNunchukRoom::createWalletFromJson(const QJsonObject &json)
{
    int m = max(0, json["m"].toInt());
    int n = max(0, json["n"].toInt());

    nunchuk::AddressType address_type = addr_map[json["address_type"].toString()];
    QString description = json["description"].toString();
    bool is_escrow = json["is_escrow"].toBool();
    QString name = json["name"].toString();
    std::vector<nunchuk::SingleSigner> signers;
    QJsonArray members = json["members"].toArray();
    auto getSigner = [](const QJsonObject &js)->nunchuk::SingleSigner{
        QString  derivation_path    = js["derivation_path"].toString();
        QString  master_fingerprint = js["master_fingerprint"].toString();
        QString  public_key         = js["public_key"].toString();
        QString  xpub               = js["xpub"].toString();
        nunchuk::SingleSigner singlesinger("",
                                           xpub.toStdString(),
                                           public_key.toStdString(),
                                           derivation_path.toStdString(),
                                           {},
                                           master_fingerprint.toStdString(),
                                           std::time(0),
                                           "");
        return singlesinger;
    };
    for(QJsonValue js:members){
        signers.push_back(getSigner(js.toObject()));
    }
    nunchuk::Wallet wallet(false);
    if(signers.size() > 0){
        try {
            wallet.set_signers(signers);
            wallet.set_address_type(address_type);
            wallet.set_description(description.toStdString());
            wallet.set_m(m);
            wallet.set_n(n);
            // wallet.set_escrow(is_escrow); //FIXME
            wallet.set_name(name.toStdString());
        } catch (const nunchuk::NunchukException &e) {
            DBG_INFO << "nunchuk::NunchukException: " << e.what() << m << n << signers.size();
        }
    }
    return wallet;
}

bool QNunchukRoom::isEncrypted()
{
    return m_IsEncrypted;
}

QNunchukRoomListModel::QNunchukRoomListModel(Connection *c):
    m_connection(c),
    m_currentIndex(-1),
    m_currentRoom(NULL)
{
    m_data.clear();
    m_servive.clear();
    m_roomWallets.clear();
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
    QObject::connect(&m_watcherSync, &QFutureWatcher<void>::finished, this, &QNunchukRoomListModel::synchonizesUserDataFinished);
}

QNunchukRoomListModel::~QNunchukRoomListModel()
{

}

int QNunchukRoomListModel::rowCount(const QModelIndex &parent) const
{
    return m_data.count();
}

int QNunchukRoomListModel::count() const
{
    return m_data.count();
}

QVariant QNunchukRoomListModel::data(const QModelIndex &index, int role) const
{
    switch (role) {
    case room_id:
        return m_data[index.row()].data()->id();
    case room_name:
        return m_data[index.row()].data()->roomName();
    case room_joinstate:
        return (int)m_data[index.row()].data()->roomJoinState();
    case room_unreadmsg_count:
        return m_data[index.row()].data()->unreadCount();
    case room_last_timestamp:
        return m_data[index.row()].data()->lasttimestamp();
    case room_typing_users:
        return m_data[index.row()].data()->typingNames();
    case room_wallet_ready:
        return (m_data[index.row()].data()->roomWallet() && m_data[index.row()].data()->roomWallet()->get_init_event_id() != "");
    case room_last_message:
        return m_data[index.row()].data()->lastMessage();
    case room_users_count:
        return m_data[index.row()].data()->userCount();
    case room_avatar_url:
        return m_data[index.row()].data()->roomAvatar();
    case room_is_encrypted:
        return m_data[index.row()].data()->isEncrypted();
    case room_type:
        return m_data[index.row()].data()->roomType();
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> QNunchukRoomListModel::roleNames() const
{
    QHash<int, QByteArray> names;
    names[room_id]         = "id";
    names[room_name]       = "name";
    names[room_type]       = "room_type";
    names[room_joinstate]  = "joinstate";
    names[room_unreadmsg_count] = "unreadCount";
    names[room_last_timestamp]  = "lasttimestamp";
    names[room_typing_users]    = "typing_users";
    names[room_wallet_ready]    = "wallet_ready";
    names[room_last_message]    = "last_message";
    names[room_users_count]     = "users_count";
    names[room_avatar_url]      = "room_avatar";
    names[room_is_encrypted]    = "is_encrypted";
    return names;
}

QNunchukRoomPtr QNunchukRoomListModel::getRoomByIndex(const int index) const
{
    if(0 <= index && index < m_data.count() ){
        return m_data[index];
    }
    else{
        return NULL;
    }
}

QNunchukRoomPtr QNunchukRoomListModel::getRoomById(const QString &id) const
{
    for(QNunchukRoomPtr r : m_data){
        if(qUtils::strCompare(id, r.data()->id())){
            return r;
        }
    }
    for(QNunchukRoomPtr r : m_servive){
        if(qUtils::strCompare(id, r.data()->id())){
            return r;
        }
    }
    return NULL;
}

int QNunchukRoomListModel::getIndex(const QString &id) const
{
    for(int i = 0; i < m_data.size(); i++){
        QNunchukRoomPtr r = m_data.at(i);
        if(qUtils::strCompare(id, r.data()->id())){
            return i;
        }
    }
    return -1;
}

Connection *QNunchukRoomListModel::connection()
{
    return m_connection;
}

void QNunchukRoomListModel::downloadRooms()
{
    if(connection()){
        CLIENT_INSTANCE->setReadySupport(true);
        connect(&m_time, &QTimer::timeout, connection(), &Connection::capabilitiesLoaded);
        m_time.setSingleShot(true);
        m_time.start(10000);
        connectSingleShot(connection(), &Connection::capabilitiesLoaded, this, [this] {
            DBG_INFO << "downloadRooms Connection::capabilitiesLoaded";
            m_time.stop();
            timeoutHandler(300, [this]() {
                connect(connection(), &Connection::joinedRoom,  this, &QNunchukRoomListModel::joinedRoom);
                connect(connection(), &Connection::newRoom,     this, &QNunchukRoomListModel::newRoom);
                connect(connection(), &Connection::leftRoom,    this, &QNunchukRoomListModel::leftRoom);
                connect(connection(), &Connection::invitedRoom, this, &QNunchukRoomListModel::invitedRoom);
                connect(connection(), &Connection::aboutToDeleteRoom,   this, &QNunchukRoomListModel::aboutToDeleteRoom);
                connect(connection(), &Connection::aboutToDeleteRoom,  ClientController::instance(), &ClientController::refreshContacts);
                DBG_INFO << "ALL ROOM" << m_connection->allRooms().count() << "ROOM INVITED" << m_connection->roomsCount(JoinState::Invite);
                for (Room *room : m_connection->allRooms()) {
                    if(JoinState::Join == room->joinState()){
                        DBG_INFO << "room Join " << room->name();
                        doAddRoom(QNunchukRoomPtr(new QNunchukRoom(room), &QObject::deleteLater));
                        if(currentIndex() == -1){
                            setCurrentIndex(0);
                        }
                    }
                    else if(JoinState::Invite == room->joinState()){
                        DBG_INFO << "room Invite " << room->name();
                        connection()->joinRoom(room->id());
                    }
                    else if(JoinState::Leave == room->joinState()){
                        DBG_INFO << "room Leave " << room->name();
                    }
                    else{
                        DBG_INFO << "room ELSE " << room->name();
                    }
                }
                resort();
                checkNunchukSyncRoom();
                if(currentRoom()){
                    if(currentRoom()->conversation()){
                        currentRoom()->conversation()->refresh();
                    }
                }
                if(AppSetting::instance()->enableMultiDeviceSync()){
                    AppModel::instance()->startMultiDeviceSync(true);
                }
                else{
                    AppModel::instance()->startMultiDeviceSync(false);
                }
                emit finishedDownloadRoom();
                CLIENT_INSTANCE->setReadySupport(true);
                downloadRoomWallets();
                synchonizesUserData();
            });
        });
    }
}

void QNunchukRoomListModel::downloadRoomWallets()
{
    // Download all shared wallet
    QWarningMessage warningmsg;
    QList<QRoomWalletPtr> wallets = matrixbrigde::GetAllRoomWallets(warningmsg);
    if((int)EWARNING::WarningType::NONE_MSG == warningmsg.type()){
        setRoomWallets(wallets);
    }
    AppModel::instance()->requestSyncSharedWallets();
}

int QNunchukRoomListModel::currentIndex() const
{
    return m_currentIndex;
}

void QNunchukRoomListModel::setCurrentIndex(int index)
{
    if(index == -1){
        m_currentIndex = index;
        stopCountdown();
        setCurrentRoom(NULL);
    }
    else{
        QNunchukRoomPtr roomIndex = getRoomByIndex(index);
        if(roomIndex){
            m_currentIndex = index;
            setCurrentRoom(roomIndex);
        }
    }
    emit currentIndexChanged();
}

QNunchukRoom *QNunchukRoomListModel::currentRoom() const
{
    return m_currentRoom.data();
}

QNunchukRoomPtr QNunchukRoomListModel::currentRoomPtr() const
{
    return m_currentRoom;
}

void QNunchukRoomListModel::setCurrentRoom(const QNunchukRoomPtr &newRoom)
{
    if(m_currentRoom != newRoom){
        stopCountdown();
        m_currentRoom = newRoom;
        if(m_currentRoom){
            m_currentRoom.data()->setDisplayed(true);
            m_currentRoom.data()->startGetPendingTxs();
            if(m_currentRoom->conversation()){
                if(m_currentRoom->conversation()->unreadLastIndex() + 10 > m_currentRoom->conversation()->count()){
                    m_currentRoom.data()->conversation()->setCurrentIndex(m_currentRoom.data()->conversation()->rowCount() - 1);
                    m_currentRoom->markAllMessagesAsRead();
                }
                else{
                    m_currentRoom.data()->conversation()->setCurrentIndex(m_currentRoom->conversation()->unreadLastIndex());
                }
            }
            if((int)ENUNCHUCK::TabSelection::CHAT_TAB == AppModel::instance()->tabIndex()){
                startCountdown();
            }
        }
        emit currentRoomChanged();
    }
}

void QNunchukRoomListModel::sendMessage(const QString &msg)
{
    if(currentRoom()){
        currentRoom()->sendMessage(msg);
    }
}

int QNunchukRoomListModel::totalUnread()
{
    int ret = 0;
    for (int i = 0; i < m_data.count(); i++) {
        ret += m_data.at(i).data() ? m_data.at(i).data()->unreadCount() : 0;
    }
    return ret;
}

void QNunchukRoomListModel::requestSort()
{
    qSort(m_data.begin(), m_data.end(), sortRoomListByTimeDescending);
}

QString QNunchukRoomListModel::getRoomIdByWalletId(const QString &wallet_id)
{
    foreach (QNunchukRoomPtr it, m_data) {
        if(it && it.data()->roomWallet() && (qUtils::strCompare(it.data()->roomWallet()->get_wallet_id(), wallet_id))){
            return it.data()->id();
        }
    }
    return "";
}

void QNunchukRoomListModel::transactionChanged(const QString &wallet_id, const QString &tx_id, const int status, const int height)
{
    for( QNunchukRoomPtr it: m_data){
        if(it && it.data()->roomWallet() && (qUtils::strCompare(it.data()->roomWallet()->get_wallet_id(), wallet_id))){
            it.data()->transactionChanged(tx_id, status, height);
        }
    }
}

void QNunchukRoomListModel::updateTransactionMemo(const QString& wallet_id, const QString &tx_id, const QString &memo)
{
    for( QNunchukRoomPtr r: m_data){
        if(r && r.data()->roomWallet() && (qUtils::strCompare(wallet_id, r.data()->roomWallet()->get_wallet_id()))){
            r.data()->updateTransactionMemo(tx_id, memo);
        }
    }
}

void QNunchukRoomListModel::doAddRoom(QNunchukRoomPtr r)
{
    if(!r){return;}
    if( r.data()->isServerNoticeRoom() || r.data()->isNunchukSyncRoom()){
        if(!r.data()->id().isEmpty() && !containsServiceRoom(r.data()->id()) ){
            m_servive.append(r);
            if(r.data()->isServerNoticeRoom()){
                r.data()->connectRoomServiceSignals();
//                r.data()->downloadHistorical();//FIXME - DEBUG
            }
            if(r.data()->isNunchukSyncRoom()){
                r.data()->connectRoomSignals();
            }
        }
    }
    else{
        if(!r.data()->id().isEmpty() && !containsRoomId(r.data()->id()) ){
            beginInsertRows(QModelIndex(), rowCount(), rowCount());
            m_data.append(r);
            endInsertRows();
            connect(r.data(),         &QNunchukRoom::roomNameChanged,       this, [this, r] { refresh(r); });
            connect(r.data(),         &QNunchukRoom::lastMessageChanged,    this, [this, r] { refresh(r); });
            connect(r.data(),         &QNunchukRoom::lasttimestampChanged,  this, [this, r] { resort(); });
            connect(r.data(),         &QNunchukRoom::roomNeedTobeLeaved,    this, &QNunchukRoomListModel::roomNeedTobeLeaved);
            connect(r.data()->room(), &Room::unreadMessagesChanged,         this, [this, r] { refresh(r); });
            connect(r.data()->room(), &Room::typingChanged,                 this, [this, r] { refresh(r); });
            connect(r.data()->room(), &Room::unreadMessagesChanged,         this, &QNunchukRoomListModel::totalUnreadChanged);
            connect(r.data(),         &QNunchukRoom::pendingTxsChanged,     this, [this, r] { refresh(r); });
            if(r.data()->room()->currentState().contains(NUNCHUK_ROOM_RETENTION)){
                DBG_INFO << "room name" << r.data()->roomName() << r.data()->room()->currentState().contentJson(NUNCHUK_ROOM_RETENTION);
                qint64 max_lifetime = r.data()->room()->currentState().contentJson(NUNCHUK_ROOM_RETENTION)["max_lifetime"].toDouble();
                r.data()->setMessageMaxLifeTime(max_lifetime);
            }
            else{
                // Set room state event is rentention (byzantine room)
                if(r.data()->isByzantineRoom()){
                    r.data()->activateRetention();
                }
            }
            r.data()->connectRoomSignals();
        }
    }
    emit countChanged();
}

void QNunchukRoomListModel::removeRoomByIndex(const int index)
{
    beginResetModel();
    m_data.removeAt(index);
    if(m_data.count() > 0){
        setCurrentIndex(0);
    }
    else{
        setCurrentIndex(-1);
    }
    endResetModel();
    emit countChanged();
}

void QNunchukRoomListModel::removeRoomById(const QString &id)
{
    beginResetModel();
    for( QNunchukRoomPtr it: m_data){
        if(it && (qUtils::strCompare(it.data()->id(), id))){
            m_data.removeOne(it);
            break;
        }
    }
    endResetModel();
    emit countChanged();
}

void QNunchukRoomListModel::removeAll()
{
    beginResetModel();
    while (m_data.count() > 0) {
        m_data.removeAt(0);
    }
    setCurrentIndex(-1);
    endResetModel();
    emit countChanged();
}

void QNunchukRoomListModel::forgetRoom(const int index)
{
    Room* room = getRoomByIndex(index).data()->room();
    if(room && connection()){
        auto joinJob = connection()->forgetRoom(room->id());
        connect(joinJob, &BaseJob::success, this, [this, index] {
            // Ensure that the room has been joined and filled with some events so that other tests could use that
            removeRoomByIndex(index);
        });
    }
    emit countChanged();
}

void QNunchukRoomListModel::leaveCurrentRoom()
{
    if(currentRoom()){
        bool hasWallet = currentRoom()->roomWallet() ? true : false;
        bool isByzantineRoom = currentRoom()->isNunchukByzantineRoom();
        QString byzantineGroupId = currentRoom()->byzantineRoomGroupId();
        QString currentRoomId = currentRoom()->id();

        auto* job = currentRoom()->room()->leaveRoom();
        connect(job, &BaseJob::success, this, [this, currentRoomId, hasWallet, isByzantineRoom, byzantineGroupId] {
            removeRoomByIndex(currentIndex());
            if(hasWallet){
                AppModel::instance()->startReloadUserDb();
            }
            if(isByzantineRoom){
                emit byzantineRoomDeleted(currentRoomId, byzantineGroupId);
            }
        });
        emit countChanged();
    }
}

void QNunchukRoomListModel::leaveRoom(const int index)
{
    Room* room = getRoomByIndex(index).data()->room();
    if(room && connection()){
        auto joinJob = connection()->leaveRoom(room);
        connect(joinJob, &BaseJob::success, this, [this, index] {
            // Ensure that the room has been joined and filled with some events so that other tests could use that
            removeRoomByIndex(index);
            DBG_INFO << "LEAVE ROOM SUCCEED";
        });
        connect(joinJob, &BaseJob::failure, this, [] {
            DBG_INFO << "Failed to join the room";
        });
    }
    emit countChanged();
}

void QNunchukRoomListModel::joinRoom(QString roomAliasOrId)
{
    if(connection()){
        auto joinJob = connection()->joinRoom(roomAliasOrId);
        connect(joinJob, &BaseJob::failure, this, [] {
            DBG_INFO << "Failed to join the room";
        });
    }
    emit countChanged();
}

void QNunchukRoomListModel::createRoomChat(const QStringList invitees_id, const QString& room_name, QVariant firstMessage)
{
    if(connection()){
        Connection::RoomVisibility in_visibility = Connection::UnpublishRoom;
        const QString   in_alias = {};
        const QString   in_name = room_name;
        const QString   in_topic = {};
        QStringList     in_invites = invitees_id;
        const QString   in_presetName = {};
        const QString   in_roomVersion = {};
        bool            in_isDirect = false;
        CreateRoomJob::StateEvent state_encryption_Evt;
        state_encryption_Evt.type = "m.room.encryption";
        state_encryption_Evt.content["algorithm"] = "m.megolm.v1.aes-sha2";

#if 0 // BYZANTINE
        CreateRoomJob::StateEvent state_retention_Evt;
        state_retention_Evt.type = NUNCHUK_ROOM_RETENTION;
        state_retention_Evt.content["max_lifetime"] = NUNCHUK_ROOM_RETENTION_TIME;

        const QVector<CreateRoomJob::StateEvent> in_initialState = {state_encryption_Evt, state_retention_Evt};
#else
        const QVector<CreateRoomJob::StateEvent> in_initialState = {state_encryption_Evt};
#endif

        auto createJob = connection()->createRoom(in_visibility,
                                                  in_alias,
                                                  in_name,
                                                  in_topic,
                                                  in_invites,
                                                  in_presetName,
                                                  in_roomVersion,
                                                  in_isDirect,
                                                  in_initialState);
        connect(createJob, &BaseJob::success, this, [this, createJob, firstMessage] {
            if(!firstMessage.isNull() && firstMessage.toString() != ""){
                connection()->room(createJob->roomId())->postPlainText(firstMessage.toString());
            }
        });
        connect(createJob, &BaseJob::failure, this, [createJob] {
            DBG_INFO << "//FIXME Failed to create the room";
            AppModel::instance()->showToast(createJob->error(), createJob->errorString(), EWARNING::WarningType::EXCEPTION_MSG);
        });
    }
    emit countChanged();
}

void QNunchukRoomListModel::createRoomDirectChat(const QString invitee_id, const QString &invitee_name, QVariant firstMessage)
{
    DBG_INFO << invitee_id << invitee_name << invitee_name;
    if(connection()){
        int index = -1;
        QString room_id = "";
        if(containsRoomName(invitee_name, index, room_id)){
            if(index >= 0){
                setCurrentIndex(index);
            }
        }
        else{
            Connection::RoomVisibility in_visibility = Connection::UnpublishRoom;
            const QString   in_alias = {};
            const QString   in_name = invitee_name;
            const QString   in_topic = {};
            QStringList     in_invites = {invitee_id};
            const QString   in_presetName = {};
            const QString   in_roomVersion = {};
            bool            in_isDirect = true;
            CreateRoomJob::StateEvent state_encryption_Evt;
            state_encryption_Evt.type = "m.room.encryption";
            state_encryption_Evt.content["algorithm"] = "m.megolm.v1.aes-sha2";
#if 0 // BYZANTINE
            CreateRoomJob::StateEvent state_retention_Evt;
            state_retention_Evt.type = NUNCHUK_ROOM_RETENTION;
            state_retention_Evt.content["max_lifetime"] = NUNCHUK_ROOM_RETENTION_TIME;

            const QVector<CreateRoomJob::StateEvent> in_initialState = {state_encryption_Evt, state_retention_Evt};
#else
            const QVector<CreateRoomJob::StateEvent> in_initialState = {state_encryption_Evt};
#endif
            auto createJob = connection()->createRoom(in_visibility,
                                                      in_alias,
                                                      in_name,
                                                      in_topic,
                                                      in_invites,
                                                      in_presetName,
                                                      in_roomVersion,
                                                      in_isDirect,
                                                      in_initialState);
            connect(createJob, &BaseJob::success, this, [this, createJob, firstMessage] {
                if(!firstMessage.isNull() && firstMessage.toString() != ""){
                    connection()->room(createJob->roomId())->postPlainText(firstMessage.toString());
                }
            });
            connect(createJob, &BaseJob::failure, this, [createJob] {
                AppModel::instance()->showToast(createJob->error(), createJob->errorString(), EWARNING::WarningType::EXCEPTION_MSG);
            });
            emit countChanged();
        }
    }
}

void QNunchukRoomListModel::createRoomByzantineChat(const QStringList invitees_id, const QString &room_name, const QString &group_id, QVariant firstMessage)
{
    DBG_INFO << invitees_id << room_name;
    if(connection() && invitees_id.size() > 0){
        int index = -1;
        QString room_id = "";
        if(containsRoomName(room_name, index, room_id)){
            if(index >= 0){
                setCurrentIndex(index);
                emit byzantineRoomCreated(room_id, group_id, true);
            }
        }
        else{
            Connection::RoomVisibility in_visibility = Connection::UnpublishRoom;
            const QString   in_alias = {};
            const QString   in_name = room_name;
            const QString   in_topic = {};
            QStringList     in_invites = invitees_id;
            const QString   in_presetName = {};
            const QString   in_roomVersion = {};
            bool            in_isDirect = in_invites.count() > 1 ? false : true;
            CreateRoomJob::StateEvent state_byzantine_Evt;
            state_byzantine_Evt.type = NUNCHUK_ROOM_BYZANTINE;
            state_byzantine_Evt.content["group_id"] = group_id;

            CreateRoomJob::StateEvent state_retention_Evt;
            state_retention_Evt.type = NUNCHUK_ROOM_RETENTION;
            state_retention_Evt.content["max_lifetime"] = NUNCHUK_ROOM_RETENTION_TIME;

            const QVector<CreateRoomJob::StateEvent> in_initialState = {state_byzantine_Evt, state_retention_Evt};

            auto createJob = connection()->createRoom(in_visibility,
                                                      in_alias,
                                                      in_name,
                                                      in_topic,
                                                      in_invites,
                                                      in_presetName,
                                                      in_roomVersion,
                                                      in_isDirect,
                                                      in_initialState);
            connect(createJob, &BaseJob::success, this, [this, createJob, firstMessage, group_id] {
                if(!firstMessage.isNull() && firstMessage.toString() != ""){
                    connection()->room(createJob->roomId())->postPlainText(firstMessage.toString());
                }
                emit byzantineRoomCreated(createJob->roomId(), group_id, false);
            });
            connect(createJob, &BaseJob::failure, this, [createJob] {
                DBG_INFO << "//FIXME Failed to create the room";
                AppModel::instance()->showToast(createJob->error(), createJob->errorString(), EWARNING::WarningType::EXCEPTION_MSG);
            });
            emit countChanged();
        }
    }
}

void QNunchukRoomListModel::createSupportRoom()
{
    if((int)ENUNCHUCK::Chain::MAIN == (int)AppSetting::instance()->primaryServer() || (int)ENUNCHUCK::Chain::TESTNET == (int)AppSetting::instance()->primaryServer())
    {
        if(connection()){
            QString tagname = (int)ENUNCHUCK::Chain::MAIN == (int)AppSetting::instance()->primaryServer() ?  NUNCHUK_ROOM_SUPPORT : NUNCHUK_ROOM_SUPPORTTESTNET;
            if(containsSupportRoom(tagname)){
                return;
            }
            CLIENT_INSTANCE->setReadySupport(false);
            auto createJob = connection()->createDirectChat("@support:nunchuk.io");
            createJob->setMaxRetries(0);
            connect(createJob, &BaseJob::success, this, [this, createJob, tagname] {
                Quotient::Room *newroom = connection()->room(createJob->roomId());
                if(newroom){
                    newroom->addTag(tagname);
                }
                CLIENT_INSTANCE->setReadySupport(true);
            });
            connect(createJob, &BaseJob::failure, this, [createJob] {
                AppModel::instance()->showToast(createJob->error(), createJob->errorString(), EWARNING::WarningType::EXCEPTION_MSG);
                CLIENT_INSTANCE->setReadySupport(true);
            });
        }
        emit countChanged();
    }
}

bool QNunchukRoomListModel::allHisLoaded()
{
    bool ret = true;
    if(m_data.size() < 1) return false;
    foreach (QNunchukRoomPtr it, m_data) {
        if(it){
            ret = ret && it->isDownloaded();
        }
    }
    return ret;
}

QList<QRoomWalletPtr> QNunchukRoomListModel::getRoomWallets() const
{
    return m_roomWallets;
}

void QNunchukRoomListModel::setRoomWallets(const QList<QRoomWalletPtr> &roomWallets)
{
    m_roomWallets = roomWallets;
}

void QNunchukRoomListModel::renameRoomByzantineChat(const QString room_id, const QString group_id, const QString newname)
{
    DBG_INFO << "room_id:" << room_id << "group_id:" << group_id << "newname:" << newname;
    foreach (QNunchukRoomPtr it, m_data) {
        if(qUtils::strCompare(it.data()->id(), room_id) || qUtils::strCompare(it.data()->byzantineRoomGroupId(), group_id)){
            if(!qUtils::strCompare(it.data()->roomName(), newname)){
                it.data()->setRoomName(newname);
            }
        }
    }
}

void QNunchukRoomListModel::stopCountdown()
{
    if(m_currentRoom){ // Stop count in old room
        m_currentRoom->stopCountdown();
    }
}

void QNunchukRoomListModel::startCountdown()
{
    if(m_currentRoom){ // Start count in new room
        m_currentRoom->startCountdown();
    }
}

void QNunchukRoomListModel::updateMaxLifeTime(QString &roomId, qint64 maxLifeTime)
{
    for(QNunchukRoomPtr it : m_data){
        if(it && qUtils::strCompare(it.data()->id(), roomId)){
            it.data()->setMessageMaxLifeTime(maxLifeTime);
            it.data()->activateRetention(maxLifeTime);
        }
    }
}

bool QNunchukRoomListModel::containsRoomId(const QString &id)
{
    foreach (QNunchukRoomPtr it, m_data) {
        if(qUtils::strCompare(it.data()->id(), id)){
            return true;
        }
    }
    return false;
}

bool QNunchukRoomListModel::containsRoomName(const QString &name, int &index, QString &room_id)
{
    for(int i = 0; i < m_data.count(); i++){
        if(m_data.at(i)){
            if(qUtils::strCompare(m_data.at(i)->roomName(), name)){
                index = i;
                room_id = m_data.at(i)->id();
                return true;
            }
        }
    }
    return false;
}

bool QNunchukRoomListModel::containsServiceRoom(const QString &id)
{
    foreach (QNunchukRoomPtr it, m_servive) {
        if(qUtils::strCompare(it.data()->id(), id)){
            return true;
        }
    }
    return false;
}

bool QNunchukRoomListModel::containsSyncRoom()
{
    foreach (QNunchukRoomPtr it, m_servive) {
        if(it.data()->isNunchukSyncRoom()){
            return true;
        }
    }
    return false;
}

bool QNunchukRoomListModel::containsSupportRoom(const QString& tagname){
    foreach (QNunchukRoomPtr it, m_data) {
        if(it.data()->room() && it.data()->room()->tagNames().contains(tagname)){
            if(it.data()->room()->joinedCount()){
                it.data()->room()->inviteToRoom("@support:nunchuk.io");
                int index = getIndex(it.data()->room()->id());
                if(index >= 0){
                    setCurrentIndex(index);
                }
            }
            return true;
        }
    }
    return false;
}

void QNunchukRoomListModel::synchonizesUserData()
{
    AppModel::instance()->openPromtNunchukSync();
    QFuture<void> future = QtConcurrent::run([=]() {
        QList<QNunchukRoomPtr> syncRooms;
        syncRooms.clear();
        foreach (QNunchukRoomPtr it, m_servive) {
            if(it.data()->isNunchukSyncRoom()){
                syncRooms.append(it);
            }
        }
        for(int i = 0; i< syncRooms.count(); i++){
            int percent = ((double)(i+1)/syncRooms.count())*100;
            AppSetting::instance()->setSyncPercent(percent);
            if(syncRooms.at(i).data()){
                syncRooms.at(i).data()->synchonizesUserData();
            }
        }
    });
    m_watcherSync.setFuture(future);
}

void QNunchukRoomListModel::checkNunchukSyncRoom()
{
    if(connection() && !containsSyncRoom()){
        QStringList invitees_id;
        invitees_id.clear();
        invitees_id.append(connection()->userId());
        auto createJob = connection()->createRoom(Connection::UnpublishRoom, "", "Nunchuk Sync", "Nunchuk Sync", invitees_id);
        connect(createJob, &BaseJob::success, this, [this, createJob] {
            Quotient::Room *newroom = connection()->room(createJob->roomId());
            if(newroom){
                newroom->addTag(NUNCHUK_ROOM_SYNC);
            }
        });
        emit countChanged();
    }
    else{
        for (QNunchukRoomPtr r : m_servive) {
            if(r.data()->isNunchukSyncRoom()){
                r.data()->connectRoomSignals();
            }
        }
    }
}

bool QNunchukRoomListModel::hasContact(const QString &id)
{
    for(QNunchukRoomPtr ptr: m_data){
        if(ptr->HasContact(id)){
            return true;
        }
    }
    return false;
}

void QNunchukRoomListModel::newRoom(Room *room)
{
    DBG_INFO << room->name() << (int)room->joinState();
    emit countChanged();
}

void QNunchukRoomListModel::invitedRoom(Room *room, Room *prev)
{
    DBG_INFO << room->name() << (int)room->joinState();
    joinRoom(room->id());
    emit countChanged();
}

void QNunchukRoomListModel::joinedRoom(Room *room, Room *prev)
{
//    connectSingleShot(room, &Room::tagsChanged, this, [this, room] {
//        DBG_INFO << "FIXME Room::tags" << room->name() << (int)room->joinState() << room->tagNames();
//    });
    connectSingleShot(room, &Room::baseStateLoaded, this, [this, room] {
        m_time.stop();
        timeoutHandler(3000, [this, room]() {
            QNunchukRoomPtr newRoom = QNunchukRoomPtr(new QNunchukRoom(room), &QObject::deleteLater);
            doAddRoom(newRoom);
            if( !newRoom.data()->isServerNoticeRoom() && !newRoom.data()->isNunchukSyncRoom()){
                setCurrentIndex(rowCount()-1);
                resort();
                emit countChanged();
            }
        });
    });
    emit countChanged();
}

void QNunchukRoomListModel::leftRoom(Room *room, Room *prev)
{
    DBG_INFO << room->id() << (int)room->joinState();
    if(room && connection()){
        removeRoomById(room->id());
    }
    emit countChanged();
}

void QNunchukRoomListModel::aboutToDeleteRoom(Room *room)
{
    DBG_INFO << room->name() << (int)room->joinState();
    if(room && connection()){
        removeRoomById(room->id());
    }
    emit countChanged();
}

void QNunchukRoomListModel::loadedRoomState(Room *room)
{
    DBG_INFO << "loadedRoomState" << room->name() << (int)room->joinState() << room->tagNames();
    emit countChanged();
}

void QNunchukRoomListModel::refresh(QNunchukRoomPtr room, const QVector<int> &roles)
{
    const auto it = std::find(m_data.begin(), m_data.end(), room);
    if (it == m_data.end()) {
        return;
    }
    const auto idx = index(it - m_data.begin());
    emit dataChanged(idx, idx, roles);
    emit countChanged();
}

void QNunchukRoomListModel::resort()
{
    if (m_data.count() < 2){
        return;
    }
    beginResetModel();
    qSort(m_data.begin(), m_data.end(), sortRoomListByTimeDescending);
    endResetModel();
    setCurrentIndex(m_data.indexOf(currentRoomPtr()));
    emit refreshRoomList();
}

void QNunchukRoomListModel::roomNeedTobeLeaved(const QString &id)
{
    int index = getIndex(id);
    if(index >= 0){
        leaveRoom(index);
    }
}

void QNunchukRoomListModel::synchonizesUserDataFinished()
{
    AppModel::instance()->closePromtNunchukSync();
    AppModel::instance()->startReloadUserDb();
}

bool sortRoomListTimeAscending(const QNunchukRoomPtr &v1, const QNunchukRoomPtr &v2)
{
    return v1.data()->lasttimestamp_timet() < v2.data()->lasttimestamp_timet();
}

bool sortRoomListByTimeDescending(const QNunchukRoomPtr &v1, const QNunchukRoomPtr &v2)
{
    if(v1->roomWallet() != nullptr && v2->roomWallet() != nullptr){
        return v1.data()->lasttimestamp_timet() > v2.data()->lasttimestamp_timet();
    }
    else if(v1->roomWallet() != nullptr){
        return true;
    }
    else if(v2->roomWallet() != nullptr){
        return  false;
    }
    else{
        return v1.data()->lasttimestamp_timet() > v2.data()->lasttimestamp_timet();
    }
}
