#include "RecoverViaSeedViewModel.h"
#include "core/ui/UiServices.inc"
#include "core/common/resources/AppStrings.h"
#include "features/signers/flows/KeySetupFlow.h"
#include "features/signers/flows/KeyProceedFlow.h"

namespace features::signers::viewmodels {
using features::signers::flows::KeyProceedFlow;
using features::signers::flows::KeySetupFlow;
using namespace features::signers::usecases;

RecoverViaSeedViewModel::RecoverViaSeedViewModel(QObject *parent) 
: AddKeyBaseViewModel(parent) {

}

void RecoverViaSeedViewModel::onInit() {
    AddKeyBaseViewModel::onInit();
    GUARD_APP_MODEL()
    SuggestMnemonicsInput input;
    m_suggestMnemonicsUC.executeAsync(input, [this](core::usecase::Result<SuggestMnemonicsResult> result) {
        if (result.isSuccess()) {
            setsuggestMnemonics(result.value().suggests);
        } 
    });
}

void RecoverViaSeedViewModel::requestCreateSigner() {
    GUARD_APP_MODEL()
    CreateSoftwareInput input;
    input.mnemonic = mnemonic();
    input.passphrase = "";
    input.isPrimaryKey = false;
    input.replace = true;
    input.keyName = m_signerNammingUC.addParameter(appModel).getInheritanceNameKey();
    m_createSoftwareSignerUC.executeAsync(input, [this](core::usecase::Result<CreateSoftwareResult> result) {
        if (result.isSuccess()) {
            auto master = result.value().master;
            GUARD_FLOW_MANAGER()
            auto flow = flowMng->startFlow<KeySetupFlow>();
            flowMng->resumeFlow(flow->workFlowId());
            auto flowResult = qobject_cast<KeyProceedFlow*>(flowMng->currentFlow());
            if (flowResult) {
                flowResult->proceedAfterRecoverViaSeed(QString::fromStdString(master.get_id()));
            }
        } else {
            emit showToast(-1, Strings.STR_QML_2089(), EWARNING::WarningType::ERROR_MSG);
        }
    });
}

void RecoverViaSeedViewModel::checkSignerExist() {
     if (setupOption() == FeatureOption::ClaimOffChain) {
        requestCreateSigner();
        return;
    }
    GUARD_APP_MODEL()
    QString xfpSelected    = qUtils::GetMasterFingerprint(mnemonic(), "");
    auto masterList = appModel->masterSignerListPtr();
    auto remoteList = appModel->remoteSignerListPtr();
    if (masterList->containsFingerPrint(xfpSelected)) {
        auto oldKey = masterList->getMasterSignerByXfp(xfpSelected);
        if (oldKey->originMasterSigner().get_type() == nunchuk::SignerType::SOFTWARE) {
            emit notifySignerExist(true, xfpSelected);
        } else {
            emit notifySignerExist(false, xfpSelected);
        }
    } 
    else if (remoteList->containsFingerPrint(xfpSelected)) {
        auto oldKey = remoteList->getSingleSignerByFingerPrint(xfpSelected);
        if (oldKey->originSingleSigner().get_type() == nunchuk::SignerType::SOFTWARE) {
            emit notifySignerExist(true, xfpSelected);
        } else {
            emit notifySignerExist(false, xfpSelected);
        }
    }
    else {
        requestCreateSigner();
    }
}

} // namespace features::signers::viewmodels
