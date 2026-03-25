#include "AddSoftwareExistingKeyViewModel.h"
#include "core/ui/UiServices.inc"
#include "core/bridge/ExternalBridges.h"
#include "generated_qml_keys.hpp"
#include "core/common/resources/AppStrings.h"
#include "features/signers/flows/KeySetupFlow.h"
#include "features/signers/flows/KeyProceedFlow.h"

namespace features::signers::viewmodels {
using namespace features::signers::flows;

AddSoftwareExistingKeyViewModel::AddSoftwareExistingKeyViewModel(QObject *parent) 
: AddExistingKeyViewModel(parent) {
    setsignerType(nunchuk::SignerType::SOFTWARE);
    setwalletType(nunchuk::WalletType::MULTI_SIG);
}

void AddSoftwareExistingKeyViewModel::onContinueClicked() {
    auto single = selectedSigner();
    GUARD_FLOW_MANAGER()
    auto flow = flowMng->startFlow<KeySetupFlow>();
    flowMng->resumeFlow(flow->workFlowId());
    auto flowResult = qobject_cast<KeyProceedFlow*>(flowMng->currentFlow());
    if (flowResult) {
        flowResult->proceedAfterSelectExistKey(QString::fromStdString(single.get_master_fingerprint()));
    }
}

void AddSoftwareExistingKeyViewModel::onTakeMeAddNewKeyClicked() {
    GUARD_SUB_SCREEN_MANAGER()
    subMng->show(qml::features::signers::qrecoveryaddsoftwarekey);
}

} // namespace features::signers::viewmodels
