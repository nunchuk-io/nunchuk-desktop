#include "GlobalPlatformKeyPoliciesViewModel.h"
#include "ViewsEnums.h"
#include "core/common/resources/AppStrings.h"
#include "core/ui/UiServices.inc"
#include "features/signers/flows/SetupPlatformKeyPolicyFlow.h"
#include "generated_qml_keys.hpp"

namespace features::signers::viewmodels {
using namespace features::signers::flows;
using namespace features::transactions::usecases;

GlobalPlatformKeyPoliciesViewModel::GlobalPlatformKeyPoliciesViewModel(QObject *parent) : BasePlatformKeyPoliciesViewModel(parent) {
    QVariantMap default_policy = helper::defaultGlobalPolicy();
    setglobalPolicy(default_policy);
    setisTypeChanged(false);
    setafterHours(0);
}

void GlobalPlatformKeyPoliciesViewModel::onInit() {
    BaseViewModel::onInit();

    GUARD_FLOW_MANAGER()
    auto flow = dynamic_cast<SetupPlatformKeyPolicyFlow *>(flowMng->currentFlow());
    if (!flow) {
        DBG_ERROR << "Current flow is not SetupPlatformKeyPolicyFlow. Cannot initialize policy from sandbox.";
        return;
    }
    flow->setperKeyPolicies({});
    setpending_signatures(flow->pendingSignatures());
    if (isEntryPointGroup()) {
        initPolicyFromSandbox();
    } else if (isEntryPointWallet()) {
        initPolicyFromWalletConfig();
    } else if (isEntryPointAlert()) {
        initPolicyFromDummyTransaction();
    }
}

void GlobalPlatformKeyPoliciesViewModel::initPolicyFromSandbox() {
    GUARD_FLOW_MANAGER()
    auto flow = dynamic_cast<SetupPlatformKeyPolicyFlow *>(flowMng->currentFlow());
    if (!flow) {
        DBG_ERROR << "Current flow is not SetupPlatformKeyPolicyFlow. Cannot initialize policy from sandbox.";
        return;
    }
    bool hasAnyChanged = flow->hasAnyChanged();
    if (hasAnyChanged) {
        setglobalPolicy(flow->policy());
    } else {
        QString groupId = flow->groupId();
        if (groupId.isEmpty()) {
            return;
        }
        GUARD_SHARE_WALLETS()
        if (auto group_wallet = sharedWallets->sandboxListPtr()->GetGroup(groupId)) {
            nunchuk::GroupSandbox sanbox = group_wallet->sandbox();
            if (sanbox.get_platform_key().has_value()) {
                auto policies = sanbox.get_platform_key().value().get_policies();
                if (policies.get_global().has_value()) {
                    nunchuk::GroupPlatformKeyPolicy global_policies = policies.get_global().value();
                    QVariantMap platform_policies = helper::platformKeyPolicyToVariantMap(global_policies);
                    setglobalPolicy(platform_policies);
                    flow->setpolicy(platform_policies);
                    flow->sethasAnyChanged(false);
                    return;
                }
            }
        } else {
            QVariantMap default_policy = helper::defaultGlobalPolicy();
            setglobalPolicy(default_policy);
            flow->setpolicy(default_policy);
            flow->sethasAnyChanged(true);
        }
    }
}

void GlobalPlatformKeyPoliciesViewModel::initPolicyFromWalletConfig() {
    DBG_INFO;
    GUARD_FLOW_MANAGER()
    auto flow = dynamic_cast<SetupPlatformKeyPolicyFlow *>(flowMng->currentFlow());
    if (!flow) {
        DBG_ERROR << "Current flow is not SetupPlatformKeyPolicyFlow. Cannot initialize policy from sandbox.";
        return;
    }
    GUARD_WALLET(flow->walletId())
    if (!wallet) {
        DBG_ERROR << "Wallet not found for walletId: " << flow->walletId();
        return;
    }
    wallet->GetGroupWalletConfig();
    auto walletConfig = wallet->nunchukConfig();
    if (!walletConfig.get_platform_key().has_value()) {
        return;
    }
    auto platformKey = walletConfig.get_platform_key().value();
    auto policies = platformKey.get_policies();
    bool has_global_policies = policies.get_global().has_value();
    QVariantMap old_platform_policies = has_global_policies ? helper::platformKeyPolicyToVariantMap(policies.get_global().value()) : helper::defaultGlobalPolicy();
    bool hasAnyChanged = flow->hasAnyChanged();
    if (hasAnyChanged) {
        QVariantMap result_global_compare_old_new = makeRedHighlightPolicy(old_platform_policies, flow->policy());
        setglobalPolicy(result_global_compare_old_new);
    } else {
        setglobalPolicy(old_platform_policies);
    }
    setisTypeChanged(policies.get_signers().size() > 0);
}

void GlobalPlatformKeyPoliciesViewModel::initPolicyFromDummyTransaction() {
    GUARD_FLOW_MANAGER()
    auto flow = dynamic_cast<SetupPlatformKeyPolicyFlow *>(flowMng->currentFlow());
    if (!flow) {
        DBG_ERROR << "Current flow is not SetupPlatformKeyPolicyFlow. Cannot initialize policy from sandbox.";
        return;
    }
    auto dummyTx = flow->dummyTx();
    if (dummyTx.get_id().empty()) {
        return;
    }
    auto payload_opt = dummyTx.get_payload();
    if (!payload_opt.has_value()) {
        return;
    }
    auto payload = payload_opt.value();
    auto old_policies = payload.get_old_policies();
    auto new_policies = payload.get_new_policies();
    auto old_global_policies_opt = old_policies.get_global();
    auto old_signer_policies = old_policies.get_signers();
    QVariantMap old_platform_policies, new_platform_policies;
    if (old_global_policies_opt.has_value()) {
        old_platform_policies = helper::platformKeyPolicyToVariantMap(old_global_policies_opt.value());
    }
    auto new_global_policies_opt = new_policies.get_global();
    if (new_global_policies_opt.has_value()) {
        new_platform_policies = helper::platformKeyPolicyToVariantMap(new_global_policies_opt.value());
    }

    setisTypeChanged(false);
    if (old_signer_policies.size() > 0) {
        setisTypeChanged(true);
        if (new_platform_policies.isEmpty()) {
            if (flow->policy().isEmpty()) {
                new_platform_policies = helper::defaultGlobalPolicy();
            } else {
                new_platform_policies = flow->policy();
            }
        }
        setglobalPolicy(new_platform_policies);
    } else {
        bool hasAnyChanged = flow->hasAnyChanged();
        if (hasAnyChanged) {
            QVariantMap result_global_compare_old_new = makeRedHighlightPolicy(old_platform_policies, flow->policy());
            setglobalPolicy(result_global_compare_old_new);
        } else {
            QVariantMap result_global_compare_old_new = makeRedHighlightPolicy(old_platform_policies, new_platform_policies);
            setglobalPolicy(result_global_compare_old_new);
        }
    }
}

QVariantMap GlobalPlatformKeyPoliciesViewModel::makeRedHighlightPolicy(const QVariantMap &old_global_policy, const QVariantMap &new_global_policy) {
    QVariantMap result_policy_compare_old_new = new_global_policy;
    for (auto it = old_global_policy.begin(); it != old_global_policy.end(); ++it) {
        QString key = it.key();
        QVariant old_value = it.value();
        QVariant new_value = new_global_policy.value(key, QVariant());
        if (old_value != new_value) {
            result_policy_compare_old_new[key] = new_value;
            result_policy_compare_old_new[key + "Changed"] = true;
        } else {
            result_policy_compare_old_new[key + "Changed"] = false;
        }
    }
    return result_policy_compare_old_new;
}

void GlobalPlatformKeyPoliciesViewModel::onEditClicked() {
    GUARD_SUB_SCREEN_MANAGER()
    GUARD_FLOW_MANAGER()
    auto flow = dynamic_cast<SetupPlatformKeyPolicyFlow *>(flowMng->currentFlow());
    if (flow) {
        flow->sethasAnyChanged(false);
    }
    subMng->show(qml::features::signers::qplatformkeypolicies);
}

void GlobalPlatformKeyPoliciesViewModel::onEditPolicyClicked() {
    GUARD_SUB_SCREEN_MANAGER()
    GUARD_FLOW_MANAGER()
    auto flow = dynamic_cast<SetupPlatformKeyPolicyFlow *>(flowMng->currentFlow());
    if (flow) {
        flow->sethasAnyChanged(false);
        flow->setpolicy(globalPolicy());
        flow->setglobalPolicy(globalPolicy());
    }
    subMng->show(qml::features::signers::qeditplatformkeypolicies);
}

void GlobalPlatformKeyPoliciesViewModel::onRemovePlatformKeyClicked() {
    GUARD_FLOW_MANAGER()
    GUARD_SHARE_WALLETS()
    auto flow = dynamic_cast<SetupPlatformKeyPolicyFlow *>(flowMng->currentFlow());
    if (flow) {
        QString groupId = flow->groupId();
        QWarningMessage msg;
        nunchuk::GroupSandbox sanbox = bridge::DisableGroupPlatformKey(groupId, msg);
        if ((int)EWARNING::WarningType::NONE_MSG == msg.type()) {
            GUARD_APP_MODEL()
            if (auto group_wallet = sharedWallets->sandboxListPtr()->GetGroup(groupId)) {
                group_wallet->setScreenFlow("setup-group-wallet");
                group_wallet->setSandbox(sanbox);
                close();
                QJsonObject json;
                json["type"] = "setup-group-wallet";
                QEventProcessor::instance()->sendEvent(E::EVT_SETUP_GROUP_WALLET_REQUEST, json);
            }
        } else {
            emit showToast(msg.code(), msg.what(), (EWARNING::WarningType)msg.type());
        }
    }
}

void GlobalPlatformKeyPoliciesViewModel::onContinueToSaveChangesClicked() {
    GUARD_FLOW_MANAGER()
    GUARD_SUB_SCREEN_MANAGER()
    auto flow = dynamic_cast<SetupPlatformKeyPolicyFlow *>(flowMng->currentFlow());
    DBG_INFO << "onContinueToSaveChangesClicked, flow: " << (void *)flow;
    if (flow) {
        bool ret = flow->SetGroupGlobalPlatformKeyPolicies(globalPolicy());
        if (ret) {
            emit showToast(0, "Platform key policies saved.", EWARNING::WarningType::SUCCESS_MSG);
            close();
        }
    }
}

void GlobalPlatformKeyPoliciesViewModel::onApplyClicked() {
    GUARD_FLOW_MANAGER()
    GUARD_SUB_SCREEN_MANAGER()
    auto flow = dynamic_cast<SetupPlatformKeyPolicyFlow *>(flowMng->currentFlow());
    DBG_INFO << "onApplyClicked, flow: " << (void *)flow;
    if (flow) {
        flow->sethasAnyChanged(false);
        flow->setpolicy(globalPolicy());
        flow->setglobalPolicy(globalPolicy());
        flow->PreviewGroupPlatformKeyPolicyUpdate();
    }
}

void GlobalPlatformKeyPoliciesViewModel::onDiscardChangesClicked() {
    cancelDummyTransaction();
}
void GlobalPlatformKeyPoliciesViewModel::onContinueSignaturePendingClicked() {
    GUARD_SUB_SCREEN_MANAGER()
    subMng->show(qml::features::transactions::qsignaturesrequired);
}

void GlobalPlatformKeyPoliciesViewModel::slotConfirmDummyTransaction() {
    GUARD_FLOW_MANAGER()
    auto flow = dynamic_cast<SetupPlatformKeyPolicyFlow *>(flowMng->currentFlow());
    if (flow) {
        setafterHours(flow->afterHours());
        DBG_INFO << "requiresDummyTransaction: " << flow->requiresDummyTransaction();
        if (flow->requiresDummyTransaction()) {
            emit notifyDummyTransactionRequired();
        } else {
            emit notifySecurityDelayRequired();
        }
    }
}

void GlobalPlatformKeyPoliciesViewModel::onSignClicked() {
    GUARD_SUB_SCREEN_MANAGER()
    subMng->show(qml::features::transactions::qsignaturesrequired);
}

void GlobalPlatformKeyPoliciesViewModel::cancelDummyTransaction() {
    GUARD_FLOW_MANAGER()
    auto flow = dynamic_cast<SetupPlatformKeyPolicyFlow *>(flowMng->currentFlow());
    if (!flow) {
        DBG_ERROR << "Current flow is not SetupPlatformKeyPolicyFlow";
        return;
    }

    auto dummyTx = flow->dummyTx();
    if (dummyTx.get_id().empty()) {
        return;
    }

    CancelGroupDummyTransactionInput input;
    input.wallet_id = QString::fromStdString(dummyTx.get_wallet_id());
    input.dummy_transaction_id = QString::fromStdString(dummyTx.get_id());

    m_cancelGroupDummyTransactionUC.executeAsync(input, [this](core::usecase::Result<CancelGroupDummyTransactionResult> result) {
        if (result.isSuccess()) {
            // Handle success case
            emit showToast(0, "Dummy transaction cancelled successfully", EWARNING::WarningType::SUCCESS_MSG);
            GUARD_APP_MODEL()
            if (auto wallet = appModel->walletInfoPtr()) {
                if (auto dashboard = wallet->dashboard()) {
                    dashboard->GetAlertsInfo();
                    appModel->startReloadWallets();
                }
            }
            close();
        } else {
            // Handle error case
            emit showToast(result.code(), result.error(), EWARNING::WarningType::ERROR_MSG);
        }
    });
}

} // namespace features::signers::viewmodels
