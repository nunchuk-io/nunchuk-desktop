#include "WalletListViewModel.h"
#include "core/ui/UiServices.inc"
#include "features/wallets/flows/SyncWalletFromRemoteFlow.h"

namespace features::wallets::viewmodels {
using namespace core::viewmodels;
using features::wallets::flows::SyncWalletFromRemoteFlow;

WalletListViewModel::WalletListViewModel(QObject *parent) : ActionViewModel(parent) {}

WalletListViewModel::~WalletListViewModel() {
    GUARD_FLOW_MANAGER()
    flowMng->stopFlow<SyncWalletFromRemoteFlow>();
}

void WalletListViewModel::onInit() {
    GUARD_FLOW_MANAGER()
    flowMng->startFlow<SyncWalletFromRemoteFlow>();
}

} // namespace features::wallets::viewmodels
