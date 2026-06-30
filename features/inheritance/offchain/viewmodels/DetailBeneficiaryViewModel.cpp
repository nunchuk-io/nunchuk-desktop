#include "DetailBeneficiaryViewModel.h"
#include "core/ui/UiServices.inc"
#include "features/inheritance/offchain/flows/PhasedRolloutFlow.h"
#include "generated_qml_keys.hpp"

namespace features::inheritance::offchain::viewmodels {
using namespace core::viewmodels;
using namespace features::inheritance::offchain::flows;

DetailBeneficiaryViewModel::DetailBeneficiaryViewModel(QObject *parent) : ActionViewModel(parent) {}

void DetailBeneficiaryViewModel::onInit() {
    GUARD_FLOW_MANAGER()
    auto flow = flowMng->startFlow<PhasedRolloutFlow>();
    if (!flow) {
        return;
    }
    QJsonObject inheritanceData = flow->inheritance();
    setbeneficiary_mode(inheritanceData.value("beneficiary_mode").toString());
    setrelease_method(inheritanceData.value("release_method").toString());
    initBeneficiaryData(flow->beneficiarySelected());
    DBG_INFO << "beneficiarySelected: " << flow->beneficiarySelected();
    if (!flow->editStageSelected().isEmpty()) {
        applyStageData(flow->editStageSelected());
        flow->seteditStageSelected(QVariantMap{});
    } else if (stagesData().isEmpty()) {
        // Only auto-add initial stage when the beneficiary has no stages yet
        addStage();
        saveBeneficiaryData();
    }
}

void DetailBeneficiaryViewModel::initBeneficiaryData(const QJsonObject &beneficiarySelected) {
    QString email = beneficiarySelected.value("email").toString();
    setbenefiEmail(email);
    QJsonArray stagesArray = beneficiarySelected.value("stages").toArray();
    DBG_INFO << "initBeneficiaryData: stagesArray: " << stagesArray;
    setstagesData(helper::convertStagesData(stagesArray));
    settimelineStagesData(helper::convertTimelineStagesData(stagesData()));
    updateStageTotalPercentage();
}

void DetailBeneficiaryViewModel::updateStageTotalPercentage() {
    int totalPercentage = 0;
    for (const auto &stageValue : stagesData()) {
        QVariantMap stage = stageValue.toMap();
        totalPercentage += stage["percentage"].toInt();
    }
    settotalPercentage(totalPercentage);
}

void DetailBeneficiaryViewModel::applyStageData(const QVariantMap &stageData) {
    int index = stageData["index"].toInt() - 1; // stageData["index"] is 1-based; convert to 0-based
    if (index < 0 || index >= stagesData().size()) {
        return;
    }
    auto stagesDataList = stagesData();
    stagesDataList[index] = stageData;
    setstagesData(stagesDataList);
    settimelineStagesData(helper::convertTimelineStagesData(stagesDataList));
    updateStageTotalPercentage();
}

void DetailBeneficiaryViewModel::saveBeneficiaryData() {
    GUARD_FLOW_MANAGER()
    auto flow = flowMng->startFlow<PhasedRolloutFlow>();
    if (!flow) {
        return;
    }
    QJsonObject beneficiarySelected = flow->beneficiarySelected();
    QJsonArray stagesArray = beneficiarySelected.value("stages").toArray();
    QJsonArray updatedStagesArray = helper::convertStagesToJsonArray(stagesData());
    beneficiarySelected["stages"] = updatedStagesArray;
    flow->setbeneficiarySelected(beneficiarySelected);
    // You can also trigger any save action in the flow if needed
}

void DetailBeneficiaryViewModel::addStage() {
    GUARD_FLOW_MANAGER()
    auto flow = flowMng->startFlow<PhasedRolloutFlow>();
    if (!flow) {
        return;
    }
    int index = stagesData().size() + 1; // 1-based index
    QVariantMap newStage = helper::addStage(index);
    flow->seteditStageSelected(newStage);
    auto stagesDataList = stagesData();
    stagesDataList.append(newStage);
    setstagesData(stagesDataList);
    settimelineStagesData(helper::convertTimelineStagesData(stagesDataList));
    updateStageTotalPercentage();
}

void DetailBeneficiaryViewModel::onExpandClicked(int index) {
    if (index < 0 || index >= stagesData().size()) {
        return;
    }

    QVariantMap stage = stagesData()[index].toMap();
    bool isExpanded = stage["isExpanded"].toBool();
    stage["isExpanded"] = !isExpanded;
    stagesData()[index] = stage;
    setstagesData(stagesData());
}

void DetailBeneficiaryViewModel::onEditStageClicked(int index) {
    if (index < 0 || index >= stagesData().size()) {
        return;
    }

    QVariantMap stage = stagesData()[index].toMap();
    GUARD_FLOW_MANAGER()
    auto flow = flowMng->startFlow<PhasedRolloutFlow>();
    if (!flow) {
        return;
    }
    flow->setcurrent_step(ConfigStep_t::None);
    stage["index"] = index + 1;
    flow->seteditStageSelected(stage);
    GUARD_SUB_SCREEN_MANAGER()
    subMng->show(qml::features::inheritance::offchain::qaddstagewithdrawalrule);
}

void DetailBeneficiaryViewModel::onAddStageClicked() {
    GUARD_FLOW_MANAGER()
    auto flow = flowMng->startFlow<PhasedRolloutFlow>();
    if (!flow) {
        return;
    }
    flow->setcurrent_step(ConfigStep_t::None);
    addStage();
    GUARD_SUB_SCREEN_MANAGER()
    subMng->show(qml::features::inheritance::offchain::qaddstagewithdrawalrule);
}

void DetailBeneficiaryViewModel::onContinueClicked() {
    GUARD_FLOW_MANAGER()
    auto flow = flowMng->startFlow<PhasedRolloutFlow>();
    if (!flow) {
        return;
    }
    auto beneficiarySelected = flow->beneficiarySelected();
    auto inheritanceData = flow->inheritance();
    QJsonArray beneficiariesArray = inheritanceData.value("beneficiaries").toArray();
    for (int i = 0; i < beneficiariesArray.size(); ++i) {
        QJsonObject beneficiaryObj = beneficiariesArray[i].toObject();
        if (beneficiaryObj.value("email").toString() == beneficiarySelected.value("email").toString()) {
            beneficiariesArray[i] = beneficiarySelected;
            break;
        }
    }
    inheritanceData["beneficiaries"] = beneficiariesArray;
    flow->setinheritance(inheritanceData);

    GUARD_SUB_SCREEN_MANAGER()
    if (flow->current_step() == ConfigStep_t::PassViaStageConfig || flow->current_step() == ConfigStep_t::None) {
        subMng->show(qml::features::inheritance::offchain::qsetupbufferperiod);
    } else if (flow->current_step() == ConfigStep_t::PassViaBufferPeriod) {
        subMng->show(qml::features::inheritance::offchain::qbeneficiaryschedules);
    }
}

} // namespace features::inheritance::offchain::viewmodels
