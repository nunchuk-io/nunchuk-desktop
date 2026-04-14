#include "KeyProceedFlow.h"
#include "core/ui/UiServices.inc"
#include "generated_qml_keys.hpp"
#include "core/common/resources/AppStrings.h"
#include "features/signers/viewmodels/AddKeyBaseViewModel.h"

namespace features::signers::flows {
using namespace features::signers::viewmodels;
using namespace features::signers::usecases;

KeyProceedFlow::KeyProceedFlow(FlowContext* ctx, QObject *parent) : core::flow::BaseFlow(ctx, parent) {}

void KeyProceedFlow::requestGetSigner(const QString& xfp) {
    GetSingleSignerInput input;
    input.xfp = xfp;
    input.wallet_type = walletType();
    input.address_type = addressType();
    input.account_index = accountIndex();
    m_getSingleSignerUC.executeAsync(input, [this, xfp](core::usecase::Result<GetSingleSignerResult> result) {
        if (result.isSuccess()) {
            auto single = result.value().signer;
            proceedResult(single);
        } else {
            QString message = Strings.STR_QML_2094().arg(accountIndex()).arg(xfp);
            emit showToast(-1, message, EWARNING::WarningType::ERROR_MSG);
        }
    });
}

QString KeyProceedFlow::getSignerName(const QString& signerName) {
    GUARD_APP_MODEL("")
    return m_signerNammingUC.addParameter(appModel).getNormalNameKey(signerName);
}

} // namespace features::signers::flows