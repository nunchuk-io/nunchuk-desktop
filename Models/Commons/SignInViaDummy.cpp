#include "SignInViaDummy.h"
#include "ViewsEnums.h"
#include "Servers/Draco.h"
#include "Premiums/QWalletManagement.h"
#include "Premiums/QGroupWallets.h"
#include "Premiums/QGroupWalletDummyTx.h"
#include "Premiums/QWalletDummyTx.h"
#include "Premiums/QGroupDashboard.h"
#include "WalletModel.h"
#include "AppSetting.h"
#include "AppModel.h"

bool SignInViaDummy::SignInUsingXPUBorWallet(const QString &bsms)
{
    QJsonObject output;
    QString errormsg = "";
    bool ret {false};
    ret = Draco::instance()->SignInUsingXPUBorWallet(bsms, output, errormsg);
    DBG_INFO << ret << output;
    if(ret){
        QJsonObject dummy_transaction = output["dummy_transaction"].toObject();
        m_dummy_SignIn = output;
        convertWallet();
        if (dashboard()) {
            dashboard()->setFlow((int)AlertEnum::E_Alert_t::WELCOME_SIGN_IN_SIGNATURE_XPUB);
            if (auto dummy = dashboard()->groupDummyTxPtr()) {
                dummy->setDummyTxData(dummy_transaction);
            }
        }
        QEventProcessor::instance()->sendEvent(E::EVT_HEALTH_CHECK_STARTING_REQUEST);
    }
    return ret;
}

void SignInViaDummy::initWallet()
{
    qUtils::SetChain((nunchuk::Chain)AppSetting::instance()->primaryServer());
    AppModel::instance()->setWalletInfo(QWalletPtr(new Wallet()), true);
}

void SignInViaDummy::initializeWalletData(const QString& walletId)
{
    WalletsMng->CreateData<QGroupWalletHealthCheckPtr>(walletId);
    WalletsMng->CreateData<QWalletDummyTxPtr>(walletId);
    WalletsMng->initSignInWallet(walletId);
    auto dash = WalletsMng->data<QGroupDashboardPtr>(walletId);
    QGroupWallets::instance()->SignInDashBoard(dash);
}

void SignInViaDummy::convertWallet()
{
    AppModel::instance()->timerFeeRatesHandle();
    m_card_ids.clear();
    auto getSigner = [this](const QJsonObject &js)->nunchuk::SingleSigner{
        QString  name               = js["name"].toString();
        QString  xfp                = js["xfp"].toString();
        QString  derivation_path    = js["derivation_path"].toString();
        QString  type               = js["type"].toString();
        QJsonArray tags = js["tags"].toArray();
        QJsonObject tapsigner = js["tapsigner"].toObject();
        if (!tapsigner.isEmpty()) {
            m_card_ids.insert(xfp, tapsigner["card_id"].toString());
        }
        nunchuk::SingleSigner singlesinger(name.toStdString(),
                                           "",
                                           "",
                                           derivation_path.toStdString(),
                                           xfp.toStdString(),
                                           std::time(0),
                                           "",
                                           false,
                                           SignerTypeFromStr(type.toStdString()));
        std::vector<nunchuk::SignerTag> _tags;
        for(auto tag : tags){
            _tags.push_back(SignerTagFromStr(tag.toString().toStdString()));
        }
        singlesinger.set_tags(_tags);
        return singlesinger;
    };
    std::vector<nunchuk::SingleSigner> signers;
    QJsonObject wallet = m_dummy_SignIn["wallet"].toObject();
    QJsonArray list = wallet["signers"].toArray();
    for(auto signer : list){
        signers.push_back(getSigner(signer.toObject()));
    }
    m_wallet = nunchuk::Wallet(false);
    m_wallet.set_signers(signers);
    if (auto w = AppModel::instance()->walletInfoPtr()) {
        w->convert(m_wallet);
        if (auto signers = w->singleSignersAssigned()) {
            signers->setCardIDList(m_card_ids);
        }
        initializeWalletData(w->walletId());
    }
}

