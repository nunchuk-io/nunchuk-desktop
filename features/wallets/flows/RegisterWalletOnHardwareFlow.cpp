#include "RegisterWalletOnHardwareFlow.h"
#include "core/ui/UiServices.inc"
#include "Premiums/QWalletServicesTag.h"
#include "generated_qml_keys.hpp"
#include "features/wallets/viewmodels/RegisterWalletOnHardwareViewModel.h"

namespace features::claiming::flows {
using namespace features::claiming::usecases;
using features::wallets::viewmodels::RegisterWalletOnHardwareViewModel;

RegisterWalletOnHardwareFlow::RegisterWalletOnHardwareFlow(FlowContext* ctx, QObject *parent) : core::flow::BaseFlow(ctx, parent) {}

void RegisterWalletOnHardwareFlow::bind(QObject* vm) {
    auto realVm = qobject_cast<RegisterWalletOnHardwareViewModel*>(vm);
    if (realVm) {
        realVm->setEvent(static_cast<RegisterWalletOnHardwareViewModel::FlowEvent>(m_flowEvent));
    }
}

void RegisterWalletOnHardwareFlow::requestWithdrawBitcoin(QVariant msg) {
    QMap<QString, QVariant> maps = msg.toMap();
    if (!maps.isEmpty()) {
        QString firstKey = "isWithdrawBitcoin";
        bool firstValue = maps.value(firstKey).toBool();
        ctx()->serviceSetting()->servicesTagPtr()->updateInheritanceCheckStatus(firstKey, firstValue);
    }
}

void RegisterWalletOnHardwareFlow::withdrawBitcoinClicked(QVariant msg) {
    m_flowEvent = static_cast<int>(RegisterWalletOnHardwareViewModel::FlowEvent::WithdrawBitcoin);
    QMap<QString, QVariant> maps = msg.toMap();
    if (auto wallet = ctx()->serviceSetting()->walletInfoPtr()) {
        GetClaimingWalletInput input;
        input.wallet_id = wallet.data()->walletId();
        m_getClaimingWalletUC.executeAsync(input, [this, msg, maps](core::usecase::Result<GetClaimingWalletResult> result) {
            if (result.isSuccess()) {
                auto walletJson = result.value().wallet;
                bool requires_registration = walletJson.value("requires_registration").toBool();
                bool isRegistered = maps.value("isRegistered").toBool();
                if (!isRegistered && requires_registration) {
                    ctx()->subScreenManager()->show(qml::components::customizes::wallets::qregisterwalletonhardware);
                } else {
                    requestWithdrawBitcoin(msg);
                }
            } else {
                requestWithdrawBitcoin(msg);
            }
        });
    }
}

} // namespace features::claiming::flows