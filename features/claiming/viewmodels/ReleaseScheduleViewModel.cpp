#include "ReleaseScheduleViewModel.h"
#include "QEventProcessor.h"
#include "core/ui/UiServices.inc"
#include "features/claiming/flows/ClaimingFlow.h"
#include "features/inheritance/offchain/flows/PhasedRolloutFlow.h"
#include "generated_qml_keys.hpp"

namespace features::claiming::viewmodels {
using namespace core::viewmodels;
using namespace features::claiming::usecases;
using features::claiming::flows::ClaimingFlow;
using namespace features::inheritance::offchain::flows;

ReleaseScheduleViewModel::ReleaseScheduleViewModel(QObject *parent) : ActionViewModel(parent) {
    setavailableBalanceSats(0);
}

void ReleaseScheduleViewModel::onInit() {
    GUARD_FLOW_MANAGER()
    auto flow = flowMng->currentFlow();
    auto claimingFlow = qobject_cast<ClaimingFlow *>(flow);
    if (!claimingFlow) {
        return;
    }

    QJsonArray stagesArray = claimingFlow->stages();
    setstagesData(helper::convertStagesData(stagesArray));
    setavailableBalanceSats(claimingFlow->availableBalanceSats());
    setavailableBalanceDisplay(claimingFlow->availableBalanceDisplay());
    setavailableBalanceCurrency(claimingFlow->availableBalanceCurrency());
    setcurrent_stage_index(claimingFlow->current_stage_index());
    setcurrent_installment_index(claimingFlow->current_installment_index());
    setavailablePercentage(0);
    if (stagesArray.size() > current_stage_index()) {
        QJsonObject currentStage = stagesArray.at(current_stage_index()).toObject();
        QJsonArray installmentsArray = currentStage.value("expanded_installments").toArray();
        for (int i = 0; i < installmentsArray.size(); ++i) {
            QJsonObject installment = installmentsArray.at(i).toObject();
            if (installment.value("index").toInt() == current_installment_index()) {
                int allocation_percentage = installment.value("allocation_percentage").toInt();
                setavailablePercentage(allocation_percentage);
                break;
            }
        }
    }
}

void ReleaseScheduleViewModel::requestWithdrawBitcoin() {
    GUARD_RIGHT_PANEL_NAV()
    rightPanel->request(qml::components::rightpannel::service::common::qserviceclaiminheritancephaserolloutwithdrawbitcoin);
    close();
}

void ReleaseScheduleViewModel::withdrawBitcoinClicked() {
    if (isRegistered()) {
        requestWithdrawBitcoin();
    } else {
        GUARD_FLOW_MANAGER()
        auto currentFlow = flowMng->currentFlow();
        auto claimingFlow = qobject_cast<ClaimingFlow *>(currentFlow);
        if (!claimingFlow) {
            return;
        }
        setisRegistered(true);
        claimingFlow->setisRegistered(isRegistered());
        GetClaimingWalletInput input;
        input.wallet_id = QString::fromStdString(claimingFlow->nunWallet().get_id());
        m_getClaimingWalletUC.executeAsync(input, [this](core::usecase::Result<GetClaimingWalletResult> result) {
            if (result.isSuccess()) {
                auto walletJson = result.value().wallet;
                bool requires_registration = walletJson.value("requires_registration").toBool();
                if (requires_registration) {
                    GUARD_SUB_SCREEN_MANAGER()
                    subMng->show(qml::components::customizes::wallets::qregisterwalletonhardware);
                } else {
                    requestWithdrawBitcoin();
                }
            } else {
                requestWithdrawBitcoin();
            }
        });
    }
}

} // namespace features::claiming::viewmodels
