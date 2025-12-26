#include "RegisterWalletOnHardwareViewModel.h"
#include "core/ui/UiServices.inc"

namespace features::wallets::viewmodels {
using namespace core::viewmodels;
// using namespace features::wallets::usecases;

RegisterWalletOnHardwareViewModel::RegisterWalletOnHardwareViewModel(QObject *parent) 
: ActionViewModel(parent),
m_walletInfo(QWalletPtr(new Wallet())) {

}

Wallet* RegisterWalletOnHardwareViewModel::walletInfo() const {
    return m_walletInfo.data();
}

RegisterWalletOnHardwareViewModel::FlowEvent RegisterWalletOnHardwareViewModel::event() const {
    return m_event;
}
void RegisterWalletOnHardwareViewModel::setEvent(RegisterWalletOnHardwareViewModel::FlowEvent event) {
    if (m_event != event) {
        m_event = event;
        emit eventChanged();
    }
}

void RegisterWalletOnHardwareViewModel::onInit() {
    if (auto wallet = ctx()->serviceSetting()->walletInfoPtr()) {
        m_walletInfo = wallet;
        emit walletInfoChanged();
    }
}

} // namespace features::wallets::viewmodels
