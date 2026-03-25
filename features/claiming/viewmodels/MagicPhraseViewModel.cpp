#include "MagicPhraseViewModel.h"
#include "core/bridge/ExternalBridges.h"
#include "core/ui/UiServices.inc"
#include "features/claiming/flows/OffChainClaimingFlow.h"
#include "features/claiming/flows/OnChainClaimingFlow.h"
#include "generated_qml_keys.hpp"
#include "nunchuk.h"

namespace features::claiming::viewmodels {
using namespace core::viewmodels;
using namespace features::claiming::usecases;
using features::claiming::flows::OffChainClaimingFlow;
using features::claiming::flows::OnChainClaimingFlow;

MagicPhraseViewModel::MagicPhraseViewModel(QObject *parent) : BaseClaimingViewModel(parent) {}

void MagicPhraseViewModel::startOffChainClaimingFlow(const QString &magicPhrase, const QJsonArray &keyOrigins, int inheritance_key_count) {
    GUARD_FLOW_MANAGER()
    flowMng->stopFlow<OffChainClaimingFlow>();
    auto flow = flowMng->startFlow<OffChainClaimingFlow>();
    flow->setmagicWord(magicPhrase);
    flow->setkeyOrigins(keyOrigins);
    flow->setinheritance_key_count(inheritance_key_count);
}

void MagicPhraseViewModel::claimInit(const QString &magicPhrase) {
    InitializeInheritanceClaimingInput input;
    input.magic = magicPhrase;
    initializeInheritanceClaimingUC.executeAsync(input, [this, magicPhrase](const core::usecase::Result<InitializeInheritanceClaimingResult> &result) {
        if (result.isSuccess()) {
            GUARD_SERVICE_TAG()
            const auto &res = result.value();
            serviceTag->setClaimInit(res);
            serviceTag->clearInheritanceKeys();
            if (qUtils::strCompare(res.wallet_type, "MINISCRIPT")) {
                GUARD_APP_MODEL()
                GUARD_FLOW_MANAGER()
                flowMng->stopFlow<OnChainClaimingFlow>();
                auto flow = flowMng->startFlow<OnChainClaimingFlow>();
                flow->setinheritance_key_count(res.inheritance_key_count);
                flow->setmagicWord(magicPhrase);
                auto wallet = appModel->walletListPtr()->getWalletById(res.wallet_id);
                if (!wallet) {
                    #ifdef NEW_ON_CLAIM
                        GUARD_SUB_SCREEN_MANAGER()
                        if (res.inheritance_key_count < 2) {
                            subMng->show(qml::features::claiming::onchain::qprepareinheritancekey);
                        } else {
                            subMng->show(qml::features::claiming::onchain::qyourplanrequirestwoinheritancekeys);
                        }
                    #else
                        emit gotoOnChain(res.inheritance_key_count);
                    #endif
                    return;
                }
                auto nunchukWallet = wallet->nunchukWallet();
                QWarningMessage msg;
                QString bsms = bridge::wallet::GetWalletExportData(nunchukWallet, nunchuk::ExportFormat::BSMS, msg);
                if ((int)EWARNING::WarningType::NONE_MSG == msg.type()) {
                    serviceTag->claimInitSaveBsms(bsms, nunchukWallet);
                    serviceSetting->setWalletInfo(wallet);
                    flow->setnunWallet(nunchukWallet);                    
                    flow->setbsms(bsms);                    
                    flow->claimStatus();
                } else if (res.inheritance_key_count > 0) {
                    emit gotoOnChain(res.inheritance_key_count);
                    serviceSetting->CreateAssignAvailableSigners("MINISCRIPT");
                }
            } else {
                // OFF CHAIN FLOW --- IGNORE ---
                startOffChainClaimingFlow(magicPhrase, res.key_origins, res.inheritance_key_count);
                if (res.inheritance_key_count > 1) {
                    GUARD_SUB_SCREEN_MANAGER()
                    subMng->show(qml::features::claiming::offchain::qyourplanrequirestwoinheritancekeys);
                } else {
                    GUARD_RIGHT_PANEL_NAV()
                    rightPanel->request(qml::components::rightpannel::service::common::qserviceclaiminheritanceoptionsproceed);
                }
            }
        } else {
            handleErrorCode(result.code(), result.error());
        }
    });
}

} // namespace features::claiming::viewmodels