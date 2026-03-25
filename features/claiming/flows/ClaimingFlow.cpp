#include "ClaimingFlow.h"
#include "core/bridge/ExternalBridges.h"
#include "core/common/resources/AppStrings.h"
#include "core/ui/UiServices.inc"
#include "core/utils/Utils.h"
#include "features/claiming/viewmodels/BufferPeriodHasStartedViewModel.h"
#include "features/claiming/viewmodels/InheritanceUnlockedViewModel.h"
#include "features/claiming/viewmodels/WidthdrawToAddressViewModel.h"
#include "features/claiming/viewmodels/WithdrawACustomAmountViewModel.h"
#include "features/claiming/viewmodels/WithdrawBitcoinViewModel.h"
#include "features/claiming/viewmodels/WithdrawSelectWalletViewModel.h"
#include "features/claiming/viewmodels/YourPlanRequireInheritanceKeys.h"
#include "features/transactions/viewmodels/TransactionDetailsClaimedViewModel.h"
#include "generated_qml_keys.hpp"
#include <QLocale>

namespace features::claiming::flows {
using namespace features::claiming::viewmodels;
using namespace features::transactions::viewmodels;
using namespace features::claiming::usecases;
using namespace features::transactions::usecases;

ClaimingFlow::ClaimingFlow(FlowContext *ctx, QObject *parent) : KeyProceedFlow(ctx, parent) {}

void ClaimingFlow::claimStatus() {
    ClaimStatusInput input;
    input.magic = magicWord();
    input.bsms = bsms();
    input.authos = tokenList();
    input.messageId = messageId();
    m_claimStatusUC.executeAsync(input, [this](const core::usecase::Result<ClaimStatusResult> &result) {
        if (result.isSuccess()) {
            proceedClaimStatusResult(result.value());
        } else {
            emit showToast(-1, result.error(), EWARNING::WarningType::ERROR_MSG);
        }
    });
}

void ClaimingFlow::createTransaction() {
    CreateTransactionInput input;
    input.magic = magicWord();
    input.address = withdrawAddress();
    input.amount = withdrawAmountSats();
    input.fee_rate = fee_rate();
    input.anti_fee_sniping = anti_fee_sniping();
    input.bsms = bsms();
    input.subtract_fee_from_amount = subtract_fee_from_amount();
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

bool ClaimingFlow::addSingleSigner(const nunchuk::SingleSigner &single) {
    auto currentSigners = signers();
    if (std::find_if(currentSigners.begin(), currentSigners.end(), [&single](const nunchuk::SingleSigner &s) {
            return s.get_master_fingerprint() == single.get_master_fingerprint();
        }) != currentSigners.end()) {
        return false;
    }
    currentSigners.push_back(single);
    setsigners(currentSigners);
    return true;
}

void ClaimingFlow::displayStatusInfo(const ClaimStatusResult &status) {
    static const int64_t COIN = 100000000;
    GUARD_APP_SETTING()
    int balanceSats = status.balance * COIN;
    QLocale locale(QLocale::English);
    QString balanceDisplay;
    if ((int)AppSetting::Unit::SATOSHI == appSetting->unit()) {
        balanceDisplay = locale.toString(balanceSats); // sats
    } else {
        balanceDisplay = locale.toString(status.balance, 'f', qUtils::Precision(status.balance));
    }
    setbalanceSats(balanceSats);
    setbalanceDisplay(balanceDisplay);
    setbalanceCurrency(qUtils::currencyLocale(balanceSats));
    setnote(status.inheritance.value("note").toString());
    setremaining_display_name(status.buffer_period_countdown.value("remaining_display_name").toString());
}

void ClaimingFlow::proceedClaimStatusResult(const ClaimStatusResult &status) {
    displayStatusInfo(status);
    GUARD_RIGHT_PANEL_NAV()
    if (!status.buffer_period_countdown.isEmpty()) { // Buffer period has started
        rightPanel->request(qml::components::rightpannel::service::common::qserviceclaiminheritancebufferperiodhasstarted);
    } else if (!status.inheritance.isEmpty()) { // Inheritance found
        rightPanel->request(qml::components::rightpannel::service::common::qserviceclaiminheritanceyourinheritance);
    }
}

bool ClaimingFlow::addAuthorization(const QString &token) {
    auto list = tokenList();
    const auto hasFingerprintInTokenList = [&list](const QString &inputToken) -> bool {
        QString inputSignature, inputFingerprint;
        if (!qUtils::SplitRequestToken(inputToken, inputSignature, inputFingerprint)) {
            return false;
        }

        return std::any_of(list.begin(), list.end(), [&inputFingerprint](const QString &existingToken) {
            QString signature, fingerprint;
            return qUtils::SplitRequestToken(existingToken, signature, fingerprint) && qUtils::strCompare(fingerprint, inputFingerprint);
        });
    };

    QString inputSignature, inputFingerprint;
    if (!qUtils::SplitRequestToken(token, inputSignature, inputFingerprint)) {
        return false;
    }

    if (hasFingerprintInTokenList(token)) {
        // Remove old token with same fingerprint
        auto it = std::remove_if(list.begin(), list.end(), [&inputFingerprint](const QString &existingToken) {
            QString signature, fingerprint;
            return qUtils::SplitRequestToken(existingToken, signature, fingerprint) && qUtils::strCompare(fingerprint, inputFingerprint);
        });
        list.erase(it, list.end());
    }

    if (list.contains(token)) {
        return false;
    }
    list.append(token);
    settokenList(list);
    return true;
}

void ClaimingFlow::bind(QObject *vm) {
    auto realVm2 = qobject_cast<YourPlanRequireInheritanceKeys *>(vm);
    if (realVm2) {
        realVm2->setinheritance_key_count(inheritance_key_count());
    }

    auto realVm3 = qobject_cast<BufferPeriodHasStartedViewModel *>(vm);
    if (realVm3) {
        realVm3->setremaining_display_name(remaining_display_name());
    }
    auto realVm4 = qobject_cast<InheritanceUnlockedViewModel *>(vm);
    if (realVm4) {
        realVm4->setnote(note());
        realVm4->setbalanceDisplay(balanceDisplay());
        realVm4->setbalanceCurrency(balanceCurrency());
        realVm4->setisRegistered(isRegistered());
        realVm4->setisOffClaim(qUtils::strCompare(id(), "OffChainClaimingFlow"));
    }

    auto realVm5 = qobject_cast<WithdrawBitcoinViewModel *>(vm);
    if (realVm5) {
        realVm5->setnote(note());
        realVm5->setbalanceDisplay(balanceDisplay());
        realVm5->setbalanceCurrency(balanceCurrency());
    }

    auto realVm6 = qobject_cast<WithdrawACustomAmountViewModel *>(vm);
    if (realVm6) {
        realVm6->setnote(note());
        realVm6->setbalanceDisplay(balanceDisplay());
        realVm6->setbalanceCurrency(balanceCurrency());
    }

    auto realVm7 = qobject_cast<WidthdrawToAddressViewModel *>(vm);
    if (realVm7) {
        realVm7->setwithdrawAmountSats(withdrawAmountSats());
        setisAddressFlow(true);
        connect(this, &ClaimingFlow::forwardTransaction, realVm7, &WidthdrawToAddressViewModel::proceedTransactionResult);
    }

    auto realVm8 = qobject_cast<WithdrawSelectWalletViewModel *>(vm);
    if (realVm8) {
        realVm8->setwithdrawAmountSats(withdrawAmountSats());
        setisAddressFlow(false);
        connect(this, &ClaimingFlow::forwardTransaction, realVm8, &WithdrawSelectWalletViewModel::proceedTransactionResult);
    }

    auto realVm = qobject_cast<TransactionDetailsClaimedViewModel *>(vm);
    if (realVm) {
        realVm->setisAddressFlow(isAddressFlow());
        realVm->setnunTx(nunTx());
        realVm->setnunWallet(nunWallet());
    }
}

} // namespace features::claiming::flows