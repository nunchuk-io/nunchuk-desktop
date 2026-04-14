#pragma once
#include "core/viewmodel/ActionViewModel.h"
#include "features/transactions/usecases/GetGroupDummyTransactionUseCase.h"
#include "features/wallets/usecases/MarkGroupWalletAlertViewedUseCase.h"
#include "features/wallets/usecases/DismissGroupWalletAlertUseCase.h"

namespace features::wallets::viewmodels {
using core::viewmodels::ActionViewModel;
using features::transactions::usecases::GetGroupDummyTransactionUseCase;
using features::wallets::usecases::MarkGroupWalletAlertViewedUseCase;
using features::wallets::usecases::DismissGroupWalletAlertUseCase;
class HomePendingAcceptedViewModel : public ActionViewModel {
    Q_OBJECT
  public:
    explicit HomePendingAcceptedViewModel(QObject *parent = nullptr);
    ~HomePendingAcceptedViewModel();
    void onInit() override;

  public slots:
    void onViewClicked(const QString &alertId);
    void onDismissClicked(const QString &alertId);

  private:
    void handlePolicyChange(QJsonObject alertJson);
    void handlePolicyChangInProgress(QJsonObject alertJson);
    void GetTransaction(const QString &walletId, const QString &dummyTransactionId);

  private:
    GetGroupDummyTransactionUseCase m_getTransactionUseCase;
    MarkGroupWalletAlertViewedUseCase m_markAlertViewedUseCase;
    DismissGroupWalletAlertUseCase m_dismissAlertUseCase;
};
} // namespace features::wallets::viewmodels
