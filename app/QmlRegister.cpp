#include "AppRegister.h"
#include <QQmlEngine>
#include "core/viewmodel/ViewModelContext.h"
#include "features/claiming/viewmodels/DefineViewModel.hpp"
#include "features/signers/viewmodels/DefineViewModel.hpp"
#include "features/wallets/viewmodels/DefineViewModel.hpp"
#include "features/draftwallets/onchain/viewmodels/DefineViewModel.hpp"
#include "features/common/viewmodels/DefineViewModel.hpp"
#include "features/inheritance/onchain/viewmodels/DefineViewModel.hpp"
#include "features/inheritance/offchain/viewmodels/DefineViewModel.hpp"

namespace app {
    void registerViewModels() {
        qmlRegisterType<core::viewmodels::ViewModelContext>("Core.ViewModel", 1, 0, "ViewModelContext");
        features::signers::viewmodels::registerViewModels();
        features::claiming::viewmodels::registerViewModels();
        features::wallets::viewmodels::registerViewModels();
        features::draftwallets::onchain::viewmodels::registerViewModels();
        features::common::viewmodels::registerViewModels();
        features::inheritance::onchain::viewmodels::registerViewModels();
        features::inheritance::offchain::viewmodels::registerViewModels();
    }
}