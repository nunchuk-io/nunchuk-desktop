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

#include "QGroupWalletRoomModel.h"
#include "bridgeifaces.h"
#include "ifaces/Servers/Draco.h"
#include "QThreadForwarder.h"
#include <unordered_set>
#include "Premiums/QSharedWallets.h"

QGroupMessageModel::QGroupMessageModel()
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

QGroupMessageModel::~QGroupMessageModel()
{

}

int QGroupMessageModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return (int)m_messages.size();
}

QVariant QGroupMessageModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_messages.size()){
        return QVariant();
    }
    switch (role) {
    case group_message_id:
        return QString::fromStdString(m_messages[index.row()].get_id());
    case group_message_wallet_id:
        return QString::fromStdString(m_messages[index.row()].get_wallet_id());
    case group_message_sender:
        return QString::fromStdString(m_messages[index.row()].get_sender());
    case group_message_content:
        return convertLinksToHtml(QString::fromStdString(m_messages[index.row()].get_content()));
    case group_message_signer:
        return QString::fromStdString(m_messages[index.row()].get_signer());
    case group_message_time:
    {
        time_t timeInfo = m_messages[index.row()].get_ts();
        return timeToString(timeInfo);
    }
    case group_message_receive:
    {
        return sendByMe(QString::fromStdString(m_messages[index.row()].get_sender()));
    }
    default:
        return QVariant("");
    }
}

bool QGroupMessageModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || index.row() >= m_messages.size()){
        return false;
    }
    switch (role) {
    case group_message_id:
        return false;
    case group_message_wallet_id:
        m_messages[index.row()].set_wallet_id(value.toString().toStdString());
        return true;
    case group_message_sender:
        m_messages[index.row()].set_sender(value.toString().toStdString());
        return true;
    case group_message_content:
        m_messages[index.row()].set_content(value.toString().toStdString());
        return true;
    case group_message_signer:
        m_messages[index.row()].set_signer(value.toString().toStdString());
        return true;
    case group_message_time:
        return false;
    default:
        return false;
    }
    emit dataChanged(index, index, { role } );
}

QHash<int, QByteArray> QGroupMessageModel::roleNames() const
{
    QHash<int, QByteArray> names;
    names[group_message_id]         = "group_message_id";
    names[group_message_wallet_id]  = "group_message_wallet_id";
    names[group_message_sender]     = "group_message_sender";
    names[group_message_content]    = "group_message_content";
    names[group_message_signer]     = "group_message_signer";
    names[group_message_time]       = "group_message_time";
    names[group_message_receive]    = "group_message_receive";
    return names;
}

void QGroupMessageModel::appendGroupMessage(const nunchuk::GroupMessage data)
{
    int index = m_messages.size();
    beginInsertRows(QModelIndex(), index, index);
    m_messages.push_back(data);
    requestSort();
    endInsertRows();
    emit countChanged();
    emit groupMessageAdded((m_messages.size() - 1));
}

void QGroupMessageModel::appendGroupMessage(const std::vector<nunchuk::GroupMessage> data)
{
    if (data.empty()) {
        return;
    }
    std::vector<nunchuk::GroupMessage> newmessage = data;
    std::reverse(newmessage.begin(), newmessage.end());
    beginResetModel();
    m_messages.clear();
    m_messages.insert(m_messages.begin(), newmessage.begin(), newmessage.end());
    requestSort();
    endResetModel();
    setCurrentIndex((m_messages.size() - 1));
    emit countChanged();
    emit groupMessageDownloaded();
}

void QGroupMessageModel::removeGroupMessage(const nunchuk::GroupMessage data)
{
    std::string target = data.get_id();
    auto it = std::find_if(m_messages.begin(), m_messages.end(),
                           [&target](const nunchuk::GroupMessage &msg) {
                               return msg.get_id() == target;
                           });

    if (it != m_messages.end()) {
        int index = std::distance(m_messages.begin(), it);
        beginRemoveRows(QModelIndex(), index, index);
        m_messages.erase(it);
        endRemoveRows();
    }
    emit countChanged();
}

void QGroupMessageModel::markAllMessagesAsRead()
{
    if (auto latestId = getLatestMessageId(m_messages)) {
        DBG_INFO << " Last ID " << *latestId;
        bridge::SetLastReadMessage(m_wallet_id, QString::fromStdString(*latestId));
    }
}

