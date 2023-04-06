#include <QQmlEngine>
#include <QJsonDocument>
#include "bridgeifaces.h"
#include "AppModel.h"
#include "QUserWallets.h"
#include "localization/STR_CPP.h"

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
                                        errormsg);
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
        return m_quesAnswers.size() == correct_answer && correct_answer != 0;
    }
}

QVariantList QUserWallets::lockdownPeriods()
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
                                        errormsg);
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
                                        errormsg);
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
                                        errormsg);
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
        DBG_INFO << base64;
        QByteArray ba;
        ba.append(base64);
        m_base64bin = QByteArray::fromBase64(ba);
    } else {
        if (errormsg.contains("Incorrect")) {
            emit answerErrorAlert(errormsg);
        } else {
            AppModel::instance()->showToast(0,
                                            errormsg,
                                            EWARNING::WarningType::ERROR_MSG,
                                            errormsg);
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

