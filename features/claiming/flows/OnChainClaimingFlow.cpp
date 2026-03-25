#include "OnChainClaimingFlow.h"
#include "core/ui/UiServices.inc"
#include "core/bridge/ExternalBridges.h"
#include "core/common/resources/AppStrings.h"
#include "features/common/viewmodels/BaseGuideViewModel.h"
#include "features/wallets/viewmodels/RegisterWalletOnHardwareViewModel.h"

namespace features::claiming::flows {
using features::common::viewmodels::BaseGuideViewModel;
using features::wallets::viewmodels::RegisterWalletOnHardwareViewModel;

OnChainClaimingFlow::OnChainClaimingFlow(FlowContext* ctx, QObject *parent) : ClaimingFlow(ctx, parent) {}

void OnChainClaimingFlow::passClaimingError(int code, const QString& message) {
    m_errorCode = code;
    m_errorMessage = message;
}

void OnChainClaimingFlow::bind(QObject* vm) {
    auto realVm = qobject_cast<BaseGuideViewModel*>(vm);
    if (realVm) {
        realVm->settitle(getTitleByErrorCode(m_errorCode));
        realVm->setdescription(m_errorMessage);      
    }
    auto realVm1 = qobject_cast<RegisterWalletOnHardwareViewModel*>(vm);
    if (realVm1) {
        realVm1->setnunWallet(nunWallet());
        realVm1->setEvent(RegisterWalletOnHardwareViewModel::FlowEvent::WithdrawBitcoin);
    }
    ClaimingFlow::bind(vm);
}

QString OnChainClaimingFlow::getTitleByErrorCode(int code) {
    switch (code) {
    case DRACO_CODE::INHERITANCE_803:
        return Strings.STR_QML_2077();
    case DRACO_CODE::INHERITANCE_831:
        return Strings.STR_QML_2045();
    default:
        return "Claiming Error";
    }
}

void OnChainClaimingFlow::proceedClaimTransactionResult(const CreateTransactionResult &txData) {
    QWarningMessage msg;
    auto tx = qUtils::DecodeTx(nunWallet(), txData.psbt, txData.tx_sub_amount, txData.tx_fee, txData.tx_fee_rate, txData.subtract_fee_from_amount, msg);

    if (msg.isSuccess()) {
        tx.set_wallet_type(nunchuk::WalletType::MINISCRIPT);
        tx.set_change_index(txData.change_pos);
        setnunTx(tx);
        emit forwardTransaction();
        QString wallet_local_id = QString::fromStdString(nunWallet().get_id());
        bridge::nunchukImportPsbt(wallet_local_id, txData.psbt, msg);
        GUARD_APP_MODEL()
        if (auto wallet = appModel->walletListPtr()->getWalletById(QString::fromStdString(nunWallet().get_id()))) {
            wallet->CreateAsisstedTxs(QString::fromStdString(tx.get_txid()), QString::fromStdString(tx.get_psbt()), QString::fromStdString(tx.get_memo()));
        }
    } else {
        emit showToast(msg.code(), msg.what(), (EWARNING::WarningType)msg.type());
    }
}

} // namespace features::claiming::flows