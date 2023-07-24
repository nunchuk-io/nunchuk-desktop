#include <QQmlEngine>
#include <QJsonDocument>
#include "bridgeifaces.h"
#include "AppModel.h"
#include "QUserWallets.h"
#include "localization/STR_CPP.h"
#include "ServiceSetting.h"
#include "nunchuckiface.h"
#include "ViewsEnums.h"

QUserWallets::QUserWallets():
    m_passwordToken(""),
    m_secQuesToken("")
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

QUserWallets::~QUserWallets()
{

}

QUserWallets *QUserWallets::instance()
{
    static QUserWallets mInstance;
    return &mInstance;
}

QString QUserWallets::secQuesToken() const
{
    return m_secQuesToken;
}

void QUserWallets::setSecQuesToken(const QString &secQuesToken)
{
    m_secQuesToken = secQuesToken;
}

QString QUserWallets::passwordToken() const
{
    return m_passwordToken;
}

bool QUserWallets::requestVerifyPassword(const QString &password, const int action)
{
    QString errormsg_or_token = "";
    bool ret = Draco::instance()->verifyPasswordToken(password, action, errormsg_or_token);
    QStringList walletIds = AppModel::instance()->getUserWallets();
    if (walletIds.size() > 0) {
        m_wallet_id = walletIds.first();
    } else {
        m_wallet_id = "";
    }
    if(!ret){
        emit verifyPasswordTokenAlert(errormsg_or_token);
    }
    else{
        m_passwordToken = errormsg_or_token;
    }
    return ret;
}

bool QUserWallets::requestLockDownVerifyPassword(const QString &password)
{
    return requestVerifyPassword(password,(int)TARGET_ACTION::EMERGENCY_LOCKDOWN);
}

bool QUserWallets::requestRecoverKeyVerifyPassword(const QString &password)
{
    return requestVerifyPassword(password,(int)TARGET_ACTION::DOWNLOAD_KEY_BACKUP);
}

bool QUserWallets::requestServerKeyVerifyPassword(const QString &password)
{
    return requestVerifyPassword(password,(int)TARGET_ACTION::UPDATE_SERVER_KEY);
}

bool QUserWallets::requestInheritancePlanVerifyPassword(const QString &password)
{
    return requestVerifyPassword(password,(int)TARGET_ACTION::UPDATE_INHERITANCE_PLAN);
}

QTransactionPtr QUserWallets::getDummyTx(const QString& wallet_id, const QString& period_id)
{
    QWarningMessage msgWallet;
    m_wallet = nunchukiface::instance()->GetWallet(wallet_id.toStdString(), msgWallet);
    if((int)EWARNING::WarningType::NONE_MSG == msgWallet.type()){
        QJsonObject bodydata;
        bodydata["period_id"] = period_id;
        bodydata["wallet"] = QString::fromStdString(m_wallet.get_id());
        QJsonObject body;
        body["nonce"] = Draco::instance()->randomNonce();
        body["body"] = bodydata;
        QJsonDocument doc(bodydata);
        QString bodyString(doc.toJson(QJsonDocument::Compact));

        QWarningMessage msgHeathCheck;
        QString tx_to_sign = qUtils::GetHealthCheckDummyTx(m_wallet, bodyString, msgHeathCheck);
        if((int)EWARNING::WarningType::NONE_MSG == msgHeathCheck.type()){
            QWarningMessage msgDecode;
            nunchuk::Transaction tx = qUtils::DecodeDummyTx(m_wallet, tx_to_sign, msgDecode);
            if((int)EWARNING::WarningType::NONE_MSG == msgHeathCheck.type()){
                return bridge::convertTransaction(tx, wallet_id);
                // Sau khi lay tx thi set transaction info
            }
        }
        else{
            //SHOW TOAST
        }
    }
    return NULL;
}

void QUserWallets::signDummyTx(const QString &xfp)
{
    DBG_INFO << xfp;
    if(m_dummyTx && m_dummyTx->singleSignersAssigned()){
        QSingleSignerPtr signer = m_dummyTx->singleSignersAssigned()->getSingleSignerByFingerPrint(xfp);
        int signerType = signer->signerType();
        QString message_to_sign = qUtils::QGenerateRandomMessage();
        DBG_INFO << message_to_sign;
        QWarningMessage signmsg;
        switch (signerType) {
        case (int)ENUNCHUCK::SignerType::HARDWARE:
        case (int)ENUNCHUCK::SignerType::SOFTWARE: {
            QString signature = bridge::SignHealthCheckMessage(signer, message_to_sign, signmsg);
            DBG_INFO << signature;
            if (!signature.isEmpty()) {
                m_signatures.insert(xfp,signature);
            }
        }   break;
        case (int)ENUNCHUCK::SignerType::NFC:
            break;
        case (int)ENUNCHUCK::SignerType::COLDCARD_NFC:
            //auto records = NDEFRecordsFromPSBT(tx_to_sign)
            // sign tx coldcard nfc flow
            // please see nunchuck.atlassian.net/wiki/spaces/NUN/pages/787087378/Coldcard+mk4
            //std::string psbt = NDEFRecordToPSBT(records)
            //signatures[signer.get_master_fingerprint()] = Utils::GetPartialSignature(signer, psbt)
            // tx.set_signer(signer.get_master_fingerprint(), true)
            break;
        default: // Not support for now
            break; // Show exx
        }
        DBG_INFO << m_signatures;
        emit AppModel::instance()->finishedSigningTransaction();
    }
}

