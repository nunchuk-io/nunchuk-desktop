#include "EditPlatformKeyPoliciesViewModel.h"
#include "core/bridge/ExternalBridges.h"
#include "core/ui/UiServices.inc"
#include "features/signers/flows/SetupPlatformKeyPolicyFlow.h"
#include "features/transactions/flows/DummyTransactionFlow.h"
#include "generated_qml_keys.hpp"
#include "core/common/resources/AppStrings.h"

namespace features::signers::viewmodels {
using namespace features::signers::flows;

EditPlatformKeyPoliciesViewModel::EditPlatformKeyPoliciesViewModel(QObject *parent) : BaseViewModel(parent) {
    setpolicyName("Policy Name");
    setautoBroadcast(false);
    setenableCoSigningDelay(false);
    setenableSpendingLimit(false);
    setcurrency("USD");
    setbalance(0.0);
    setinterval("Daily");
    sethour(2);
    setminute(0);
    setsigningDelaySeconds(0);
}

void EditPlatformKeyPoliciesViewModel::onInit() {
    GUARD_FLOW_MANAGER();
    GUARD_SHARE_WALLETS()
    auto flow = dynamic_cast<SetupPlatformKeyPolicyFlow *>(flowMng->currentFlow());
    if (flow) {
        QVariantMap policy = flow->policy();
        setautoBroadcast(policy.value("autoBroadcast").toBool());
        setenableCoSigningDelay(policy.value("enableCoSigningDelay").toBool());
        setenableSpendingLimit(policy.value("enableSpendingLimit").toBool());
        setcurrency(policy.value("currency").toString());
        setbalance(policy.value("balance").toDouble());
        setinterval(policy.value("interval").toString());
        setsigningDelaySeconds(policy.value("signingDelaySeconds").toInt());
        int seconds = policy.value("signingDelaySeconds").toInt();
        sethour(seconds == 0 ? 2 : policy.value("hour").toInt());
        setminute(policy.value("minute").toInt());
        setpolicyName(policy.value("singleSigner_name").toString());
        if (policy.contains("single_signer_type")) {
            setsignerType(policy.value("single_signer_type").toInt());
            setsignerTag(policy.value("single_signer_tag").toString());
        } else {
            setsignerType(-1);
            setsignerTag("");
        }

        QString interval_str = policy.value("interval").toString();
        if (qUtils::strCompare(interval_str, "DAILY")) {
            interval_str = "DAY";
        } else if (qUtils::strCompare(interval_str, "WEEKLY")) {
            interval_str = "WEEK";
        } else if (qUtils::strCompare(interval_str, "MONTHLY")) {
            interval_str = "MONTH";
        } else if (qUtils::strCompare(interval_str, "YEARLY")) {
            interval_str = "YEAR";
        } else {
            interval_str = "DAY";
        }
        setintervalStr(interval_str);
    }
}

void EditPlatformKeyPoliciesViewModel::onApplyClicked() {
    GUARD_FLOW_MANAGER();
    auto flow = dynamic_cast<SetupPlatformKeyPolicyFlow *>(flowMng->currentFlow());
    if (flow) {
        QString walletId = flow->walletId();
        int hour_input = hour();
        int minute_input = minute();
        int delaySecond_intput = hour_input * 3600 + minute_input * 60;
        if (enableCoSigningDelay()) {
            setsigningDelaySeconds(delaySecond_intput);
            if (delaySecond_intput <= 0) {                
                showToast(-1, Strings.STR_QML_2149(), EWARNING::WarningType::ERROR_MSG);
                return;
            }
        } else {
            setsigningDelaySeconds(0);
        }
        QVariantMap policy_old = flow->policy();
        QVariantMap policy_new = flow->policy();
        policy_new["enableSpendingLimit"] = enableSpendingLimit();
        policy_new["autoBroadcast"] = autoBroadcast();
        policy_new["enableCoSigningDelay"] = enableCoSigningDelay();
        policy_new["signingDelaySeconds"] = signingDelaySeconds();
        policy_new["currency"] = currency(); // fixed typo: curency -> currency
        policy_new["balance"] = balance();
        policy_new["interval"] = interval();
        policy_new["hour"] = hour();
        policy_new["minute"] = minute();
        policy_new["singleSigner_name"] = policyName();

        DBG_INFO << "Old policy: " << policy_old;
        DBG_INFO << "New policy: " << policy_new;

        flow->setpolicy(policy_new); // fixed naming convention
        flow->sethasAnyChanged(true);

        DBG_INFO << "Applying policy changes for walletId: " << walletId;
        back();
    }
}

} // namespace features::signers::viewmodels
