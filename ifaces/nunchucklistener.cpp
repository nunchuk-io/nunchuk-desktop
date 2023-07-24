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
#include "nunchucklistener.h"
#include "bridgeifaces.h"
#include "AppModel.h"
#include "QOutlog.h"
#include <nunchuk.h>

void balance_listener(std::string id, nunchuk::Amount balance)
{
    bridge::nunchukBalanceChanged(QString::fromStdString(id), static_cast<qint64>(balance));
}

void balances_listener(string id, nunchuk::Amount balance, nunchuk::Amount unconfirmed_balance)
{
    DBG_INFO;
    bridge::nunchukBalanceChanged(QString::fromStdString(id), static_cast<qint64>(unconfirmed_balance));
}

void devices_listener(std::string fingerprint, bool connected)
{
    bridge::nunchukDevicesChanged(QString::fromStdString(fingerprint), connected);
}

void transaction_listener(std::string tx_id, nunchuk::TransactionStatus status, std::string wallet_id)
{
    DBG_INFO;
    bridge::nunchukTransactionChanged(QString::fromStdString(tx_id), (int)status, QString::fromStdString(wallet_id));
}

void block_listener(int height, std::string hex_header)
{
    bridge::nunchukBlockChanged(height, QString::fromStdString(hex_header));
}

bool create_master_signer_listener(int progress)
{
    AppModel::instance()->setAddSignerPercentage(progress);
    return true;
}

bool cache_master_signer_XPub(int progress)
{
    return true;
}

void blockchain_connection_listener(nunchuk::ConnectionStatus status, int percent)
{
    AppSetting::instance()->setConnectionState((int)status);
}

bool create_software_signer_listener(int progress)
{
    AppModel::instance()->setAddSignerPercentage(progress);
    return true;
}

bool CacheTapsignerMasterSignerXPubProgress(int percent)
{
    return true;
}

bool CacheDefaultTapsignerMasterSignerXPubProgress(int percent)
{
    return true;
}

bool ImportTapsignerMasterSignerProgress(int percent)
{
    return true;
}