QGroupDashboardPtr SignInViaDummy::dashboard() const
{
    return QGroupWallets::instance()->dashboardInfoPtr();
}

bool SignInViaDummy::isSignIn() const {
    QJsonObject dummy_transaction = m_dummy_SignIn["dummy_transaction"].toObject();
    int pending_signatures = dummy_transaction["pending_signatures"].toInt();
    return pending_signatures > 0;
}

QJsonObject SignInViaDummy::getSigner(const QString &xfp)
{
    QJsonObject wallet = m_dummy_SignIn["wallet"].toObject();
    QJsonArray signers = wallet["signers"].toArray();
    for(auto signer : signers){
        if(signer.toObject()["xfp"].toString() == xfp){
            return signer.toObject();
        }
    }
    return {};
}

void SignInViaDummy::SignInCreateDummyTransaction()
{
    QWarningMessage msg;
    QJsonObject dummy_transaction = m_dummy_SignIn["dummy_transaction"].toObject();
    QString psbt = dummy_transaction["psbt"].toString();
    tx = qUtils::DecodeDummyTx(m_wallet, psbt, msg);
    tx.set_status(nunchuk::TransactionStatus::PENDING_SIGNATURES);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        QDummyTransactionPtr trans = QDummyTransactionPtr(new QDummyTransaction());
        trans->setNunchukTransaction(tx);
        trans->setWalletId(QString::fromStdString(m_wallet.get_id()));
        trans->setTxJson(dummy_transaction);
        if (auto signers = trans->singleSignersAssigned()) {
            signers->setCardIDList(m_card_ids);
        }
        if(trans){
            if (auto dummy = dashboard()->groupDummyTxPtr()) {
                dummy->setTransactionInfo(trans);
                QStringList xfps;
                for (auto s : m_wallet.get_signers()) {
                    if (s.get_type() == nunchuk::SignerType::SOFTWARE) {
                        xfps.append(QString::fromStdString(s.get_master_fingerprint()));
                    }
                }
                trans->setHideSignBtns(xfps);
                emit dummy->transactionInfoChanged();
            }
            emit trans->nunchukTransactionChanged();
        }
    }
    else{
        AppModel::instance()->showToast(msg.code(), msg.what(), (EWARNING::WarningType)msg.type());
    }
}

bool SignInViaDummy::SignInRequestSignTx(const QString &xfp)
{
    QWarningMessage warningmsg;
    nunchuk::SingleSigner signer = *std::find_if(m_wallet.get_signers().begin(), m_wallet.get_signers().end(), [xfp](const nunchuk::SingleSigner &s) {
        return s.get_master_fingerprint() == xfp.toStdString();
    });
    std::vector<nunchuk::Device> devices = qUtils::GetDevices(bridge::hwiPath(), warningmsg);
    auto it = std::find_if(devices.begin(), devices.end(), [xfp](const nunchuk::Device &d) {
        return d.get_master_fingerprint() == xfp.toStdString();
    });
    nunchuk::Device device;
    if (it != devices.end()) {
        device = *it;
    }
    else {
        emit AppModel::instance()->finishedSigningTransaction();
        AppModel::instance()->showToast(0, "There not device", EWARNING::WarningType::ERROR_MSG);
        return false;
    }

    QJsonObject dummy_transaction = m_dummy_SignIn["dummy_transaction"].toObject();
    QString psbt = dummy_transaction["psbt"].toString();

    QMap<QString, QString> signatures;
    if((int)EWARNING::WarningType::NONE_MSG == warningmsg.type()){
        warningmsg.resetWarningMessage();
        QString signature = "";
        DBG_INFO << (int)signer.get_type();
        switch (signer.get_type()) {
        case nunchuk::SignerType::HARDWARE:
        case nunchuk::SignerType::SOFTWARE:
        case nunchuk::SignerType::COLDCARD_NFC:
        {
            QString psbt_signed = qUtils::SignPsbt(bridge::hwiPath(), device, psbt, warningmsg);
            if((int)EWARNING::WarningType::NONE_MSG == warningmsg.type()){
                signature = qUtils::GetPartialSignature(signer, psbt_signed);
                if((int)EWARNING::WarningType::NONE_MSG == warningmsg.type()){
                    signatures[xfp] = signature;
                }
            }
            break;
        }
        default:
            // Not support -> Show popup
            break;
        }
        emit AppModel::instance()->finishedSigningTransaction();
        if(!signatures.isEmpty()){
            SignInRequestUpdateDummyTx(signatures);
        }
        else {
            DBG_INFO << warningmsg.type();
        }
        return true;
    }
    else {
        emit AppModel::instance()->finishedSigningTransaction();
        AppModel::instance()->showToast(warningmsg.code(), warningmsg.what(), (EWARNING::WarningType)warningmsg.type());
    }
    return false;
}

