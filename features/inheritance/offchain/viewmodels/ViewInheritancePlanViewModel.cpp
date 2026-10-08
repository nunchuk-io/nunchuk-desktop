#include "ViewInheritancePlanViewModel.h"
#include "Premiums/QInheritancePlan.h"
#include "core/ui/UiServices.inc"
#include "core/utils/Utils.h"
#include "features/inheritance/offchain/flows/PhasedRolloutFlow.h"
#include "features/inheritance/offchain/flows/SetupOffChainTimelockFlow.h"
#include "features/inheritance/offchain/viewmodels/FallbackSettingsViewModel.h"
#include "generated_qml_keys.hpp"

namespace features::inheritance::offchain::viewmodels {
using namespace core::viewmodels;
using namespace features::inheritance::usecases;
using namespace features::inheritance::offchain::flows;
const int DURATION = 2; // years
ViewInheritancePlanViewModel::ViewInheritancePlanViewModel(QObject *parent) : OffChainTimelockViewModel(parent) {}

void ViewInheritancePlanViewModel::onInit() {
    GUARD_SERVICE_WALLET_DASHBOARD()
    GetInheritancePlanInput input;
    input.wallet_id = dashboard->wallet_id();
    input.group_id = dashboard->groupId();
    m_getInheritancePlanUC.executeAsync(input, [this](core::usecase::Result<GetInheritancePlanResult> result) {
        if (result.isSuccess()) {
            auto inheritance = result.value().inheritance;
            DBG_INFO << "inheritance: " << inheritance;
            GUARD_FLOW_MANAGER()
            auto flow = flowMng->startFlow<PhasedRolloutFlow>();
            if (!flow) {
                return;
            }
            connect(flow, &PhasedRolloutFlow::inheritanceChanged, this, &ViewInheritancePlanViewModel::onPhasedRolloutRefresh, Qt::UniqueConnection);
            flow->setinheritance(inheritance);
            initPhasedRolloutData(inheritance);
        }
    });
}

void ViewInheritancePlanViewModel::initPhasedRolloutData(QJsonObject inheritance) {
    QJsonObject timelock;
    double activation_time_milis = inheritance.value("activation_time_milis").toDouble();
    if (activation_time_milis > 0) {
        timelock["value"] = inheritance.value("activation_time_milis");
        timelock["timezone"] = inheritance.value("timezone");
    }
    clearTimeLock(timelock);
    setdistribution_method(inheritance.value("distribution_method").toString());
    setbeneficiary_mode(inheritance.value("beneficiary_mode").toString());
    setbuffer_apply_on(inheritance.value("buffer_apply_on").toString());
    setrelease_method(inheritance.value("release_method").toString());
    QJsonObject bufferPeriod = inheritance.value("buffer_period").toObject();
    if (!bufferPeriod.contains("id"))           bufferPeriod["id"]           = QString("");
    if (!bufferPeriod.contains("display_name")) bufferPeriod["display_name"] = QString("");
    setbuffer_period(QVariant::fromValue(bufferPeriod));
    QJsonArray stagesArray = inheritance["stages"].toArray();
    setstagesData(helper::convertStagesData(stagesArray));
    setfirstWithdrawalDate(helper::getFormatFirstWithdrawalDate(stagesArray));
    settimelineStagesData(helper::convertTimelineStagesData(stagesData()));
    QJsonArray beneficiariesArray = inheritance["beneficiaries"].toArray();
    setbeneficiariesData(helper::convertBeneficiariesData(beneficiariesArray));
    setassetAllocation(helper::convertAssetAllocationData(beneficiariesArray));
    auto oldFallbackPolicy = inheritance.value("fallback_policy").toObject();
    int inactivity_interval_count = oldFallbackPolicy.value("inactivity_interval_count").toInt();
    setfallback_interval_count(inactivity_interval_count);
    QString fallback_interval = oldFallbackPolicy.value("inactivity_interval").toString();
    setfallback_interval(helper::getIntervalDisplayString(fallback_interval, inactivity_interval_count));
    auto fallback_time_Ms = oldFallbackPolicy.value("fallback_time_millis").toDouble() / 1000;
    QString fallback_dateTime = helper::getDateFromTimestamp(fallback_time_Ms);
    setfallback_dateTime(fallback_dateTime);
    QString fallback_type = oldFallbackPolicy.value("type").toString();
    setfallback_type(fallback_type);
    setfallback_policy(QVariant::fromValue(oldFallbackPolicy));
}

void ViewInheritancePlanViewModel::timeLockEditClicked() {
    GUARD_SUB_SCREEN_MANAGER()
    GUARD_FLOW_MANAGER()
    auto flow = flowMng->startFlow<SetupOffChainTimelockFlow>();
    if (isCurrentTime()) {
        flow->passInheritanceInfo(qUtils::increaseDateByYears(valueDate(), DURATION), valueTimezone());
    } else {
        flow->passInheritanceInfo(valueDate(), valueTimezone());
    }
    connect(
        flow, &SetupOffChainTimelockFlow::callbackTimelockChanged, this,
        [this](const QString &date, const QString &timezone) {
            setvalueDate(date);
            setvalueTimezone(timezone);
        },
        Qt::UniqueConnection);
    subMng->show(qml::screens::onlinemode::setupwallets::timelocks::qsetupoffchaintimelockwallet);
}

void ViewInheritancePlanViewModel::onAssetAllocationEditClicked() {
    GUARD_SUB_SCREEN_MANAGER()
    subMng->show(qml::features::inheritance::offchain::qassetallocation);
}

void ViewInheritancePlanViewModel::onReleaseMethodEditClicked() {
    GUARD_SUB_SCREEN_MANAGER()
    subMng->show(qml::features::inheritance::offchain::qreleasemethod);
}

void ViewInheritancePlanViewModel::onReleaseScheduleEditClicked() {
    GUARD_SUB_SCREEN_MANAGER()
    subMng->show(qml::features::inheritance::offchain::qreleaseschedule);
}

void ViewInheritancePlanViewModel::onTimezoneEditClicked() {
    GUARD_SUB_SCREEN_MANAGER()
    GUARD_FLOW_MANAGER()
    auto flow = flowMng->startFlow<SetupOffChainTimelockFlow>();
    if (isCurrentTime()) {
        flow->passInheritanceInfo(qUtils::increaseDateByYears(valueDate(), DURATION), valueTimezone());
    } else {
        flow->passInheritanceInfo(valueDate(), valueTimezone());
    }

    connect(
        flow, &SetupOffChainTimelockFlow::callbackTimelockChanged, this,
        [this](const QString &date, const QString &timezone) {
            setvalueDate(date);
            setvalueTimezone(timezone);
        },
        Qt::UniqueConnection);
    subMng->show(qml::features::inheritance::offchain::qchangetimezone);
}

void ViewInheritancePlanViewModel::onFallbackSettingsEditClicked() {
    GUARD_FLOW_MANAGER()
    auto flow = flowMng->startFlow<PhasedRolloutFlow>();
    if (!flow) {
        return;
    }
    flow->setfallbackOption((int)FallbackOption::FallbackFirst);
    flow->setvalueTimezone(valueTimezone());
    GUARD_SUB_SCREEN_MANAGER()
    subMng->show(qml::features::inheritance::offchain::qfallbacksettings);
}

void ViewInheritancePlanViewModel::onBeneficiarySchedulesEditClicked() {
    GUARD_FLOW_MANAGER()
    auto flow = flowMng->startFlow<PhasedRolloutFlow>();
    if (!flow) {
        return;
    }
    flow->setvalueTimezone(valueTimezone());
    GUARD_SUB_SCREEN_MANAGER()
    subMng->show(qml::features::inheritance::offchain::qbeneficiaryschedules);
}

void ViewInheritancePlanViewModel::onPhasedRolloutRefresh() {
    GUARD_FLOW_MANAGER()
    auto flow = flowMng->startFlow<PhasedRolloutFlow>();
    if (!flow) {
        return;
    }
    QJsonObject inheritance = flow->inheritance();
    double activation_time_milis = inheritance.value("activation_time_milis").toDouble();
    flow->setactivation_time_milis(activation_time_milis);
    initPhasedRolloutData(inheritance);
    compareAndSet("stages");
    compareAndSet("beneficiaries");
    compareAndSet("buffer_period");
    compareAndSet("buffer_apply_on");
    compareAndSet("release_method");
    compareAndSet("fallback_policy");
    compareAndSet("timezone");
    compareAndSet("distribution_method");
    compareAndSet("beneficiary_mode");
    compareAndSet("buffer_period_id");    
}

void ViewInheritancePlanViewModel::compareAndSet(const QString &key) {
    GUARD_FLOW_MANAGER()
    auto flow = flowMng->startFlow<PhasedRolloutFlow>();
    if (!flow) {
        return;
    }
    QJsonObject inheritance = flow->inheritance();
    GUARD_SERVICE_WALLET_INHERITANCE_PLAN()
    QJsonObject original = ihPlan->planInfoCurrent();
    QJsonObject change = ihPlan->planInfoJs();
    if (inheritance.contains(key)) {
        QJsonValue _new = inheritance.value(key);
        QJsonValue _old = original.value(key);
        if (_new != _old) {
            change[key] = _new;
            setisDataChanged(true);
            ihPlan->setPlanInfo(change);
        }
    }
}

void ViewInheritancePlanViewModel::discardChanges() {
    GUARD_FLOW_MANAGER()
    auto flow = flowMng->startFlow<PhasedRolloutFlow>();
    if (!flow) {
        return;
    }
    flow->discardChanges();
}

void ViewInheritancePlanViewModel::finalizeChanges() {
    GUARD_FLOW_MANAGER()
    auto flow = flowMng->startFlow<PhasedRolloutFlow>();
    if (!flow) {
        return;
    }
    flow->finalizeInheritanceChanges();
}

void ViewInheritancePlanViewModel::securityQuestionVerified() {
    GUARD_FLOW_MANAGER()
    auto flow = flowMng->startFlow<PhasedRolloutFlow>();
    if (!flow) {
        return;
    }
    flow->securityQuestionVerified();
}

void ViewInheritancePlanViewModel::cancelInheritancePlan() {
    GUARD_FLOW_MANAGER()
    auto flow = flowMng->startFlow<PhasedRolloutFlow>();
    if (!flow) {
        return;
    }
    flow->cancelInheritancePlan();
}

} // namespace features::inheritance::offchain::viewmodels
