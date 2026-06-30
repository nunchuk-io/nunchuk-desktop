#include "PhasedRolloutFlow.h"
#include "core/common/resources/AppStrings.h"
#include "core/ui/UiServices.inc"
#include "core/utils/Utils.h"

namespace features::inheritance::offchain::flows {
using namespace core::flow;
class TimezoneManager {
  public:
    TimezoneManager() : m_timezones(QWalletTimezoneModelPtr(new QWalletTimezoneModel())) {}
    static TimezoneManager &instance() {
        static TimezoneManager mgr;
        return mgr;
    }
    DEFINE_SET_GET(QTimeZone, timezone)
    void updatetimezone(const QString &timezone) {
        m_timezones->setSelectedTimezone(timezone);
        auto timezoneId = m_timezones->selectedTimezoneId();
        QTimeZone targetTimeZone(timezoneId);
        settimezone(targetTimeZone);
    }

  private:
    QWalletTimezoneModelPtr m_timezones;
};

PhasedRolloutFlow::PhasedRolloutFlow(FlowContext *ctx, QObject *parent) : InheritanceFlow(ctx, parent) {
    connect(this, &PhasedRolloutFlow::valueTimezoneChanged, this, [this]() { TimezoneManager::instance().updatetimezone(valueTimezone()); });
    setcurrent_step(ConfigStep_t::None); // Initialize current step to None
}

void PhasedRolloutFlow::bind(QObject *vm) {
    if (!vm)
        return;
    TimezoneManager::instance().updatetimezone(valueTimezone());
}


bool PhasedRolloutFlow::checkValidCanGoNext() {
    if (!checkStageCanGoNext()) {
        return false;
    }

    if (!checkFallbackValid()) {
        return false;
    }

    return true;
}

bool PhasedRolloutFlow::checkFallbackValid() {
    QJsonObject inheritance = this->inheritance();
    QJsonObject fallback_policy = inheritance.value("fallback_policy").toObject();
    auto fallback_time_millis = fallback_policy.value("fallback_time_millis").toDouble();
    QString type = fallback_policy.value("type").toString();
    if (qUtils::strCompare(type, "DATE_BASED") && fallback_time_millis < lastStageTimeMLs()) {
        emit showToast(0, Strings.STR_QML_2253(), EWARNING::WarningType::ERROR_MSG);
        return false;
    }
    return true;
}

double PhasedRolloutFlow::lastStageTimeMLs() {
    QJsonObject inheritance = this->inheritance();
    QString beneficiary_mode = inheritance.value("beneficiary_mode").toString();
    QString release_method = inheritance.value("release_method").toString();
    if (!(qUtils::strCompare(beneficiary_mode, "MULTIPLE") && qUtils::strCompare(release_method, "INDIVIDUAL"))) {
        QJsonArray stages = inheritance.value("stages").toArray();
        if (stages.isEmpty()) {
            return -1;
        }
        QJsonObject lastStage = stages.last().toObject();
        DBG_INFO << "[PhasedRolloutFlow::lastStageTimeMLs] beneficiary:" << "lastStage:" << lastStage;
        return helper::getLastStageWithdrawalTimeMillis(lastStage.toVariantMap());
    } else {
        QJsonArray beneficiaries = inheritance.value("beneficiaries").toArray();
        double maxLastStageTimeMillis = -1;
        for (const auto &beneficiaryValue : beneficiaries) {
            QJsonObject beneficiary = beneficiaryValue.toObject();
            QJsonArray stages = beneficiary.value("stages").toArray();
            if (stages.isEmpty()) {
                return -1;
            }
            QJsonObject lastStage = stages.last().toObject();
            DBG_INFO << "[PhasedRolloutFlow::lastStageTimeMLs] beneficiary:" << beneficiary.value("email").toString() << "lastStage:" << lastStage;
            maxLastStageTimeMillis = std::max(maxLastStageTimeMillis, (double)helper::getLastStageWithdrawalTimeMillis(lastStage.toVariantMap()));
        }
        return maxLastStageTimeMillis;
    }
}

bool PhasedRolloutFlow::checkStageCanGoNext() {
    QJsonObject inheritance = this->inheritance();
    QString beneficiary_mode = inheritance.value("beneficiary_mode").toString();
    QString release_method = inheritance.value("release_method").toString();
    if (!(qUtils::strCompare(beneficiary_mode, "MULTIPLE") && qUtils::strCompare(release_method, "INDIVIDUAL"))) {
        // For single beneficiary with timeline release method, we can directly finalize changes without associate magic
        QJsonArray stages = inheritance.value("stages").toArray();
        for (int i = 0; i < stages.size(); ++i) {
            QJsonObject first = stages.at(i).toObject();
            if (i < stages.size() - 1) {
                QJsonObject second = stages.at(i + 1).toObject();
                if (!validateDateTime(first.toVariantMap(), second.toVariantMap())) {
                    return false;
                }
            }
        }
    } else {
        QJsonArray beneficiaries = inheritance.value("beneficiaries").toArray();
        for (const auto &beneficiaryValue : beneficiaries) {
            QJsonObject beneficiary = beneficiaryValue.toObject();
            QJsonArray stages = beneficiary.value("stages").toArray();
            for (int i = 0; i < stages.size(); ++i) {
                QJsonObject first = stages.at(i).toObject();
                if (i < stages.size() - 1) {
                    QJsonObject second = stages.at(i + 1).toObject();
                    if (!validateDateTime(first.toVariantMap(), second.toVariantMap())) {
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

bool PhasedRolloutFlow::validateDateTime(const QVariantMap &firstStage, const QVariantMap &secondStage) {
    auto calculatedLastPhraseFirstTimestamp = helper::getLastStageWithdrawalTimeMillis(firstStage);
    auto secondTimestamp = secondStage.value("first_withdrawal_time_millis").toLongLong();

    if (secondTimestamp <= calculatedLastPhraseFirstTimestamp) {
        emit showToast(0, Strings.STR_QML_2233(), EWARNING::WarningType::ERROR_MSG);
        return false;
    }
    // QDateTime dt = QDateTime::currentDateTime();
    // auto currentTimestamp = dt.toMSecsSinceEpoch();
    // if (currentTimestamp >= firstTimestamp || currentTimestamp >= secondTimestamp) {
    //     emit showToast(0, Strings.STR_QML_2248(), EWARNING::WarningType::ERROR_MSG);
    //     return false;
    // }

    // if (activation_time_milis() <= firstTimestamp || activation_time_milis() <= secondTimestamp) {
    //     emit showToast(0, Strings.STR_QML_2234(), EWARNING::WarningType::ERROR_MSG);
    //     return false;
    // }
    return true;
}

QString helper::getIntervalDisplayString(const QString &repeatInterval, int repeatEvery) {
    if (repeatInterval == "DAY") {
        return QString("%1 day%2").arg(repeatEvery).arg(repeatEvery > 1 ? "s" : "");
    } else if (repeatInterval == "WEEK") {
        return QString("%1 week%2").arg(repeatEvery).arg(repeatEvery > 1 ? "s" : "");
    } else if (repeatInterval == "MONTH") {
        return QString("%1 month%2").arg(repeatEvery).arg(repeatEvery > 1 ? "s" : "");
    } else if (repeatInterval == "YEAR") {
        return QString("%1 year%2").arg(repeatEvery).arg(repeatEvery > 1 ? "s" : "");
    }
    return "";
}

long long helper::addIntervalStage(long long firstWithdrawalTimeMillis, const QString &repeatInterval, int repeatEvery) {
    QDateTime firstWithdrawalDateTime = QDateTime::fromMSecsSinceEpoch(firstWithdrawalTimeMillis);
    if (repeatInterval == "DAY") {
        firstWithdrawalDateTime = firstWithdrawalDateTime.addDays(repeatEvery);
    } else if (repeatInterval == "WEEK") {
        firstWithdrawalDateTime = firstWithdrawalDateTime.addDays(repeatEvery * 7);
    } else if (repeatInterval == "MONTH") {
        firstWithdrawalDateTime = firstWithdrawalDateTime.addMonths(repeatEvery);
    } else if (repeatInterval == "YEAR") {
        firstWithdrawalDateTime = firstWithdrawalDateTime.addYears(repeatEvery);
    }
    return firstWithdrawalDateTime.toMSecsSinceEpoch();
}

long long helper::getLastStageWithdrawalTimeMillis(const QVariantMap &stageData) {
    long long firstWithdrawalTimeMillis = stageData.value("first_withdrawal_time_millis").toLongLong();
    QString repeatInterval = stageData.value("repeat_interval").toString();
    int repeatCount = stageData.value("repeat_interval_count").toInt();
    double amountPerRelease = stageData.value("amount_per_release_percentage").toDouble();
    double totalAllocation = stageData.value("total_stage_allocation_percentage").toDouble();
    int totalInstallments = static_cast<int>(std::ceil(totalAllocation / amountPerRelease));
    int totalIntervals = (totalInstallments - 1) * repeatCount; // Total intervals after the first release

    return addIntervalStage(firstWithdrawalTimeMillis, repeatInterval, totalIntervals);
}

QJsonArray helper::generateInstallmentSchedule(const QVariantMap &stageData) {
    DBG_INFO << "[helper::generateInstallmentSchedule] stageData:" << stageData;
    QJsonArray installments;
    long long firstWithdrawalTimeMillis = stageData.value("first_withdrawal_time_millis").toLongLong();
    QString repeatInterval = stageData.value("repeat_interval").toString();
    int repeatCount = stageData.value("repeat_interval_count").toInt();
    double amountPerRelease = stageData.value("amount_per_release_percentage").toDouble();
    double totalAllocation = stageData.value("total_stage_allocation_percentage").toDouble();
    int totalInstallments = static_cast<int>(std::ceil(totalAllocation / amountPerRelease));
    double remainingAllocation = totalAllocation;
    for (int i = 0; i < totalInstallments; ++i) {
        long long installmentTimeMillis = addIntervalStage(firstWithdrawalTimeMillis, repeatInterval, i * repeatCount);
        QJsonObject installment;
        installment["index"] = i + 1;
        if (i < totalInstallments - 1) {
            installment["allocation_percentage"] = amountPerRelease;
            remainingAllocation -= amountPerRelease; // Decrease remaining allocation for next installments
        } else {
            installment["allocation_percentage"] = remainingAllocation; // Assign remaining allocation to the last installment to ensure total matches
        }
        installment["withdrawal_time_millis"] = installmentTimeMillis;
        installments.append(installment);
    }
    DBG_INFO << "[helper::generateInstallmentSchedule] installments:" << installments;
    return installments;
}

uint64_t helper::getTimestampFromDateTime(const QString &dateTimeStr) {
    return qUtils::getTimestampFromDateTime(dateTimeStr, TimezoneManager::instance().timezone());
}

QString helper::getDateFromTimestamp(qint64 timestamp) {
    return qUtils::getDateFromTimestamp(timestamp, TimezoneManager::instance().timezone());
}

QString helper::getTimeFromTimestamp(qint64 timestamp) {
    return qUtils::getTimeFromTimestamp(timestamp, TimezoneManager::instance().timezone());
}

QStringList helper::colorStages() {
    return {"#4CAF50", "#7B3FF2", "#FFD700", "#FF6B6B", "#00BCD4", "#FF9800", "#9C27B0", "#00897B", "#F44336", "#3F51B5"};
}
QStringList helper::colorBeneficiaries() {
    return {"#2C3E50", "#F5A17D", "#F4D03F", "#FF6B6B", "#00BCD4", "#E91E63", "#00E676", "#FF5722", "#673AB7", "#00ACC1"};
}

QVariantMap helper::addStage(int stageIndex) {
    auto colorList = helper::colorStages();
    QVariantMap newStage;
    newStage["index"] = stageIndex;
    newStage["name"] = QString("Stage %1").arg(stageIndex);
    newStage["percentage"] = 100;
    newStage["color"] = colorList[(stageIndex - 1) % colorList.size()];

    QDateTime dt = QDateTime::currentDateTime();
    dt = dt.addYears(2); // Default to 2 years in the future for new stage
    QString formattedDate = dt.toString("MM/dd/yyyy");
    QString formattedTime = dt.toString("HH:mm");

    auto repeatInterval = "YEAR";
    auto repeatCount = 1;
    auto amountPerRelease = 5;
    QJsonObject tmpStage;
    tmpStage["first_withdrawal_time_millis"] = dt.toMSecsSinceEpoch();
    tmpStage["repeat_interval"] = repeatInterval;
    tmpStage["repeat_interval_count"] = repeatCount;
    tmpStage["amount_per_release_percentage"] = amountPerRelease;
    tmpStage["total_stage_allocation_percentage"] = 100;
    QJsonArray installments = helper::generateInstallmentSchedule(tmpStage.toVariantMap());
    newStage["firstWithdrawalDate"] = formattedDate;
    newStage["firstWithdrawalTime"] = formattedTime;
    newStage["amountPerRelease"] = amountPerRelease;
    newStage["repeatInterval"] = repeatInterval;
    newStage["repeatEvery"] = repeatCount;

    QString releaseInfo;
    if (repeatInterval == "DAY") {
        releaseInfo = QString("%1% every %2 days (%3 installments)").arg(amountPerRelease).arg(repeatCount).arg(installments.size());
    } else if (repeatInterval == "MONTH") {
        int months = repeatCount;
        int years = months / 12;
        releaseInfo = QString("%1% every %2 %3 (%4 installments)")
                          .arg(amountPerRelease)
                          .arg(years > 0 ? years : months)
                          .arg(years > 0 ? "yrs" : "months")
                          .arg(installments.size());
    } else if (repeatInterval == "YEAR") {
        releaseInfo = QString("%1% annually (%2 installments)").arg(amountPerRelease).arg(installments.size());
    }

    newStage["releaseInfo"] = releaseInfo;
    newStage["isExpanded"] = false;
    QVariantList displayInstallments = helper::convertInstallmentsDisplayData(installments);
    newStage["displayInstallments"] = QVariant::fromValue(displayInstallments);
    
    return newStage;
}

QVariantList helper::convertStagesData(const QJsonArray &stagesArray) {
    QVariantList convertedStages;

    // Color palette for stages
    QStringList colors = helper::colorStages();

    for (int i = 0; i < stagesArray.size(); ++i) {
        QJsonObject stageObj = stagesArray[i].toObject();

        int totalAllocation = stageObj["total_stage_allocation_percentage"].toInt();
        qint64 firstWithdrawalMs = stageObj["first_withdrawal_time_millis"].toVariant().toLongLong() / 1000; // Convert ms to s for timestamp
        QString repeatInterval = stageObj["repeat_interval"].toString();
        int repeatCount = stageObj["repeat_interval_count"].toInt();
        QJsonArray installments = stageObj["expanded_installments"].toArray();
        int amountPerRelease = stageObj["amount_per_release_percentage"].toInt();

        // Format first withdrawal date
        QString formattedDate = helper::getDateFromTimestamp(firstWithdrawalMs);
        QString formattedTime = helper::getTimeFromTimestamp(firstWithdrawalMs);

        // Generate release info string
        QString releaseInfo;
        QString repeatUnit;
        if (repeatInterval == "DAY") {
            releaseInfo = QString("%1% every %2 days (%3 installments)").arg(amountPerRelease).arg(repeatCount).arg(installments.size());
            repeatUnit = "days";
        } else if (repeatInterval == "MONTH") {
            int months = repeatCount;
            int years = months / 12;
            releaseInfo = QString("%1% every %2 %3 (%4 installments)")
                              .arg(amountPerRelease)
                              .arg(years > 0 ? years : months)
                              .arg(years > 0 ? "yrs" : "months")
                              .arg(installments.size());
            repeatUnit = years > 0 ? "yrs" : "months";
        } else if (repeatInterval == "YEAR") {
            releaseInfo = QString("%1% annually (%2 installments)").arg(amountPerRelease).arg(installments.size());
            repeatUnit = "years";
        }

        QVariantList installmentList;
        for (const QJsonValue &installmentVal : installments) {
            QJsonObject installmentObj = installmentVal.toObject();
            QVariantMap installmentMap;
            installmentMap["index"] = installmentObj["index"].toInt();
            qint64 withdrawalMs = installmentObj["withdrawal_time_millis"].toVariant().toLongLong() / 1000; // Convert ms to s for timestamp
            installmentMap["withdrawal_datetime"] = helper::getDateFromTimestamp(withdrawalMs);
            installmentMap["allocation_percentage"] = installmentObj["allocation_percentage"].toInt();
            installmentList.append(installmentMap);
        }

        // Convert installments to display format for QML
        QVariantList displayInstallments = convertInstallmentsDisplayData(installments);

        QVariantMap stageMap = stageObj.toVariantMap();
        stageMap["name"] = QString("Stage %1").arg(i + 1);
        stageMap["percentage"] = QString("%1").arg(totalAllocation);
        stageMap["color"] = colors[i % colors.size()];
        stageMap["firstWithdrawalDate"] = formattedDate;
        stageMap["firstWithdrawalTime"] = formattedTime;
        stageMap["releaseInfo"] = releaseInfo;
        stageMap["isExpanded"] = false;
        stageMap["installments"] = QVariant::fromValue(installmentList);
        stageMap["displayInstallments"] = QVariant::fromValue(displayInstallments);
        stageMap["amountPerRelease"] = amountPerRelease;
        stageMap["repeatInterval"] = repeatInterval;
        stageMap["repeatEvery"] = repeatCount;

        convertedStages.append(stageMap);
    }
    return convertedStages;
}

QVariantList helper::convertTimelineStagesData(const QVariantList &stagesData) {
    QVariantList timelineStages;
    int totalPercentage = 0;
    for (const QVariant &stageVar : stagesData) {
        QVariantMap stageMap = stageVar.toMap();
        QVariantMap timelineStage;
        timelineStage["label"] = stageMap["name"];
        timelineStage["date"] = stageMap["firstWithdrawalDate"];
        timelineStage["color"] = stageMap["color"];
        timelineStage["percentage"] = stageMap["percentage"];
        timelineStages.append(timelineStage);
        totalPercentage += stageMap["percentage"].toInt();
    }
    // Append an "Unallocated" segment for any remaining percentage not yet assigned to a stage.
    // When total == 100 the segment is omitted (remaining == 0). This allows the timeline bar
    // to visually communicate how much of the inheritance is still unassigned.
    int remaining = 100 - totalPercentage;
    if (remaining > 0) {
        QVariantMap unallocated;
        unallocated["label"] = QString("Unallocated");
        unallocated["date"] = QString("");
        unallocated["color"] = QString("#E0E0E0");
        unallocated["percentage"] = remaining;
        timelineStages.append(unallocated);
    }
    return timelineStages;
}

QVariantList helper::convertBeneficiariesData(const QJsonArray &beneficiariesArray) {
    QVariantList convertedBeneficiaries;
    QStringList colors = helper::colorBeneficiaries();

    for (int i = 0; i < beneficiariesArray.size(); ++i) {
        QJsonObject beneficiaryObj = beneficiariesArray[i].toObject();
        QString email = beneficiaryObj["email"].toString();
        QString bufferApplyOn = beneficiaryObj["buffer_apply_on"].toString();
        QJsonObject bufferPeriod = beneficiaryObj["buffer_period"].toObject();
        DBG_INFO << "[buffer_period debug] email:" << email << "raw buffer_period:" << bufferPeriod << "buffer_apply_on:" << bufferApplyOn;
        // Normalize buffer_period: server may omit id/display_name; reconstruct from interval fields
        if (bufferPeriod.isEmpty()) {
            bufferPeriod["id"] = "";
            bufferPeriod["display_name"] = "";
        } else {
            if (!bufferPeriod.contains("id") || bufferPeriod["id"].toString().isEmpty()) {
                int intervalCount = bufferPeriod.value("interval_count").toInt();
                QString interval = bufferPeriod.value("interval").toString();
                bufferPeriod["id"] = (intervalCount > 0 && !interval.isEmpty()) ? QString("%1_%2").arg(intervalCount).arg(interval.toLower()) : QString("");
            }
            if (!bufferPeriod.contains("display_name") || bufferPeriod["display_name"].toString().isEmpty()) {
                int intervalCount = bufferPeriod.value("interval_count").toInt();
                QString interval = bufferPeriod.value("interval").toString();
                if (intervalCount > 0 && !interval.isEmpty()) {
                    QString unit;
                    if (interval == "DAY")
                        unit = intervalCount == 1 ? "day" : "days";
                    else if (interval == "MONTH")
                        unit = intervalCount == 1 ? "month" : "months";
                    else if (interval == "YEAR")
                        unit = intervalCount == 1 ? "year" : "years";
                    else
                        unit = interval.toLower();
                    bufferPeriod["display_name"] = QString("%1 %2").arg(intervalCount).arg(unit);
                } else {
                    bufferPeriod["display_name"] = "";
                }
            }
        }
        // Convert stages for this beneficiary using helper functions
        QJsonArray stagesArray = beneficiaryObj["stages"].toArray();
        QVariantList stagesData = convertStagesData(stagesArray);
        QVariantList convertedStages = convertTimelineStagesData(stagesData);

        // Get firstWithdrawalDate from the first converted stage
        QString firstWithdrawalDate;
        if (!stagesData.isEmpty()) {
            firstWithdrawalDate = getFormatFirstWithdrawalDate(stagesArray);
        }

        QVariantMap beneficiaryMap;
        beneficiaryMap["email"] = email;
        beneficiaryMap["firstWithdrawalDate"] = firstWithdrawalDate;
        beneficiaryMap["buffer_apply_on"] = bufferApplyOn;
        beneficiaryMap["buffer_period"] = QVariant::fromValue(bufferPeriod);
        beneficiaryMap["stages"] = QVariant::fromValue(convertedStages);
        beneficiaryMap["note"] = beneficiaryObj["note"].toString();
        beneficiaryMap["magic"] = beneficiaryObj["magic"].toString();
        convertedBeneficiaries.append(beneficiaryMap);
    }

    return convertedBeneficiaries;
}

QVariantList helper::convertAssetAllocationData(const QJsonArray &beneficiariesArray) {
    QVariantList convertedAssetAllocations;

    // Color palette for beneficiaries
    QStringList colors = {"#2C3E50", "#F5A17D", "#F4D03F", "#FF6B6B", "#00BCD4"};

    for (int i = 0; i < beneficiariesArray.size(); ++i) {
        QJsonObject beneficiaryObj = beneficiariesArray[i].toObject();

        QString email = beneficiaryObj["email"].toString();
        int assetPercentage = beneficiaryObj["asset_percentage"].toInt();
        QString note = beneficiaryObj["note"].toString();

        QVariantMap allocationMap;
        allocationMap["email"] = email;
        allocationMap["color"] = colors[i % colors.size()];
        allocationMap["asset_percentage"] = assetPercentage;
        allocationMap["note"] = note;
        allocationMap["magic"] = beneficiaryObj["magic"].toString();
        convertedAssetAllocations.append(allocationMap);
    }

    return convertedAssetAllocations;
}

QString helper::getFormatFirstWithdrawalDate(const QJsonArray &stagesArray) {
    if (stagesArray.isEmpty()) {
        return QString();
    }

    QJsonObject firstStage = stagesArray[0].toObject();
    qint64 firstWithdrawalMs = firstStage["first_withdrawal_time_millis"].toVariant().toLongLong() / 1000; // Convert ms to s for timestamp
    if (firstWithdrawalMs <= 0) {
        return QString();
    }
    return helper::getDateFromTimestamp(firstWithdrawalMs);
}

QVariantList helper::convertInstallmentsDisplayData(const QJsonArray &installmentsArray) {
    QVariantList displayInstallments;

    QStringList ordinals = {"1st", "2nd", "3rd", "4th", "5th", "6th", "7th", "8th", "9th", "10th",
                            "11th", "12th", "13th", "14th", "15th", "16th", "17th", "18th", "19th", "20th"};

    double cumulative = 0.0; // Accumulate to produce cumulative % for display ("X% available by date")
    for (int i = 0; i < installmentsArray.size(); ++i) {
        QJsonObject installmentObj = installmentsArray[i].toObject();

        QVariantMap installmentMap;

        // Use loop position i (0-based) for ordinal — index stored in data is 1-based
        // so ordinals[index] would be off by one. Use i to always produce "1st", "2nd", etc.
        if (i < ordinals.size()) {
            installmentMap["number"] = ordinals[i];
        } else {
            installmentMap["number"] = QString("%1th").arg(i + 1);
        }

        // Cumulative percentage: "X% available by [date]" matches mobile design.
        // Each installment releases amount_per_release%; the final installment may be smaller
        // (remainder when total is not evenly divisible). Summing gives the running total
        // the beneficiary can access after each release date.
        cumulative += installmentObj["allocation_percentage"].toDouble();
        installmentMap["percentage"] = qRound(cumulative);

        // Convert withdrawal time millis to date
        qint64 withdrawalMs = installmentObj["withdrawal_time_millis"].toVariant().toLongLong() / 1000; // Convert ms to s for timestamp
        QString date = helper::getDateFromTimestamp(withdrawalMs);
        installmentMap["date"] = date;

        displayInstallments.append(installmentMap);
    }

    return displayInstallments;
}

QJsonObject helper::convertStageToJson(const QVariantMap &stageData) {
    QJsonObject stageObj;
    stageObj["total_stage_allocation_percentage"] = stageData["percentage"].toInt();
    auto dateTimeStr = stageData["firstWithdrawalDate"].toString() + " " + stageData["firstWithdrawalTime"].toString();
    stageObj["first_withdrawal_time_millis"] = (long long)helper::getTimestampFromDateTime(dateTimeStr) * 1000; // Convert s to ms for timestamp
    stageObj["repeat_interval"] = stageData["repeatInterval"].toString();
    stageObj["repeat_interval_count"] = stageData["repeatEvery"].toInt();
    stageObj["amount_per_release_percentage"] = stageData["amountPerRelease"].toInt();

    // Convert installments back to JSON array
    QVariantList installments = stageData["installments"].toList();
    QJsonArray installmentsArray;
    for (const QVariant &installmentVar : installments) {
        QVariantMap installmentMap = installmentVar.toMap();
        QJsonObject installmentObj;
        installmentObj["index"] = installmentMap["index"].toInt();
        installmentObj["withdrawal_time_millis"] =
            (long long)helper::getTimestampFromDateTime(installmentMap["withdrawal_datetime"].toString()) * 1000; // Convert s to ms for timestamp
        installmentObj["allocation_percentage"] = installmentMap["allocation_percentage"].toInt();
        installmentsArray.append(installmentObj);
    }
    stageObj["expanded_installments"] = installmentsArray;

    return stageObj;
}

QJsonArray helper::convertStagesToJsonArray(const QVariantList &stagesData) {
    QJsonArray stagesArray;
    for (const QVariant &stageVar : stagesData) {
        QVariantMap stageMap = stageVar.toMap();
        QJsonObject stageObj = convertStageToJson(stageMap);
        stagesArray.append(stageObj);
    }
    return stagesArray;
}

void helper::convertAssetAllocationsToJsonArray(QJsonArray &beneficiaries, const QVariantList &assetAllocationsData) {
    QJsonArray beneficiariesArray;

    for (const QVariant &allocationVar : assetAllocationsData) {
        bool found = false;
        for (const QJsonValue &beneficiaryVal : beneficiaries) {
            QJsonObject beneficiaryObj = beneficiaryVal.toObject();
            QVariantMap allocationMap = allocationVar.toMap();
            if (beneficiaryObj["email"].toString() == allocationMap["email"].toString()) {
                for (const auto &key : beneficiaryObj.keys()) {
                    if (allocationMap.contains(key)) {
                        beneficiaryObj[key] = QJsonValue::fromVariant(allocationMap[key]);
                    } else {
                        beneficiaryObj[key] = beneficiaryObj[key];
                    }
                }
                beneficiariesArray.append(beneficiaryObj);
                found = true;
                break; // Move to the next allocation after finding the match
            }
        }

        // If allocation not found in beneficiaries, create a new JSON object
        if (!found) {
            QJsonObject newBeneficiary =
                beneficiaries.isEmpty() ? QJsonObject() : beneficiaries[0].toObject(); // Start with the first beneficiary's data or an empty object
            QVariantMap allocationMap = allocationVar.toMap();
            for (const auto &key : newBeneficiary.keys()) {
                if (allocationMap.contains(key)) {
                    newBeneficiary[key] = QJsonValue::fromVariant(allocationMap[key]);
                } else {
                    newBeneficiary[key] = newBeneficiary[key];
                }
            }
            beneficiariesArray.append(newBeneficiary);
        }
    }

    beneficiaries = beneficiariesArray;
}

void helper::convertBeneficiariesToJsonArray(QJsonArray &beneficiaries, const QVariantList &beneficiariesData) {
    QJsonArray beneficiariesArray;

    for (const QVariant &beneficiaryVar : beneficiariesData) {
        QVariantMap beneficiaryMap = beneficiaryVar.toMap();
        bool found = false;

        // Try to find matching beneficiary in existing beneficiaries array
        for (const QJsonValue &beneficiaryVal : beneficiaries) {
            QJsonObject beneficiaryObj = beneficiaryVal.toObject();
            if (beneficiaryObj["email"].toString() == beneficiaryMap["email"].toString()) {
                for (const auto &key : beneficiaryObj.keys()) {
                    if (beneficiaryMap.contains(key)) {
                        beneficiaryObj[key] = QJsonValue::fromVariant(beneficiaryMap[key]);
                    } else {
                        beneficiaryObj[key] = beneficiaryObj[key];
                    }
                }
                beneficiariesArray.append(beneficiaryObj);
                found = true;
                break;
            }
        }

        // If no matching beneficiary found, create new JSON object with full data
        if (!found) {
            QJsonObject newBeneficiary =
                beneficiaries.isEmpty() ? QJsonObject() : beneficiaries[0].toObject(); // Start with the first beneficiary's data or an empty object
            for (const auto &key : newBeneficiary.keys()) {
                if (beneficiaryMap.contains(key)) {
                    newBeneficiary[key] = QJsonValue::fromVariant(beneficiaryMap[key]);
                } else {
                    newBeneficiary[key] = newBeneficiary[key];
                }
            }
            beneficiariesArray.append(newBeneficiary);
        }
    }

    beneficiaries = beneficiariesArray;
}

bool helper::isBeneficiaryDataValid(const QJsonArray &beneficiaries) {
    if (beneficiaries.isEmpty()) {
        return false;
    }

    QSet<QString> emailSet;
    QRegularExpression emailRegex(R"(^[^\s@]+@[^\s@]+\.[^\s@]+$)");

    for (int i = 0; i < beneficiaries.size(); ++i) {
        QJsonObject beneficiary = beneficiaries[i].toObject();

        // 1. Validate email field
        if (!beneficiary.contains("email")) {
            return false;
        }
        QString email = beneficiary["email"].toString().trimmed().toLower();
        if (email.isEmpty()) {
            return false;
        }

        // 2. Validate email format
        if (!emailRegex.match(email).hasMatch()) {
            return false;
        }

        // 3. Check for duplicate emails (normalized)
        if (emailSet.contains(email)) {
            return false;
        }
        emailSet.insert(email);

        // 4. Validate stages array
        if (!beneficiary.contains("stages")) {
            return false;
        }
        QJsonArray stagesArray = beneficiary["stages"].toArray();
        if (stagesArray.isEmpty()) {
            return false;
        }

        // 5. Validate each stage
        int totalStageAllocation = 0;
        qint64 prevStageWithdrawalTime = 0;

        for (int j = 0; j < stagesArray.size(); ++j) {
            QJsonObject stage = stagesArray[j].toObject();

            // Check first_withdrawal_time_millis
            if (!stage.contains("first_withdrawal_time_millis")) {
                return false;
            }
            qint64 firstWithdrawalTime = stage["first_withdrawal_time_millis"].toVariant().toLongLong();
            if (firstWithdrawalTime <= 0) {
                return false;
            }

            // Validate stages are in chronological order
            if (j > 0 && firstWithdrawalTime <= prevStageWithdrawalTime) {
                return false;
            }
            prevStageWithdrawalTime = firstWithdrawalTime;

            // Check repeat_interval
            if (!stage.contains("repeat_interval")) {
                return false;
            }
            QString repeatInterval = stage["repeat_interval"].toString();
            if (repeatInterval.isEmpty() || (repeatInterval != "DAY" && repeatInterval != "WEEK" && repeatInterval != "MONTH" && repeatInterval != "YEAR")) {
                return false;
            }

            // Check repeat_interval_count
            if (!stage.contains("repeat_interval_count")) {
                return false;
            }
            int repeatIntervalCount = stage["repeat_interval_count"].toInt();
            if (repeatIntervalCount <= 0) {
                return false;
            }

            // Check amount_per_release_percentage (0-100)
            if (!stage.contains("amount_per_release_percentage")) {
                return false;
            }
            int amountPerRelease = stage["amount_per_release_percentage"].toInt();
            if (amountPerRelease < 0 || amountPerRelease > 100) {
                return false;
            }

            // Check total_stage_allocation_percentage
            if (!stage.contains("total_stage_allocation_percentage")) {
                return false;
            }
            int totalAllocation = stage["total_stage_allocation_percentage"].toInt();
            if (totalAllocation <= 0 || totalAllocation > 100) {
                return false;
            }
            totalStageAllocation += totalAllocation;
        }

        // Validate total stage allocation equals 100%
        if (totalStageAllocation != 100) {
            return false;
        }
    }

    // 7. Validate cross-beneficiary asset allocation if present
    int totalAssetAllocation = 0;
    bool hasAssetPercentage = false;

    for (int i = 0; i < beneficiaries.size(); ++i) {
        QJsonObject beneficiary = beneficiaries[i].toObject();
        if (beneficiary.contains("asset_percentage")) {
            hasAssetPercentage = true;
            int assetPercentage = beneficiary["asset_percentage"].toInt();

            if (assetPercentage < 0 || assetPercentage > 100) {
                return false;
            }
            totalAssetAllocation += assetPercentage;
        }
    }

    if (hasAssetPercentage && totalAssetAllocation != 100) {
        return false;
    }

    return true;
}

} // namespace features::inheritance::offchain::flows
