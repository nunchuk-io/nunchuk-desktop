#include "QGroupWalletDummyTx.h"
#include "Servers/Byzantine.h"
#include "bridgeifaces.h"
#include "qUtils.h"
#include "Servers/Draco.h"
#include <QFile>
#include <QTextStream>
#include "QGroupWalletHealthCheck.h"
#include "Premiums/QGroupWallets.h"
#include "Premiums/QServerKey.h"
#include "Premiums/QInheritancePlan.h"
#include "ViewsEnums.h"

QGroupWalletDummyTx::QGroupWalletDummyTx(WalletId wallet_id)
    : QWalletDummyTx(wallet_id)
{
    connect(this, &QGroupWalletDummyTx::requestSignout, ClientController::instance(), &ClientController::requestSignout, Qt::QueuedConnection);
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

void QGroupWalletDummyTx::setDummyTxData(QJsonObject data)
{
    m_tx = data;
    if(!m_tx.isEmpty() && dashBoardPtr()){
        if (auto dashboard = dashBoardPtr()) {
            dashboard->setDummyTxAlert(data);
        }
        if (auto serverKey = serverKeyPtr()) {
            serverKey->UpdateFromDummyTx(m_tx);
        }
        if (auto inheritance = inheritancePlanPtr()) {
            inheritance->UpdateFromDummyTx(m_tx);
        }
        // Convert tx
        QString request_body    = m_tx["request_body"].toString();
        QString wallet_local_id = m_tx["wallet_local_id"].toString();
        QWarningMessage warningmsg;
        nunchuk::Wallet wallet = bridge::nunchukGetOriginWallet(wallet_local_id, warningmsg);
        if((int)EWARNING::WarningType::NONE_MSG == warningmsg.type()){
            warningmsg.resetWarningMessage();
            QString tx_to_sign = qUtils::GetHealthCheckDummyTx(wallet, request_body, warningmsg);
            if((int)EWARNING::WarningType::NONE_MSG == warningmsg.type()){
                warningmsg.resetWarningMessage();
                nunchuk::Transaction tx = qUtils::DecodeDummyTx(wallet, tx_to_sign, warningmsg);
                transactionPtr()->setNunchukTransaction(tx);
                transactionPtr()->setWalletId(wallet_local_id);
                transactionPtr()->setTxJson(m_tx);
            }
        }
        if (transactionPtr()) {
            emit transactionPtr()->singleSignerAssignedChanged();
        }
        emit transactionInfoChanged();
    }
}

bool QGroupWalletDummyTx::requestForceSyncTx(const QString &group_id, const QString &wallet_id, const QString &txid)
{
    bool ret = false;
    QJsonObject output;
    QString     errormsg = "";
    if (isUserWallet()) {
        ret = Draco::instance()->GetDummyTransaction(wallet_id, txid, output, errormsg);
    }
    else {
        ret = Byzantine::instance()->GetDummyTransaction(group_id, wallet_id, txid, output, errormsg);
    }
    if(ret){
        QJsonObject dummy_transaction = output["dummy_transaction"].toObject();
        setDummyTxData(dummy_transaction);

        AppModel::instance()->showToast(0, "Transaction updated", EWARNING::WarningType::SUCCESS_MSG);
    }
    else {
        AppModel::instance()->showToast(0, "Transaction not updated", EWARNING::WarningType::ERROR_MSG);
    }
    return ret;
}

bool QGroupWalletDummyTx::requestSignTx(const QString &xfp)
{
    if(transactionPtr()){
        setCurrentXfp(xfp);
        QWarningMessage warningmsg;
        nunchuk::Wallet wallet = bridge::nunchukGetOriginWallet(wallet_id(), warningmsg);
        nunchuk::SingleSigner signer = *std::find_if(wallet.get_signers().begin(), wallet.get_signers().end(), [xfp](const nunchuk::SingleSigner &s) {
            return s.get_master_fingerprint() == xfp.toStdString();
        });
        QMap<QString, QString> signatures;
        QMap<QString, QString> signers;
        if((int)EWARNING::WarningType::NONE_MSG == warningmsg.type()){
            warningmsg.resetWarningMessage();
            QString request_body    = m_tx["request_body"].toString();
            QString tx_to_sign = qUtils::GetHealthCheckDummyTx(wallet, request_body, warningmsg);
            QString signature = "";
            DBG_INFO << (int)signer.get_type();
            switch (signer.get_type()) {
            case nunchuk::SignerType::HARDWARE:
            case nunchuk::SignerType::SOFTWARE:
            case nunchuk::SignerType::COLDCARD_NFC:
            {
                signature = bridge::SignHealthCheckMessage(signer, tx_to_sign, warningmsg);
                if((int)EWARNING::WarningType::NONE_MSG == warningmsg.type()){
                    signatures[xfp] = signature;
                    signers[xfp] = QString::fromStdString(signer.get_name());
                }
                break;
            }
            default:
                // Not support -> Show popup
                break;
            }
            emit AppModel::instance()->finishedSigningTransaction();
            if(!signatures.isEmpty()){
                requestUpdateDummyTx(signatures, signers);
            }
            else {
                DBG_INFO << warningmsg.type();
                emit registerWalletWithDevice();
            }
            return true;
        }
        else {
            emit AppModel::instance()->finishedSigningTransaction();
            AppModel::instance()->showToast(warningmsg.code(), warningmsg.what(), (EWARNING::WarningType)warningmsg.type());
        }
    }
    return false;
}

bool QGroupWalletDummyTx::requestSignTxViaQR(const QStringList &qrtags)
{
    QWarningMessage warningmsg;
    QMap<QString, QString> signatures;
    QMap<QString, QString> keys;
    QString psbt = qUtils::ParseQRTransaction(qrtags, warningmsg);
    if((int)EWARNING::WarningType::NONE_MSG == warningmsg.type() && psbt != "" && dashBoardPtr()){
        warningmsg.resetWarningMessage();
        nunchuk::Wallet wallet = bridge::nunchukGetOriginWallet(dashBoardPtr()->wallet_id(), warningmsg);
        if((int)EWARNING::WarningType::NONE_MSG == warningmsg.type()){
            warningmsg.resetWarningMessage();
            nunchuk::Transaction tx = qUtils::DecodeDummyTx(wallet, psbt, warningmsg);
            if((int)EWARNING::WarningType::NONE_MSG == warningmsg.type()){
                transactionPtr()->setNunchukTransaction(tx);
                transactionPtr()->setWalletId(dashBoardPtr()->wallet_id());
                transactionPtr()->setTxJson(m_tx);
                auto signers = transactionPtr()->singleSignersAssigned();
                for (auto ptr: signers->fullList()) {
                    if (ptr->signerSigned()) {
                        QString signature = qUtils::GetPartialSignature(ptr->originSingleSigner(), QString::fromStdString(tx.get_psbt()));
                        signatures[ptr->masterFingerPrint()] = signature.isEmpty() ? ptr->signature() : signature;
                        keys[ptr->masterFingerPrint()] = QString::fromStdString(ptr->originSingleSigner().get_name());
                    }
                }
                if(!signatures.isEmpty()){
                    requestUpdateDummyTx(signatures, keys);
                }
                emit transactionInfoChanged();
            }
        }
    }
    if((int)EWARNING::WarningType::NONE_MSG != warningmsg.type()){
        AppModel::instance()->showToast(warningmsg.code(), warningmsg.what(), (EWARNING::WarningType)warningmsg.type());
    }
    emit AppModel::instance()->finishedSigningTransaction();
    return false;
}

bool QGroupWalletDummyTx::requestSignTxViaFile(const QString &filepath)
{
    QMap<QString, QString> signatures;
    QMap<QString, QString> keys;
    QString psbt = ImportPsbtViaFile(filepath);
    if(psbt != "" && dashBoardPtr()){
        // Convert tx
        QWarningMessage warningmsg;
        nunchuk::Wallet wallet = bridge::nunchukGetOriginWallet(dashBoardPtr()->wallet_id(), warningmsg);
        if((int)EWARNING::WarningType::NONE_MSG == warningmsg.type()){
            warningmsg.resetWarningMessage();
            nunchuk::Transaction tx = qUtils::DecodeDummyTx(wallet, psbt, warningmsg);
            if((int)EWARNING::WarningType::NONE_MSG == warningmsg.type()){
                transactionPtr()->setNunchukTransaction(tx);
                transactionPtr()->setWalletId(dashBoardPtr()->wallet_id());
                transactionPtr()->setTxJson(m_tx);
                auto signers = transactionPtr()->singleSignersAssigned();
                for (auto ptr: signers->fullList()) {
                    if (ptr->signerSigned()) {
                        QString signature = qUtils::GetPartialSignature(ptr->originSingleSigner(), QString::fromStdString(tx.get_psbt()));
                        signatures[ptr->masterFingerPrint()] = signature.isEmpty() ? ptr->signature() : signature;
                        keys[ptr->masterFingerPrint()] = QString::fromStdString(ptr->originSingleSigner().get_name());
                    }
                }
                if(!signatures.isEmpty()){
                    requestUpdateDummyTx(signatures, keys);
                }
            }
        }
        if((int)EWARNING::WarningType::NONE_MSG != warningmsg.type()){
            AppModel::instance()->showToast(warningmsg.code(), warningmsg.what(), (EWARNING::WarningType)warningmsg.type());
        }
    }
    emit AppModel::instance()->finishedSigningTransaction();
    return false;
}

void QGroupWalletDummyTx::requestUpdateDummyTx(const QMap<QString, QString> &signatures, const QMap<QString, QString> &signers)
{
    if (auto dashboard = dashBoardPtr()){
        QStringList authorizations;
        authorizations.clear();
        QWarningMessage msg;
        QJsonObject output;
        QString errormsg = "";
        QString group_id = dashboard->groupId();
        QString wallet_id = dashboard->wallet_id();
        QString txid = tx_id();
        QStringList xfps = signatures.keys();
        for (QString xfp : xfps) {
            QString signature = signatures[xfp];
            QString authorization = qUtils::CreateRequestToken(signature, xfp, msg);
            authorizations.append(authorization);
        }
        DBG_INFO << authorizations;
        bool ret {false};
        if (isUserWallet()) {
            ret = Draco::instance()->UpdateDummyTransaction(wallet_id, authorizations, txid, output, errormsg);
        }
        else {
            ret = Byzantine::instance()->UpdateDummyTransaction(group_id, wallet_id, authorizations, txid, output, errormsg);
        }
        if(ret){
            DBG_INFO << output;
            dashboard->GetAlertsInfo();
            dashboard->GetHealthCheckInfo();
            // GO TO KEY STATUS SCREEN
            QJsonObject dummy_transaction = output["dummy_transaction"].toObject();
            transactionPtr()->setTxJson(dummy_transaction);
            emit transactionPtr()->singleSignerAssignedChanged();
            QString type = dummy_transaction["type"].toString();
            int pending_signatures = dummy_transaction["pending_signatures"].toInt();
            int flow = StringToInt(type);
            switch ((AlertEnum::E_Alert_t)flow) {
            case AlertEnum::E_Alert_t::HEALTH_CHECK_REQUEST:
            case AlertEnum::E_Alert_t::HEALTH_CHECK_PENDING:
            case AlertEnum::E_Alert_t::HEALTH_CHECK_STATUS:
            {
                if (dashboard->flow() == (int)AlertEnum::E_Alert_t::GROUP_WALLET_SETUP) {
                    dashboard->setConfigFlow("accessing-wallet-configuration");
                    dashboard->registerKeyDone();
                    QEventProcessor::instance()->sendEvent(E::EVT_SHOW_GROUP_WALLET_CONFIG_REQUEST);
                    AppModel::instance()->showToast(0, "The key has been claimed", EWARNING::WarningType::SUCCESS_MSG);
                }
                else {
                    QEventProcessor::instance()->sendEvent(E::EVT_KEY_HEALTH_CHECK_STATUS_REQUEST);
                    QString keyName = QString("%1 %2 healthy").arg(signers.values().join(", ")).arg(signers.count() > 1 ? "are" : "is");
                    AppModel::instance()->showToast(0, keyName, EWARNING::WarningType::SUCCESS_MSG);
                }
                break;
            }
            case AlertEnum::E_Alert_t::UPDATE_SECURITY_QUESTIONS:
            case AlertEnum::E_Alert_t::UPDATE_SERVER_KEY:
            case AlertEnum::E_Alert_t::CREATE_INHERITANCE_PLAN:
            case AlertEnum::E_Alert_t::UPDATE_INHERITANCE_PLAN:
            case AlertEnum::E_Alert_t::CANCEL_INHERITANCE_PLAN:
            case AlertEnum::E_Alert_t::REQUEST_INHERITANCE_PLANNING:
            {
                DBG_INFO << dashBoardPtr()->flow();
                if (dashBoardPtr()->flow() == (int)AlertEnum::E_Alert_t::SERVICE_TAG_POLICY_UPDATE
                    || dashBoardPtr()->flow() == (int)AlertEnum::E_Alert_t::SERVICE_TAG_INHERITANCE_PLAN_UPDATE
                    || dashBoardPtr()->flow() == (int)AlertEnum::E_Alert_t::SERVICE_TAG_INHERITANCE_PLAN_CANCEL
                    || dashBoardPtr()->flow() == (int)AlertEnum::E_Alert_t::SERVICE_TAG_UPDATE_SECURITY_QUESTION) {
                    QEventProcessor::instance()->sendEvent(E::EVT_ONS_CLOSE_REQUEST);
                    dashBoardPtr()->setShowDashBoard(true);
                    int wallet_index = AppModel::instance()->walletListPtr()->getWalletIndexById(dashBoardPtr()->wallet_id());
                    AppModel::instance()->setWalletListCurrentIndex(wallet_index);
                }
                else {
                    QEventProcessor::instance()->sendEvent(E::EVT_DUMMY_TRANSACTION_INFO_BACK);
                }
                QString msg_name = QString("Transaction updated");
                if (pending_signatures > 0) {
                    AppModel::instance()->showToast(0, msg_name, EWARNING::WarningType::SUCCESS_MSG);
                }
                else {
                    QString msg_approved = textForToast(flow);
                    AppModel::instance()->showToast(0, msg_name, EWARNING::WarningType::SUCCESS_MSG);
                    AppModel::instance()->showToast(0, msg_approved, EWARNING::WarningType::SUCCESS_MSG);
                }
                break;
            }

            case AlertEnum::E_Alert_t::KEY_RECOVERY_REQUEST:
            case AlertEnum::E_Alert_t::KEY_RECOVERY_APPROVED:
            {
                dashBoardPtr()->setFlow(flow);
                QEventProcessor::instance()->sendEvent(E::EVT_DASHBOARD_ALERT_SUCCESS_REQUEST);
                break;
            }
            case AlertEnum::E_Alert_t::CANCEL_RECURRING_PAYMENT:
            case AlertEnum::E_Alert_t::CREATE_RECURRING_PAYMENT: {
                QEventProcessor::instance()->sendEvent(E::EVT_DUMMY_TRANSACTION_INFO_BACK);
                QString msg_name = QString("Transaction updated");
                if (pending_signatures > 0) {
                    AppModel::instance()->showToast(0, msg_name, EWARNING::WarningType::SUCCESS_MSG);
                }
                else {
                    QString msg_approved = textForToast(flow);
                    AppModel::instance()->showToast(0, msg_approved, EWARNING::WarningType::SUCCESS_MSG);
                }
            }
            case AlertEnum::E_Alert_t::CHANGE_EMAIL: {
                QEventProcessor::instance()->sendEvent(E::EVT_ONS_CLOSE_ALL_REQUEST);
                emit requestSignout();
                QString msg_name = QString("Email has been changed. Please sign in again.");
                AppModel::instance()->showToast(0, msg_name, EWARNING::WarningType::SUCCESS_MSG);
                break;
            }
            default:
                break;
            }
        }
    }
}

void QGroupWalletDummyTx::finishScanDevices()
{
    if (!transactionPtr()) return;
    emit transactionPtr()->singleSignerAssignedChanged();
}

QString QGroupWalletDummyTx::textForToast(int flow)
{
    switch ((AlertEnum::E_Alert_t)flow) {
    case AlertEnum::E_Alert_t::UPDATE_SERVER_KEY: return "The platform key policies have been updated";
    case AlertEnum::E_Alert_t::CREATE_INHERITANCE_PLAN: return "The inheritance plan has been created";
    case AlertEnum::E_Alert_t::UPDATE_INHERITANCE_PLAN: return "The inheritance plan has been updated";
    case AlertEnum::E_Alert_t::CANCEL_INHERITANCE_PLAN: return "The inheritance plan has been canceled";
    case AlertEnum::E_Alert_t::REQUEST_INHERITANCE_PLANNING: return "Inheritance planning request approved";
    case AlertEnum::E_Alert_t::RECURRING_PAYMENT_APPROVED: return "The recurring payment has been approved";
    case AlertEnum::E_Alert_t::CREATE_RECURRING_PAYMENT: return "Recurring payment approved";
    case AlertEnum::E_Alert_t::CANCEL_RECURRING_PAYMENT: return "Recurring payment canceled";
    case AlertEnum::E_Alert_t::UPDATE_SECURITY_QUESTIONS: return "Security questions updated";
    default:
        break;
    }
    return "";
}

QString QGroupWalletDummyTx::tx_id() const
{
    return m_tx["id"].toString();
}

QStringList QGroupWalletDummyTx::ExportPsbtViaQR(QWarningMessage &msg)
{
    return QWalletDummyTx::ExportPsbtViaQR(m_tx["request_body"].toString(), msg);
}

QVariant QGroupWalletDummyTx::requester_user()
{
    if(auto dash = dashBoardPtr()){
        QString requester_user_id = m_tx["requester_user_id"].toString();
        if(requester_user_id != ""){
            for(const auto item : dashBoardPtr()->groupInfo()["members"].toArray()) {
                QJsonObject user = item.toObject()["user"].toObject();
                if(qUtils::strCompare(requester_user_id, user["id"].toString())){
                    return QVariant::fromValue(user);
                }
            }
        }
        else {
            QJsonObject payload = dash->alertJson()["payload"].toObject();
            QString membership_id_alert = payload["membership_id"].toString();
            for(const auto item : dash->groupInfo()["members"].toArray()) {
                QString membership_id = item.toObject()["membership_id"].toString();
                QJsonObject user = item.toObject()["user"].toObject();
                if(qUtils::strCompare(membership_id_alert, membership_id)){
                    return QVariant::fromValue(user);
                }
            }
        }
    }
    QJsonObject default_user = {
        {"name", "Someone"},
        {"email", ""}
    };
    return QVariant::fromValue(default_user);
}

QVariant QGroupWalletDummyTx::txJson() const
{
    return QVariant::fromValue(m_tx);
}

bool QGroupWalletDummyTx::CancelDummyTransaction()
{
    if (!dashBoardPtr()) return false;
    QJsonObject output;
    QString errormsg = "";
    QJsonObject payload = dashBoardPtr()->alertJson()["payload"].toObject();
    QString dummy_transaction_id = payload["dummy_transaction_id"].toString();
    bool ret {false};
    if (isUserWallet()) {
        ret = Draco::instance()->CancelDummyTransaction(dashBoardPtr()->wallet_id(), dummy_transaction_id, output, errormsg);
    }
    else {
        ret = Byzantine::instance()->CancelDummyTransaction(dashBoardPtr()->groupId(), dashBoardPtr()->wallet_id(), dummy_transaction_id, output, errormsg);
    }
    DBG_INFO << ret;
    if(ret){

    }
    else {
        //Show toast ?
    }
    return ret;
}

QString QGroupWalletDummyTx::currentXfp() const
{
    if (auto tran = transactionPtr())
        return tran->dummyXfp();
    return {};
}

void QGroupWalletDummyTx::setCurrentXfp(const QString &currentXfp)
{
    if (auto tran = transactionPtr()) {
        tran->setDummyXfp(currentXfp);
    }
}

QJsonObject QGroupWalletDummyTx::tx() const
{
    return m_tx;
}

