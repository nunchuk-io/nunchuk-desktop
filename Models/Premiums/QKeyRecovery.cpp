#include "QKeyRecovery.h"
#include "ViewsEnums.h"
#include "Servers/Draco.h"
#include "Servers/Byzantine.h"
#include "WalletModel.h"
#include "Premiums/QGroupWallets.h"
#include "ServiceSetting.h"
#include "QWalletServicesTag.h"
#include "Premiums/QGroupWalletDummyTx.h"

QKeyRecovery::QKeyRecovery()
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

QVariant QKeyRecovery::signer() const
{
    return QVariant::fromValue(m_signer);
}

void QKeyRecovery::setSigner(const QString& xfp)
{
    m_signer = {};
    for (auto js : m_tapsigners) {
        QJsonObject json = js.toObject();
        QString fingerPrint = json["key_xfp"].toString();
        if (qUtils::strCompare(fingerPrint, xfp)) {
            m_signer = json;
        }
    }
}

QVariantList QKeyRecovery::tapsigners() const
{
    return m_tapsigners.toVariantList();
}

void QKeyRecovery::setTapsigners(const QJsonArray &tapsigners)
{
    if (tapsigners == m_tapsigners)
        return;

    m_tapsigners = tapsigners;
    emit tapsignersChanged();
}

QVariantList QKeyRecovery::requireQuestions() const
{
    return m_requireQuestions.toVariantList();
}

void QKeyRecovery::setRequireQuestions(const QJsonArray &requireQuestions)
{
    if (m_requireQuestions == requireQuestions)
        return;

    m_requireQuestions = requireQuestions;
    emit requireQuestionsChanged();
}

QVariant QKeyRecovery::firstQuestion() const
{
    QJsonObject json;
    json["isChanged"] = false;
    if (m_requireQuestions.size() == 0) {
        return QVariant::fromValue(json);
    }
    for (auto js : m_requireQuestions) {
        QJsonObject json = js.toObject();
        bool isChanged = json["isChanged"].toBool();
        if (isChanged) {
            return json;
        }
    }
    return QVariant::fromValue(json);
}

bool QKeyRecovery::CreateTapsigners()
{
    QJsonArray tapsigners;
    QJsonArray array;
    QString errormsg;
    bool ret = Draco::instance()->UserKeysGetListUserKey(servicesTagPtr()->passwordToken(), array, errormsg);
    if (ret) {
        for (auto js : array) {
            QJsonObject key = js.toObject();
            QString key_xfp = key["key_xfp"].toString();
            if(AppModel::instance()->masterSignerList()){
                QMasterSignerPtr master_key = AppModel::instance()->masterSignerList()->getMasterSignerByXfp(key_xfp);
                if (master_key && master_key->signerType() == (int)ENUNCHUCK::SignerType::NFC) {
                    DBG_INFO << key;
                    QString card_id = key["card_id"].toString();
                    QStringRef textR = card_id.rightRef(5);
                    key["key_name"] = master_key->name();
                    key["displayName"] = QString("%1 (••%2)").arg(key["key_name"].toString()).arg(textR);
                    tapsigners.append(key);
                }
            }
        }
        setTapsigners(tapsigners);
    }
    return ret;
}