bool SignInViaDummy::SignInRequestSignTxViaQR(const QStringList &qrtags)
{
    if(auto dummy = dashboard()->groupDummyTxPtr()){
        if(auto trans = dummy->transactionPtr()){
            QJsonObject dummy_transaction = m_dummy_SignIn["dummy_transaction"].toObject();
            QWarningMessage warningmsg;
            QMap<QString, QString> signatures;
            QMap<QString, QString> keys;
            QString psbt = qUtils::ParseQRTransaction(qrtags, warningmsg);
            if((int)EWARNING::WarningType::NONE_MSG == warningmsg.type() && psbt != ""){
                warningmsg.resetWarningMessage();
                if((int)EWARNING::WarningType::NONE_MSG == warningmsg.type()){
                    warningmsg.resetWarningMessage();
                    nunchuk::Transaction tx = qUtils::DecodeDummyTx(m_wallet, psbt, warningmsg);
                    if((int)EWARNING::WarningType::NONE_MSG == warningmsg.type()){
                        trans->setNunchukTransaction(tx);
                        trans->setWalletId(QString::fromStdString(m_wallet.get_id()));
                        trans->setTxJson(dummy_transaction);
                        auto signers = trans->singleSignersAssigned();
                        for (auto ptr: signers->fullList()) {
                            if (ptr->signerSigned()) {
                                QString signature = qUtils::GetPartialSignature(ptr->originSingleSigner(), QString::fromStdString(tx.get_psbt()));
                                signatures[ptr->masterFingerPrint()] = signature.isEmpty() ? ptr->signature() : signature;
                                keys[ptr->masterFingerPrint()] = QString::fromStdString(ptr->originSingleSigner().get_name());
                            }
                        }
                        if(!signatures.isEmpty()){
                            SignInRequestUpdateDummyTx(signatures);
                        }
                        emit dummy->transactionInfoChanged();
                    }
                }
            }
            if((int)EWARNING::WarningType::NONE_MSG != warningmsg.type()){
                AppModel::instance()->showToast(warningmsg.code(), warningmsg.what(), (EWARNING::WarningType)warningmsg.type());
            }
            emit AppModel::instance()->finishedSigningTransaction();
        }
    }
    return false;
}

bool SignInViaDummy::SignInRequestSignTxViaFile(const QString &filepath)
{
    if(auto dummy = dashboard()->groupDummyTxPtr()){
        if(auto trans = dummy->transactionPtr()){
            QJsonObject dummy_transaction = m_dummy_SignIn["dummy_transaction"].toObject();
            QMap<QString, QString> signatures;
            QString psbt = qUtils::ImportDataViaFile(filepath);
            if(psbt != ""){
                // Convert tx
                QWarningMessage warningmsg;
                if((int)EWARNING::WarningType::NONE_MSG == warningmsg.type()){
                    warningmsg.resetWarningMessage();
                    nunchuk::Transaction tx = qUtils::DecodeDummyTx(m_wallet, psbt, warningmsg);
                    if((int)EWARNING::WarningType::NONE_MSG == warningmsg.type()){
                        trans->setNunchukTransaction(tx);
                        trans->setWalletId(QString::fromStdString(m_wallet.get_id()));
                        trans->setTxJson(dummy_transaction);
                        auto signers = trans->singleSignersAssigned();
                        for (auto ptr: signers->fullList()) {
                            if (ptr->signerSigned()) {
                                QString signature = qUtils::GetPartialSignature(ptr->originSingleSigner(), QString::fromStdString(tx.get_psbt()));
                                signatures[ptr->masterFingerPrint()] = signature.isEmpty() ? ptr->signature() : signature;
                            }
                        }
                        if(!signatures.isEmpty()){
                            SignInRequestUpdateDummyTx(signatures);
                        }
                    }
                }
                if((int)EWARNING::WarningType::NONE_MSG != warningmsg.type()){
                    AppModel::instance()->showToast(warningmsg.code(), warningmsg.what(), (EWARNING::WarningType)warningmsg.type());
                }
            }
            emit AppModel::instance()->finishedSigningTransaction();
        }
    }
    return false;
}

