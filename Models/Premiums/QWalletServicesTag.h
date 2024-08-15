/**************************************************************************
 * This file is part of the Nunchuk software (https://nunchuk.io/)        *
 * Copyright (C) 2020-2022 Enigmo								          *
 * Copyright (C) 2022 Nunchuk								              *
 *                                                                        *
 * This program is free software; you can redistribute it and/or          *
 * modify it under the terms of the GNU General Public License            *
 * as published by the Free Software Foundation; either version 3         *
 * of the License, or (at your option) any later version.                 *
 *                                                                        *
 * This program is distributed in the hope that it will be useful,        *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 * GNU General Public License for more details.                           *
 *                                                                        *
 * You should have received a copy of the GNU General Public License      *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
 *                                                                        *
 **************************************************************************/

#ifndef QWALLETINHERITANCE_H
#define QWALLETINHERITANCE_H

#include <QObject>
#include <QJsonArray>
#include "TransactionModel.h"
#include "ServiceSetting.h"
#include "QSwitchAPI.h"
#include "WalletModel.h"

class InheritanceEnum : public QObject
{
    Q_OBJECT
public:
    Q_ENUMS(CIWithDraw)
    enum class CIWithDraw {
        WITHDRAW_TO_NUNCHUK_WALLET,
        WITHDRAW_TO_ADDRESS
    };

    Q_ENUMS(InheritanceEdit)
    enum class InheritanceEdit {
        IE_NONE,
        IE_ACTIVATION_DATE,
        IE_LEAVE_MESSAGE,
        IE_BUFFER_PERIOD,
        IE_NOTIFICATION
    };
};
//QWalletServicesTag
class QWalletServicesTag : public QSwitchAPI
{
    Q_OBJECT
    Q_PROPERTY(QVariantList securityQuestions   READ securityQuestions      NOTIFY securityQuestionChanged)
    Q_PROPERTY(QVariantList periods     READ periods        CONSTANT)
    Q_PROPERTY(QString untilTime                READ untilTime              NOTIFY untilTimeChanged)

    Q_PROPERTY(QVariant inheritanceInfo         READ inheritanceInfo        NOTIFY inheritanceInfoChanged)
    Q_PROPERTY(QStringList listInheritantPlans  READ listInheritantPlans    NOTIFY listInheritantPlansChanged)
    Q_PROPERTY(QStringList listLockdown         READ listLockdown           NOTIFY listGroupWChanged)
    Q_PROPERTY(QStringList listLocked           READ listLocked             NOTIFY listLockedChanged)
    Q_PROPERTY(QStringList listPolicy           READ listPolicy             NOTIFY listPolicyChanged)

    // For Claim Inheritance
    Q_PROPERTY(QVariant inheritanceCheckStatus  READ inheritanceCheckStatus NOTIFY inheritanceCheckStatusChanged)
    Q_PROPERTY(QVariant inheritanceClaimPlan    READ inheritanceClaimPlan   NOTIFY inheritanceClaimPlanChanged)
    Q_PROPERTY(QVariant walletConfig            READ walletConfig           NOTIFY walletConfigChanged)
    Q_PROPERTY(QVariant reqiredSignatures       READ reqiredSignatures      NOTIFY reqiredSignaturesChanged)

    Q_PROPERTY(QVariant keyRecovery             READ keyRecovery        CONSTANT)

public:
    QWalletServicesTag(int mode);
    virtual ~QWalletServicesTag();
    struct inheritance_t
    {
        QString m_destinationAddress = {};
        nunchuk::Wallet wallet = {};
        nunchuk::Transaction tx = {};
    };
    QString passwordToken() const;
    bool requestVerifyPassword(const QString& password, const int action);
    bool requestLockDownVerifyPassword(const QString &password);
    bool requestRecoverKeyVerifyPassword(const QString &password);
    bool requestUpdateSecurityQuestionPassword(const QString &password);
    bool requestServerKeyVerifyPassword(const QString &password);
    bool requestInheritancePlanVerifyPassword(const QString &password);
    bool requestDeleteWalletVerifyPassword(const QString &password);
    bool requestChangeEmailVerifyPassword(const QString &password);
    bool requestReplaceKeysVerifyPassword(const QString &password);
    bool requestEditMemberVerifyPassword(const QString &password);

    Q_INVOKABLE bool verifyConfirmationCode(const QString &code);
    QVariantList securityQuestions();
    void setQuestions(const QJsonArray &questions);
    bool CreateSecurityQuestionsAnswered();
    Q_INVOKABLE void secQuesAnswer(const QString &id, const QString &answer);
    bool secQuesAnswer();
    QJsonArray questionsAndAnswers() const;

