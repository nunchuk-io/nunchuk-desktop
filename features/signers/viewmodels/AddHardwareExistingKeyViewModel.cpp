#include "AddHardwareExistingKeyViewModel.h"
#include "core/ui/UiServices.inc"
#include "generated_qml_keys.hpp"
#include "core/common/resources/AppStrings.h"
#include "features/signers/flows/KeySetupFlow.h"
#include "features/signers/flows/KeyProceedFlow.h"

namespace features::signers::viewmodels {
using namespace features::signers::flows;

AddHardwareExistingKeyViewModel::AddHardwareExistingKeyViewModel(QObject *parent) 
: AddExistingKeyViewModel(parent) {
    setsignerType(nunchuk::SignerType::HARDWARE);
    setwalletType(nunchuk::WalletType::MULTI_SIG);
}

void AddHardwareExistingKeyViewModel::initialize() {
    switch (setupOption())
    {
    case FeatureOption::ClaimOffChain:{
        setwalletType(nunchuk::WalletType::MULTI_SIG);
        break;
    }
    
    default:
        setwalletType(nunchuk::WalletType::MINISCRIPT);
        break;
    }
}

void AddHardwareExistingKeyViewModel::initializeTextGuide() {
    switch (static_cast<SignerKeyType>(keyType()))
    {
    case SignerKeyType::ColdcardHW:        
        settitle(Strings.STR_QML_916());
        break;
    case SignerKeyType::TrezorHW:
        settitle(Strings.STR_QML_915());
       break;
    case SignerKeyType::LedgerHW:
        settitle(Strings.STR_QML_902());
       break;
    case SignerKeyType::JadeHW:
        settitle(Strings.STR_QML_1539());
       break;
    case SignerKeyType::BitBoxHW:
       settitle(Strings.STR_QML_925());
      break;
    default:
        break;
    }
}

void AddHardwareExistingKeyViewModel::onInit() {
    initialize();
    initializeTextGuide();
    AddExistingKeyViewModel::onInit();
}

void AddHardwareExistingKeyViewModel::onContinueClicked() {
    auto single = selectedSigner();
    GUARD_FLOW_MANAGER()
    auto flow = flowMng->startFlow<KeySetupFlow>();
    flowMng->resumeFlow(flow->workFlowId());
    auto flowResult = qobject_cast<KeyProceedFlow*>(flowMng->currentFlow());
    if (flowResult) {
        flowResult->proceedAfterSelectExistKey(QString::fromStdString(single.get_master_fingerprint()));
    }
}

void AddHardwareExistingKeyViewModel::onTakeMeAddNewKeyClicked() {
    GUARD_SUB_SCREEN_MANAGER()
    switch (static_cast<SignerKeyType>(keyType()))
    {
    case SignerKeyType::ColdcardHW:        
        subMng->show(qml::features::signers::qcoldcardrefreshdevices);
        break;
    case SignerKeyType::TrezorHW:
       break;
    case SignerKeyType::LedgerHW:
       break;
    case SignerKeyType::JadeHW:
       break;
    case SignerKeyType::BitBoxHW:
      break;
    default:
        break;
    }
    
}

} // namespace features::signers::viewmodels
