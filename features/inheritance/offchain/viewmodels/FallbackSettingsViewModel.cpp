#include "FallbackSettingsViewModel.h"
#include "core/ui/UiServices.inc"
#include "core/utils/Utils.h"
#include "features/inheritance/offchain/flows/PhasedRolloutFlow.h"
#include "generated_qml_keys.hpp"
#include "core/common/resources/AppStrings.h"

namespace features::inheritance::offchain::viewmodels {
using namespace core::viewmodels;
using namespace features::inheritance::offchain::flows;

FallbackSettingsViewModel::FallbackSettingsViewModel(QObject *parent) : ActionViewModel(parent) {
    setselectedFallback("NONE");
    setrepeatEvery(1);
    setinactivityUnit("DAY");
    connect(this, &FallbackSettingsViewModel::valueDateChanged, this, &FallbackSettingsViewModel::slotValueDateChanged);
}

void FallbackSettingsViewModel::onInit() {
    GUARD_FLOW_MANAGER()
    auto flow = flowMng->startFlow<PhasedRolloutFlow>();
    if (!flow) {
        return;
    }
    QVariantMap stage = flow->editStageSelected();
    QString formattedDate = stage.value("firstWithdrawalDate").toString();
    if (formattedDate.isEmpty()) {
        QDateTime dt = QDateTime::currentDateTime();
        formattedDate = dt.toString("MM/dd/yyyy");
    }
    setvalueDate(formattedDate);
    setfallbackOption((FallbackOption)flow->fallbackOption());

    // Load current saved fallback_policy so dialog shows existing values, not defaults
    QJsonObject inheritance = flow->inheritance();
    QJsonObject fallbackPolicy = inheritance.value("fallback_policy").toObject();
    QString type = fallbackPolicy.value("type").toString();
    if (!type.isEmpty()) {
        setselectedFallback(type);
        if (qUtils::strCompare(type, "INACTIVITY")) {
            int count = fallbackPolicy.value("inactivity_interval_count").toInt();
            QString interval = fallbackPolicy.value("inactivity_interval").toString();
            if (count > 0) setrepeatEvery(count);
            if (!interval.isEmpty()) setinactivityUnit(interval);
        } else if (qUtils::strCompare(type, "DATE_BASED")) {
            double fallback_time_Ms = fallbackPolicy.value("fallback_time_millis").toDouble() / 1000;
            if (fallback_time_Ms > 0) {
                setvalueDate(helper::getDateFromTimestamp((qint64)fallback_time_Ms));
            }
        }
    }
}

void FallbackSettingsViewModel::slotValueDateChanged() {
    if(!qUtils::strCompare(selectedFallback(), "DATE_BASED")) return;
    GUARD_FLOW_MANAGER()
    auto flow = flowMng->startFlow<PhasedRolloutFlow>();
    if (!flow) {
        return;
    }
    
    QDateTime dt = QDateTime::currentDateTime();
    QString formattedTime = dt.toString("HH:mm");
    auto timestamp = qUtils::convertDateTimeToTimestamp(valueDate(), formattedTime, flow->valueTimezone())*1000; // Convert to milliseconds
    auto lastStageTimeMLs = flow->lastStageTimeMLs();
    DBG_INFO << "[FallbackSettingsViewModel::slotValueDateChanged] timestamp:" << timestamp << "lastStageTimeMLs:" << lastStageTimeMLs;
    if (timestamp < lastStageTimeMLs) {
        emit showToast(0, Strings.STR_QML_2253(), EWARNING::WarningType::ERROR_MSG);
    }    
}

void FallbackSettingsViewModel::onContinueClicked() {
    GUARD_FLOW_MANAGER()
    auto flow = flowMng->startFlow<PhasedRolloutFlow>();
    if (!flow) {
        return;
    }
    QJsonObject inheritance = flow->inheritance();
    QJsonObject fallbackPolicy = inheritance.value("fallback_policy").toObject();
    fallbackPolicy["type"] = selectedFallback();
    if(qUtils::strCompare(selectedFallback(), "DATE_BASED")) {
        QDateTime dt = QDateTime::currentDateTime();
        QString formattedTime = dt.toString("HH:mm");
        auto timestamp = qUtils::convertDateTimeToTimestamp(valueDate(), formattedTime, flow->valueTimezone())*1000; // Convert to milliseconds
        auto lastStageTimeMLs = flow->lastStageTimeMLs();
        if (timestamp < lastStageTimeMLs) {
            emit showToast(0, Strings.STR_QML_2253(), EWARNING::WarningType::ERROR_MSG);
            return;
        }
        fallbackPolicy["fallback_time_millis"] = timestamp;
    } else if (qUtils::strCompare(selectedFallback(), "INACTIVITY")) {
        fallbackPolicy["inactivity_interval"] = inactivityUnit();
        fallbackPolicy["inactivity_interval_count"] = repeatEvery();
    }
    inheritance["fallback_policy"] = fallbackPolicy;
    flow->setinheritance(inheritance);
    if (fallbackOption() == FallbackOption::FallbackSecond) {
        flow->setcurrent_step(ConfigStep_t::PassViaFallbackSettings); // Set current step to indicate we're passing through fallback settings config
        GUARD_SUB_SCREEN_MANAGER()
        subMng->show(qml::features::inheritance::offchain::qbeneficiaryschedules);
    } else {
        close();
    }
}

} // namespace features::inheritance::offchain::viewmodels
