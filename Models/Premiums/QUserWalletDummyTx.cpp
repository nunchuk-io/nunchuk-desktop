#include "QUserWalletDummyTx.h"
#include "Servers/Draco.h"
#include "ServiceSetting.h"
#include "Premiums/QKeyRecovery.h"
#include "Premiums/QWalletServicesTag.h"
#include "Premiums/QGroupDashboard.h"
#include "Premiums/QInheritancePlan.h"
#include "ViewsEnums.h"

QUserWalletDummyTx::QUserWalletDummyTx(WalletId wallet_id)
    : QWalletDummyTx(wallet_id)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

bool QUserWalletDummyTx::CreateDummyTransaction()
{
    QJsonObject bodydata;
    if (auto w = walletInfoPtr()) {
        int flow = w->flow();
        switch ((AlertEnum::E_Alert_t)flow) {
        case AlertEnum::E_Alert_t::SERVICE_TAG_INHERITANCE_PLAN_CREATE:
        case AlertEnum::E_Alert_t::SERVICE_TAG_INHERITANCE_PLAN_UPDATE:
            if (auto plan = w->inheritancePlanPtr()) {
                bodydata = plan->JsBody();
            }
            break;
        case AlertEnum::E_Alert_t::SERVICE_TAG_INHERITANCE_PLAN_CANCEL:
            bodydata["wallet"] = wallet_id();
            bodydata["group_id"] = w->groupId();
            break;
        default:
            break;
        }
    }
    QJsonObject reqiredSignatures = ServiceSetting::instance()->servicesTagPtr()->reqiredSignaturesJs();
    m_required_signatures = reqiredSignatures["required_signatures"].toInt();
    QJsonObject body;
    body["nonce"] = Draco::instance()->randomNonce();
    body["body"] = bodydata;
    m_nonceBody = body;
    if (CreateDummyTx(bodyString())) {
        if (transactionPtr()) {
            transactionPtr()->setSignatures({});
            emit transactionPtr()->singleSignerAssignedChanged();
        }
        emit transactionInfoChanged();
        return true;
    }
    return false;
}

bool QUserWalletDummyTx::CreateDummyTransaction(const QString &period_id)
{
    QJsonObject bodydata;
    bodydata["period_id"] = period_id;
    bodydata["wallet"] = wallet_id();
    QJsonObject body;
    body["nonce"] = Draco::instance()->randomNonce();
    body["body"] = bodydata;
    m_nonceBody = body;
    if (CreateDummyTx(bodyString())) {
        if (transactionPtr()) {
            transactionPtr()->setSignatures({});
            emit transactionPtr()->singleSignerAssignedChanged();
        }
        emit transactionInfoChanged();
        return true;
    }
    return false;
}

bool QUserWalletDummyTx::requestSignTx(const QString &xfp)
{
    if(transactionPtr()){
        QWarningMessage warningmsg;
        nunchuk::Wallet wallet = bridge::nunchukGetOriginWallet(wallet_id(), warningmsg);
        nunchuk::SingleSigner signer = *std::find_if(wallet.get_signers().begin(), wallet.get_signers().end(), [xfp](const nunchuk::SingleSigner &s) {
            return s.get_master_fingerprint() == xfp.toStdString();
        });
        QMap<QString, QString> signatures = transactionPtr()->signatures();
        if((int)EWARNING::WarningType::NONE_MSG == warningmsg.type()){
            QString tx_to_sign = qUtils::GetHealthCheckDummyTx(wallet, bodyString(), warningmsg);
            QString signature = "";
            switch (signer.get_type()) {
            case nunchuk::SignerType::HARDWARE:
            case nunchuk::SignerType::SOFTWARE:
            case nunchuk::SignerType::COLDCARD_NFC:
            {
                signature = bridge::SignHealthCheckMessage(signer, tx_to_sign, warningmsg);
                if((int)EWARNING::WarningType::NONE_MSG == warningmsg.type()){
                    signatures[xfp] = signature;
                }
                break;
            }
            default:
                // Not support -> Show popup
                break;
            }
            emit AppModel::instance()->finishedSigningTransaction();
            requestUpdateDummyTx(signatures);
            return true;
        }
        else {
            emit AppModel::instance()->finishedSigningTransaction();
            AppModel::instance()->showToast(warningmsg.code(), warningmsg.what(), (EWARNING::WarningType)warningmsg.type());
        }
    }
    return false;
}

