#include "QWalletServicesTag.h"
#include "Premiums/QGroupWallets.h"
#include "Premiums/QInheritancePlan.h"
#include "Premiums/QKeyRecovery.h"
#include "Premiums/QServerKey.h"
#include "Servers/Byzantine.h"
#include "Servers/Draco.h"
#include "ViewsEnums.h"
#include "WalletModel.h"

QWalletServicesTagPtr QWalletServicesTag::instance() {
    static QWalletServicesTagPtr mInstance(new QWalletServicesTag());
    return mInstance;
}

QWalletServicesTag::QWalletServicesTag() : QInheritanceClaiming(), m_keyRecovery(QKeyRecoveryPtr(new QKeyRecovery())) {
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
    qmlRegisterType<InheritanceEnum>("NUNCHUCKTYPE", 1, 0, "INHERITANCE");
}

QWalletServicesTag::~QWalletServicesTag() {}

QString QWalletServicesTag::passwordToken() const {
    return m_passwordToken;
}

bool QWalletServicesTag::requestVerifyPassword(const QString &password, const int action) {
    clearToken();
    QString token = "";
    QString errormsg = "";
    bool ret = Draco::instance()->verifyPasswordToken(password, action, token, errormsg);
    if (!ret) {
        ServiceSetting::instance()->verifyPasswordTokenAlert(errormsg);
    } else {
        m_passwordToken = token;
    }
    return ret;
}

bool QWalletServicesTag::requestLockDownVerifyPassword(const QString &password) {
    return requestVerifyPassword(password, (int)TARGET_ACTION::EMERGENCY_LOCKDOWN);
}

bool QWalletServicesTag::requestRecoverKeyVerifyPassword(const QString &password) {
    return requestVerifyPassword(password, (int)TARGET_ACTION::DOWNLOAD_KEY_BACKUP);
}

bool QWalletServicesTag::requestUpdateSecurityQuestionPassword(const QString &password) {
    return requestVerifyPassword(password, (int)TARGET_ACTION::UPDATE_SECURITY_QUESTIONS);
}

bool QWalletServicesTag::requestServerKeyVerifyPassword(const QString &password) {
    return requestVerifyPassword(password, (int)TARGET_ACTION::UPDATE_SERVER_KEY);
}

bool QWalletServicesTag::requestInheritancePlanVerifyPassword(const QString &password) {
    return requestVerifyPassword(password, (int)TARGET_ACTION::UPDATE_INHERITANCE_PLAN);
}

bool QWalletServicesTag::requestDeleteWalletVerifyPassword(const QString &password) {
    return requestVerifyPassword(password, (int)TARGET_ACTION::DELETE_WALLET);
}

bool QWalletServicesTag::requestChangeEmailVerifyPassword(const QString &password) {
    return requestVerifyPassword(password, (int)TARGET_ACTION::CHANGE_EMAIL);
}

bool QWalletServicesTag::requestReplaceKeysVerifyPassword(const QString &password) {
    return requestVerifyPassword(password, (int)TARGET_ACTION::REPLACE_KEYS);
}

bool QWalletServicesTag::requestEditMemberVerifyPassword(const QString &password) {
    return requestVerifyPassword(password, (int)TARGET_ACTION::EDIT_GROUP_MEMBERS);
}

bool QWalletServicesTag::RequestConfirmationCodeEmergencyLockdown() {
    QWalletPtr w = ServiceSetting::instance()->walletInfoPtr();
    if (w.isNull())
        return false;
    QString errormsg;
    QJsonObject output;

    QJsonObject body;
    body["period_id"] = m_period_id;
    body["group_id"] = w->isUserWallet() ? "" : w->groupId();
    body["wallet"] = w->walletId();
    QJsonObject data;
    data["nonce"] = Draco::instance()->randomNonce();
    data["body"] = body;
    setConfirmCodeNonceBody(data);
    bool ret = Draco::instance()->RequestConfirmationCode("EMERGENCY_LOCKDOWN", data, output, errormsg);
    if (ret) {
        DBG_INFO << output;
        m_code_id = output["code_id"].toString();
    }
    return ret;
}

