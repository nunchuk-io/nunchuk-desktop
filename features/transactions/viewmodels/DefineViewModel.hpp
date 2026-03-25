#pragma once
#include "core/viewmodel/DefineViewModelMacros.h"
#include "ConfirmTransactionViewModel.h"
#include "TransactionDetailsViewModel.h"
#include "TransactionDetailsClaimedViewModel.h"

namespace features::transactions::viewmodels {
static inline void registerViewModels() {
const char* uri = "Features.Transactions.ViewModels";

REGISTER_VIEWMODEL(ConfirmTransactionViewModel)
REGISTER_VIEWMODEL(TransactionDetailsViewModel)
REGISTER_VIEWMODEL(TransactionDetailsClaimedViewModel)

}
} // namespace features::transactions::viewmodels
    
