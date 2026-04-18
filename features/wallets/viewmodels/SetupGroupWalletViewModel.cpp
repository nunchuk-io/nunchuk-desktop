#include "SetupGroupWalletViewModel.h"
#include "core/ui/UiServices.inc"
#include "generated_qml_keys.hpp"
#include "features/signers/flows/SetupPlatformKeyPolicyFlow.h"

namespace features::wallets::viewmodels {
using namespace core::viewmodels;
using namespace features::signers::flows;

SetupGroupWalletViewModel::SetupGroupWalletViewModel(QObject *parent) : ActionViewModel(parent) {}

void SetupGroupWalletViewModel::onPlatformKeyConfigClicked(const QString &groupid) {
    GUARD_SUB_SCREEN_MANAGER()
    GUARD_FLOW_MANAGER();
    GUARD_SHARE_WALLETS()
    flowMng->stopFlow<SetupPlatformKeyPolicyFlow>();
    auto flow = flowMng->startFlow<SetupPlatformKeyPolicyFlow>();
    if (flow){
        flow->setentryPoint(Constants::EntryPointGroup);
        flow->setgroupId(groupid);
        if (auto group_wallet = sharedWallets->sandboxListPtr()->GetGroup(groupid)) {
            DBG_INFO;
            group_wallet->setScreenFlow("setup-group-wallet");
            nunchuk::GroupSandbox sanbox = group_wallet->sandbox();
            if (sanbox.get_platform_key().has_value()) {
                auto policies = sanbox.get_platform_key().value().get_policies();
                bool has_global_policies = policies.get_global().has_value();
                closeToWalletTab();
                if(has_global_policies){
                    subMng->show(qml::features::signers::qglobalplatformkeypolicies);
                }
                else {
                    subMng->show(qml::features::signers::qperkeyplatformkeypolicies);
                }
            }
        }
    }
}

void SetupGroupWalletViewModel::onAddKeyClicked(const QVariant& msg)
{
    GUARD_FLOW_MANAGER()
    GUARD_SHARE_WALLETS()
    GUARD_APP_MODEL()
    QVariantMap inputObj = msg.toMap();
    DBG_INFO << "msg:" << inputObj;
    QString groupid = inputObj.value("groupId").toString();
    QVariant indexOrKey = QVariant();
    if (auto gw = appModel->newWalletInfoPtr()) {
        if (auto sandbox = sharedWallets->sandboxListPtr()->GetGroup(groupid)) {
            gw->setWalletAddressType(sandbox->addressType());
            gw->setGroupSandbox(sandbox);
            gw->setWalletOptType((int)ENUNCHUCK::WalletOptionType_t::E_GROUP_WALLET);

            int walletType = sandbox->walletType();
            if(walletType == (int)nunchuk::WalletType::MINISCRIPT){
                indexOrKey = inputObj.value("key");
                DBG_INFO << "key:" << indexOrKey.toString();
            }
            else {
                indexOrKey = inputObj.value("key");
                DBG_INFO << "index:" << indexOrKey.toInt();
            }
        }
    }

    flowMng->stopFlow<SetupPlatformKeyPolicyFlow>();
    auto flow = flowMng->startFlow<SetupPlatformKeyPolicyFlow>();
    if (flow){
        flow->setentryPoint(Constants::EntryPointGroup);
        flow->setgroupId(groupid);
        flow->setindexOrKey(indexOrKey);
    }
}

} // namespace features::wallets::viewmodels
