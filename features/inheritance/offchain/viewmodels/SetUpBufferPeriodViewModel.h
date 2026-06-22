#pragma once
#include "TypeDefine.h"
#include "core/viewmodel/ActionViewModel.h"
#include "core/viewmodel/DefinePropertyMacros.h"
#include "features/inheritance/common/usecases/GetListBufferPeriodsUseCase.h"

namespace features::inheritance::offchain::viewmodels {
using core::viewmodels::ActionViewModel;
using features::inheritance::usecases::GetListBufferPeriodsUseCase;
class SetUpBufferPeriodViewModel : public ActionViewModel {
    Q_OBJECT
    DEFINE_QT_PROPERTY(QString, buffer_period_id)
    DEFINE_QT_PROPERTY(QVariantList, periods)
  public:
    explicit SetUpBufferPeriodViewModel(QObject *parent = nullptr);

    void onInit() override;

    DEFINE_SET_GET(QJsonArray, periodList)
    
  public slots:
    void onContinueClicked();
  private: 
    GetListBufferPeriodsUseCase m_getListBufferPeriodsUC;
};
} // namespace features::inheritance::offchain::viewmodels
