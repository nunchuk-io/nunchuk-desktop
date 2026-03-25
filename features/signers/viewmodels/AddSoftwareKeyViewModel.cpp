#include "AddSoftwareKeyViewModel.h"
#include "core/ui/UiServices.inc"
#include "generated_qml_keys.hpp"
#include "core/common/resources/AppStrings.h"
#include "features/signers/flows/KeySetupFlow.h"

namespace features::signers::viewmodels {
using namespace features::signers::flows;

static const QJsonArray viaOptions = {
    QJsonObject{{ "id", (int)RecoveryOption::ViaSeed }, { "label", Strings.STR_QML_2087() }},
    QJsonObject{{ "id", (int)RecoveryOption::ViaXPRV }, { "label", Strings.STR_QML_2088() }},
};

AddSoftwareKeyViewModel::AddSoftwareKeyViewModel(QObject *parent) 
: AddKeyBaseViewModel(parent) {
    setoptions(viaOptions.toVariantList());
    setoption(RecoveryOption::ViaSeed);
}

void AddSoftwareKeyViewModel::selectedOption(int option) {
    setoption(option);
}

void AddSoftwareKeyViewModel::onContinueClicked() {
    GUARD_SUB_SCREEN_MANAGER()
    switch (option()) {
    case RecoveryOption::ViaSeed: {        
        subMng->show(qml::features::signers::qrecoverviaseed);
        break;
    }
    case RecoveryOption::ViaXPRV: {
        subMng->show(qml::features::signers::qrecoverviaxprv);
        break;
    }
    default:
        break;
    }
}
} // namespace features::signers::viewmodels
