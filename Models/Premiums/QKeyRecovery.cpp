#include "QKeyRecovery.h"
#include "ViewsEnums.h"
#include "Servers/Draco.h"
#include "Servers/Byzantine.h"
#include "WalletModel.h"
#include "Premiums/QGroupWallets.h"
#include "ServiceSetting.h"
#include "QWalletServicesTag.h"
#include "Premiums/QGroupDashboard.h"
#include "Premiums/QGroupWalletDummyTx.h"
#include "Premiums/QInheritancePlan.h"
#include "Premiums/QServerKey.h"

#include <QScopeGuard>
#include <QSet>

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
                    QString textR = card_id.right(5);
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
        QByteArray ba = base64.toUtf8();
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
        QByteArray ba = base64.toUtf8();
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
    if (!m_securityQuestionWalletId.isEmpty()) {
        body["wallet"] = m_securityQuestionWalletId;
    }
    return body;
}

bool QKeyRecovery::securityQuestionUpdateInProgress() const
{
    return m_securityQuestionUpdateInProgress;
}

QString QKeyRecovery::securityQuestionCustomDraft() const
{
    return m_securityQuestionCustomDraft;
}

void QKeyRecovery::setSecurityQuestionUpdateInProgress(bool inProgress)
{
    if (m_securityQuestionUpdateInProgress == inProgress) {
        return;
    }
    m_securityQuestionUpdateInProgress = inProgress;
    emit securityQuestionUpdateInProgressChanged();
}

void QKeyRecovery::setSecurityQuestionCustomDraft(const QString &draft)
{
    if (m_securityQuestionCustomDraft == draft) {
        return;
    }
    m_securityQuestionCustomDraft = draft;
    emit securityQuestionCustomDraftChanged();
}

QWalletPtr QKeyRecovery::securityQuestionWallet() const
{
    if (m_securityQuestionWalletId.isEmpty()) {
        return {};
    }
    if (auto walletList = AppModel::instance()->walletListPtr()) {
        return walletList->getWalletById(m_securityQuestionWalletId);
    }
    return {};
}

bool QKeyRecovery::isSecurityQuestionUpdateWalletValid() const
{
    auto wallet = securityQuestionWallet();
    return wallet &&
           (canUpdateSecurityQuestionViaInheritance(wallet) || canUpdateSecurityQuestionVia2FA(wallet));
}

bool QKeyRecovery::isSecurityQuestionWalletUsable(const QWalletPtr &wallet) const
{
    if (!wallet || wallet->walletId().isEmpty() || !wallet->isAssistedWallet() || wallet->isReplaced()) {
        return false;
    }
    if (wallet->isLocked() || qUtils::strCompare(wallet->status(), "LOCKED")) {
        return false;
    }
    if (auto dashboard = wallet->dashboard()) {
        if (dashboard->isLocked()) {
            return false;
        }
        if (wallet->isGroupWallet() && dashboard->role() != "MASTER") {
            return false;
        }
    } else if (wallet->isGroupWallet()) {
        // Group permissions and lock status are unknown until dashboard data exists.
        return false;
    }
    return true;
}

bool QKeyRecovery::canUpdateSecurityQuestionViaInheritance(const QWalletPtr &wallet) const
{
    if (!isSecurityQuestionWalletUsable(wallet)) {
        return false;
    }
    if (auto plan = wallet->inheritancePlanPtr()) {
        return plan->IsActived();
    }
    return false;
}

bool QKeyRecovery::canUpdateSecurityQuestionVia2FA(const QWalletPtr &wallet) const
{
    if (!isSecurityQuestionWalletUsable(wallet)) {
        return false;
    }
    if (auto serverKey = wallet->serverKeyPtr()) {
        return serverKey->hasServerKey();
    }
    return false;
}

void QKeyRecovery::clearSecurityQuestionUpdateContext(bool clearQuestions)
{
    ++m_securityQuestionUpdateGeneration;
    m_securityQuestionWalletId.clear();
    m_securityQuestionUpdateBody = {};
    m_securityQuestionCustomQuestionIds.clear();
    setSecurityQuestionCustomDraft({});

    if (clearQuestions) {
        setRequireQuestions({});
        m_allQuestion = {};
        m_securityQuestionInitialQuestions = {};
    }

    if (auto tag = servicesTagPtr()) {
        tag->setConfirmCodeNonceBody({});
        tag->setReqiredSignatures({});
        tag->setCode_id({});
        tag->clearToken();
        tag->clearSecurityQuestionAnswers();
    }
}