void QUserWallets::createDummyTx()
{
    m_dummyTx = getDummyTx(m_wallet_id, m_period_id);
    AppModel::instance()->setTransactionInfo(m_dummyTx);
}

bool QUserWallets::checkDummyTx()
{
    return true;
}

QVariantList QUserWallets::securityQuestions()
{
    return m_questions;
}

bool QUserWallets::createSecurityQuestions()
{
    m_questions.clear();
    QJsonArray questions;
    QString errormsg;
    bool ret = Draco::instance()->secQuesGet(questions, errormsg);
    if (!ret) {
        AppModel::instance()->showToast(0,
                                        errormsg,
                                        EWARNING::WarningType::ERROR_MSG,
                                        STR_CPP_112);
    } else {
        int required_answers = required_question.required_answers == 0 ? 1 : required_question.required_answers;
        QList<int> rIndex;
        while (rIndex.size() != required_answers) {
            int random = rand() % 10;
            if (random < questions.size() && !rIndex.contains(random)) {
                rIndex.append(random);
            }
        }
        m_quesAnswers.clear();
        for (auto index : rIndex) {
            auto ques = questions[index].toVariant();
            m_questions.append(questions[index].toVariant());
            SecurityQuestion answer;
            answer.id = ques.toMap()["id"].toString();
            answer.question = ques.toMap()["question"].toString();
            m_quesAnswers.append(answer);
            qInfo() << ques.toMap();
        }
        emit securityQuestionChanged();
    }
    return ret;
}

void QUserWallets::secQuesAnswer(const QString &id, const QString &answer)
{
    for (int i = 0; i < m_quesAnswers.size(); i ++) {
        if (m_quesAnswers.at(i).id.localeAwareCompare(id) == 0) {
            m_quesAnswers[i].answer = answer;
        }
    }
}

bool QUserWallets::secQuesAnswer()
{
    int correct_answer = 0;
    QString errormsg = "";
    bool ret = Draco::instance()->secQuesAnswer(m_quesAnswers, m_secQuesToken, correct_answer, errormsg);
    if (!ret) {
        emit answerErrorAlert(errormsg);
        return false;
    } else {
        if (required_question.required_answers == 0) {
            return correct_answer;
        } else {
            return m_quesAnswers.size() == correct_answer && correct_answer != 0;
        }
    }
}

QVariantList QUserWallets::periods()
{
    return m_periods;
}

bool QUserWallets::createLockdownPeriods()
{
    m_periods.clear();
    QJsonArray periods;
    QString errormsg;
    bool ret = Draco::instance()->lockdownPeriods(periods, errormsg);
    if (!ret) {
        lockdownPeriodsAlert(errormsg);
        return false;
    } else {
        for (auto v : periods) {
            QJsonObject it = v.toObject();
            DBG_INFO << it.toVariantMap();
            m_periods.append(it.toVariantMap());
        }
        if (m_periods.size() == 0) {
            errormsg = "There no period";
            lockdownPeriodsAlert(errormsg);
            return false;
        }
    }
    return true;
}

bool QUserWallets::lockdownRequired(const QString &period_id)
{
    m_period_id = period_id;
    DBG_INFO << m_wallet_id << m_period_id;
    QString errormsg;
    bool ret = Draco::instance()->lockdownRequiredSignatures(m_period_id, m_wallet_id, required_question, errormsg);
    if (!ret) {
        //show toast
        AppModel::instance()->showToast(0,
                                        errormsg,
                                        EWARNING::WarningType::ERROR_MSG,
                                        STR_CPP_112);
    }
    return ret;
}

int QUserWallets::lockdownType()
{
    return required_question.type;
}

bool QUserWallets::lockdownByAnswerSecQues()
{
    QString errormsg;
    QString until_time;
    bool ret = Draco::instance()->lockdownByAnswerSecQues(m_passwordToken,
                                                          m_secQuesToken,
                                                          m_period_id,
                                                          m_wallet_id,
                                                          until_time,
                                                          errormsg);
    if (!ret) {
        //show toast
        AppModel::instance()->showToast(0,
                                        errormsg,
                                        EWARNING::WarningType::ERROR_MSG,
                                        STR_CPP_112);
    } else {
        setUntilTime(until_time);
    }
    return ret;
}

bool QUserWallets::lockdownBySignDummyTx()
{
    QStringList signatures = m_signatures.values(); //==> Signatures sau khi sign dummy tx
    QString errormsg;
    QString until_time;
    bool ret = Draco::instance()->lockdownBySignDummyTx(signatures,
                                                        m_passwordToken,
                                                        m_period_id,
                                                        m_wallet_id,
                                                        until_time,
                                                        errormsg);
    if (!ret) {
        //show toast
        AppModel::instance()->showToast(0,
                                        errormsg,
                                        EWARNING::WarningType::ERROR_MSG,
                                        STR_CPP_112);
    } else {
        setUntilTime(until_time);
    }
    return ret;
}

bool QUserWallets::downloadBackup()
{
    if(!m_signer) return false;
    QString base64;
    QString errormsg;
    bool ret = Draco::instance()->userKeysDownloadBackup(m_passwordToken,
                                                         m_signer->fingerPrint(),
                                                         m_quesAnswers,
                                                         base64,
                                                         errormsg);
    if(ret){
        QByteArray ba;
        ba.append(base64);
        m_base64bin = QByteArray::fromBase64(ba);
    }
    else {
        if (errormsg.contains("Incorrect")) {
            emit answerErrorAlert(errormsg);
        } else {
            AppModel::instance()->showToast(0,
                                            errormsg,
                                            EWARNING::WarningType::ERROR_MSG,
                                            STR_CPP_112);
        }
    }
    return ret;
}

