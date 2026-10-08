#pragma once
#include "TypeDefine.h"
#include "WalletModel.h"
#include "core/viewmodel/DefinePropertyMacros.h"
#include "features/common/viewmodels/OffChainTimelockViewModel.h"
#include "features/inheritance/usecases/GetInheritancePlanUseCase.h"

namespace features::inheritance::offchain::viewmodels {
using features::common::viewmodels::OffChainTimelockViewModel;
using features::inheritance::usecases::GetInheritancePlanUseCase;
class ViewInheritancePlanViewModel : public OffChainTimelockViewModel {
    Q_OBJECT
    DEFINE_QT_PROPERTY(QString, distribution_method)
    DEFINE_QT_PROPERTY(QString, beneficiary_mode)
    DEFINE_QT_PROPERTY(QString, buffer_apply_on)
    DEFINE_QT_PROPERTY(QString, release_method)
    DEFINE_QT_PROPERTY(QVariant, buffer_period)
    DEFINE_QT_PROPERTY(QString, firstWithdrawalDate)
    DEFINE_QT_PROPERTY(QVariantList, stagesData)
    DEFINE_QT_PROPERTY(QVariantList, timelineStagesData)
    DEFINE_QT_PROPERTY(QVariantList, beneficiariesData)
    DEFINE_QT_PROPERTY(QVariantList, assetAllocation)
    DEFINE_QT_PROPERTY(bool, isDataChanged)
    DEFINE_QT_PROPERTY(QVariant, fallback_policy)
    DEFINE_QT_PROPERTY(int, fallback_interval_count)
    DEFINE_QT_PROPERTY(QString, fallback_interval)
    DEFINE_QT_PROPERTY(QString, fallback_dateTime)
    DEFINE_QT_PROPERTY(QString, fallback_type)

  public:
    explicit ViewInheritancePlanViewModel(QObject *parent = nullptr);

    void onInit() override;
    void initPhasedRolloutData(QJsonObject inheritance);
  public slots:
    void timeLockEditClicked();
    void onAssetAllocationEditClicked();
    void onReleaseMethodEditClicked();
    void onReleaseScheduleEditClicked();
    void onTimezoneEditClicked();
    void onFallbackSettingsEditClicked();
    void onBeneficiarySchedulesEditClicked();
    void onPhasedRolloutRefresh();
    void compareAndSet(const QString &key);

    void discardChanges();
    void finalizeChanges();
    void securityQuestionVerified();
    void cancelInheritancePlan();

  private:
    GetInheritancePlanUseCase m_getInheritancePlanUC;
};
} // namespace features::inheritance::offchain::viewmodels
