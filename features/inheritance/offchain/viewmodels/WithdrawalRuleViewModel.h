#pragma once
#include "TypeDefine.h"
#include "core/viewmodel/ActionViewModel.h"
#include "core/viewmodel/DefinePropertyMacros.h"

namespace features::inheritance::offchain::viewmodels {
using core::viewmodels::ActionViewModel;
class WithdrawalRuleViewModel : public ActionViewModel {
    Q_OBJECT
    DEFINE_QT_PROPERTY(QVariant, stage)
    DEFINE_QT_PROPERTY(int, amountPerRelease)
    DEFINE_QT_PROPERTY(int, repeatEvery)
    DEFINE_QT_PROPERTY(QString, repeatInterval)
    DEFINE_QT_PROPERTY(int, totalAllocation)
  public:
    explicit WithdrawalRuleViewModel(QObject *parent = nullptr);

    void onInit() override;
  public slots:
    void nextClicked();
    void prevClicked();
    void deleteClicked();
    void continueClicked();

  private:
};
} // namespace features::inheritance::offchain::viewmodels
