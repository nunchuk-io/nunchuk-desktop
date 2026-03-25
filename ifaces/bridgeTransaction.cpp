#include "bridgeTransaction.h"
#include "QWarningMessage.h"

namespace bridge::transaction {
bool UpdateTransactionMemo(const QString &wallet_id, const QString &tx_id, const QString &new_memo) {
    QWarningMessage msg;
    return nunchukiface::instance()->UpdateTransactionMemo(wallet_id.toStdString(), tx_id.toStdString(), new_memo.toStdString(), msg);
}

nunchuk::Transaction ImportTransaction(const QString &wallet_id, const QString &file_path, QWarningMessage &msg) {
    nunchuk::Transaction trans_result = nunchukiface::instance()->ImportTransaction(wallet_id.toStdString(), file_path.toStdString(), msg);
    if ((int)EWARNING::WarningType::NONE_MSG == msg.type()) {
        return trans_result;
    } else {
        return nunchuk::Transaction();
    }
}

nunchuk::Transaction BroadcastTransaction(const QString &wallet_id, const QString &tx_id, QWarningMessage &msg) {
    nunchuk::Transaction trans_result = nunchukiface::instance()->BroadcastTransaction(wallet_id.toStdString(), tx_id.toStdString(), msg);
    if ((int)EWARNING::WarningType::NONE_MSG == msg.type()) {
        return trans_result;
    } else {
        return nunchuk::Transaction();
    }
}

nunchuk::Transaction ImportPsbt(const QString &wallet_id, const QString &psbt, QWarningMessage &msg) {
    return nunchukiface::instance()->ImportPsbt(wallet_id.toStdString(), psbt.toStdString(), msg);
}

nunchuk::Transaction UpdateTransaction(const QString &wallet_id, const QString &tx_id, const QString &new_txid, const QString &raw_tx,
                                       const QString &reject_msg, QWarningMessage &msg) {
    return nunchukiface::instance()->UpdateTransaction(wallet_id.toStdString(), tx_id.toStdString(), new_txid.toStdString(), raw_tx.toStdString(),
                                                       reject_msg.toStdString(), msg);
}

nunchuk::Transaction SignTransaction(const QString &wallet_id, const QString &tx_id, const nunchuk::Device &device, QWarningMessage &msg) {
    return nunchukiface::instance()->SignTransaction(wallet_id.toStdString(), tx_id.toStdString(), device, msg);
}

} // namespace bridge::transaction