#include "BaseClaimingViewModel.h"
#include "core/bridge/ExternalBridges.h"
#include "core/ui/UiServices.inc"
#include "generated_qml_keys.hpp"
#include "features/claiming/flows/OnChainClaimingFlow.h"

namespace features::claiming::viewmodels {
using namespace core::viewmodels;
using features::claiming::flows::OnChainClaimingFlow;

BaseClaimingViewModel::BaseClaimingViewModel(QObject *parent) : ActionViewModel(parent) {}

void BaseClaimingViewModel::handleErrorCode(int error_code, const QString &error_msg) {
    GUARD_SERVICE_TAG()
    GUARD_SUB_SCREEN_MANAGER()
    GUARD_FLOW_MANAGER()
    switch (error_code) {
    case DRACO_CODE::INHERITANCE_801:
        // show popup: Invalid phrase. Please try again
        showToast(0, STR_CPP_115, EWARNING::WarningType::ERROR_MSG);
        break;
    case DRACO_CODE::INHERITANCE_802:
        // show popup: It looks like the original subscription has expired for more than 3 years.
        //             To claim the inheritance, please reactivate the plan first by visiting nunchuk.io/claim.
        emit serviceTag->isExpiredAlert();
        break;
    case DRACO_CODE::INHERITANCE_831:
    case DRACO_CODE::INHERITANCE_803: {
        // show popup: Your inheritance plan has not been activated yet. Please try again later.
        flowMng->stopFlow<OnChainClaimingFlow>();
        auto flow = flowMng->startFlow<OnChainClaimingFlow>();
        flow->passClaimingError(error_code, error_msg);
        subMng->show(qml::screens::onlinemode::claiming::claiminheritanceplanresult);
    } break;
    case DRACO_CODE::INHERITANCE_829:
        // show popup: Please come back later
        emit serviceTag->comeBackLaterAlert(error_msg);
        break;
    case DRACO_CODE::INHERITANCE_830:
        // show popup: Security deposit required
        emit serviceTag->securityDepositRequiredAlert(error_msg);
        break;
    default:
        showToast(error_code, "Invalid Magic Phrase", EWARNING::WarningType::EXCEPTION_MSG);
        break;
    }
}

} // namespace features::claiming::viewmodels