bool QWalletServicesTag::verifyConfirmationCode(const QString &code) {
    DBG_INFO << "id: " << m_code_id << "code: " << code;
    QString errormsg;
    QJsonObject output;
    bool ret = Draco::instance()->VerifyConfirmationCode(m_code_id, code, output, errormsg);
    if (ret) {
        DBG_INFO << output;
        m_confirmToken = output["token"].toString();
        emit confirmCodeVerified();
    } else {
        emit answerErrorAlert(errormsg);
    }
    return ret;
}

QVariantList QWalletServicesTag::securityQuestions() {
    DBG_INFO << QString().sprintf("%p", this);
    return m_questions.toVariantList();
}

void QWalletServicesTag::setQuestions(const QJsonArray &questions) {
    m_questions = questions;
    emit securityQuestionChanged();
}

bool QWalletServicesTag::CreateSecurityQuestionsAnswered() {
    QJsonObject output;
    QString errormsg;
    bool ret = Draco::instance()->secQuesGet(output, errormsg);
    if (ret) {
        QJsonArray questions_answered;
        for (auto q : output["questions"].toArray()) {
            auto q_js = q.toObject();
            bool is_answered = q_js["is_answered"].toBool();
            if (is_answered) {
                questions_answered.append(q);
            }
        }
        ReqiredSignaturesInfo required_signatures_info = reqiredSignaturesInfo();
        int required_answers = required_signatures_info.required_answers == 0 ? 1 : required_signatures_info.required_answers;
        QList<int> rIndex;
        rIndex.clear();
        for (int i = 0; i < questions_answered.size(); i++) {
            if (i < required_answers) {
                rIndex.append(i);
            } else {
                break;
            }
        }
        QJsonArray questions_required;
        m_quesAnswers.clear();
        for (auto index : rIndex) {
            auto ques = questions_answered[index].toObject();
            questions_required.append(ques);
            SecurityQuestion answer;
            answer.id = ques["id"].toString();
            answer.question = ques["question"].toString();
            answer.is_answered = ques["is_answered"].toBool();
            m_quesAnswers.append(answer);
            DBG_INFO << ques;
        }
        DBG_INFO << QString().sprintf("%p", this) << questions_required;
        setQuestions(questions_required);
    }
    return ret;
}

void QWalletServicesTag::secQuesAnswer(const QString &id, const QString &answer) {
    for (int i = 0; i < m_quesAnswers.size(); i++) {
        if (m_quesAnswers.at(i).id.localeAwareCompare(id) == 0) {
            if (!qUtils::strCompare(m_quesAnswers[i].answer, answer)) {
                m_quesAnswers[i].is_changed = true;
            }
            m_quesAnswers[i].answer = answer;
        }
    }
}

bool QWalletServicesTag::secQuesAnswer() {
    int correct_answer = 0;
    QString errormsg = "";
    QString token = "";
    bool ret = Draco::instance()->secQuesAnswer(questionsAndAnswers(), token, correct_answer, errormsg);
    if (!ret) {
        emit answerErrorAlert(errormsg);
        return false;
    } else {
        m_secQuesToken = token;
        if (reqiredSignaturesInfo().required_answers == 0) {
            return correct_answer;
        } else {
            return m_quesAnswers.size() == correct_answer && correct_answer != 0;
        }
    }
}

QJsonArray QWalletServicesTag::questionsAndAnswers() const {
    QJsonArray questions;
    for (auto v : m_quesAnswers) {
        QJsonObject question;
        question["question_id"] = v.id;
        question["answer"] = v.answer;
        questions.append(question);
    }
    return questions;
}

QVariantList QWalletServicesTag::periods() {
    return m_periods.toVariantList();
}

bool QWalletServicesTag::createLockdownPeriods() {
    QWalletPtr w = ServiceSetting::instance()->walletInfoPtr();
    if (w.isNull())
        return false;
    QJsonArray periods;
    QString errormsg;
    bool ret{false};
    if (w->isUserWallet()) {
        ret = Draco::instance()->lockdownPeriods(periods, errormsg);
    } else if (w->isGroupWallet()) {
        ret = Byzantine::instance()->lockdownPeriods(periods, errormsg);
    }
    m_periods = periods;
    if (!ret) {
        lockdownPeriodsAlert(errormsg);
        return false;
    } else {
        if (periods.size() == 0) {
            errormsg = "There no period";
            lockdownPeriodsAlert(errormsg);
            return false;
        }
    }
    return true;
}

