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

#ifndef QGROUPWALLETROOMMODEL_H
#define QGROUPWALLETROOMMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QtCore/qreadwritelock.h>
#include "nunchuk.h"
#include "QOutlog.h"

Q_DECLARE_METATYPE(std::vector<nunchuk::GroupMessage>)
class QGroupMessageModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int      currentIndex    READ currentIndex   WRITE setCurrentIndex   NOTIFY currentIndexChanged)
    Q_PROPERTY(QString  lasttimestamp   READ lasttimestamp                          NOTIFY countChanged)
    Q_PROPERTY(QString  lastMessage     READ lastMessage                            NOTIFY countChanged)

public:
    QGroupMessageModel();
    ~QGroupMessageModel();
    enum GroupMessageRoles {
        group_message_id,
        group_message_wallet_id,
        group_message_sender,
        group_message_content,
        group_message_signer,
        group_message_time,
        group_message_receive
    };
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QHash<int, QByteArray> roleNames() const;
    void createGroupMessage(const std::vector<nunchuk::GroupMessage> data);
    void appendGroupMessage(const nunchuk::GroupMessage data);
    void removeGroupMessage(const nunchuk::GroupMessage data);
    Q_INVOKABLE void markAllMessagesAsRead();
    Q_INVOKABLE void markFiveMessagesAsRead();

    // Conversations
    void startDownloadConversation(const QString& wallet_id);
    int  currentIndex() const;
    void setCurrentIndex(int index);
    QString lasttimestamp();
    QString lastMessage();
    void requestSort();
    void clear();
private:
    QString timeToString(const time_t info) const;
    bool    sendByMe(const QString &sender) const;
    std::optional<std::string> getLatestMessageId(const std::vector<nunchuk::GroupMessage>& messages);
    QString convertLinksToHtml(const QString &text) const;
signals:
    void groupMessageAdded(int index);
    void groupMessageDownloaded();
    void countChanged();
    void currentIndexChanged();

private:
    int m_currentIndex {0};
    std::vector<nunchuk::GroupMessage> m_messages;
    mutable QMutex m_lock; // Can be used for multithread read
    QString m_wallet_id {};
};
typedef OurSharedPointer<QGroupMessageModel> QGroupMessageModelPtr;

bool sortGroupMessageTime(const nunchuk::GroupMessage &v1, const nunchuk::GroupMessage &v2);

#endif // QGROUPWALLETROOMMODEL_H
