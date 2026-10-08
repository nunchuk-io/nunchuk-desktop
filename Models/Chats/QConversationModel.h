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

#ifndef QCONVERSATIONMODEL_H
#define QCONVERSATIONMODEL_H
#include <QAbstractListModel>
#include "QRoomWallet.h"
#include "QRoomTransaction.h"
#include <room.h>
#include <connection.h>
#include <ssosession.h>
#include <user.h>
#include <uriresolver.h>
#include <events/encryptionevent.h>
#include <events/redactionevent.h>
#include <events/roomavatarevent.h>
#include <events/roomcreateevent.h>
#include <events/roommemberevent.h>
#include <events/roommessageevent.h>
#include <events/roomcanonicalaliasevent.h>
#include <events/simplestateevents.h>
#include "bridgeifaces.h"
using namespace Quotient;

struct Conversation {
    bool isStateEvent = false;
    int  messageType = (int)ENUNCHUCK::ROOM_EVT::INVALID;
    bool sendByMe = false;
    QString sender = "";
    QString receiver = "";
    time_t  timestamp = 0;
    QString message = "";
    QString avatar = "";
    QString matrixType = "";
    QString init_event_id = "";
    QJsonObject init_event_json = QJsonObject();
    QRoomTransactionPtr transaction = NULL;
    QString senderId = "";
    // RoomEventId
    QString evtId = "";
    QString txnId = "";

    // Attachment
    QString file_path = "";

    // Filter
    bool    visible = true;
};

class QConversationModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int currentIndex    READ currentIndex   WRITE setCurrentIndex   NOTIFY currentIndexChanged)
    Q_PROPERTY(int count           READ count                                  NOTIFY countChanged)
    Q_PROPERTY(bool initConsShow   READ initConsShow   WRITE setInitConsShow   NOTIFY initConsShowChanged)
public:
    QConversationModel(Room* r);
    ~QConversationModel();
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QHash<int, QByteArray> roleNames() const;
    enum ConversationRoles {
        role_sendByMe,
        role_sender,
        role_receiver,
        role_timestamp,
        role_timesection,
        role_message,
        role_avartar,
        role_istateEvent,
        role_messageType,
        role_init_event_id,
        role_transaction,
        role_evtId,
        role_txnId,
        role_file_path,
        role_progressInfo
    };
    void addMessage(const Conversation data);
    void addHistoryMessage(const Conversation data);
    void insertMessage(int index, const Conversation data);
    void requestSortByTimeAscending(bool ui_update = true);
    bool isWalletCreator(const QString& init_event_id);
    int currentIndex() const;
    void setCurrentIndex(int currentIndex);
    void updateTransaction(const Conversation data, const QRoomTransactionPtr &tx);
    void updateCancelWallet(const QString &init_event_id);
    void updateCancelTransaction(const Conversation data);
    void transactionChanged(const QString &tx_id, const int status, const int height);
    bool isDuplicateTransactionEvent(const QString &tx_id, const int messageType, const QString &init_event_id);
    void updateTransactionMemo(const QString &tx_id, const QString &memo);
    void updateInitEventId(const Conversation data);
    void messageSent(QString txnId, QString eventId);
    Conversation getConversation(const QString &eventId);
    Conversation lastMessage();
    Conversation lastTime();
    QRoomTransactionPtr pinTransaction();
    int lastIndex();
    int count() const;
    void clear();
    int unreadLastIndex() const;

    bool initConsShow() const;
    void setInitConsShow(bool initConsShow);
    void refresh();

    void startCountdown();
    void stopCountdown();
    qint64 maxLifeTime() const;
    void setMaxLifeTime(qint64 value);

private:
    Quotient::Room      *m_room;
    QList<Conversation> m_data;
    int m_currentIndex;
    bool containsTxReadyMessage(const Conversation data);
    bool containsWalletReadyMessage(const Conversation data);
    bool needIgnoreInSupportRoom(const Conversation data);
    bool isSupportRoom();
    bool m_initConsShow;
    QRoomTransactionPtr m_pinTransaction;
    QList<time_t>       m_firstToday;
    QTimer              m_RetentionTimer;
    qint64              m_maxLifeTime;

signals:
    void currentIndexChanged();
    void countChanged();
    void initConsShowChanged();

public slots:
    void processingRetentionMessage();
};
typedef OurSharedPointer<QConversationModel> QConversationModelPtr;

bool sortConversationByTimeAscending(const Conversation &v1, const Conversation &v2);
bool sortConversationByTimeDescending(const Conversation &v1, const Conversation &v2);

#endif // QCONVERSATIONMODEL_H
