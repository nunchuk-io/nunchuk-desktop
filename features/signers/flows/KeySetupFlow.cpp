#include "KeySetupFlow.h"
#include "core/ui/UiServices.inc"
#include "core/ui/GuardMacros.h"
#include "generated_qml_keys.hpp"
#include "core/common/resources/AppStrings.h"
#include "features/signers/viewmodels/AddKeyBaseViewModel.h"
#include "features/signers/flows/KeyProceedFlow.h"

namespace features::signers::flows {
using namespace features::signers::viewmodels;
using namespace features::signers::usecases;

static const QHash<QString, FeatureOption> s_flowIdMap = {
    { "OffChainClaimingFlow", FeatureOption::ClaimOffChain },
};

KeySetupFlow::KeySetupFlow(FlowContext* ctx, QObject *parent) : core::flow::BaseFlow(ctx, parent) {}

void KeySetupFlow::bind(QObject* vm) {
    auto realVm = qobject_cast<AddKeyBaseViewModel*>(vm);
    setsetupOption(s_flowIdMap.value(workFlowId(), FeatureOption::None));
    if (realVm) {
        DBG_INFO << "KeySetupFlow::bind setupOption:" << setupOption()
                 << ", keyType:" << keyType() << ", keyName:" << keyName()
                 << ", keyTag:" << keyTag() << ", keyXfp:" << keyXfp();
        realVm->setsetupOption(static_cast<FeatureOption>(setupOption()));
        realVm->setselectedXfp(keyXfp());
        realVm->setsignerName(keyName());
        realVm->sethardwareTag(keyTag());
        realVm->setkeyType(static_cast<SignerKeyType>(keyType()));
        realVm->setdevice(device());
        connect(this, &KeySetupFlow::signalProceedAddSigner,
                realVm, &AddKeyBaseViewModel::slotProceedAddSigner);
    }
}

} // namespace features::signers::flows