nunchuk::Wallet QUserWallets::getWallet() const
{
    return m_wallet;
}

QString QUserWallets::untilTime() const
{
    return m_untilTime;
}

void QUserWallets::setUntilTime(QString untilTime)
{
    if (m_untilTime == untilTime)
        return;

    m_untilTime = untilTime;
    emit untilTimeChanged();
}

void QUserWallets::createTapsigners()
{
    m_tapsigners.clear();
    for (auto signer : AppModel::instance()->masterSignerList()->fullList()) {
        if (signer->signerType() == (int)ENUNCHUCK::SignerType::NFC) {
            QMap<QString,QVariant> maps;
            maps["displayName"] = signer->name();
            maps["fingerPrint"] = signer->fingerPrint();
            m_tapsigners.append(QVariant::fromValue(maps));
        }
    }
    emit tapsignersChanged();
}

QVariantList QUserWallets::tapsigners() const
{
    return m_tapsigners;
}

void QUserWallets::keyRecovery(const QString &fingerPrint)
{
    m_signer = AppModel::instance()->masterSignerList()->getMasterSignerByXfp(fingerPrint);
}

bool QUserWallets::startRecovery(const QString& backup_password)
{
    std::vector<unsigned char> base64vec(m_base64bin.begin(), m_base64bin.end());
    if(!m_signer) return false;
    QString backupkey = backup_password;  //Input from user
    QString keyname = m_signer->name();    //keyname
    bool    isPrimary = m_signer->isPrimaryKey();  //isPrimary key
    QWarningMessage msg;
    QMasterSignerPtr signer = bridge::ImportTapsignerMasterSigner(base64vec, backupkey, keyname, isPrimary, msg);
    if (signer && (int)EWARNING::WarningType::NONE_MSG == msg.type()) {
        // REPLACE KEY IN MASTER KEY
        AppModel::instance()->masterSignerList()->removeMasterSigner(m_signer);
        AppModel::instance()->masterSignerList()->addMasterSigner(signer);
        AppModel::instance()->setMasterSignerInfo(signer);
        AppModel::instance()->setWalletsUsingSigner(AppModel::instance()->walletList()->walletListByMasterSigner(AppModel::instance()->masterSignerInfo()->id()));
        return true;
    } else {
        if (msg.code() == -6625) {
            emit backupPasswordErrorAlert(STR_CPP_113);
        } else {
            emit backupPasswordErrorAlert(msg.what());
        }
        return false;
    }
}

QMasterSigner *QUserWallets::signer() const
{
    return m_signer.data();
}

int QUserWallets::inheritanceCheck(const QString& magic, const QString& environment)
{
    using CIStatus = ServiceSetting::CIStatus;
    QJsonObject result;
    QString errormsg;
    bool ret = Draco::instance()->inheritanceCheck(magic,environment,result,errormsg);
    qInfo() << result;
    if (ret){
        //HANDLE RESULT
        bool is_valid   = result["is_valid"].toBool();
        bool is_paid    = result["is_paid"].toBool();
        bool is_expired = result["is_expired"].toBool();
        if (!is_paid) {
            emit notPaidAlert();
            return (int)CIStatus::CI_IS_NOT_PAID;
        }
        return (int)CIStatus::CI_NONE;
    } else {
        AppModel::instance()->showToast(0,
                                        errormsg,
                                        EWARNING::WarningType::ERROR_MSG,
                                        STR_CPP_112);
        return (int)CIStatus::CI_IS_ERROR;
    }
}

bool QUserWallets::inheritanceGetPlan(const QString &wallet_id)
{
    QStringList ids {};
    if (m_periods.size() == 0) {
        inheritancePlanCreatePeriods();
    }
    for (QVariant var : m_periods) {
        QMap<QString, QVariant> map = var.toMap();
        ids.append(map.value("id").toString());
    }
    QJsonObject response;
    QString errormsg;
    bool ret = Draco::instance()->inheritanceGetPlan(wallet_id, response, errormsg);
    if(ret){
        //HANDLE RESULT
        QJsonObject inheritance = response["inheritance"].toObject();
        qInfo() << "inheritanceGetPlan" << inheritance;
        long int activation_time_milis = static_cast<long int>(inheritance.value("activation_time_milis").toDouble()/1000);
        if (activation_time_milis > 0) {
            ServiceSetting::instance()->setInheritanceActivationDate(QDateTime::fromTime_t(activation_time_milis).date().toString("MM/dd/yyyy"));
        } else {
            ServiceSetting::instance()->setInheritanceActivationDate("");
        }
        ServiceSetting::instance()->setInheritanceNote(inheritance.value("note").toString());
        QJsonArray emails = inheritance.value("notification_emails").toArray();
        QStringList emailList;
        for (QJsonValue js : emails) {
            emailList.append(js.toString());
        }
        QString email = emailList.join(",");
        ServiceSetting::instance()->setInheritanceEmail(email);
        ServiceSetting::instance()->setInheritanceMagic(inheritance.value("magic").toString());
        QJsonObject buffer_period = inheritance["buffer_period"].toObject();
        if (!buffer_period.isEmpty()) {
            QString id = buffer_period.value("id").toString();
            if (ids.contains(id)) {
                ServiceSetting::instance()->setInheritancePeriod(buffer_period.value("display_name").toString());
                ServiceSetting::instance()->setInheritancePeriodId(id);
            }
        } else {
            ServiceSetting::instance()->setInheritancePeriod("");
            ServiceSetting::instance()->setInheritancePeriodId("");
        }
        ServiceSetting::instance()->inheritanceDataChanged();
        ServiceSetting::instance()->setViewInheritanceIsEdit(false);
    }
    else {
        AppModel::instance()->showToast(0,
                                        errormsg,
                                        EWARNING::WarningType::ERROR_MSG,
                                        STR_CPP_112);
    }
    return ret;
}

