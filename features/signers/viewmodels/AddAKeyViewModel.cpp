#include "AddAKeyViewModel.h"
#include "ViewsEnums.h"
#include "core/common/resources/AppStrings.h"
#include "core/ui/UiServices.inc"
#include "features/signers/flows/KeySetupFlow.h"
#include "generated_qml_keys.hpp"
#include "features/signers/flows/SetupPlatformKeyPolicyFlow.h"

namespace features::signers::viewmodels {
using namespace features::signers::flows;

AddAKeyViewModel::AddAKeyViewModel(QObject *parent) : AddKeyBaseViewModel(parent) {
    setisPrimaryKeyFlow(false);
    setcanAddHardwareKey(true);
    setsoftwareSignerLimited(false);
    setplatformKeyAvailable(true);
}

void AddAKeyViewModel::onInit() {
    AddKeyBaseViewModel::onInit();
    updateIsPrimaryKeyFlow();
    updateHardwareKeyAvailability();
    updateSoftwareSignerLimitStatus();
    updatePlatformKeyAvailability();
}

void AddAKeyViewModel::onAddPlatformKeyClicked() {
    DBG_INFO;
    GUARD_SUB_SCREEN_MANAGER()
    GUARD_FLOW_MANAGER()
    GUARD_SHARE_WALLETS()

    auto flow = dynamic_cast<SetupPlatformKeyPolicyFlow*> (flowMng->currentFlow());
    DBG_INFO << flow;
    if (flow){
        QString  groupId = flow->groupId();
        QVariant indexOrKey = flow->indexOrKey();
        if (auto group_wallet = sharedWallets->sandboxListPtr()->GetGroup(groupId)) {
            int walletType = (int)group_wallet->sandbox().get_wallet_type();
            QVector<QString> keyslots = {};
            if(walletType == (int)nunchuk::WalletType::MINISCRIPT){
                QString firstLine = indexOrKey.toString();
                keyslots.append(firstLine);
            }

            QWarningMessage msg;
            nunchuk::GroupSandbox sanbox = bridge::EnableGroupPlatformKey(groupId, keyslots, msg);
            if((int)EWARNING::WarningType::NONE_MSG == msg.type()) {
                GUARD_APP_MODEL()
                group_wallet->setScreenFlow("setup-group-wallet");
                group_wallet->setSandbox(sanbox);
                QJsonObject json;
                json["type"] = "setup-group-wallet";
                QEventProcessor::instance()->sendEvent(E::EVT_SETUP_GROUP_WALLET_REQUEST, json);
            }
            else {
                emit showToast(msg.code(), msg.what(), (EWARNING::WarningType)msg.type());
            }
        }
    }
}

void AddAKeyViewModel::updateIsPrimaryKeyFlow() {
    bool isPrimaryKey = (QEventProcessor::instance()->currentFlow() == (int)ENUNCHUCK::IN_FLOW::FLOW_PRIMARY_KEY) ||
                        (QEventProcessor::instance()->currentFlow() == (int)ENUNCHUCK::IN_FLOW::FLOW_REPLACE_PRIMARY_KEY);
    setisPrimaryKeyFlow(isPrimaryKey);
}

void AddAKeyViewModel::updateHardwareKeyAvailability() {
    GUARD_APP_MODEL()
    bool canAddHw;
    bool isPrimaryKey = (QEventProcessor::instance()->currentFlow() == (int)ENUNCHUCK::IN_FLOW::FLOW_PRIMARY_KEY) ||
                        (QEventProcessor::instance()->currentFlow() == (int)ENUNCHUCK::IN_FLOW::FLOW_REPLACE_PRIMARY_KEY);
    if(auto newWalletInfo = appModel->newWalletInfo()){
        if (isPrimaryKey) {
            canAddHw = false;
        } else {
            bool isTaproot = (newWalletInfo->walletAddressType() == (int)nunchuk::AddressType::TAPROOT);
            canAddHw = !isTaproot;
        }
        setcanAddHardwareKey(canAddHw);
    }
}

void AddAKeyViewModel::updateSoftwareSignerLimitStatus() {
    GUARD_APP_MODEL()

    bool isLimited = appModel->limitSoftwareSigner() ? (appModel->softwareSignerDeviceList()->rowCount() == 0) : true;
    setsoftwareSignerLimited(isLimited);
}

void AddAKeyViewModel::updatePlatformKeyAvailability() {
    // Platform key availability depends on subscription and wallet type
    // This will be set based on the application context and subscription status
    setplatformKeyAvailable(true);
}

} // namespace features::signers::viewmodels
