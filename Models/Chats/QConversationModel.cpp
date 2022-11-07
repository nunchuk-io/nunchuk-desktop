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
#include "Draco.h"
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
}

QConversationModel::~QConversationModel()
{

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
        if(cons.messageType == MSG_EXCEPTION && !AppSetting::instance()->enableDebug()) return "";
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
        case MSG_WALLET_JOIN:
        case MSG_WALLET_LEAVE:
        case MSG_WALLET_CANCEL:
        case MSG_TX_SIGN:
        case MSG_TX_BROADCAST:
        case MSG_TX_CANCEL:
        case MSG_STATE_EVENT:{
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
    return names;
}

void QConversationModel::addMessage(const Conversation data)
{
    QDateTime today = QDateTime::currentDateTime();
    QDateTime day = QDateTime::fromTime_t(data.timestamp);
    qint64 numberDay = day.daysTo(today);
    if(numberDay == 0){
        m_firstToday.push_back(data.timestamp);
    }
    beginInsertRows(QModelIndex(), m_data.size(), m_data.size());
    if(data.messageType == MSG_TX_READY){
        if(!containsTxReadyMessage(data)){
            m_data.append(data);
        }
    }
    else if(data.messageType == MSG_WALLET_READY){
        if(!containsWalletReadyMessage(data)){
            m_data.append(data);
        }
    }
    else{
        m_data.append(data);
        if(MSG_WALLET_CREATE == data.messageType){
            Conversation backup;
            backup.messageType = MSG_WALLET_BACKUP;
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

void QConversationModel::addMessage_(const Conversation data)
{
    QDateTime today = QDateTime::currentDateTime();
    QDateTime day = QDateTime::fromTime_t(data.timestamp);
    qint64 numberDay = day.daysTo(today);
    if(numberDay == 0){
        m_firstToday.push_back(data.timestamp);
    }
    if(data.messageType == MSG_TX_READY){
        if(!containsTxReadyMessage(data)){
            m_data.append(data);
        }
    }
    else if(data.messageType == MSG_WALLET_READY){
        if(!containsWalletReadyMessage(data)){
            m_data.append(data);
        }
    }
    else{
        m_data.append(data);
        if(MSG_WALLET_CREATE == data.messageType){
            Conversation backup;
            backup.messageType = MSG_WALLET_BACKUP;
            backup.message = STR_CPP_001;
            backup.timestamp = data.timestamp+1;
            backup.init_event_id = data.init_event_id;
            m_data.append(backup);
        }
    }
}

void QConversationModel::insertMessage(int index, const Conversation data)
{
    if(data.messageType == MSG_TX_READY){
        if(!containsTxReadyMessage(data)){
            m_data.insert(index, data);
        }
    }
    else if(data.messageType == MSG_WALLET_READY){
        if(!containsWalletReadyMessage(data)){
            m_data.insert(index, data);
        }
    }
    else{
        m_data.insert(index, data);
        if(MSG_WALLET_CREATE == data.messageType){
            Conversation backup;
            backup.messageType = MSG_WALLET_BACKUP;
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
        if(0 == QString::compare(init_event_id, it.init_event_id, Qt::CaseInsensitive)){
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
            if(0 != QString::compare(NUNCHUK_EVENT_TRANSACTION, m_data.at(i).matrixType, Qt::CaseInsensitive)){
                continue;
            }
            if((0 == QString::compare(data.evtId, m_data.at(i).evtId, Qt::CaseInsensitive))
                    || (0 == QString::compare(tx.data()->get_init_event_id(), m_data.at(i).init_event_id, Qt::CaseInsensitive)))
            {
                if(m_data.at(i).messageType == MSG_TX_INIT){
                    createByMe = m_data.at(i).sendByMe;
                }
                if(tx.data()->transaction()){
                    tx.data()->transaction()->setCreateByMe(createByMe);
                }
                if(tx.data()->get_cancel_event_id() != "" && m_data.at(i).messageType == MSG_TX_INIT){
                    m_data[i].messageType = MSG_TX_CANCELED;
                }
                if((m_data.at(i).messageType == MSG_TX_SIGN && data.messageType == MSG_TX_SIGN)
                        || (m_data.at(i).messageType == MSG_TX_BROADCAST && data.messageType == MSG_TX_BROADCAST)
                        || (m_data.at(i).messageType == MSG_TX_RECEIVE && data.messageType == MSG_TX_RECEIVE))
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
        if(MSG_WALLET_INIT != m_data.at(i).messageType){
            continue;
        }
        else{
            if(0 == QString::compare(init_event_id, m_data.at(i).init_event_id, Qt::CaseInsensitive)){
                m_data[i].messageType = MSG_WALLET_PAST;
                emit dataChanged(index(i),index(i));
            }
        }
    }
}

void QConversationModel::updateCancelTransaction(const Conversation data)
{
    for (int i = 0; i < m_data.count(); i++) {
        if(0 == QString::compare(NUNCHUK_EVENT_TRANSACTION, m_data.at(i).matrixType, Qt::CaseInsensitive)){
            if((0 == QString::compare(data.evtId, m_data.at(i).evtId, Qt::CaseInsensitive))
                    || (0 == QString::compare(data.init_event_id, m_data.at(i).init_event_id, Qt::CaseInsensitive))){
                if(m_data[i].messageType == MSG_TX_INIT){
                    m_data[i].messageType = MSG_TX_CANCELED;
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
            if(0 == QString::compare(tx_id, m_data.at(i).transaction.data()->get_tx_id(), Qt::CaseInsensitive) && m_data.at(i).transaction.data()->transaction()){
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
                    && (0 == QString::compare(it.transaction.data()->get_tx_id(), tx_id, Qt::CaseInsensitive))
                    && (0 == QString::compare(it.transaction.data()->get_init_event_id(), init_event_id, Qt::CaseInsensitive)))
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
            if(0 == QString::compare(tx_id, m_data.at(i).transaction.data()->get_tx_id(), Qt::CaseInsensitive)){
                m_data[i].transaction.data()->transaction()->setMemo(memo);
                emit dataChanged(index(i),index(i));
            }
        }
    }
}

void QConversationModel::updateInitEventId(const Conversation data)
{
    for (int i = 0; i < m_data.count(); i++) {
        if(0 == QString::compare(data.evtId, m_data.at(i).evtId, Qt::CaseInsensitive)){
            m_data[i].init_event_id = data.init_event_id;
            m_data[i].message = data.message;
            emit dataChanged(index(i),index(i));
        }
    }
}

void QConversationModel::messageSent(QString txnId, QString eventId)
{
    for (int i = 0; i < m_data.count(); i++) {
        if(m_data.at(i).sendByMe && 0 == QString::compare(txnId, m_data.at(i).txnId, Qt::CaseInsensitive)){
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
        if(lastMessage.messageType != MSG_WALLET_PAST
                && lastMessage.messageType != MSG_WALLET_INIT
                && lastMessage.messageType != MSG_TX_CANCELED
                && lastMessage.messageType != MSG_TX_INIT
                && lastMessage.messageType != INVALID_MESSAGE
                && lastMessage.messageType != MSG_EXCEPTION)
        {
            break;
        }
    }
    if(lastMessage.messageType == MSG_EXCEPTION){
        return Conversation();
    }
    return lastMessage;
}

Conversation QConversationModel::lastTime()
{
    Conversation lastTime;
    for(int i = m_data.size() - 1; i >= 0; i--){
        lastTime = m_data.at(i);
        if(lastTime.messageType != INVALID_MESSAGE &&
                lastTime.messageType != MSG_EXCEPTION)
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
        if(last.messageType != INVALID_MESSAGE)
        {
            if(last.messageType != MSG_EXCEPTION){
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
            if(maxUnread > 0 && cons.messageType == MSG_PURE_STRING){
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
    if(MSG_TX_READY == data.messageType){
        foreach (Conversation it , m_data ){
            if((it.messageType == data.messageType) && (0 == QString::compare(it.init_event_id, data.init_event_id, Qt::CaseInsensitive))) {
                return true;
            }
        }
    }
    return false;
}

bool QConversationModel::containsWalletReadyMessage(const Conversation data)
{
    if(MSG_WALLET_READY == data.messageType){
        for (int i = 0; i < m_data.count(); i++) {
            if((data.messageType == m_data.at(i).messageType) && (0 == QString::compare(data.init_event_id, m_data.at(i).init_event_id, Qt::CaseInsensitive))){
                m_data[i].timestamp = max(data.timestamp, m_data.at(i).timestamp);
                return true;
            }
        }
    }
    return false;
}

bool sortConversationByTimeAscending(const Conversation &v1, const Conversation &v2)
{
    return v1.timestamp < v2.timestamp;
}

bool sortConversationByTimeDescending(const Conversation &v1, const Conversation &v2)
{
    return v1.timestamp > v2.timestamp;
}
