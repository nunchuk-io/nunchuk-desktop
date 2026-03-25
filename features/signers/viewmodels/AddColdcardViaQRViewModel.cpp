#include "AddColdcardViaQRViewModel.h"
#include "core/ui/UiServices.inc"
#include "generated_qml_keys.hpp"
#include "core/common/resources/AppStrings.h"
#include "features/signers/flows/KeySetupFlow.h"
#include "features/signers/flows/KeyProceedFlow.h"

namespace features::signers::viewmodels {
using namespace features::signers::flows;
using namespace features::signers::usecases;

AddColdcardViaQRViewModel::AddColdcardViaQRViewModel(QObject *parent) 
: AddKeyBaseViewModel(parent) {
    
}

void AddColdcardViaQRViewModel::importQR(const QStringList &tags) {
    GUARD_APP_MODEL()
    ImportViaQrInput input;
    input.qr_data = tags;
    m_importViaQrUC.executeAsync(input, [this](core::usecase::Result<ImportViaQrResult> result) {
        if (result.isSuccess()) {
            auto signers = result.value().signers;
            GUARD_FLOW_MANAGER()
            auto flow = flowMng->startFlow<KeySetupFlow>();
            flowMng->resumeFlow(flow->workFlowId());
            auto flowResult = qobject_cast<KeyProceedFlow*>(flowMng->currentFlow());
            if (flowResult) {
                flowResult->proceedAfterQrImportColdcard(signers, signerName());
            }
        } else {
            emit showToast(0, result.error(), EWARNING::WarningType::ERROR_MSG);
        }
    });
}

} // namespace features::signers::viewmodels