void QKeyRecovery::resetSecurityQuestionUpdate()
{
    clearSecurityQuestionUpdateContext(true);
}

void QKeyRecovery::clearSecurityQuestionUpdateAuthorization()
{
    if (securityQuestionUpdateInProgress()) {
        return;
    }
    if (auto tag = servicesTagPtr()) {
        tag->clearSecurityQuestionAuthorizationState();
    }
}

bool QKeyRecovery::prepareSecurityQuestionUpdateWallet(QWalletPtr &wallet)
{
    wallet.clear();
    if (securityQuestionUpdateInProgress()) {
        return false;
    }
    resetSecurityQuestionUpdate();

    auto walletList = AppModel::instance()->walletListPtr();
    auto tag = servicesTagPtr();
    if (!walletList || !tag) {
        return false;
    }

    // Rebuild the existing caches, then validate every candidate again for this
    // Security Question operation. The shared list semantics remain unchanged.
    tag->ConfigServiceTag();
    for (const QString &walletId : tag->listInheritantPlans()) {
        auto candidate = walletList->getWalletById(walletId);
        if (canUpdateSecurityQuestionViaInheritance(candidate)) {
            wallet = candidate;
            break;
        }
    }
    if (!wallet) {
        for (const QString &walletId : tag->list2FA()) {
            auto candidate = walletList->getWalletById(walletId);
            if (canUpdateSecurityQuestionVia2FA(candidate)) {
                wallet = candidate;
                break;
            }
        }
    }
    if (!wallet) {
        return false;
    }

    m_securityQuestionWalletId = wallet->walletId();
    return true;
}

bool QKeyRecovery::startSecurityQuestionUpdate(const QString &password, QWalletPtr &wallet)
{
    if (securityQuestionUpdateInProgress()) {
        return false;
    }

    bool completed = false;
    const auto failureGuard = qScopeGuard([this, &completed]() {
        if (!completed) {
            emit securityQuestionUpdateStartFailed();
        }
    });
    if (!prepareSecurityQuestionUpdateWallet(wallet)) {
        AppModel::instance()->showToast(0,
                                        "No eligible wallet is available to update security questions.",
                                        EWARNING::WarningType::ERROR_MSG);
        return false;
    }

    setSecurityQuestionUpdateInProgress(true);
    const quint64 generation = m_securityQuestionUpdateGeneration;
    const auto progressGuard = qScopeGuard([this]() {
        setSecurityQuestionUpdateInProgress(false);
    });
    const auto canCommit = [this, generation]() {
        return generation == m_securityQuestionUpdateGeneration;
    };

    auto tag = servicesTagPtr();
    if (!tag->requestUpdateSecurityQuestionPasswordGuarded(password, canCommit) ||
        generation != m_securityQuestionUpdateGeneration ||
        !CreateAllSecurityQuestionsGuarded(canCommit) ||
        generation != m_securityQuestionUpdateGeneration) {
        if (generation == m_securityQuestionUpdateGeneration) {
            clearSecurityQuestionUpdateContext(true);
        }
        wallet.clear();
        return false;
    }
    if (!isSecurityQuestionUpdateWalletValid()) {
        AppModel::instance()->showToast(0,
                                        "The selected wallet is no longer eligible to update security questions.",
                                        EWARNING::WarningType::ERROR_MSG);
        clearSecurityQuestionUpdateContext(true);
        wallet.clear();
        return false;
    }

    completed = true;
    return true;
}