bool QUserWalletDummyTx::requestSignTxViaQR(const QStringList &qrtags)
{
    if (transactionPtr()) {
        QMap<QString, QString> signatures = transactionPtr()->signatures();
        QWarningMessage warningmsg;
        QString psbt = qUtils::ParseQRTransaction(qrtags, warningmsg);
        if((int)EWARNING::WarningType::NONE_MSG == warningmsg.type() && psbt != ""){
            warningmsg.resetWarningMessage();
            nunchuk::Wallet wallet = bridge::nunchukGetOriginWallet(wallet_id(), warningmsg);
            if((int)EWARNING::WarningType::NONE_MSG == warningmsg.type()){
                warningmsg.resetWarningMessage();
                nunchuk::Transaction tx = qUtils::DecodeDummyTx(wallet, psbt, warningmsg);
                if((int)EWARNING::WarningType::NONE_MSG == warningmsg.type()){
                    transactionPtr()->setNunchukTransaction(tx);
                    transactionPtr()->setWalletId(wallet_id());
                    auto signers = transactionPtr()->singleSignersAssigned();
                    for (auto ptr: signers->fullList()) {
                        if (ptr->signerSigned()) {
                            QString signature = qUtils::GetPartialSignature(ptr->originSingleSigner(), QString::fromStdString(tx.get_psbt()));
                            signatures[ptr->masterFingerPrint()] = signature.isEmpty() ? ptr->signature() : signature;
                        }
                    }
                    emit AppModel::instance()->finishedSigningTransaction();
                    requestUpdateDummyTx(signatures);
                    return true;
                }
            }
        }
        if((int)EWARNING::WarningType::NONE_MSG != warningmsg.type()){
            AppModel::instance()->showToast(warningmsg.code(), warningmsg.what(), (EWARNING::WarningType)warningmsg.type());
        }
        emit AppModel::instance()->finishedSigningTransaction();
    }
    return false;
}

bool QUserWalletDummyTx::requestSignTxViaFile(const QString &filepath)
{
    if(transactionPtr()){
        QString psbt = ImportPsbtViaFile(filepath);
        DBG_INFO << psbt;
        if(psbt != ""){
            // Convert tx
            QMap<QString, QString> signatures = transactionPtr()->signatures();
            QWarningMessage warningmsg;
            nunchuk::Wallet wallet = bridge::nunchukGetOriginWallet(wallet_id(), warningmsg);
            if((int)EWARNING::WarningType::NONE_MSG == warningmsg.type()){
                warningmsg.resetWarningMessage();
                nunchuk::Transaction tx = qUtils::DecodeDummyTx(wallet, psbt, warningmsg);
                if((int)EWARNING::WarningType::NONE_MSG == warningmsg.type()){
                    transactionPtr()->setNunchukTransaction(tx);
                    transactionPtr()->setWalletId(wallet_id());
                    auto signers = transactionPtr()->singleSignersAssigned();
                    for (auto ptr: signers->fullList()) {
                        if (ptr->signerSigned()) {
                            QString signature = qUtils::GetPartialSignature(ptr->originSingleSigner(), QString::fromStdString(tx.get_psbt()));
                            signatures[ptr->masterFingerPrint()] = signature.isEmpty() ? ptr->signature() : signature;
                        }
                    }
                    emit AppModel::instance()->finishedSigningTransaction();
                    requestUpdateDummyTx(signatures);
                    return true;
                }
            }
            if((int)EWARNING::WarningType::NONE_MSG != warningmsg.type()){
                AppModel::instance()->showToast(warningmsg.code(), warningmsg.what(), (EWARNING::WarningType)warningmsg.type());
            }
        }
        emit AppModel::instance()->finishedSigningTransaction();
    }
    return false;
}

QStringList QUserWalletDummyTx::ExportPsbtViaQR(QWarningMessage &msg)
{
    return QWalletDummyTx::ExportPsbtViaQR(bodyString(), msg);
}

void QUserWalletDummyTx::requestUpdateDummyTx(const QMap<QString, QString> &signatures)
{
    DBG_INFO << signatures;
    if(!signatures.isEmpty()){
        transactionPtr()->setSignatures(signatures);
        emit transactionPtr()->singleSignerAssignedChanged();
        QJsonObject reqiredSignatures = ServiceSetting::instance()->servicesTagPtr()->reqiredSignaturesJs();
        reqiredSignatures["required_signatures"] = (m_required_signatures - signatures.size());
        ServiceSetting::instance()->servicesTagPtr()->setReqiredSignatures(reqiredSignatures);
        emit transactionInfoChanged();
        if (auto w = walletInfoPtr()) {
            int flow = w->flow();
            switch ((AlertEnum::E_Alert_t)flow) {
            case AlertEnum::E_Alert_t::SERVICE_TAG_INHERITANCE_PLAN_CREATE:
                if (auto plan = w->inheritancePlanPtr()) {
                    if (pending_signatures() == 0) {
                        if (plan->InheritancePlanCreateSucceed()) {
                            QQuickViewer::instance()->sendEvent(E::EVT_ONS_CLOSE_REQUEST);
                        }
                    }
                }
                break;
            case AlertEnum::E_Alert_t::SERVICE_TAG_INHERITANCE_PLAN_UPDATE:
                if (auto plan = w->inheritancePlanPtr()) {
                    if (pending_signatures() == 0) {
                        if (plan->InheritancePlanUpdateSucceed()) {
                            QQuickViewer::instance()->sendEvent(E::EVT_ONS_CLOSE_REQUEST);
                        }
                    }
                }
                break;
            case AlertEnum::E_Alert_t::SERVICE_TAG_INHERITANCE_PLAN_CANCEL:
                if (auto plan = w->inheritancePlanPtr()) {
                    if (pending_signatures() == 0) {
                        if (plan->InheritancePlanCancelSucceed()) {
                            QQuickViewer::instance()->sendEvent(E::EVT_ONS_CLOSE_REQUEST);
                        }
                    }
                }
                break;
            default:
                break;
            }
        }
    }
}
