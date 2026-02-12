#pragma once
#include "nunchuckiface.h"
#include <QString>

namespace bridge::transaction {
    bool UpdateTransactionMemo(const QString &wallet_id, const QString &tx_id, const QString &new_memo);
}