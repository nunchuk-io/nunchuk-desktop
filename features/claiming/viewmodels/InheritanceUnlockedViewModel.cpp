#include "InheritanceUnlockedViewModel.h"
#include "core/ui/UiServices.inc"
#include "features/wallets/flows/RegisterWalletOnHardwareFlow.h"

namespace features::claiming::viewmodels {
using namespace core::viewmodels;
using features::claiming::flows::RegisterWalletOnHardwareFlow;
InheritanceUnlockedViewModel::InheritanceUnlockedViewModel(QObject *parent)
    : ActionViewModel(parent) {
}

void InheritanceUnlockedViewModel::withdrawBitcoinClicked(QVariant msg) {
    GUARD_FLOW_MANAGER()
    QMap<QString, QVariant> maps = msg.toMap();
    maps["isRegistered"] = isRegistered;
    auto flow = flowMng->startFlow<RegisterWalletOnHardwareFlow>();
    flow->withdrawBitcoinClicked(QVariant::fromValue(maps));
    if (!isRegistered) {
        isRegistered = true;
    }
}
} // namespace features::claiming::viewmodels