int QUserWallets::inheritanceDownloadBackup(const QString &magic, const QString &backup_password)
{
    DBG_INFO << magic;
    using CIStatus = ServiceSetting::CIStatus;
    const constexpr auto NONE_MSG = (int)EWARNING::WarningType::NONE_MSG;
    QJsonObject result;
    QString errormsg;
    int response_code = DRACO_CODE::RESPONSE_OK;
    bool ret = Draco::instance()->inheritanceDownloadBackup(magic, response_code, result, errormsg);
    qInfo() << result << response_code;
    if (ret) {
        QString base64 = result["key_backup_base64"].toString();
        QByteArray ba;
        ba.append(base64);
        QByteArray base64bin = QByteArray::fromBase64(ba);
        std::vector<unsigned char> base64vec(base64bin.begin(), base64bin.end());
        QString backupkey = backup_password; //Inputted from user
        QString keyname = result["key_name"].toString();
        mInheritance.derivation_path = result["derivation_path"].toString();
        QWarningMessage msg;
        QMasterSignerPtr masterSigner = bridge::ImportTapsignerMasterSigner(base64vec, backupkey, keyname, false, msg);
        if (masterSigner && NONE_MSG == msg.type()) {
            mInheritance.masterSignerId = masterSigner->id();
            std::vector<nunchuk::SignerTag> tags = {nunchuk::SignerTag::INHERITANCE};
            masterSigner.data()->setSignerTags(tags);

            //Add master signer list
            if(AppModel::instance()->masterSignerList()){
                AppModel::instance()->masterSignerList()->addMasterSigner(masterSigner);
            }
            // Update master signer
            bridge::nunchukUpdateMasterSigner(masterSigner);
            QJsonObject body;
            body["magic"] = magic;

            QJsonObject data;
            data["nonce"] = Draco::instance()->randomNonce();
            data["body"]  = body;
            QJsonDocument doc(data);
            QString user_data(doc.toJson());
            //[SIGN_MESSAGE] flow
            nunchuk::SingleSigner signer = nunchukiface::instance()->GetSignerFromMasterSigner(masterSigner->id().toStdString(), mInheritance.derivation_path.toStdString(), msg);
            if (NONE_MSG == msg.type()) {
                QString messages_to_sign = qUtils::GetHealthCheckMessage(user_data,msg); // user_data in json string
                if (NONE_MSG == msg.type()) {
                    std::string signature = nunchukiface::instance()->SignHealthCheckMessage(signer, messages_to_sign.toStdString(), msg);
                    if (NONE_MSG == msg.type()) {
                        QString authorization = qUtils::CreateRequestToken(QString::fromStdString(signature), QString::fromStdString(signer.get_master_fingerprint()), msg);
                        if (NONE_MSG == msg.type()) {
                            auto status = inheritanceClaimStatus(data, authorization);
                            if (status == (int)CIStatus::CI_NONE) {
                                ServiceSetting::instance()->setClaimInheritanceStatus((int)CIStatus::CI_IS_VALID);
                                return response_code;
                            } else {
                                response_code = -1;
                                ServiceSetting::instance()->setClaimInheritanceStatus(status);
                            }
                        }
                    }
                }
            }
        }
        if (NONE_MSG != msg.type()) {
            response_code = -1;
            AppModel::instance()->showToast(msg.code(),
                                            msg.what(),
                                            (EWARNING::WarningType)msg.type(),
                                            STR_CPP_112);
        }
    } else {
        switch (response_code) {
        case DRACO_CODE::INHERITANCE_801:
            // show popup: Invalid phrase. Please try again
            AppModel::instance()->showToast(0,
                                            STR_CPP_115,
                                            EWARNING::WarningType::ERROR_MSG,
                                            STR_CPP_115);
            break;
        case DRACO_CODE::INHERITANCE_802:
            // show popup: It looks like the original subscription has expired for more than 3 years.
            //             To claim the inheritance, please reactivate the plan first by visiting nunchuk.io/claim.
            emit isExpiredAlert();
            break;
        case DRACO_CODE::INHERITANCE_803:
            // show popup: Your inheritance plan has not been activated yet. Please try again later.
            emit hasNotBeenActivatedYetAlert();
            break;
        default:
            AppModel::instance()->showToast(0,
                                            errormsg,
                                            EWARNING::WarningType::ERROR_MSG,
                                            errormsg);
            break;
        }
    }
    return response_code;
}

bool QUserWallets::inheritanceClaimRequest(const nunchuk::Wallet wallet, const nunchuk::Transaction txSigned, const QString& magic)
{
    QJsonObject result;
    QString errormsg;
    bool ret = Draco::instance()->inheritanceClaimRequest(magic,QString::fromStdString(txSigned.get_psbt()),result, errormsg);
    if(ret){
        //HANDLE RESULT
        QJsonObject transaction = result["transaction"].toObject();
        QString status = transaction.value("status").toString();
        QString psbt = transaction.value("psbt").toString();
        if (status == "PENDING_CONFIRMATION" ||
                status == "CONFIRMED"){
            QWarningMessage _msg;
            bridge::nunchukImportPsbt(QString::fromStdString(mInheritance.wallet.get_id()), psbt, _msg);
            QString id = transaction.value("id").toString();
            QString hex = transaction.value("hex").toString();
            QString reject_msg = transaction.value("reject_msg").toString();
            bridge::nunchukUpdateTransaction(QString::fromStdString(wallet.get_id()), QString::fromStdString(txSigned.get_txid()), id, hex, reject_msg, _msg);
        }
        mInheritance.balance = 0;//BTC
        emit ServiceSetting::instance()->inheritanceChanged();
    }
    else {
        AppModel::instance()->showToast(0,
                                        errormsg,
                                        EWARNING::WarningType::ERROR_MSG,
                                        STR_CPP_118);
    }
    return ret;
}

