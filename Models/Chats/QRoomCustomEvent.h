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

#ifndef QROOMCUSTOMEVENT_H
#define QROOMCUSTOMEVENT_H

#include <QObject>
#include <room.h>
#include <connection.h>
#include <ssosession.h>
#include <user.h>
#include <uriresolver.h>
#include <events/event.h>
#include <events/roomevent.h>
#include <events/encryptionevent.h>
#include <events/redactionevent.h>
#include <events/roomavatarevent.h>
#include <events/roomcreateevent.h>
#include <events/roommemberevent.h>
#include <events/roommessageevent.h>
#include <events/roomcanonicalaliasevent.h>
#include <events/simplestateevents.h>

using namespace Quotient;

#define NUNCHUK_ROOM_MESSAGE        "m.room.message"
#define NUNCHUK_ROOM_MEMBER         "m.room.member"
#define NUNCHUK_ROOM_NAME           "m.room.name"
#define NUNCHUK_ROOM_SYNC           "io.nunchuk.sync"
#define NUNCHUK_ROOM_SUPPORT        "io.nunchuk.support"
#define NUNCHUK_ROOM_SUPPORTTESTNET "io.nunchuk.support.testnet"
#define NUNCHUK_ROOM_ENCRYPTED      "m.room.encrypted"
#define NUNCHUK_ROOM_ENCRYPTION     "m.room.encryption"
#define NUNCHUK_ROOM_BYZANTINE      "io.nunchuk.premium"
#define NUNCHUK_ROOM_RETENTION      "m.room.retention"
const qint64 NUNCHUK_ROOM_RETENTION_TIME (2592000000);  // 30 days = 30*24*60*60*1000 ms

#define NUNCHUK_EVENT_SYNC          "io.nunchuk.sync"
#define NUNCHUK_EVENT_WALLET        "io.nunchuk.wallet"
#define NUNCHUK_EVENT_TRANSACTION   "io.nunchuk.transaction"
#define NUNCHUK_EVENT_EXCEPTION     "io.nunchuk.error"

// Wallet message type: Wallet
#define NUNCHUK_MSG_WALLET_INIT     "io.nunchuk.wallet.init"
#define NUNCHUK_MSG_WALLET_JOIN     "io.nunchuk.wallet.join"
#define NUNCHUK_MSG_WALLET_LEAVE    "io.nunchuk.wallet.leave"
#define NUNCHUK_MSG_WALLET_CREATE   "io.nunchuk.wallet.create"
#define NUNCHUK_MSG_WALLET_READY    "io.nunchuk.wallet.ready"
#define NUNCHUK_MSG_WALLET_CANCEL   "io.nunchuk.wallet.cancel"

// Wallet message type: Transaction
#define NUNCHUK_MSG_TX_INIT         "io.nunchuk.transaction.init"
#define NUNCHUK_MSG_TX_SIGN         "io.nunchuk.transaction.sign"
#define NUNCHUK_MSG_TX_BROADCAST    "io.nunchuk.transaction.broadcast"
#define NUNCHUK_MSG_TX_READY        "io.nunchuk.transaction.ready"
#define NUNCHUK_MSG_TX_CANCEL       "io.nunchuk.transaction.cancel"
#define NUNCHUK_MSG_TX_RECEIVE      "io.nunchuk.transaction.receive"

class QNunchukWalletEvent : public RoomEvent
{
public:
    DEFINE_EVENT_TYPEID("io.nunchuk.wallet", QNunchukWalletEvent)
    explicit QNunchukWalletEvent(const QString& matrixType, const QJsonObject& contentJson);
};

class QNunchukTransactionEvent : public RoomEvent
{
public:
    DEFINE_EVENT_TYPEID("io.nunchuk.transaction", QNunchukTransactionEvent)
    explicit QNunchukTransactionEvent(const QString& matrixType, const QJsonObject& contentJson);
};

class QNunchukSyncEvent : public RoomEvent
{
public:
    DEFINE_EVENT_TYPEID("io.nunchuk.sync", QNunchukSyncEvent)
    explicit QNunchukSyncEvent(const QString& matrixType, const QJsonObject& contentJson);
};

class QNunchukExceptionEvent : public RoomEvent
{
public:
    DEFINE_EVENT_TYPEID("io.nunchuk.error", QNunchukExceptionEvent)
    explicit QNunchukExceptionEvent(const QString& matrixType, const QJsonObject& contentJson);
};

// class QNunchukRentionState : public StateEventBase
// {
// public:
//     DEFINE_EVENT_TYPEID("m.room.retention", QNunchukRentionState)
//     explicit QNunchukRentionState(const QJsonObject& contentJson);
// };

// DEFINE_SIMPLE_STATE_EVENT(QNunchukRentionState, "m.room.retention", QJsonObject, contentJson)

#endif // QROOMCUSTOMEVENT_H
