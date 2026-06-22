#pragma once
#include "TypeDefine.h"
#include "core/viewmodel/ActionViewModel.h"
#include "core/viewmodel/DefinePropertyMacros.h"

namespace features::inheritance::offchain::viewmodels {
using core::viewmodels::ActionViewModel;
class DetailBeneficiaryViewModel : public ActionViewModel {
    Q_OBJECT
    DEFINE_QT_PROPERTY(QString, beneficiary_mode)
    DEFINE_QT_PROPERTY(QString, release_method)
    DEFINE_QT_PROPERTY(QString, benefiEmail)
    DEFINE_QT_PROPERTY(QVariantList, stagesData)
    DEFINE_QT_PROPERTY(QVariantList, timelineStagesData)
    DEFINE_QT_PROPERTY(int, totalPercentage)
  public:
    explicit DetailBeneficiaryViewModel(QObject *parent = nullptr);

    void onInit() override;
    void initBeneficiaryData(const QJsonObject &beneficiarySelected);
    void updateStageTotalPercentage();
    void applyStageData(const QVariantMap &stageData);
    void saveBeneficiaryData();
    void addStage();
  public slots:
    void onExpandClicked(int index);
    void onEditStageClicked(int index);
    void onAddStageClicked();
    void onContinueClicked();
};
} // namespace features::inheritance::offchain::viewmodels
