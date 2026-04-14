#include "AppRegister.h"
#include "core/common/datatypes/NunchukMetaType.hpp"
#include "features/claiming/viewmodels/DefineViewModel.hpp"
#include "features/common/viewmodels/DefineViewModel.hpp"
#include "features/draftwallets/onchain/viewmodels/DefineViewModel.hpp"
#include "features/draftwallets/sandbox/viewmodels/DefineViewModel.hpp"
#include "features/inheritance/offchain/viewmodels/DefineViewModel.hpp"
#include "features/inheritance/onchain/viewmodels/DefineViewModel.hpp"
#include "features/rightpanel/viewmodels/DefineViewModel.hpp"
#include "features/signers/viewmodels/DefineViewModel.hpp"
#include "features/transactions/viewmodels/DefineViewModel.hpp"
#include "features/wallets/viewmodels/DefineViewModel.hpp"
#include <QQmlEngine>

namespace app {
void registerViewModels() {
    core::common::datatypes::registerNunchukMetaTypes();
    features::signers::viewmodels::registerViewModels();
    features::claiming::viewmodels::registerViewModels();
    features::wallets::viewmodels::registerViewModels();
    features::draftwallets::onchain::viewmodels::registerViewModels();
    features::common::viewmodels::registerViewModels();
    features::inheritance::onchain::viewmodels::registerViewModels();
    features::inheritance::offchain::viewmodels::registerViewModels();
    features::rightpanel::viewmodels::registerViewModels();
    features::transactions::viewmodels::registerViewModels();
    features::draftwallets::sandbox::viewmodels::registerViewModels();
}
} // namespace app