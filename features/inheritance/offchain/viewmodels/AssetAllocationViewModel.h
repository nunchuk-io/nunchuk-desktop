#pragma once
#include "TypeDefine.h"
#include "core/viewmodel/ActionViewModel.h"
#include "core/viewmodel/DefinePropertyMacros.h"

namespace features::inheritance::offchain::viewmodels {
using core::viewmodels::ActionViewModel;
class AssetAllocationViewModel : public ActionViewModel {
    Q_OBJECT
    DEFINE_QT_PROPERTY(QVariantList, assetAllocation)
  public:
    explicit AssetAllocationViewModel(QObject *parent = nullptr);

    void onInit() override;
    void initAssetAllocationData(QJsonObject inheritanceData);

    bool isValidEmail(const QString &email);
    bool validateEmails(const QVariantList &allocations);

  public slots:
    void deleteBeneficiary(int index);
    void updateBeneficiaryEmail(int index, const QString &email);
    void updateBeneficiaryPercentage(int index, int percentage);
    void addBeneficiary();
    void saveBeneficiaries();
};
} // namespace features::inheritance::offchain::viewmodels
