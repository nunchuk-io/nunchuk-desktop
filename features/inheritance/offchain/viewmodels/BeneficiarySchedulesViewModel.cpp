#include "BeneficiarySchedulesViewModel.h"
#include "core/ui/UiServices.inc"
#include "features/inheritance/offchain/flows/PhasedRolloutFlow.h"
#include "generated_qml_keys.hpp"
#include "features/inheritance/offchain/viewmodels/FallbackSettingsViewModel.h"

namespace features::inheritance::offchain::viewmodels {
using namespace core::viewmodels;
using namespace features::inheritance::offchain::flows;

BeneficiarySchedulesViewModel::BeneficiarySchedulesViewModel(QObject *parent) : ActionViewModel(parent) {
    sethasEditBtn(false);
}

void BeneficiarySchedulesViewModel::onInit() {
    GUARD_FLOW_MANAGER()
    auto flow = flowMng->startFlow<PhasedRolloutFlow>();
    if (!flow) {
        return;
    }

    QJsonObject inheritanceData = flow->inheritance();
    if (inheritanceData.isEmpty() || !inheritanceData.contains("stages")) {
        return;
    }
    initBeneficiarySchedulesData(inheritanceData);
}

void BeneficiarySchedulesViewModel::initBeneficiarySchedulesData(QJsonObject inheritanceData) {
    DBG_INFO << "inheritanceData: " << inheritanceData;
    QJsonArray beneficiariesArray = inheritanceData.value("beneficiaries").toArray();
    setrelease_method(inheritanceData.value("release_method").toString());
    initBeneficiarySchedulesData(beneficiariesArray);
    setbeneficiariesJs(beneficiariesArray);
    setbeneficiaries(helper::convertBeneficiariesData(beneficiariesArray));
    setassetAllocation(helper::convertAssetAllocationData(beneficiariesArray));
}

void BeneficiarySchedulesViewModel::initBeneficiarySchedulesData(QJsonArray beneficiaries) {
    for (const auto &beneficiary : beneficiaries) {
        QJsonObject beneficiaryObj = beneficiary.toObject();
        QJsonArray stages = beneficiaryObj.value("stages").toArray();
        if (stages.isEmpty()) {
            sethasEditBtn(true);
            return;
        }
    }
}

void BeneficiarySchedulesViewModel::onEditBeneficiaryEmailClicked(QString email) {
    QJsonObject selectedBeneficiary;
    for (const auto &beneficiary : beneficiariesJs()) {
        QJsonObject beneficiaryObj = beneficiary.toObject();
        if (beneficiaryObj.value("email").toString() == email) {
            selectedBeneficiary = beneficiaryObj;
            break;
        }
    }
    GUARD_FLOW_MANAGER()
    auto flow = flowMng->startFlow<PhasedRolloutFlow>();
    if (!flow) {
        return;
    }
    // You can pass the selected beneficiary data to the flow if needed
    flow->setcurrent_step(ConfigStep_t::None);
    flow->setbeneficiarySelected(selectedBeneficiary);
    GUARD_SUB_SCREEN_MANAGER()
    subMng->show(qml::features::inheritance::offchain::qdetailbeneficiary);
}

void BeneficiarySchedulesViewModel::onSaveClicked() {
    GUARD_FLOW_MANAGER()
    auto flow = flowMng->startFlow<PhasedRolloutFlow>();
    if (!flow) {
        return;
    }
    if (flow->fallbackOption() == FallbackOption::FallbackSecond) {
        close();
        flow->finalizeInheritanceChanges();
    } else {
        flow->setfallbackOption((int)FallbackOption::FallbackSecond);
        GUARD_SUB_SCREEN_MANAGER()
        subMng->show(qml::features::inheritance::offchain::qfallbacksettings);
    }
}

void BeneficiarySchedulesViewModel::onReleaseMethodEditClicked() {
    GUARD_SUB_SCREEN_MANAGER()
    subMng->show(qml::features::inheritance::offchain::qreleasemethod);
}

} // namespace features::inheritance::offchain::viewmodels