void QGroupMessageModel::markFiveMessagesAsRead()
{
    int max = 10;
    int index = 0;
    int number = bridge::GetUnreadMessagesCount(m_wallet_id);
    for (int i = m_messages.size() - number; i < m_messages.size(); i++) {
        if (index < max) {
            index ++;
            auto messageId = QString::fromStdString(m_messages.at(i).get_id());
            bridge::SetLastReadMessage(m_wallet_id, messageId);
        }
    }
}

QString QGroupMessageModel::timeToString(const time_t info) const
{
    QDateTime dateTime = QDateTime::fromSecsSinceEpoch(info);
    QDateTime currentDateTime = QDateTime::currentDateTime();
    int daysDifference = dateTime.daysTo(currentDateTime);
    if (daysDifference == 0) {
        return QString("Today, %1").arg(dateTime.toString("hh:mm AP"));
    }
    else if (daysDifference == 1) {
        return "Yesterday,";
    }
    else if (daysDifference <= 3) {
        return QString("%1 day%2 ago")
            .arg(daysDifference)
            .arg(daysDifference > 1 ? "s" : "");
    }
    else {
        return dateTime.toString("ddd, MMMM dd, yyyy");
    }
}

bool QGroupMessageModel::sendByMe(const QString &sender) const
{
    return !qUtils::strCompare(sender, QSharedWallets::instance()->uid());
}

std::optional<string> QGroupMessageModel::getLatestMessageId(const std::vector<nunchuk::GroupMessage> &messages) {
    if (messages.empty()){
        return std::nullopt;
    }
    auto latestMessage = std::max_element(messages.begin(), messages.end(),
                                          [](const nunchuk::GroupMessage& a, const nunchuk::GroupMessage& b) {
                                              return a.get_ts() < b.get_ts();
                                          });
    return latestMessage->get_id();
}

QString QGroupMessageModel::convertLinksToHtml(const QString &text) const{
    QRegularExpression urlRegex(R"((https?:\/\/[^\s]+))");
    QString htmlText = text;
    htmlText.replace(urlRegex, "<a href='\\1'>\\1</a>");
    return htmlText;
}

void QGroupMessageModel::startDownloadConversation(const QString& wallet_id)
{
    m_wallet_id = wallet_id;
    QtConcurrent::run([wallet_id, this]() {
        QMutexLocker locker(&m_lock);
        QWarningMessage msg;
        int page_num = 0;
        int page_size = 100;
        bool more_data = false;
        std::vector<nunchuk::GroupMessage> list;
        do {
            std::vector<nunchuk::GroupMessage> ret = bridge::GetGroupMessages(wallet_id.toStdString(), page_num, page_size, true, msg);
            if (ret.size() < page_size) {
                more_data = false;
            } else {
                page_num++;
            }
            list.insert(list.begin(), ret.begin(), ret.end());
        }
        while (more_data);
        DBG_INFO << wallet_id << list.size();
        QThreadForwarder::instance()->forwardInQueuedConnection([this, list](){
            appendGroupMessage(list);
        });
    });
}

int QGroupMessageModel::currentIndex() const
{
    return m_currentIndex;
}

void QGroupMessageModel::setCurrentIndex(int index)
{
    if (m_currentIndex == index)
        return;
    m_currentIndex = index;
    emit currentIndexChanged();
}

QString QGroupMessageModel::lasttimestamp()
{
    if(m_messages.size() > 0){
        int last_index = min((int)m_messages.size(), max(0, (int)m_messages.size() - 1));
        time_t timeInfo = m_messages[last_index].get_ts();
        return timeToString(timeInfo);
    }
    else {
        return "";
    }
}

QString QGroupMessageModel::lastMessage()
{
    if(m_messages.size() > 0){
        int last_index = min((int)m_messages.size(), max(0, (int)m_messages.size() - 1));
        QString sender = QString::fromStdString(m_messages[last_index].get_sender());
        QString message = QString::fromStdString(m_messages[last_index].get_content());
        return QString("%1: %2").arg(sender).arg(message);
    }
    else {
        return "";
    }
}

void QGroupMessageModel::requestSort()
{
    qSort(m_messages.begin(), m_messages.end(), sortGroupMessageTime);
}

void QGroupMessageModel::clear()
{
    beginResetModel();
    m_messages.clear();
    endResetModel();
}

bool sortGroupMessageTime(const nunchuk::GroupMessage &v1, const nunchuk::GroupMessage &v2)
{
    return (v1.get_ts() < v2.get_ts());
}