bool QKeyRecovery::buildSecurityQuestionUpdateBody(QJsonObject &body, QString &errorMessage) const
{
    body = {};
    errorMessage.clear();

    if (!isSecurityQuestionUpdateWalletValid()) {
        errorMessage = "No eligible wallet is available to update security questions.";
        return false;
    }

    QJsonArray answers;
    QSet<QString> questionIds;
    bool hasChangedAnswer = false;
    for (const QJsonValue &value : m_requireQuestions) {
        const QJsonObject question = value.toObject();
        const QString questionId = question["id"].toString().trimmed();
        QString requestQuestionId = questionId;
        const QString answerText = question["answer"].toString();
        const bool isAnswered = question["is_answered"].toBool();
        const bool isChanged = question["isChanged"].toBool();
        const bool change = question["change"].toBool();

        if (questionId.isEmpty()) {
            errorMessage = "Please complete the custom security question before continuing.";
            return false;
        }
        if (questionId == "my_question") {
            const QString stagedQuestionId =
                m_securityQuestionCustomQuestionIds.value(m_securityQuestionCustomDraft);
            if (stagedQuestionId.isEmpty()) {
                errorMessage = "Please complete the custom security question before continuing.";
                return false;
            }
            requestQuestionId = stagedQuestionId;
        }
        if (questionIds.contains(requestQuestionId)) {
            errorMessage = "Each security question must be unique.";
            return false;
        }
        questionIds.insert(requestQuestionId);

        if (isChanged && !change) {
            errorMessage = "Please complete the changed security question before continuing.";
            return false;
        }
        if (change) {
            hasChangedAnswer = true;
            if (answerText.trimmed().isEmpty()) {
                errorMessage = "Please enter an answer for every changed security question.";
                return false;
            }
        }

        // An unchanged answered question intentionally carries an empty answer:
        // the backend keeps the existing case-sensitive answer in that case.
        if (!answerText.isEmpty() || isAnswered || change) {
            QJsonObject answer;
            answer["question_id"] = requestQuestionId;
            answer["answer"] = answerText;
            answer["change"] = change;
            answers.append(answer);
        }
    }

    if (answers.isEmpty() || !hasChangedAnswer) {
        errorMessage = "Please change at least one security question before continuing.";
        return false;
    }

    body["questions_and_answers"] = answers;
    body["wallet"] = m_securityQuestionWalletId;
    return true;
}

bool QKeyRecovery::validateSecurityQuestionDraft(const QString &customQuestion,
                                                 QString &errorMessage) const
{
    errorMessage.clear();
    if (!isSecurityQuestionUpdateWalletValid()) {
        errorMessage = "No eligible wallet is available to update security questions.";
        return false;
    }

    QSet<QString> questionIds;
    bool hasCustomQuestion = false;
    bool hasCompleteChange = false;
    for (const QJsonValue &value : m_requireQuestions) {
        const QJsonObject question = value.toObject();
        const QString questionId = question["id"].toString().trimmed();
        if (questionId.isEmpty()) {
            errorMessage = "Please select every security question before continuing.";
            return false;
        }
        if (questionIds.contains(questionId)) {
            errorMessage = "Each security question must be unique.";
            return false;
        }
        questionIds.insert(questionId);
        hasCustomQuestion = hasCustomQuestion || questionId == "my_question";

        if (!question["isChanged"].toBool()) {
            continue;
        }
        if (!question["change"].toBool() ||
            question["answer"].toString().trimmed().isEmpty()) {
            errorMessage = "Please enter an answer for every changed security question.";
            return false;
        }
        hasCompleteChange = true;
    }

    if (!hasCompleteChange) {
        errorMessage = "Please change at least one security question before continuing.";
        return false;
    }
    if (hasCustomQuestion && customQuestion.trimmed().isEmpty()) {
        errorMessage = "Please enter your custom security question.";
        return false;
    }
    return true;
}

bool QKeyRecovery::submitSecurityQuestionUpdate(const QString &customQuestion)
{
    if (securityQuestionUpdateInProgress()) {
        return false;
    }

    // Complete the local preflight before creating a custom question on the
    // server so an invalid/locked wallet or incomplete draft has no remote
    // side effect.
    QString validationError;
    if (!validateSecurityQuestionDraft(customQuestion, validationError)) {
        AppModel::instance()->showToast(0, validationError, EWARNING::WarningType::ERROR_MSG);
        return false;
    }

    setSecurityQuestionUpdateInProgress(true);
    const quint64 generation = m_securityQuestionUpdateGeneration;
    const auto progressGuard = qScopeGuard([this]() {
        setSecurityQuestionUpdateInProgress(false);
    });

    bool hasCustomQuestion = false;
    for (const QJsonValue &value : m_requireQuestions) {
        if (value.toObject()["id"].toString() == "my_question") {
            hasCustomQuestion = true;
            break;
        }
    }
    if (hasCustomQuestion) {
        if (customQuestion.trimmed().isEmpty()) {
            AppModel::instance()->showToast(0,
                                            "Please enter your custom security question.",
                                            EWARNING::WarningType::ERROR_MSG);
            return false;
        }
        if (!createUserCustomizedQuestion(customQuestion)) {
            return false;
        }
        if (generation != m_securityQuestionUpdateGeneration) {
            return false;
        }
    }

    return UpdateSecurityQuestionsRequiredSignatures();
}

