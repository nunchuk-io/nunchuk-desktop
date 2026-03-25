#include "InheritanceUnlockedViewModel.h"
#include "QEventProcessor.h"
#include "ViewsEnums.h"
#include "core/ui/UiServices.inc"
#include "features/claiming/flows/ClaimingFlow.h"
#include "generated_qml_keys.hpp"
#include <QLocale>

namespace features::claiming::viewmodels {
using namespace core::viewmodels;
using namespace features::claiming::usecases;
using features::claiming::flows::ClaimingFlow;

InheritanceUnlockedViewModel::InheritanceUnlockedViewModel(QObject *parent) : ActionViewModel(parent) {}

void InheritanceUnlockedViewModel::requestWithdrawBitcoin() {
    GUARD_RIGHT_PANEL_NAV()
    rightPanel->request(qml::components::rightpannel::service::common::qserviceclaiminheritancewithdrawbitcoin);
}

void InheritanceUnlockedViewModel::viewWalletClicked() {
    GUARD_APP_MODEL()
    QEventProcessor::instance()->sendEvent(E::EVT_GOTO_HOME_WALLET_TAB);
}

void InheritanceUnlockedViewModel::withdrawBitcoinClicked() {
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