int QUserWallets::inheritanceClaimStatus(const QJsonObject& data, const QString& autho)
{
    using CIStatus = ServiceSetting::CIStatus;
    QJsonObject result;
    QString errormsg;
    bool ret = Draco::instance()->inheritanceClaimStatus(data, autho, result, errormsg);
    qInfo() << result;
    if(ret){
        //HANDLE RESULT
        QJsonObject inheritance = result["inheritance"].toObject();
#if 0
        if (inheritance.isEmpty()) {
            Draco::instance()->inheritanceFakeUpdate();
        }
        inheritance["wallet_id"] = "abcdef";
        inheritance["magic"] = "fatigue good appear";
        inheritance["note"] = "fatigue good appear";
        result["balance"] = QJsonValue(9464300000000.0);
        result["buffer_period_countdown"] = "";
#endif
        if (!inheritance.isEmpty()) {
            mInheritance.magic = inheritance["magic"].toString();
            mInheritance.note = inheritance["note"].toString();
            mInheritance.balance = result["balance"].toDouble();//BTC
            emit ServiceSetting::instance()->inheritanceChanged();
        }
        QJsonObject buffer_period_countdown = result["buffer_period_countdown"].toObject();
        if (!buffer_period_countdown.isEmpty()) {
            QString period = buffer_period_countdown["remaining_display_name"].toString();
            ServiceSetting::instance()->setClaimInheritancePeriod(period);
            return (int)CIStatus::CI_IS_PAID;
        }
        return (int)CIStatus::CI_NONE;
    } else {
        DBG_INFO << errormsg;
        AppModel::instance()->showToast(0,
                                        errormsg,
                                        EWARNING::WarningType::ERROR_MSG,
                                        STR_CPP_112);
        return (int)CIStatus::CI_IS_ERROR;
    }
}

bool QUserWallets::inheritanceCreateTx(const nunchuk::SingleSigner& signer, const QJsonObject& data, const QString& autho)
{
    QJsonObject result;
    QString errormsg;
    QWarningMessage msg;
    bool ret = Draco::instance()->inheritanceCreateTx(data, autho, result, errormsg);
    if(ret){
        QJsonObject transaction = result["transaction"].toObject();
        QString psbt = transaction["psbt"].toString();
        QString sub_amount = QString("%1").arg(result["sub_amount"].toDouble());
        QString fee = QString("%1").arg(result["fee"].toDouble());
        QString fee_rate = QString("%1").arg(result["fee_rate"].toDouble());
        qint64 tx_fee = qUtils::QAmountFromValue(fee); // fee in BTC
        qint64 tx_fee_rate = qUtils::QAmountFromValue(fee_rate); // fee_rate in BTC
        qint64 tx_sub_amount = qUtils::QAmountFromValue(sub_amount); // sub amount in BTC
        //HANDLE RESULT
        // decode transaction
        mInheritance.wallet = nunchuk::Wallet(false);
        mInheritance.wallet.set_signers({signer});

        mInheritance.tx = qUtils::DecodeTx(mInheritance.wallet, psbt, tx_sub_amount, tx_fee, tx_fee_rate, msg);
        QTransactionPtr trans = bridge::convertTransaction(mInheritance.tx, QString::fromStdString(mInheritance.wallet.get_id()));
        AppModel::instance()->setTransactionInfo(trans);
        QList<uint> states = QQuickViewer::instance()->getCurrentStates();
        if(!states.isEmpty() && states.last() == E::STATE_ID_SCR_INHERITANCE_WITHDRAW_BALANCE){
            DBG_INFO << "Entry here ";
            QQuickViewer::instance()->sendEvent(E::EVT_INHERITANCE_CONFIRM_TRANSACTION_REQUEST);
        }
    }
    else {
        AppModel::instance()->showToast(0,
                                        errormsg,
                                        EWARNING::WarningType::ERROR_MSG,
                                        STR_CPP_112);
    }
    return ret;
}

void QUserWallets::setInheritanceAddress(const QString& to_wallet_id)
{
    QWalletPtr ptr = AppModel::instance()->walletList()->getWalletById(to_wallet_id);
    if (!ptr) return;
    AppModel::instance()->setWalletInfo(ptr);
    if(AppModel::instance()->walletInfo()){
        QString wallet_id = AppModel::instance()->walletInfo()->id();
        QStringList addrs = bridge::nunchukGetUnusedAddresses(wallet_id, false);
        if (addrs.size() > 0) {
            mInheritance.m_destinationAddress = addrs.first();
        } else {
            mInheritance.m_destinationAddress = bridge::nunchukGenNewAddresses(wallet_id,false);
        }
        DBG_INFO << mInheritance.m_destinationAddress;
    }
}

void QUserWallets::setInheritanceAddressNewTransaction(const QString &address)
{
    mInheritance.m_destinationAddress = address;
}