bool QKeyRecovery::completeSecurityQuestionUpdateAuthorization(const QString &authorizationInput)
{
    if (securityQuestionUpdateInProgress()) {
        return false;
    }

    setSecurityQuestionUpdateInProgress(true);
    const quint64 generation = m_securityQuestionUpdateGeneration;
    const auto progressGuard = qScopeGuard([this]() {
        setSecurityQuestionUpdateInProgress(false);
    });

    auto tag = servicesTagPtr();
    const auto canCommit = [this, generation]() {
        return generation == m_securityQuestionUpdateGeneration;
    };
    const int type = tag->reqiredSignaturesInfo().type;
    bool authorized = false;
    if (type == (int)REQUIRED_SIGNATURE_TYPE_INT::SECURITY_QUESTION) {
        authorized = tag->secQuesAnswerGuarded(canCommit);
    } else if (type == (int)REQUIRED_SIGNATURE_TYPE_INT::CONFIRMATION_CODE) {
        authorized = tag->verifyConfirmationCodeGuarded(authorizationInput, canCommit);
    } else {
        AppModel::instance()->showToast(0,
                                        "This authorization method is not valid for this screen.",
                                        EWARNING::WarningType::ERROR_MSG);
        return false;
    }

    if (!authorized || generation != m_securityQuestionUpdateGeneration) {
        return false;
    }
    return UpdateSecurityQuestions();
}

bool QKeyRecovery::CreateAllSecurityQuestions()
{
    return CreateAllSecurityQuestionsGuarded([]() { return true; });
}

bool QKeyRecovery::CreateAllSecurityQuestionsGuarded(const std::function<bool()> &canCommit)
{
    QJsonObject output;
    QString errormsg;
    bool ret = Draco::instance()->secQuesGet(output, errormsg);
    if (canCommit && !canCommit()) {
        return false;
    }
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
        // Publish only the completed model below, after remain_questions and
        // local change-tracking fields have been populated.
        m_requireQuestions = require_questions;
    }
    UpdateRequiredQuestion();
    m_securityQuestionInitialQuestions = m_requireQuestions;
    return true;
}

bool QKeyRecovery::UpdateSecurityQuestions()
{
    if (m_securityQuestionUpdateBody.isEmpty() ||
        !isSecurityQuestionUpdateWalletValid()) {
        AppModel::instance()->showToast(0,
                                        "The selected wallet is no longer eligible to update security questions.",
                                        EWARNING::WarningType::ERROR_MSG);
        return false;
    }

    const QJsonObject authorizationBody = servicesTagPtr()->confirmCodeNonceBody();
    if (authorizationBody["body"].toObject() != m_securityQuestionUpdateBody) {
        AppModel::instance()->showToast(0,
                                        "The security question authorization has expired. Please try again.",
                                        EWARNING::WarningType::ERROR_MSG);
        return false;
    }

    const quint64 generation = m_securityQuestionUpdateGeneration;
    QJsonObject output;
    QString errormsg;
    bool ret = Draco::instance()->SecQuesUpdate(authorizationBody,
                                                {},
                                                servicesTagPtr()->passwordToken(),
                                                servicesTagPtr()->secQuesToken(),
                                                servicesTagPtr()->confirmToken(),
                                                false,
                                                output,
                                                errormsg);
    if (generation != m_securityQuestionUpdateGeneration) {
        return false;
    }
    if (ret) {
        setRequireQuestions({});
        AutomaticGenerateSecurityQuestions();
        clearSecurityQuestionUpdateContext(false);
    }
    return ret;
}