bool QWalletServicesTag::lockdownRequired(const QString &period_id) {
    QWalletPtr w = ServiceSetting::instance()->walletInfoPtr();
    if (w.isNull())
        return false;
    m_period_id = period_id;
    DBG_INFO << w->walletId() << m_period_id;
    QJsonObject output;
    QString errormsg;
    bool ret{false};
    if (w->isUserWallet()) {
        ret = Draco::instance()->lockdownRequiredSignatures(m_period_id, w->walletId(), output, errormsg);
    } else if (w->isGroupWallet()) {
        ret = Byzantine::instance()->lockdownRequiredSignatures(w->groupId(), m_period_id, w->walletId(), output, errormsg);
    } else {
    }
    if (ret) {
        QJsonObject resultObj = output["result"].toObject();
        setReqiredSignatures(resultObj);
    }
    return ret;
}

bool QWalletServicesTag::lockdownByAnswerSecQues() {
    QWalletPtr w = ServiceSetting::instance()->walletInfoPtr();
    if (w.isNull())
        return false;
    QString errormsg;
    QString until_time;
    bool ret{false};
    if (w->isUserWallet()) {
        ret = Draco::instance()->lockdownByAnswerSecQues(m_passwordToken, m_secQuesToken, m_period_id, w->walletId(), until_time, errormsg);
    } else if (w->isGroupWallet()) {
        ret = Byzantine::instance()->lockdownByAnswerSecQues(w->groupId(), m_passwordToken, m_secQuesToken, m_period_id, w->walletId(), until_time, errormsg);
    } else {
    }

    if (ret) {
        setUntilTime(until_time);
    }
    return ret;
}

bool QWalletServicesTag::lockdownByConfirmationCode() {
    QWalletPtr w = ServiceSetting::instance()->walletInfoPtr();
    if (w.isNull())
        return false;
    QString errormsg;
    QString until_time;
    bool ret{false};
    if (w->isUserWallet()) {
        DBG_INFO << m_passwordToken << m_period_id << w->walletId();
    } else if (w->isGroupWallet()) {
        ret = Byzantine::instance()->lockdownByConfirmationCode(m_passwordToken, confirmToken(), confirmCodeNonceBody(), until_time, errormsg);
    } else {
    }
    if (ret) {
        setUntilTime(until_time);
    }
    setConfirmCodeNonceBody({}); // Clear
    return ret;
}

bool QWalletServicesTag::lockdownBySignDummyTx() {
    QWalletPtr w = ServiceSetting::instance()->walletInfoPtr();
    if (w.isNull())
        return false;
    QStringList signatures = m_signatures.values(); //==> Signatures sau khi sign dummy tx
    QString errormsg;
    QString until_time;
    bool ret{false};
    if (w->isUserWallet()) {
        ret = Draco::instance()->lockdownBySignDummyTx(signatures, m_passwordToken, m_period_id, w->walletId(), until_time, errormsg);
    } else if (w->isGroupWallet()) {
        ret = Byzantine::instance()->lockdownBySignDummyTx(w->groupId(), signatures, m_passwordToken, m_period_id, w->walletId(), until_time, errormsg);
    } else {
    }
    if (ret) {
        setUntilTime(until_time);
    }
    return ret;
}

QString QWalletServicesTag::untilTime() const {
    return m_untilTime;
}

void QWalletServicesTag::setUntilTime(QString untilTime) {
    if (m_untilTime == untilTime)
        return;

    m_untilTime = untilTime;
    emit untilTimeChanged();
}

QString QWalletServicesTag::secQuesToken() const {
    return m_secQuesToken;
}

void QWalletServicesTag::clearToken() {
    m_passwordToken = "";
    m_secQuesToken = "";
    m_confirmToken = "";
}

QVariant QWalletServicesTag::setupConfig() const {
    return QVariant::fromValue(m_setupConfig);
}

QJsonObject QWalletServicesTag::setupConfigJs() const {
    return m_setupConfig;
}

void QWalletServicesTag::setSetupConfig(const QJsonObject& config) {
    if (m_setupConfig == config)
        return;
        
    m_setupConfig = config;
    emit setupConfigChanged();
}