QStringList SignInViaDummy::ExportPsbtViaQR()
{
    if(auto dummy = dashboard()->groupDummyTxPtr()){
        if(auto trans = dummy->transactionPtr()){
            QWarningMessage msg;
            return qUtils::ExportQRTransaction(trans->psbt(), msg);
        }
    }
    return {};
}

QStringList SignInViaDummy::ExportPsbtViaBBQR()
{
    if(auto dummy = dashboard()->groupDummyTxPtr()){
        if(auto trans = dummy->transactionPtr()){
            QWarningMessage msg;
            return qUtils::ExportBBQRTransaction(trans->psbt(), msg);
        }
    }
    return {};
}

void SignInViaDummy::SignInRequestUpdateDummyTx(const QMap<QString, QString>& signatures)
{
    if(auto dummy = dashboard()->groupDummyTxPtr()){
        if(auto trans = dummy->transactionPtr()){
            QStringList authorizations;
            authorizations.clear();
            QWarningMessage msg;
            QJsonObject output;
            QJsonObject dummy_transaction = m_dummy_SignIn["dummy_transaction"].toObject();
            QString id = dummy_transaction["id"].toString();
            QString errormsg = "";
            QStringList xfps = signatures.keys();
            for (QString xfp : xfps) {
                QString signature = signatures[xfp];
                QString authorization = qUtils::CreateRequestToken(signature, xfp, msg);
                authorizations.append(authorization);
            }
            bool ret = Draco::instance()->SignDummyTransactionToSignIn(id, authorizations, output, errormsg);
            if(ret){
                DBG_INFO << output;
                m_dummy_SignIn = output;
                // GO TO KEY STATUS SCREEN
                if (isSignIn()) {
                    QJsonObject dummy_transaction = output["dummy_transaction"].toObject();
                    trans->setTxJson(dummy_transaction);
                    emit trans->nunchukTransactionChanged();
                    if (auto dummy = dashboard()->groupDummyTxPtr()) {
                        dummy->setDummyTxData(dummy_transaction);
                    }
                }
                if (!isSignIn()) {
                    QJsonObject token = output["token"].toObject();
                    Draco::instance()->setDeviceId(token["deviceId"].toString());
                    Draco::instance()->setDracoToken(token["tokenId"].toString());
                    Draco::instance()->setExpireSec(token["expireInSeconds"].toInt());
                    // ClientController::instance()->loginHandler()->setUserid(token["userId"].toString());
                    Draco::instance()->getMe();
                    auto user = CLIENT_INSTANCE->getMe();
                    if (!user.name.isEmpty()) {
                        Draco::instance()->setUid(user.name);
                    } else if (!user.username.isEmpty()) {
                        Draco::instance()->setUid(user.username);
                    } else {
                        Draco::instance()->setUid(user.email);
                    }
                    QEventProcessor::instance()->sendEvent(E::EVT_ONS_CLOSE_ALL_REQUEST);
                    QEventProcessor::instance()->sendEvent(E::EVT_GOTO_HOME_WALLET_TAB);
                    emit AppModel::instance()->signViaSingature();
                }
            }
        }
    }
}
