#pragma once
#include "TypeDefine.h"
#include "core/viewmodel/ActionViewModel.h"
#include "core/viewmodel/DefinePropertyMacros.h"
#include "features/inheritance/usecases/GetInheritancePlanUseCase.h"

namespace features::inheritance::offchain::viewmodels {
using core::viewmodels::ActionViewModel;
using features::inheritance::usecases::GetInheritancePlanUseCase;
class ReleaseMethodViewModel : public ActionViewModel {
    Q_OBJECT
    DEFINE_QT_PROPERTY(QString, selectedMethod)
    DEFINE_QT_PROPERTY(QString, selectedMethodOld)
  public:
    explicit ReleaseMethodViewModel(QObject *parent = nullptr);

    void onInit() override;

  public slots:
    void onContinueClicked();
  private:
    GetInheritancePlanUseCase m_getInheritancePlanUC;
};
} // namespace features::inheritance::offchain::viewmodels
