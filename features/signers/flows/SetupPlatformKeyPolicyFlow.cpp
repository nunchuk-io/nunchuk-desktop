#include "SetupPlatformKeyPolicyFlow.h"
#include "Premiums/QSharedWallets.h"
#include "ViewsEnums.h"
#include "core/bridge/ExternalBridges.h"
#include "core/common/resources/AppStrings.h"
#include "core/ui/UiServices.inc"
#include "core/utils/Utils.h"
#include "features/signers/viewmodels/GlobalPlatformKeyPoliciesViewModel.h"
#include "features/signers/viewmodels/PerKeyPlatformKeyPoliciesViewModel.h"
#include "features/signers/viewmodels/PlatformKeyPoliciesViewModel.h"
#include "features/transactions/viewmodels/SignaturesRequiredViewModel.h"
#include "generated_qml_keys.hpp"
#include <QPointer>

namespace features::signers::flows {
using namespace features::signers::viewmodels;
using namespace features::transactions::viewmodels;
using namespace features::transactions::usecases;

SetupPlatformKeyPolicyFlow::SetupPlatformKeyPolicyFlow(FlowContext *ctx, QObject *parent) : core::flow::BaseFlow(ctx, parent) {
    sethasAnyChanged(false);
}

bool SetupPlatformKeyPolicyFlow::SetGroupGlobalPlatformKeyPolicies(const QVariantMap &policy) {
    nunchuk::GroupPlatformKeyPolicies policies = helper::globalPolicyMapToPolicies(policy);
    QWarningMessage msg;
    nunchuk::GroupSandbox sanbox = bridge::SetGroupPlatformKeyPolicies(groupId(), policies, msg);
    if ((int)EWARNING::WarningType::NONE_MSG == msg.type()) {
        GUARD_SHARE_WALLETS(false)
        if (auto group_wallet = sharedWallets->sandboxListPtr()->GetGroup(groupId())) {
            group_wallet->setScreenFlow("setup-group-wallet");
            group_wallet->setSandbox(sanbox);
            QJsonObject json;
            json["type"] = "setup-group-wallet";
            QEventProcessor::instance()->sendEvent(E::EVT_SETUP_GROUP_WALLET_REQUEST, json);
            return true;
        }
    } else {
        emit showToast(msg.code(), msg.what(), (EWARNING::WarningType)msg.type());
    }
    return false;
}

bool SetupPlatformKeyPolicyFlow::SetGroupPerkeyPlatformKeyPolicies(const QVariantList &perkey_policies) {
    DBG_INFO << perkey_policies;
    nunchuk::GroupPlatformKeyPolicies policies = helper::perkeyPolicyListToPolicies(perkey_policies);
    QWarningMessage msg;
    nunchuk::GroupSandbox sanbox = bridge::SetGroupPlatformKeyPolicies(groupId(), policies, msg);
    if ((int)EWARNING::WarningType::NONE_MSG == msg.type()) {
        GUARD_SHARE_WALLETS(false)
        if (auto group_wallet = sharedWallets->sandboxListPtr()->GetGroup(groupId())) {
            group_wallet->setScreenFlow("setup-group-wallet");
            group_wallet->setSandbox(sanbox);

            QJsonObject json;
            json["type"] = "setup-group-wallet";
            QEventProcessor::instance()->sendEvent(E::EVT_SETUP_GROUP_WALLET_REQUEST, json);
            return true;
        }
    } else {
        emit showToast(msg.code(), msg.what(), (EWARNING::WarningType)msg.type());
    }

    return false;
}

void SetupPlatformKeyPolicyFlow::PreviewGroupPlatformKeyPolicyUpdate() { // Called when finish setup platform key policy, call to check use dummy tx or not, and
                                                                         // how many signatures are required if use dummy tx
    GUARD_APP_MODEL()
    if (appModel->walletInfoPtr()) {
        PreviewGroupPlatformKeyPolicyUpdateInput input;
        input.wallet_id = appModel->walletInfoPtr()->walletId();
        DBG_INFO << "PreviewGroupPlatformKeyPolicyUpdate: " << input.wallet_id;

        auto policiesOpt = getPoliciesFromWalletConfig();
        if (!policiesOpt.has_value()) {
            return;
        }
        input.policies = policiesOpt.value();
        m_previewGroupPlatformKeyPolicyUpdateUC.executeAsync(input, [this](const core::usecase::Result<PreviewGroupPlatformKeyPolicyUpdateResult> &result) {
            if (result.isSuccess()) {
                DBG_INFO << result.value().requires_dummy_transaction << ", pending_signatures: " << result.value().pending_signatures
                         << ", delay_apply_in_seconds: " << result.value().delay_apply_in_seconds;
                setrequiresDummyTransaction(result.value().requires_dummy_transaction);
                int pending_signatures = result.value().pending_signatures;
                int delay_apply_in_seconds = result.value().delay_apply_in_seconds;
                int after_hours = delay_apply_in_seconds / 3600;
                setpendingSignatures(pending_signatures);
                setafterHours(after_hours);
                emit startConfirmDummyTransaction();
            } else {
                showToast(result.code(), result.error(), EWARNING::WarningType::ERROR_MSG);
                return;
            }
        });
    }
}

void SetupPlatformKeyPolicyFlow::RequestGroupPlatformKeyPolicyUpdate() {
    GUARD_APP_MODEL()
    if (appModel->walletInfoPtr()) {
        RequestGroupPlatformKeyPolicyUpdateInput input;
        input.wallet_id = appModel->walletInfoPtr()->walletId();
        DBG_INFO << "RequestGroupPlatformKeyPolicyUpdate: " << input.wallet_id;

        auto policiesOpt = getPoliciesFromWalletConfig();
        if (!policiesOpt.has_value()) {
            return;
        }
        input.policies = policiesOpt.value();
        m_requestGroupPlatformKeyPolicyUpdateUC.executeAsync(input, [this](const core::usecase::Result<RequestGroupPlatformKeyPolicyUpdateResult> &result) {
            if (result.isSuccess()) {
                setdummyTx(result.value().dumyTx.value());
                emit startDummyTransaction();
                GUARD_APP_MODEL()
                if (auto wallet = appModel->walletInfoPtr()) {
                    if (auto dashboard = wallet->dashboard()) {
                        dashboard->GetAlertsInfo();
                        appModel->startReloadWallets();
                    }
                }
            } else {
                showToast(result.code(), result.error(), EWARNING::WarningType::ERROR_MSG);
            }
        });
    }
}

void SetupPlatformKeyPolicyFlow::RequestGroupPlatformKeyPolicyUpdateLater() {
    GUARD_APP_MODEL()
    if (appModel->walletInfoPtr()) {
        RequestGroupPlatformKeyPolicyUpdateInput input;
        input.wallet_id = appModel->walletInfoPtr()->walletId();
        DBG_INFO << "RequestGroupPlatformKeyPolicyUpdateLater: " << input.wallet_id;

        auto policiesOpt = getPoliciesFromWalletConfig();
        if (!policiesOpt.has_value()) {
            return;
        }
        input.policies = policiesOpt.value();
        m_requestGroupPlatformKeyPolicyUpdateUC.executeAsync(input, [this](const core::usecase::Result<RequestGroupPlatformKeyPolicyUpdateResult> &result) {
            if (result.isSuccess()) {
                GUARD_SUB_SCREEN_MANAGER()
                subMng->clear();
                GUARD_APP_MODEL()
                if (auto wallet = appModel->walletInfoPtr()) {
                    if (auto dashboard = wallet->dashboard()) {
                        dashboard->GetAlertsInfo();
                    }
                }
            } else {
            }
        });
    }
}

std::optional<nunchuk::GroupPlatformKeyPolicies> SetupPlatformKeyPolicyFlow::getPoliciesFromWalletConfig() {
    if (perKeyPolicies().size() > 0) {
        QVariantList perkeyPolicyList = perKeyPolicies();
        return helper::perkeyPolicyListToPolicies(perkeyPolicyList);
    } else {
        QVariantMap globalPolicyMap = policy();
        return helper::globalPolicyMapToPolicies(globalPolicyMap);
    }
}

void SetupPlatformKeyPolicyFlow::bind(QObject *vm) {
    auto baseKeyVm = qobject_cast<BasePlatformKeyPoliciesViewModel *>(vm);
    if (baseKeyVm) {
        baseKeyVm->setisEntryPointAlert(entryPoint() == Constants::EntryPointAlert);
        baseKeyVm->setisEntryPointWallet(entryPoint() == Constants::EntryPointWallet);
        baseKeyVm->setisEntryPointGroup(entryPoint() == Constants::EntryPointGroup);
    }
    auto realVm = qobject_cast<PlatformKeyPoliciesViewModel *>(vm);
    if (realVm) {
        if (selectedPolicyId().isEmpty()) {
            setselectedPolicyId(realVm->selectedPolicyId());
        } else {
            realVm->setselectedPolicyId(selectedPolicyId());
        }

        connect(realVm, &PlatformKeyPoliciesViewModel::selectedPolicyIdChanged, this, [this, realVm]() { setselectedPolicyId(realVm->selectedPolicyId()); });
    }
    auto signaturesRequiredVm = qobject_cast<SignaturesRequiredViewModel *>(vm);
    if (signaturesRequiredVm) {
        signaturesRequiredVm->setdescribeOne(Strings.STR_QML_1012());
        signaturesRequiredVm->setdescribeTwo(Strings.STR_QML_1013());
        signaturesRequiredVm->setpending_signatures(pendingSignatures());
        signaturesRequiredVm->setdummyTx(dummyTx());
        connect(this, &SetupPlatformKeyPolicyFlow::startDummyTransaction, signaturesRequiredVm, &SignaturesRequiredViewModel::slotStartDummyTransaction);
    }
    auto globalKeyVm = qobject_cast<GlobalPlatformKeyPoliciesViewModel *>(vm);
    if (globalKeyVm) {
        connect(this, &SetupPlatformKeyPolicyFlow::startConfirmDummyTransaction, globalKeyVm, &GlobalPlatformKeyPoliciesViewModel::slotConfirmDummyTransaction);
    }
    auto perKeyVm = qobject_cast<PerKeyPlatformKeyPoliciesViewModel *>(vm);
    if (perKeyVm) {
        connect(this, &SetupPlatformKeyPolicyFlow::startConfirmDummyTransaction, perKeyVm, &PerKeyPlatformKeyPoliciesViewModel::slotConfirmDummyTransaction);
    }
}

void SetupPlatformKeyPolicyFlow::checkingPolicyChangeWithDummyTransaction() {
    if (dummyTx().get_id().empty()) {
        return;
    }
    auto payload_opt = dummyTx().get_payload();
    if (!payload_opt.has_value()) {
        return;
    }
    auto payload = payload_opt.value();
    auto new_policies = payload.get_new_policies();
    int new_type = new_policies.get_global().has_value() ? 1 : (new_policies.get_signers().size() > 0 ? 2 : 0);
    GUARD_SUB_SCREEN_MANAGER()
    if (new_type == 1) {
        subMng->show(qml::features::signers::qglobalplatformkeypolicies);
    } else if (new_type == 2) {
        subMng->show(qml::features::signers::qperkeyplatformkeypolicies);
    }
}

namespace helper {
QVariantMap platformKeyPolicyToVariantMap(const nunchuk::GroupPlatformKeyPolicy &policy) {
    QVariantMap platform_polocies;

    platform_polocies.insert("singleSigner_name", "All keys");
    bool enable_spending_limit = policy.get_spending_limit().has_value();
    platform_polocies.insert("enableSpendingLimit", enable_spending_limit);
    if (enable_spending_limit) {
        nunchuk::GroupSpendingLimit spending_limit = policy.get_spending_limit().value();
        nunchuk::GroupSpendingLimitInterval spending_interval = spending_limit.get_interval();
        std::string spending_amount = spending_limit.get_amount();
        std::string spending_currency = spending_limit.get_currency();
        QString spending_interval_string = qUtils::GroupSpendingLimitIntervalToString(spending_interval);
        platform_polocies.insert("currency", QString::fromStdString(spending_currency));
        platform_polocies.insert("balance", QString::fromStdString(spending_amount));
        platform_polocies.insert("interval", spending_interval_string);
    }

    int signing_delay_seconds = policy.get_signing_delay_seconds();
    int hours = signing_delay_seconds / 3600;
    int minutes = (signing_delay_seconds % 3600) / 60;
    platform_polocies.insert("enableCoSigningDelay", (signing_delay_seconds > 0) ? true : false);
    platform_polocies.insert("signingDelaySeconds", signing_delay_seconds);
    platform_polocies.insert("hour", hours);
    platform_polocies.insert("minute", minutes);

    bool auto_broadcast_transaction = policy.get_auto_broadcast_transaction();
    platform_polocies.insert("autoBroadcast", auto_broadcast_transaction);

    return platform_polocies;
}

QVariantList perKeyPolicyToVariantListNormalWallet(const std::vector<nunchuk::SingleSigner> &signers,
                                                   const std::vector<nunchuk::GroupPlatformKeySignerPolicy> &policies, const int platformkey_index) {
    QVariantList list;
    for (int i = 0; i < signers.size(); i++) {
        const auto signer = signers[i];
        if (signer.get_master_fingerprint().empty() || signer.get_master_fingerprint() == "") {
            continue;
        }

        if (platformkey_index != -1 && i == platformkey_index) {
            continue;
        }

        if (signer.get_type() == nunchuk::SignerType::PLATFORM || signer.get_type() == nunchuk::SignerType::SERVER) {
            continue;
        }

        DBG_INFO << signer.get_name() << signer.get_master_fingerprint() << (int)signer.get_type();

        QString xfp_fromsigner = QString::fromStdString(signer.get_master_fingerprint());
        bool found_policy_for_signer = false;
        for (const auto &policy : policies) {
            QString xfp_frompolicy = QString::fromStdString(policy.get_master_fingerprint());
            if (qUtils::strCompare(xfp_fromsigner, xfp_frompolicy)) {
                QVariantMap signer_policy = platformKeyPolicyToVariantMap(policy.get_policy());
                const QVariantMap signer_info = singleSignerToVariantMap(signer);
                for (auto it = signer_info.constBegin(); it != signer_info.constEnd(); ++it) {
                    signer_policy.insert(it.key(), it.value());
                }
                list.append(signer_policy);
                found_policy_for_signer = true;
                break;
            }
        }
        if (!found_policy_for_signer) {
            QVariantMap signer_policy = defaultGlobalPolicy();
            const QVariantMap signer_info = singleSignerToVariantMap(signer);
            for (auto it = signer_info.constBegin(); it != signer_info.constEnd(); ++it) {
                signer_policy.insert(it.key(), it.value());
            }
            list.append(signer_policy);
        }
    }
    return list;
}

QVariantList perKeyPolicyToVariantListMiniscriptWallet(const std::map<std::string, nunchuk::SingleSigner> &signers,
                                                       std::vector<nunchuk::GroupPlatformKeySignerPolicy> policies, QStringList platform_key_slots) {
    QVariantList list;
    for (const auto &signer : signers) {
        const auto signer_slot = signer.first;
        const auto signer_obj = signer.second;
        if (signer_obj.get_master_fingerprint().empty() || signer_obj.get_master_fingerprint() == "") {
            continue;
        }

        if (platform_key_slots.size() > 0) {
            QString signer_name = QString::fromStdString(signer_slot);
            DBG_INFO << "Checking signer: " << signer_name << " against platform key slots: " << platform_key_slots;
            if (platform_key_slots.contains(signer_name)) {
                continue;
            }
        }
        if (signer_obj.get_type() == nunchuk::SignerType::PLATFORM || signer_obj.get_type() == nunchuk::SignerType::SERVER) {
            continue;
        }

        QString xfp_fromsigner = QString::fromStdString(signer_obj.get_master_fingerprint());
        bool found_policy_for_signer = false;
        for (const auto &policy : policies) {
            QString xfp_frompolicy = QString::fromStdString(policy.get_master_fingerprint());
            if (qUtils::strCompare(xfp_fromsigner, xfp_frompolicy)) {
                QVariantMap signer_policy = platformKeyPolicyToVariantMap(policy.get_policy());
                const QVariantMap signer_info = singleSignerToVariantMap(signer_obj);
                for (auto it = signer_info.constBegin(); it != signer_info.constEnd(); ++it) {
                    signer_policy.insert(it.key(), it.value());
                }
                list.append(signer_policy);
                found_policy_for_signer = true;
                break;
            }
        }
        if (!found_policy_for_signer) {
            QVariantMap signer_policy = defaultGlobalPolicy();
            const QVariantMap signer_info = singleSignerToVariantMap(signer_obj);
            for (auto it = signer_info.constBegin(); it != signer_info.constEnd(); ++it) {
                signer_policy.insert(it.key(), it.value());
            }
            list.append(signer_policy);
        }
    }
    return list;
}

QVariantMap singleSignerToVariantMap(const nunchuk::SingleSigner &signer) {
    QWarningMessage msg;
    nunchuk::SingleSigner localSigner = bridge::nunchukGetOriginSingleSigner(signer, msg);
    auto signerPtr = QSingleSignerPtr(new QSingleSigner(localSigner));
    auto signerVar = SingleSignerListModel::useQml(signerPtr);
    return signerVar.toMap();
}

nunchuk::GroupPlatformKeyPolicies globalPolicyMapToPolicies(const QVariantMap &policy) {
    DBG_INFO << policy;
    nunchuk::GroupPlatformKeyPolicies policies;
    std::vector<nunchuk::GroupPlatformKeySignerPolicy> perkeyPolicy{};
    policies.set_signers(perkeyPolicy);

    nunchuk::GroupPlatformKeyPolicy globalPolicy;

    bool enableSpendingLimit = policy.value("enableSpendingLimit").toBool();
    if (enableSpendingLimit) {
        nunchuk::GroupSpendingLimit limit;

        QString interval_string = policy.value("interval").toString();
        nunchuk::GroupSpendingLimitInterval interval_enum = qUtils::GroupSpendingLimitIntervalFromString(interval_string);
        limit.set_amount(policy.value("balance").toString().toStdString());
        limit.set_currency(policy.value("currency").toString().toStdString());
        limit.set_interval(interval_enum);
        globalPolicy.set_spending_limit(limit);
    } else {
        globalPolicy.set_spending_limit(std::nullopt);
    }

    bool enableCoSigningDelay = policy.value("enableCoSigningDelay").toBool();
    if (enableCoSigningDelay) {
        int hours = policy.value("hour").toInt();
        int minute = policy.value("minute").toInt();
        int signing_delay_seconds = hours * 3600 + minute * 60;
        globalPolicy.set_signing_delay_seconds(signing_delay_seconds);
    } else {
        globalPolicy.set_signing_delay_seconds(0);
    }

    bool autoBroadcast = policy.value("autoBroadcast").toBool();
    globalPolicy.set_auto_broadcast_transaction(autoBroadcast);

    std::optional<nunchuk::GroupPlatformKeyPolicy> global_opt = globalPolicy;
    policies.set_global(global_opt);
    return policies;
}

nunchuk::GroupPlatformKeyPolicies perkeyPolicyListToPolicies(const QVariantList &perkey_policies) {
    DBG_INFO << perkey_policies;
    nunchuk::GroupPlatformKeyPolicies policies;
    policies.set_global(std::nullopt);

    std::vector<nunchuk::GroupPlatformKeySignerPolicy> perkeyPolicies;
    perkeyPolicies.reserve(perkey_policies.size());

    for (const QVariant &item : perkey_policies) {
        QVariantMap policy = item.toMap();
        nunchuk::GroupPlatformKeySignerPolicy perkeyPolicy;

        // master fingerprint
        perkeyPolicy.set_master_fingerprint(policy.value("masterFingerprint").toString().toStdString());

        // build inner policy
        nunchuk::GroupPlatformKeyPolicy keyPolicy;

        bool enableSpendingLimit = policy.value("enableSpendingLimit").toBool();
        if (enableSpendingLimit) {
            nunchuk::GroupSpendingLimit limit;

            QString interval_string = policy.value("interval").toString();
            nunchuk::GroupSpendingLimitInterval interval_enum = qUtils::GroupSpendingLimitIntervalFromString(interval_string);
            limit.set_amount(policy.value("balance").toString().toStdString());
            limit.set_currency(policy.value("currency").toString().toStdString());
            limit.set_interval(interval_enum);
            keyPolicy.set_spending_limit(limit);
        } else {
            keyPolicy.set_spending_limit(std::nullopt);
        }

        bool enableCoSigningDelay = policy.value("enableCoSigningDelay").toBool();
        if (enableCoSigningDelay) {
            int hours = policy.value("hour").toInt();
            int minute = policy.value("minute").toInt();
            int signing_delay_seconds = hours * 3600 + minute * 60;
            keyPolicy.set_signing_delay_seconds(signing_delay_seconds);
        } else {
            keyPolicy.set_signing_delay_seconds(0);
        }

        bool autoBroadcast = policy.value("autoBroadcast").toBool();
        keyPolicy.set_auto_broadcast_transaction(autoBroadcast);

        QString xfp = policy.value("singleSigner_masterFingerPrint").toString();

        // attach policy to signer
        perkeyPolicy.set_policy(keyPolicy);
        perkeyPolicy.set_master_fingerprint(xfp.toStdString());
        perkeyPolicies.push_back(perkeyPolicy);
    }

    policies.set_signers(perkeyPolicies);
    return policies;
}

nunchuk::GroupPlatformKeySignerPolicy filterPolicy(const std::vector<nunchuk::GroupPlatformKeySignerPolicy> &signers_policies, const QString &xfp) {
    for (const auto &policy : signers_policies) {
        const QString policy_fingerprint = QString::fromStdString(policy.get_master_fingerprint());
        if (qUtils::strCompare(policy_fingerprint, xfp)) {
            return policy;
        }
    }
    return nunchuk::GroupPlatformKeySignerPolicy{};
}

QVariantMap defaultGlobalPolicy() {
    QVariantMap policy;
    policy.insert("singleSigner_name", "All keys");
    policy.insert("currency", "USD");
    policy.insert("balance", "0");
    policy.insert("interval", qUtils::GroupSpendingLimitIntervalToString(nunchuk::GroupSpendingLimitInterval::DAILY));
    policy.insert("enableCoSigningDelay", false);
    policy.insert("autoBroadcast", false);
    policy.insert("signingDelaySeconds", "0");
    policy.insert("hour", "2");
    policy.insert("minute", "");
    return policy;
}

QVariantList defaultPerkeyPolices(const std::vector<nunchuk::SingleSigner> signers) {
    QVariantList perkeyPolices;

    for (auto signer : signers) {
        if (signer.get_type() == nunchuk::SignerType::PLATFORM || signer.get_type() == nunchuk::SignerType::SERVER) {
            continue;
        }
        QVariantMap policy = singleSignerToVariantMap(signer);
        policy.insert("currency", "USD");
        policy.insert("balance", "0");
        policy.insert("interval", qUtils::GroupSpendingLimitIntervalToString(nunchuk::GroupSpendingLimitInterval::DAILY));
        policy.insert("enableCoSigningDelay", false);
        policy.insert("autoBroadcast", false);
        policy.insert("signingDelaySeconds", "0");
        policy.insert("hour", "2");
        policy.insert("minute", "");
        perkeyPolices.append(policy);
    }
    return perkeyPolices;
}

} // namespace helper
} // namespace features::signers::flows
