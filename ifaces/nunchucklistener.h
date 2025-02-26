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

#ifndef NUNCHUCKLISTENER_H
#define NUNCHUCKLISTENER_H
#include <QCoreApplication>
#include <QObject>
#include <QMetaObject>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <nunchuk.h>

void balance_listener(std::string id, nunchuk::Amount balance);
void balances_listener(std::string id, nunchuk::Amount balance, nunchuk::Amount unconfirmed_balance);
void devices_listener(std::string fingerprint, bool connected);
void transaction_listener(std::string tx_id, nunchuk::TransactionStatus status, std::string wallet_id);
void block_listener(int height, std::string hex_header);
bool cache_master_signer_XPub(int progress);
bool create_master_signer_listener(int progress);
void blockchain_connection_listener(nunchuk::ConnectionStatus status, int percent);
bool create_software_signer_listener(int progress);
bool create_software_signer_xprv_listener(int progress);

bool CacheTapsignerMasterSignerXPubProgress(int percent);
bool CacheDefaultTapsignerMasterSignerXPubProgress(int percent);
bool ImportTapsignerMasterSignerProgress(int percent);

// For Groupwallet
void GroupUpdateListener(const nunchuk::GroupSandbox& state);
void GroupMessageListener(const nunchuk::GroupMessage& msg);
void GroupOnlineListener(const std::string& groupId, int online);
void GroupDeleteListener(const std::string& groupId);

#endif // NUNCHUCKLISTENER_H
