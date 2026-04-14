#pragma once
#include "ConfirmTransactionViewModel.h"
#include "GroupDummyTransactionDetailsViewModel.h"
#include "NormalTransactionDetailsViewModel.h"
#include "OffClaimTransactionDetailsViewModel.h"
#include "OnClaimTransactionDetailsViewModel.h"
#include "SignaturesRequiredViewModel.h"
#include "TransactionDetailsClaimedViewModel.h"
#include "TransactionDetailsViewModel.h"
#include "core/viewmodel/DefineViewModelMacros.h"

namespace features::transactions::viewmodels {
static inline void registerViewModels() {
    const char *uri = "Features.Transactions.ViewModels";

    REGISTER_VIEWMODEL(ConfirmTransactionViewModel)
    REGISTER_VIEWMODEL(GroupDummyTransactionDetailsViewModel)
    REGISTER_VIEWMODEL(NormalTransactionDetailsViewModel)
    REGISTER_VIEWMODEL(OffClaimTransactionDetailsViewModel)
    REGISTER_VIEWMODEL(OnClaimTransactionDetailsViewModel)
    REGISTER_VIEWMODEL(SignaturesRequiredViewModel)
    REGISTER_VIEWMODEL(TransactionDetailsViewModel)
    REGISTER_VIEWMODEL(TransactionDetailsClaimedViewModel)
}
} // namespace features::transactions::viewmodels
