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
#include "csapi/inviting.h"
#include "csapi/kicking.h"
#include "csapi/typing.h"
#include "csapi/directory.h"
#include "csapi/room_send.h"
#include "csapi/room_state.h"
#include "csapi/tags.h"
#include "events/roomevent.h"
#include "events/reactionevent.h"
#include "events/redactionevent.h"
#include "events/simplestateevents.h"
#include "events/roommessageevent.h"
#include "events/stateevent.h"
#include "events/roompowerlevelsevent.h"
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
#include "QAppEngine/QEventProcessor/Common/WorkerThread.h"
#include "Premiums/QSharedWallets.h"
#include <QCryptographicHash>
#include <QPointer>
#include <chrono>
#include <utility>
#include <vector>

namespace {
constexpr auto NUNCHUK_SUPPORT_MARKER_EVENT = "io.nunchuk.support_room";

struct DownloadTransactionResult {
    bool shouldNotify{false};
    nunchuk::RoomTransaction roomTransaction;
    nunchuk::Transaction transaction;
};

struct PendingTransactionData {
    nunchuk::RoomTransaction roomTransaction;
    nunchuk::Transaction transaction;
};

using PendingTransactionDataList = std::vector<PendingTransactionData>;

struct HistoricalEventSnapshot {
    QJsonObject fullJson;
    bool shouldConsume{false};
    nunchuk::NunchukMatrixEvent consumeEvent;
    NunchukEventBackendResolution backendResolution;
};

using HistoricalEventSnapshotList = std::vector<HistoricalEventSnapshot>;

QRoomTransactionModelPtr buildPendingTransactionModel(
        const QString &roomId,
        const PendingTransactionDataList &transactions)
{
    QRoomTransactionModelPtr result(new QRoomTransactionModel());
    for (const PendingTransactionData &item : transactions) {
        const QString walletId = QString::fromStdString(
                    item.roomTransaction.get_wallet_id());
        QTransactionPtr transaction = bridge::convertTransaction(
                    item.transaction, walletId);
        if (!transaction) {
            continue;
        }

        transaction->setRoomId(roomId);
        transaction->setInitEventId(QString::fromStdString(
                    item.roomTransaction.get_init_event_id()));
        QRoomTransactionPtr target(new QRoomTransaction(item.roomTransaction));
        target->setTransaction(transaction);
        const int status = target->transaction()
                ? target->transaction()->status()
                : (int)ENUNCHUCK::TransactionStatus::NETWORK_REJECTED;
        if (status == (int)ENUNCHUCK::TransactionStatus::PENDING_SIGNATURES
                || status == (int)ENUNCHUCK::TransactionStatus::READY_TO_BROADCAST
                || status == (int)ENUNCHUCK::TransactionStatus::PENDING_CONFIRMATION) {
            result->addTransaction(target);
        }
    }
    return result;
}

QString currentSupportRoomTag()
{
    return (int)ENUNCHUCK::Chain::MAIN == (int)AppSetting::instance()->primaryServer()
            ? NUNCHUK_ROOM_SUPPORT : NUNCHUK_ROOM_SUPPORTTESTNET;
}

QString supportRoomAliasLocalpart(const Connection* connection,
                                  const QString& tagname)
{
    // This key must be stable across devices and token rotations so concurrent
    // clients of the same account still compete for one server-side alias.
    // The alias is never trusted as room identity on its own; only the owned,
    // versioned marker created atomically with the room is authoritative.
    const QByteArray identity = connection->userId().toUtf8() + '|'
            + tagname.toUtf8();
    const QByteArray digest = QCryptographicHash::hash(
        identity, QCryptographicHash::Sha256).toHex().left(24);
    return QStringLiteral("nunchuk-support-") + QString::fromLatin1(digest);
}

QString supportRoomAlias(const Connection* connection, const QString& tagname)
{
    return QStringLiteral("#") + supportRoomAliasLocalpart(connection, tagname)
            + QStringLiteral(":") + connection->domain();
}

QString supportMarkerTag(const Room* room)
{
    if(!room || !room->creation()
            || room->creation()->senderId() != room->connection()->userId()){
        return {};
    }
    const StateEvent* markerEvent = room->currentState().get(
        QString::fromLatin1(NUNCHUK_SUPPORT_MARKER_EVENT));
    if(!markerEvent
            || markerEvent->senderId() != room->connection()->userId()){
        return {};
    }
    const QJsonObject marker = markerEvent->contentJson();
    if(marker.value("version").toInt() != 1){
        return {};
    }
    return marker.value("tag").toString();
}

QString supportTagForRoom(const Room* room)
{
    if(!room){
        return {};
    }
    const QString markerTag = supportMarkerTag(room);
    if(markerTag == NUNCHUK_ROOM_SUPPORT
            || markerTag == NUNCHUK_ROOM_SUPPORTTESTNET){
        return markerTag;
    }
    const QString canonicalTag =
            room->property("nunchukSupportCanonicalTag").toString();
    if(canonicalTag == NUNCHUK_ROOM_SUPPORT
            || canonicalTag == NUNCHUK_ROOM_SUPPORTTESTNET){
        return canonicalTag;
    }
    const bool hasMainTag = room->tagNames().contains(NUNCHUK_ROOM_SUPPORT);
    const bool hasTestnetTag =
            room->tagNames().contains(NUNCHUK_ROOM_SUPPORTTESTNET);
    if(hasMainTag && hasTestnetTag){
        return currentSupportRoomTag();
    }
    if(hasMainTag){
        return NUNCHUK_ROOM_SUPPORT;
    }
    if(hasTestnetTag){
        return NUNCHUK_ROOM_SUPPORTTESTNET;
    }
    return {};
}

bool isAmbiguousMutationError(int errorCode)
{
    return errorCode == BaseJob::Abandoned
            || errorCode == BaseJob::NetworkError
            || errorCode == BaseJob::Timeout
            || errorCode == BaseJob::IncorrectResponse
            || errorCode == BaseJob::UnexpectedResponseType;
}
}

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
    m_typingIdleTimer.setSingleShot(true);
    m_typingIdleTimer.setInterval(4500);
    connect(&m_typingIdleTimer, &QTimer::timeout, this, [this] {
        setTyping(false);
    });
    qRegisterMetaType<FileTransferInfo>();
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
    const QString tagname = currentSupportRoomTag();
    if(!m_room){
        return false;
    }
    const QString otherTag = qUtils::strCompare(tagname, NUNCHUK_ROOM_SUPPORT)
            ? NUNCHUK_ROOM_SUPPORTTESTNET : NUNCHUK_ROOM_SUPPORT;
    const QString markerTag = supportMarkerTag(m_room);
    const bool isSupportDirectChat = m_room->isDirectChat()
            && m_room->connection()->directChatMemberIds(m_room).contains("@support:nunchuk.io");
    if(m_room->property("nunchukSupportSuppressedTag").toString() == tagname){
        return false;
    }
    if(markerTag == tagname){
        return true;
    }
    if(markerTag == otherTag){
        return false;
    }
    if(m_room->tagNames().contains(tagname)){
        return true;
    }
    if(m_room->tagNames().contains(otherTag)){
        return false;
    }
    for(const Quotient::Room* taggedRoom : m_room->connection()->roomsWithTag(tagname)){
        if(taggedRoom && taggedRoom->joinState() != JoinState::Leave
                && taggedRoom->property("nunchukSupportSuppressedTag").toString() != tagname){
            return false;
        }
    }
    return isSupportDirectChat
            && m_room->property("nunchukSupportCanonicalTag").toString() == tagname;
}

bool QNunchukRoom::isAnySupportRoom() const
{
    if(!m_room){
        return false;
    }
    const QString markerTag = supportMarkerTag(m_room);
    const QString canonicalTag =
            m_room->property("nunchukSupportCanonicalTag").toString();
    return markerTag == NUNCHUK_ROOM_SUPPORT
            || markerTag == NUNCHUK_ROOM_SUPPORTTESTNET
            || m_room->tagNames().contains(NUNCHUK_ROOM_SUPPORT)
            || m_room->tagNames().contains(NUNCHUK_ROOM_SUPPORTTESTNET)
            || canonicalTag == NUNCHUK_ROOM_SUPPORT
            || canonicalTag == NUNCHUK_ROOM_SUPPORTTESTNET;
}

bool QNunchukRoom::isDirectChat() const
{
    return m_room ? m_room->isDirectChat() : false;
}

bool QNunchukRoom::isByzantineRoom() const
{
    bool ret = (m_room ? m_room->currentState().contains(NUNCHUK_ROOM_BYZANTINE) : false );
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
    if(m_room) return m_room->localMember().displayName();
    else return "";
}

QString QNunchukRoom::localUserId() const
{
    return m_room ? m_room->localMember().id() : QString{};
}

bool QNunchukRoom::canRenameRoom() const
{
    return m_room && m_room->joinState() == JoinState::Join
            && m_room->memberEffectivePowerLevel()
               >= m_room->powerLevelFor(RoomNameEvent::TypeId, true);
}

bool QNunchukRoom::canInviteMembers() const
{
    if(!m_room || m_room->joinState() != JoinState::Join){
        return false;
    }
    const auto* powerLevels = m_room->currentState().get<RoomPowerLevelsEvent>();
    return powerLevels
            && m_room->memberEffectivePowerLevel() >= powerLevels->invite();
}

bool QNunchukRoom::canKickMembers() const
{
    if(!m_room || m_room->joinState() != JoinState::Join){
        return false;
    }
    const auto* powerLevels = m_room->currentState().get<RoomPowerLevelsEvent>();
    return powerLevels
            && m_room->memberEffectivePowerLevel() >= powerLevels->kick();
}

bool QNunchukRoom::canKickMember(const QString& memberId) const
{
    if(!canKickMembers() || memberId.isEmpty() || memberId == localUserId()){
        return false;
    }
    return m_room->memberEffectivePowerLevel() > m_room->memberEffectivePowerLevel(memberId);
}

bool QNunchukRoom::roomNameChangeInProgress() const
{
    return m_roomNameChangeInProgress;
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
        return m_room->joinedMembers().count();
    }
    else{
        return 0;
    }
}

QStringList QNunchukRoom::userNames()
{
    QStringList ret;
    ret.clear();
    if(!m_room){
        return ret;
    }
    for (const RoomMember& member : m_room->joinedMembers()) {
        ret << member.displayName();
    }
    ret.count();
    return ret;
}

