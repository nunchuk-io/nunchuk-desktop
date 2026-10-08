#ifndef QKEYRECOVERY_H
#define QKEYRECOVERY_H

#include <QObject>
#include <QHash>
#include <QJsonArray>
#include <QJsonObject>
#include <functional>
#include "TypeDefine.h"

#define Start_key_recovery public
#define Update_security_questions public
class QKeyRecovery : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList tapsigners          READ tapsigners             NOTIFY tapsignersChanged)
    Q_PROPERTY(QVariant signer                  READ signer                 CONSTANT)
    Q_PROPERTY(QVariantList requireQuestions    READ requireQuestions       NOTIFY requireQuestionsChanged)
    Q_PROPERTY(QVariant firstQuestion           READ firstQuestion          NOTIFY requireQuestionsChanged)
    Q_PROPERTY(bool securityQuestionUpdateInProgress READ securityQuestionUpdateInProgress NOTIFY securityQuestionUpdateInProgressChanged)
    Q_PROPERTY(QString securityQuestionCustomDraft READ securityQuestionCustomDraft NOTIFY securityQuestionCustomDraftChanged)
public:
    explicit QKeyRecovery();

    QVariant signer() const;
    void setSigner(const QString& xfp);

    QVariantList tapsigners() const;
    void setTapsigners(const QJsonArray& tapsigners);

    QVariantList requireQuestions() const;
    void setRequireQuestions(const QJsonArray &requireQuestions);

    QVariant firstQuestion() const;
Start_key_recovery:
    bool CreateTapsigners();
    bool UserKeysCalculateRequiredSignatures(const QString& xfp);
    bool RequestConfirmationCodeKeyRecovery();
    bool startRecovery(const QString& backup_password);
    bool UserKeysMarkRecoverStatus();
    bool UserKeysRequestRecoveryKey();
    bool UserKeysRecoveryKey(const QString& xfp);
    bool UserKeysRecoveryKey();
    bool KeyRecoveryDownloadBackup();

Update_security_questions:
    QJsonObject JsBody() const;
    bool startSecurityQuestionUpdate(const QString &password, QWalletPtr &wallet);
    bool prepareSecurityQuestionUpdateWallet(QWalletPtr &wallet);
    bool submitSecurityQuestionUpdate(const QString &customQuestion);
    bool completeSecurityQuestionUpdateAuthorization(const QString &authorizationInput);
    bool securityQuestionUpdateInProgress() const;
    QString securityQuestionCustomDraft() const;
    bool CreateAllSecurityQuestions();
    bool AutomaticGenerateSecurityQuestions();
    bool UpdateSecurityQuestions();
    bool UpdateSecurityQuestionsRequiredSignatures();
    bool RequestConfirmationCodeSetupSecurityQuestions();
    bool required(const QJsonObject& question);
    QJsonArray findRemainQuestion();
    void UpdateRequiredQuestion();
    QJsonObject convert(QVariant question);

    QJsonArray questionsAndChangeAnswers() const;
public slots:
    void changeRequiredQuestion(int index, QVariant new_ques);
    void setupSecQuesAnswer(int index, const QString &id, const QString &answer);
    bool createUserCustomizedQuestion(const QString &question);
    void resetSecurityQuestionUpdate();
    void clearSecurityQuestionUpdateAuthorization();
private:
    QWalletServicesTagPtr servicesTagPtr() const;
    QWalletPtr securityQuestionWallet() const;
    bool isSecurityQuestionUpdateWalletValid() const;
    bool isSecurityQuestionWalletUsable(const QWalletPtr &wallet) const;
    bool canUpdateSecurityQuestionViaInheritance(const QWalletPtr &wallet) const;
    bool canUpdateSecurityQuestionVia2FA(const QWalletPtr &wallet) const;
    bool validateSecurityQuestionDraft(const QString &customQuestion, QString &errorMessage) const;
    bool buildSecurityQuestionUpdateBody(QJsonObject &body, QString &errorMessage) const;
    bool CreateAllSecurityQuestionsGuarded(const std::function<bool()> &canCommit);
    void setSecurityQuestionUpdateInProgress(bool inProgress);
    void setSecurityQuestionCustomDraft(const QString &draft);
    void clearSecurityQuestionUpdateContext(bool clearQuestions);
signals:
    void tapsignersChanged();
    void backupPasswordErrorAlert(const QString& errormsg);
    void keyRecoveryPendingApproval();
    void requireQuestionsChanged();
    void securityQuestionUpdateInProgressChanged();
    void securityQuestionCustomDraftChanged();
    void securityQuestionUpdateStartFailed();
private:
    QJsonObject m_signer {};
    QJsonArray  m_tapsigners {};
    QByteArray m_base64bin {};
    QJsonObject m_resultRecovery {};
    QJsonArray m_requireQuestions {};
    QJsonArray m_allQuestion {};
    QJsonArray m_securityQuestionInitialQuestions {};
    QString m_securityQuestionWalletId {};
    QJsonObject m_securityQuestionUpdateBody {};
    QString m_securityQuestionCustomDraft {};
    QHash<QString, QString> m_securityQuestionCustomQuestionIds {};
    bool m_securityQuestionUpdateInProgress {false};
    quint64 m_securityQuestionUpdateGeneration {0};
};

#endif // QKEYRECOVERY_H