bool QKeyRecovery::UpdateSecurityQuestionsRequiredSignatures()
{
    const quint64 generation = m_securityQuestionUpdateGeneration;
    QJsonObject requestBody;
    QString validationError;
    if (!buildSecurityQuestionUpdateBody(requestBody, validationError)) {
        AppModel::instance()->showToast(0, validationError, EWARNING::WarningType::ERROR_MSG);
        return false;
    }
    m_securityQuestionUpdateBody = requestBody;

    QJsonObject output;
    QString errormsg;
    bool ret = Draco::instance()->UpdateSecQuesRequiredSignatures(m_securityQuestionUpdateBody,
                                                output,
                                                errormsg);
    if (generation != m_securityQuestionUpdateGeneration) {
        return false;
    }
    if (ret) {
        if (m_securityQuestionUpdateBody["wallet"].toString() != m_securityQuestionWalletId ||
            !isSecurityQuestionUpdateWalletValid()) {
            AppModel::instance()->showToast(0,
                                            "The selected wallet is no longer eligible to update security questions.",
                                            EWARNING::WarningType::ERROR_MSG);
            return false;
        }
        QJsonObject resultObj = output["result"].toObject();
        DBG_INFO << resultObj;
        const QString authorizationType = resultObj["type"].toString();
        if (!required_signatures_type.contains(authorizationType)) {
            AppModel::instance()->showToast(0,
                                            "The server returned an unsupported authorization method.",
                                            EWARNING::WarningType::ERROR_MSG);
            return false;
        }
        servicesTagPtr()->setReqiredSignatures(resultObj);
        ReqiredSignaturesInfo required_question = servicesTagPtr()->reqiredSignaturesInfo();
        if (required_question.type == (int)REQUIRED_SIGNATURE_TYPE_INT::SECURITY_QUESTION) {
            const QString nonce = Draco::instance()->randomNonce();
            if (generation != m_securityQuestionUpdateGeneration || nonce.isEmpty()) {
                return false;
            }
            if (!isSecurityQuestionUpdateWalletValid()) {
                AppModel::instance()->showToast(0,
                                                "The selected wallet is no longer eligible to update security questions.",
                                                EWARNING::WarningType::ERROR_MSG);
                return false;
            }
            QJsonObject data;
            data["nonce"] = nonce;
            data["body"] = m_securityQuestionUpdateBody;
            servicesTagPtr()->setConfirmCodeNonceBody(data);
            const auto canCommit = [this, generation]() {
                return generation == m_securityQuestionUpdateGeneration;
            };
            if (servicesTagPtr()->CreateSecurityQuestionsAnsweredGuarded(canCommit)) {
                if (generation != m_securityQuestionUpdateGeneration ||
                    !isSecurityQuestionUpdateWalletValid()) {
                    return false;
                }
                QEventProcessor::instance()->sendEvent(E::EVT_SETUP_ANSWER_SECURITY_QUESTION_REQ);
                return true;
            }
            return false;
        } else if (required_question.type == (int)REQUIRED_SIGNATURE_TYPE_INT::CONFIRMATION_CODE) {
            if (RequestConfirmationCodeSetupSecurityQuestions()) {
                QEventProcessor::instance()->sendEvent(E::EVT_SETUP_ANSWER_SECURITY_QUESTION_REQ);
                return true;
            }
            return false;
        } else if (required_question.type == (int)REQUIRED_SIGNATURE_TYPE_INT::SIGN_DUMMY_TX) {
            const QString nonce = Draco::instance()->randomNonce();
            if (generation != m_securityQuestionUpdateGeneration || nonce.isEmpty()) {
                return false;
            }
            if (!isSecurityQuestionUpdateWalletValid()) {
                AppModel::instance()->showToast(0,
                                                "The selected wallet is no longer eligible to update security questions.",
                                                EWARNING::WarningType::ERROR_MSG);
                return false;
            }
            QJsonObject output;
            QString errormsg = "";
            QJsonObject data;
            data["nonce"] = nonce;
            data["body"]  = m_securityQuestionUpdateBody;
            QStringList authorizations;
            bool ret = Draco::instance()->SecQuesUpdate(data,
                                                        authorizations,
                                                        servicesTagPtr()->passwordToken(),
                                                        "",
                                                        "",
                                                        true,
                                                        output,
                                                        errormsg);
            if (generation != m_securityQuestionUpdateGeneration) {
                return false;
            }
            if(ret){
                QJsonObject dummy_transaction = output["dummy_transaction"].toObject();
                if (auto w = securityQuestionWallet()) {
                    if (auto dummy = w->groupDummyTxPtr()) {
                        DBG_INFO << dummy_transaction;
                        dummy->setDummyTxData(dummy_transaction);
                        QEventProcessor::instance()->sendEvent(E::EVT_HEALTH_CHECK_STARTING_REQUEST);
                        // sendEvent() replaces the setup popup synchronously.
                        // Clear the sensitive draft only after the old form is
                        // gone so it cannot repaint with an empty model first.
                        clearSecurityQuestionUpdateContext(true);
                        return true;
                    }
                }
            }
            return false;
        } else if (required_question.type == (int)REQUIRED_SIGNATURE_TYPE_INT::NONE) {
            const QString nonce = Draco::instance()->randomNonce();
            if (generation != m_securityQuestionUpdateGeneration || nonce.isEmpty()) {
                return false;
            }
            if (!isSecurityQuestionUpdateWalletValid()) {
                AppModel::instance()->showToast(0,
                                                "The selected wallet is no longer eligible to update security questions.",
                                                EWARNING::WarningType::ERROR_MSG);
                return false;
            }
            QJsonObject output;
            QString errormsg = "";
            QJsonObject data;
            data["nonce"] = nonce;
            data["body"]  = m_securityQuestionUpdateBody;
            bool ret = Draco::instance()->SecQuesUpdate(data,
                                                        {},
                                                        servicesTagPtr()->passwordToken(),
                                                        "",
                                                        "",
                                                        false,
                                                        output,
                                                        errormsg);
            if (generation != m_securityQuestionUpdateGeneration) {
                return false;
            }
            if (ret) {
                setRequireQuestions({});
                AutomaticGenerateSecurityQuestions();
                QString msg_name = QString("Security questions updated");
                AppModel::instance()->showToast(0, msg_name, EWARNING::WarningType::SUCCESS_MSG);
                clearSecurityQuestionUpdateContext(false);
                QEventProcessor::instance()->sendEvent(E::EVT_CLOSE_TO_SERVICE_SETTINGS_REQUEST);
            }
            return ret;
        } else {
            AppModel::instance()->showToast(0,
                                            "This authorization method is not supported for updating security questions.",
                                            EWARNING::WarningType::ERROR_MSG);
            return false;
        }
    }
    else {
        DBG_INFO << errormsg;
    }
    return ret;
}

