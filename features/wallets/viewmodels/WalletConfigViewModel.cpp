#include "WalletConfigViewModel.h"
#include "core/ui/UiServices.inc"
#include "generated_qml_keys.hpp"

namespace features::wallets::viewmodels {
using namespace core::viewmodels;

WalletConfigViewModel::WalletConfigViewModel(QObject *parent) 
: ActionViewModel(parent)
{
}

} // namespace features::wallets::viewmodels
