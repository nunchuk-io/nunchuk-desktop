#pragma once
#include "TypeDefine.h"
#include "WalletModel.h"
#include "core/viewmodel/ActionViewModel.h"
#include "core/viewmodel/DefinePropertyMacros.h"
#include <nunchuk.h>

namespace features::wallets::viewmodels {
using core::viewmodels::ActionViewModel;
class RegisterWalletOnHardwareViewModel : public ActionViewModel {
    Q_OBJECT
    Q_PROPERTY(FlowEvent event READ event NOTIFY eventChanged)
    DEFINE_QT_PROPERTY_PTR(Wallet, walletInfo)

  public:
    explicit RegisterWalletOnHardwareViewModel(QObject *parent = nullptr);
    DEFINE_SET_GET(nunchuk::Wallet, nunWallet)

    enum class FlowEvent: int {
      WithdrawBitcoin,
      CreateWallet,
      ViewWallets,
      None,
    };
    Q_ENUM(FlowEvent)
    FlowEvent event() const;
    void setEvent(FlowEvent event);
    void onInit() override;
  signals:
    void eventChanged();
  private:
    FlowEvent m_event {FlowEvent::None};
};
} // namespace features::wallets::viewmodels
