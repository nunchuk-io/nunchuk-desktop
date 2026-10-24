#include "SetUpBufferPeriodViewModel.h"
#include "core/ui/UiServices.inc"
#include "features/inheritance/offchain/flows/PhasedRolloutFlow.h"
#include "generated_qml_keys.hpp"

namespace features::inheritance::offchain::viewmodels {
using namespace core::viewmodels;
using namespace features::inheritance::offchain::flows;
using namespace features::inheritance::usecases;

SetUpBufferPeriodViewModel::SetUpBufferPeriodViewModel(QObject *parent) : ActionViewModel(parent) {}

void SetUpBufferPeriodViewModel::onInit() {
    m_getListBufferPeriodsUC.executeAsync(GetListBufferPeriodsInput(), [this](core::usecase::Result<GetListBufferPeriodsResult> result) {
        if (result.isSuccess()) {
            QVariantList periodsList;
            setperiodList(result.value().periods);
            for (const auto &period : result.value().periods) {
                QJsonObject periodObj = period.toObject();
                bool is_recommended = periodObj.value("is_recommended").toBool();
                if (is_recommended) {
                    periodObj["textWidth"] = 91;
                    setbuffer_period_id(periodObj.value("id").toString()); // set default selected buffer period to the recommended one
                } else {
                    periodObj["textWidth"] = 356;
                }
                periodsList.append(periodObj.toVariantMap());
            }
            setperiods(periodsList);
            applyCurrentBufferPeriod();
        }
    });
}

void SetUpBufferPeriodViewModel::applyCurrentBufferPeriod() {
    GUARD_FLOW_MANAGER()
    auto flow = flowMng->startFlow<PhasedRolloutFlow>();
    if (!flow) {
        return;
    }
    const QJsonObject inheritance = flow->inheritance();
    const bool isShared = qUtils::strCompare(inheritance.value("release_method").toString(), "SHARED");
    const QJsonObject holder = isShared ? inheritance : flow->beneficiarySelected();
    if (holder.isEmpty()) {
        return;
    }
    const QJsonObject current = holder.value("buffer_period").toObject();
    QString currentId = current.value("id").toString();
    if (currentId.isEmpty()) {
        currentId = holder.value("buffer_period_id").toString();
    }
    const QString interval = current.value("interval").toString();
    const int count = current.value("interval_count").toInt();
    const bool hasBuffer = !currentId.isEmpty() || (count > 0 && !interval.isEmpty());
    if (!hasBuffer) {
        setbuffer_period_id(QString());
        return;
    }
    for (const auto &period : periodList()) {
        const QJsonObject periodObj = period.toObject();
        const QString id = periodObj.value("id").toString();
        if (id.isEmpty()) {
            continue;
        }
        const bool sameId = !currentId.isEmpty() && id == currentId;
        const bool sameInterval = count > 0 && qUtils::strCompare(periodObj.value("interval").toString(), interval) &&
                                  periodObj.value("interval_count").toInt() == count;
        if (sameId || sameInterval) {
            setbuffer_period_id(id);
            return;
        }
    }
}

void SetUpBufferPeriodViewModel::onContinueClicked() {
    GUARD_FLOW_MANAGER()
    auto flow = flowMng->startFlow<PhasedRolloutFlow>();
    if (!flow) {
        return;
    }
    QJsonObject buffer_period;
    for (const auto &period : periodList()) {
        QJsonObject periodObj = period.toObject();
        if (periodObj.value("id").toString() == buffer_period_id()) {
            buffer_period = periodObj;
            break;
        }
    }

    GUARD_SUB_SCREEN_MANAGER()
    QJsonObject inheritance = flow->inheritance();
    QString release_method = inheritance.value("release_method").toString();
    if (qUtils::strCompare(release_method, "SHARED")) {
        inheritance["buffer_period_id"] = buffer_period_id();
        inheritance["buffer_period"] = buffer_period;
        flow->setinheritance(inheritance);
    } else {
        QJsonObject beneficiarySelected = flow->beneficiarySelected();
        beneficiarySelected["buffer_period_id"] = buffer_period_id();
        beneficiarySelected["buffer_period"] = buffer_period;
        flow->setbeneficiarySelected(beneficiarySelected);
    }
    subMng->show(qml::features::inheritance::offchain::qsetupbufferperiodwithdrawal);
}

} // namespace features::inheritance::offchain::viewmodels
