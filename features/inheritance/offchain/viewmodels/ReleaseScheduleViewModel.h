#pragma once
#include "TypeDefine.h"
#include "core/viewmodel/ActionViewModel.h"
#include "core/viewmodel/DefinePropertyMacros.h"

namespace features::inheritance::offchain::viewmodels {
using core::viewmodels::ActionViewModel;
class ReleaseScheduleViewModel : public ActionViewModel {
    Q_OBJECT
    DEFINE_QT_PROPERTY(QVariantList, stagesData)
    DEFINE_QT_PROPERTY(QVariantList, timelineStagesData)
    DEFINE_QT_PROPERTY(QVariant, buffer_period)
    DEFINE_QT_PROPERTY(QString, buffer_apply_on)
    DEFINE_QT_PROPERTY(QString, beneficiary_mode)
    DEFINE_QT_PROPERTY(QString, release_method)
    DEFINE_QT_PROPERTY(int, totalPercentage)
  public:
    explicit ReleaseScheduleViewModel(QObject *parent = nullptr);

    void onInit() override;
    void initStagesData(QJsonObject inheritance);
    void updateStageTotalPercentage();
    void addStage();
  public slots:
    void onExpandClicked(int index);
    void onEditStageClicked(int index);
    void onAddStageClicked();
    void onSaveClicked();
    void onBufferPeriodonClicked();
};
} // namespace features::inheritance::offchain::viewmodels
