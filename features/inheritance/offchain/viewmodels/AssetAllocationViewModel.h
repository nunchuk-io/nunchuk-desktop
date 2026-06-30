#pragma once
#include "TypeDefine.h"
#include "core/viewmodel/ActionViewModel.h"
#include "core/viewmodel/DefinePropertyMacros.h"

namespace features::inheritance::offchain::viewmodels {
using core::viewmodels::ActionViewModel;
class AssetAllocationViewModel : public ActionViewModel {
    Q_OBJECT
    DEFINE_QT_PROPERTY(QVariantList, assetAllocation)
    DEFINE_QT_PROPERTY(bool, isValidData)
  public:
    explicit AssetAllocationViewModel(QObject *parent = nullptr);

    void onInit() override;
    void initAssetAllocationData(QJsonObject inheritanceData);

    bool isValidEmail(const QString &email);
    bool validateEmails(const QVariantList &allocations);
    bool markEmailErrors();         // validates, writes emailError into each allocation, shows toast for first failure
    void calculateTotalPercentage();

  public slots:
    void deleteBeneficiary(int index);
    void updateBeneficiaryEmail(int index, const QString &email);
    void updateBeneficiaryPercentage(int index, int percentage);
    void addBeneficiary();
    void saveBeneficiaries();
};
} // namespace features::inheritance::offchain::viewmodels
