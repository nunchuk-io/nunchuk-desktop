#include "OnChainClaimingFlow.h"
#include "core/bridge/ExternalBridges.h"
#include "core/common/resources/AppStrings.h"
#include "features/common/viewmodels/BaseGuideViewModel.h"

namespace features::claiming::flows {
using features::common::viewmodels::BaseGuideViewModel;

OnChainClaimingFlow::OnChainClaimingFlow(FlowContext* ctx, QObject *parent) : core::flow::BaseFlow(ctx, parent) {}

void OnChainClaimingFlow::passClaimingError(int code, const QString& message) {
    m_errorCode = code;
    m_errorMessage = message;
}

void OnChainClaimingFlow::bind(QObject* vm) {
    auto realVm = qobject_cast<BaseGuideViewModel*>(vm);
    if (realVm) {
        realVm->settitle(getTitleByErrorCode(m_errorCode));
        realVm->setdescription(m_errorMessage);      
    }
}

QString OnChainClaimingFlow::getTitleByErrorCode(int code) {
    switch (code) {
    case DRACO_CODE::INHERITANCE_803:
        return Strings.STR_QML_2077();
    case DRACO_CODE::INHERITANCE_831:
        return Strings.STR_QML_2045();
    default:
        return "Claiming Error";
    }
}

} // namespace features::claiming::flows