bool QKeyRecovery::UserKeysCalculateRequiredSignatures(const QString &xfp)
{
    setSigner(xfp);
    if (m_signer.isEmpty()) return false;
    DBG_INFO << m_signer;
    if (ClientController::instance()->isUserWallet()) {
        QJsonObject resultObj;
        resultObj["type"] = "SECURITY_QUESTION";
        servicesTagPtr()->setReqiredSignatures(resultObj);
        if (servicesTagPtr()->CreateSecurityQuestionsAnswered()) {
            QEventProcessor::instance()->sendEvent(E::EVT_ANSER_SECURITY_QUESTION_REQUEST);
        }
        return true;
    }
    else {
        QJsonObject output;
        QString errormsg;
        bool ret = Draco::instance()->UserKeysCalculateRequiredSignatures(xfp, output, errormsg);
        if (ret) {
            DBG_INFO << output;
            QJsonObject resultObj = output["result"].toObject();
            QString step = output["step"].toString();
            resultObj["step"] = step;
            if (step == "PENDING_APPROVAL") {
                emit keyRecoveryPendingApproval();
                return false;
            }
            servicesTagPtr()->setReqiredSignatures(resultObj);
            ReqiredSignaturesInfo required_question = servicesTagPtr()->reqiredSignaturesInfo();
            if (required_question.type == (int)REQUIRED_SIGNATURE_TYPE_INT::SECURITY_QUESTION) {
                if (servicesTagPtr()->CreateSecurityQuestionsAnswered()) {
                    QEventProcessor::instance()->sendEvent(E::EVT_ANSER_SECURITY_QUESTION_REQUEST);
                }
                return true;
            } else if (required_question.type == (int)REQUIRED_SIGNATURE_TYPE_INT::CONFIRMATION_CODE) {
                if (RequestConfirmationCodeKeyRecovery()) {
                    QEventProcessor::instance()->sendEvent(E::EVT_ANSER_SECURITY_QUESTION_REQUEST);
                }
                return true;
            } else if (required_question.type == (int)REQUIRED_SIGNATURE_TYPE_INT::SIGN_DUMMY_TX) {
                return false;
            } else if (required_question.type == (int)REQUIRED_SIGNATURE_TYPE_INT::NONE) {
                if (step == "RECOVER") {
                    if (UserKeysRecoveryKey()) {
                        QEventProcessor::instance()->sendEvent(E::EVT_ANSER_SECURITY_QUESTION_REQUEST);
                        return true;
                    }
                }
                return false;
            } else {
                return false;
            }
        }
        return ret;
    }
    return false;
}

bool QKeyRecovery::RequestConfirmationCodeKeyRecovery()
{
    QString errormsg;
    QJsonObject output;
    QJsonObject data;
    data["nonce"] = Draco::instance()->randomNonce();
    data["body"] = {};
    servicesTagPtr()->setConfirmCodeNonceBody(data);
    bool ret = Draco::instance()->RequestConfirmationCode("DOWNLOAD_KEY_BACKUP", data, output, errormsg);
    if (ret) {
        DBG_INFO << output;
        QString code_id = output["code_id"].toString();
        servicesTagPtr()->setCode_id(code_id);
    }
    return ret;
}

