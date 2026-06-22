#include "SetUpBufferPeriodViewModel.h"
#include "core/ui/UiServices.inc"
#include "generated_qml_keys.hpp"
#include "features/inheritance/offchain/flows/PhasedRolloutFlow.h"

namespace features::inheritance::offchain::viewmodels {
using namespace core::viewmodels;
using namespace features::inheritance::offchain::flows;
using namespace features::inheritance::usecases;

SetUpBufferPeriodViewModel::SetUpBufferPeriodViewModel(QObject *parent) : ActionViewModel(parent) {
    
}

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
        }
    });
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
