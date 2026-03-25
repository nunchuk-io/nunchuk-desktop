#include "VerifyInheritanceKeyViewModel.h"
#include "core/bridge/ExternalBridges.h"
#include "core/common/resources/AppStrings.h"
#include "core/ui/UiServices.inc"
#include "core/utils/Utils.h"
#include "features/claiming/flows/OffChainClaimingFlow.h"
#include "generated_qml_keys.hpp"

namespace features::claiming::viewmodels {
using namespace core::viewmodels;
using namespace features::claiming::usecases;
using namespace features::signers::usecases;
using features::claiming::flows::OffChainClaimingFlow;

VerifyInheritanceKeyViewModel::VerifyInheritanceKeyViewModel(QObject *parent) : ActionViewModel(parent) {
    setisVerified(false);
    setisMoreShow(false);
}

void VerifyInheritanceKeyViewModel::proceedVerification(const nunchuk::SingleSigner &signer) {
    DBG_INFO << "VerifyInheritanceKeyViewModel::proceedVerification - signer:" << signer.get_master_fingerprint();
    setcurrentSigner(signer);
    bool canSign = signer.get_type() == nunchuk::SignerType::SOFTWARE || signer.get_type() == nunchuk::SignerType::HARDWARE;
    setisMoreShow(!canSign);
    setcurrentXfp(QString::fromStdString(signer.get_master_fingerprint()));
    std::vector<nunchuk::SingleSigner> signers;
    signers.push_back(signer);
    QSingleSignerListModelPtr signerListModel = bridge::nunchukConvertRemoteSigners(signers);
    setsignerList(signerListModel);
}

void VerifyInheritanceKeyViewModel::initializeChallengeMessage() {
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

void VerifyInheritanceKeyViewModel::onInit() {
    initializeChallengeMessage();
}

void VerifyInheritanceKeyViewModel::createTokenAfterSign(const QString &sig) {
    QWarningMessage msg;
    QString token = qUtils::CreateRequestToken(sig, QString::fromStdString(currentSigner().get_master_fingerprint()), msg);
    if (msg.isSuccess() == false) {
        emit showToast(-1, Strings.STR_QML_2117(), EWARNING::WarningType::ERROR_MSG);
        return;
    }
    signerList()->updateSignatures(QString::fromStdString(currentSigner().get_master_fingerprint()), true, sig);
    signerList()->requestSort(true);
    setisVerified(true);
    GUARD_FLOW_MANAGER()
    auto flow = flowMng->startFlow<OffChainClaimingFlow>();
    flow->setmessageId(messageId());
    flow->addAuthorization(token);
}

void VerifyInheritanceKeyViewModel::back() {
    emit signalBack();
    ActionViewModel::back();
}

void VerifyInheritanceKeyViewModel::exportFileSignMessage(const QString &filename) {
    QWarningMessage msg;
    QString data = qUtils::GenerateColdCardHealthCheckMessage(QString::fromStdString(currentSigner().get_derivation_path()), message(), addressType(), msg);
    if (msg.isSuccess()) {
        QString file_path = qUtils::QGetFilePath(filename);
        qUtils::ExportDataViaFile(file_path, data);
        GUARD_SUB_SCREEN_MANAGER()
        subMng->show(qml::features::claiming::offchain::qexportcompleted);
    } else {
        emit showToast(-1, msg.what(), EWARNING::WarningType::ERROR_MSG);
    }
}

void VerifyInheritanceKeyViewModel::exportQRSignMessage() {
    QWarningMessage msg;
    QString data = qUtils::GenerateColdCardHealthCheckMessage(QString::fromStdString(currentSigner().get_derivation_path()), message(), addressType(), msg);
    if (msg.isSuccess()) {
        msg.resetWarningMessage();
        QStringList qrList = qUtils::ExportBBQRJSON(data, msg);
        if (msg.isSuccess()) {
            setqrExported(qrList);
        } else {
            emit showToast(-1, msg.what(), EWARNING::WarningType::ERROR_MSG);
        }
    } else {
        emit showToast(-1, msg.what(), EWARNING::WarningType::ERROR_MSG);
    }
}

void VerifyInheritanceKeyViewModel::importFileSignedMessage(const QString &filename) {
    QString file_path = qUtils::QGetFilePath(filename);
    QString fileContent = qUtils::ImportDataViaFile(file_path);
    QWarningMessage msg;
    QString sig = qUtils::ExtractColdcardMessageSignature(fileContent, msg);
    if (msg.isSuccess() == false) {
        emit showToast(-1, Strings.STR_QML_2117(), EWARNING::WarningType::ERROR_MSG);
        return;
    }
    createTokenAfterSign(sig);
}

void VerifyInheritanceKeyViewModel::importQRSignedMessage(const QStringList &qrData) {
    QWarningMessage msg;
    DBG_INFO << "importQRSignedMessage qrData size: " << qrData;
    QString sig = qUtils::ExtractColdcardMessageSignature(qrData, msg);
    if (msg.isSuccess() == false) {
        emit showToast(-1, Strings.STR_QML_2116(), EWARNING::WarningType::ERROR_MSG);
        return;
    }
    createTokenAfterSign(sig);
}

void VerifyInheritanceKeyViewModel::next() {
    GUARD_FLOW_MANAGER()
    auto flow = flowMng->startFlow<OffChainClaimingFlow>();
    if (flow->isAllKeysAdded()) {
        flow->claimStatus();
        close();
    } else {
        GUARD_SUB_SCREEN_MANAGER()
        subMng->show(qml::features::claiming::offchain::qyourplanrequirestwoinheritancekeysaddedone);
    }
}

void VerifyInheritanceKeyViewModel::onSignClicked() {
    nunchuk::SignerType type = currentSigner().get_type();
    switch (type) {
    case nunchuk::SignerType::COLDCARD_NFC:
    case nunchuk::SignerType::NFC: {
        emit showToast(-1, Strings.STR_QML_688(), EWARNING::WarningType::WARNING_MSG);
        break;
    }
    case nunchuk::SignerType::HARDWARE:
    case nunchuk::SignerType::SOFTWARE: {
        qApp->setOverrideCursor(QCursor(Qt::WaitCursor));
        SignMessageInput input;
        input.message = message();
        input.signers = {currentSigner()};
        m_signMessageUC.executeAsync(input, [this](const core::usecase::Result<SignMessageResult> &result) {
            if (result.isSuccess()) {
                auto signatures = result.value().signatures.values();
                createTokenAfterSign(signatures.first());
            } else {
                emit showToast(-1, result.error(), EWARNING::WarningType::ERROR_MSG);
            }
            qApp->restoreOverrideCursor();
        });
        break;
    }
    default:
        break;
        {
            break;
        }
    }
}

} // namespace features::claiming::viewmodels