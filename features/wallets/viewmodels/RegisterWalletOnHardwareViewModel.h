#pragma once
#include "TypeDefine.h"
#include "WalletModel.h"
#include "core/viewmodel/ActionViewModel.h"

namespace features::wallets::viewmodels {
using core::viewmodels::ActionViewModel;
class RegisterWalletOnHardwareViewModel : public ActionViewModel {
    Q_OBJECT
    Q_PROPERTY(Wallet*  walletInfo READ walletInfo NOTIFY walletInfoChanged)
    Q_PROPERTY(FlowEvent event READ event NOTIFY eventChanged)
  public:
    explicit RegisterWalletOnHardwareViewModel(QObject *parent = nullptr);
    ~RegisterWalletOnHardwareViewModel() override = default;
    enum class FlowEvent: int {
      WithdrawBitcoin,
      CreateWallet,
      ViewWallets,
      None,
    };
    Q_ENUM(FlowEvent)
    Wallet* walletInfo() const;
    FlowEvent event() const;
    void setEvent(FlowEvent event);
    void onInit() override;
  signals:
    void walletInfoChanged();
    void eventChanged();
  private:
    QWalletPtr m_walletInfo;
    FlowEvent m_event {FlowEvent::None};
};
} // namespace features::wallets::viewmodels
