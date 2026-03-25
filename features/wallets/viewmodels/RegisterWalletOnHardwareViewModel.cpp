#include "RegisterWalletOnHardwareViewModel.h"
#include "core/bridge/ExternalBridges.h"
#include "core/restapi/RestApi.h"
#include "core/ui/UiServices.inc"
#include "core/utils/Utils.h"

namespace features::wallets::viewmodels {
using namespace core::viewmodels;
// using namespace features::wallets::usecases;

RegisterWalletOnHardwareViewModel::RegisterWalletOnHardwareViewModel(QObject *parent) 
: ActionViewModel(parent){

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
    auto walletPtr = bridge::convertWallet(nunWallet());
    if (walletPtr) {
        setwalletInfo(walletPtr);
    }
}

} // namespace features::wallets::viewmodels
