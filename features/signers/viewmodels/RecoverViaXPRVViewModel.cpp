#include "RecoverViaXPRVViewModel.h"
#include "core/ui/UiServices.inc"
#include "core/utils/Utils.h"
#include "core/common/resources/AppStrings.h"
#include "features/signers/flows/KeySetupFlow.h"
#include "features/signers/flows/KeyProceedFlow.h"

namespace features::signers::viewmodels {
using features::signers::flows::KeyProceedFlow;
using features::signers::flows::KeySetupFlow;
using namespace features::signers::usecases;

RecoverViaXPRVViewModel::RecoverViaXPRVViewModel(QObject *parent) 
: AddKeyBaseViewModel(parent) {

}

bool RecoverViaXPRVViewModel::validateXPRV(const QString xprv) {
    QWarningMessage msg;
    bool isValid = qUtils::isValidXPRV(xprv, msg);
    DBG_INFO << "Validating isValid: " << isValid << " XPRV:" << xprv;
    if (!isValid) {
        emit showToast(-1, Strings.STR_QML_2092(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return isValid;
}

void RecoverViaXPRVViewModel::requestCreateSigner() {
    GUARD_APP_MODEL()
    CreateSoftwareSignerXprvInput input;
    input.xprv = xprv();
    input.isPrimaryKey = false;
    input.replace = true;
    input.keyName = m_signerNammingUC.addParameter(appModel).getInheritanceNameKey();
    m_createSoftwareSignerUC.executeAsync(input, [this](core::usecase::Result<CreateSoftwareSignerXprvResult> result) {
        if (result.isSuccess()) {
            auto master = result.value().master;
            GUARD_FLOW_MANAGER()
            auto flow = flowMng->startFlow<KeySetupFlow>();
            flowMng->resumeFlow(flow->workFlowId());
            auto flowResult = qobject_cast<KeyProceedFlow*>(flowMng->currentFlow());
            if (flowResult) {
                flowResult->proceedAfterRecoverViaXprv(QString::fromStdString(master.get_id()));
            }
        } else {
            emit showToast(-1, Strings.STR_QML_2092(), EWARNING::WarningType::ERROR_MSG);
        }
    });
}

void RecoverViaXPRVViewModel::checkSignerExist() {
     if (setupOption() == FeatureOption::ClaimOffChain) {
        requestCreateSigner();
        return;
    }
    GUARD_APP_MODEL()
    QString xfpSelected    = qUtils::GetMasterFingerprintFromMasterXprv(xprv());
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
