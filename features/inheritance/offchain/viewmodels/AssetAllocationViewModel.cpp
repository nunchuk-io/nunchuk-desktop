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
    calculateTotalPercentage();
}

bool AssetAllocationViewModel::isValidEmail(const QString &email) {
    QRegularExpression emailRegex(R"(^[^\s@]+@[^\s@]+\.[^\s@]+$)");
    return emailRegex.match(email).hasMatch();
}

bool AssetAllocationViewModel::validateEmails(const QVariantList &allocations) {
    // Pure check — no side effects, no toasts. Use markEmailErrors() to show per-field errors.
    QSet<QString> emailSet;
    for (const auto &allocation : allocations) {
        QVariantMap allocationMap = allocation.toMap();
        QString email = allocationMap["email"].toString();
        if (email.isEmpty() || !isValidEmail(email) || emailSet.contains(email)) {
            return false;
        }
        emailSet.insert(email);
    }
    return true;
}

bool AssetAllocationViewModel::markEmailErrors() {
    // Validates each allocation, writes "emailError" field into the list (triggers QML highlight),
    // and emits a toast for the first failure found. Returns true if all valid.
    auto allocations = assetAllocation();
    bool allValid = true;
    bool firstErrorToasted = false;
    QSet<QString> emailSet;

    for (int i = 0; i < allocations.size(); ++i) {
        QVariantMap allocationMap = allocations[i].toMap();
        QString email = allocationMap["email"].toString();
        QString error;

        if (email.isEmpty()) {
            error = Strings.STR_QML_2229();
        } else if (!isValidEmail(email)) {
            error = Strings.STR_QML_2230();
        } else if (emailSet.contains(email)) {
            error = Strings.STR_QML_2228();
        } else {
            emailSet.insert(email);
        }

        allocationMap["emailError"] = error;
        allocations[i] = allocationMap;

        if (!error.isEmpty()) {
            allValid = false;
            if (!firstErrorToasted) {
                firstErrorToasted = true;
                emit showToast(0, error, EWARNING::WarningType::ERROR_MSG);
            }
        }
    }

    setassetAllocation(allocations);
    return allValid;
}

void AssetAllocationViewModel::deleteBeneficiary(int index) {
    auto currentAllocations = assetAllocation();
    if (index < 0 || index >= currentAllocations.size()) {
        return;
    }
    currentAllocations.removeAt(index);
    setassetAllocation(currentAllocations);
    calculateTotalPercentage();
}

void AssetAllocationViewModel::calculateTotalPercentage() {
    int totalPercentage = 0;
    bool hasZeroPercentage = false;
    for (const auto &beneficiary : assetAllocation()) {
        QVariantMap beneficiaryObj = beneficiary.toMap();
        int pct = beneficiaryObj["asset_percentage"].toInt();
        totalPercentage += pct;
        if (pct == 0) {
            hasZeroPercentage = true;
        }
    }
    // Email validity is intentionally NOT included here.
    // isValidData only gates the percentage constraint so the Save button
    // stays enabled even when emails are incomplete. Email errors are shown
    // (toast + red border) only when the user clicks Save → saveBeneficiaries()
    // → markEmailErrors().
    // Must be exactly 100% with no beneficiary at 0% (0% beneficiary has no meaningful allocation).
    setisValidData(totalPercentage == 100 && !hasZeroPercentage);
}

void AssetAllocationViewModel::updateBeneficiaryEmail(int index, const QString &email) {
    auto currentAllocations = assetAllocation();
    if (index < 0 || index >= currentAllocations.size()) {
        return;
    }
    QVariantMap allocationMap = currentAllocations[index].toMap();
    allocationMap["email"] = email;
    allocationMap["emailError"] = "";  // clear per-field error as user types
    currentAllocations[index] = allocationMap;
    setassetAllocation(currentAllocations);
}

void AssetAllocationViewModel::updateBeneficiaryPercentage(int index, int percentage) {
    auto currentAllocations = assetAllocation();
    if (index < 0 || index >= currentAllocations.size()) {
        return;
    }
    // Compute total % contributed by all other beneficiaries (excluding this one).
    // Clamp the new value so the grand total cannot exceed 100%.
    // QML slider binding (value: beneficiaryPercentage) will snap back to the clamped value automatically.
    int otherTotal = 0;
    for (int i = 0; i < currentAllocations.size(); ++i) {
        if (i != index) {
            otherTotal += currentAllocations[i].toMap()["asset_percentage"].toInt();
        }
    }
    int clamped = qBound(0, percentage, 100 - otherTotal);

    QVariantMap allocationMap = currentAllocations[index].toMap();
    allocationMap["asset_percentage"] = clamped;
    currentAllocations[index] = allocationMap;
    setassetAllocation(currentAllocations);
    calculateTotalPercentage();
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
    calculateTotalPercentage();
}

void AssetAllocationViewModel::saveBeneficiaries() {
    GUARD_FLOW_MANAGER()
    auto flow = flowMng->startFlow<PhasedRolloutFlow>();
    if (!flow) {
        return;
    }
    // markEmailErrors() writes per-field emailError into assetAllocation (triggers red highlight in QML)
    // and emits a toast for the first failure.
    if (!markEmailErrors()) {
        return;
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
