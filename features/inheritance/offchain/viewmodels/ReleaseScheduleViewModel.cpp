#include "ReleaseScheduleViewModel.h"
#include "core/ui/UiServices.inc"
#include "features/inheritance/offchain/flows/PhasedRolloutFlow.h"
#include "generated_qml_keys.hpp"
#include <QDateTime>
#include <QJsonArray>
#include <QJsonObject>
#include <QVariantList>

namespace features::inheritance::offchain::viewmodels {
using namespace core::viewmodels;
using namespace features::inheritance::offchain::flows;

ReleaseScheduleViewModel::ReleaseScheduleViewModel(QObject *parent) : ActionViewModel(parent) {}

void ReleaseScheduleViewModel::onInit() {
    GUARD_FLOW_MANAGER()
    auto flow = flowMng->startFlow<PhasedRolloutFlow>();
    if (!flow) {
        return;
    }

    QJsonObject inheritanceData = flow->inheritance();
    if (inheritanceData.isEmpty() || !inheritanceData.contains("stages")) {
        return;
    }
    initStagesData(inheritanceData);
}

void ReleaseScheduleViewModel::initStagesData(QJsonObject inheritanceData) {
    setbeneficiary_mode(inheritanceData.value("beneficiary_mode").toString());
    setrelease_method(inheritanceData.value("release_method").toString());
    QJsonObject bufferPeriod = inheritanceData.value("buffer_period").toObject();
    if (!bufferPeriod.contains("id"))           bufferPeriod["id"]           = QString("");
    if (!bufferPeriod.contains("display_name")) bufferPeriod["display_name"] = QString("");
    setbuffer_period(QVariant::fromValue(bufferPeriod));
    setbuffer_apply_on(inheritanceData.value("buffer_apply_on").toString());
    QJsonArray stagesArray = inheritanceData["stages"].toArray();
    if (stagesArray.isEmpty()) {
        addStage();        
    } else {
        setstagesData(helper::convertStagesData(stagesArray));
        settimelineStagesData(helper::convertTimelineStagesData(stagesData()));
        updateStageTotalPercentage();
    }
}

void ReleaseScheduleViewModel::updateStageTotalPercentage() {
    int totalPercentage = 0;
    for (const auto &stageValue : stagesData()) {
        QVariantMap stage = stageValue.toMap();
        totalPercentage += stage["percentage"].toInt();
    }
    settotalPercentage(totalPercentage);
}

void ReleaseScheduleViewModel::addStage() {
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
    settimelineStagesData(helper::convertTimelineStagesData(stagesData()));
    updateStageTotalPercentage();
}

void ReleaseScheduleViewModel::onExpandClicked(int index) {
    if (index < 0 || index >= stagesData().size()) {
        return;
    }

    QVariantMap stage = stagesData()[index].toMap();
    bool isExpanded = stage["isExpanded"].toBool();
    stage["isExpanded"] = !isExpanded;
    stagesData()[index] = stage;
    setstagesData(stagesData());
}

void ReleaseScheduleViewModel::onEditStageClicked(int index) {
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

void ReleaseScheduleViewModel::onAddStageClicked() {
    GUARD_FLOW_MANAGER()
    auto flow = flowMng->startFlow<PhasedRolloutFlow>();
    if (!flow) {
        return;
    }
    flow->setcurrent_step(ConfigStep_t::None);
    addStage();
    QJsonObject inheritanceData = flow->inheritance();
    QJsonArray stagesArray = inheritanceData["stages"].toArray();
    QJsonArray updatedStagesArray = helper::convertStagesToJsonArray(stagesData());
    inheritanceData["stages"] = updatedStagesArray;
    flow->setinheritance(inheritanceData);
    GUARD_SUB_SCREEN_MANAGER()
    subMng->show(qml::features::inheritance::offchain::qaddstagewithdrawalrule);
}

void ReleaseScheduleViewModel::onSaveClicked() {
    GUARD_FLOW_MANAGER()
    auto flow = flowMng->startFlow<PhasedRolloutFlow>();
    if (!flow) {
        return;
    }
    QJsonObject inheritanceData = flow->inheritance();
    QJsonArray stagesArray = inheritanceData["stages"].toArray();
    QJsonArray updatedStagesArray = helper::convertStagesToJsonArray(stagesData());
    inheritanceData["stages"] = updatedStagesArray;
    flow->setinheritance(inheritanceData);
    GUARD_SUB_SCREEN_MANAGER()
    if (flow->current_step() == ConfigStep_t::PassViaStageConfig || flow->current_step() == ConfigStep_t::None) {
        subMng->show(qml::features::inheritance::offchain::qsetupbufferperiod);
    } else if (flow->current_step() == ConfigStep_t::PassViaBufferPeriod) {
        close();
        flow->finalizeInheritanceChanges();
    }
}

void ReleaseScheduleViewModel::onBufferPeriodonClicked() {
    GUARD_FLOW_MANAGER()
    auto flow = flowMng->startFlow<PhasedRolloutFlow>();
    if (!flow) {
        return;
    }
    GUARD_SUB_SCREEN_MANAGER()
    subMng->show(qml::features::inheritance::offchain::qsetupbufferperiod);
}

} // namespace features::inheritance::offchain::viewmodels