void QWalletServicesTag::additionalGetWalletConfig() {
    QJsonObject config;
    if (ClientController::instance()->isMultiSubscriptions()) {
        config = Byzantine::instance()->assistedGetWalletConfig();
    } else {
        if (ClientController::instance()->isUserWallet()) {
            config = Draco::instance()->assistedGetWalletConfig();
        } else if (ClientController::instance()->isGroupWallet()) {
            config = Byzantine::instance()->assistedGetWalletConfig();
        } else {
        }
    }
    DBG_INFO << config;
    setWalletConfig(config);
}

void QWalletServicesTag::configWalletSetup() {
    QJsonObject config;
    QString errormsg;
    bool ret = Draco::instance()->GetWalletSetupConfig(config, errormsg);
    if (ret) {
        DBG_INFO << config;
        setSetupConfig(config);
    }
}

QStringList QWalletServicesTag::listInheritantPlans() const {
    return m_inheritantPlans;
}

void QWalletServicesTag::setListInheritantPlans() {
    QStringList inheritantPlans{};
    for (auto wallet_id : WalletsMng->wallets()) {
        if (auto w = AppModel::instance()->walletListPtr()->getWalletById(wallet_id)) {
            if (w->isReplaced())
                continue;
            if (auto plan = w->inheritancePlanPtr()) {
                if (plan->IsActived()) {
                    inheritantPlans << wallet_id;
                }
            }
        }
    }
    if (m_inheritantPlans == inheritantPlans)
        return;
    m_inheritantPlans = inheritantPlans;
    emit listInheritantPlansChanged();
}

QStringList QWalletServicesTag::listLockdown() const {
    return m_listLockdown;
}

void QWalletServicesTag::setListLockdown() {
    QStringList setuped{};
    for (auto wallet_id : WalletsMng->wallets()) {
        if (auto w = AppModel::instance()->walletListPtr()->getWalletById(wallet_id)) {
            if (w->isReplaced())
                continue;
            if (auto dash = w->dashboard()) {
                if (dash->isGroupWallet()) {
                    auto hasPermission = dash->role() == "MASTER" || dash->role() == "ADMIN";
                    if (hasPermission) {
                        setuped << wallet_id; // for GroupWallet
                    }
                } else {
                    setuped << wallet_id;
                }
            } else {
                setuped << wallet_id;
            }
        }
    }
    if (m_listLockdown == setuped)
        return;
    m_listLockdown = setuped;
    emit listGroupWChanged();
}

QStringList QWalletServicesTag::listLocked() const {
    return m_listLocked;
}

void QWalletServicesTag::setListLocked() {
    QStringList setuped{};
    for (auto wallet_id : WalletsMng->wallets()) {
        if (auto w = AppModel::instance()->walletListPtr()->getWalletById(wallet_id)) {
            if (w->isReplaced())
                continue;
            if (auto dash = w->dashboard()) {
                auto hasPermission = dash->role() == "MASTER" || dash->role() == "ADMIN";
                if (dash->isLocked() && hasPermission) {
                    setuped << wallet_id;
                }
            }
        }
    }
    if (m_listLocked == setuped)
        return;
    m_listLocked = setuped;
    emit listLockedChanged();
}

QStringList QWalletServicesTag::listPolicy() const {
    return m_listPolicy;
}

void QWalletServicesTag::setListPolicy() {
    QStringList setuped{};
    for (auto wallet_id : WalletsMng->wallets()) {
        if (auto w = AppModel::instance()->walletListPtr()->getWalletById(wallet_id)) {
            if (w->isReplaced())
                continue;
            if (auto server = w->serverKeyPtr()) {
                if (!server->hasServerKey())
                    continue;
            }
            if (auto dash = w->dashboard()) {
                auto hasPermission = dash->role() == "KEYHOLDER" || dash->role() == "MASTER" || dash->role() == "ADMIN" || dash->role() == "FACILITATOR_ADMIN";
                if (hasPermission) {
                    setuped << wallet_id;
                } else if (w->isUserWallet()) {
                    setuped << wallet_id;
                }
            } else {
                setuped << wallet_id;
            }
        }
    }
    if (m_listPolicy == setuped)
        return;
    m_listPolicy = setuped;
    emit listPolicyChanged();
}

