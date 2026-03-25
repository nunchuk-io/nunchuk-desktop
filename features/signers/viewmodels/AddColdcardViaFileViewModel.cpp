#include "AddColdcardViaFileViewModel.h"
#include "core/common/resources/AppStrings.h"
#include "core/ui/UiServices.inc"
#include "features/signers/flows/KeyProceedFlow.h"
#include "features/signers/flows/KeySetupFlow.h"
#include "generated_qml_keys.hpp"

namespace features::signers::viewmodels {
using namespace features::signers::flows;
using namespace features::signers::usecases;

AddColdcardViaFileViewModel::AddColdcardViaFileViewModel(QObject *parent) : AddKeyBaseViewModel(parent) {}

void AddColdcardViaFileViewModel::importColdcardFile(const QString &filePath) {
    GUARD_APP_MODEL()
    ImportViaFileInput input;
    input.filePath = filePath;
    DBG_INFO << "importColdcardFile signerName: " << signerName();
    input.signer_type = nunchuk::SignerType::AIRGAP;
    m_importViaFileUC.executeAsync(input, [this](core::usecase::Result<ImportViaFileResult> result) {
        if (result.isSuccess()) {
            auto signers = result.value().signers;
            GUARD_FLOW_MANAGER()
            auto flow = flowMng->startFlow<KeySetupFlow>();
            flowMng->resumeFlow(flow->workFlowId());
            auto flowResult = qobject_cast<KeyProceedFlow *>(flowMng->currentFlow());
            if (flowResult) {
                flowResult->proceedAfterFileImportColdcard(signers, signerName());
            }
        } else {
            emit showToast(0, result.error(), EWARNING::WarningType::ERROR_MSG);
        }
    });
}

} // namespace features::signers::viewmodels
