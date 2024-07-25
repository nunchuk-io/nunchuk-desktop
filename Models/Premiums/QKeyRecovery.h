#ifndef QKEYRECOVERY_H
#define QKEYRECOVERY_H

#include <QObject>
#include <QJsonArray>
#include <QJsonObject>
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
    void createUserCustomizedQuestion(const QString &question);
private:
    QWalletServicesTagPtr servicesTagPtr() const;
signals:
    void tapsignersChanged();
    void backupPasswordErrorAlert(const QString& errormsg);
    void keyRecoveryPendingApproval();
    void requireQuestionsChanged();
private:
    QJsonObject m_signer {};
    QJsonArray  m_tapsigners {};
    QByteArray m_base64bin {};
    QJsonObject m_resultRecovery {};
    QJsonArray m_requireQuestions {};
    QJsonArray m_allQuestion {};
};

#endif // QKEYRECOVERY_H
