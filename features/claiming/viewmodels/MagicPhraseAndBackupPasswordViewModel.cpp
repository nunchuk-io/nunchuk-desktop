#include "MagicPhraseAndBackupPasswordViewModel.h"
#include "core/ui/UiServices.inc"
#include "core/bridge/ExternalBridges.h"
#include "generated_qml_keys.hpp"
#include "nunchuk.h"
#include "features/claiming/flows/OnChainClaimingFlow.h"

namespace features::claiming::viewmodels {
using namespace core::viewmodels;
using namespace features::claiming::usecases;
using features::claiming::flows::OnChainClaimingFlow;

MagicPhraseAndBackupPasswordViewModel::MagicPhraseAndBackupPasswordViewModel(QObject *parent)
    : ActionViewModel(parent) {
}

void MagicPhraseAndBackupPasswordViewModel::claimInit(const QString& magicPhrase) {
    GUARD_SERVICE_TAG()
    InitializeInheritanceClaimingInput input;
    input.magic = magicPhrase;
    initializeInheritanceClaimingUC.executeAsync(input, [this, serviceTag, serviceSetting](const core::usecase::Result<InitializeInheritanceClaimingResult>& result) {
            if (result.isSuccess()) {
                const auto& res = result.value();                
                serviceTag->setClaimInit(res);
                serviceTag->clearInheritanceKeys();
                if (!res.wallet_id.isEmpty() && qUtils::strCompare(res.wallet_type, "MINISCRIPT")) {
                    GUARD_APP_MODEL()
                    auto wallet = appModel->walletListPtr()->getWalletById(res.wallet_id);
                    if (!wallet) {
                        emit gotoOnChain(res.inheritance_key_count);
                        return;
                    }
                    auto nunchukWallet = wallet->nunchukWallet();
                    QWarningMessage msg;
                    QString bsms = bridge::wallet::GetWalletExportData(nunchukWallet, nunchuk::ExportFormat::BSMS, msg);
                    if ((int)EWARNING::WarningType::NONE_MSG == msg.type()) {
                        serviceTag->claimInitSaveBsms(bsms, nunchukWallet);
                        serviceSetting->setWalletInfo(wallet);
                        emit serviceTag->resultClaimInheritanceAlert("CLAIM_INHERITANCE_PLAN_RESULT_SUCCESS");
                        serviceTag->inheritanceClaimStatus(res.magic);
                        return;
                    }
                } else {
                    emit gotoOffChain();
                    return;
                }
                if (res.inheritance_key_count > 0) {
                    emit gotoOnChain(res.inheritance_key_count);
                    serviceSetting->CreateAssignAvailableSigners("MINISCRIPT");
                }
            } else {
                GUARD_SUB_SCREEN_MANAGER()
                GUARD_FLOW_MANAGER()
                switch (result.code()) {
                case DRACO_CODE::INHERITANCE_801:
                    // show popup: Invalid phrase. Please try again
                    AppModel::instance()->showToast(0, STR_CPP_115, EWARNING::WarningType::ERROR_MSG);
                    break;
                case DRACO_CODE::INHERITANCE_802:
                    // show popup: It looks like the original subscription has expired for more than 3 years.
                    //             To claim the inheritance, please reactivate the plan first by visiting nunchuk.io/claim.
                    emit serviceTag->isExpiredAlert();
                    break;
                case DRACO_CODE::INHERITANCE_831:
                case DRACO_CODE::INHERITANCE_803: {
                    // show popup: Your inheritance plan has not been activated yet. Please try again later.
                    auto flow = flowMng->startFlow<OnChainClaimingFlow>();
                    flow->passClaimingError(result.code(), result.error());
                    subMng->show(qml::screens::onlinemode::claiming::claiminheritanceplanresult);
                }    break;
                case DRACO_CODE::INHERITANCE_829:
                    // show popup: Please come back later
                    emit serviceTag->comeBackLaterAlert(result.error());
                    break;
                case DRACO_CODE::INHERITANCE_830:
                    // show popup: Security deposit required
                    emit serviceTag->securityDepositRequiredAlert(result.error());
                    break;
                default:
                    AppModel::instance()->showToast(result.code(), "Invalid Magic Phrase", EWARNING::WarningType::EXCEPTION_MSG);
                    break;
                }
            }
        });
}
} // namespace features::claiming::viewmodels