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
#include "Draco.h"
#include "localization/STR_CPP.h"
#include <QTextDocumentFragment>

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
    m_IsEncrypted(false)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
    QQmlEngine::setObjectOwnership(m_users.data(), QQmlEngine::CppOwnership);
    m_typingNames.clear();

    if(m_room){
        QQmlEngine::setObjectOwnership(m_room, QQmlEngine::CppOwnership);
    }
    qmlRegisterType<FileTransferInfo>();
    qRegisterMetaType<FileTransferInfo>();
}

QNunchukRoom::~QNunchukRoom()
{

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
                if(isDirectChat() && 0 == QString::compare(user_id, local_id, Qt::CaseInsensitive)){
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
                if(isDirectChat() && 0 == QString::compare(user_id, local_id, Qt::CaseInsensitive)){
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
    if(0 == QString::compare(eventType, NUNCHUK_EVENT_WALLET, Qt::CaseInsensitive)){
        evt = new QNunchukWalletEvent(eventType, content);
    }
    else if(0 == QString::compare(eventType, NUNCHUK_EVENT_TRANSACTION, Qt::CaseInsensitive)){
        evt = new QNunchukTransactionEvent(eventType, content);
    }
    else if(0 == QString::compare(eventType, NUNCHUK_EVENT_SYNC, Qt::CaseInsensitive)){
        evt = new QNunchukSyncEvent(eventType, content);
    }
    else if(0 == QString::compare(eventType, NUNCHUK_EVENT_EXCEPTION, Qt::CaseInsensitive)){
        evt = new QNunchukExceptionEvent(eventType, content);
    }
    else {}
    if(m_room && evt){
        if(this->isNunchukSyncRoom() && !AppSetting::instance()->enableMultiDeviceSync()){
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
            rawtx.data()->setRoomId(this->id());
            rawtx.data()->setInitEventId(QString::fromStdString(room_tx.get_init_event_id()));
            target.data()->setTransaction(rawtx);
        }
    }
    if(!this->isDownloaded()){
        this->startGetPendingTxs();
    }
    if(conversation()){
        conversation()->updateTransaction(cons, target);
        setPinTransaction(conversation()->pinTransaction());
    }
    if(AppModel::instance()->transactionInfo() && target.data()->transaction()
            && (0 == QString::compare(QString::fromStdString(room_tx.get_init_event_id()), target.data()->transaction()->initEventId(), Qt::CaseInsensitive)))
    {
        AppModel::instance()->setTransactionInfo(target.data()->transactionPtr());
    }
}

void QNunchukRoom::slotFinishFinalizeWallet(QString what, int type, int code)
{
    if(type != (int)EWARNING::WarningType::NONE_MSG){
        AppModel::instance()->showToast(code, what, (EWARNING::WarningType)type,  STR_CPP_003);
    }
}

void QNunchukRoom::slotFinishCancelWallet(QString what, int type, int code)
{
    if(type != (int)EWARNING::WarningType::NONE_MSG){
        AppModel::instance()->showToast(code, what, (EWARNING::WarningType)type,  STR_CPP_004);
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
            conversation()->addMessage(cons);
            conversation()->requestSortByTimeAscending();
            this->setLastMessage(cons);
            this->setLasttimestamp(cons);
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
                qDebug() << "fileTransferProgress:" << progress << total;
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
            this->setLastMessage(cons);
            this->setLasttimestamp(cons);
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
        QWarningMessage msgWarning;
        QSingleSignerPtr signer = bridge::nunchukGetUnusedSignerFromMasterSigner(id,
                                                                                 walletType,
                                                                                 (ENUNCHUCK::AddressType)roomWallet()->walletAddressType().toInt(),
                                                                                 msgWarning);
        if(signer && msgWarning.type() == (int)EWARNING::WarningType::NONE_MSG){
            msgWarning.resetWarningMessage();
            matrixbrigde::JoinWallet(this->id(), signer, msgWarning);
            if((int)EWARNING::WarningType::NONE_MSG == msgWarning.type()){
                ret = true;
            }
            else{
                AppModel::instance()->showToast(msgWarning.code(),
                                                msgWarning.what(),
                                                (EWARNING::WarningType)msgWarning.type(),
                                                STR_CPP_006);
            }
        }
        else{
            needXpub = true;
            AppModel::instance()->showToast(msgWarning.code(),
                                            msgWarning.what(),
                                            (EWARNING::WarningType)msgWarning.type(),
                                            STR_CPP_007);
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
        matrixbrigde::JoinWallet(this->id(), signer, msgWarning);
        if((int)EWARNING::WarningType::NONE_MSG == msgWarning.type()){
            ret = true;
        }
        else{
            AppModel::instance()->showToast(msgWarning.code(),
                                            msgWarning.what(),
                                            (EWARNING::WarningType)msgWarning.type(),
                                            STR_CPP_006);
        }
    }
    return ret;
}

bool QNunchukRoom::joinWalletUseSignerFromWalletImport(const QString &id, const QString &xfp)
{
    bool ret = false;
    nunchuk::SingleSigner signer;
    for(nunchuk::SingleSigner s : m_walletImport.get_signers()){
        if((s.get_master_signer_id() != "" && (s.get_master_signer_id() == id.toStdString() || s.get_master_signer_id() == xfp.toStdString()))
          || (s.get_master_fingerprint() != "" && (s.get_master_fingerprint() == id.toStdString() || s.get_master_fingerprint() == xfp.toStdString()))){
            signer = s;
            break;
        }
    }
    QSingleSignerPtr signerPtr = QSingleSignerPtr(new QSingleSigner(signer));
    if(signerPtr && bridge::nunchukHasSinger(signer)){
        QWarningMessage msgWarning;
        matrixbrigde::JoinWallet(this->id(), signerPtr, msgWarning);
        if((int)EWARNING::WarningType::NONE_MSG == msgWarning.type()){
            ret = true;
        }
        else{
            AppModel::instance()->showToast(msgWarning.code(),
                                            msgWarning.what(),
                                            (EWARNING::WarningType)msgWarning.type(),
                                            STR_CPP_006);
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
    if(0 == QString::compare(matrixType, NUNCHUK_EVENT_WALLET, Qt::CaseInsensitive)){
        QJsonObject body = json["body"].toObject();
        cons.matrixType = NUNCHUK_EVENT_WALLET;
        if(0 == QString::compare(msgtype, NUNCHUK_MSG_WALLET_INIT, Qt::CaseInsensitive)){
            cons.init_event_id = init_event_id;
            cons.init_event_json = json;
            bool isCanceled = !roomWallet() || (0 != QString::compare(cons.init_event_id, roomWallet()->get_init_event_id(), Qt::CaseInsensitive));
            cons.messageType = isCanceled  ? (int)ENUNCHUCK::ROOM_EVT::WALLET_PAST : (int)ENUNCHUCK::ROOM_EVT::WALLET_INIT;
            cons.message = QString(QJsonDocument(body).toJson(QJsonDocument::Compact));
        }
        else if(0 == QString::compare(msgtype, NUNCHUK_MSG_WALLET_JOIN, Qt::CaseInsensitive)){
            QString xfp = body["key"].toString().split('/')[0].remove('[');
            QJsonObject init_event = body["io.nunchuk.relates_to"].toObject()["init_event"].toObject();
            QString wallet_name = init_event["content"].toObject()["body"].toObject()["name"].toString();
            QString init_event_id = init_event["event_id"].toString();
            cons.message = STR_CPP_011.arg(xfp).arg(wallet_name);
            cons.init_event_id = init_event_id;
            cons.messageType = (int)ENUNCHUCK::ROOM_EVT::WALLET_JOIN;
        }
        else if(0 == QString::compare(msgtype, NUNCHUK_MSG_WALLET_LEAVE, Qt::CaseInsensitive)){
            QJsonObject io_nunchuk_relates_to = body["io.nunchuk.relates_to"].toObject();
            QString join_event_id = io_nunchuk_relates_to["join_event_id"].toString();
            QJsonObject init_event = io_nunchuk_relates_to["init_event"].toObject();
            QString wallet_name = init_event["content"].toObject()["body"].toObject()["name"].toString();
            QString init_event_id = init_event["event_id"].toString();
            QString xfp = "";
            QWarningMessage joinmsg;
            QNunchukMatrixEvent nunJoinEvent = matrixbrigde::GetEvent(this->id(), join_event_id, joinmsg);
            if((int)EWARNING::WarningType::NONE_MSG == joinmsg.type()){
                QJsonObject joinjson = matrixbrigde::stringToJson(nunJoinEvent.get_content());
                xfp = joinjson["body"].toObject()["key"].toString().split('/')[0].remove('[');
            }
            cons.message = STR_CPP_012.arg(xfp).arg(wallet_name);
            cons.init_event_id = init_event_id;
            cons.messageType = (int)ENUNCHUCK::ROOM_EVT::WALLET_LEAVE;
        }
        else if(0 == QString::compare(msgtype, NUNCHUK_MSG_WALLET_READY, Qt::CaseInsensitive)){
            QJsonObject init_event = body["io.nunchuk.relates_to"].toObject()["init_event"].toObject();
            QString wallet_name = init_event["content"].toObject()["body"].toObject()["name"].toString();
            QString init_event_id = init_event["event_id"].toString();
            cons.message = STR_CPP_013.arg(wallet_name);
            cons.init_event_id = init_event_id;
            cons.messageType = (int)ENUNCHUCK::ROOM_EVT::WALLET_READY;
        }
        else if(0 == QString::compare(msgtype, NUNCHUK_MSG_WALLET_CREATE, Qt::CaseInsensitive)){
            QJsonObject init_event = body["io.nunchuk.relates_to"].toObject()["init_event"].toObject();
            QString wallet_name = init_event["content"].toObject()["body"].toObject()["name"].toString();
            QString init_event_id = init_event["event_id"].toString();
            cons.message = STR_CPP_014.arg(wallet_name);
            cons.init_event_id = init_event_id;
            cons.messageType = (int)ENUNCHUCK::ROOM_EVT::WALLET_CREATE;
        }
        else if(0 == QString::compare(msgtype, NUNCHUK_MSG_WALLET_CANCEL, Qt::CaseInsensitive)){
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
    else if(0 == QString::compare(matrixType, NUNCHUK_EVENT_TRANSACTION, Qt::CaseInsensitive)){
        QJsonObject body = json["body"].toObject();
        cons.matrixType = NUNCHUK_EVENT_TRANSACTION;
        if(0 == QString::compare(msgtype, NUNCHUK_MSG_TX_INIT, Qt::CaseInsensitive)){
            cons.init_event_id = init_event_id;
            cons.message = QString(QJsonDocument(body).toJson(QJsonDocument::Compact));
            cons.messageType = (int)ENUNCHUCK::ROOM_EVT::TX_INIT;
        }
        else if(0 == QString::compare(msgtype, NUNCHUK_MSG_TX_SIGN, Qt::CaseInsensitive)){
            QString xfp = body["master_fingerprint"].toString();
            QJsonObject init_event = body["io.nunchuk.relates_to"].toObject()["init_event"].toObject();
            cons.message = STR_CPP_016.arg(xfp);
            cons.messageType = (int)ENUNCHUCK::ROOM_EVT::TX_SIGN;
            cons.init_event_id = init_event["event_id"].toString();
            cons.init_event_json = json;
        }
        else if(0 == QString::compare(msgtype, NUNCHUK_MSG_TX_BROADCAST, Qt::CaseInsensitive)){
            QJsonObject init_event = body["io.nunchuk.relates_to"].toObject()["init_event"].toObject();
            cons.message = STR_CPP_017;
            cons.messageType = (int)ENUNCHUCK::ROOM_EVT::TX_BROADCAST;
            cons.init_event_id = init_event["event_id"].toString();
            cons.init_event_json = json;
        }
        else if(0 == QString::compare(msgtype, NUNCHUK_MSG_TX_READY, Qt::CaseInsensitive)){
            QJsonObject init_event = body["io.nunchuk.relates_to"].toObject()["init_event"].toObject();
            cons.message = STR_CPP_018;
            cons.messageType = (int)ENUNCHUCK::ROOM_EVT::TX_READY;
            cons.init_event_id = init_event["event_id"].toString();
            cons.init_event_json = json;
        }
        else if(0 == QString::compare(msgtype, NUNCHUK_MSG_TX_CANCEL, Qt::CaseInsensitive)){
            QJsonObject init_event = body["io.nunchuk.relates_to"].toObject()["init_event"].toObject();
            cons.message = STR_CPP_019;
            cons.messageType = (int)ENUNCHUCK::ROOM_EVT::TX_CANCEL;
            cons.init_event_id = init_event["event_id"].toString();
            cons.init_event_json = json;
        }
        else if(0 == QString::compare(msgtype, NUNCHUK_MSG_TX_RECEIVE, Qt::CaseInsensitive)){
            if(matrixbrigde::HasRoomWallet(this->id()) == false) return false;
            QWarningMessage roomTxWarning;
            QString tx_id = matrixbrigde::GetTransactionId(this->id(), init_event_id, roomTxWarning);
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
        downloadTransactionThread(cons, this->id());
    }
    else if(0 == QString::compare(matrixType, NUNCHUK_EVENT_EXCEPTION, Qt::CaseInsensitive)){
        QJsonObject body = json["body"].toObject();
        cons.matrixType = NUNCHUK_EVENT_EXCEPTION;
        if(0 == QString::compare(msgtype, NUNCHUK_EVENT_EXCEPTION, Qt::CaseInsensitive)){
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

void QNunchukRoom::downloadTransactionThread(Conversation cons, const QString &roomid)
{
    if(roomWallet() && roomWallet()->get_wallet_id() != ""){
        if(cons.init_event_id != ""){ // FIXME FOR CHECK DUP RECIEVED TX EVT
            QtConcurrent::run([this, cons, roomid]() {
                if(cons.messageType == (int)ENUNCHUCK::ROOM_EVT::TX_RECEIVE){
                    QString wallet_id = this->roomWallet() ? this->roomWallet()->get_wallet_id() : "";
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
                    QWarningMessage roomTxWarning;
                    nunchuk::RoomTransaction room_tx = matrixbrigde::GetOriginRoomTransaction(roomid,
                                                                                              cons.init_event_id,
                                                                                              roomTxWarning);
                    if((int)EWARNING::WarningType::NONE_MSG == roomTxWarning.type() && room_tx.get_wallet_id() != ""){
                        QWarningMessage txWarning;
                        room_tx.set_room_id(id().toStdString());
                        nunchuk::Transaction tx = bridge::nunchukGetOriginTransaction(QString::fromStdString(room_tx.get_wallet_id()),
                                                                                      QString::fromStdString(room_tx.get_tx_id()),
                                                                                      txWarning);
                        emit signalFinishedDownloadTransaction(room_tx, tx, cons);
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
            matrixbrigde::LeaveWallet(this->id(), join_id, STR_CPP_008, msgWarning);
            if((int)EWARNING::WarningType::NONE_MSG == msgWarning.type()){
                ret = true;
            }
            else{
                AppModel::instance()->showToast(msgWarning.code(),
                                                msgWarning.what(),
                                                (EWARNING::WarningType)msgWarning.type(),
                                                STR_CPP_009);
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
        AppModel::instance()->showToast(warningmsg.code(),
                                        warningmsg.what(),
                                        (EWARNING::WarningType)warningmsg.type(),
                                        STR_CPP_007);
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
            QQuickViewer::instance()->sendEvent(E::EVT_ONLINE_ONS_CLOSE_REQUEST, E::STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET);
        }
    }
    else{
        QQuickViewer::instance()->sendEvent(E::EVT_ONLINE_ONS_CLOSE_REQUEST, E::STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET);
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
    if(!m_room) return;
    else{
        if(isServerNoticeRoom()){
            // TODO
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
                    if(!roomWallet()){
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
                    this->setLastMessage(conversation()->lastMessage());
                    this->setLasttimestamp(conversation()->lastTime());
                    if(conversation()->lastIndex() == 0){
                        if(allHisLoaded() == false){
                            getMoreContents(10);
                        }
                    }

                    if(roomWallet()){
                        AppModel::instance()->requestSyncWalletDb(roomWallet()->get_wallet_id());
                        bool isCreator = conversation()->isWalletCreator(roomWallet()->get_init_event_id());
                        roomWallet()->setIsCreator(isCreator);
                    }
                }
                this->startGetPendingTxs();
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
        this->downloadHistorical();
    }
}

void QNunchukRoom::connectRoomServiceSignals()
{
    if(m_room){
        connect(this,   &QNunchukRoom::noticeService, ClientController::instance(), &ClientController::refreshContacts);
        connect(m_room, &Room::addedMessages, ClientController::instance(), &ClientController::refreshContacts);
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
    return (0 == QString::compare(senderUserId, localUserId, Qt::CaseInsensitive));
}

QList<DracoUser> QNunchukRoom::getNunchukMembers() const
{
    return m_nunchukMembers;
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
        if(0 == QString::compare(my_id, id_input, Qt::CaseInsensitive)){
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

QString QNunchukRoom::getNunchukMemberChatId(const QString &input)
{
    return getNunchukMemberUser(input).chat_id;
}

QString QNunchukRoom::getNunchukMemberName(const QString &input)
{
    return getNunchukMemberUser(input).name;
}

QString QNunchukRoom::getNunchukMemberEmail(const QString &input)
{
    return getNunchukMemberUser(input).email;
}

QString QNunchukRoom::getNunchukMemberAvatar(const QString &input)
{
    return getNunchukMemberUser(input).avatar;
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
    this->startGetPendingTxs();
}

void QNunchukRoom::updateTransactionMemo(const QString &tx_id, const QString &memo)
{
    if(conversation()){
        conversation()->updateTransactionMemo(tx_id, memo);
    }
}

void QNunchukRoom::startGetPendingTxs()
{
    if(matrixbrigde::HasRoomWallet(this->id())){
        QtConcurrent::run([this]() {
            QRoomTransactionModelPtr ret = matrixbrigde::GetPendingTransactions(this->id());
            emit this->signalFinishedGetPendingTxs(ret);
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
        if(0 == QString::compare(cons.matrixType, NUNCHUK_ROOM_ENCRYPTED, Qt::CaseInsensitive)){
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
    if(isServerNoticeRoom()){
        emit noticeService();
    }
    else if(isNunchukSyncRoom()){
        QtConcurrent::run([=]() {
            for (auto e = m_room->messageEvents().rbegin(); e != m_room->messageEvents().rend(); ++e){
                if(fromIndex <= e->index() &&  toIndex >= e->index()){
                    const RoomEvent* lastEvent = e->get();
                    nunchukConsumeSyncEvent(*lastEvent);
                }
            }
            AppModel::instance()->startReloadWallets();
            AppModel::instance()->startReloadMasterSigners();
            AppModel::instance()->startReloadRemoteSigners();
        });
    }
    else{
        DBG_INFO << fromIndex << toIndex << m_room->maxTimelineIndex() <<  m_room->minTimelineIndex();
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
            auto subjectName = this->m_room->user(e.userId())->displayname(room());
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
                        this->updateCancelWallet(cons.init_event_id);
                    }
                    if(cons.messageType == (int)ENUNCHUCK::ROOM_EVT::TX_CANCEL){
                        this->updateCancelTransaction(cons);
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
        this->setLastMessage(conversation()->lastMessage());
        this->setLasttimestamp(conversation()->lastTime());
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
    QString matrixType = evt.matrixType();
    cons.matrixType = matrixType;
    if((0 == QString::compare(matrixType, NUNCHUK_ROOM_MESSAGE, Qt::CaseInsensitive))
            || (0 == QString::compare(matrixType, NUNCHUK_ROOM_MEMBER, Qt::CaseInsensitive))
            || (0 == QString::compare(matrixType, NUNCHUK_ROOM_NAME, Qt::CaseInsensitive))
            || (0 == QString::compare(matrixType, NUNCHUK_ROOM_ENCRYPTED, Qt::CaseInsensitive))
            || (0 == QString::compare(matrixType, NUNCHUK_ROOM_ENCRYPTION, Qt::CaseInsensitive)))
    {
        eventToConversation(evt, cons, Qt::RichText);
    }
    else if((0 == QString::compare(matrixType, NUNCHUK_EVENT_WALLET, Qt::CaseInsensitive)) ||
             (0 == QString::compare(matrixType, NUNCHUK_EVENT_TRANSACTION, Qt::CaseInsensitive)) ||
             (0 == QString::compare(matrixType, NUNCHUK_EVENT_EXCEPTION, Qt::CaseInsensitive)))
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
    if((0 == QString::compare(NUNCHUK_EVENT_WALLET, matrixType, Qt::CaseInsensitive))
            || (0 == QString::compare(NUNCHUK_EVENT_TRANSACTION, matrixType, Qt::CaseInsensitive))){
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
        if(0 == QString::compare(NUNCHUK_EVENT_SYNC, matrixType, Qt::CaseInsensitive)){
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
            wallet.set_escrow(is_escrow);
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
        if(0 == QString::compare(id, r.data()->id(), Qt::CaseInsensitive)){
            return r;
        }
    }
    for(QNunchukRoomPtr r : m_servive){
        if(0 == QString::compare(id, r.data()->id(), Qt::CaseInsensitive)){
            return r;
        }
    }
    return NULL;
}

int QNunchukRoomListModel::getIndex(const QString &id) const
{
    for(int i = 0; i < m_data.size(); i++){
        QNunchukRoomPtr r = m_data.at(i);
        if(0 == QString::compare(id, r.data()->id(), Qt::CaseInsensitive)){
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
                synchonizesUserData();
                emit finishedDownloadRoom();
                CLIENT_INSTANCE->setReadySupport(true);
                downloadRoomWallets();
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

void QNunchukRoomListModel::setCurrentRoom(const QNunchukRoomPtr &currentRoom)
{
    if(m_currentRoom != currentRoom){
        m_currentRoom = currentRoom;
        if(m_currentRoom){
            m_currentRoom.data()->setDisplayed(true);
            if(m_currentRoom->conversation()){
                if(m_currentRoom->conversation()->unreadLastIndex() + 10 > m_currentRoom->conversation()->count()){
                    m_currentRoom.data()->conversation()->setCurrentIndex(m_currentRoom.data()->conversation()->rowCount() - 1);
                    m_currentRoom->markAllMessagesAsRead();
                }else{
                    m_currentRoom.data()->conversation()->setCurrentIndex(m_currentRoom->conversation()->unreadLastIndex());
                }
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
        if(it && it.data()->roomWallet() && (0 == QString::compare(it.data()->roomWallet()->get_wallet_id(), wallet_id, Qt::CaseInsensitive))){
            return it.data()->id();
        }
    }
    return "";
}

void QNunchukRoomListModel::transactionChanged(const QString &wallet_id, const QString &tx_id, const int status, const int height)
{
    for( QNunchukRoomPtr it: m_data){
        if(it && it.data()->roomWallet() && (0 == QString::compare(it.data()->roomWallet()->get_wallet_id(), wallet_id, Qt::CaseInsensitive))){
            it.data()->transactionChanged(tx_id, status, height);
        }
    }
}

void QNunchukRoomListModel::updateTransactionMemo(const QString& wallet_id, const QString &tx_id, const QString &memo)
{
    for( QNunchukRoomPtr r: m_data){
        if(r && r.data()->roomWallet() && (0 == QString::compare(wallet_id, r.data()->roomWallet()->get_wallet_id(), Qt::CaseInsensitive))){
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
            }
            if(r.data()->isNunchukSyncRoom()){
                r.data()->connectRoomSignals();
            }
        }
    }
    else{
        if(!r.data()->id().isEmpty() && !containsRoom(r.data()->id()) ){
            QList<DracoUser> users = Draco::instance()->getRoomMembers(r.data()->id());
            beginInsertRows(QModelIndex(), rowCount(), rowCount());
            m_data.append(r);
            endInsertRows();
            r.data()->setNunchukMembers(users);
            r.data()->connectRoomSignals();
            connect(r.data(),         &QNunchukRoom::roomNameChanged,       this, [this, r] { refresh(r); });
            connect(r.data(),         &QNunchukRoom::lastMessageChanged,    this, [this, r] { refresh(r); });
            connect(r.data(),         &QNunchukRoom::lasttimestampChanged,  this, [this, r] { resort(); });
            connect(r.data(),         &QNunchukRoom::roomNeedTobeLeaved,    this, &QNunchukRoomListModel::roomNeedTobeLeaved);
            connect(r.data()->room(), &Room::unreadMessagesChanged,         this, [this, r] { refresh(r); });
            connect(r.data()->room(), &Room::typingChanged,                 this, [this, r] { refresh(r); });
            connect(r.data()->room(), &Room::unreadMessagesChanged,         this, &QNunchukRoomListModel::totalUnreadChanged);
        }
    }
    emit this->countChanged();
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
        if(it && (0 == QString::compare(it.data()->id(), id, Qt::CaseInsensitive))){
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
    auto* job = currentRoom()->room()->leaveRoom();
    connect(job, &BaseJob::success, this, [this] {
        removeRoomByIndex(currentIndex());
        AppModel::instance()->startReloadWallets();
        AppModel::instance()->startReloadMasterSigners();
        AppModel::instance()->startReloadRemoteSigners();
    });
    emit countChanged();
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

void QNunchukRoomListModel::createRoomChat(const QStringList invitees_id, const QString& topic, const QString& name, QVariant firstMessage)
{
    if(connection()){
        Connection::RoomVisibility in_visibility = Connection::UnpublishRoom;
        const QString   in_alias = {};
        const QString   in_name = name;
        const QString   in_topic = topic;
        QStringList     in_invites = invitees_id;
        const QString   in_presetName = {};
        const QString   in_roomVersion = {};
        bool            in_isDirect = false;
        CreateRoomJob::StateEvent stateEvt;
        stateEvt.type = "m.room.encryption";
        stateEvt.content["algorithm"] = "m.megolm.v1.aes-sha2";
        const QVector<CreateRoomJob::StateEvent> in_initialState = {stateEvt};

        auto createJob = connection()->createRoom(in_visibility,
                                                  in_alias,
                                                  in_name,
                                                  in_topic,
                                                  in_invites,
                                                  in_presetName,
                                                  in_roomVersion,
                                                  in_isDirect,
                                                  in_initialState/*,
                                                  in_invite3pids,
                                                  in_creationContent*/);
        connect(createJob, &BaseJob::success, this, [this, createJob, firstMessage] {
            if(!firstMessage.isNull() && firstMessage.toString() != ""){
                connection()->room(createJob->roomId())->postPlainText(firstMessage.toString());
            }
        });
        connect(createJob, &BaseJob::failure, this, [createJob] {
            DBG_INFO << "//FIXME Failed to create the room";
            AppModel::instance()->showToast(createJob->error(),
                                            "Failed to create the room",
                                            EWARNING::WarningType::EXCEPTION_MSG,
                                            createJob->errorString());
        });
    }
    emit countChanged();
}

void QNunchukRoomListModel::createSupportRoom()
{
    if((int)ENUNCHUCK::Chain::MAIN == (int)AppSetting::instance()->primaryServer()
            || (int)ENUNCHUCK::Chain::TESTNET == (int)AppSetting::instance()->primaryServer())
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
                AppModel::instance()->showToast(createJob->error(),
                                                createJob->errorString(),
                                                EWARNING::WarningType::EXCEPTION_MSG,
                                                "Failed to create support room");
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

bool QNunchukRoomListModel::containsRoom(const QString &id)
{
    foreach (QNunchukRoomPtr it, m_data) {
        if(0 == QString::compare(it.data()->id(), id, Qt::CaseInsensitive)){
            return true;
        }
    }
    return false;
}

bool QNunchukRoomListModel::containsServiceRoom(const QString &id)
{
    foreach (QNunchukRoomPtr it, m_servive) {
        if(0 == QString::compare(it.data()->id(), id, Qt::CaseInsensitive)){
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
    beginResetModel();
    qSort(m_data.begin(), m_data.end(), sortRoomListByTimeDescending);
    endResetModel();
    setCurrentIndex(m_data.indexOf(currentRoomPtr()));
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
    AppModel::instance()->startReloadWallets();
    AppModel::instance()->startReloadMasterSigners();
    AppModel::instance()->startReloadRemoteSigners();
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