void QUserWallets::inheritanceCreateDraftTransaction(double fee_rate)
{
    using CIStatus = ServiceSetting::CIStatus;
    const constexpr auto NONE_MSG = (int)EWARNING::WarningType::NONE_MSG;
    QJsonObject body;
    body["magic"] = mInheritance.magic;
    body["address"] = mInheritance.m_destinationAddress;
    body["fee_rate"] = qUtils::QValueFromAmount(fee_rate);

    QJsonObject data;
    data["nonce"] = Draco::instance()->randomNonce();
    data["body"]  = body;
    QJsonDocument doc(data);
    QString user_data(doc.toJson());
    QWarningMessage msg;
    //[SIGN_MESSAGE] flow
    nunchuk::SingleSigner signer = nunchukiface::instance()->GetSignerFromMasterSigner(mInheritance.masterSignerId.toStdString(), mInheritance.derivation_path.toStdString(), msg);
    if (NONE_MSG == msg.type()) {
        QString messages_to_sign = qUtils::GetHealthCheckMessage(user_data,msg); // user_data in json string
        if (NONE_MSG == msg.type()) {
            std::string signature = nunchukiface::instance()->SignHealthCheckMessage(signer, messages_to_sign.toStdString(), msg);
            if (NONE_MSG == msg.type()) {
                QString authorization = qUtils::CreateRequestToken(QString::fromStdString(signature), QString::fromStdString(signer.get_master_fingerprint()), msg);
                if (NONE_MSG == msg.type()) {
                    inheritanceCreateTx(signer, data, authorization);
                    return;
                }
            }
        }
    }
    if (NONE_MSG != msg.type()) {
        AppModel::instance()->showToast(msg.code(),
                                        msg.what(),
                                        (EWARNING::WarningType)msg.type(),
                                        STR_CPP_112);
    }
}

void QUserWallets::inheritanceSignTransaction()
{
    using CIStatus = ServiceSetting::CIStatus;
    const constexpr auto NONE_MSG = (int)EWARNING::WarningType::NONE_MSG;
    auto signer = mInheritance.wallet.get_signers().front();
    QWarningMessage msg;
    nunchuk::Transaction signed_tx = nunchukiface::instance()->SignTransaction(mInheritance.wallet, mInheritance.tx, nunchuk::Device(signer.get_master_signer_id()),msg);
    DBG_INFO << msg.type();
    if (NONE_MSG == msg.type()) {
        QTransactionPtr trans = bridge::convertTransaction(signed_tx, QString::fromStdString(signer.get_master_signer_id()));
        AppModel::instance()->setTransactionInfo(trans);
        inheritanceClaimRequest(mInheritance.wallet, signed_tx, mInheritance.magic);
    } else {
        AppModel::instance()->showToast(msg.code(),
                                        msg.what(),
                                        (EWARNING::WarningType)msg.type(),
                                        STR_CPP_059);
    }
}

QJsonObject QUserWallets::inheritancePlanBody()
{
    QJsonObject body;
    body["wallet"] = ServiceSetting::instance()->getInheritanceWalletId();
    body["note"] = ServiceSetting::instance()->getInheritanceNote();
    body["buffer_period_id"] = ServiceSetting::instance()->getInheritancePeriodId();
    QString email = ServiceSetting::instance()->getInheritanceEmail();
    QStringList emails = email.split(",");
    QJsonArray arrays;
    for (QString e : emails) {
        if (e != "None") {
            arrays.append(e);
        }
    }
    body["notification_emails"] = arrays;
    body["notify_today"] = ServiceSetting::instance()->getInheritanceIsNotify();
    long int t = qUtils::GetTimeSecond(ServiceSetting::instance()->getInheritanceActivationDate());
    body["activation_time_milis"] = (double)t*1000;
    return body;
}

void QUserWallets::inheritancePlanFinalizeChanges()
{
    long int activeTime = qUtils::GetTimeSecond(ServiceSetting::instance()->getInheritanceActivationDate());
    long int currentTime = qUtils::GetCurrentTimeSecond();
    if (activeTime < currentTime) {
        AppModel::instance()->showToast(0,
                                        STR_CPP_120,
                                        EWARNING::WarningType::ERROR_MSG,
                                        STR_CPP_120);
        return;
    }
    QJsonObject body_data = inheritancePlanBody();
    QString errormsg = "";
    bool ret = Draco::instance()->inheritancePlanRequiredSignatures(body_data, required_question, errormsg);
    DBG_INFO << errormsg << body_data;
    if (ret) {
        if (required_question.type == (int)REQUIRED_SIGNATURE_TYPE_INT::SECURITY_QUESTION) {
            QUserWallets::instance()->createSecurityQuestions();
        } else if (required_question.type == (int)REQUIRED_SIGNATURE_TYPE_INT::SIGN_DUMMY_TX) {
            //Show popup support in moble
            emit inheritanceDummyTransactionAlert();
        } else {
        }
    } else {
        emit inheritanceInvalidActivationDateAlert();
    }
    return;
}

void QUserWallets::inheritancePlanUpdate()
{
    QJsonObject data;
    data["nonce"] = Draco::instance()->randomNonce();
    data["body"]  = inheritancePlanBody();
    QStringList authorizations;
    QJsonObject output;
    QString errormsg = "";
    DBG_INFO << m_passwordToken << m_secQuesToken << data;
    bool ret = Draco::instance()->inheritancePlanUpdate(m_passwordToken, m_secQuesToken, authorizations, data, output, errormsg);
    DBG_INFO << errormsg << output;
    if (ret) {
        emit securityQuestionClosed();
        AppModel::instance()->showToast(0,
                                        errormsg,
                                        EWARNING::WarningType::SUCCESS_MSG,
                                        STR_CPP_119);
    } else {
        AppModel::instance()->showToast(0,
                                        errormsg,
                                        EWARNING::WarningType::ERROR_MSG,
                                        STR_CPP_112);
    }
}

