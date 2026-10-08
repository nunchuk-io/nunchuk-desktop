#include "AddHardwareLoadingViewModel.h"
#include "QThreadForwarder.h"
#include "core/common/resources/AppStrings.h"
#include "core/ui/UiServices.inc"
#include "features/signers/flows/KeyProceedFlow.h"
#include "features/signers/flows/KeySetupFlow.h"
#include "generated_qml_keys.hpp"

namespace features::signers::viewmodels {
using namespace features::signers::flows;
using namespace features::signers::usecases;

AddHardwareLoadingViewModel::AddHardwareLoadingViewModel(QObject *parent) : AddKeyBaseViewModel(parent) {}

void AddHardwareLoadingViewModel::setupProgressTitle() {
    switch (static_cast<SignerKeyType>(keyType())) {
    case SignerKeyType::ColdcardHW:
        settitle(Strings.STR_QML_912());
        break;
    case SignerKeyType::TrezorHW:
        settitle(Strings.STR_QML_831());
        break;
    case SignerKeyType::LedgerHW:
        settitle(Strings.STR_QML_826());
        break;
    case SignerKeyType::JadeHW:
        settitle(Strings.STR_QML_1540());
        break;
    case SignerKeyType::BitBoxHW:
        settitle(Strings.STR_QML_930());
        break;
    default:
        break;
    }
}

void AddHardwareLoadingViewModel::onInit() {
    setupProgressTitle();
    proceedAddSignerViaUSB(device());
}

void AddHardwareLoadingViewModel::proceedAddSignerViaUSB(const nunchuk::Device &device) {
    GUARD_APP_MODEL()
    CreateMasterSignerInput input;
    input.name = signerName();
    input.progress = [this](int progress) {
        QThreadForwarder::instance()->forwardInQueuedConnection([this, progress]() { setproceedLoadingStep(progress); });
        return true;
    };
    input.device = device;
    m_createMasterSignerUC.executeAsync(input, [this](core::usecase::Result<CreateMasterSignerResult> result) {
        if (result.isSuccess()) {
            auto masterSigner = result.value().signer;
            QString xfp = QString::fromStdString(masterSigner.get_id());
            setproceedLoadingStep(100);
            GUARD_FLOW_MANAGER()
            auto flow = flowMng->startFlow<KeySetupFlow>();
            flowMng->resumeFlow(flow->workFlowId());
            auto flowResult = qobject_cast<KeyProceedFlow *>(flowMng->currentFlow());
            if (flowResult) {
                flowResult->proceedAfterAddedViaUSB(xfp);
            }
        } else {
            emit showToast(0, result.error(), EWARNING::WarningType::ERROR_MSG);
        }
    });
}

void AddHardwareLoadingViewModel::slotProceedAddSigner(int progress) {
    setproceedLoadingStep(progress);
    if (progress == 101) {
        close();
    }
}

} // namespace features::signers::viewmodels
