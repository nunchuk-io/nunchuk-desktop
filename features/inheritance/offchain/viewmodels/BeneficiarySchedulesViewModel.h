#pragma once
#include "TypeDefine.h"
#include "core/viewmodel/ActionViewModel.h"
#include "core/viewmodel/DefinePropertyMacros.h"
#include <QJsonArray>

namespace features::inheritance::offchain::viewmodels {
using core::viewmodels::ActionViewModel;
class BeneficiarySchedulesViewModel : public ActionViewModel {
    Q_OBJECT
    DEFINE_QT_PROPERTY(QVariantList, beneficiaries)
    DEFINE_QT_PROPERTY(QVariantList, assetAllocation)
    DEFINE_QT_PROPERTY(bool, hasEditBtn)
    DEFINE_QT_PROPERTY(QString, release_method)
    
  public:
    explicit BeneficiarySchedulesViewModel(QObject *parent = nullptr);

    DEFINE_SET_GET(QJsonArray, beneficiariesJs)

    void onInit() override;
    void initBeneficiarySchedulesData(QJsonObject inheritanceData);
    void initBeneficiarySchedulesData(QJsonArray beneficiaries);

  public slots:
    void onEditBeneficiaryEmailClicked(QString email);
    void onSaveClicked();
    void onReleaseMethodEditClicked();
};
} // namespace features::inheritance::offchain::viewmodels
