#include "AssetAllocationViewModel.h"
#include "core/ui/UiServices.inc"
#include "features/inheritance/offchain/flows/PhasedRolloutFlow.h"
#include "generated_qml_keys.hpp"
#include "core/common/resources/AppStrings.h"

namespace features::inheritance::offchain::viewmodels {
using namespace core::viewmodels;
using namespace features::inheritance::offchain::flows;

AssetAllocationViewModel::AssetAllocationViewModel(QObject *parent) : ActionViewModel(parent) {}

void AssetAllocationViewModel::onInit() {
    GUARD_FLOW_MANAGER()
    auto flow = flowMng->startFlow<PhasedRolloutFlow>();
    if (!flow) {
        return;
    }

    QJsonObject inheritanceData = flow->inheritance();
    if (inheritanceData.isEmpty() || !inheritanceData.contains("stages")) {
        return;
    }
    initAssetAllocationData(inheritanceData);
}

void AssetAllocationViewModel::initAssetAllocationData(QJsonObject inheritanceData) {
    QJsonArray beneficiariesArray = inheritanceData["beneficiaries"].toArray();
    setassetAllocation(helper::convertAssetAllocationData(beneficiariesArray));
}

bool AssetAllocationViewModel::isValidEmail(const QString &email) {
    QRegularExpression emailRegex(R"(^[^\s@]+@[^\s@]+\.[^\s@]+$)");
    return emailRegex.match(email).hasMatch();
}

bool AssetAllocationViewModel::validateEmails(const QVariantList &allocations) {
    QSet<QString> emailSet;
    for (const auto &allocation : allocations) {
        QVariantMap allocationMap = allocation.toMap();
        QString email = allocationMap["email"].toString();
        if (email.isEmpty()) {
            emit showToast(0, Strings.STR_QML_2229(), EWARNING::WarningType::ERROR_MSG);
            return false; // Allow empty email, validation can be handled separately if needed
        }
        if (!isValidEmail(email)) {
            emit showToast(0, Strings.STR_QML_2230(), EWARNING::WarningType::ERROR_MSG);
            return false; // Invalid email format
        }
        if (emailSet.contains(email)) {
            emit showToast(0, Strings.STR_QML_2228(), EWARNING::WarningType::ERROR_MSG);
            return false; // Duplicate email found
        }
        emailSet.insert(email);
    }
    return true; // All emails are valid and unique
}

void AssetAllocationViewModel::deleteBeneficiary(int index) {
    auto currentAllocations = assetAllocation();
    if (index < 0 || index >= currentAllocations.size()) {
        return;
    }
    currentAllocations.removeAt(index);
    setassetAllocation(currentAllocations);
}

void AssetAllocationViewModel::updateBeneficiaryEmail(int index, const QString &email) {
    auto currentAllocations = assetAllocation();
    if (index < 0 || index >= currentAllocations.size()) {
        return;
    }
    QVariantMap allocationMap = currentAllocations[index].toMap();
    allocationMap["email"] = email;
    currentAllocations[index] = allocationMap;
    setassetAllocation(currentAllocations);
}

void AssetAllocationViewModel::updateBeneficiaryPercentage(int index, int percentage) {
    auto currentAllocations = assetAllocation();
    if (index < 0 || index >= currentAllocations.size()) {
        return;
    }
    QVariantMap allocationMap = currentAllocations[index].toMap();
    allocationMap["asset_percentage"] = percentage;
    currentAllocations[index] = allocationMap;
    setassetAllocation(currentAllocations);
}

void AssetAllocationViewModel::addBeneficiary() {
    auto currentAllocations = assetAllocation();
    int newIndex = currentAllocations.size();
    auto colors = helper::colorBeneficiaries();
    QVariantMap newBeneficiary;
    newBeneficiary["email"] = "";
    newBeneficiary["color"] = colors[newIndex % colors.size()]; // Default color for new beneficiary
    newBeneficiary["asset_percentage"] = 0;                     // Default percentage for new beneficiary
    currentAllocations.append(newBeneficiary);
    setassetAllocation(currentAllocations);
}

void AssetAllocationViewModel::saveBeneficiaries() {
    GUARD_FLOW_MANAGER()
    auto flow = flowMng->startFlow<PhasedRolloutFlow>();
    if (!flow) {
        return;
    }
    if(!validateEmails(assetAllocation())) {
        return; // Stop saving if email validation fails
    }
    QJsonObject inheritance = flow->inheritance();
    QJsonArray beneficiariesArray = inheritance["beneficiaries"].toArray();
    helper::convertAssetAllocationsToJsonArray(beneficiariesArray, assetAllocation());
    inheritance["beneficiaries"] = beneficiariesArray;
    flow->setinheritance(inheritance);
    close();
    flow->finalizeInheritanceChanges();
}

} // namespace features::inheritance::offchain::viewmodels