    //Emergency Lockdown
    bool RequestConfirmationCodeEmergencyLockdown();
    QVariantList periods();
    bool createLockdownPeriods();
    bool lockdownRequired(const QString &period_id);
    bool lockdownByAnswerSecQues();
    bool lockdownByConfirmationCode();
    bool lockdownBySignDummyTx();

    nunchuk::Wallet getWallet() const;

    QString untilTime() const;
    void setUntilTime(QString untilTime);

    //Inheritance
    Q_INVOKABLE void clearClaimAnInheritance();
    Q_INVOKABLE bool inheritanceCheck(const QString& magic = "", const QString& environment = "PRODUCTION");
    int inheritanceDownloadBackup(const QString& magic, const QString& backup_password, const QString& backup_password_two);
    bool inheritanceClaimRequest(const nunchuk::Wallet wallet, const nunchuk::Transaction txSigned, const QString& magic);
    bool inheritanceClaimStatus(const QString& magic);
    bool inheritanceCreateTx(const QJsonObject& data, const QStringList& authos);
    void setInheritanceAddress(const QString& to_wallet_id);
    void setInheritanceAddressNewTransaction(const QString& address);
    bool inheritanceCreateDraftTransaction(double fee_rate = 1000.0);
    Q_INVOKABLE bool inheritanceSignTransaction();

    // Get additional wallet
    Q_INVOKABLE void additionalGetWalletConfig();
    QVariant inheritanceInfo() const;

    QStringList listInheritantPlans() const;
    void setListInheritantPlans();

    QStringList listLockdown() const;
    void setListLockdown();

    QStringList listLocked() const;
    void setListLocked();

    QStringList listPolicy() const;
    void setListPolicy();

    QStringList list2FA() const;
    void setList2FA();

    QString secQuesToken() const;
    void clearToken();

    QVariant inheritanceCheckStatus() const;
    void setInheritanceCheckStatus(const QJsonObject& status);

    QVariant inheritanceClaimPlan() const;
    void setInheritanceClaimPlan(const QJsonObject& claim);

    QVariant walletConfig() const;
    void setWalletConfig(const QJsonObject& config);

    QVariant reqiredSignatures() const;
    QJsonObject reqiredSignaturesJs() const;
    void setReqiredSignatures(const QJsonObject& reqired);
    ReqiredSignaturesInfo reqiredSignaturesInfo() const;

    QKeyRecoveryPtr keyRecoveryPtr() const;
    QVariant keyRecovery() const;

    QString code_id() const;
    void setCode_id(const QString &code_id);

    QString confirmToken() const;

    void ConfigServiceTag();

    QJsonObject confirmCodeNonceBody() const;
    void setConfirmCodeNonceBody(const QJsonObject& nonceBody);

    // For change Email
    bool RequestConfirmationChangeEmail(const QString &new_email);
public slots:
    void clearBufferPeriodCountdown();
    void clearInheritance();

signals:
    void verifyPasswordTokenAlert(const QString& errormsg);
    void lockdownPeriodsAlert(const QString& errormsg);
    void untilTimeChanged();
    void answerErrorAlert(const QString& errormsg);
    void notPaidAlert();
    void isExpiredAlert();
    void hasNotBeenActivatedYetAlert();
    void serverKeyVerifyPasswordAlert();
    void securityQuestionChanged();
    void thereNoAssistedWalletAlert();
    void inheritanceDiscardChangeAlert();
    void inheritanceInfoChanged();
    void listInheritantPlansChanged();
    void listGroupWChanged();
    void listLockedChanged();
    void listPolicyChanged();
    void inheritanceCheckStatusChanged();
    void inheritanceClaimPlanChanged();
    void confirmCodeVerified();
    void walletConfigChanged();
    void reqiredSignaturesChanged();

private:
    QString m_passwordToken {""};
    QString m_secQuesToken {""};
    QString m_confirmToken {""};
    QJsonObject m_confirmCodeRequestBody {};
    QString m_period_id {};
    QList<SecurityQuestion> m_quesAnswers;
    QJsonObject m_inheritanceCheckStatus;
    QJsonObject m_inheritanceClaimPlan;
    QJsonObject m_walletConfig;

    QJsonObject m_reqiredSignatures;
    QJsonArray m_questions;

    QKeyRecoveryPtr m_keyRecovery;

    nunchuk::Wallet m_wallet;
    QJsonArray m_periods;
    QMap<QString,QString> m_signatures;
    QString m_untilTime;

    inheritance_t mInheritance = {};
    QStringList m_inheritantPlans;
    QStringList m_listLockdown;
    QStringList m_listLocked;
    QStringList m_listPolicy;
    QStringList m_list2FA;
    QString m_code_id {};

    std::vector<nunchuk::SingleSigner> single_signers;
    std::vector<nunchuk::MasterSigner> master_signers;
};

#endif // QWALLETINHERITANCE_H
