#pragma once
#include "BasePlatformKeyPoliciesViewModel.h"
#include "core/viewmodel/BaseViewModel.h"
#include "core/viewmodel/DefinePropertyMacros.h"
#include "features/transactions/usecases/CancelGroupDummyTransactionUseCase.h"
#include <nunchuk.h>

namespace features::signers::viewmodels {
using core::viewmodels::BaseViewModel;
using features::transactions::usecases::CancelGroupDummyTransactionUseCase;

class PerKeyPlatformKeyPoliciesViewModel : public BasePlatformKeyPoliciesViewModel {
    Q_OBJECT
    DEFINE_QT_PROPERTY(QVariantList, perKeyPolicies)
    DEFINE_QT_PROPERTY(int, pending_signatures)
    DEFINE_QT_PROPERTY(bool, isTypeChanged)
    DEFINE_QT_PROPERTY(int, afterHours)
  public:
    explicit PerKeyPlatformKeyPoliciesViewModel(QObject *parent = nullptr);

  protected:
    void onInit() override;
    void initPolicyFromSandbox();
    void initPolicyFromDummyTransaction();
    void initPolicyFromWalletConfig();
    QVariantList makeRedHighlightPolicy(const QVariantList &old_perkey_policies, const QVariantList &new_perkey_policies);

    void cancelDummyTransaction();
  public slots:
    void onEditClicked();
    void onEditPolicyClicked(QString xfp);
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

  private:
    std::optional<nunchuk::GroupSandbox> m_groupSandbox;
    QVariantMap filterSignerPolicy(QString xfp);
    void replacePolicy(const QVariantMap &newPolicy);
    CancelGroupDummyTransactionUseCase m_cancelGroupDummyTransactionUC;
};
} // namespace features::signers::viewmodels
