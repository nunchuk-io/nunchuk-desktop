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
#include "Premiums/QSharedWallets.h"
#include "QOutlog.h"
#include "QThreadForwarder.h"
#include <QMetaObject>
#include <nunchuk.h>

void balance_listener(std::string id, nunchuk::Amount balance)
{
    QThreadForwarder::instance()->forwardInQueuedConnection([balance, id](){
        bridge::nunchukBalanceChanged(QString::fromStdString(id), static_cast<qint64>(balance));
    });
}

void balances_listener(string id, nunchuk::Amount balance, nunchuk::Amount unconfirmed_balance)
{
    QThreadForwarder::instance()->forwardInQueuedConnection([id, unconfirmed_balance](){
        bridge::nunchukBalanceChanged(QString::fromStdString(id), static_cast<qint64>(unconfirmed_balance));
    });
}

void devices_listener(std::string fingerprint, bool connected)
{
    QThreadForwarder::instance()->forwardInQueuedConnection([fingerprint, connected](){
        bridge::nunchukDevicesChanged(QString::fromStdString(fingerprint), connected);
    });
}

void transaction_listener(std::string tx_id, nunchuk::TransactionStatus status, std::string wallet_id)
{
    QThreadForwarder::instance()->forwardInQueuedConnection([tx_id, status, wallet_id](){
        bridge::nunchukTransactionChanged(QString::fromStdString(tx_id), (int)status, QString::fromStdString(wallet_id));
    });
}

void block_listener(int height, std::string hex_header)
{
    QThreadForwarder::instance()->forwardInQueuedConnection([height, hex_header](){
        bridge::nunchukBlockChanged(height, QString::fromStdString(hex_header));
    });
}

bool create_master_signer_listener(int progress)
{
    QThreadForwarder::instance()->forwardInQueuedConnection([progress](){
        AppModel::instance()->setAddSignerPercentage(progress);
    });
    return true;
}

bool cache_master_signer_XPub(int progress)
{
    return true;
}

void blockchain_connection_listener(nunchuk::ConnectionStatus status, int percent)
{
    QThreadForwarder::instance()->forwardInQueuedConnection([status, percent](){
        AppSetting::instance()->setConnectionState((int)status);
    });
}

bool create_software_signer_listener(int progress)
{
    QThreadForwarder::instance()->forwardInQueuedConnection([progress](){
        AppModel::instance()->setAddSignerPercentage(progress);
    });
    return true;
}

bool create_software_signer_xprv_listener(int progress)
{
    QThreadForwarder::instance()->forwardInQueuedConnection([progress](){
        AppModel::instance()->setAddSignerPercentage(progress);
    });
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

void GroupUpdateListener(const nunchuk::GroupSandbox &state)
{
    QThreadForwarder::instance()->forwardInQueuedConnection([state](){
        QWarningMessage msg;
        nunchuk::GroupSandbox sanbox = bridge::GetGroup(QString::fromStdString(state.get_id()), msg);
        if((int)EWARNING::WarningType::NONE_MSG == msg.type()) {
            if(QSharedWallets::instance()->sandboxList()){
                DBG_INFO << sanbox.get_name() << sanbox.get_url();
                QSharedWallets::instance()->sandboxList()->updateSandox(sanbox);
            }
            if(AppModel::instance()->walletList()){
                AppModel::instance()->walletList()->updateGroupSandbox(QString::fromStdString(sanbox.get_wallet_id()), sanbox);
            }
            if (auto nw = AppModel::instance()->newWalletInfo()) {
                if (auto sandbox = nw->groupSandboxPtr()) {
                    if (qUtils::strCompare(sandbox->groupId(), QString::fromStdString(sanbox.get_id()))) {
                        nw->updateGroupSandbox(sanbox);
                    }
                }
            }
            if (auto w = AppModel::instance()->walletInfo()) {
                if (auto sandbox = w->groupSandboxPtr()) {
                    if (qUtils::strCompare(sandbox->groupId(), QString::fromStdString(sanbox.get_id()))) {
                        w->updateGroupSandbox(sanbox);
                    }
                }
            }
        }
    });
}

void GroupMessageListener(const nunchuk::GroupMessage &msg)
{
    DBG_INFO << msg.get_wallet_id() << QThread::currentThread() << qApp->thread();
    QThreadForwarder::instance()->forwardInQueuedConnection([msg](){
        if(auto list = AppModel::instance()->walletList()){
            list->updateGroupMessage(QString::fromStdString(msg.get_wallet_id()), msg);
        }
        if(auto list = AppModel::instance()->groupWalletList()) {
            list->updateGroupMessage(QString::fromStdString(msg.get_wallet_id()), msg);
        }
    });
}

void GroupOnlineListener(const std::string &groupId, int online)
{
    DBG_INFO << "groupId: " << groupId << "online: " << online;
    QThreadForwarder::instance()->forwardInQueuedConnection([groupId, online](){
        if (auto nw = AppModel::instance()->newWalletInfo()) {
            if (auto sandbox = nw->groupSandboxPtr()) {
                if (qUtils::strCompare(sandbox->groupId(), QString::fromStdString(groupId))) {
                    sandbox->setUserCount(online);
                }
            }
        }
        if (auto w = AppModel::instance()->walletInfo()) {
            w->numberOnlineChanged();
            if (auto sandbox = w->groupSandboxPtr()) {
                if (qUtils::strCompare(sandbox->groupId(), QString::fromStdString(groupId))) {
                    sandbox->setUserCount(online);
                }
            }
        }
    });
}

void GroupDeleteListener(const std::string &groupId)
{
    DBG_INFO;
    QThreadForwarder::instance()->forwardInQueuedConnection([groupId](){
        if (auto nw = AppModel::instance()->newWalletInfo()) {
            if (auto sandbox = nw->groupSandboxPtr()) {
                sandbox->DeleteGroup(QString::fromStdString(groupId));
            }
        }
    });
}
