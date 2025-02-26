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
#include "QConversationModel.h"
#include <QDateTime>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlQuery>
#include "Servers/Draco.h"
#include "QConversationModel.h"
#include "QOutlog.h"
#include "localization/STR_CPP.h"

QConversationModel::QConversationModel(Room *r):m_room(r),m_currentIndex(0),m_initConsShow(true),
    m_pinTransaction(nullptr)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
    if(m_room){
        QQmlEngine::setObjectOwnership(m_room, QQmlEngine::CppOwnership);
    }
    connect(&m_RetentionTimer, SIGNAL(timeout()), this, SLOT(processingRetentionMessage()));
}

QConversationModel::~QConversationModel()
{
    disconnect();
    stopCountdown();
}

int QConversationModel::rowCount(const QModelIndex &parent) const
{
    return m_data.count();
}

QVariant QConversationModel::data(const QModelIndex &index, int role) const
{
    if(index.row() >= m_data.size() || m_room == nullptr) return QVariant("");
    Conversation cons = m_data.at(index.row());
    switch (role) {
    case role_sender:{
        QString name = cons.sender;
        if(m_room){
        User* sender = m_room->user(cons.senderId);
            if(sender) name = sender->displayname(m_room) != "" ? sender->displayname(m_room) : sender->id();
        }
        return name;
    }
    case role_receiver:
        return cons.receiver;
    case role_timestamp:
        return QDateTime::fromTime_t(cons.timestamp).toString( "dd-MMM-yyyy hh:mm AP");
    case role_timesection:{
        if(cons.messageType == (int)ENUNCHUCK::ROOM_EVT::EXCEPTION && !AppSetting::instance()->enableDebug()) return "";
        QDateTime today = QDateTime::currentDateTime();
        QDateTime day = QDateTime::fromTime_t(cons.timestamp);
        if(today.date().year() == day.date().year()){
            qint64 numberDay = day.daysTo(today);
            if(numberDay == 0){
                if(m_firstToday.first() == cons.timestamp){
                    return QString("Today, %1").arg(day.toString("hh:mm AP"));
                }
                else{
                    return day.toString("hh:mm AP");
                }
            }
            else if(numberDay <= 7 && numberDay > 0){
                if(today.date().dayOfWeek() > day.date().dayOfWeek()){
                    return day.toString("ddd, hh:mm AP");
                }else{
                    return day.toString("MMMM dd, hh:mm AP");
                }
            }
            else{
                return day.toString("MMMM dd, hh:mm AP");
            }
        }
        else{
            return day.toString("ddd, MMMM dd, yyyy, hh:mm AP");
        }
    }
    case role_message:{
        QString message = cons.message;
        switch (cons.messageType) {
        case (int)ENUNCHUCK::ROOM_EVT::WALLET_JOIN:
        case (int)ENUNCHUCK::ROOM_EVT::WALLET_LEAVE:
        case (int)ENUNCHUCK::ROOM_EVT::WALLET_CANCEL:
        case (int)ENUNCHUCK::ROOM_EVT::TX_SIGN:
        case (int)ENUNCHUCK::ROOM_EVT::TX_BROADCAST:
        case (int)ENUNCHUCK::ROOM_EVT::TX_CANCEL:
        case (int)ENUNCHUCK::ROOM_EVT::STATE_EVT:{
            if(m_room){
                User* sender = m_room->user(cons.senderId);
                if(sender) {
                    QString picName = sender->displayname(m_room) != "" ? sender->displayname(m_room) : sender->id();
                    message = QString("<b>%1</b> %2").arg(picName).arg(cons.message);
                }
            }
            break;
        }
        default:break;
        }
        return message;
    }
    case role_avartar:
        return cons.avatar;
    case role_sendByMe:
        return cons.sendByMe;
    case role_istateEvent:
        return cons.isStateEvent;
    case role_messageType:
        return cons.messageType;
    case role_init_event_id:
        return cons.init_event_id;
    case role_transaction:
        return QVariant::fromValue(cons.transaction.data());
    case role_evtId:
        return cons.evtId;
    case role_txnId:
        return cons.txnId;
    case role_file_path:
        return cons.file_path;
    case role_progressInfo:
        return QVariant::fromValue(m_room->fileTransferInfo(cons.evtId));
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> QConversationModel::roleNames() const
{
    QHash<int, QByteArray> names;
    names[role_sendByMe]  = "sendByMe";
    names[role_sender]    = "sender";
    names[role_receiver]  = "receiver";
    names[role_timestamp] = "timestamp";
    names[role_timesection] = "timesection";
    names[role_message]   = "message";
    names[role_avartar]   = "avatar";
    names[role_istateEvent] = "isStateEvent";
    names[role_messageType] = "messageType";
    names[role_init_event_id] = "init_event_id";
    names[role_transaction] = "transaction";
    names[role_evtId] = "evtId";
    names[role_txnId] = "txnId";
    names[role_file_path] = "file_path";
    names[role_progressInfo] = "progressInfo";
    return names;
}

void QConversationModel::addMessage(const Conversation data)
{
    if(needIgnoreInSupportRoom(data)){
        return;
    }
    QDateTime today = QDateTime::currentDateTime();
    QDateTime day = QDateTime::fromTime_t(data.timestamp);
    qint64 numberDay = day.daysTo(today);
    if(numberDay == 0){
        m_firstToday.push_back(data.timestamp);
    }
    beginInsertRows(QModelIndex(), m_data.size(), m_data.size());
    if(data.messageType == (int)ENUNCHUCK::ROOM_EVT::TX_READY){
        if(!containsTxReadyMessage(data)){
            m_data.append(data);
        }
    }
    else if(data.messageType == (int)ENUNCHUCK::ROOM_EVT::WALLET_READY){
        if(!containsWalletReadyMessage(data)){
            m_data.append(data);
        }
    }
    else{
        m_data.append(data);
        if((int)ENUNCHUCK::ROOM_EVT::WALLET_CREATE == data.messageType){
            Conversation backup;
            backup.messageType = (int)ENUNCHUCK::ROOM_EVT::WALLET_BACKUP;
            backup.message = STR_CPP_001;
            backup.timestamp = data.timestamp+1;
            backup.init_event_id = data.init_event_id;
            m_data.append(backup);
        }
    }
    endInsertRows();
    setCurrentIndex(lastIndex());
    emit countChanged();
}

void QConversationModel::addHistoryMessage(const Conversation data)
{
    if(needIgnoreInSupportRoom(data)){
        return;
    }
    QDateTime today = QDateTime::currentDateTime();
    QDateTime day = QDateTime::fromTime_t(data.timestamp);
    qint64 numberDay = day.daysTo(today);
    if(numberDay == 0){
        m_firstToday.push_back(data.timestamp);
    }
    if(data.messageType == (int)ENUNCHUCK::ROOM_EVT::TX_READY){
        if(!containsTxReadyMessage(data)){
            m_data.append(data);
        }
    }
    else if(data.messageType == (int)ENUNCHUCK::ROOM_EVT::WALLET_READY){
        if(!containsWalletReadyMessage(data)){
            m_data.append(data);
        }
    }
    else{
        m_data.append(data);
        if((int)ENUNCHUCK::ROOM_EVT::WALLET_CREATE == data.messageType){
            Conversation backup;
            backup.messageType = (int)ENUNCHUCK::ROOM_EVT::WALLET_BACKUP;
            backup.message = STR_CPP_001;
            backup.timestamp = data.timestamp+1;
            backup.init_event_id = data.init_event_id;
            m_data.append(backup);
        }
    }
}

void QConversationModel::insertMessage(int index, const Conversation data)
{
    if(data.messageType == (int)ENUNCHUCK::ROOM_EVT::TX_READY){
        if(!containsTxReadyMessage(data)){
            m_data.insert(index, data);
        }
    }
    else if(data.messageType == (int)ENUNCHUCK::ROOM_EVT::WALLET_READY){
        if(!containsWalletReadyMessage(data)){
            m_data.insert(index, data);
        }
    }
    else{
        m_data.insert(index, data);
        if((int)ENUNCHUCK::ROOM_EVT::WALLET_CREATE == data.messageType){
            Conversation backup;
            backup.messageType = (int)ENUNCHUCK::ROOM_EVT::WALLET_BACKUP;
            backup.message = STR_CPP_001;
            backup.timestamp = data.timestamp+1;
            backup.init_event_id = data.init_event_id;
            m_data.insert(index, data);
        }
    }
    setCurrentIndex(lastIndex());
    emit countChanged();
}

void QConversationModel::requestSortByTimeAscending(bool ui_update)
{
    if(ui_update){
        beginResetModel();
    }
    if(m_data.size() > 1){
        qSort(m_data.begin(), m_data.end(), sortConversationByTimeAscending);
    }
    if(m_firstToday.size() > 1){
        qSort(m_firstToday.begin(), m_firstToday.end(), [](const time_t &t1,const time_t &t2)->bool{
                  return t1 < t2;
              });
    }
    if(ui_update){
        endResetModel();
    }
}

bool QConversationModel::isWalletCreator(const QString &init_event_id)
{
    foreach (Conversation it , m_data ){
        if(qUtils::strCompare(init_event_id, it.init_event_id)){
            return it.sendByMe;
        }
    }
    return false;
}

int QConversationModel::currentIndex() const
{
    return m_currentIndex;
}

void QConversationModel::setCurrentIndex(int currentIndex)
{
    if(m_currentIndex != currentIndex){
        m_currentIndex = currentIndex;
        emit currentIndexChanged();
    }
}

void QConversationModel::updateTransaction(const Conversation data, const QRoomTransactionPtr &tx)
{
    if(tx){
        bool createByMe = true;
        m_pinTransaction = nullptr;
        for (int i = 0; i < m_data.count(); i++) {
            if(!qUtils::strCompare(NUNCHUK_EVENT_TRANSACTION, m_data.at(i).matrixType)){
                continue;
            }
            if((qUtils::strCompare(data.evtId, m_data.at(i).evtId))
                    || (qUtils::strCompare(tx.data()->get_init_event_id(), m_data.at(i).init_event_id)))
            {
                if(m_data.at(i).messageType == (int)ENUNCHUCK::ROOM_EVT::TX_INIT){
                    createByMe = m_data.at(i).sendByMe;
                }
                if(tx.data()->transaction()){
                    tx.data()->transaction()->setCreateByMe(createByMe);
                }
                if(tx.data()->get_cancel_event_id() != "" && m_data.at(i).messageType == (int)ENUNCHUCK::ROOM_EVT::TX_INIT){
                    m_data[i].messageType = (int)ENUNCHUCK::ROOM_EVT::TX_CANCELED;
                }
                if((m_data.at(i).messageType == (int)ENUNCHUCK::ROOM_EVT::TX_SIGN && data.messageType == (int)ENUNCHUCK::ROOM_EVT::TX_SIGN)
                        || (m_data.at(i).messageType == (int)ENUNCHUCK::ROOM_EVT::TX_BROADCAST && data.messageType == (int)ENUNCHUCK::ROOM_EVT::TX_BROADCAST)
                        || (m_data.at(i).messageType == (int)ENUNCHUCK::ROOM_EVT::TX_RECEIVE && data.messageType == (int)ENUNCHUCK::ROOM_EVT::TX_RECEIVE))
                {
                    m_data[i].message = data.message;
                    m_data[i].init_event_id = data.init_event_id;
                }
                m_data[i].transaction = tx;
                emit dataChanged(index(i),index(i));
            }
            QRoomTransactionPtr ptr = m_data[i].transaction;
            if(ptr){
                QTransactionPtr tx_tmp = ptr->transactionPtr();
                if(tx_tmp){
                    if(tx_tmp->status() == (int)ENUNCHUCK::TransactionStatus::PENDING_SIGNATURES
                            || tx_tmp->status() == (int)ENUNCHUCK::TransactionStatus::READY_TO_BROADCAST
                            || tx_tmp->status() == (int)ENUNCHUCK::TransactionStatus::PENDING_CONFIRMATION)
                    {
                        m_pinTransaction = ptr;
                    }
                }
            }
        }
    }
}

void QConversationModel::updateCancelWallet(const QString &init_event_id)
{
    for (int i = 0; i < m_data.count(); i++) {
        if((int)ENUNCHUCK::ROOM_EVT::WALLET_INIT != m_data.at(i).messageType){
            continue;
        }
        else{
            if(qUtils::strCompare(init_event_id, m_data.at(i).init_event_id)){
                m_data[i].messageType = (int)ENUNCHUCK::ROOM_EVT::WALLET_PAST;
                emit dataChanged(index(i),index(i));
            }
        }
    }
}

void QConversationModel::updateCancelTransaction(const Conversation data)
{
    for (int i = 0; i < m_data.count(); i++) {
        if(qUtils::strCompare(NUNCHUK_EVENT_TRANSACTION, m_data.at(i).matrixType)){
            if((qUtils::strCompare(data.evtId, m_data.at(i).evtId))
                    || (qUtils::strCompare(data.init_event_id, m_data.at(i).init_event_id))){
                if(m_data[i].messageType == (int)ENUNCHUCK::ROOM_EVT::TX_INIT){
                    m_data[i].messageType = (int)ENUNCHUCK::ROOM_EVT::TX_CANCELED;
                    emit dataChanged(index(i),index(i));
                }
            }
        }
    }
}

void QConversationModel::transactionChanged(const QString &tx_id, const int status, const int height)
{
    for (int i = 0; i < m_data.count(); i++) {
        if(m_data.at(i).transaction){
            if(qUtils::strCompare(tx_id, m_data.at(i).transaction.data()->get_tx_id()) && m_data.at(i).transaction.data()->transaction()){
                m_data[i].transaction.data()->transaction()->setStatus(status);
                m_data[i].transaction.data()->transaction()->setHeight(height);
                emit dataChanged(index(i),index(i));
            }
        }
    }
}

bool QConversationModel::isDuplicateTransactionEvent(const QString &tx_id, const int messageType, const QString& init_event_id)
{
    foreach (Conversation it , m_data ){
        if(it.transaction){
            if(it.messageType == messageType
                    && (qUtils::strCompare(it.transaction.data()->get_tx_id(), tx_id))
                    && (qUtils::strCompare(it.transaction.data()->get_init_event_id(), init_event_id)))
            {
                return true;
            }
        }
    }
    return false;
}

void QConversationModel::updateTransactionMemo(const QString &tx_id, const QString &memo)
{
    for (int i = 0; i < m_data.count(); i++) {
        if(m_data.at(i).transaction && m_data.at(i).transaction.data()->transaction()){
            if(qUtils::strCompare(tx_id, m_data.at(i).transaction.data()->get_tx_id())){
                m_data[i].transaction.data()->transaction()->setMemo(memo);
                emit dataChanged(index(i),index(i));
            }
        }
    }
}

void QConversationModel::updateInitEventId(const Conversation data)
{
    for (int i = 0; i < m_data.count(); i++) {
        if(qUtils::strCompare(data.evtId, m_data.at(i).evtId)){
            m_data[i].init_event_id = data.init_event_id;
            m_data[i].message = data.message;
            emit dataChanged(index(i),index(i));
        }
    }
}

void QConversationModel::messageSent(QString txnId, QString eventId)
{
    for (int i = 0; i < m_data.count(); i++) {
        if(m_data.at(i).sendByMe && qUtils::strCompare(txnId, m_data.at(i).txnId)){
            m_data[i].evtId = eventId;
            emit dataChanged(index(i),index(i));
        }
    }
}

Conversation QConversationModel::getConversation(const QString &eventId)
{
    foreach(Conversation con, m_data){
        if(con.senderId.compare(eventId,Qt::CaseInsensitive) == 0){
            return  con;
        }
    }
    return Conversation();
}

Conversation QConversationModel::lastMessage()
{
    Conversation lastMessage;
    for(int i = m_data.size() - 1; i >= 0; i--){
        lastMessage = m_data.at(i);
        if(lastMessage.messageType == (int)ENUNCHUCK::ROOM_EVT::WALLET_PAST
                || lastMessage.messageType == (int)ENUNCHUCK::ROOM_EVT::INVALID
                || lastMessage.messageType == (int)ENUNCHUCK::ROOM_EVT::EXCEPTION ) {
            continue;
        }
        else{
            return m_data[i];
        }
    }
    return Conversation();
}

Conversation QConversationModel::lastTime()
{
    Conversation lastTime;
    for(int i = m_data.size() - 1; i >= 0; i--){
        lastTime = m_data.at(i);
        if(lastTime.messageType != (int)ENUNCHUCK::ROOM_EVT::INVALID &&
                lastTime.messageType != (int)ENUNCHUCK::ROOM_EVT::EXCEPTION)
        {
            break;
        }
    }
    return lastTime;
}

QRoomTransactionPtr QConversationModel::pinTransaction()
{
    return m_pinTransaction;
}

int QConversationModel::lastIndex()
{
    Conversation last;
    int index_ = 0;
    for(int i = m_data.size() - 1; i >= 0; i--){
        last = m_data.at(i);
        if(last.messageType != (int)ENUNCHUCK::ROOM_EVT::INVALID)
        {
            if(last.messageType != (int)ENUNCHUCK::ROOM_EVT::EXCEPTION){
                index_ = i;
                break;
            }
        }
    }
    return index_;
}

int QConversationModel::count() const
{
    return m_data.count();
}

void QConversationModel::clear()
{
    beginResetModel();
    m_data.clear();
    endResetModel();
}

int QConversationModel::unreadLastIndex() const
{
    if(m_room){
        int maxUnread = m_room->unreadCount();
        for(int i = m_data.count() - 1; i > 0 ; i--){
            Conversation cons = m_data.at(i);
            if(maxUnread > 0 && cons.messageType == (int)ENUNCHUCK::ROOM_EVT::PLAIN_TEXT){
                maxUnread --;
            }
            if(maxUnread <= 0){
                return i;
            }
        }
    }
    return 0;
}

bool QConversationModel::initConsShow() const
{
    return m_initConsShow;
}

void QConversationModel::setInitConsShow(bool initConsShow)
{
    if (m_initConsShow == initConsShow)
        return;
    m_initConsShow = initConsShow;
    emit initConsShowChanged();
    if(m_initConsShow == false && m_data.size() > 0){
        beginRemoveRows(QModelIndex(),0,0);
        m_data.removeAt(0);
        endRemoveRows();
    }
}

void QConversationModel::refresh()
{
    beginResetModel();
    endResetModel();
    emit countChanged();
}

bool QConversationModel::containsTxReadyMessage(const Conversation data)
{
    if((int)ENUNCHUCK::ROOM_EVT::TX_READY == data.messageType){
        foreach (Conversation it , m_data ){
            if((it.messageType == data.messageType) && (qUtils::strCompare(it.init_event_id, data.init_event_id))) {
                return true;
            }
        }
    }
    return false;
}

bool QConversationModel::containsWalletReadyMessage(const Conversation data)
{
    if((int)ENUNCHUCK::ROOM_EVT::WALLET_READY == data.messageType){
        for (int i = 0; i < m_data.count(); i++) {
            if((data.messageType == m_data.at(i).messageType) && (qUtils::strCompare(data.init_event_id, m_data.at(i).init_event_id))){
                m_data[i].timestamp = max(data.timestamp, m_data.at(i).timestamp);
                return true;
            }
        }
    }
    return false;
}

bool QConversationModel::needIgnoreInSupportRoom(const Conversation data)
{
    if(isSupportRoom()){
        if (data.messageType == (int)ENUNCHUCK::ROOM_EVT::PLAIN_TEXT
                || data.messageType == (int)ENUNCHUCK::ROOM_EVT::FILE_IMAGE
                || data.messageType == (int)ENUNCHUCK::ROOM_EVT::FILE_VIDEO
                || data.messageType == (int)ENUNCHUCK::ROOM_EVT::FILE_OTHER ) {
            return false;
        }
        else{
            return true;
        }
    }
    else{
        return false;
    }
}

bool QConversationModel::isSupportRoom()
{
    QString tagname = (int)ENUNCHUCK::Chain::MAIN == (int)AppSetting::instance()->primaryServer() ?  NUNCHUK_ROOM_SUPPORT : NUNCHUK_ROOM_SUPPORTTESTNET;
    return m_room ? (m_room->tagNames().contains(tagname)) : false;
}

qint64 QConversationModel::maxLifeTime() const
{
    return m_maxLifeTime;
}

void QConversationModel::setMaxLifeTime(qint64 value)
{
    m_maxLifeTime = value;
}

void QConversationModel::startCountdown()
{
    m_RetentionTimer.stop();
    m_RetentionTimer.start(2*60*1000); // Every 2 minutes
    processingRetentionMessage();
}

void QConversationModel::stopCountdown()
{
    m_RetentionTimer.stop();
}

void QConversationModel::processingRetentionMessage()
{
    if(m_room && m_RetentionTimer.isActive()){
        // Check all coversation, if conversation age > timelife max then remove it
        QVector<int> indicesToRemove;
        for (int i = 0; i < m_data.count(); i++) {
            Conversation cons = m_data.at(i);
            qint64 timestamp_milisec = QDateTime::fromTime_t(cons.timestamp).toMSecsSinceEpoch();
            qint64 time_msg_age = QDateTime::currentDateTimeUtc().toMSecsSinceEpoch() - timestamp_milisec;
            if(time_msg_age > m_maxLifeTime){
                indicesToRemove.append(i);
            }
        }
        if (!indicesToRemove.isEmpty()) {
            // Remove the items in reverse order to maintain correct indices
            beginRemoveRows(QModelIndex(), indicesToRemove.first(), indicesToRemove.last());
            for (int i = indicesToRemove.count() - 1; i >= 0; --i) {
                m_data.removeAt(indicesToRemove.at(i));
            }
            endRemoveRows();
            emit countChanged();
        }
    }
}

bool sortConversationByTimeAscending(const Conversation &v1, const Conversation &v2)
{
    return v1.timestamp < v2.timestamp;
}

bool sortConversationByTimeDescending(const Conversation &v1, const Conversation &v2)
{
    return v1.timestamp > v2.timestamp;
}
