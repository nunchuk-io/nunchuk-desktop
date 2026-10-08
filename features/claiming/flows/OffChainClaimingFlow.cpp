#include "OffChainClaimingFlow.h"
#include "core/bridge/ExternalBridges.h"
#include "core/common/resources/AppStrings.h"
#include "core/ui/UiServices.inc"
#include "features/claiming/viewmodels/BackupPasswordViewModel.h"
#include "features/claiming/viewmodels/ProceedOptionsViewModel.h"
#include "features/claiming/viewmodels/VerifyInheritanceKeyViewModel.h"
#include "features/wallets/viewmodels/RegisterWalletOnHardwareViewModel.h"
#include "generated_qml_keys.hpp"

namespace features::claiming::flows {
using namespace features::claiming::viewmodels;
using features::wallets::viewmodels::RegisterWalletOnHardwareViewModel;

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
        // Qt::UniqueConnection: defensive backstop in case bind() ever re-runs for the same vm
        // instance (would otherwise stack duplicate connections and pop >1 signer per back-click).
        connect(realVm1, &VerifyInheritanceKeyViewModel::signalBack, this, &OffChainClaimingFlow::rollbackSigner, Qt::UniqueConnection);
        realVm1->setmagicWord(magicWord());
        realVm1->setwalletType(walletType());
        realVm1->setaddressType(addressType());
        // Confirmed with backend: one signing-challenge message is shared across all keys in a session.
        if (!challengeMessage().isEmpty()) {
            realVm1->setmessage(challengeMessage());
            realVm1->setmessageId(challengeMessageId());
        }
        realVm1->proceedVerification(currentSigner());
    }

    auto realVm2 = qobject_cast<BackupPasswordViewModel *>(vm);
    if (realVm2) {
        realVm2->setmagicWord(magicWord());
        realVm2->setbsms(bsms());
        int remainingCount = keyOrigins().size() - signers().size();
        realVm2->setremainingCount(remainingCount);
    }

    // BUGFIX: D14 "Register wallet on hardware" reads AppModel.walletInfo (the app's currently open
    // wallet) unless vm.event == WithdrawBitcoin, in which case it uses vm.walletInfo instead
    // (QRegisterWalletOnHardware.qml:52,423-429). OnChainClaimingFlow already sets nunWallet/event
    // here; off-chain claiming reached this same D14 screen (InheritanceUnlockedViewModel::
    // withdrawBitcoinClicked() -> requires_registration) without this wiring, so it would silently
    // export/register whatever wallet happens to be open in the app instead of the claimed one.
    auto realVm3 = qobject_cast<RegisterWalletOnHardwareViewModel *>(vm);
    if (realVm3) {
        realVm3->setnunWallet(nunWallet());
        realVm3->setEvent(RegisterWalletOnHardwareViewModel::FlowEvent::WithdrawBitcoin);
    }

    ClaimingFlow::bind(vm);
}

void OffChainClaimingFlow::proceedResult(const nunchuk::SingleSigner &single) {
    // BUGFIX: addSingleSigner()'s return value (false = duplicate fingerprint, already added) was
    // discarded, so re-adding the same device silently did nothing but still navigated to the verify
    // screen as if it had succeeded - no feedback to the user, and a later "back" from that screen
    // would pop the wrong (previously, legitimately added) signer off the list instead of just
    // canceling this no-op attempt. BackupPasswordViewModel::createTokenForBackupPassword() already
    // checks this same return value; hardware/existing-key adds now do too.
    if (!addSingleSigner(single)) {
        emit showToast(-1, Strings.STR_QML_090(), EWARNING::WarningType::ERROR_MSG);
        return;
    }
    setcurrentSigner(single);
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
    // BUGFIX: on wrong xfp, addKey() only shows a toast - it never leaves the "Adding [Device]..."
    // loading screen (pushed by HardwareRefreshDevicesViewModel::requestCreateSigner()), so the user
    // was stuck there forever with no visible error. Pop it here, scoped to the USB path only - the
    // other 3 addKey() callers (select-existing-key, recover-via-seed, recover-via-xprv) call
    // proceedAfterXxx() directly from their own screen with no loading screen pushed, so back() there
    // would incorrectly pop the user's current screen instead.
    if (!isCorrectXFP(xfp)) {
        GUARD_SUB_SCREEN_MANAGER()
        subMng->back();
    }
    addKey(xfp);
}

bool OffChainClaimingFlow::isCorrectXFP(const QString &xfp) {
    auto keyOriginsArray = keyOrigins();
    for (const auto &origin : keyOriginsArray) {
        QString originXfp = origin.toObject().value("xfp").toString();
        // BUGFIX: MasterSigner::get_id() (the xfp of a just-added hardware signer) is always
        // lowercased by libnunchuk (storage.cpp: to_lower_copy(...)), while key_origins[].xfp from
        // the backend is uppercase - a case-sensitive compare here always failed for freshly-added
        // hardware keys, silently stopping the flow (toast only, no screen transition) even though
        // the signer was created successfully. Use the project's existing qUtils::strCompare()
        // (case-insensitive, trimmed) instead of raw == - same helper used for other string matches.
        if (qUtils::strCompare(originXfp, xfp)) {
            return true;
        }
    }
    return false;
}

int OffChainClaimingFlow::expectedAccountIndex(const QString &xfp) {
    auto keyOriginsArray = keyOrigins();
    for (const auto &origin : keyOriginsArray) {
        QString originXfp = origin.toObject().value("xfp").toString();
        if (qUtils::strCompare(originXfp, xfp)) {
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