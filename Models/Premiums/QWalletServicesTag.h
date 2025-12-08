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
#include <QSharedPointer>
#include "TransactionModel.h"
#include "ServiceSetting.h"
#include "WalletModel.h"
#include "Commons/QStateFlow.h"
#include "Premiums/QKeyRecovery.h"
#include "Premiums/QServerKey.h"
#include "Premiums/QInheritanceClaiming.h"
#include "ifaces/Servers/DracoDefines.h"

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

class QWalletServicesTag : public QInheritanceClaiming
{
    Q_OBJECT
    Q_PROPERTY(QVariantList securityQuestions   READ securityQuestions      NOTIFY securityQuestionChanged)
    Q_PROPERTY(QVariantList periods     READ periods        CONSTANT)
    Q_PROPERTY(QString untilTime                READ untilTime              NOTIFY untilTimeChanged)
    // Keep non-claim list properties in this class
    Q_PROPERTY(QStringList listInheritantPlans  READ listInheritantPlans    NOTIFY listInheritantPlansChanged)
    Q_PROPERTY(QStringList listLockdown         READ listLockdown           NOTIFY listGroupWChanged)
    Q_PROPERTY(QStringList listLocked           READ listLocked             NOTIFY listLockedChanged)
    Q_PROPERTY(QStringList listPolicy           READ listPolicy             NOTIFY listPolicyChanged)

    Q_PROPERTY(QVariant keyRecovery             READ keyRecovery        CONSTANT)
    Q_PROPERTY(QVariant setupConfig             READ setupConfig            NOTIFY setupConfigChanged)

public:
    static QWalletServicesTagPtr instance();
    QWalletServicesTag();
    ~QWalletServicesTag();

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

    // Emergency Lockdown
    bool RequestConfirmationCodeEmergencyLockdown();
    QVariantList periods();
    bool createLockdownPeriods();
    bool lockdownRequired(const QString &period_id);
    bool lockdownByAnswerSecQues();
    bool lockdownByConfirmationCode();
    bool lockdownBySignDummyTx();

    QString untilTime() const;
    void setUntilTime(QString untilTime);

    // Additional wallet config
    Q_INVOKABLE void additionalGetWalletConfig();
    Q_INVOKABLE void configWalletSetup();

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

    QVariant setupConfig() const;
    QJsonObject setupConfigJs();
    void setSetupConfig(const QJsonObject& config);

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

signals:
    void lockdownPeriodsAlert(const QString& errormsg);
    void untilTimeChanged();
    void answerErrorAlert(const QString& errormsg);
    // Inheritance claim signals come from base class; no re-declaration here
    void serverKeyVerifyPasswordAlert();
    void securityQuestionChanged();
    void thereNoAssistedWalletAlert();
    void inheritanceDiscardChangeAlert();
    void listInheritantPlansChanged();
    void listGroupWChanged();
    void listLockedChanged();
    void listPolicyChanged();
    void confirmCodeVerified();
    void walletConfigChanged();
    void reqiredSignaturesChanged();
    void setupConfigChanged();
private:
    QString m_passwordToken {""};
    QString m_secQuesToken {""};
    QString m_confirmToken {""};
    QJsonObject m_confirmCodeRequestBody {};
    QString m_period_id {};
    QList<SecurityQuestion> m_quesAnswers;
    QJsonObject m_walletConfig;

    QJsonObject m_reqiredSignatures;
    QJsonArray m_questions;

    QKeyRecoveryPtr m_keyRecovery;

    QJsonObject m_setupConfig;

    QJsonArray m_periods;
    QMap<QString,QString> m_signatures;
    QString m_untilTime;

    QStringList m_inheritantPlans;
    QStringList m_listLockdown;
    QStringList m_listLocked;
    QStringList m_listPolicy;
    QStringList m_list2FA;
    QString m_code_id {};
};

#endif // QWALLETINHERITANCE_H
