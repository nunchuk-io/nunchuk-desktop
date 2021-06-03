#include "nunchucklistener.h"
#include "bridgeifaces.h"
#include "AppModel.h"
#include "QOutlog.h"

#ifndef USING_STUB_API

#include <nunchuk.h>

void balance_listener(std::string id, nunchuk::Amount value)
{
    bridge::nunchukBalanceChanged(QString::fromStdString(id), (int)value);
}

void devices_listener(std::string fingerprint, bool connected)
{
    bridge::nunchukDevicesChanged(QString::fromStdString(fingerprint), connected);
}

void transaction_listener(std::string tx_id, nunchuk::TransactionStatus status)
{
    bridge::nunchukTransactionChanged(QString::fromStdString(tx_id), (int)status);
}

void block_listener(int height, std::string hex_header)
{
    bridge::nunchukBlockChanged(height, QString::fromStdString(hex_header));
}

bool create_master_signer_listener(int progress)
{
    DBG_INFO << progress;
    AppModel::instance()->setAddSignerPercentage(progress);
    return true;
}

bool cache_master_signer_XPub(int progress)
{
    DBG_INFO << progress;
    AppModel::instance()->setCacheXpubsPercentage(progress);
    return true;
}

void blockchain_connection_listener(nunchuk::ConnectionStatus status, int percent)
{
    DBG_INFO << (int)status << percent;
    AppSetting::instance()->setConnectionState((int)status);
    AppSetting::instance()->setSyncPercent(percent);
}

bool create_software_signer_listener(int progress)
{
    DBG_INFO << progress;
    AppModel::instance()->setAddSignerPercentage(progress);
    return true;
}
#endif // USING_STUB_API