bool QKeyRecovery::startRecovery(const QString &backup_password)
{
    if (m_signer.isEmpty()) return false;
    std::vector<unsigned char> base64vec(m_base64bin.begin(), m_base64bin.end());
    QString fingerPrint = m_signer["key_xfp"].toString();
    QString backupkey = backup_password;  //Input from user
    QString keyname = m_signer["key_name"].toString();    //keyname
    bool    isPrimary = AppModel::instance()->masterSignerListPtr()->isPrimaryKey(fingerPrint);  //isPrimary key
    DBG_INFO << fingerPrint << keyname << isPrimary << backupkey << m_base64bin;
    QWarningMessage msg;
    QMasterSignerPtr signer = bridge::ImportTapsignerMasterSigner(base64vec, backupkey, keyname, isPrimary, msg);
    if (signer && (int)EWARNING::WarningType::NONE_MSG == msg.type()) {
        // REPLACE KEY IN MASTER KEY
        AppModel::instance()->masterSignerList()->removeMasterSigner(fingerPrint);
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

bool QKeyRecovery::UserKeysMarkRecoverStatus()
{
    QString fingerPrint = m_signer["key_xfp"].toString();
    QString errormsg;
    QJsonObject output;
    bool ret = Draco::instance()->UserKeysMarkRecoverStatus(fingerPrint, output, errormsg);
    if (ret) {

    }
    return ret;
}

bool QKeyRecovery::UserKeysRequestRecoveryKey()
{
    QString fingerPrint = m_signer["key_xfp"].toString();
    QString errormsg;
    QJsonObject output;
    bool ret = Draco::instance()->UserKeysRequestRecoveryKey(fingerPrint, {},
                                                             servicesTagPtr()->passwordToken(),
                                                             servicesTagPtr()->secQuesToken(),
                                                             servicesTagPtr()->confirmToken(),
                                                             servicesTagPtr()->confirmCodeNonceBody(),
                                                             output,
                                                             errormsg);
    DBG_INFO << ret << output;
    if (ret) {
        QEventProcessor::instance()->sendEvent(E::EVT_ENTER_BACKUP_PASSWORD_RERQUEST);
    }
    return ret;
}

bool QKeyRecovery::UserKeysRecoveryKey(const QString &xfp)
{
    if (m_tapsigners.size() == 0) {
        CreateTapsigners();
    }
    setSigner(xfp);
    return UserKeysRecoveryKey();
}

bool QKeyRecovery::UserKeysRecoveryKey()
{
    QString fingerPrint = m_signer["key_xfp"].toString();
    QString errormsg;
    QJsonObject output;
    bool ret = Draco::instance()->UserKeysRecoveryKey(fingerPrint, {},
                                                             servicesTagPtr()->passwordToken(),
                                                             servicesTagPtr()->secQuesToken(),
                                                             output,
                                                             errormsg);
    if (ret) {
        DBG_INFO << output;
        QJsonObject key = output["key"].toObject();
        QString base64 = key["key_backup_base64"].toString();
        QByteArray ba;
        ba.append(base64);
        m_base64bin = QByteArray::fromBase64(ba);
    }
    return ret;
}

bool QKeyRecovery::KeyRecoveryDownloadBackup()
{
    if (m_signer.isEmpty()) return false;
    QString base64;
    QString errormsg;
    QString fingerPrint = m_signer["key_xfp"].toString();
    bool ret = Draco::instance()->UserKeysDownloadBackup(servicesTagPtr()->passwordToken(), fingerPrint, servicesTagPtr()->questionsAndAnswers(), base64, errormsg);
    DBG_INFO << ret << servicesTagPtr()->passwordToken() << fingerPrint << base64;
    if(ret){
        QByteArray ba;
        ba.append(base64);
        m_base64bin = QByteArray::fromBase64(ba);
    }
    else {
        if (errormsg.contains("Incorrect")) {
            emit servicesTagPtr()->answerErrorAlert(errormsg);
        }
    }
    return ret;
}

QJsonObject QKeyRecovery::JsBody() const
{
    QJsonObject body;
    body["questions_and_answers"] = questionsAndChangeAnswers();
    if (auto w = ServiceSetting::instance()->walletInfoPtr())
    {
        body["wallet"] = w->walletId();
    }
    return body;
}

bool QKeyRecovery::CreateAllSecurityQuestions()
{
    QJsonObject output;
    QString errormsg;
    bool ret = Draco::instance()->secQuesGet(output, errormsg);
    if (ret) {
        DBG_INFO << output;
        QJsonArray all_question = output["questions"].toArray();
        QJsonObject my_question;
        my_question["id"] = "my_question";
        my_question["question"] = "Create my own question";
        all_question.append(my_question);
        m_allQuestion = all_question;
        AutomaticGenerateSecurityQuestions();
    }
    return ret;
}

bool QKeyRecovery::AutomaticGenerateSecurityQuestions()
{
    if (m_requireQuestions.size() == 0)
    {
        const int required_max = qMin(3, m_allQuestion.size());
        QJsonArray require_questions;
        int index = 0;
        for (int i = 0; i < m_allQuestion.size(); i++) {
            QJsonObject json = m_allQuestion.at(i).toObject();
            bool is_answered = json["is_answered"].toBool();
            if (is_answered && index < required_max) {
                require_questions.append(json);
                index++;
            }
        }
        setRequireQuestions(require_questions);
        UpdateRequiredQuestion();
    }
    else {
        UpdateRequiredQuestion();
    }
    return true;
}

bool QKeyRecovery::UpdateSecurityQuestions()
{
    QJsonObject output;
    QString errormsg;
    bool ret = Draco::instance()->SecQuesUpdate(servicesTagPtr()->confirmCodeNonceBody(),
                                                {},
                                                servicesTagPtr()->passwordToken(),
                                                servicesTagPtr()->secQuesToken(),
                                                servicesTagPtr()->confirmToken(),
                                                false,
                                                output,
                                                errormsg);
    if (ret) {
        AutomaticGenerateSecurityQuestions();
    }
    return ret;
}

bool QKeyRecovery::UpdateSecurityQuestionsRequiredSignatures()
{
    QJsonObject output;
    QString errormsg;
    bool ret = Draco::instance()->UpdateSecQuesRequiredSignatures(JsBody(),
                                                output,
                                                errormsg);
    if (ret) {
        QJsonObject resultObj = output["result"].toObject();
        DBG_INFO << resultObj;
        servicesTagPtr()->setReqiredSignatures(resultObj);
        ReqiredSignaturesInfo required_question = servicesTagPtr()->reqiredSignaturesInfo();
        if (required_question.type == (int)REQUIRED_SIGNATURE_TYPE_INT::SECURITY_QUESTION) {
//            if (servicesTagPtr()->CreateSecurityQuestions()) {
//                QEventProcessor::instance()->sendEvent(E::EVT_ANSER_SECURITY_QUESTION_REQUEST);
//            }
            return false;
        } else if (required_question.type == (int)REQUIRED_SIGNATURE_TYPE_INT::CONFIRMATION_CODE) {
            if (RequestConfirmationCodeSetupSecurityQuestions()) {
                QEventProcessor::instance()->sendEvent(E::EVT_SETUP_ANSWER_SECURITY_QUESTION_REQ);
            }
            return true;
        } else if (required_question.type == (int)REQUIRED_SIGNATURE_TYPE_INT::SIGN_DUMMY_TX) {
            QJsonObject output;
            QString errormsg = "";
            QJsonObject data;
            data["nonce"] = Draco::instance()->randomNonce();
            data["body"]  = JsBody();
            QStringList authorizations;
            bool ret = Draco::instance()->SecQuesUpdate(data,
                                                        authorizations,
                                                        servicesTagPtr()->passwordToken(),
                                                        "",
                                                        "",
                                                        true,
                                                        output,
                                                        errormsg);
            if(ret){
                QJsonObject dummy_transaction = output["dummy_transaction"].toObject();
                if (auto w = ServiceSetting::instance()->walletInfoPtr()) {
                    if (auto dummy = w->groupDummyTxPtr()) {
                        DBG_INFO << dummy_transaction;
                        dummy->setDummyTxData(dummy_transaction);
                        QEventProcessor::instance()->sendEvent(E::EVT_HEALTH_CHECK_STARTING_REQUEST);
                    }
                }
            }
            return true;
        } else {
            QJsonObject output;
            QString errormsg = "";
            QJsonObject data;
            data["nonce"] = Draco::instance()->randomNonce();
            data["body"]  = JsBody();
            bool ret = Draco::instance()->SecQuesUpdate(data,
                                                        {},
                                                        servicesTagPtr()->passwordToken(),
                                                        "",
                                                        "",
                                                        false,
                                                        output,
                                                        errormsg);
            if (ret) {
                setRequireQuestions({});
                AutomaticGenerateSecurityQuestions();
                QString msg_name = QString("Security questions updated");
                AppModel::instance()->showToast(0, msg_name, EWARNING::WarningType::SUCCESS_MSG);
            }
            return true;
        }
    }
    else {
        DBG_INFO << errormsg;
    }
    return ret;
}

bool QKeyRecovery::RequestConfirmationCodeSetupSecurityQuestions()
{
    QString errormsg;
    QJsonObject output;
    QJsonObject data;
    data["nonce"] = Draco::instance()->randomNonce();
    data["body"] = JsBody();
    servicesTagPtr()->setConfirmCodeNonceBody(data);
    bool ret = Draco::instance()->RequestConfirmationCode("UPDATE_SECURITY_QUESTIONS", data, output, errormsg);
    if (ret) {
        DBG_INFO << output;
        QString code_id = output["code_id"].toString();
        servicesTagPtr()->setCode_id(code_id);
    }
    return ret;
}

bool QKeyRecovery::required(const QJsonObject& question)
{
    QString id = question["id"].toString();
    for (auto js : m_requireQuestions) {
        QJsonObject json = js.toObject();
        QString id_require = json["id"].toString();
        if (qUtils::strCompare(id_require, id))
        {
            return true;
        }
    }
    return false;
}

QJsonArray QKeyRecovery::findRemainQuestion()
{
    QJsonArray remain_questions;
    for (auto js : m_allQuestion) {
        QJsonObject json = js.toObject();
        if (!required(json)) {
            remain_questions.append(json);
        }
    }
    return remain_questions;
}

void QKeyRecovery::UpdateRequiredQuestion()
{
    QJsonArray remain_questions = findRemainQuestion();
    QJsonArray require_questions = m_requireQuestions;
    for(int i = 0; i < require_questions.size(); i++)
    {
        QJsonObject json = require_questions.at(i).toObject();
        QJsonArray remain = remain_questions;
        remain.prepend(json);
        if(json["answer"].isNull()) {
            json["answer"] = "";
        }
        if(json["change"].isNull()) {
            json["change"] = false;
        }
        if(json["isChanged"].isNull()) {
            json["isChanged"] = false;
        }
        json["remain_questions"] = remain;
        require_questions[i] = json;
    }
    setRequireQuestions(require_questions);
}

QJsonObject QKeyRecovery::convert(QVariant question)
{
    QMap<QString, QVariant> maps = question.toMap();
    QJsonObject json;
    json["id"] = maps["id"].toString();
    json["question"] = maps["question"].toString();
    json["answer"] = maps["answer"].toString();
    json["change"] = maps["change"].toBool();
    json["remain_questions"] = maps["remain_questions"].toJsonArray();
    return json;
}

QJsonArray QKeyRecovery::questionsAndChangeAnswers() const
{
    QJsonArray answers;
    for(auto js : m_requireQuestions) {
        QJsonObject obj = js.toObject();
        QJsonObject answer;
        QString str_answer = obj["answer"].toString();
        bool is_answered = obj["is_answered"].toBool();
        if (!str_answer.isEmpty() || is_answered) {
            answer["question_id"] = obj["id"];
            answer["answer"] = obj["answer"];
            answer["change"] = obj["change"];
            answers.append(answer);
        }
    }
    return answers;
}

void QKeyRecovery::changeRequiredQuestion(int index, QVariant new_ques)
{
    if (index >= m_requireQuestions.size()) return;
    QJsonArray require_questions = m_requireQuestions;
    QJsonObject own = convert(new_ques);
    require_questions[index] = own;
    setRequireQuestions(require_questions);
    UpdateRequiredQuestion();
}

void QKeyRecovery::setupSecQuesAnswer(int index, const QString &id, const QString &answer)
{
    if (m_requireQuestions.size() <= index) return;
    QJsonArray require_questions = m_requireQuestions;
    QJsonObject json = require_questions.at(index).toObject();
    QString id_change = json["id"].toString();
    json["isChanged"] = true;
    if (qUtils::strCompare(id_change, id)) {
        json["answer"] = answer.trimmed();
        json["change"] = true;
    }
    require_questions[index] = json;
    setRequireQuestions(require_questions);
}

void QKeyRecovery::createUserCustomizedQuestion(const QString &question)
{
    QString errormsg;
    QJsonObject output;
    QJsonArray require_questions = m_requireQuestions;
    bool ret = Draco::instance()->UserKeysCreateUserCustomizedQuestion(question, output, errormsg);
    if (ret) {
        DBG_INFO << output;
        QJsonObject newQuestion = output["question"].toObject();
        for(int i = 0; i < require_questions.size(); i++)
        {
            QJsonObject json = require_questions.at(i).toObject();
            if(json["id"].toString() == "my_question") {
                json["isChanged"] = true;
                json["id"] = newQuestion["id"];
                json["question"] = newQuestion["question"];
                json["is_answered"] = newQuestion["is_answered"];
                require_questions[i] = json;
                break;
            }
        }
        setRequireQuestions(require_questions);
    }
}

QWalletServicesTagPtr QKeyRecovery::servicesTagPtr() const
{
    return ServiceSetting::instance()->servicesTagPtr();
}

