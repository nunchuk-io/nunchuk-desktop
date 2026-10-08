#include "OffChainClaimingFlow.h"
#include "core/bridge/ExternalBridges.h"
#include "core/common/resources/AppStrings.h"
#include "core/ui/UiServices.inc"
#include "features/claiming/viewmodels/BackupPasswordViewModel.h"
#include "features/claiming/viewmodels/ProceedOptionsViewModel.h"
#include "features/claiming/viewmodels/VerifyInheritanceKeyViewModel.h"
#include "generated_qml_keys.hpp"

namespace features::claiming::flows {
using namespace features::claiming::viewmodels;

OffChainClaimingFlow::OffChainClaimingFlow(FlowContext *ctx, QObject *parent) : ClaimingFlow(ctx, parent) {
    setwalletType(nunchuk::WalletType::MULTI_SIG);
    setaddressType(nunchuk::AddressType::NATIVE_SEGWIT);
    setisRegistered(true);
    setfee_rate(1000.0);
    setanti_fee_sniping(false);
    setsubtract_fee_from_amount(false);
}

void OffChainClaimingFlow::bind(QObject *vm) {
    auto realVm1 = qobject_cast<VerifyInheritanceKeyViewModel *>(vm);
    if (realVm1) {
        connect(realVm1, &VerifyInheritanceKeyViewModel::signalBack, this, &OffChainClaimingFlow::rollbackSigner);
        realVm1->setmagicWord(magicWord());
        realVm1->setwalletType(walletType());
        realVm1->setaddressType(addressType());
        realVm1->proceedVerification(currentSigner());
    }

    auto realVm2 = qobject_cast<BackupPasswordViewModel *>(vm);
    if (realVm2) {
        realVm2->setmagicWord(magicWord());
        realVm2->setbsms(bsms());
        int remainingCount = keyOrigins().size() - signers().size();
        realVm2->setremainingCount(remainingCount);
    }

    ClaimingFlow::bind(vm);
}

void OffChainClaimingFlow::proceedResult(const nunchuk::SingleSigner &single) {
    setcurrentSigner(single);
    addSingleSigner(single);
    GUARD_SUB_SCREEN_MANAGER()
    subMng->show(qml::features::claiming::offchain::qverifyinheritancekey);
}

void OffChainClaimingFlow::proceedAfterFileImportColdcard(const std::vector<nunchuk::SingleSigner> &signers, const QString &signerName) {
    if (signers.empty()) {
        emit showToast(-1, Strings.STR_QML_2093(), EWARNING::WarningType::ERROR_MSG);
        return;
    }
    QString xfp = QString::fromStdString(signers[0].get_master_fingerprint());
    if (isCorrectXFP(xfp)) {
        setaccountIndex(expectedAccountIndex(xfp));
        nunchuk::SingleSigner single = bridge::signer::PickSignerFromList(signers, accountIndex());
        if (single.get_master_fingerprint().empty()) {
            QString message = Strings.STR_QML_2094().arg(accountIndex()).arg(xfp);
            emit showToast(-1, message, EWARNING::WarningType::ERROR_MSG);
            return;
        }
        QString inputName = getSignerName(signerName);
        single.set_name(inputName.toStdString());
        single.set_tags({nunchuk::SignerTag::COLDCARD});
        proceedResult(single);
    } else {
        emit showToast(-1, Strings.STR_QML_2093(), EWARNING::WarningType::ERROR_MSG);
    }
}

void OffChainClaimingFlow::proceedAfterQrImportColdcard(const std::vector<nunchuk::SingleSigner> &signers, const QString &signerName) {
    proceedAfterFileImportColdcard(signers, signerName);
}

void OffChainClaimingFlow::proceedAfterSelectExistKey(const QString &xfp) {
    addKey(xfp);
}

void OffChainClaimingFlow::proceedAfterRecoverViaSeed(const QString &xfp) {
    addKey(xfp);
}

void OffChainClaimingFlow::proceedAfterRecoverViaXprv(const QString &xfp) {
    addKey(xfp);
}

void OffChainClaimingFlow::proceedAfterAddedViaUSB(const QString &xfp) {
    addKey(xfp);
}

bool OffChainClaimingFlow::isCorrectXFP(const QString &xfp) {
    auto keyOriginsArray = keyOrigins();
    for (const auto &origin : keyOriginsArray) {
        QString originXfp = origin.toObject().value("xfp").toString();
        if (originXfp == xfp) {
            return true;
        }
    }
    return false;
}

int OffChainClaimingFlow::expectedAccountIndex(const QString &xfp) {
    auto keyOriginsArray = keyOrigins();
    for (const auto &origin : keyOriginsArray) {
        QString originXfp = origin.toObject().value("xfp").toString();
        if (originXfp == xfp) {
            QString derivation_path = origin.toObject().value("derivation_path").toString();
            return qUtils::GetIndexFromPath(derivation_path);
        }
    }
    return -1;
}

void OffChainClaimingFlow::addKey(const QString &xfp) {
    if (isCorrectXFP(xfp)) {
        setaccountIndex(expectedAccountIndex(xfp));
        requestGetSigner(xfp);
    } else {
        emit showToast(-1, Strings.STR_QML_2093(), EWARNING::WarningType::ERROR_MSG);
    }
}

bool OffChainClaimingFlow::isAllKeysAdded() {
    auto keyOriginsArray = keyOrigins();
    auto ret = signers().size() >= static_cast<size_t>(keyOriginsArray.size());
    if (ret) {
        auto wallet = nunchuk::Wallet("", keyOriginsArray.size(), keyOriginsArray.size(), signers(), nunchuk::AddressType::NATIVE_SEGWIT, false, 0, true);
        setnunWallet(wallet);
        DBG_INFO << "All keys added. Wallet is ready for claiming:" << wallet.get_id();
    }
    return ret;
}

void OffChainClaimingFlow::rollbackSigner() {
    auto currentSigners = signers();
    if (!currentSigners.empty()) {
        currentSigners.pop_back();
        setsigners(currentSigners);
    }
}

void OffChainClaimingFlow::proceedClaimTransactionResult(const CreateTransactionResult &txData) {
    QWarningMessage msg;
    auto tx = qUtils::DecodeTx(nunWallet(), txData.psbt, txData.tx_sub_amount, txData.tx_fee, txData.tx_fee_rate, txData.subtract_fee_from_amount, msg);

    if (msg.isSuccess()) {
        tx.set_change_index(txData.change_pos);
        setnunTx(tx);
        emit forwardTransaction();
    } else {
        emit showToast(msg.code(), msg.what(), (EWARNING::WarningType)msg.type());
    }
}

bool OffChainClaimingFlow::isManualClaiming() {
    return backupSigners().size() == 0 || (backupSigners().size() > 0 && backupSigners().size() < keyOrigins().size());
}

} // namespace features::claiming::flows