#pragma once
#include "nunchuckiface.h"
#include <QString>

namespace bridge::transaction {
bool UpdateTransactionMemo(const QString &wallet_id, const QString &tx_id, const QString &new_memo);
nunchuk::Transaction ImportTransaction(const QString &wallet_id, const QString &file_path, QWarningMessage &msg);
nunchuk::Transaction BroadcastTransaction(const QString &wallet_id, const QString &tx_id, QWarningMessage &msg);
nunchuk::Transaction ImportPsbt(const QString &wallet_id, const QString &psbt, QWarningMessage &msg);
nunchuk::Transaction UpdateTransaction(const QString &wallet_id, const QString &tx_id, const QString &new_txid, const QString &raw_tx,
                                       const QString &reject_msg, QWarningMessage &msg);
nunchuk::Transaction SignTransaction(const QString &wallet_id, const QString &tx_id, const nunchuk::Device &device, QWarningMessage &msg);
} // namespace bridge::transaction