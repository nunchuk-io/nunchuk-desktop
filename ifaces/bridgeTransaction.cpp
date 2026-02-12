#include "bridgeTransaction.h"
#include "QWarningMessage.h"

namespace bridge::transaction {
    bool UpdateTransactionMemo(const QString &wallet_id, const QString &tx_id, const QString &new_memo) {
        QWarningMessage msg;
        return nunchukiface::instance()->UpdateTransactionMemo(wallet_id.toStdString(),
                                                        tx_id.toStdString(),
                                                        new_memo.toStdString(),
                                                        msg);
    }
}