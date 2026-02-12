#include "SetupOffChainTimelockFlow.h"
#include "core/ui/UiServices.inc"
#include "Premiums/QWalletServicesTag.h"
#include "generated_qml_keys.hpp"

#include "features/inheritance/offchain/viewmodels/SetupAnOffChainTimelockViewModel.h"

namespace features::inheritance::offchain::flows {
using features::inheritance::offchain::viewmodels::SetupAnOffChainTimelockViewModel;

SetupOffChainTimelockFlow::SetupOffChainTimelockFlow(FlowContext* ctx, QObject *parent) : core::flow::BaseFlow(ctx, parent) {}

void SetupOffChainTimelockFlow::bind(QObject* vm) {
    auto realVm = qobject_cast<SetupAnOffChainTimelockViewModel*>(vm);
    if (realVm) {
        realVm->setvalueDate(m_date);
        realVm->setvalueTimezone(m_timezone);
        connect(realVm, &SetupAnOffChainTimelockViewModel::valueDateChanged, this, [realVm, this] {
            m_date = realVm->valueDate();
            slotTimelockChanged();
        });
        connect(realVm, &SetupAnOffChainTimelockViewModel::valueTimezoneChanged, this, [realVm, this] {
            m_timezone = realVm->valueTimezone();
            slotTimelockChanged();
        });
    }
}

void SetupOffChainTimelockFlow::passInheritanceInfo(const QString& date, const QString& timezone) {
    m_date = date;
    m_timezone = timezone;
}

void SetupOffChainTimelockFlow::slotTimelockChanged() {
    emit callbackTimelockChanged(m_date, m_timezone);
}
} // namespace features::inheritance::offchain::flows
