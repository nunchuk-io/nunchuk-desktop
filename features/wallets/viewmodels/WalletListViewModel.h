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
    ~WalletListViewModel() override = default;

    void onInit() override;
    void onDispose() override;
};
} // namespace features::wallets::viewmodels
