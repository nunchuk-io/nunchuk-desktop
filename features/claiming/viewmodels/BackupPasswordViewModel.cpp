#include "BackupPasswordViewModel.h"
#include "core/bridge/ExternalBridges.h"
#include "core/common/resources/AppStrings.h"
#include "core/restapi/RestApi.h"
#include "core/ui/UiServices.inc"
#include "core/utils/Utils.h"
#include "features/claiming/flows/OffChainClaimingFlow.h"
#include "generated_qml_keys.hpp"

namespace features::claiming::viewmodels {
using namespace core::viewmodels;
using namespace features::claiming::usecases;
using namespace features::signers::usecases;
using features::claiming::flows::OffChainClaimingFlow;

BackupPasswordViewModel::BackupPasswordViewModel(QObject *parent) : BaseClaimingViewModel(parent) {}

void BackupPasswordViewModel::initializeChallengeMessage() {
    SigningChallengeInput input;
    input.magic = magicWord();
    m_signingChallengeUC.executeAsync(input, [this](const core::usecase::Result<SigningChallengeResult> &result) {
        if (result.isSuccess()) {
            auto message = result.value().message;
            auto guide = message.value("message").toString();
            auto messageId = message.value("id").toString();
            setmessage(guide);
            setmessageId(messageId);
        } else {
            emit showToast(-1, result.error(), EWARNING::WarningType::ERROR_MSG);
        }
    });
}

void BackupPasswordViewModel::onInit() {
    initializeChallengeMessage();
}

void BackupPasswordViewModel::startDownloadBackup() {
    GUARD_APP_MODEL()
    DownloadBackupInput input;
    input.magic = magicWord();
    input.backupPwd = backupPwd();
    input.backupPwdTwo = backupPwdTwo();
    input.keyName = m_signerNammingUC.addParameter(appModel).getInheritanceNameKey();
    input.keyNameTwo = m_signerNammingUC.addParameter(appModel).getInheritanceNameKey();
    m_downloadBackupUC.executeAsync(input, [this, appModel](const core::usecase::Result<DownloadBackupResult> &result) {
        if (result.isSuccess()) {
            emit appModel->startReloadMasterSigners();
            auto backupResult = result.value();
            GUARD_FLOW_MANAGER()
            auto flow = flowMng->startFlow<OffChainClaimingFlow>();
            DBG_INFO << "startDownloadBackup Downloaded backup signers:" << backupResult.signers.size();
            flow->setbackupSigners(backupResult.signers);
            flow->setmessageId(messageId());
            createTokenForBackupPassword();
        } else {
            handleErrorCode(result.code(), result.error());
        }
    });
}

void BackupPasswordViewModel::createTokenForBackupPassword() {
    GUARD_FLOW_MANAGER()
    auto flow = flowMng->startFlow<OffChainClaimingFlow>();
    for (int i = 0; i < flow->backupSigners().size(); i++) {
        auto signer = flow->backupSigners()[i];
        if (!flow->addSingleSigner(signer)) {
            emit showToast(-1, Strings.STR_QML_2115(), EWARNING::WarningType::ERROR_MSG);
            return;
        }
    }
    SignMessageInput input;
    input.message = message();
    input.signers = flow->backupSigners();
    m_signMessageUC.executeAsync(input, [this, flow](const core::usecase::Result<SignMessageResult> &result) {
        if (result.isSuccess()) {
            QWarningMessage msg;
            QMap<QString, QString> sigs = result.value().signatures;
            for (auto it = sigs.begin(); it != sigs.end(); ++it) {
                QString authorization = qUtils::CreateRequestToken(it.value(), it.key(), msg);
                if (msg.isSuccess()) {
                    flow->addAuthorization(authorization);
                } else {
                    emit showToast(-1, msg.what(), EWARNING::WarningType::ERROR_MSG);
                    return;
                }
            }
            handleClaim();
        } else {
            emit showToast(-1, result.error(), EWARNING::WarningType::ERROR_MSG);
        }
    });
}

void BackupPasswordViewModel::next() {
    startDownloadBackup();
}

void BackupPasswordViewModel::handleClaim() {
    GUARD_FLOW_MANAGER()
    auto flow = flowMng->startFlow<OffChainClaimingFlow>();
    if (!flow->isManualClaiming()) {
        // all keys are downloaded, proceed to claim status
        if (flow->isAllKeysAdded()) {
            flow->claimStatus();
        }
    } else {
        // some keys are downloaded, proceed to verify inheritance key screen
        if (flow->isAllKeysAdded()) {
            flow->claimStatus();
        } else {
            GUARD_SUB_SCREEN_MANAGER()
            subMng->show(qml::features::claiming::offchain::qyourplanrequirestwoinheritancekeysaddedone);
        }
    }
}
} // namespace features::claiming::viewmodels