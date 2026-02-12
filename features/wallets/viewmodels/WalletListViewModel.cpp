#include "WalletListViewModel.h"
#include "core/ui/UiServices.inc"
#include "features/wallets/flows/SyncWalletFromRemoteFlow.h"

namespace features::wallets::viewmodels {
using namespace core::viewmodels;
using features::wallets::flows::SyncWalletFromRemoteFlow;

WalletListViewModel::WalletListViewModel(QObject *parent) 
: ActionViewModel(parent)
{
}


void WalletListViewModel::onInit() {
    ctx()->flowManager()->startFlow<SyncWalletFromRemoteFlow>();
}

void WalletListViewModel::onDispose() {
    ctx()->flowManager()->stopFlow<SyncWalletFromRemoteFlow>();
}

} // namespace features::wallets::viewmodels