bool QUserWallets::inheritancePlanCreatePeriods()
{
    m_periods.clear();
    QJsonArray periods;
    QString errormsg;
    bool ret = Draco::instance()->inheritancePlanBufferPeriod(periods, errormsg);
    if (!ret) {
        return false;
    } else {
        for (auto v : periods) {
            QJsonObject it = v.toObject();
            DBG_INFO << it.toVariantMap();
            m_periods.append(it.toVariantMap());
        }
        QJsonObject notNeed;
        notNeed["id"] = "";
        notNeed["display_name"] = "I don’t need a buffer period";
        notNeed["is_recommended"] = false;
        m_periods.append(notNeed.toVariantMap());
        if (m_periods.size() == 0) {
            errormsg = "There no period";
            return false;
        }
    }
    return true;
}

bool QUserWallets::serverKeyGetCurrentPolicies(const QString& wallet_id)
{
    QWarningMessage msgWallet;
    nunchuk::Wallet wallet = nunchukiface::instance()->GetWallet(wallet_id.toStdString(), msgWallet);; // Get current Assisted wallet = <get assisted wallet>
    nunchuk::SingleSigner server_key;// Get server key from wallet type = <SignerType::SERVER>
    for (auto key : wallet.get_signers()) {
        if (key.get_type() == nunchuk::SignerType::SERVER) {
            server_key = key;
        }
    }
    mCoSigning.m_server_key = server_key;
    mCoSigning.key_id_or_xfp = QString::fromStdString(server_key.get_master_fingerprint());
    mCoSigning.wallet_id = wallet_id;
    QJsonObject output;
    QString errormsg = "";
    bool ret = Draco::instance()->serverKeysGet(mCoSigning.key_id_or_xfp, output, errormsg);
    if(ret){
        qInfo() << output;
        QJsonObject policies = output["policies"].toObject();
        bool auto_broadcast_transaction = policies["auto_broadcast_transaction"].toBool();
        qint64 signing_delay_seconds = policies["signing_delay_seconds"].toInt();
        QJsonObject spendlimit = policies["spending_limit"].toObject();
        AppSetting::instance()->setEnableCoSigning(signing_delay_seconds > 0);
        qint64 hour = signing_delay_seconds/(60*60);
        qint64 minute = (signing_delay_seconds - hour * (60*60))/60;
        QMap<QString,QVariant> maps;
        maps["hours"] = hour;
        maps["minutes"] = minute;
        maps["auto_broadcast_transaction"] = auto_broadcast_transaction;
        if(spendlimit.isEmpty()){
            // Iron hand
        } else {
            //Honey badger
            QLocale locale(QLocale::English);
            maps["interval"] = spendlimit["interval"].toString(); //[ DAILY, WEEKLY, MONTHLY, YEARLY ]
            maps["limit"] = locale.toString(spendlimit["limit"].toDouble(), 'f', qUtils::Precision(spendlimit["limit"].toDouble())); //unit is interval
            maps["currency"] = spendlimit["currency"].toString();
        }
        ServiceSetting::instance()->setKeyCoSigning(QVariant::fromValue(maps));
    } else {
        AppModel::instance()->showToast(0,
                                        errormsg,
                                        EWARNING::WarningType::ERROR_MSG,
                                        STR_CPP_112);
    }
    return ret;
}

QJsonObject QUserWallets::serverKeyBody()
{
    QMap<QString,QVariant> maps = ServiceSetting::instance()->keyCoSigning().toMap();
    QJsonObject spending_limit;
    QMap<QString,QVariant> plans = CLIENT_INSTANCE->user().toMap();
    if (plans["plan_slug"] == "honey_badger") {
        spending_limit["interval"] = maps["interval"].toString();
        spending_limit["limit"] = maps["limit"].toDouble();
        spending_limit["currency"] = maps["currency"].toString();
    }

    qint64 signing_delay_seconds = 0;
    if (AppSetting::instance()->enableCoSigning()) {
        signing_delay_seconds = maps["hours"].toUInt()*(60*60) + maps["minutes"].toUInt()*60;
    }
    QJsonObject policies;
    policies["auto_broadcast_transaction"] = maps["auto_broadcast_transaction"].toBool();
    policies["signing_delay_seconds"] = signing_delay_seconds;
    policies["spending_limit"] = spending_limit;

    QJsonObject data;
    data["name"] = "name";
    data["wallet"] = mCoSigning.wallet_id;
    data["policies"] = policies;
    return data;
}

bool QUserWallets::serverKeyUpdatePolicies()
{
    QJsonObject data = serverKeyBody();
    QString errormsg = "";
    qInfo() << data;
    bool ret = Draco::instance()->serverKeysRequiredSignatures(mCoSigning.key_id_or_xfp, data, required_question, errormsg);
    if (ret) {

        if (required_question.type == (int)REQUIRED_SIGNATURE_TYPE_INT::SECURITY_QUESTION) {
            QUserWallets::instance()->createSecurityQuestions();
            //HANDLE
            // Get security question
            // Answer security question
            // etc..
            return true;
        } else if (required_question.type == (int)REQUIRED_SIGNATURE_TYPE_INT::SIGN_DUMMY_TX) {
            //Show popup support in moble
            emit serverKeyDummyTransactionAlert();
        } else {
            QUserWallets::instance()->serverKeyUpdatePoliciesSucceed();
            return false;
        }
    } else {
    }
    return ret;
}

