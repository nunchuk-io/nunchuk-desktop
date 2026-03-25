#include "ClaimTransactionFlow.h"
#include "core/ui/UiServices.inc"
#include "core/utils/Utils.h"
#include "generated_qml_keys.hpp"
#include "core/common/resources/AppStrings.h"

namespace features::transactions::flows {
using namespace features::claiming::usecases;
using namespace features::transactions::usecases;

ClaimTransactionFlow::ClaimTransactionFlow(FlowContext *ctx, QObject *parent) : TransactionFlow(ctx, parent) {
    setisClaimTx(true);
}

void ClaimTransactionFlow::proceedTransaction(QVariant feeInput) {
    GUARD_APP_MODEL()
    auto maps = feeInput.toMap();
    bool subtractFromFeeAmout = maps.value("subtractFromFeeAmout").toBool();
    int feeRate = maps.value("feeRate").toDouble() * 1000; // Convert sats/Byte to sats/kB
    bool manualFee = maps.value("manualFee").toBool();
    if (!manualFee)
        feeRate = appModel->fastestFeeOrigin(); // default value
    bool antiFeeSnipping = maps.value("antiFeeSnipping").toBool();
    DBG_INFO << "subtract:" << subtractFromFeeAmout << "| manual Fee:" << manualFee << "| free rate:" << feeRate;

    CreateTransactionInput input;
    input.magic = magicWord();
    input.address = withdrawAddress();
    input.amount = withdrawAmountSats();
    input.fee_rate = feeRate;
    input.anti_fee_sniping = antiFeeSnipping;
    input.bsms = bsms();
    input.subtract_fee_from_amount = subtractFromFeeAmout;
    input.authos = tokenList();
    input.messageId = messageId();
    m_createTransactionUC.executeAsync(input, [this](const core::usecase::Result<CreateTransactionResult> &result) {
        if (result.isSuccess()) {
            proceedClaimTransactionResult(result.value());
        } else {
            emit showToast(-1, result.error(), EWARNING::WarningType::ERROR_MSG);
        }
    });
}

void ClaimTransactionFlow::bind(QObject *vm) {
    TransactionFlow::bind(vm);
}

void ClaimTransactionFlow::autoSignClaimingTransaction() {
    if (backupSigners().empty()) {
        return;
    }
    GUARD_APP_MODEL()
    std::vector<nunchuk::Device> devices;
    for(const auto& signer : backupSigners()) {
        devices.emplace_back(signer.get_master_fingerprint());
    }
    LibSignTransactionInput input;
    input.wallet = nunWallet();
    input.wallet_id = std::nullopt;
    input.tx = nunTx();
    input.tx_id = std::nullopt;
    input.devices = devices;
    m_libSignTransactionUC.executeAsync(input, [this](core::usecase::Result<LibSignTransactionResult> result) {
        if (result.isSuccess()) {
            const auto &signedTx = result.value().tx;
            setnunTx(signedTx);
            if (backupSigners().size() == nunWallet().get_m()) {
                proceedTransaction(signedTx); // for all keys is backup password
            } else if (backupSigners().size() != nunWallet().get_m()) {
                GUARD_SUB_SCREEN_MANAGER()
                subMng->show(qml::features::claiming::qtransactiondetails);
            }
        } else {
            QString errorMsg = result.error();
            emit showToast(0, errorMsg, EWARNING::WarningType::ERROR_MSG);
        }
    });
}

void ClaimTransactionFlow::proceedTransaction(const nunchuk::Transaction &tx) {
    auto txData = txResult();
    txData.psbt = QString::fromStdString(tx.get_psbt());
    proceedClaimTransactionResult(txData);
}

void ClaimTransactionFlow::proceedTransaction(const QString &psbt_signed) {
    auto txData = txResult();
    txData.psbt = psbt_signed;
    proceedClaimTransactionResult(txData);
}

void ClaimTransactionFlow::proceedClaimTransactionResult(const CreateTransactionResult &txData) {
    settxResult(txData);
    QWarningMessage msg;
    auto tx = qUtils::DecodeTx(nunWallet(), txData.psbt, txData.tx_sub_amount, txData.tx_fee, txData.tx_fee_rate, txData.subtract_fee_from_amount, msg);
    if (msg.isSuccess()) {
        tx.set_change_index(txData.change_pos);
        setnunTx(tx);
        emit forwardTransaction();
        msg.resetWarningMessage();
        if (isOnClaim()) {
            QString wallet_local_id = QString::fromStdString(nunWallet().get_id());
            bridge::nunchukImportPsbt(wallet_local_id, txData.psbt, msg);
            GUARD_APP_MODEL()
            if (auto wallet = appModel->walletListPtr()->getWalletById(QString::fromStdString(nunWallet().get_id()))) {
                wallet->CreateAsisstedTxs(QString::fromStdString(tx.get_txid()), QString::fromStdString(tx.get_psbt()), QString::fromStdString(tx.get_memo()));
            }
        }
    } else {
        emit showToast(0, Strings.STR_QML_2117(), EWARNING::WarningType::ERROR_MSG);
    }
}

bool ClaimTransactionFlow::isAllreadySigned() {
    auto wallet = nunWallet();
    auto tx = nunTx();
    int count = 0;
    for (auto &&[xfp, is_signed] : tx.get_signers()) {
        if (is_signed)
            ++count;
    }
    return count == wallet.get_m();
}

void ClaimTransactionFlow::requestClaimInheritance() {
    ClaimInheritanceInput input;
    input.wallet_id = QString::fromStdString(nunWallet().get_id());
    input.txSigned = nunTx();
    input.magic = magicWord();
    input.bsms = bsms();
    m_claimInheritanceUC.executeAsync(input, [this](const core::usecase::Result<ClaimInheritanceResult> &result) {
        if (result.isSuccess()) {
            proceedClaimingResult(result.value());
        } else {
            emit showToast(-1, result.error(), EWARNING::WarningType::ERROR_MSG);
        }
    });
}

void ClaimTransactionFlow::manualClaimingTransaction(const nunchuk::Transaction &tx) {
    setnunTx(tx);
    requestClaimInheritance();
}

void ClaimTransactionFlow::proceedClaimingResult(const ClaimInheritanceResult &claim) {
    auto tx = claim.tx;
    tx.set_status(nunchuk::TransactionStatus::PENDING_CONFIRMATION);
    setnunTx(tx);
    GUARD_SUB_SCREEN_MANAGER()
    subMng->show(qml::features::claiming::qtransactiondetailsclaimed);
}

} // namespace features::transactions::flows