bool QKeyRecovery::RequestConfirmationCodeSetupSecurityQuestions()
{
    const quint64 generation = m_securityQuestionUpdateGeneration;
    QString errormsg;
    QJsonObject output;
    const QString nonce = Draco::instance()->randomNonce();
    if (generation != m_securityQuestionUpdateGeneration || nonce.isEmpty()) {
        return false;
    }
    if (!isSecurityQuestionUpdateWalletValid()) {
        AppModel::instance()->showToast(0,
                                        "The selected wallet is no longer eligible to update security questions.",
                                        EWARNING::WarningType::ERROR_MSG);
        return false;
    }
    QJsonObject data;
    data["nonce"] = nonce;
    data["body"] = m_securityQuestionUpdateBody;
    servicesTagPtr()->setConfirmCodeNonceBody(data);
    bool ret = Draco::instance()->RequestConfirmationCode("UPDATE_SECURITY_QUESTIONS", data, output, errormsg);
    if (generation != m_securityQuestionUpdateGeneration ||
        !isSecurityQuestionUpdateWalletValid()) {
        return false;
    }
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
    json["is_answered"] = maps["is_answered"].toBool();
    json["remain_questions"] = maps["remain_questions"].toJsonArray();
    return json;
}

QJsonArray QKeyRecovery::questionsAndChangeAnswers() const
{
    QJsonArray answers;
    for(auto js : m_requireQuestions) {
        QJsonObject obj = js.toObject();
        QJsonObject answer;
        QString questionId = obj["id"].toString();
        if (questionId == "my_question") {
            const QString stagedQuestionId =
                m_securityQuestionCustomQuestionIds.value(m_securityQuestionCustomDraft);
            if (!stagedQuestionId.isEmpty()) {
                questionId = stagedQuestionId;
            }
        }
        QString str_answer = obj["answer"].toString();
        bool is_answered = obj["is_answered"].toBool();
        if (!str_answer.isEmpty() || is_answered) {
            answer["question_id"] = questionId;
            answer["answer"] = obj["answer"];
            answer["change"] = obj["change"];
            answers.append(answer);
        }
    }
    return answers;
}

