#include "InheritancePlanViewModel.h"
#include "ViewsEnums.h"
#include "Premiums/QGroupWalletDummyTx.h"
#include "Premiums/QGroupWalletHealthCheck.h"
#include "Premiums/QInheritancePlan.h"
#include "core/ui/UiServices.inc"
#include "core/utils/Utils.h"
#include "features/inheritance/offchain/flows/PhasedRolloutFlow.h"
#include "generated_qml_keys.hpp"

namespace features::inheritance::common::viewmodels {
using namespace core::viewmodels;
using namespace features::inheritance::offchain::flows;

InheritancePlanViewModel::InheritancePlanViewModel(QObject *parent) : BaseViewModel(parent) {}

void InheritancePlanViewModel::onInit() {
    // GUARD_APP_MODEL_INHERITANCE_PLAN()
    // GUARD_DASHBOARD()
    // connect(ihPlan.data(), &QInheritancePlan::planInfoUpdateChanged, this, &InheritancePlanViewModel::slotUpdateInheritancePlan);
    // QJsonObject payload = dashboard->alertJson()["payload"].toObject();
    // QString dummy_transaction_id = payload["dummy_transaction_id"].toString();
    // if (!dummy_transaction_id.isEmpty()) {
    //     if (dashboard->healthPtr()) {
    //         dashboard->healthPtr()->HealthCheckPendingForTx(dummy_transaction_id);
    //     }
    // }
    slotUpdateInheritancePlan();
}

void InheritancePlanViewModel::slotUpdateInheritancePlan() {
    processAlertData();
    processWalletData();
}

void InheritancePlanViewModel::processAlertData() {
    GUARD_DASHBOARD()
    QVariantMap alertData = dashboard->alert().toMap();
    QString title = alertData.value("title").toString();
    settitle(title);
    auto payload = alertData.value("payload").toMap();
    int pending_signatures = payload.value("pending_signatures").toInt();
    setpending_signatures(pending_signatures);
}

void InheritancePlanViewModel::processWalletData() {
    GUARD_APP_MODEL_DUMMY_TX()
    QString walletName = wallet->walletName();
    auto walletType = wallet->walletType();
    setwalletName(walletName);
    auto requester_user = dummyTx->requester_user();
    setrequester_user(requester_user);
    setisOnChain(walletType == (int)nunchuk::WalletType::MINISCRIPT);
    auto inheritancePlanInfo = wallet->inheritancePlanPtr();
    auto planInfoOld = inheritancePlanInfo->planInfoOldJs();
    auto planInfoNew = inheritancePlanInfo->planInfoNewJs();
    QString old_activation_date = planInfoOld.value("activation_date").toString();
    QString new_activation_date = planInfoNew.value("activation_date").toString();
    setold_activation_date(old_activation_date);
    setnew_activation_date(new_activation_date);
    setnote(planInfoNew.value("note").toString());
    setis_note_changed(planInfoOld.value("note").toString() != planInfoNew.value("note").toString());
    {
        QJsonArray stagesArray = planInfoNew["stages"].toArray();
        QString firstWithdrawalDateNew = helper::getFormatFirstWithdrawalDate(stagesArray);
        QJsonArray oldStagesArray = planInfoOld["stages"].toArray();
        QString firstWithdrawalDateOld = helper::getFormatFirstWithdrawalDate(oldStagesArray);

        setfirstWithdrawalDate(firstWithdrawalDateNew);
        setisFirstWithdrawalDateChanged(firstWithdrawalDateOld != firstWithdrawalDateNew);
        auto buffer_apply_on = planInfoNew.value("buffer_apply_on").toString();
        setbuffer_apply_on(buffer_apply_on);
        setis_buffer_apply_on_changed(planInfoOld.value("buffer_apply_on").toString() != buffer_apply_on);

        auto buffer_period = planInfoNew["buffer_period"].toObject();
        if (!buffer_period.contains("id"))           buffer_period["id"]           = QString("");
        if (!buffer_period.contains("display_name")) buffer_period["display_name"] = QString("");
        setbuffer_period(buffer_period.toVariantMap());
        setis_buffer_period_changed(planInfoOld["buffer_period"].toObject() != buffer_period);
    }

    {
        QJsonArray oldEmails = planInfoOld.value("notification_emails").toArray();
        QJsonArray emails = planInfoNew.value("notification_emails").toArray();
        if (emails.size() > 0) {
            QStringList emailList;
            for (QJsonValue js : emails) {
                emailList.append(js.toString());
            }
            QString email = emailList.join(",");
            setdisplay_emails(email);
            setisDisplay_emailsChanged(oldEmails != emails);
        }
    }

    {
        QJsonObject oldBufferPeriod = planInfoOld["buffer_period"].toObject();
        QJsonObject bufferPeriod = planInfoNew["buffer_period"].toObject();
        if (!bufferPeriod.contains("id"))           bufferPeriod["id"]           = QString("");
        if (!bufferPeriod.contains("display_name")) bufferPeriod["display_name"] = QString("");
        setbuffer_period(bufferPeriod.toVariantMap());

        bool notifyTodayOld = oldBufferPeriod.value("enabled").toBool();
        bool notifyTodayNew = bufferPeriod.value("enabled").toBool();
        setnotifyEnabled(notifyTodayNew);
        setisNotifyEnabledChanged(notifyTodayOld != notifyTodayNew);
    }

    {
        QString distribution_method = planInfoNew.value("distribution_method").toString();
        QString beneficiary_mode = planInfoNew.value("beneficiary_mode").toString();
        QString buffer_apply_on = planInfoNew.value("buffer_apply_on").toString();
        QVariant buffer_period = planInfoNew.value("buffer_period").toVariant();
        setdistribution_method(distribution_method);
        setbeneficiary_mode(beneficiary_mode);
        setbuffer_apply_on(buffer_apply_on);
        setbuffer_period(buffer_period);
    }

    {
        QJsonArray oldBeneficiaries = planInfoOld["beneficiaries"].toArray();
        QJsonArray newBeneficiaries = planInfoNew["beneficiaries"].toArray();
        auto oldBeneficiariesArray = helper::convertBeneficiariesData(oldBeneficiaries);
        auto newBeneficiariesArray = helper::convertBeneficiariesData(newBeneficiaries);
        QVariantList beneficiariesList = qUtils::compareAndSetChanges(oldBeneficiariesArray, newBeneficiariesArray);
        setbeneficiariesData(beneficiariesList);
        auto oldAssetAllocationArray = helper::convertAssetAllocationData(oldBeneficiaries);
        auto newAssetAllocationArray = helper::convertAssetAllocationData(newBeneficiaries);
        QVariantList assetAllocationList = qUtils::compareAndSetChanges(oldAssetAllocationArray, newAssetAllocationArray);
        setassetAllocation(assetAllocationList);
    }

    {
        auto oldFallbackPolicy = planInfoOld["fallback_policy"].toObject();
        auto fallbackPolicy = planInfoNew["fallback_policy"].toObject();
        setfallbackChanged(oldFallbackPolicy != fallbackPolicy);
        setfallback_policy(fallbackPolicy.toVariantMap());

        int inactivity_interval_count = fallbackPolicy.value("inactivity_interval_count").toInt();
        setfallback_interval_count(inactivity_interval_count);
        QString fallback_interval = fallbackPolicy.value("inactivity_interval").toString();
        setfallback_interval(helper::getIntervalDisplayString(fallback_interval, inactivity_interval_count));
        auto fallback_time_Ms = fallbackPolicy.value("fallback_time_millis").toDouble() / 1000;
        QString fallback_dateTime = helper::getDateFromTimestamp(fallback_time_Ms);
        setfallback_dateTime(fallback_dateTime);
        QString fallback_type = fallbackPolicy.value("type").toString();
        setfallback_type(fallback_type);
    }

    {
        QString release_method = planInfoNew.value("release_method").toString();
        setrelease_method(release_method);
        QString old_release_method = planInfoOld.value("release_method").toString();
        setis_release_method_changed(release_method != old_release_method);
    }

    {
        QString old_timezone = planInfoOld.value("activation_timezone").toString();
        QString new_timezone = planInfoNew.value("activation_timezone").toString();
        setisTimezoneDisplayChanged(old_timezone != new_timezone);
    }
}

void InheritancePlanViewModel::processTimeData() {
    GUARD_DASHBOARD()
    auto timelockObj = dashboard->timelockObj();
    QString timezone = timelockObj.value("timezone").toString();
    settimelockDisplay(isOnChain() ? dashboard->timelockDisplay() : activation_date());
    settimezoneDisplay(isOnChain() ? timezone : activation_timezone());
}

void InheritancePlanViewModel::onSignDummyTxClicked() {
    close();
    QEventProcessor::instance()->sendEvent(E::EVT_HEALTH_CHECK_STARTING_REQUEST);
}

} // namespace features::inheritance::common::viewmodels
