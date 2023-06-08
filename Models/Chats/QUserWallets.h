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

#ifndef QUSERWALLETS_H
#define QUSERWALLETS_H

#include <QObject>
#include <QJsonArray>
#include "TransactionModel.h"
#include "Draco.h"

class QUserWallets : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList securityQuestions READ securityQuestions NOTIFY securityQuestionChanged)
    Q_PROPERTY(QVariantList lockdownPeriods READ lockdownPeriods CONSTANT)
    Q_PROPERTY(QString untilTime READ untilTime WRITE setUntilTime NOTIFY untilTimeChanged)
    Q_PROPERTY(QVariantList tapsigners READ tapsigners NOTIFY tapsignersChanged)
    Q_PROPERTY(QMasterSigner* signer READ signer CONSTANT)
public:
    struct inheritance_t
    {
        QString m_destinationAddress = {};
        QString magic = {};
        QString note = {};
        QString masterSignerId = {};
        nunchuk::Wallet wallet = {};
        nunchuk::Transaction tx = {};
        double balance = {0.0};
    };
    QUserWallets();
    ~QUserWallets();
    static QUserWallets *instance();

    QString secQuesToken() const;
    void setSecQuesToken(const QString &secQuesToken);

    QString passwordToken() const;
    bool requestVerifyPassword(const QString& password, const int action);
    bool requestLockDownVerifyPassword(const QString &password);
    bool requestRecoverKeyVerifyPassword(const QString &password);
    bool requestServerKeyVerifyPassword(const QString &password);

    QTransactionPtr getDummyTx(const QString &wallet_id, const QString &period_id);
    void signDummyTx(const QString& xfp);
    void createDummyTx();
    bool checkDummyTx();

    QVariantList securityQuestions();
    bool createSecurityQuestions();
    Q_INVOKABLE void secQuesAnswer(const QString &id, const QString &answer);
    bool secQuesAnswer();

    QVariantList lockdownPeriods();
    bool createLockdownPeriods();
    bool lockdownRequired(const QString &period_id);
    int  lockdownType();
    bool lockdownByAnswerSecQues();
    bool lockdownBySignDummyTx();

    bool downloadBackup();
    nunchuk::Wallet getWallet() const;

    QString untilTime() const;
    void setUntilTime(QString untilTime);

    Q_INVOKABLE void createTapsigners();
    QVariantList tapsigners() const;
    void keyRecovery(const QString& fingerPrint);
    bool startRecovery(const QString& backup_password);

    QMasterSigner *signer() const;
    //Inheritance
    Q_INVOKABLE int inheritanceCheck(const QString& magic = "", const QString& environment = "PRODUCTION");
    bool inheritanceGetPlan(const QString &magic_inpputed, const QString& wallet_id);
    int inheritanceDownloadBackup(const QString& magic, const QString& backup_password);
    bool inheritanceClaimRequest(const nunchuk::Wallet wallet, const nunchuk::Transaction txSigned, const QString& magic);
    int inheritanceClaimStatus(const QJsonObject& data, const QString& autho);
    bool inheritanceCreateTx(const nunchuk::SingleSigner& signer, const QJsonObject& data, const QString& autho);
    void setInheritanceAddress(const QString& to_wallet_id);
    void setInheritanceAddressNewTransaction(const QString& address);
    void inheritanceCreateDraftTransaction(double fee_rate = 1000.0);
    void inheritanceSignTransaction();

    // For co-signing policies
    bool serverKeyGetCurrentPolicies(const QString& wallet_id);
    QJsonObject serverKeyBody();
    bool serverKeyUpdatePolicies();
    bool serverKeyUpdatePoliciesSucceed();
    inheritance_t inheritance() const;

signals:
    void verifyPasswordTokenAlert(const QString& errormsg);
    void lockdownPeriodsAlert(const QString& errormsg);
    void untilTimeChanged();
    void answerErrorAlert(const QString& errormsg);
    void backupPasswordErrorAlert(const QString& errormsg);
    void tapsignersChanged();
    void notPaidAlert();
    void isExpiredAlert();
    void hasNotBeenActivatedYetAlert();
    void serverKeyVerifyPasswordAlert();
    void securityQuestionChanged();
    void serverKeyDummyTransactionAlert();
    void securityQuestionClosed();
    void thereNoAssistedWalletAlert();
private:
    QString m_passwordToken;
    QString m_secQuesToken;
    ReqiredSignaturesInfo required_question {};
    QString m_period_id {};
    QList<SecurityQuestion> m_quesAnswers;
    QString m_wallet_id {};
    nunchuk::Wallet m_wallet;
    QTransactionPtr m_dummyTx;
    QVariantList m_questions;
    QVariantList m_periods;
    QMap<QString,QString> m_signatures;
    QString m_untilTime;
    QVariantList m_tapsigners;
    QMasterSignerPtr m_signer {NULL};
    QByteArray m_base64bin;
    inheritance_t mInheritance = {};
    struct co_signing_t
    {
        QString key_id_or_xfp  = {};
        QString wallet_id = {};
        nunchuk::SingleSigner m_server_key = {};
    };
    co_signing_t mCoSigning = {};
};

#endif // QUSERWALLETS_H