void QKeyRecovery::changeRequiredQuestion(int index, QVariant new_ques)
{
    if (index < 0 || index >= m_requireQuestions.size()) return;
    QJsonArray require_questions = m_requireQuestions;
    const QJsonObject current = require_questions.at(index).toObject();
    QJsonObject own = convert(new_ques);
    if (current["id"].toString() == own["id"].toString()) {
        return;
    }

    const QJsonObject initial = index < m_securityQuestionInitialQuestions.size()
                                    ? m_securityQuestionInitialQuestions.at(index).toObject()
                                    : QJsonObject{};

    // A selection differing from the initial row is a real form edit, but it
    // is not submittable until the user supplies the new answer. Returning to
    // the initial selection clears the dirty state.
    own["isChanged"] = initial["id"].toString() != own["id"].toString();
    own["change"] = false;
    own["answer"] = "";
    require_questions[index] = own;

    // Stage the selection without notifying QML. UpdateRequiredQuestion()
    // rebuilds every remain_questions list and publishes one complete model.
    // Emitting the intermediate item here leaves a recreated ComboBox with an
    // empty options model when the selected item came from m_allQuestion.
    m_requireQuestions = require_questions;
    UpdateRequiredQuestion();
}

void QKeyRecovery::setupSecQuesAnswer(int index, const QString &id, const QString &answer)
{
    if (index < 0 || m_requireQuestions.size() <= index) return;
    QJsonArray require_questions = m_requireQuestions;
    QJsonObject json = require_questions.at(index).toObject();
    if (!qUtils::strCompare(json["id"].toString(), id)) {
        return;
    }

    // Answers are case-sensitive. Compare and preserve the exact committed
    // value; trimming is used only to decide whether the draft is complete.
    json["answer"] = answer;
    const QJsonObject initial = index < m_securityQuestionInitialQuestions.size()
                                    ? m_securityQuestionInitialQuestions.at(index).toObject()
                                    : QJsonObject{};
    const bool questionChanged = initial["id"].toString() != id;
    const bool answerChanged = initial["answer"].toString() != answer;
    const bool isChanged = questionChanged || answerChanged;
    json["isChanged"] = isChanged;
    json["change"] = isChanged && !answer.trimmed().isEmpty();
    require_questions[index] = json;
    setRequireQuestions(require_questions);
}

bool QKeyRecovery::createUserCustomizedQuestion(const QString &question)
{
    const quint64 generation = m_securityQuestionUpdateGeneration;
    const QString customQuestion = question.trimmed();
    if (customQuestion.isEmpty()) {
        return false;
    }

    bool hasPlaceholder = false;
    for (const QJsonValue &value : m_requireQuestions) {
        if (value.toObject()["id"].toString() == "my_question") {
            hasPlaceholder = true;
            break;
        }
    }
    if (!hasPlaceholder) {
        return false;
    }

    // A failed calculate-required-signatures request may be retried from the
    // same form. Reuse any question already created in this update context,
    // including an A -> B -> A edit sequence, instead of creating a duplicate.
    if (!m_securityQuestionCustomQuestionIds.value(customQuestion).isEmpty()) {
        setSecurityQuestionCustomDraft(customQuestion);
        return true;
    }

    QString errormsg;
    QJsonObject output;
    bool ret = Draco::instance()->UserKeysCreateUserCustomizedQuestion(customQuestion, output, errormsg);
    if (generation != m_securityQuestionUpdateGeneration) {
        return false;
    }
    if (ret) {
        DBG_INFO << output;
        const QJsonObject newQuestion = output["question"].toObject();
        const QString newQuestionId = newQuestion["id"].toString().trimmed();
        if (newQuestionId.isEmpty()) {
            AppModel::instance()->showToast(0,
                                            "Could not create the custom security question.",
                                            EWARNING::WarningType::ERROR_MSG);
            return false;
        }
        // Keep the live edit model on the local "my_question" sentinel. The
        // staged server ID is substituted only while freezing the request
        // body, preventing the visible delegates from being recreated between
        // the create-custom and calculate-signatures calls.
        m_securityQuestionCustomQuestionIds.insert(customQuestion, newQuestionId);
        setSecurityQuestionCustomDraft(customQuestion);
        return true;
    }
    return false;
}

QWalletServicesTagPtr QKeyRecovery::servicesTagPtr() const
{
    return QWalletServicesTag::instance();
}
