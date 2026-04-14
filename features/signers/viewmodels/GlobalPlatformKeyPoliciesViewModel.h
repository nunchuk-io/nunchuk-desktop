#pragma once
#include "core/viewmodel/BaseViewModel.h"
#include "core/viewmodel/DefinePropertyMacros.h"
#include "features/transactions/usecases/CancelGroupDummyTransactionUseCase.h"
#include <QVariant>

namespace features::signers::viewmodels {
using core::viewmodels::BaseViewModel;
using features::transactions::usecases::CancelGroupDummyTransactionUseCase;

class GlobalPlatformKeyPoliciesViewModel : public BaseViewModel {
    Q_OBJECT
    DEFINE_QT_PROPERTY(QVariantMap, globalPolicy)
    DEFINE_QT_PROPERTY(bool, isWallet)
    DEFINE_QT_PROPERTY(bool, isDummyTx)
    DEFINE_QT_PROPERTY(int, pending_signatures)
    DEFINE_QT_PROPERTY(bool, isTypeChanged)
    DEFINE_QT_PROPERTY(int, afterHours)
  public:
    explicit GlobalPlatformKeyPoliciesViewModel(QObject *parent = nullptr);

  protected:
    void onInit() override;
    void initPolicyFromSandbox();
    void initPolicyFromWalletConfig();
    void initPolicyFromDummyTransaction();

    void cancelDummyTransaction();
  public slots:
    void onEditClicked();
    void onEditPolicyClicked();
    void onRemovePlatformKeyClicked();
    void onContinueToSaveChangesClicked();
    void onApplyClicked();
    void onDiscardChangesClicked();
    void onContinueSignaturePendingClicked();
    void slotConfirmDummyTransaction();
    void onSignClicked();

  signals:
    void notifySecurityDelayRequired();
    void notifyDummyTransactionRequired();

  public:
    int backToScreen() override;

  private:
    CancelGroupDummyTransactionUseCase m_cancelGroupDummyTransactionUC;
};
} // namespace features::signers::viewmodels
