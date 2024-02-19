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

#ifndef MATRIXBRIGDE_H
#define MATRIXBRIGDE_H
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
#include "matrixifaces.h"
#include "bridgeifaces.h"
#include "Chats/QNunchukMatrixEvent.h"
#include "Chats/QRoomTransaction.h"
#include "Chats/QRoomWallet.h"
#include "Chats/QNunchukRoomModel.h"
#include "SingleSignerModel.h"
#include "TransactionModel.h"
#include "QWarningMessage.h"

namespace matrixbrigde {

QJsonObject stringToJson(const QString& content);

void makeMatrixInstance(const QString &account,
                        const QString &device_id,
                        QWarningMessage &msg);

QNunchukMatrixEvent InitWallet(const QString& room_id,
                               const QString& name,
                               int m,
                               int n,
                               ENUNCHUCK::AddressType address_type,
                               bool is_escrow,
                               const QString& description,
                               QWarningMessage& msg);

QNunchukMatrixEvent JoinWallet(const QString& room_id,
                               const QSingleSignerPtr &signer,
                               QWarningMessage& msg);

QNunchukMatrixEvent LeaveWallet(const QString& room_id,
                                const QString& join_id,
                                const QString& reason,
                                QWarningMessage& msg);

QNunchukMatrixEvent CancelWallet(const QString& room_id,
                                 const QString& reason,
                                 QWarningMessage& msg);

QNunchukMatrixEvent CreateWallet(const QString& room_id,
                                 QWarningMessage& msg);

QNunchukMatrixEvent DeleteWallet(const QString& room_id,
                                 QWarningMessage& msg);

QNunchukMatrixEvent InitTransaction(const QString& room_id,
                                    const QMap<QString, qint64> outputs,
                                    const QString &memo,
                                    const QUTXOListModelPtr inputs,
                                    const int fee_rate,
                                    bool subtract_fee_from_amount,
                                    QWarningMessage& msg);

QNunchukMatrixEvent SignTransaction(const QString &room_id,
                                    const QString& init_event_id,
                                    const QDevicePtr &device,
                                    QWarningMessage& msg);

QNunchukMatrixEvent SignAirgapTransaction(const QString &init_event_id,
                                          const QString &master_fingerprint,
                                          QWarningMessage &msg);

QNunchukMatrixEvent RejectTransaction(const QString &room_id,
                                      const QString& init_event_id,
                                      const QString& reason,
                                      QWarningMessage& msg);

QNunchukMatrixEvent CancelTransaction(const QString &room_id,
                                      const QString& init_event_id,
                                      const QString& reason,
                                      QWarningMessage& msg);

QNunchukMatrixEvent BroadcastTransaction(const QString &room_id,
                                         const QString& init_event_id,
                                         QWarningMessage& msg);

QRoomTransactionPtr GetRoomTransaction(const QString &room_id,
                                       const QString& init_event_id,
                                       QWarningMessage& msg);

nunchuk::RoomTransaction GetOriginRoomTransaction(const QString &room_id,
                                                  const QString& init_event_id,
                                                  QWarningMessage& msg);

nunchuk::RoomTransaction GetOriginPendingTransaction(const QString &room_id,
                                                     const QString& tx_id,
                                                     QWarningMessage& msg);

QString GetTransactionId(const QString &room_id,
                         const QString& event_id,
                         QWarningMessage& msg);

QRoomWalletPtr ReloadRoomWallet(QNunchukRoom * const room);

QList<QRoomWalletPtr> GetAllRoomWallets(QWarningMessage& msg);

QRoomWalletPtr GetRoomWallet(const QString& room_id,
                             QWarningMessage& msg);

QRoomTransactionModelPtr GetPendingTransactions(const QString& room_id);

QNunchukMatrixEvent GetEvent(const QString& room_id,
                             const QString& event_id,
                             QWarningMessage& msg);

void ConsumeEvent(const QString& room_id,const QNunchukMatrixEvent& event);

void ConsumeSyncEvent(const QString& room_id,const QNunchukMatrixEvent& event);

void RegisterAutoBackup(const QString &sync_room_id,
                        const QString &access_token);

void EnableGenerateReceiveEvent();

bool HasRoomWallet(const QString& room_id);

void UploadFileCallback(const QString& json_info,
                        const QString& file_url);

void DownloadFileCallback(const QString& json_info,
                          const std::vector<unsigned char> &file_data);

void EnableAutoBackup(bool state);

void Backup();

QNunchukMatrixEvent ImportWallet(const QString& room_id,
                                 const QString& name,
                                 const QString& description,
                                 const QString& filepath,
                                 QWarningMessage& msg);

QNunchukMatrixEvent ImportWalletQR(const QString &room_id,
                                   const QString &name,
                                   const QString &description,
                                   nunchuk::Chain chain,
                                   const QStringList qrtags,
                                   QWarningMessage& msg);
};

#endif // MATRIXBRIGDE_H