QStringList QWalletServicesTag::list2FA() const {
    return m_list2FA;
}

void QWalletServicesTag::setList2FA() {
    QStringList setuped{};
    for (auto wallet_id : WalletsMng->wallets()) {
        if (auto w = AppModel::instance()->walletListPtr()->getWalletById(wallet_id)) {
            if (w->isReplaced())
                continue;
            if (auto dash = w->dashboard()) {
                if (w->isGroupWallet()) {
                    auto hasPermission = dash->role() == "MASTER";
                    if (hasPermission) {
                        setuped << wallet_id;
                    }
                } else {
                    setuped << wallet_id;
                }
            } else {
                setuped << wallet_id;
            }
        }
    }
    if (m_list2FA == setuped)
        return;
    m_list2FA = setuped;
}

void QWalletServicesTag::ConfigServiceTag() {
    setListInheritantPlans();
    setListLockdown();
    setListLocked();
    setListPolicy();
    setList2FA();
}

QString QWalletServicesTag::code_id() const {
    return m_code_id;
}

void QWalletServicesTag::setCode_id(const QString &code_id) {
    m_code_id = code_id;
}

QKeyRecoveryPtr QWalletServicesTag::keyRecoveryPtr() const {
    return m_keyRecovery;
}

QVariant QWalletServicesTag::keyRecovery() const {
    return QVariant::fromValue(keyRecoveryPtr().data());
}

QJsonObject QWalletServicesTag::confirmCodeNonceBody() const {
    return m_confirmCodeRequestBody;
}

void QWalletServicesTag::setConfirmCodeNonceBody(const QJsonObject &nonceBody) {
    m_confirmCodeRequestBody = nonceBody;
}

bool QWalletServicesTag::RequestConfirmationChangeEmail(const QString &new_email) {
    QString errormsg;
    QJsonObject output;
    QJsonObject body;
    body["new_email"] = new_email;
    QJsonObject data;
    data["nonce"] = Draco::instance()->randomNonce();
    data["body"] = body;
    setConfirmCodeNonceBody(data);
    bool ret = Draco::instance()->RequestConfirmationCode("CHANGE_EMAIL", data, output, errormsg);
    if (ret) {
        DBG_INFO << output;
        m_code_id = output["code_id"].toString();
    }
    return ret;
}

QString QWalletServicesTag::confirmToken() const {
    return m_confirmToken;
}

QVariant QWalletServicesTag::walletConfig() const {
    return QVariant::fromValue(m_walletConfig);
}

void QWalletServicesTag::setWalletConfig(const QJsonObject &config) {
    if (m_walletConfig == config)
        return;

    m_walletConfig = config;

    int total_remaining = 0;
    for (const QString &key : m_walletConfig.keys()) {
        if (m_walletConfig[key].isObject()) {
            QJsonObject subObj = m_walletConfig[key].toObject();
            total_remaining += subObj["remaining_wallet_count"].toInt();
        }
    }
    if (total_remaining != 0) {
        m_walletConfig["remaining_wallet_count"] = total_remaining;
    }
    emit walletConfigChanged();
}

QVariant QWalletServicesTag::reqiredSignatures() const {
    return QVariant::fromValue(m_reqiredSignatures);
}

QJsonObject QWalletServicesTag::reqiredSignaturesJs() const {
    return m_reqiredSignatures;
}

void QWalletServicesTag::setReqiredSignatures(const QJsonObject &reqired) {
    if (m_reqiredSignatures == reqired)
        return;

    m_reqiredSignatures = reqired;
    emit reqiredSignaturesChanged();
}

ReqiredSignaturesInfo QWalletServicesTag::reqiredSignaturesInfo() const {
    ReqiredSignaturesInfo reqired_signature{};
    reqired_signature.type = required_signatures_type[m_reqiredSignatures["type"].toString()];
    reqired_signature.required_signatures = m_reqiredSignatures["required_signatures"].toInt();
    reqired_signature.required_answers = m_reqiredSignatures["required_answers"].toInt();
    reqired_signature.required_confirmation_codes = m_reqiredSignatures["required_confirmation_codes"].toInt();
    return reqired_signature;
}
