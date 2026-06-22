#include "WithdrawalRuleViewModel.h"
#include "core/ui/UiServices.inc"
#include "core/common/resources/AppStrings.h"
#include "features/inheritance/offchain/flows/PhasedRolloutFlow.h"
#include "generated_qml_keys.hpp"

namespace features::inheritance::offchain::viewmodels {
using namespace core::viewmodels;
using namespace features::inheritance::offchain::flows;

WithdrawalRuleViewModel::WithdrawalRuleViewModel(QObject *parent) : ActionViewModel(parent) {

    connect(this, &WithdrawalRuleViewModel::amountPerReleaseChanged, this, [this]() {
        if (amountPerRelease() <= 0) {
            emit showToast(0, Strings.STR_QML_2232(), EWARNING::WarningType::ERROR_MSG);
        }
    });
    connect(this, &WithdrawalRuleViewModel::totalAllocationChanged, this, [this]() {
        if (totalAllocation() > 100) {
            emit showToast(0, Strings.STR_QML_2225(), EWARNING::WarningType::ERROR_MSG);
        }
    });

}

void WithdrawalRuleViewModel::onInit() {
    GUARD_FLOW_MANAGER()
    auto flow = flowMng->startFlow<PhasedRolloutFlow>();
    if (!flow) {
        return;
    }
    QVariantMap stage = flow->editStageSelected();
    setstage(QVariant::fromValue(stage));
    setamountPerRelease(stage["amountPerRelease"].toInt());
    setrepeatEvery(stage["repeatEvery"].toInt());
    setrepeatInterval(stage["repeatInterval"].toString());
    settotalAllocation(stage["percentage"].toInt());
}

void WithdrawalRuleViewModel::nextClicked() {
    // Handle next action
}

void WithdrawalRuleViewModel::prevClicked() {
    // Handle previous action
}

void WithdrawalRuleViewModel::deleteClicked() {
    GUARD_FLOW_MANAGER()
    auto flow = flowMng->startFlow<PhasedRolloutFlow>();
    if (!flow) {
        return;
    }
    QVariantMap stageToDelete = flow->editStageSelected();
    int arrayPos = stageToDelete["index"].toInt() - 1; // index is 1-based → convert to 0-based

    QJsonObject inheritance = flow->inheritance();
    QString releaseMethod = inheritance.value("release_method").toString();

    if (releaseMethod == "SHARED" || releaseMethod.isEmpty()) {
        QJsonArray stagesArray = inheritance["stages"].toArray();
        if (arrayPos >= 0 && arrayPos < stagesArray.size()) {
            stagesArray.removeAt(arrayPos);
            inheritance["stages"] = stagesArray;
            flow->setinheritance(inheritance);
        }
    } else {
        // INDIVIDUAL: stages belong to the selected beneficiary
        QJsonObject beneficiary = flow->beneficiarySelected();
        QJsonArray stagesArray = beneficiary["stages"].toArray();
        if (arrayPos >= 0 && arrayPos < stagesArray.size()) {
            stagesArray.removeAt(arrayPos);
            beneficiary["stages"] = stagesArray;
            flow->setbeneficiarySelected(beneficiary);
        }
    }

    GUARD_SUB_SCREEN_MANAGER()
    if (releaseMethod == "SHARED" || releaseMethod.isEmpty()) {
        subMng->show(qml::features::inheritance::offchain::qreleaseschedule);
    } else {
        subMng->show(qml::features::inheritance::offchain::qdetailbeneficiary);
    }
}

void WithdrawalRuleViewModel::continueClicked() {
    GUARD_FLOW_MANAGER()
    auto flow = flowMng->startFlow<PhasedRolloutFlow>();
    if (!flow) {
        return;
    }
    QVariantMap stage = flow->editStageSelected();
    stage["amountPerRelease"] = amountPerRelease();
    stage["repeatEvery"] = repeatEvery();
    stage["repeatInterval"] = repeatInterval();
    stage["percentage"] = totalAllocation();
    flow->seteditStageSelected(stage);
    GUARD_SUB_SCREEN_MANAGER()
    subMng->show(qml::features::inheritance::offchain::qaddstagefirstwithdrawaldate);
}

} // namespace features::inheritance::offchain::viewmodels
