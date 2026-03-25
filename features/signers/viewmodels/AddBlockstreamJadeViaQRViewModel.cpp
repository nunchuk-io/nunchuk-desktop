#include "AddBlockstreamJadeViaQRViewModel.h"
#include "core/ui/UiServices.inc"
#include "generated_qml_keys.hpp"
#include "core/common/resources/AppStrings.h"
#include "features/signers/flows/KeySetupFlow.h"

namespace features::signers::viewmodels {
using namespace features::signers::flows;

AddBlockstreamJadeViaQRViewModel::AddBlockstreamJadeViaQRViewModel(QObject *parent) 
: AddKeyBaseViewModel(parent) {
    
}


} // namespace features::signers::viewmodels
