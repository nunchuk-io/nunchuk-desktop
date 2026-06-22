#pragma once
#include "core/flow/BaseFlow.h"
#include "core/viewmodel/DefinePropertyMacros.h"
#include "features/inheritance/common/flows/InheritanceFlow.h"

namespace features::inheritance::offchain::flows {
using core::flow::FlowContext;
using namespace features::inheritance::common::flows;

enum class ConfigStep_t {
    None = 0,
    PassViaStageConfig = 1,
    PassViaBufferPeriod = 2,
    PassViaFallbackSettings = 3,
};
class PhasedRolloutFlow : public InheritanceFlow {
    Q_OBJECT
    DEFINE_QT_PROPERTY(QString, valueTimezone)
  public:
    explicit PhasedRolloutFlow(FlowContext *ctx, QObject *parent = nullptr);
    virtual ~PhasedRolloutFlow() = default;

    QString id() const override {
        return "PhasedRolloutFlow";
    }

    DEFINE_SET_GET(ConfigStep_t, current_step)
    DEFINE_SET_GET(double, activation_time_milis)

    bool checkStageCanGoNext() override;
    bool validateDateTime(const QVariantMap &firstStage, const QVariantMap &secondStage);
  public slots:
    void bind(QObject *vm) override;

  public:
    DEFINE_SET_GET(QJsonObject, beneficiarySelected)
    DEFINE_SET_GET(QVariantMap, editStageSelected)
    DEFINE_SET_GET(int, fallbackOption)
};

namespace helper {
QString getIntervalDisplayString(const QString &repeatInterval, int repeatEvery);
long long addIntervalStage(long long firstWithdrawalTimeMillis, const QString &repeatInterval, int repeatEvery);
uint64_t getTimestampFromDateTime(const QString &dateTimeStr);
QString getDateFromTimestamp(qint64 timestamp);
QString getTimeFromTimestamp(qint64 timestamp);
QStringList colorStages();
QStringList colorBeneficiaries();
QVariantMap addStage(int stageIndex);
QVariantList convertStagesData(const QJsonArray &stagesArray);
QVariantList convertTimelineStagesData(const QVariantList &stagesData);
QVariantList convertBeneficiariesData(const QJsonArray &beneficiariesArray);
QVariantList convertAssetAllocationData(const QJsonArray &beneficiariesArray);
QString getFormatFirstWithdrawalDate(const QJsonArray &stagesArray);
QVariantList convertInstallmentsDisplayData(const QJsonArray &installmentsArray);
QJsonObject convertStageToJson(const QVariantMap &stageData);
QJsonArray convertStagesToJsonArray(const QVariantList &stagesData);
void convertAssetAllocationsToJsonArray(QJsonArray &beneficiaries, const QVariantList &assetAllocationsData);
void convertBeneficiariesToJsonArray(QJsonArray &beneficiaries, const QVariantList &beneficiariesData);
bool isBeneficiaryDataValid(const QJsonArray &beneficiaries);
} // namespace helper
} // namespace features::inheritance::offchain::flows
