#ifndef EDITPLATFORMKEYPOLICIESVIEWMODEL_H
#define EDITPLATFORMKEYPOLICIESVIEWMODEL_H

#include "core/viewmodel/BaseViewModel.h"
#include "core/viewmodel/DefinePropertyMacros.h"

namespace features::signers::viewmodels {

using core::viewmodels::BaseViewModel;

class EditPlatformKeyPoliciesViewModel : public BaseViewModel {
    Q_OBJECT
    DEFINE_QT_PROPERTY(QString, policyName)
    DEFINE_QT_PROPERTY(int, signerType)
    DEFINE_QT_PROPERTY(QString, signerTag)
    DEFINE_QT_PROPERTY(bool, autoBroadcast)
    DEFINE_QT_PROPERTY(bool, enableCoSigningDelay)
    DEFINE_QT_PROPERTY(bool, enableSpendingLimit)
    DEFINE_QT_PROPERTY(QString, currency)
    DEFINE_QT_PROPERTY(double, balance)
    DEFINE_QT_PROPERTY(QString, interval)
    DEFINE_QT_PROPERTY(QString, intervalStr)
    DEFINE_QT_PROPERTY(int, hour)
    DEFINE_QT_PROPERTY(int, minute)
    DEFINE_QT_PROPERTY(int, signingDelaySeconds)

  public:
    explicit EditPlatformKeyPoliciesViewModel(QObject *parent = nullptr);
    void onInit() override;

  public slots:
    void onApplyClicked();
};

} // namespace features::signers::viewmodels

#endif // EDITPLATFORMKEYPOLICIESVIEWMODEL_H