QStringList QNunchukRoom::talkersName()
{
    QStringList ret;
    ret.clear();
    if(m_room){
        QString local_id = room()->localMember().id();
        const auto members = m_room->joinedMembers();
        for (int i = 0; i < members.count(); ++i) {
            QString user_id = members.at(i).id();
            if(isDirectChat() && qUtils::strCompare(user_id, local_id)){
                continue;
            }
            else{
                ret.append(members.at(i).name());
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
        QString local_id = room()->localMember().id();
        const auto members = m_room->joinedMembers();
        for (int i = 0; i < members.count(); ++i) {
            QString user_id = members.at(i).id();
            if(isDirectChat() && qUtils::strCompare(user_id, local_id)){
                continue;
            }
            else{
                ret.append(members.at(i).avatarMediaId());
            }
        }
    }
    return ret;
}

QString QNunchukRoom::roomAvatar()
{
    if(m_room && userCount() == 2){
        const auto members = m_room->joinedMembers();
        for(const RoomMember& member : members){
            if(member.id() != m_room->localMember().id()){
                return member.avatarMediaId();
            }
        }
    }
    return "";
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
            return "Support room";
        }
        else if(isNunchukByzantineRoom()){
            return m_room->name() != "" ? m_room->name() : m_room->displayName();
        }
        else if(isDirectChat()){
            return m_room->name() != "" ? m_room->name() : m_room->displayName() != "" ? m_room->displayName() : userNames().join(", ");
        }
        else{
            if(userCount() == 2){
                int targetId = userNames().indexOf(m_room->localMember().name()) == 0 ? 1 : 0 ;
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
    const QString requestedName = name.trimmed();
    if(!m_room || m_roomNameChangeInProgress || requestedName.isEmpty()
            || requestedName == m_room->name()){
        return;
    }
    if(!canRenameRoom()){
        const QString error = tr("You do not have permission to rename this room");
        AppModel::instance()->showToast(BaseJob::Unauthorised, error,
                                        EWARNING::WarningType::EXCEPTION_MSG);
        emit roomNameChangeFailed(error);
        return;
    }
    m_roomNameChangeInProgress = true;
    emit roomNameChangeInProgressChanged();
    auto* job = m_room->setState<RoomNameEvent>(requestedName);
    const auto finish = [this] {
        if(m_roomNameChangeInProgress){
            m_roomNameChangeInProgress = false;
            emit roomNameChangeInProgressChanged();
        }
    };
    connect(job, &BaseJob::finished, this, [this, job, finish] {
        if(job->error() == BaseJob::Abandoned){
            finish();
            AppModel::instance()->showToast(job->error(), job->errorString(),
                                            EWARNING::WarningType::EXCEPTION_MSG);
            emit roomNameChangeFailed(job->errorString());
        }
    });
    connect(job, &BaseJob::success, this, [this, finish] {
        finish();
        emit roomNameChangeSucceeded();
    });
    connect(job, &BaseJob::failure, this, [this, job, finish] {
        finish();
        AppModel::instance()->showToast(job->error(), job->errorString(),
                                        EWARNING::WarningType::EXCEPTION_MSG);
        emit roomNameChangeFailed(job->errorString());
    });
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
        return m_room->notificationCount() > 0 ? m_room->notificationCount() : 0;
    }
    return 0;
}

QString QNunchukRoom::postEvent(const QString& eventType, const QJsonObject& content)
{
    QString txnId;
    RoomEvent* evt = nullptr;
    QJsonObject json{
        { "type", eventType },
        { "content", content }
    };

    if (qUtils::strCompare(eventType, NUNCHUK_EVENT_WALLET)) {
        evt = new QNunchukWalletEvent(json);
    }
    else if (qUtils::strCompare(eventType, NUNCHUK_EVENT_TRANSACTION)) {
        evt = new QNunchukTransactionEvent(json);
    }
    else if (qUtils::strCompare(eventType, NUNCHUK_EVENT_SYNC)) {
        evt = new QNunchukSyncEvent(json);
    }
    else if (qUtils::strCompare(eventType, NUNCHUK_EVENT_EXCEPTION)) {
        evt = new QNunchukExceptionEvent(json);
    }

    if (m_room && evt) {
        if (isNunchukSyncRoom()
            && !AppSetting::instance()->enableMultiDeviceSync()) {
            delete evt;
            return "";
        }
        txnId = m_room->postEvent(evt);
        if (validatePendingEvent(txnId))
            return txnId;
    }

    delete evt;
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
            cons.sender   = m_room->localMember().displayName() != "" ? m_room->localMember().displayName() : m_room->localMember().id();
            cons.receiver =  m_room->localMember().displayName();
            cons.timestamp = QDateTime::currentDateTime().toMSecsSinceEpoch();
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
    if (!m_room) {
        return;
    }

    const QString filepath = qUtils::QGetFilePath(localFile);
    if (filepath.isEmpty()) {
        return;
    }

    int file_mimeType = (int)ENUNCHUCK::ROOM_EVT::FILE_OTHER;

    QMimeDatabase db;
    QMimeType mime = db.mimeTypeForFile(filepath);
    const QString file_caption = QFileInfo(localFile).fileName();

    if (mime.name().startsWith("image/")) {
        file_mimeType = (int)ENUNCHUCK::ROOM_EVT::FILE_IMAGE;
    } else if (mime.name().startsWith("video/")) {
        file_mimeType = (int)ENUNCHUCK::ROOM_EVT::FILE_VIDEO;
    } else {
        file_mimeType = (int)ENUNCHUCK::ROOM_EVT::FILE_OTHER;
    }

    auto fileContent = std::make_unique<EventContent::FileContent>(
        QUrl::fromLocalFile(filepath));

    const QString txnId = m_room->postFile(
        description.isEmpty() ? QFileInfo(filepath).fileName() : description,
        std::move(fileContent));

    QObject::connect(m_room, &Room::fileTransferCompleted,
                     [=](QString id, QUrl fileurl, FileSourceInfo fileinfo) {
                         if (id == txnId) {
                             DBG_INFO << "fileTransferCompleted";
                         }
                     });

    QObject::connect(m_room, &Room::fileTransferFailed,
                     [=](QString id, QString error) {
                         if (id == txnId) {
                             DBG_INFO << "fileTransferFailed";
                         }
                     });

    QObject::connect(m_room, &Room::fileTransferProgress,
                     [=](QString id, qint64 progress, qint64 total) {
                         if (id == txnId) {
                             DBG_INFO << "fileTransferProgress:" << progress << total;
                         }
                     });

    if (validatePendingEvent(txnId)) {
        Conversation cons;
        cons.sendByMe = true;
        cons.sender = !m_room->localMember().displayName().isEmpty()
                          ? m_room->localMember().displayName()
                          : m_room->localMember().id();
        cons.receiver = m_room->localMember().displayName();
        cons.timestamp = QDateTime::currentDateTime().toMSecsSinceEpoch();
        cons.messageType = file_mimeType;
        cons.file_path = QUrl::fromLocalFile(filepath).toString();
        cons.txnId = txnId;
        cons.visible = isValidMessageTime(cons);

        if (file_mimeType == (int)ENUNCHUCK::ROOM_EVT::FILE_OTHER) {
            if (!description.isEmpty()) {
                const QString messageInput = QString("%1 \n %2").arg(file_caption, description);
                cons.message = Quotient::prettyPrint(messageInput);
            } else {
                cons.message = Quotient::prettyPrint(file_caption);
            }
        } else {
            cons.message = Quotient::prettyPrint(description);
        }

        conversation()->addMessage(cons);
        conversation()->requestSortByTimeAscending();
        setLastMessage(cons);
        setLasttimestamp(cons);
    }
}

void QNunchukRoom::inviteToRoom(const QString &memberId)
{
    const QString targetId = memberId.trimmed();
    const QString operationId = QStringLiteral("invite:") + targetId;
    if(!m_room || targetId.isEmpty() || m_memberOperations.contains(operationId)){
        return;
    }
    if(!canInviteMembers()){
        const QString error = tr("You do not have permission to invite members");
        AppModel::instance()->showToast(BaseJob::Unauthorised, error,
                                        EWARNING::WarningType::EXCEPTION_MSG);
        emit memberInviteFailed(targetId, error);
        return;
    }
    const Membership membership = m_room->member(targetId).membershipState();
    if(membership == Membership::Join || membership == Membership::Invite){
        emit memberInviteSucceeded(targetId);
        return;
    }
    m_memberOperations.insert(operationId);
    auto inviteJob = m_room->connection()->callApi<InviteUserJob>(m_room->id(), targetId);
    connect(inviteJob, &BaseJob::finished, this,
            [this, inviteJob, targetId, operationId] {
        if(inviteJob->error() == BaseJob::Abandoned){
            m_memberOperations.remove(operationId);
            AppModel::instance()->showToast(inviteJob->error(), inviteJob->errorString(),
                                            EWARNING::WarningType::EXCEPTION_MSG);
            emit memberInviteFailed(targetId, inviteJob->errorString());
        }
    });
    inviteJob.then(this,
        [this, targetId, operationId](InviteUserJob*) {
            m_memberOperations.remove(operationId);
            emit memberInviteSucceeded(targetId);
        },
        [this, targetId, operationId](InviteUserJob* job) {
            m_memberOperations.remove(operationId);
            AppModel::instance()->showToast(job->error(), job->errorString(),
                                            EWARNING::WarningType::EXCEPTION_MSG);
            emit memberInviteFailed(targetId, job->errorString());
        });
}

void QNunchukRoom::kickMember(const QString& memberId)
{
    const QString targetId = memberId.trimmed();
    const QString operationId = QStringLiteral("kick:") + targetId;
    if(!m_room || targetId.isEmpty() || m_memberOperations.contains(operationId)){
        return;
    }
    if(!canKickMember(targetId)){
        const QString error = tr("You do not have permission to remove this member");
        AppModel::instance()->showToast(BaseJob::Unauthorised, error,
                                        EWARNING::WarningType::EXCEPTION_MSG);
        emit memberKickFailed(targetId, error);
        return;
    }
    m_memberOperations.insert(operationId);
    const QString kickreason = QString("%1 %2").arg(STR_CPP_005, localUserName());
    auto kickJob = m_room->connection()->callApi<KickJob>(m_room->id(), targetId,
                                                          kickreason);
    connect(kickJob, &BaseJob::finished, this,
            [this, kickJob, targetId, operationId] {
        if(kickJob->error() == BaseJob::Abandoned){
            m_memberOperations.remove(operationId);
            AppModel::instance()->showToast(kickJob->error(), kickJob->errorString(),
                                            EWARNING::WarningType::EXCEPTION_MSG);
            emit memberKickFailed(targetId, kickJob->errorString());
        }
    });
    kickJob.then(this,
        [this, targetId, operationId](KickJob*) {
            m_memberOperations.remove(operationId);
            emit memberKickSucceeded(targetId);
        },
        [this, targetId, operationId](KickJob* job) {
            m_memberOperations.remove(operationId);
            AppModel::instance()->showToast(job->error(), job->errorString(),
                                            EWARNING::WarningType::EXCEPTION_MSG);
            emit memberKickFailed(targetId, job->errorString());
        });
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

NunchukEventBackendResolution QNunchukRoom::resolveNunchukEventBackend(
        const QString &roomId,
        const QString &matrixType,
        const QString &eventId,
        const QJsonObject &json)
{
    NunchukEventBackendResolution result;
    const QString messageType = json["msgtype"].toString();
    const QJsonObject body = json["body"].toObject();

    if (qUtils::strCompare(matrixType, NUNCHUK_EVENT_TRANSACTION)
            && qUtils::strCompare(messageType, NUNCHUK_MSG_TX_RECEIVE)) {
        result.txReceiveLookupAttempted = true;
        result.txReceiveHasRoomWallet = matrixbrigde::HasRoomWallet(roomId);
        if (result.txReceiveHasRoomWallet) {
            QWarningMessage warning;
            result.txReceiveTransactionId = matrixbrigde::GetTransactionId(
                        roomId, eventId, warning);
            result.txReceiveTransactionIdAccepted =
                    (int)EWARNING::WarningType::NONE_MSG == warning.type()
                    && !result.txReceiveTransactionId.isEmpty();
        }
    }
    else if (qUtils::strCompare(matrixType, NUNCHUK_EVENT_WALLET)
             && qUtils::strCompare(messageType, NUNCHUK_MSG_WALLET_LEAVE)) {
        result.walletLeaveLookupAttempted = true;
        const QString joinEventId = body["io.nunchuk.relates_to"].toObject()
                ["join_event_id"].toString();
        QWarningMessage warning;
        const nunchuk::NunchukMatrixEvent joinEvent =
                matrixbrigde::GetEventData(roomId, joinEventId, warning);
        if ((int)EWARNING::WarningType::NONE_MSG == warning.type()) {
            const QJsonObject joinJson = matrixbrigde::stringToJson(
                        QString::fromStdString(joinEvent.get_content()));
            result.walletLeaveFingerprint = joinJson["body"].toObject()["key"]
                    .toString().split('/')[0].remove('[');
        }
    }
    return result;
}

bool QNunchukRoom::extractNunchukEvent(const QString &matrixType,
                                       const QString &init_event_id,
                                       const QJsonObject &json,
                                       Conversation &cons)
{
    return extractNunchukEventImpl(matrixType, init_event_id, json, cons, nullptr);
}

bool QNunchukRoom::extractNunchukEvent(
        const QString &matrixType,
        const QString &init_event_id,
        const QJsonObject &json,
        Conversation &cons,
        const NunchukEventBackendResolution &resolution)
{
    return extractNunchukEventImpl(
                matrixType, init_event_id, json, cons, &resolution);
}

bool QNunchukRoom::extractNunchukEventImpl(
        const QString &matrixType,
        const QString &init_event_id,
        const QJsonObject &json,
        Conversation &cons,
        const NunchukEventBackendResolution *resolution)
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
            if (resolution && resolution->walletLeaveLookupAttempted) {
                xfp = resolution->walletLeaveFingerprint;
            }
            else {
                QWarningMessage joinmsg;
                QNunchukMatrixEvent nunJoinEvent = matrixbrigde::GetEvent(id(), join_event_id, joinmsg);
                if((int)EWARNING::WarningType::NONE_MSG == joinmsg.type()){
                    QJsonObject joinjson = matrixbrigde::stringToJson(nunJoinEvent.get_content());
                    xfp = joinjson["body"].toObject()["key"].toString().split('/')[0].remove('[');
                }
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
            if (resolution && resolution->txReceiveLookupAttempted) {
                if (!resolution->txReceiveHasRoomWallet) return false;
                if (resolution->txReceiveTransactionIdAccepted) {
                    cons.init_event_id = resolution->txReceiveTransactionId;
                    cons.message = STR_CPP_020;
                    cons.messageType = (int)ENUNCHUCK::ROOM_EVT::TX_RECEIVE;
                    cons.init_event_json = json;
                }
            }
            else {
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

void QNunchukRoom::notifySupportClassificationChanged()
{
    emit roomNameChanged();
    emit roomTypeChanged();
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
        QDateTime message_time_utc = QDateTime::fromMSecsSinceEpoch(cons.timestamp);
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
    QMetaObject::invokeMethod(
        this,
        [this, cons, roomid]() {
            downloadTransactionThreadOnMain(cons, roomid);
        },
        Qt::AutoConnection);
}

void QNunchukRoom::downloadTransactionThreadOnMain(Conversation cons,
                                                   const QString &roomid)
{
    QRoomWallet *wallet = roomWallet();
    if (wallet && !wallet->get_wallet_id().isEmpty()) {
        const QString walletId = wallet->get_wallet_id();
        if (!cons.init_event_id.isEmpty()) { // FIXME FOR CHECK DUP RECIEVED TX EVT
            const QString ownerRoomId = id();
            const QString initEventId = cons.init_event_id;
            const int messageType = cons.messageType;
            QPointer<QNunchukRoom> safeThis(this);
            runInThread(
                this,
                [walletId,
                 roomid,
                 ownerRoomId,
                 initEventId,
                 messageType]() -> DownloadTransactionResult {
                    DownloadTransactionResult result;
                    if (messageType == (int)ENUNCHUCK::ROOM_EVT::TX_RECEIVE) {
                        QWarningMessage txWarning;
                        nunchuk::Transaction tx = bridge::nunchukGetOriginTransaction(
                                    walletId, initEventId, txWarning);
                        nunchuk::RoomTransaction roomTransaction;
                        roomTransaction.set_wallet_id(walletId.toStdString());
                        roomTransaction.set_tx_id(initEventId.toStdString());
                        roomTransaction.set_room_id(ownerRoomId.toStdString());
                        roomTransaction.set_tx(tx);
                        roomTransaction.set_init_event_id(initEventId.toStdString());
                        result.shouldNotify = true;
                        result.roomTransaction = std::move(roomTransaction);
                        result.transaction = std::move(tx);
                        return result;
                    }

                    DBG_INFO << "FIXME TRANSACTON: init_event:>" << initEventId;
                    QWarningMessage roomTxWarning;
                    nunchuk::RoomTransaction roomTransaction =
                            matrixbrigde::GetOriginRoomTransaction(
                                roomid, initEventId, roomTxWarning);
#if 0 //FIXME ==> HANDLE CANCEL
                    if(messageType == (int)ENUNCHUCK::ROOM_EVT::TX_CANCEL){
                        QWarningMessage msggetevt;
                        QNunchukMatrixEvent evt = matrixbrigde::GetEvent(roomid, initEventId, msggetevt);
                        DBG_INFO << "FIXME"
                                 << evt.get_content();
                    }
#endif
                    if ((int)EWARNING::WarningType::NONE_MSG == roomTxWarning.type()
                            && !roomTransaction.get_wallet_id().empty()) {
                        DBG_INFO << "FIXME TRANSACTON: get_wallet_id:>"
                                 << roomTransaction.get_wallet_id()
                                 << "get_tx_id:" << roomTransaction.get_tx_id();
                        QWarningMessage txWarning;
                        roomTransaction.set_room_id(ownerRoomId.toStdString());
                        nunchuk::Transaction tx = bridge::nunchukGetOriginTransaction(
                                    QString::fromStdString(roomTransaction.get_wallet_id()),
                                    QString::fromStdString(roomTransaction.get_tx_id()),
                                    txWarning);
                        if ((int)EWARNING::WarningType::NONE_MSG == txWarning.type()
                                && !tx.get_txid().empty()) {
                            result.shouldNotify = true;
                            result.roomTransaction = std::move(roomTransaction);
                            result.transaction = std::move(tx);
                        }
                    }
                    return result;
                },
                [safeThis, cons](DownloadTransactionResult result) {
                    if (!safeThis || !result.shouldNotify) {
                        return;
                    }
                    emit safeThis->signalFinishedDownloadTransaction(
                        result.roomTransaction, result.transaction, cons);
                });
        }
        AppModel::instance()->requestSyncWalletDb(walletId);
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

void QNunchukRoom::markMessagesAsRead(const QString& eventId)
{
    if(m_room && !eventId.isEmpty()){
        m_room->setLastDisplayedEventId(eventId);
        m_room->markMessagesAsRead(eventId);
    }
}

void QNunchukRoom::markFiveMessagesAsRead()
{
    if(!m_room || !m_conversation){
        return;
    }
    // Conversation rows are filtered/sorted independently of Quotient's
    // timeline indices. Always advance the marker by Matrix event id.
    for(int row = m_conversation->count() - 1; row >= 0; --row){
        const QString eventId = m_conversation->eventIdAt(row);
        if(!eventId.isEmpty()){
            markMessagesAsRead(eventId);
            return;
        }
    }
}

void QNunchukRoom::sendTypingState(bool typing)
{
    if(!m_room || !m_room->connection() || m_room->joinState() != JoinState::Join){
        return;
    }
    m_room->connection()->callApi<SetTypingJob>(
        m_room->connection()->userId(), m_room->id(), typing,
        typing ? std::optional<int>(5000) : std::nullopt);
    if(typing){
        m_typingSentAt.restart();
    }
}

void QNunchukRoom::setTyping(bool typing)
{
    if(!typing){
        m_typingIdleTimer.stop();
        if(m_localTyping){
            m_localTyping = false;
            sendTypingState(false);
        }
        return;
    }
    m_typingIdleTimer.start();
    if(!m_localTyping || !m_typingSentAt.isValid()
            || m_typingSentAt.elapsed() >= 3000){
        m_localTyping = true;
        sendTypingState(true);
    }
}

void QNunchukRoom::downloadHistorical()
{
    if (!m_room) {
        return;
    }
    DBG_INFO << "Room[" << roomName() << "], Tags[" << m_room->tagNames() << "]";
    if(isServerNoticeRoom()){
        //FIXME - DEBUG
//            for (auto e = m_room->messageEvents().rbegin(); e != m_room->messageEvents().rend(); ++e){
//                nunchukNoticeEvent(**e); // FIXME
//            }
    }
    else if(isNunchukSyncRoom()){
        const bool shouldRegister = CLIENT_INSTANCE->isNunchukLoggedIn()
                && CLIENT_INSTANCE->isMatrixLoggedIn();
        const QString roomId = shouldRegister ? id() : QString();
        const QString accessToken = shouldRegister
                ? CLIENT_INSTANCE->accessToken() : QString();
        QtConcurrent::run([shouldRegister, roomId, accessToken]() {
            if (shouldRegister) {
                matrixbrigde::RegisterAutoBackup(roomId, accessToken);
            }
        });
    }
    else{
        setRoomWallet(matrixbrigde::ReloadRoomWallet(this));

        const QString roomId = id();
        HistoricalEventSnapshotList snapshots;
        snapshots.reserve(m_room->messageEvents().size());
        for (auto eventItem = m_room->messageEvents().begin();
             eventItem != m_room->messageEvents().end(); ++eventItem) {
            const RoomEvent *event = eventItem->get();
            if (!event) {
                continue;
            }

            HistoricalEventSnapshot snapshot;
            snapshot.fullJson = event->fullJson();
            const QString matrixType = event->matrixType();
            snapshot.shouldConsume =
                    qUtils::strCompare(NUNCHUK_EVENT_WALLET, matrixType)
                    || qUtils::strCompare(NUNCHUK_EVENT_TRANSACTION, matrixType);
            if (snapshot.shouldConsume) {
                snapshot.consumeEvent.set_event_id(event->id().toStdString());
                snapshot.consumeEvent.set_type(matrixType.toStdString());
                snapshot.consumeEvent.set_content(
                            QString(QJsonDocument(event->contentJson())
                                    .toJson(QJsonDocument::Compact)).toStdString());
                snapshot.consumeEvent.set_room_id(roomId.toStdString());
                snapshot.consumeEvent.set_sender(event->senderId().toStdString());
                snapshot.consumeEvent.set_ts(
                            event->originTimestamp().toMSecsSinceEpoch());
            }
            snapshots.push_back(std::move(snapshot));
        }

        QPointer<QNunchukRoom> safeThis(this);
        runInThread(
            this,
            [roomId, snapshots = std::move(snapshots)]() mutable
                    -> HistoricalEventSnapshotList {
                for (const HistoricalEventSnapshot &snapshot : snapshots) {
                    if (snapshot.shouldConsume) {
                        matrixbrigde::ConsumeEvent(roomId, snapshot.consumeEvent);
                    }
                }

                for (auto snapshot = snapshots.rbegin();
                     snapshot != snapshots.rend(); ++snapshot) {
                    const QString matrixType =
                            snapshot->fullJson["type"].toString();
                    const QString eventId =
                            snapshot->fullJson["event_id"].toString();
                    const QJsonObject content =
                            snapshot->fullJson["content"].toObject();
                    snapshot->backendResolution =
                            QNunchukRoom::resolveNunchukEventBackend(
                                roomId, matrixType, eventId, content);
                }
                return snapshots;
            },
            [safeThis](HistoricalEventSnapshotList snapshots) {
                if (!safeThis) {
                    return;
                }

                if (safeThis->conversation()) {
                    safeThis->conversation()->clear();
                    if (!safeThis->roomWallet()) {
                        Conversation init;
                        init.timestamp = -100;
                        init.messageType = (int)ENUNCHUCK::ROOM_EVT::INITIALIZE;
                        safeThis->conversation()->addHistoryMessage(init);
                    }

                    for (auto snapshot = snapshots.rbegin();
                         snapshot != snapshots.rend(); ++snapshot) {
                        RoomEventPtr event = loadEvent<RoomEvent>(snapshot->fullJson);
                        if (!event) {
                            continue;
                        }
                        Conversation cons = safeThis->createConversation(
                                    *event, &snapshot->backendResolution);
                        if (cons.messageType != (int)ENUNCHUCK::ROOM_EVT::INVALID) {
                            safeThis->conversation()->addHistoryMessage(cons);
                        }
                    }

                    safeThis->conversation()->requestSortByTimeAscending(false);
                    safeThis->setLastMessage(safeThis->conversation()->lastMessage());
                    safeThis->setLasttimestamp(safeThis->conversation()->lastTime());
                    // A room key can arrive while the snapshots above are
                    // being resolved off-thread. Mark hydration complete and
                    // replay copied decrypted JSON only after the stale
                    // snapshots have finished rebuilding the model.
                    safeThis->m_downloaded = true;
                    const auto pendingDecryptedEvents = std::exchange(
                                safeThis->m_pendingDecryptedEvents, {});
                    for(const QJsonObject& eventJson : pendingDecryptedEvents){
                        safeThis->applyDecryptedEvent(eventJson);
                    }
                    if (safeThis->roomWallet()) {
                        AppModel::instance()->requestSyncWalletDb(
                                    safeThis->roomWallet()->get_wallet_id());
                        const bool isCreator = safeThis->conversation()->isWalletCreator(
                                    safeThis->roomWallet()->get_init_event_id());
                        safeThis->roomWallet()->setIsCreator(isCreator);
                    }
                }

                safeThis->startGetPendingTxsOnMain();
            });
    }
}

void QNunchukRoom::connectRoomSignals()
{
    if(m_room && !m_roomSignalsConnected){
        m_roomSignalsConnected = true;
        m_downloaded = false;
        connect(m_room, &Room::unreadStatsChanged, this, &QNunchukRoom::unreadMessagesChanged);
        connect(m_room, &Room::highlightCountChanged, this, &QNunchukRoom::highlightCountChanged);
        connect(m_room, &Room::namesChanged, this, &QNunchukRoom::roomNameChanged);
        connect(m_room, &Room::tagsChanged, this, &QNunchukRoom::notifySupportClassificationChanged);
        connect(m_room, &Room::pendingEventAboutToMerge, this, &QNunchukRoom::pendingEventAboutToMerge);
        connect(m_room, &Room::pendingEventChanged, this, &QNunchukRoom::pendingEventChanged);
        connect(m_room, &Room::messageSent, this, &QNunchukRoom::messageSent);
        connect(m_room, &Room::aboutToAddNewMessages, this, &QNunchukRoom::aboutToAddNewMessages);
        connect(m_room, &Room::addedMessages, this, &QNunchukRoom::addedMessages);
        connect(m_room, &Room::replacedEvent, this, &QNunchukRoom::replacedEvent);
        connect(m_room, &Room::addedMessages, this, &QNunchukRoom::allHisLoadedChanged);
        connect(m_room, &Room::aboutToAddHistoricalMessages, this, &QNunchukRoom::aboutToAddHistoricalMessages);
        connect(m_room, &Room::memberListChanged, this, &QNunchukRoom::usersChanged);
        connect(m_room, &Room::memberListChanged, this, &QNunchukRoom::roomNameChanged);
        connect(m_room, &Room::memberListChanged, this, &QNunchukRoom::userCountChanged);
        connect(m_room, &Room::memberListChanged, this, &QNunchukRoom::permissionsChanged);
        connect(m_room, &Room::changed, this, [this](Room::Changes) {
            emit permissionsChanged();
        });
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
    QString localUserId = room()->localMember().id();
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
    QMetaObject::invokeMethod(
        this,
        [this]() {
            startGetPendingTxsOnMain();
        },
        Qt::AutoConnection);
}

void QNunchukRoom::startGetPendingTxsOnMain()
{
    const QString roomId = id();
    if (matrixbrigde::HasRoomWallet(roomId)) {
        QPointer<QNunchukRoom> safeThis(this);
        runInThread(
            this,
            [roomId]() -> PendingTransactionDataList {
                PendingTransactionDataList data;
                QWarningMessage message;
                const std::vector<nunchuk::RoomTransaction> roomTransactions =
                        matrixbrigde::GetOriginPendingTransactions(roomId, message);
                if ((int)EWARNING::WarningType::NONE_MSG != message.type()) {
                    return data;
                }

                data.reserve(roomTransactions.size());
                for (const nunchuk::RoomTransaction &roomTransaction : roomTransactions) {
                    QWarningMessage transactionMessage;
                    nunchuk::Transaction transaction =
                            bridge::nunchukGetOriginTransaction(
                                QString::fromStdString(roomTransaction.get_wallet_id()),
                                QString::fromStdString(roomTransaction.get_tx_id()),
                                transactionMessage);
                    if ((int)EWARNING::WarningType::NONE_MSG
                            == transactionMessage.type()) {
                        data.push_back({roomTransaction, std::move(transaction)});
                    }
                }
                return data;
            },
            [safeThis, roomId](PendingTransactionDataList data) {
                if (!safeThis) {
                    return;
                }
                QRoomTransactionModelPtr model =
                        buildPendingTransactionModel(roomId, data);
                emit safeThis->signalFinishedGetPendingTxs(model);
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
    if(cons.messageType == (int)ENUNCHUCK::ROOM_EVT::INVALID
            || cons.messageType == (int)ENUNCHUCK::ROOM_EVT::INITIALIZE){
        if(!m_lastMessage.isEmpty()){
            m_lastMessage.clear();
            emit lastMessageChanged();
        }
        return;
    }
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
    if(m_lasttimestamp <= 0){
        return "";
    }
    QDateTime today = QDateTime::currentDateTime();
    QDateTime day = QDateTime::fromMSecsSinceEpoch(m_lasttimestamp);
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
    if(lasttimestamp > 0 && m_lasttimestamp < lasttimestamp){
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
    emit unreadCountChanged();
}

void QNunchukRoom::notificationCountChanged()
{
    emit unreadCountChanged();
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
    else if(!m_downloaded){
        // downloadHistorical() rebuilds from an immutable snapshot on a
        // worker. Preserve live events that arrive meanwhile so that its
        // callback cannot erase them (whether already decrypted or not).
        for(auto eventItem = m_room->messageEvents().rbegin();
            eventItem != m_room->messageEvents().rend(); ++eventItem){
            if(fromIndex <= eventItem->index()
                    && toIndex >= eventItem->index()
                    && eventItem->get()){
                m_pendingDecryptedEvents.append(eventItem->get()->fullJson());
            }
        }
    }
    else{
        receiveMessage(fromIndex, toIndex);
    }
}

void QNunchukRoom::replacedEvent(const RoomEvent* newEvent,
                                 const RoomEvent* oldEvent)
{
    // Room::replacedEvent is also used for edits and redactions. Only rebuild
    // a conversation when Quotient has replaced an encrypted placeholder with
    // the event decrypted after its room key arrived.
    const auto* encryptedOld = eventCast<const EncryptedEvent>(oldEvent);
    if(!newEvent || !encryptedOld
            || newEvent->originalEvent() != encryptedOld){
        return;
    }
    if(isServerNoticeRoom()){
        nunchukNoticeEvent(*newEvent);
        return;
    }
    if(isNunchukSyncRoom()){
        nunchukConsumeSyncEvent(*newEvent);
        return;
    }
    const QJsonObject eventJson = newEvent->fullJson();
    if(!m_downloaded){
        m_pendingDecryptedEvents.append(eventJson);
        return;
    }
    applyDecryptedEvent(eventJson);
}

bool QNunchukRoom::applyDecryptedEvent(const QJsonObject& eventJson)
{
    if(!conversation()){
        return false;
    }
    RoomEventPtr event = loadEvent<RoomEvent>(eventJson);
    if(!event){
        return false;
    }

    nunchukConsumeEvent(*event);
    if(event->matrixType() == NUNCHUK_EVENT_WALLET){
        setRoomWallet(matrixbrigde::ReloadRoomWallet(this));
    }

    const Conversation replacement = createConversation(*event);
    if(replacement.messageType == (int)ENUNCHUCK::ROOM_EVT::INVALID){
        const bool removed = conversation()->replaceMessage(
                    event->id(), replacement);
        if(removed){
            conversation()->requestSortByTimeAscending();
            setLastMessage(conversation()->lastMessage());
            setLasttimestamp(conversation()->lastTime());
        }
        return removed;
    }
    if(!conversation()->replaceMessage(event->id(), replacement)){
        // The event may have arrived after downloadHistorical() took its
        // snapshot, so there is no encrypted placeholder in the rebuilt model.
        conversation()->addMessage(replacement);
    }
    if(replacement.messageType == (int)ENUNCHUCK::ROOM_EVT::WALLET_CANCEL){
        updateCancelWallet(replacement.init_event_id);
    }
    if(replacement.messageType == (int)ENUNCHUCK::ROOM_EVT::TX_CANCEL){
        updateCancelTransaction(replacement);
    }
    conversation()->requestSortByTimeAscending();
    setLastMessage(conversation()->lastMessage());
    setLasttimestamp(conversation()->lastTime());
    return true;
}

void QNunchukRoom::aboutToAddHistoricalMessages(RoomEventsRange events)
{
}

void QNunchukRoom::unreadMessagesChanged()
{
    emit unreadCountChanged();
}

void QNunchukRoom::typingChanged()
{
    QStringList usersTypingName;
    usersTypingName.clear();
    if (!m_room || m_room->otherMembersTyping().isEmpty())  {
        setTypingNames("");
        return;
    }
    const auto& usersTyping = m_room->otherMembersTyping();
    int MaxNamesToShow = 3;
    int SampleSizeForHud = 2;
    usersTypingName.reserve(MaxNamesToShow);
    const auto endIt = usersTyping.size() > MaxNamesToShow
            ? usersTyping.cbegin() + SampleSizeForHud
            : usersTyping.cend();
    for (auto it = usersTyping.cbegin(); it != endIt; ++it)
        usersTypingName << (*it).displayName();

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
            using namespace Quotient::EventContent;
            const auto msgType = e.msgtype();

            if (msgType == RoomMessageEvent::MsgType::Image ||
                msgType == RoomMessageEvent::MsgType::Video ||
                msgType == RoomMessageEvent::MsgType::File)
            {
                auto fileCaption = prettyPrint ? Quotient::prettyPrint(e.plainBody()) : e.plainBody();

                hasFileContent = true;

                const auto filename = e.plainBody().toHtmlEscaped();

                if (msgType == RoomMessageEvent::MsgType::Image) {
                    file_mimeType = (int)ENUNCHUCK::ROOM_EVT::FILE_IMAGE;
                    fileCaption.remove(filename);
                }
                else if (msgType == RoomMessageEvent::MsgType::Video) {
                    file_mimeType = (int)ENUNCHUCK::ROOM_EVT::FILE_VIDEO;
                    fileCaption.remove(filename);
                }
                else {
                    file_mimeType = (int)ENUNCHUCK::ROOM_EVT::FILE_OTHER;
                }

                return fileCaption;
            }
            else{
                QString plainBody;
                // 1. prettyPrint/HTML
                if (prettyPrint && e.mimeType().name() != "text/plain") {
                    auto htmlBody = static_cast<const EventContent::TextContent*>(e.content().get())->body;;
                    htmlBody.replace(utils::userPillRegExp, "<b>\\1</b>");
                    htmlBody.replace(utils::strikethroughRegExp, "<s>\\1</s>");
                    return htmlBody;
                }
                // 2. prettyPrint/text 3. plainText/HTML 4. plainText/text
                if (e.content() && e.mimeType().name() == "text/plain") {  // 2/4
                    plainBody = static_cast<const EventContent::TextContent*>(e.content().get())->body;;
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
            auto subjectName = m_room->member(e.userId()).displayName();
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
                    if (e.newDisplayName()->isEmpty())
                        text = STR_CPP_025;
                    else
                        text = STR_CPP_026.arg(e.newDisplayName()->toHtmlEscaped());
                }
                if (e.isAvatarUpdate()) {
                    if (!text.isEmpty())
                        text += STR_CPP_027;
                    if (e.newAvatarUrl()->isEmpty())
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
                    int member_size = m_room->members().size();
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
        [=](const StateEvent& e) {
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
                RoomMember sender = m_room->member(lastEvent->senderId());
                QString nameDisplay = sender.displayName() != "" ? sender.displayName() : sender.id();
                QString avatar = sender.avatarMediaId();
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
    return createConversation(evt, nullptr);
}

Conversation QNunchukRoom::createConversation(
        const RoomEvent &evt,
        const NunchukEventBackendResolution *resolution)
{
    RoomMember sender = m_room->member(evt.senderId());
    Conversation cons;
    cons.isStateEvent = evt.isStateEvent();
    cons.sendByMe   = (sender == m_room->localMember());
    cons.sender     = sender.displayName() != "" ? sender.displayName() : sender.id();
    cons.avatar     = sender.avatarMediaId();
    cons.receiver   = m_room->localMember().id();
    cons.timestamp  = evt.originTimestamp().toMSecsSinceEpoch();
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
        bool ret = resolution
                ? extractNunchukEvent(
                      evt.matrixType(), evt.id(), evt.contentJson(), cons, *resolution)
                : extractNunchukEvent(evt, cons);
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
        e.set_ts(evt.originTimestamp().toMSecsSinceEpoch());
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
            e.set_ts(evt.originTimestamp().toMSecsSinceEpoch());
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
            else if (msgtype.contains("io.nunchuk.custom.wallet_created", Qt::CaseInsensitive)
                       || msgtype.contains("io.nunchuk.custom.wallet_name_changed", Qt::CaseInsensitive)
                       || msgtype.contains("io.nunchuk.custom.key_name_changed", Qt::CaseInsensitive)
                       || msgtype.contains("io.nunchuk.custom.key_updated", Qt::CaseInsensitive))
            {
                AppModel::instance()->requestCreateUserWallets();
                QList<uint> states = QEventProcessor::instance()->getCurrentStates();
                if(msgtype.contains("io.nunchuk.custom.wallet_created", Qt::CaseInsensitive)
                    &&  (!states.isEmpty())
                    && (states.last() == (uint)E::STATE_ID_SCR_DASHBOARD_ALERT_INFO))
                {
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
                                if(wallet){
                                    QTransactionPtr trans = wallet.data()->SyncAssistedTxs(tx);
                                    if (trans && wallet.data()->transactionHistory()) {
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
            else if (msgtype.contains("io.nunchuk.custom.shared", Qt::CaseInsensitive))
            {
                QSharedWallets::instance()->GetAllGroups();
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
    if(m_connection){
        // createdRoom only confirms that Quotient has provided a local Room
        // object. Its state and direct-chat mapping are not authoritative yet;
        // keep this connection strictly for lifecycle tracing.
        connect(m_connection, &Connection::createdRoom, this,
            [](Room* room) {
                DBG_INFO << "[MATRIX_ROOM_TRACE] Quotient::createdRoom id:"
                         << (room ? room->id() : QStringLiteral("<null>"))
                         << "joinState:"
                         << (room ? static_cast<int>(room->joinState()) : -1);
            });
    }
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
    if(!index.isValid() || index.row() < 0 || index.row() >= m_data.count()
            || !m_data.at(index.row())){
        return {};
    }
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
    case room_is_any_support:
        return m_data[index.row()].data()->isAnySupportRoom();
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
    names[room_is_any_support]  = "is_any_support";
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
        const quint64 hydrationGeneration = ++m_roomHydrationGeneration;
        // Do not allow support-room creation before the existing rooms and
        // direct-chat account data have been hydrated into the model.
        m_roomsHydrated = false;
        CLIENT_INSTANCE->setReadySupport(false);
        // downloadRooms is called from syncDone. Quotient has already queued
        // every Room::updateData at that point; queue this callback behind
        // those MetaCalls instead of relying on an arbitrary timer. This makes
        // base state, names, members and tags visible before model hydration.
        DBG_INFO << "[SUPPORT] Scheduling room hydration after initial sync";
        QMetaObject::invokeMethod(this, [this, hydrationGeneration]() {
                if(hydrationGeneration != m_roomHydrationGeneration){
                    return;
                }
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
                QString hydratedLocallyCreatedRoomId;
                for(const QNunchukRoomPtr& room : std::as_const(m_data)){
                    if(room && m_locallyCreatedRoomIds.contains(room->id())){
                        hydratedLocallyCreatedRoomId = room->id();
                        break;
                    }
                }
                if(!hydratedLocallyCreatedRoomId.isEmpty()){
                    m_locallyCreatedRoomIds.remove(hydratedLocallyCreatedRoomId);
                }
                resort();
                if(!hydratedLocallyCreatedRoomId.isEmpty()){
                    setCurrentIndex(getIndex(hydratedLocallyCreatedRoomId));
                }
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
                m_roomsHydrated = true;
                if(!m_supportRequestInProgress){
                    CLIENT_INSTANCE->setReadySupport(true);
                }
                if(m_supportRoomRequestPending){
                    m_supportRoomRequestPending = false;
                    QTimer::singleShot(0, this, [this, hydrationGeneration] {
                        if(hydrationGeneration != m_roomHydrationGeneration
                                || !m_roomsHydrated){
                            return;
                        }
                        createSupportRoom();
                    });
                }
                downloadRoomWallets();
                synchonizesUserData();
        }, Qt::QueuedConnection);
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
    if(index < -1 || index >= m_data.count()){
        return;
    }
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
        if(m_currentRoom){
            m_currentRoom->setTyping(false);
            m_currentRoom->setDisplayed(false);
        }
        m_currentRoom = newRoom;
        if(m_currentRoom){
            m_currentRoom.data()->setDisplayed(true);
            m_currentRoom.data()->startGetPendingTxs();
            if(m_currentRoom->conversation()){
                if(m_currentRoom->conversation()->unreadLastIndex() + 10 > m_currentRoom->conversation()->count()){
                    m_currentRoom.data()->conversation()->setCurrentIndex(m_currentRoom.data()->conversation()->rowCount() - 1);
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

bool QNunchukRoomListModel::roomCreationInProgress() const
{
    return m_roomCreationInProgress;
}

void QNunchukRoomListModel::setRoomCreationInProgress(bool inProgress)
{
    if(m_roomCreationInProgress == inProgress){
        return;
    }
    m_roomCreationInProgress = inProgress;
    emit roomCreationInProgressChanged();
}

void QNunchukRoomListModel::trackLocallyCreatedRoom(const QString &roomId)
{
    if(roomId.isEmpty()){
        return;
    }
    m_locallyCreatedRoomIds.insert(roomId);
    const int existingIndex = getIndex(roomId);
    if(existingIndex >= 0){
        m_locallyCreatedRoomIds.remove(roomId);
        resort();
        setCurrentIndex(getIndex(roomId));
        return;
    }

    Room* createdRoom = connection() ? connection()->room(roomId) : nullptr;
    if(createdRoom && createdRoom->joinState() == JoinState::Join){
        // The Connection::joinedRoom signal can precede model hydration. Feed
        // the already-known room through the same state-ready path so a local
        // create cannot remain pending solely because that signal was missed.
        joinedRoom(createdRoom, nullptr);
    }
}

void QNunchukRoomListModel::postInitialMessageWhenStateReady(
        const QString& roomId, const QVariant& firstMessage,
        quint64 operationGeneration)
{
    const QString message = firstMessage.toString();
    if(message.isEmpty() || !connection()){
        return;
    }
    QPointer<Connection> attempt = connection();
    QPointer<Room> targetRoom = attempt->room(roomId);
    if(!targetRoom){
        AppModel::instance()->showToast(
            BaseJob::IncorrectResponse,
            tr("Room was created but its local state is unavailable; the first message was not sent"),
            EWARNING::WarningType::EXCEPTION_MSG);
        return;
    }

    QObject* context = new QObject(this);
    const auto sendWhenEncrypted = [this, context, attempt, targetRoom, message,
                                    operationGeneration] {
        if(!context || !targetRoom){
            return;
        }
        if(!attempt || attempt != connection()
                || operationGeneration != m_roomOperationGeneration){
            context->deleteLater();
            return;
        }
        if(!targetRoom->usesEncryption()){
            AppModel::instance()->showToast(
                BaseJob::IncorrectResponse,
                QObject::tr("Encryption state was not confirmed; the first message was not sent"),
                EWARNING::WarningType::EXCEPTION_MSG);
            context->deleteLater();
            return;
        }
        targetRoom->postPlainText(message);
        context->deleteLater();
    };

    if(targetRoom->usesEncryption()){
        sendWhenEncrypted();
        return;
    }
    connect(targetRoom, &Room::baseStateLoaded, context, sendWhenEncrypted,
            Qt::SingleShotConnection);
    QTimer::singleShot(30000, context,
                       [this, context, attempt, operationGeneration] {
        if(!context){
            return;
        }
        if(!attempt || attempt != connection()
                || operationGeneration != m_roomOperationGeneration){
            context->deleteLater();
            return;
        }
        AppModel::instance()->showToast(
            BaseJob::Timeout,
            QObject::tr("Timed out waiting for encrypted room state; the first message was not sent"),
            EWARNING::WarningType::EXCEPTION_MSG);
        context->deleteLater();
    });
}

void QNunchukRoomListModel::requestSort()
{
    std::sort(m_data.begin(), m_data.end(), sortRoomListByTimeDescending);
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
    DBG_INFO << "[DO_ADD_ROOM] id:" << r.data()->id()
             << "isServiceNotice:" << r.data()->isServerNoticeRoom()
             << "isSyncRoom:" << r.data()->isNunchukSyncRoom()
             << "alreadyInData:" << containsRoomId(r.data()->id())
             << "tags:" << (r.data()->room() ? r.data()->room()->tagNames() : QStringList{});
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
            connect(r.data(),         &QNunchukRoom::roomTypeChanged,       this, [this, r] { refresh(r, {room_name, room_type, room_is_any_support}); });
            connect(r.data(),         &QNunchukRoom::lastMessageChanged,    this, [this, r] { refresh(r); });
            connect(r.data(),         &QNunchukRoom::lasttimestampChanged,  this, [this, r] { resort(); });
            connect(r.data(),         &QNunchukRoom::roomNeedTobeLeaved,    this, &QNunchukRoomListModel::roomNeedTobeLeaved);
            connect(r.data()->room(), &Room::unreadStatsChanged,            this, [this, r] { refresh(r, {room_unreadmsg_count}); });
            connect(r.data()->room(), &Room::unreadStatsChanged,            this, &QNunchukRoomListModel::totalUnreadChanged);
            connect(r.data(),         &QNunchukRoom::typingNamesChanged,    this, [this, r] { refresh(r, {room_typing_users}); });
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
            emit countChanged();
        }
    }
}

void QNunchukRoomListModel::removeRoomByIndex(const int index)
{
    const QNunchukRoomPtr roomPtr = getRoomByIndex(index);
    if(roomPtr){
        removeRoomById(roomPtr->id());
    }
}

void QNunchukRoomListModel::removeRoomById(const QString &id)
{
    const int removedIndex = getIndex(id);
    if(removedIndex < 0){
        return;
    }
    const QString selectedRoomId = currentRoom() ? currentRoom()->id() : QString{};
    beginResetModel();
    m_data.removeAt(removedIndex);
    endResetModel();
    if(selectedRoomId == id || selectedRoomId.isEmpty()){
        setCurrentIndex(m_data.isEmpty()
                            ? -1
                            : qMin(removedIndex, m_data.count() - 1));
    } else {
        setCurrentIndex(getIndex(selectedRoomId));
    }
    emit countChanged();
}

void QNunchukRoomListModel::removeAll()
{
    // A cleared model is no longer hydrated. Invalidate callbacks belonging
    // to the previous login and do not carry a queued Support click across
    // account changes.
    m_roomsHydrated = false;
    m_supportRoomRequestPending = false;
    m_supportRequestInProgress = false;
    m_supportRoomsLeaving.clear();
    m_roomsLeaving.clear();
    m_locallyCreatedRoomIds.clear();
    m_byzantineRoomsCreating.clear();
    setRoomCreationInProgress(false);
    ++m_roomOperationGeneration;
    m_supportCreateRequestId = 0;
    m_supportReconciliationRequestId = 0;
    ++m_roomHydrationGeneration;
    ++m_supportWatchdogId;
    ++m_supportRequestId;
    beginResetModel();
    while (m_data.count() > 0) {
        m_data.removeAt(0);
    }
    endResetModel();
    setCurrentIndex(-1);
    emit countChanged();
}

void QNunchukRoomListModel::forgetRoom(const int index)
{
    const QNunchukRoomPtr roomPtr = getRoomByIndex(index);
    if(!roomPtr || roomPtr->isAnySupportRoom()){
        return;
    }
    Room* room = roomPtr->room();
    if(room && connection()){
        const QString roomId = room->id();
        if(m_roomsLeaving.contains(roomId)){
            return;
        }
        m_roomsLeaving.insert(roomId);
        auto forgetJob = connection()->forgetRoom(roomId);
        connect(forgetJob, &BaseJob::finished, this, [this, forgetJob, roomId] {
            if(!m_roomsLeaving.contains(roomId)){
                return;
            }
            if(forgetJob->error() == BaseJob::Abandoned){
                m_roomsLeaving.remove(roomId);
                AppModel::instance()->showToast(forgetJob->error(), forgetJob->errorString(),
                                                EWARNING::WarningType::EXCEPTION_MSG);
            }
        });
        connect(forgetJob, &BaseJob::success, this, [this, roomId] {
                if(!m_roomsLeaving.contains(roomId)){
                    return;
                }
                m_roomsLeaving.remove(roomId);
                removeRoomById(roomId);
            });
        connect(forgetJob, &BaseJob::failure, this, [this, roomId, forgetJob] {
                if(!m_roomsLeaving.contains(roomId)){
                    return;
                }
                m_roomsLeaving.remove(roomId);
                AppModel::instance()->showToast(forgetJob->error(), forgetJob->errorString(),
                                                EWARNING::WarningType::EXCEPTION_MSG);
            });
    }
}

void QNunchukRoomListModel::leaveCurrentRoom()
{
    if(currentRoom()){
        leaveRoomById(currentRoom()->id());
    }
}

void QNunchukRoomListModel::leaveRoom(const int index)
{
    const QNunchukRoomPtr roomPtr = getRoomByIndex(index);
    if(roomPtr){
        leaveRoomById(roomPtr->id());
    }
}

void QNunchukRoomListModel::leaveRoomById(const QString &roomId)
{
    const int index = getIndex(roomId);
    const QNunchukRoomPtr roomPtr = getRoomByIndex(index);
    if(!roomPtr || !roomPtr->room() || !connection()){
        return;
    }
    if(roomPtr->isAnySupportRoom()){
        leaveSupportRoom(roomPtr);
        return;
    }
    if(m_roomsLeaving.contains(roomId)){
        return;
    }
    m_roomsLeaving.insert(roomId);

    const bool hasWallet = roomPtr->roomWallet();
    const bool isByzantineRoom = roomPtr->isNunchukByzantineRoom();
    const QString byzantineGroupId = roomPtr->byzantineRoomGroupId();
    auto leaveJob = roomPtr->room()->leaveRoom();
    connect(leaveJob, &BaseJob::finished, this, [this, leaveJob, roomId] {
        if(!m_roomsLeaving.contains(roomId)){
            return;
        }
        if(leaveJob->error() == BaseJob::Abandoned){
            m_roomsLeaving.remove(roomId);
            AppModel::instance()->showToast(leaveJob->error(), leaveJob->errorString(),
                                            EWARNING::WarningType::EXCEPTION_MSG);
        }
    });
    leaveJob.then(this,
        [this, roomId, hasWallet, isByzantineRoom, byzantineGroupId](LeaveRoomJob*) {
            if(!m_roomsLeaving.contains(roomId)){
                return;
            }
            m_roomsLeaving.remove(roomId);
            removeRoomById(roomId);
            if(hasWallet){
                AppModel::instance()->startReloadUserDb();
            }
            if(isByzantineRoom){
                emit byzantineRoomDeleted(roomId, byzantineGroupId);
            }
            DBG_INFO << "LEAVE ROOM SUCCEED" << roomId;
        },
        [this, roomId](LeaveRoomJob* job) {
            if(!m_roomsLeaving.contains(roomId)){
                return;
            }
            m_roomsLeaving.remove(roomId);
            DBG_INFO << "Failed to leave the room" << job->errorString();
            AppModel::instance()->showToast(job->error(), job->errorString(),
                                            EWARNING::WarningType::EXCEPTION_MSG);
        });
}

void QNunchukRoomListModel::resumePendingSupportRoomRequest()
{
    if(!m_supportRoomRequestPending || !m_roomsHydrated
            || m_supportRequestInProgress || !m_supportRoomsLeaving.isEmpty()){
        return;
    }
    m_supportRoomRequestPending = false;
    const quint64 hydrationGeneration = m_roomHydrationGeneration;
    QTimer::singleShot(0, this, [this, hydrationGeneration] {
        if(hydrationGeneration == m_roomHydrationGeneration && m_roomsHydrated){
            createSupportRoom();
        }
    });
}

void QNunchukRoomListModel::leaveSupportRoom(const QNunchukRoomPtr &roomPtr)
{
    Room* room = roomPtr ? roomPtr->room() : nullptr;
    Connection* activeConnection = connection();
    if(!room || !activeConnection){
        return;
    }
    const QString roomId = room->id();
    if(m_supportRoomsLeaving.contains(roomId)){
        return;
    }
    const QString tagname = supportTagForRoom(room);
    if(tagname.isEmpty()){
        AppModel::instance()->showToast(
            BaseJob::IncorrectResponse,
            QStringLiteral("Unable to identify this Support room"),
            EWARNING::WarningType::EXCEPTION_MSG);
        return;
    }

    m_supportRoomsLeaving.insert(roomId);
    const quint64 hydrationGeneration = m_roomHydrationGeneration;
    QPointer<Connection> attempt = activeConnection;
    QPointer<Room> safeRoom = room;
    const QString expectedAlias = supportRoomAlias(activeConnection, tagname);

    const auto isCurrentSession = [this, attempt, hydrationGeneration] {
        return attempt && attempt == connection()
                && hydrationGeneration == m_roomHydrationGeneration;
    };
    const auto notifyClassification = [this, roomId] {
        const int roomIndex = getIndex(roomId);
        const QNunchukRoomPtr wrapper = getRoomByIndex(roomIndex);
        if(wrapper){
            wrapper->notifySupportClassificationChanged();
        }
    };
    const auto finishFailure =
        [this, roomId, tagname, safeRoom, isCurrentSession,
         notifyClassification](int errorCode, const QString& errorString) {
            if(!isCurrentSession() || !m_supportRoomsLeaving.contains(roomId)){
                return;
            }
            if(safeRoom){
                safeRoom->setProperty("nunchukSupportSuppressedTag", QString{});
                notifyClassification();
            }
            m_supportRoomsLeaving.remove(roomId);
            AppModel::instance()->showToast(
                errorCode,
                errorString.isEmpty()
                    ? QStringLiteral("Unable to leave the Support room")
                    : errorString,
                EWARNING::WarningType::EXCEPTION_MSG);
            resumePendingSupportRoomRequest();
        };
    const auto finishSuccess =
        [this, roomId, tagname, attempt, isCurrentSession] {
            if(!isCurrentSession() || !m_supportRoomsLeaving.contains(roomId)){
                return;
            }
            attempt->removeFromDirectChats(roomId, QStringLiteral("@support:nunchuk.io"));
            attempt->callApi<DeleteRoomTagJob>(attempt->userId(), roomId, tagname);
            m_supportRoomsLeaving.remove(roomId);
            removeRoomById(roomId);
            resumePendingSupportRoomRequest();
        };
    const auto proceedToLeave =
        [this, roomId, tagname, expectedAlias, safeRoom, attempt,
         isCurrentSession, notifyClassification, finishFailure,
         finishSuccess](bool aliasDeleted) {
            if(!isCurrentSession() || !m_supportRoomsLeaving.contains(roomId)){
                return;
            }
            if(!safeRoom || safeRoom->joinState() == JoinState::Leave){
                finishSuccess();
                return;
            }

            safeRoom->setProperty("nunchukSupportSuppressedTag", tagname);
            notifyClassification();

            const auto rollbackLeave =
                [this, roomId, expectedAlias, attempt, isCurrentSession,
                 finishFailure, aliasDeleted](int errorCode,
                                              const QString& errorString) {
                    if(!isCurrentSession() || !m_supportRoomsLeaving.contains(roomId)){
                        return;
                    }
                    if(!aliasDeleted || isAmbiguousMutationError(errorCode)){
                        finishFailure(errorCode, errorString);
                        return;
                    }
                    auto restoreAliasJob = attempt->callApi<SetRoomAliasJob>(
                        expectedAlias, roomId);
                    connect(restoreAliasJob, &BaseJob::finished, this,
                        [finishFailure, restoreAliasJob, errorCode, errorString] {
                            if(restoreAliasJob->error() == BaseJob::Abandoned){
                                finishFailure(errorCode, errorString);
                            }
                        });
                    restoreAliasJob.then(this,
                        [finishFailure, errorCode, errorString](SetRoomAliasJob*) {
                            finishFailure(errorCode, errorString);
                        },
                        [finishFailure, errorCode, errorString](SetRoomAliasJob*) {
                            finishFailure(errorCode, errorString);
                        });
                };

            auto leaveJob = safeRoom->leaveRoom();
            connect(leaveJob, &BaseJob::finished, this,
                [leaveJob, rollbackLeave] {
                    if(leaveJob->error() == BaseJob::Abandoned){
                        rollbackLeave(leaveJob->error(), leaveJob->errorString());
                    }
                });
            leaveJob.then(this,
                [finishSuccess](LeaveRoomJob*) { finishSuccess(); },
                [rollbackLeave](LeaveRoomJob* job) {
                    rollbackLeave(job->error(), job->errorString());
                });
        };

    auto aliasJob = activeConnection->callApi<GetRoomIdByAliasJob>(expectedAlias);
    connect(aliasJob, &BaseJob::finished, this,
        [aliasJob, finishFailure] {
            if(aliasJob->error() == BaseJob::Abandoned){
                finishFailure(aliasJob->error(), aliasJob->errorString());
            }
        });
    aliasJob.then(this,
        [this, roomId, expectedAlias, attempt, isCurrentSession,
         finishFailure, proceedToLeave](GetRoomIdByAliasJob* job) {
            if(!isCurrentSession() || !m_supportRoomsLeaving.contains(roomId)){
                return;
            }
            if(job->roomId() != roomId){
                proceedToLeave(false);
                return;
            }
            auto deleteAliasJob = attempt->callApi<DeleteRoomAliasJob>(expectedAlias);
            connect(deleteAliasJob, &BaseJob::finished, this,
                [deleteAliasJob, finishFailure] {
                    if(deleteAliasJob->error() == BaseJob::Abandoned){
                        finishFailure(deleteAliasJob->error(),
                                      deleteAliasJob->errorString());
                    }
                });
            deleteAliasJob.then(this,
                [proceedToLeave](DeleteRoomAliasJob*) { proceedToLeave(true); },
                [proceedToLeave, finishFailure](DeleteRoomAliasJob* deleteJob) {
                    if(deleteJob->error() == BaseJob::NotFound){
                        proceedToLeave(false);
                    } else {
                        finishFailure(deleteJob->error(), deleteJob->errorString());
                    }
                });
        },
        [proceedToLeave, finishFailure](GetRoomIdByAliasJob* job) {
            if(job->error() == BaseJob::NotFound){
                proceedToLeave(false);
            } else {
                finishFailure(job->error(), job->errorString());
            }
        });
}

void QNunchukRoomListModel::joinRoom(QString roomAliasOrId)
{
    if(connection()){
        auto joinJob = connection()->joinRoom(roomAliasOrId);
        connect(joinJob, &BaseJob::failure, this, [] {
            DBG_INFO << "Failed to join the room";
        });
    }
}

void QNunchukRoomListModel::createRoomChat(const QStringList invitees_id, const QString& room_name, QVariant firstMessage)
{
    QStringList invitees;
    const QString localUserId = connection() ? connection()->userId() : QString{};
    for(const QString& rawId : invitees_id){
        const QString memberId = rawId.trimmed();
        if(!memberId.isEmpty() && memberId != localUserId
                && !invitees.contains(memberId)){
            invitees.append(memberId);
        }
    }
    if(!connection() || invitees.isEmpty()){
        const QString error = tr("Select at least one member before creating a room");
        AppModel::instance()->showToast(BaseJob::IncorrectRequest, error,
                                        EWARNING::WarningType::EXCEPTION_MSG);
        emit roomCreationFailed(error);
        return;
    }
    if(m_roomCreationInProgress){
        return;
    }
    setRoomCreationInProgress(true);
    {
        const quint64 operationGeneration = m_roomOperationGeneration;
        QPointer<Connection> attempt = connection();
        Connection::RoomVisibility in_visibility = Connection::UnpublishRoom;
        const QString   in_alias = {};
        const QString   in_name = room_name;
        const QString   in_topic = {};
        QStringList     in_invites = invitees;
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
        connect(createJob, &BaseJob::finished, this,
                [this, createJob, attempt, operationGeneration] {
            if(!attempt || attempt != connection()
                    || operationGeneration != m_roomOperationGeneration){
                return;
            }
            if(createJob->error() == BaseJob::Abandoned){
                setRoomCreationInProgress(false);
                AppModel::instance()->showToast(createJob->error(), createJob->errorString(),
                                                EWARNING::WarningType::EXCEPTION_MSG);
                emit roomCreationFailed(createJob->errorString());
            }
        });
        createJob.then(this,
            [this, attempt, firstMessage, operationGeneration](CreateRoomJob* job) {
                if(!attempt || attempt != connection()
                        || operationGeneration != m_roomOperationGeneration){
                    return;
                }
                setRoomCreationInProgress(false);
                trackLocallyCreatedRoom(job->roomId());
                postInitialMessageWhenStateReady(job->roomId(), firstMessage,
                                                 operationGeneration);
                emit roomCreationSucceeded(job->roomId());
            },
            [this, attempt, operationGeneration](CreateRoomJob* job) {
                if(!attempt || attempt != connection()
                        || operationGeneration != m_roomOperationGeneration){
                    return;
                }
                setRoomCreationInProgress(false);
                AppModel::instance()->showToast(job->error(), job->errorString(),
                                                EWARNING::WarningType::EXCEPTION_MSG);
                emit roomCreationFailed(job->errorString());
            });
    }
}

void QNunchukRoomListModel::createRoomDirectChat(const QString invitee_id, const QString &invitee_name, QVariant firstMessage)
{
    DBG_INFO << invitee_id << invitee_name << invitee_name;
    const QString targetId = invitee_id.trimmed();
    if(!connection() || targetId.isEmpty() || targetId == connection()->userId()){
        const QString error = tr("Select a member before creating a room");
        AppModel::instance()->showToast(BaseJob::IncorrectRequest, error,
                                        EWARNING::WarningType::EXCEPTION_MSG);
        emit roomCreationFailed(error);
        return;
    }
    if(m_roomCreationInProgress){
        return;
    }
    if(connection()){
        int existingIndex = -1;
        QString existingRoomId;
        for(int index = 0; index < m_data.count(); ++index){
            const QNunchukRoomPtr& candidate = m_data.at(index);
            Room* room = candidate ? candidate->room() : nullptr;
            if(room && room->joinState() == JoinState::Join
                    && room->isDirectChat()
                    && connection()->directChatMemberIds(room).contains(targetId)){
                existingIndex = index;
                existingRoomId = room->id();
                break;
            }
        }
        if(existingIndex >= 0){
            setCurrentIndex(existingIndex);
            const QString message = firstMessage.toString();
            if(!message.isEmpty()){
                const QNunchukRoomPtr existingRoom = getRoomByIndex(existingIndex);
                if(existingRoom){
                    existingRoom->sendMessage(message);
                }
            }
            emit roomCreationSucceeded(existingRoomId);
        } else {
            setRoomCreationInProgress(true);
            const quint64 operationGeneration = m_roomOperationGeneration;
            QPointer<Connection> attempt = connection();
            Connection::RoomVisibility in_visibility = Connection::UnpublishRoom;
            const QString   in_alias = {};
            const QString   in_name = invitee_name;
            const QString   in_topic = {};
            QStringList     in_invites = {targetId};
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
            connect(createJob, &BaseJob::finished, this,
                    [this, createJob, attempt, operationGeneration] {
                if(!attempt || attempt != connection()
                        || operationGeneration != m_roomOperationGeneration){
                    return;
                }
                if(createJob->error() == BaseJob::Abandoned){
                    setRoomCreationInProgress(false);
                    AppModel::instance()->showToast(createJob->error(), createJob->errorString(),
                                                    EWARNING::WarningType::EXCEPTION_MSG);
                    emit roomCreationFailed(createJob->errorString());
                }
            });
            createJob.then(this,
                [this, attempt, firstMessage, operationGeneration](CreateRoomJob* job) {
                    if(!attempt || attempt != connection()
                            || operationGeneration != m_roomOperationGeneration){
                        return;
                    }
                    setRoomCreationInProgress(false);
                    trackLocallyCreatedRoom(job->roomId());
                    postInitialMessageWhenStateReady(job->roomId(), firstMessage,
                                                     operationGeneration);
                    emit roomCreationSucceeded(job->roomId());
                },
                [this, attempt, operationGeneration](CreateRoomJob* job) {
                    if(!attempt || attempt != connection()
                            || operationGeneration != m_roomOperationGeneration){
                        return;
                    }
                    setRoomCreationInProgress(false);
                    AppModel::instance()->showToast(job->error(), job->errorString(),
                                                    EWARNING::WarningType::EXCEPTION_MSG);
                    emit roomCreationFailed(job->errorString());
                });
        }
    }
}

void QNunchukRoomListModel::createRoomByzantineChat(const QStringList invitees_id, const QString &room_name, const QString &group_id, QVariant firstMessage)
{
    DBG_INFO << invitees_id << room_name;
    const QString targetGroupId = group_id.trimmed();
    QStringList invitees;
    const QString localUserId = connection() ? connection()->userId() : QString{};
    for(const QString& rawId : invitees_id){
        const QString memberId = rawId.trimmed();
        if(!memberId.isEmpty() && memberId != localUserId
                && !invitees.contains(memberId)){
            invitees.append(memberId);
        }
    }
    if(!connection() || invitees.isEmpty() || targetGroupId.isEmpty()){
        AppModel::instance()->showToast(
            BaseJob::IncorrectRequest,
            tr("Unable to create group chat because its members or group id are missing"),
            EWARNING::WarningType::EXCEPTION_MSG);
        return;
    }
    if(m_byzantineRoomsCreating.contains(targetGroupId)){
        return;
    }
    for(int index = 0; index < m_data.count(); ++index){
        const QNunchukRoomPtr& candidate = m_data.at(index);
        if(candidate && qUtils::strCompare(candidate->byzantineRoomGroupId(),
                                           targetGroupId)){
            setCurrentIndex(index);
            emit byzantineRoomCreated(candidate->id(), targetGroupId, true);
            return;
        }
    }

    m_byzantineRoomsCreating.insert(targetGroupId);
    const quint64 operationGeneration = m_roomOperationGeneration;
    QPointer<Connection> attempt = connection();
    const Connection::RoomVisibility inVisibility = Connection::UnpublishRoom;
    const QString alias = {};
    const QString topic = {};
    const QString presetName = {};
    const QString roomVersion = {};
    const bool isDirect = invitees.count() == 1;
    CreateRoomJob::StateEvent byzantineEvent;
    byzantineEvent.type = NUNCHUK_ROOM_BYZANTINE;
    byzantineEvent.content["group_id"] = targetGroupId;

    CreateRoomJob::StateEvent retentionEvent;
    retentionEvent.type = NUNCHUK_ROOM_RETENTION;
    retentionEvent.content["max_lifetime"] = NUNCHUK_ROOM_RETENTION_TIME;

    CreateRoomJob::StateEvent encryptionEvent;
    encryptionEvent.type = "m.room.encryption";
    encryptionEvent.content["algorithm"] = "m.megolm.v1.aes-sha2";

    const QVector<CreateRoomJob::StateEvent> initialState = {
        encryptionEvent, byzantineEvent, retentionEvent
    };
    auto createJob = connection()->createRoom(
        inVisibility, alias, room_name, topic, invitees, presetName,
        roomVersion, isDirect, initialState);
    connect(createJob, &BaseJob::finished, this,
            [this, createJob, attempt, targetGroupId, operationGeneration] {
        if(!attempt || attempt != connection()
                || operationGeneration != m_roomOperationGeneration){
            return;
        }
        if(createJob->error() == BaseJob::Abandoned){
            m_byzantineRoomsCreating.remove(targetGroupId);
            AppModel::instance()->showToast(
                createJob->error(), createJob->errorString(),
                EWARNING::WarningType::EXCEPTION_MSG);
        }
    });
    createJob.then(this,
        [this, attempt, firstMessage, targetGroupId,
         operationGeneration](CreateRoomJob* job) {
            if(!attempt || attempt != connection()
                    || operationGeneration != m_roomOperationGeneration){
                return;
            }
            m_byzantineRoomsCreating.remove(targetGroupId);
            trackLocallyCreatedRoom(job->roomId());
            postInitialMessageWhenStateReady(job->roomId(), firstMessage,
                                             operationGeneration);
            emit byzantineRoomCreated(job->roomId(), targetGroupId, false);
        },
        [this, attempt, targetGroupId,
         operationGeneration](CreateRoomJob* job) {
            if(!attempt || attempt != connection()
                    || operationGeneration != m_roomOperationGeneration){
                return;
            }
            m_byzantineRoomsCreating.remove(targetGroupId);
            AppModel::instance()->showToast(
                job->error(), job->errorString(),
                EWARNING::WarningType::EXCEPTION_MSG);
        });
}

void QNunchukRoomListModel::createSupportRoom()
{
    if(m_supportRequestInProgress){
        DBG_INFO << "[SUPPORT] Reusing the support-room request already in progress";
        return;
    }
    if(!m_supportRoomsLeaving.isEmpty()){
        m_supportRoomRequestPending = true;
        DBG_INFO << "[SUPPORT] Deferring support-room request until the current room is left";
        return;
    }
    if(!m_roomsHydrated){
        // A click during the initial /sync must not be lost and must not race
        // room hydration. Run exactly once after downloadRooms completes.
        m_supportRoomRequestPending = true;
        DBG_INFO << "[SUPPORT] Deferring support-room request until rooms are hydrated";
        return;
    }

    if((int)ENUNCHUCK::Chain::MAIN == (int)AppSetting::instance()->primaryServer()
            || (int)ENUNCHUCK::Chain::TESTNET == (int)AppSetting::instance()->primaryServer()
            || (int)ENUNCHUCK::Chain::SIGNET == (int)AppSetting::instance()->primaryServer()
            || (int)ENUNCHUCK::Chain::REGTEST == (int)AppSetting::instance()->primaryServer())
    {
        if(connection()){
            const QString tagname = currentSupportRoomTag();
            const QString otherTag = qUtils::strCompare(tagname, NUNCHUK_ROOM_SUPPORT)
                    ? NUNCHUK_ROOM_SUPPORTTESTNET : NUNCHUK_ROOM_SUPPORT;
            const quint64 requestId = beginSupportRequest();

            // Prefer the authoritative tag for the active chain. An untagged
            // direct chat is only a fallback and must not belong to the other
            // chain. Scan Connection as well as m_data so a synced room cannot
            // be missed during a model update.
            QList<QPair<int, QString>> candidates;
            const auto considerCandidate = [this, &candidates, tagname,
                                             otherTag](Quotient::Room* room) {
                if(!room || room->joinState() == JoinState::Leave){
                    return;
                }

                int score = -1;
                const QString markerTag = supportMarkerTag(room);
                const bool localTagSuppressed =
                        room->property("nunchukSupportSuppressedTag").toString() == tagname;
                const bool isSupportDirectChat = room->isDirectChat()
                        && connection()->directChatMemberIds(room)
                               .contains("@support:nunchuk.io");
                const bool hasCurrentLocalTag = room->tagNames().contains(tagname)
                        && !localTagSuppressed;
                if(room->joinState() == JoinState::Invite
                        && markerTag != tagname
                        && !hasCurrentLocalTag && !isSupportDirectChat){
                    // Never auto-join an invite based only on attacker-owned
                    // custom state or a claimed alias.
                    return;
                }
                if(markerTag == tagname && !localTagSuppressed){
                    score = 120;
                } else if(markerTag == otherTag){
                    return;
                } else if(hasCurrentLocalTag){
                    score = 100;
                } else {
                    if(isSupportDirectChat && !room->tagNames().contains(otherTag)){
                        score = 50;
                    }
                }
                if(score < 0){
                    return;
                }
                if(room->joinState() == JoinState::Join){
                    score += 10;
                } else if(room->joinState() == JoinState::Invite){
                    score += 5;
                }
                if(qUtils::strCompare(room->name(), "Support room")){
                    score += 2;
                }

                const auto existing = std::find_if(candidates.begin(), candidates.end(),
                    [room](const auto& item) { return item.second == room->id(); });
                if(existing == candidates.end()){
                    candidates.append({score, room->id()});
                } else if(score > existing->first){
                    existing->first = score;
                }
            };

            foreach (QNunchukRoomPtr it, m_data) {
                if(it){
                    considerCandidate(it.data()->room());
                }
            }
            for(Quotient::Room* room : connection()->allRooms()){
                considerCandidate(room);
            }

            std::stable_sort(candidates.begin(), candidates.end(),
                [](const auto& lhs, const auto& rhs) { return lhs.first > rhs.first; });
            QStringList candidateIds;
            for(const auto& candidate : std::as_const(candidates)){
                candidateIds.append(candidate.second);
            }

            // Focus a locally authoritative room immediately. Server tag
            // verification still continues below to repair legacy/ghost
            // state, but temporary network trouble must not prevent opening
            // a Support room that is already present in the model.
            for(const QString& candidateId : std::as_const(candidateIds)){
                Quotient::Room* localRoom = connection()->room(
                    candidateId, JoinState::Join);
                if(!localRoom){
                    continue;
                }
                const bool isSuppressed =
                        localRoom->property("nunchukSupportSuppressedTag").toString() == tagname;
                const bool isLocallyAuthoritative = !isSuppressed
                        && (supportMarkerTag(localRoom) == tagname
                            || localRoom->tagNames().contains(tagname)
                            || localRoom->property("nunchukSupportCanonicalTag").toString() == tagname);
                const int localIndex = getIndex(candidateId);
                if(isLocallyAuthoritative && localIndex >= 0){
                    DBG_INFO << "[SUPPORT] Focusing existing local support room:" << candidateId;
                    setCurrentIndex(localIndex);
                    CLIENT_INSTANCE->notifySupportRoomReady();
                    break;
                }
            }

            DBG_INFO << "[SUPPORT] createSupportRoom called, tagname:" << tagname
                     << "candidate count:" << candidateIds.size();
            trySupportRoomCandidates(candidateIds, tagname, otherTag, requestId);
        }
    }
}

bool QNunchukRoomListModel::hasPendingSupportRoomRequest() const
{
    return m_supportRoomRequestPending || m_supportRequestInProgress;
}

quint64 QNunchukRoomListModel::beginSupportRequest()
{
    const quint64 requestId = ++m_supportRequestId;
    m_supportRequestInProgress = true;
    CLIENT_INSTANCE->setReadySupport(false);
    armSupportRequestWatchdog(requestId);
    return requestId;
}

void QNunchukRoomListModel::armSupportRequestWatchdog(quint64 requestId)
{
    const quint64 watchdogId = ++m_supportWatchdogId;
    QTimer::singleShot(30000, this, [this, requestId, watchdogId] {
        if(watchdogId == m_supportWatchdogId && isSupportRequestActive(requestId)){
            if(m_supportCreateRequestId == requestId){
                // Never open a retry while the non-idempotent create-room
                // POST is still non-terminal.
                DBG_INFO << "[SUPPORT] Watchdog is waiting for create-room to finish:" << requestId;
                return;
            }
            DBG_INFO << "[SUPPORT] Request timed out; allowing retry:" << requestId;
            finishSupportRequest(requestId);
        }
    });
}

bool QNunchukRoomListModel::isSupportRequestActive(quint64 requestId) const
{
    return requestId != 0 && requestId == m_supportRequestId;
}

bool QNunchukRoomListModel::ensureSupportRequestActive(quint64 requestId,
                                                       const QString& tagname)
{
    if(!isSupportRequestActive(requestId)){
        return false;
    }
    if(currentSupportRoomTag() != tagname){
        DBG_INFO << "[SUPPORT] Active chain changed while handling request:" << requestId;
        finishSupportRequest(requestId);
        return false;
    }
    return true;
}

void QNunchukRoomListModel::finishSupportRequest(quint64 requestId)
{
    if(!isSupportRequestActive(requestId)){
        return;
    }
    if(m_supportCreateRequestId == requestId){
        m_supportCreateRequestId = 0;
    }
    if(m_supportReconciliationRequestId == requestId){
        m_supportReconciliationRequestId = 0;
    }
    ++m_supportWatchdogId;
    ++m_supportRequestId;
    m_supportRequestInProgress = false;
    CLIENT_INSTANCE->setReadySupport(true);
}

void QNunchukRoomListModel::trySupportRoomCandidates(QStringList candidateIds,
                                                     const QString& tagname,
                                                     const QString& otherTag,
                                                     quint64 requestId,
                                                     const QString& repairCandidateId)
{
    if(!ensureSupportRequestActive(requestId, tagname)){
        return;
    }

    while(!candidateIds.isEmpty()){
        const QString candidateId = candidateIds.takeFirst();
        Quotient::Room* candidate = connection()->room(
            candidateId, JoinState::Invite | JoinState::Join);
        if(!candidate){
            continue;
        }

        if(candidate->joinState() == JoinState::Invite){
            DBG_INFO << "[SUPPORT] Joining invited support-room candidate:" << candidateId;
            auto joinJob = connection()->joinRoom(candidateId);
            joinJob.then(this,
                [this, candidateIds, tagname, otherTag, requestId,
                 repairCandidateId](Quotient::JoinRoomJob* job) {
                    if(!ensureSupportRequestActive(requestId, tagname)){
                        return;
                    }
                    Quotient::Room* joinedCandidate = connection()->room(
                        job->roomId(), JoinState::Join);
                    if(!joinedCandidate){
                        DBG_INFO << "[SUPPORT] Joined candidate is unavailable locally:" << job->roomId();
                        trySupportRoomCandidates(candidateIds, tagname, otherTag,
                                                 requestId, repairCandidateId);
                        return;
                    }
                    verifySupportRoomCandidate(joinedCandidate, tagname, otherTag,
                                               candidateIds, requestId, repairCandidateId);
                },
                [this, candidateIds, tagname, otherTag, requestId,
                 repairCandidateId](Quotient::JoinRoomJob* job) {
                    if(!ensureSupportRequestActive(requestId, tagname)){
                        return;
                    }
                    DBG_INFO << "[SUPPORT] Failed to join support-room candidate:" << job->errorString();
                    if(!candidateIds.isEmpty() || !repairCandidateId.isEmpty()){
                        trySupportRoomCandidates(candidateIds, tagname, otherTag,
                                                 requestId, repairCandidateId);
                    } else {
                        AppModel::instance()->showToast(job->error(), job->errorString(), EWARNING::WarningType::EXCEPTION_MSG);
                        finishSupportRequest(requestId);
                    }
                });
            return;
        }

        verifySupportRoomCandidate(candidate, tagname, otherTag, candidateIds,
                                   requestId, repairCandidateId);
        return;
    }

    if(!repairCandidateId.isEmpty()){
        Quotient::Room* repairCandidate = connection()->room(
            repairCandidateId, JoinState::Join);
        if(repairCandidate){
            DBG_INFO << "[SUPPORT] Reusing best verified untagged candidate:" << repairCandidateId;
            activateSupportRoom(repairCandidate, tagname, requestId, true);
            return;
        }
    }

    DBG_INFO << "[SUPPORT] No valid candidate remains, creating new room";
    doCreateSupportRoom(tagname, requestId);
}

void QNunchukRoomListModel::verifySupportRoomCandidate(Quotient::Room *room,
                                                       const QString& tagname,
                                                       const QString& otherTag,
                                                       QStringList remainingCandidateIds,
                                                       quint64 requestId,
                                                       const QString& repairCandidateId)
{
    if(!ensureSupportRequestActive(requestId, tagname)){
        return;
    }
    if(!room){
        trySupportRoomCandidates(remainingCandidateIds, tagname, otherTag,
                                 requestId, repairCandidateId);
        return;
    }

    QPointer<Quotient::Room> safeCandidate = room;
    const bool hasLocalTag = room->tagNames().contains(tagname)
            && room->property("nunchukSupportSuppressedTag").toString() != tagname;

    // Verify every candidate, including an untagged direct-chat fallback.
    // This prevents a not-yet-hydrated testnet room from being retagged as
    // mainnet (or vice versa).
    DBG_INFO << "[SUPPORT] Verifying tag on server via GetRoomTagsJob for room:" << room->id();
    auto getTagsJob = connection()->callApi<Quotient::GetRoomTagsJob>(
        connection()->userId(), room->id());
    getTagsJob.then(this,
        [this, safeCandidate, tagname, otherTag, remainingCandidateIds,
         requestId, repairCandidateId](Quotient::GetRoomTagsJob* job) {
            if(!ensureSupportRequestActive(requestId, tagname)){
                return;
            }
            if(!safeCandidate){
                trySupportRoomCandidates(remainingCandidateIds, tagname, otherTag,
                                         requestId, repairCandidateId);
                return;
            }
            if(job->tags().contains(tagname)){
                safeCandidate->setProperty("nunchukSupportSuppressedTag", QString{});
                DBG_INFO << "[SUPPORT] Server confirms tag — navigating to room:" << safeCandidate->id();
                activateSupportRoom(safeCandidate, tagname, requestId, false);
            } else if(job->tags().contains(otherTag)){
                if(safeCandidate->tagNames().contains(tagname)
                        || supportMarkerTag(safeCandidate) == tagname){
                    safeCandidate->setProperty("nunchukSupportSuppressedTag", tagname);
                    for(const QNunchukRoomPtr& roomPtr : std::as_const(m_data)){
                        if(roomPtr && roomPtr->room() == safeCandidate){
                            roomPtr->notifySupportClassificationChanged();
                            break;
                        }
                    }
                }
                DBG_INFO << "[SUPPORT] Candidate belongs to the other chain; trying the next room:" << safeCandidate->id();
                trySupportRoomCandidates(remainingCandidateIds, tagname, otherTag,
                                         requestId, repairCandidateId);
            } else {
                if(safeCandidate->tagNames().contains(tagname)
                        || supportMarkerTag(safeCandidate) == tagname){
                    // The local tag is a stale optimistic value from an older
                    // failed PUT. Suppress it immediately without issuing a
                    // second server mutation; a selected repair clears this.
                    safeCandidate->setProperty("nunchukSupportSuppressedTag", tagname);
                    for(const QNunchukRoomPtr& roomPtr : std::as_const(m_data)){
                        if(roomPtr && roomPtr->room() == safeCandidate){
                            roomPtr->notifySupportClassificationChanged();
                            break;
                        }
                    }
                }
                const QString bestRepairCandidate = repairCandidateId.isEmpty()
                        ? safeCandidate->id() : repairCandidateId;
                DBG_INFO << "[SUPPORT] Candidate is untagged; checking for an authoritative room before repair:"
                         << safeCandidate->id();
                trySupportRoomCandidates(remainingCandidateIds, tagname, otherTag,
                                         requestId, bestRepairCandidate);
            }
        },
        [this, safeCandidate, tagname, otherTag, hasLocalTag,
         remainingCandidateIds, requestId, repairCandidateId](Quotient::GetRoomTagsJob* job) {
            if(!ensureSupportRequestActive(requestId, tagname)){
                return;
            }
            DBG_INFO << "[SUPPORT] Failed to verify support tags:" << job->errorString();
            if(safeCandidate && hasLocalTag){
                // The local current-chain tag is the safest fallback when the
                // authoritative check is temporarily offline.
                activateSupportRoom(safeCandidate, tagname, requestId, false);
            } else if(!remainingCandidateIds.isEmpty() || !repairCandidateId.isEmpty()){
                trySupportRoomCandidates(remainingCandidateIds, tagname, otherTag,
                                         requestId, repairCandidateId);
            } else {
                AppModel::instance()->showToast(job->error(), job->errorString(), EWARNING::WarningType::EXCEPTION_MSG);
                finishSupportRequest(requestId);
            }
        });
}

void QNunchukRoomListModel::activateSupportRoom(Quotient::Room *room, const QString& tagname,
                                                quint64 requestId, bool setTagOnServer)
{
    if(!ensureSupportRequestActive(requestId, tagname)){
        return;
    }
    if(!room){
        finishSupportRequest(requestId);
        return;
    }

    QPointer<Quotient::Room> safeRoom = room;
    room->setProperty("nunchukSupportSuppressedTag", QString{});
    if(!room->connection()->directChatMemberIds(room).contains("@support:nunchuk.io")){
        // A successful create response can be lost before Quotient updates
        // m.direct locally. Restore that mapping when reconciliation finds
        // the room through /sync.
        room->connection()->addToDirectChats(room, "@support:nunchuk.io");
    }

    // A legacy account may contain several untagged direct rooms with
    // support. Mark only the verified/selected room as the temporary
    // canonical fallback until the server tag arrives in /sync.
    for(Quotient::Room* knownRoom : room->connection()->allRooms()){
        if(!knownRoom){
            continue;
        }
        const bool isSupportDirectChat = knownRoom->isDirectChat()
                && knownRoom->connection()->directChatMemberIds(knownRoom)
                       .contains("@support:nunchuk.io");
        if(isSupportDirectChat){
            knownRoom->setProperty("nunchukSupportCanonicalTag",
                                   knownRoom == room ? tagname : QString{});
        }
    }
    for(const QNunchukRoomPtr& roomPtr : std::as_const(m_data)){
        if(!roomPtr || !roomPtr->room()){
            continue;
        }
        Quotient::Room* knownRoom = roomPtr->room();
        if(knownRoom->isDirectChat()
                && knownRoom->connection()->directChatMemberIds(knownRoom)
                       .contains("@support:nunchuk.io")){
            roomPtr->notifySupportClassificationChanged();
        }
    }

    const auto navigateToRoom = [this, safeRoom, tagname, requestId] {
        if(!ensureSupportRequestActive(requestId, tagname)){
            return;
        }
        if(!safeRoom){
            finishSupportRequest(requestId);
            return;
        }

        int index = getIndex(safeRoom->id());
        if(index < 0){
            QNunchukRoomPtr roomPtr = QNunchukRoomPtr(new QNunchukRoom(safeRoom), &QObject::deleteLater);
            doAddRoom(roomPtr);
        }
        resort();
        index = getIndex(safeRoom->id());

        if(index >= 0){
            setCurrentIndex(index);
            finishSupportRequest(requestId);
            CLIENT_INSTANCE->notifySupportRoomReady();
        } else {
            DBG_INFO << "[SUPPORT] Room was not added to the model:" << safeRoom->id();
            finishSupportRequest(requestId);
        }
    };

    const auto ensureSupportMember = [this, safeRoom, tagname, requestId, navigateToRoom] {
        if(!ensureSupportRequestActive(requestId, tagname)){
            return;
        }
        if(!safeRoom){
            finishSupportRequest(requestId);
            return;
        }

        const auto supportMemberState = safeRoom->memberState("@support:nunchuk.io");
        if(supportMemberState == Membership::Join || supportMemberState == Membership::Invite){
            navigateToRoom();
            return;
        }

        auto inviteJob = safeRoom->connection()->callApi<Quotient::InviteUserJob>(
            safeRoom->id(), "@support:nunchuk.io");
        connect(inviteJob, &BaseJob::success, this,
            [this, safeRoom, tagname, requestId, navigateToRoom] {
                if(!ensureSupportRequestActive(requestId, tagname)){
                    return;
                }
                if(!safeRoom){
                    finishSupportRequest(requestId);
                    return;
                }
                navigateToRoom();
            });
        connect(inviteJob, &BaseJob::failure, this,
            [this, tagname, requestId, inviteJob] {
                if(!ensureSupportRequestActive(requestId, tagname)){
                    return;
                }
                DBG_INFO << "[SUPPORT] Failed to invite support member:" << inviteJob->errorString();
                AppModel::instance()->showToast(inviteJob->error(), inviteJob->errorString(), EWARNING::WarningType::EXCEPTION_MSG);
                finishSupportRequest(requestId);
            });
    };

    const auto activateAfterState = [this, safeRoom, tagname, requestId,
                                     setTagOnServer, ensureSupportMember] {
        if(!ensureSupportRequestActive(requestId, tagname)){
            return;
        }
        if(!safeRoom){
            finishSupportRequest(requestId);
            return;
        }
        if(safeRoom->localMember().id().isEmpty()){
            DBG_INFO << "[SUPPORT] Cannot activate room without local member state:" << safeRoom->id();
            finishSupportRequest(requestId);
            return;
        }

        if(!setTagOnServer){
            ensureSupportMember();
            return;
        }

        auto setTagJob = safeRoom->connection()->callApi<Quotient::SetRoomTagJob>(
            safeRoom->connection()->userId(), safeRoom->id(), tagname, Quotient::Tag{});
        connect(setTagJob, &BaseJob::success, this,
            [this, safeRoom, tagname, requestId, ensureSupportMember] {
                if(!ensureSupportRequestActive(requestId, tagname)){
                    return;
                }
                if(!safeRoom){
                    finishSupportRequest(requestId);
                    return;
                }
                ensureSupportMember();
            });
        connect(setTagJob, &BaseJob::failure, this,
            [this, tagname, requestId, setTagJob] {
                if(!ensureSupportRequestActive(requestId, tagname)){
                    return;
                }
                DBG_INFO << "[SUPPORT] Failed to persist support tag:" << setTagJob->errorString();
                AppModel::instance()->showToast(setTagJob->error(), setTagJob->errorString(), EWARNING::WarningType::EXCEPTION_MSG);
                finishSupportRequest(requestId);
            });
    };

    if(!room->localMember().id().isEmpty()){
        activateAfterState();
        return;
    }

    DBG_INFO << "[SUPPORT] Waiting for baseStateLoaded before activating room:" << room->id();
    QObject* context = new QObject(this);
    connect(room, &Quotient::Room::baseStateLoaded, context,
        [activateAfterState, context] {
            activateAfterState();
            context->deleteLater();
        }, Qt::SingleShotConnection);
    connect(room, &QObject::destroyed, context,
        [this, tagname, requestId, context] {
            if(ensureSupportRequestActive(requestId, tagname)){
                finishSupportRequest(requestId);
            }
            context->deleteLater();
        }, Qt::SingleShotConnection);
    QTimer::singleShot(30000, context, [this, tagname, requestId, context] {
        if(ensureSupportRequestActive(requestId, tagname)){
            DBG_INFO << "[SUPPORT] Timed out waiting for room state:" << requestId;
            finishSupportRequest(requestId);
        }
        context->deleteLater();
    });
}

void QNunchukRoomListModel::reconcileSupportRoomCreation(
        const QString& tagname, quint64 requestId, int errorCode,
        const QString& errorString)
{
    if(!ensureSupportRequestActive(requestId, tagname)){
        return;
    }
    if(m_supportReconciliationRequestId == requestId){
        return;
    }

    // POST /createRoom has no transaction id. A timeout or broken response
    // can therefore mean that the server created the room even though the
    // client saw a failure. Keep retry disabled until successful /sync rounds
    // have made that server-side result observable.
    m_supportReconciliationRequestId = requestId;
    ++m_supportWatchdogId;
    DBG_INFO << "[SUPPORT] Reconciling an ambiguous create-room result:"
             << requestId << errorCode << errorString;

    QObject* context = new QObject(this);
    context->setProperty("successfulSyncs", 0);
    context->setProperty("finished", false);
    context->setProperty("aliasLookupFinished", false);
    context->setProperty("aliasResolved", false);
    context->setProperty("finalAliasLookupStarted", false);
    context->setProperty("finalAliasDefinitelyMissing", false);
    const QString expectedAlias = supportRoomAlias(connection(), tagname);

    const auto inspectSyncedRooms =
        [this, context, tagname, requestId,
         errorCode, errorString](bool countSuccessfulSync) -> bool {
            if(context->property("finished").toBool()){
                return false;
            }
            if(!ensureSupportRequestActive(requestId, tagname)){
                context->setProperty("finished", true);
                context->deleteLater();
                return false;
            }

            const QString otherTag = qUtils::strCompare(tagname, NUNCHUK_ROOM_SUPPORT)
                    ? NUNCHUK_ROOM_SUPPORTTESTNET : NUNCHUK_ROOM_SUPPORT;
            Quotient::Room* reconciledRoom = nullptr;
            for(Quotient::Room* room : connection()->allRooms()){
                if(!room || room->joinState() == JoinState::Leave
                        || room->tagNames().contains(otherTag)){
                    continue;
                }
                const QString markerTag = supportMarkerTag(room);
                const QString expectedRoomId = context->property("expectedRoomId").toString();
                const bool matchesUniqueIdentity = markerTag == tagname
                        && room->property("nunchukSupportSuppressedTag").toString() != tagname
                        && (expectedRoomId.isEmpty() || room->id() == expectedRoomId);
                const bool hasVerifiedLocalTag = room->tagNames().contains(tagname)
                        && room->property("nunchukSupportSuppressedTag").toString() != tagname;
                if(matchesUniqueIdentity || hasVerifiedLocalTag){
                    reconciledRoom = room;
                    break;
                }
            }

            if(reconciledRoom){
                DBG_INFO << "[SUPPORT] Recovered room after ambiguous create:"
                         << reconciledRoom->id();
                context->setProperty("finished", true);
                context->deleteLater();
                m_supportReconciliationRequestId = 0;
                activateSupportRoom(reconciledRoom, tagname, requestId, true);
                return false;
            }

            if(countSuccessfulSync){
                context->setProperty(
                    "successfulSyncs",
                    context->property("successfulSyncs").toInt() + 1);
            }
            if(context->property("successfulSyncs").toInt() < 2
                    || !context->property("aliasLookupFinished").toBool()
                    || context->property("aliasResolved").toBool()){
                return false;
            }

            // The first alias lookup can race a late server-side commit of the
            // timed-out POST. Re-resolve once after two settled sync rounds;
            // only that final lookup is allowed to produce a negative result.
            if(!context->property("finalAliasLookupStarted").toBool()){
                context->setProperty("finalAliasLookupStarted", true);
                context->setProperty("aliasLookupFinished", false);
                return true;
            }
            if(!context->property("finalAliasDefinitelyMissing").toBool()){
                // A transport/authentication failure cannot prove that the
                // alias is absent. Keep the request locked until the bounded
                // reconciliation deadline instead of reporting a false
                // negative and inviting an unnecessary retry.
                return false;
            }

            // The deterministic alias is created atomically with the room and
            // is unique on the homeserver. A retry uses the same alias, so it
            // cannot create a second room even if the first POST commits late.
            DBG_INFO << "[SUPPORT] Reconciliation found no created room after two syncs:"
                     << requestId;
            context->setProperty("finished", true);
            context->deleteLater();
            AppModel::instance()->showToast(
                errorCode,
                errorString.isEmpty()
                    ? QStringLiteral("Unable to confirm support-room creation")
                    : errorString,
                EWARNING::WarningType::EXCEPTION_MSG);
            finishSupportRequest(requestId);
            return false;
        };

    const auto startFinalAliasLookup =
        [this, context, tagname, requestId, expectedAlias,
         inspectSyncedRooms] {
            if(context->property("finished").toBool()
                    || !ensureSupportRequestActive(requestId, tagname)){
                return;
            }
            DBG_INFO << "[SUPPORT] Rechecking support alias after settled syncs:"
                     << expectedAlias;
            auto finalAliasJob =
                connection()->callApi<Quotient::GetRoomIdByAliasJob>(
                    expectedAlias);
            connect(finalAliasJob, &BaseJob::success, context,
                [context, finalAliasJob, inspectSyncedRooms] {
                    context->setProperty("aliasLookupFinished", true);
                    context->setProperty("aliasResolved", true);
                    context->setProperty("expectedRoomId", finalAliasJob->roomId());
                    inspectSyncedRooms(false);
                });
            connect(finalAliasJob, &BaseJob::failure, context,
                [context, finalAliasJob, inspectSyncedRooms] {
                    context->setProperty("aliasLookupFinished", true);
                    context->setProperty("aliasResolved", false);
                    context->setProperty(
                        "finalAliasDefinitelyMissing",
                        finalAliasJob->error() == BaseJob::NotFound);
                    inspectSyncedRooms(false);
                });
        };

    // loadedRoomState is the semantic notification that Room::updateData has
    // applied the first state/name/member/marker payload. Use it for positive
    // detection; it must not count as a sync round because one /sync can load
    // any number of rooms (or none at all).
    connect(connection(), &Connection::loadedRoomState, context,
        [context, inspectSyncedRooms, startFinalAliasLookup](Room* room) {
            if(!room || context->property("finished").toBool()){
                return;
            }
            DBG_INFO << "[SUPPORT] Reconciliation observed hydrated room:"
                     << room->id();
            if(inspectSyncedRooms(false)){
                startFinalAliasLookup();
            }
        });
    connect(connection(), &Connection::syncDone, context,
        [context, inspectSyncedRooms, startFinalAliasLookup] {
            // Quotient queues every Room::updateData before emitting syncDone.
            // Queue one MetaCall behind those updates so an empty/negative sync
            // is counted only after loadedRoomState had a chance to recover the
            // room. This is an ordering barrier, not a timer heuristic.
            QMetaObject::invokeMethod(context,
                [context, inspectSyncedRooms, startFinalAliasLookup] {
                    if(!context->property("finished").toBool()
                            && inspectSyncedRooms(true)){
                        startFinalAliasLookup();
                    }
                }, Qt::QueuedConnection);
        });
    auto resolveAliasJob = connection()->callApi<Quotient::GetRoomIdByAliasJob>(
        expectedAlias);
    connect(resolveAliasJob, &BaseJob::success, context,
        [context, resolveAliasJob, inspectSyncedRooms] {
            context->setProperty("aliasLookupFinished", true);
            context->setProperty("aliasResolved", true);
            context->setProperty("expectedRoomId", resolveAliasJob->roomId());
            // If the room is already hydrated, recover immediately; otherwise
            // the loadedRoomState listener above will inspect it after /sync.
            inspectSyncedRooms(false);
        });
    connect(resolveAliasJob, &BaseJob::failure, context,
        [context, inspectSyncedRooms, startFinalAliasLookup] {
            context->setProperty("aliasLookupFinished", true);
            context->setProperty("aliasResolved", false);
            // This may be the last outstanding condition after two settled
            // sync rounds; re-evaluate without counting another round.
            if(inspectSyncedRooms(false)){
                startFinalAliasLookup();
            }
        });

    // Reconciliation must not keep the Support action busy forever if syncing
    // stops. Two SyncJob long-polls can consume about a minute, so leave a
    // small margin while still keeping the recovery bounded.
    QTimer::singleShot(75000, context,
        [this, context, tagname, requestId, errorCode, errorString,
         inspectSyncedRooms] {
            if(context->property("finished").toBool()){
                return;
            }
            inspectSyncedRooms(false);
            if(context->property("finished").toBool()){
                return;
            }
            if(!ensureSupportRequestActive(requestId, tagname)){
                context->setProperty("finished", true);
                context->deleteLater();
                return;
            }

            DBG_INFO << "[SUPPORT] Reconciliation timed out:" << requestId;
            context->setProperty("finished", true);
            context->deleteLater();
            AppModel::instance()->showToast(
                errorCode,
                context->property("aliasResolved").toBool()
                    ? QStringLiteral("Support room exists but its state was not received from Matrix")
                    : (errorString.isEmpty()
                        ? QStringLiteral("Unable to confirm support-room creation")
                        : errorString),
                EWARNING::WarningType::EXCEPTION_MSG);
            finishSupportRequest(requestId);
        });
    if(inspectSyncedRooms(false)){
        startFinalAliasLookup();
    }
}

void QNunchukRoomListModel::doCreateSupportRoom(const QString& tagname, quint64 requestId)
{
    if(!ensureSupportRequestActive(requestId, tagname)){
        return;
    }
    DBG_INFO << "[SUPPORT_TRACE] doCreateSupportRoom tagname:" << tagname
             << "connection:" << (connection() ? connection()->userId() : "NULL");
    // Support must remain a plaintext Matrix room even though the connection
    // has E2EE enabled for normal chat rooms. Do not add m.room.encryption here.
    QVector<CreateRoomJob::StateEvent> initialStateEvents;
    initialStateEvents.append({
        QString::fromLatin1(NUNCHUK_SUPPORT_MARKER_EVENT),
        QJsonObject{{"tag", tagname}, {"version", 1}}
    });

    // A deterministic, private alias gives create-room server-enforced
    // uniqueness for this Matrix user and chain. Retrying a POST whose result
    // was lost can therefore never create another support room.
    auto createJob = connection()->createRoom(
        Connection::UnpublishRoom,
        supportRoomAliasLocalpart(connection(), tagname),
        QStringLiteral("Support room"), {},
        {QStringLiteral("@support:nunchuk.io")},
        QStringLiteral("trusted_private_chat"), {}, true,
        initialStateEvents);
    DBG_INFO << "[SUPPORT_TRACE] create support-room job created:"
             << (createJob ? "OK" : "NULL");
    if(!createJob){
        finishSupportRequest(requestId);
        return;
    }
    m_supportCreateRequestId = requestId;
    armSupportRequestWatchdog(requestId);
    // The create-room POST must reach a terminal state before the 30-second
    // request watchdog opens the UI for a retry; otherwise two outstanding
    // POSTs could create duplicate rooms.
    createJob->setBackoffStrategy(Quotient::JobBackoffStrategy{
        {std::chrono::seconds{25}}, {std::chrono::seconds{1}}, 0
    });
    connect(createJob, &BaseJob::finished, this,
        [this, tagname, requestId](BaseJob* job) {
            if(ensureSupportRequestActive(requestId, tagname)
                    && m_supportCreateRequestId == requestId){
                m_supportCreateRequestId = 0;
                if(job->error() == BaseJob::Abandoned){
                    DBG_INFO << "[SUPPORT] createDirectChat was abandoned";
                    reconcileSupportRoomCreation(
                        tagname, requestId, job->error(), job->errorString());
                } else {
                    // The POST is terminal; give the local-room/tag/invite
                    // continuation its own bounded activation window.
                    armSupportRequestWatchdog(requestId);
                }
            }
        });
    createJob.then(this,
        [this, tagname, requestId](Quotient::CreateRoomJob* job) {
            if(!ensureSupportRequestActive(requestId, tagname)){
                return;
            }
            DBG_INFO << "[SUPPORT] createDirectChat success, roomId:" << job->roomId();
            // This continuation is chained after Quotient's internal
            // provideRoom/addToDirectChats continuations.
            Quotient::Room *newroom = connection()->room(job->roomId(), JoinState::Join);
            if(newroom){
                DBG_INFO << "[SUPPORT] newroom id:" << newroom->id() << "localMemberId:" << newroom->localMember().id();
                activateSupportRoom(newroom, tagname, requestId, true);
            } else {
                DBG_INFO << "[SUPPORT] WARNING: newroom is null after createDirectChat";
                reconcileSupportRoomCreation(
                    tagname, requestId, BaseJob::IncorrectResponse,
                    QStringLiteral("Created support room is not available locally"));
            }
        },
        [this, tagname, requestId](Quotient::CreateRoomJob* job) {
            if(!ensureSupportRequestActive(requestId, tagname)){
                return;
            }
            DBG_INFO << "[SUPPORT] create support room failed:" << job->errorString();
            if(isAmbiguousMutationError(job->error())
                    || job->error() == BaseJob::IncorrectRequest){
                reconcileSupportRoomCreation(
                    tagname, requestId, job->error(), job->errorString());
                return;
            }
            AppModel::instance()->showToast(job->error(), job->errorString(), EWARNING::WarningType::EXCEPTION_MSG);
            finishSupportRequest(requestId);
        });
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
    QNunchukRoomPtr target;
    if(!room_id.isEmpty()){
        target = getRoomById(room_id);
    }
    if(!target && !group_id.isEmpty()){
        for(const QNunchukRoomPtr& room : m_data){
            if(room && qUtils::strCompare(room->byzantineRoomGroupId(), group_id)){
                target = room;
                break;
            }
        }
    }
    const QString requestedName = newname.trimmed();
    if(!target || requestedName.isEmpty()
            || target->roomNameChangeInProgress()
            || qUtils::strCompare(target->roomName(), requestedName)){
        return;
    }
    const QString targetRoomId = target->id();
    QObject* renameContext = new QObject(this);
    connect(target.data(), &QNunchukRoom::roomNameChangeSucceeded, renameContext,
            [this, renameContext, targetRoomId, group_id] {
        emit byzantineRoomRenamed(targetRoomId, group_id);
        renameContext->deleteLater();
    });
    connect(target.data(), &QNunchukRoom::roomNameChangeFailed, renameContext,
            [renameContext](const QString&) { renameContext->deleteLater(); });
    target->setRoomName(requestedName);
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
                if(!newroom->localMember().id().isEmpty()){
                    newroom->addTag(NUNCHUK_ROOM_SYNC);
                } else {
                    QObject* ctx = new QObject(this);
                    connect(newroom, &Quotient::Room::baseStateLoaded, ctx,
                        [newroom, ctx](){
                            if(!newroom->localMember().id().isEmpty()){
                                newroom->addTag(NUNCHUK_ROOM_SYNC);
                            }
                            ctx->deleteLater();
                        });
                }
            }
        });
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
}

void QNunchukRoomListModel::invitedRoom(Room *room, Room *prev)
{
    DBG_INFO << room->name() << (int)room->joinState();
    joinRoom(room->id());
}

void QNunchukRoomListModel::joinedRoom(Room *room, Room *prev)
{
    if(!room){
        return;
    }
    DBG_INFO << "[JOINED_ROOM] id:" << room->id() << "name:" << room->name() << "tags:" << room->tagNames();
    QPointer<Room> safeRoom = room;
    const auto scheduleAddRoom = [this, safeRoom] {
        if(!safeRoom){
            return;
        }
        DBG_INFO << "[JOINED_ROOM] state ready for room:" << safeRoom->id()
                 << "tags:" << safeRoom->tagNames()
                 << "localMember:" << safeRoom->localMember().id();
        const bool locallyCreated = m_locallyCreatedRoomIds.remove(safeRoom->id());
        if(containsRoomId(safeRoom->id()) || containsServiceRoom(safeRoom->id())){
            if(locallyCreated && containsRoomId(safeRoom->id())){
                const int existingIndex = getIndex(safeRoom->id());
                if(existingIndex >= 0){
                    setCurrentIndex(existingIndex);
                }
            }
            return;
        }

        QNunchukRoomPtr newRoom = QNunchukRoomPtr(
            new QNunchukRoom(safeRoom), &QObject::deleteLater);
        const bool selectNewRoom = locallyCreated
                && !newRoom->isServerNoticeRoom()
                && !newRoom->isNunchukSyncRoom()
                && !newRoom->isSupportRoom();
        doAddRoom(newRoom);
        if(selectNewRoom){
            resort();
            const int addedIndex = getIndex(safeRoom->id());
            if(addedIndex >= 0){
                setCurrentIndex(addedIndex);
            }
        }
    };

    if(!room->localMember().id().isEmpty()){
        scheduleAddRoom();
    } else {
        connect(room, &Room::baseStateLoaded, this, scheduleAddRoom, Qt::SingleShotConnection);
    }
}

void QNunchukRoomListModel::leftRoom(Room *room, Room *prev)
{
    DBG_INFO << room->id() << (int)room->joinState();
    if(room && connection()){
        removeRoomById(room->id());
    }
}

void QNunchukRoomListModel::aboutToDeleteRoom(Room *room)
{
    DBG_INFO << room->name() << (int)room->joinState();
    if(room && connection()){
        removeRoomById(room->id());
    }
}

void QNunchukRoomListModel::loadedRoomState(Room *room)
{
    DBG_INFO << "loadedRoomState" << room->name() << (int)room->joinState() << room->tagNames();
}

void QNunchukRoomListModel::refresh(QNunchukRoomPtr room, const QVector<int> &roles)
{
    const auto it = std::find(m_data.begin(), m_data.end(), room);
    if (it == m_data.end()) {
        return;
    }
    const auto idx = index(it - m_data.begin());
    emit dataChanged(idx, idx, roles);
}

void QNunchukRoomListModel::resort()
{
    if (m_data.count() < 2){
        return;
    }
    beginResetModel();
    std::sort(m_data.begin(), m_data.end(), sortRoomListByTimeDescending);
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
