#pragma once
#include "TypeDefine.h"
#include "WalletModel.h"
#include "core/viewmodel/ActionViewModel.h"

namespace features::wallets::viewmodels {
using core::viewmodels::ActionViewModel;
class WalletListViewModel : public ActionViewModel {
    Q_OBJECT
  public:
    explicit WalletListViewModel(QObject *parent = nullptr);
    ~WalletListViewModel();
    void onInit() override;
};
} // namespace features::wallets::viewmodels
