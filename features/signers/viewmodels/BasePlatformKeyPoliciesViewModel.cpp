#include "BasePlatformKeyPoliciesViewModel.h"
#include "ViewsEnums.h"

namespace features::signers::viewmodels {

BasePlatformKeyPoliciesViewModel::BasePlatformKeyPoliciesViewModel(QObject *parent) : BaseViewModel(parent) {
    setisEntryPointAlert(false);
    setisEntryPointWallet(false);
    setisEntryPointGroup(false);
}

int BasePlatformKeyPoliciesViewModel::backToScreen() {
    if (isEntryPointAlert()) {
        return -1;
    } else if (isEntryPointWallet()) {
        return E::EVT_HOME_WALLET_INFO_REQUEST;
    } else if (isEntryPointGroup()) {
        return E::EVT_SETUP_GROUP_WALLET_REQUEST;
    }
    return -1;
}
} // namespace features::signers::viewmodels
