#include "PerKeyPlatformKeyPoliciesViewModel.h"
#include "ViewsEnums.h"
#include "core/bridge/ExternalBridges.h"
#include "core/ui/UiServices.inc"
#include "features/signers/flows/SetupPlatformKeyPolicyFlow.h"
#include "generated_qml_keys.hpp"
#include "ifaces/qUtils.h"

namespace features::signers::viewmodels {
using namespace features::signers::flows;
using namespace features::transactions::usecases;

PerKeyPlatformKeyPoliciesViewModel::PerKeyPlatformKeyPoliciesViewModel(QObject *parent) : BasePlatformKeyPoliciesViewModel(parent) {
    setisTypeChanged(false);
    setafterHours(0);
}

void PerKeyPlatformKeyPoliciesViewModel::onInit() {
    BaseViewModel::onInit();
    GUARD_FLOW_MANAGER()
    auto flow = dynamic_cast<SetupPlatformKeyPolicyFlow *>(flowMng->currentFlow());
    if (!flow) {
        DBG_ERROR << "Current flow is not SetupPlatformKeyPolicyFlow. Cannot initialize policy from sandbox.";
        return;
    }
    setpending_signatures(flow->pendingSignatures());
    if (isEntryPointGroup()) {
        initPolicyFromSandbox();
    } else if (isEntryPointWallet()) {
        initPolicyFromWalletConfig();
    } else if (isEntryPointAlert()) {
        initPolicyFromDummyTransaction();
    }
}

void PerKeyPlatformKeyPoliciesViewModel::initPolicyFromSandbox() {
    GUARD_FLOW_MANAGER()
    GUARD_SHARE_WALLETS()
    auto flow = dynamic_cast<SetupPlatformKeyPolicyFlow *>(flowMng->currentFlow());
    if (!flow) {
        DBG_ERROR << "Current flow is not SetupPlatformKeyPolicyFlow. Cannot initialize policy from sandbox.";
        return;
    }
    bool hasAnyChanged = flow->hasAnyChanged();
    if (hasAnyChanged) {
        setperKeyPolicies(flow->perKeyPolicies());
        replacePolicy(flow->policy());
    } else {
        GUARD_APP_MODEL()
        QString groupId = flow->groupId();
        if (groupId.isEmpty()) {
            return;
        }
        if (auto group_wallet = sharedWallets->sandboxListPtr()->GetGroup(groupId)) {
            QVariantList perkey_policies;
            m_groupSandbox = group_wallet->sandbox();
            if (m_groupSandbox->get_platform_key().has_value()) {
                nunchuk::WalletType wallet_type = m_groupSandbox->get_wallet_type();
                auto policy_signers = m_groupSandbox->get_platform_key().value().get_policies().get_signers();
                if (nunchuk::WalletType::MINISCRIPT == wallet_type) {
                    auto sandbox_signers = m_groupSandbox->get_named_signers();
                    QStringList platform_key_slots = {};
                    for (const auto &slot : m_groupSandbox->get_platform_key_slots()) {
                        platform_key_slots.append(QString::fromStdString(slot));
                    }
                    perkey_policies = helper::perKeyPolicyToVariantListMiniscriptWallet(sandbox_signers, policy_signers, platform_key_slots);
                } else {
                    auto sandbox_signers = m_groupSandbox->get_signers();
                    int platformkey_index = -1;
                    platformkey_index = m_groupSandbox->get_platform_key_index().has_value() ? m_groupSandbox->get_platform_key_index().value_or(-1) : -1;
                    perkey_policies = helper::perKeyPolicyToVariantListNormalWallet(sandbox_signers, policy_signers, platformkey_index);
                }
            } else {
                auto sandbox_signers = m_groupSandbox->get_signers();
                perkey_policies = helper::defaultPerkeyPolices(sandbox_signers);
            }
            setperKeyPolicies(perkey_policies);
            flow->setperKeyPolicies(perkey_policies);
        }
    }
}

void PerKeyPlatformKeyPoliciesViewModel::initPolicyFromWalletConfig() {
    GUARD_FLOW_MANAGER()
    GUARD_SHARE_WALLETS()
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
    if (walletConfig.get_platform_key().has_value()) {
        auto platformKey = walletConfig.get_platform_key().value();
        auto policies = platformKey.get_policies();
        auto wallet_signers = wallet->singleSignersAssigned()->signers();
        auto policy_signers = policies.get_signers();
        QVariantList old_perkey_policies = helper::perKeyPolicyToVariantListNormalWallet(wallet_signers, policy_signers, -1);
        bool hasAnyChanged = flow->hasAnyChanged();
        if (hasAnyChanged) {
            setperKeyPolicies(old_perkey_policies);
            replacePolicy(flow->policy());
            QVariantList new_perkey_policies = perKeyPolicies();
            QVariantList result = makeRedHighlightPolicy(old_perkey_policies, new_perkey_policies);
            setperKeyPolicies(result);
        } else {
            setperKeyPolicies(old_perkey_policies);
        }
        setisTypeChanged(policy_signers.size() == 0);
    } else {
        auto wallet_signers = wallet->singleSignersAssigned()->signers();
        QVariantList perkey_policies = helper::defaultPerkeyPolices(wallet_signers);
        setperKeyPolicies(perkey_policies);        
    }
}

void PerKeyPlatformKeyPoliciesViewModel::initPolicyFromDummyTransaction() {
    GUARD_FLOW_MANAGER()
    auto flow = dynamic_cast<SetupPlatformKeyPolicyFlow *>(flowMng->currentFlow());
    if (!flow) {
        DBG_ERROR << "Current flow is not SetupPlatformKeyPolicyFlow. Cannot initialize policy from dummy transaction.";
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
    GUARD_WALLET(flow->walletId())
    if (!wallet) {
        DBG_ERROR << "Wallet not found for walletId: " << flow->walletId();
        return;
    }
    auto nunWallet = wallet->nunchukWallet();
    auto payload = payload_opt.value();
    auto old_policies = payload.get_old_policies();
    auto new_policies = payload.get_new_policies();
    setisTypeChanged(false);
    bool hasAnyChanged = flow->hasAnyChanged();
    if (old_policies.get_signers().size() > 0 && new_policies.get_signers().size() > 0) {
        QVariantList old_perkey_policies = helper::perKeyPolicyToVariantListNormalWallet(nunWallet.get_signers(), old_policies.get_signers(), -1);
        if (hasAnyChanged) {
            setperKeyPolicies(old_perkey_policies);
            replacePolicy(flow->policy());
            QVariantList new_perkey_policies = perKeyPolicies();
            QVariantList result = makeRedHighlightPolicy(old_perkey_policies, new_perkey_policies);
            setperKeyPolicies(result);
        } else {
            QVariantList new_perkey_policies = helper::perKeyPolicyToVariantListNormalWallet(nunWallet.get_signers(), new_policies.get_signers(), -1);
            QVariantList result_perkey_policies_compare_old_new = makeRedHighlightPolicy(old_perkey_policies, new_perkey_policies);
            setperKeyPolicies(result_perkey_policies_compare_old_new);
        }
    } else if (old_policies.get_signers().size() == 0) {
        setisTypeChanged(true);
        QVariantList old_perkey_policies = helper::perKeyPolicyToVariantListNormalWallet(nunWallet.get_signers(), new_policies.get_signers(), -1);
        if (hasAnyChanged) {
            setperKeyPolicies(old_perkey_policies);
            replacePolicy(flow->policy());
        } else {
            setperKeyPolicies(old_perkey_policies);
        }
    }
}

QVariantList PerKeyPlatformKeyPoliciesViewModel::makeRedHighlightPolicy(const QVariantList &old_perkey_policies, const QVariantList &new_perkey_policies) {
    QVariantList result_perkey_policies_compare_old_new;
    for (int i = 0; i < old_perkey_policies.size(); i++) {
        QVariantMap old_policy = old_perkey_policies[i].toMap();
        for (int j = 0; j < new_perkey_policies.size(); j++) {
            QVariantMap new_policy = new_perkey_policies[j].toMap();
            if (old_policy.value("singleSigner_masterFingerPrint").toString() == new_policy.value("singleSigner_masterFingerPrint").toString()) {
                QVariantMap result_policy_compare_old_new = new_policy;
                for (auto it = old_policy.begin(); it != old_policy.end(); ++it) {
                    QString key = it.key();
                    QVariant old_value = it.value();
                    QVariant new_value = new_policy.value(key, QVariant());
                    if (old_value != new_value) {
                        result_policy_compare_old_new[key] = new_value;
                        result_policy_compare_old_new[key + "Changed"] = true;
                    } else {
                        result_policy_compare_old_new[key + "Changed"] = false;
                    }
                }
                result_perkey_policies_compare_old_new.append(result_policy_compare_old_new);
                break;
            }
        }
    }
    return result_perkey_policies_compare_old_new;
}

void PerKeyPlatformKeyPoliciesViewModel::onEditClicked() {
    GUARD_SUB_SCREEN_MANAGER()
    GUARD_FLOW_MANAGER()
    auto flow = dynamic_cast<SetupPlatformKeyPolicyFlow *>(flowMng->currentFlow());
    if (flow) {
        flow->sethasAnyChanged(false);
    }
    subMng->show(qml::features::signers::qplatformkeypolicies);
}

void PerKeyPlatformKeyPoliciesViewModel::onEditPolicyClicked(QString xfp) {
    GUARD_SUB_SCREEN_MANAGER()
    GUARD_FLOW_MANAGER()
    auto policy = filterSignerPolicy(xfp);
    auto flow = dynamic_cast<SetupPlatformKeyPolicyFlow *>(flowMng->currentFlow());
    if (flow) {
        flow->sethasAnyChanged(false);
        flow->setperKeyPolicies(perKeyPolicies());
        flow->setpolicy(policy);
    }
    subMng->show(qml::features::signers::qeditplatformkeypolicies);
}

void PerKeyPlatformKeyPoliciesViewModel::onRemovePlatformKeyClicked() {
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
                m_groupSandbox = sanbox;
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

void PerKeyPlatformKeyPoliciesViewModel::onContinueToSaveChangesClicked() {
    GUARD_FLOW_MANAGER()
    GUARD_SUB_SCREEN_MANAGER()
    auto flow = dynamic_cast<SetupPlatformKeyPolicyFlow *>(flowMng->currentFlow());
    if (flow) {
        bool ret = flow->SetGroupPerkeyPlatformKeyPolicies(perKeyPolicies());
        if (ret) {
            emit showToast(0, "Platform key policies saved.", EWARNING::WarningType::SUCCESS_MSG);
            close();
        }
    }
}

void PerKeyPlatformKeyPoliciesViewModel::onApplyClicked() {
    GUARD_FLOW_MANAGER()
    GUARD_SUB_SCREEN_MANAGER()
    auto flow = dynamic_cast<SetupPlatformKeyPolicyFlow *>(flowMng->currentFlow());
    if (flow) {
        flow->sethasAnyChanged(false);
        flow->setperKeyPolicies(perKeyPolicies());
        flow->PreviewGroupPlatformKeyPolicyUpdate();
    }
}
void PerKeyPlatformKeyPoliciesViewModel::onDiscardChangesClicked() {
    cancelDummyTransaction();
}

void PerKeyPlatformKeyPoliciesViewModel::onContinueSignaturePendingClicked() {
    GUARD_SUB_SCREEN_MANAGER()
    GUARD_FLOW_MANAGER()
    auto flow = dynamic_cast<SetupPlatformKeyPolicyFlow *>(flowMng->currentFlow());
    if (flow) {
        flow->setperKeyPolicies(perKeyPolicies());
    }
    subMng->show(qml::features::transactions::qsignaturesrequired);
}

void PerKeyPlatformKeyPoliciesViewModel::slotConfirmDummyTransaction() {
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

void PerKeyPlatformKeyPoliciesViewModel::onSignClicked() {
    GUARD_SUB_SCREEN_MANAGER()
    subMng->show(qml::features::transactions::qsignaturesrequired);
}

QVariantMap PerKeyPlatformKeyPoliciesViewModel::filterSignerPolicy(QString xfp) {
    for (const auto &policy : perKeyPolicies()) {
        auto policyMap = policy.toMap();
        if (policyMap.value("singleSigner_masterFingerPrint").toString() == xfp) {
            return policyMap;
        }
    }
    return QVariantMap();
}

void PerKeyPlatformKeyPoliciesViewModel::replacePolicy(const QVariantMap &newPolicy) {
    if (newPolicy.isEmpty()) {
        return;
    }
    const QString xfp = newPolicy.value("singleSigner_masterFingerPrint").toString().trimmed();
    QVariantList currentPolicies = perKeyPolicies();
    for (int i = 0; i < currentPolicies.size(); ++i) {
        const QVariantMap policyMap = currentPolicies[i].toMap();
        if (qUtils::strCompare(policyMap.value("singleSigner_masterFingerPrint").toString(), xfp)) {
            currentPolicies[i] = newPolicy;
            break;
        }
    }
    setperKeyPolicies(currentPolicies);
}

void PerKeyPlatformKeyPoliciesViewModel::cancelDummyTransaction() {
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