bool QUserWallets::serverKeyUpdatePoliciesSucceed()
{
    QJsonObject data;
    data["nonce"] = Draco::instance()->randomNonce();
    data["body"] = serverKeyBody();
    QStringList authorizations;
    QJsonObject output;
    QString errormsg = "";
    bool ret = Draco::instance()->serverKeysUpdate(m_passwordToken,m_secQuesToken,mCoSigning.key_id_or_xfp, authorizations, data, output, errormsg);
    DBG_INFO << errormsg;
    if (ret) {
        qInfo() << output;
        emit securityQuestionClosed();
        AppModel::instance()->showToast(0,
                                        errormsg,
                                        EWARNING::WarningType::SUCCESS_MSG,
                                        STR_CPP_117);
    } else {
        AppModel::instance()->showToast(0,
                                        errormsg,
                                        EWARNING::WarningType::ERROR_MSG,
                                        STR_CPP_112);
    }
    return ret;
}

QUserWallets::inheritance_t QUserWallets::inheritance() const
{
    return mInheritance;
}

void QUserWallets::newRequestToAddKey()
{
    QtConcurrent::run([this]() {
        QJsonObject data = Draco::instance()->assistedWalletGetListKey();
        m_requests = data.value("requests").toArray();
        setLedgerNeed(exist("LEDGER"));
        setTrezorNeed(exist("TREZOR"));
        setColdCardNeed(exist("COLDCARD"));
    });
}

void QUserWallets::addKeyRequested()
{
    QMasterSignerPtr signer = AppModel::instance()->masterSignerInfoPtr();
    QString request_id = "";
    QString key = "";
    int key_index = -1;
    QJsonObject data;
    DBG_INFO << !signer.isNull();
    QWarningMessage warningmsg;
    if (!signer.isNull()) {
        QJsonArray tags;
        if (mLedgerNeed && signer->deviceType() == "ledger") {
            key = "LEDGER";
        }
        if (mTrezorNeed && signer->deviceType() == "trezor") {
            key = "TREZOR";
        }
        if (mColdCardNeed && signer->deviceType() == "coldcard") {
            key = "COLDCARD";
        }
        DBG_INFO << signer->deviceType() << key;
        if (!key.isEmpty()) {
            tags.append(key);
            request_id = getRequestId(key);
            key_index = getKeyIndex(key);
        }
        QSingleSignerPtr ptr = bridge::nunchukGetDefaultSignerFromMasterSigner(signer->id(),
                                                                               ENUNCHUCK::WalletType::MULTI_SIG,
                                                                               ENUNCHUCK::AddressType::NATIVE_SEGWIT ,
                                                                               warningmsg);
        if (!request_id.isEmpty() && !ptr.isNull()) {
            data["name"] = signer->name();
            data["xfp"] = signer->fingerPrint();
            data["derivation_path"] = ptr->derivationPath();
            data["xpub"] = ptr->xpub();
            data["pubkey"] = ptr->publickey();
            data["type"] = qUtils::GetSignerTypeString(ptr->singleSigner().get_type());
            data["tags"] = tags;
            data["tapsigner"] = {};
            if (key_index >= 0) {
                data["key_index"] = key_index;
            } else {
                data["key_index"] = {};
            }
            bool ret = Draco::instance()->assistedWalletAddKey(request_id, data);
            qInfo() << request_id << data << ret;
            if (ret) {
                AppModel::instance()->setAddSignerWizard(3);
                emit addHardwareSuccessAlert();
                return;
            }
        } else {
            QString errormsg = "request id empty or get default error";
            AppModel::instance()->showToast(0,
                                            errormsg,
                                            EWARNING::WarningType::ERROR_MSG,
                                            errormsg);
        }
    }
    AppModel::instance()->setAddSignerWizard(0);
    emit addHardwareAlert();
}

QJsonObject QUserWallets::getRequest(const QString &key) const
{
    if (m_requests.size() > 0) {
        for (QJsonValue request : m_requests) {
            QJsonObject requestObj = request.toObject();
            QString status = requestObj.value("status").toString();
            QJsonArray tags = requestObj.value("tags").toArray();
            if (tags.contains(key) && status == "PENDING") {
                return requestObj;
            }
        }
    }
    return {};
}

QString QUserWallets::getRequestId(const QString &key) const
{
    QJsonObject request = getRequest(key);
    if (request.empty()) {
        return {};
    } else {
        return request.value("id").toString();
    }
}

int QUserWallets::getKeyIndex(const QString &key) const
{
    QJsonObject request = getRequest(key);
    if (request.empty()) {
        return -1;
    } else {
        return request.value("key_index").toInt(-1);
    }
}

bool QUserWallets::exist(const QString &key) const
{
    return !getRequest(key).empty();
}

void QUserWallets::cancelRequest(const QString& key)
{
    QString request_id = getRequestId(key);
    if (!request_id.isEmpty()) {
        DBG_INFO << key;
        Draco::instance()->assistedWalletRemoveId(request_id);
        newRequestToAddKey();
    }
}

void QUserWallets::additionalGetWalletConfig()
{
    QJsonObject config = Draco::instance()->assistedGetWalletConfig();
    DBG_INFO << config;
    int remaining_wallet_count = config.value("remaining_wallet_count").toInt();
    ServiceSetting::instance()->setRemainingAssistedWalletCount(remaining_wallet_count);
}
