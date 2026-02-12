#include "RecoverAnExistingSeedViewModel.h"
#include "claiming/flows/OnChainClaimingFlow.h"
#include "generated_qml_keys.hpp"
#include "localization/STR_CPP.h"
#include "core/ui/UiServices.inc"
#include "Premiums/QWalletServicesTag.h"
#include "qUtils.h"

namespace features::signers::viewmodels {
using namespace core::viewmodels;
using namespace features::signers::usecases;
using features::claiming::flows::OnChainClaimingFlow;

RecoverAnExistingSeedViewModel::RecoverAnExistingSeedViewModel(QObject *parent) 
: ActionViewModel(parent) {

}

void RecoverAnExistingSeedViewModel::recoverViaExistingSeed(QVariant msg) {
    QMap<QString, QVariant> maps = msg.toMap();
    QString mnemonicstr = maps.value("mnemonicstr").toString();
    CreateSoftwareInput input;
    input.mnemonic = mnemonicstr;
    input.passphrase = "";
    input.isPrimaryKey = false;
    input.replace = true;
    input.keyName = m_signerNammingUC.addParameter(ctx()->appModel()).getInheritanceNameKey();
    m_createSoftwareSignerUC.executeAsync(input, [this, mnemonicstr](core::usecase::Result<CreateSoftwareResult> result) {
        if (result.isSuccess()) {
            GUARD_SERVICE_TAG();
            serviceTag->requestDownloadWalletViaSeedPhrase(mnemonicstr);
        } else {
            emit showToast(-1, STR_CPP_081, EWARNING::WarningType::ERROR_MSG);
        }
    });
}


bool RecoverAnExistingSeedViewModel::checkExistingKey(QVariant msg) {
    QMap<QString, QVariant> maps = msg.toMap();
    QString mnemonicstr = maps.value("mnemonicstr").toString();
    QString xfpSelected    = qUtils::GetMasterFingerprint(mnemonicstr, "");
    auto masterList = ctx()->appModel()->masterSignerListPtr();
    auto remoteList = ctx()->appModel()->remoteSignerListPtr();
    if (masterList->containsFingerPrint(xfpSelected)) {
        auto oldKey = masterList->getMasterSignerByXfp(xfpSelected);
        if (oldKey->originMasterSigner().get_type() == nunchuk::SignerType::SOFTWARE) {
            emit notifySignerExist(true, xfpSelected);
        } else {
            emit notifySignerExist(false, xfpSelected);
        }
        return false;
    } 
    else if (remoteList->containsFingerPrint(xfpSelected)) {
        auto oldKey = remoteList->getSingleSignerByFingerPrint(xfpSelected);
        if (oldKey->originSingleSigner().get_type() == nunchuk::SignerType::SOFTWARE) {
            emit notifySignerExist(true, xfpSelected);
        } else {
            emit notifySignerExist(false, xfpSelected);
        }
        return false;
    }
    else {
        recoverViaExistingSeed(msg);
        return true;
    }
}

} // namespace features::signers::viewmodels
