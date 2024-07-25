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

#ifndef DRACO_H
#define DRACO_H

#include <QNetworkAccessManager>
#include <QObject>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include "Chats/ClientController.h"
#include "DracoDefines.h"
#include "QRest.h"

class Draco : public QRest
{
    Q_OBJECT
    Q_ENUMS(RESPONSE_CODE)
    Q_PROPERTY(int expireSec            READ expireSec              WRITE setExpireSec              NOTIFY expireSecChanged)
    Q_PROPERTY(QString dracoToken       READ dracoToken             WRITE setDracoToken             NOTIFY dracoTokenChanged)
    Q_PROPERTY(QString chatId           READ chatId                 WRITE setChatId                 NOTIFY chatIdChanged)
    Q_PROPERTY(QString Uid              READ Uid                    WRITE setUid                    NOTIFY uidChanged)
    Q_PROPERTY(QString emailRequested   READ emailRequested         WRITE setEmailRequested         NOTIFY emailRequestedChanged)
    Q_PROPERTY(bool stayLoggedIn        READ stayLoggedIn           WRITE setStayLoggedIn           NOTIFY stayLoggedInChanged)
public:
    static Draco *instance();
    Draco(Draco &other) = delete;
    Draco(Draco const &other) = delete;
    void operator=(const Draco &other) = delete;
    void connectDragonServer(QUrl url);
    Q_INVOKABLE void refreshDevices();
    Q_INVOKABLE void refreshContacts();
    Q_INVOKABLE void createAccount(const QString& name, const QString& email);
    Q_INVOKABLE void singin(const QString &email, const QString &password);
    Q_INVOKABLE bool signout();
    void exchangeRates(const QString &currency);
    Q_INVOKABLE void feeRates();
    Q_INVOKABLE void verifyNewDevice(const QString &pin);
    Q_INVOKABLE void resendVerifyNewDeviceCode();
    Q_INVOKABLE void inviteFriends(const QStringList& emails);
    Q_INVOKABLE void removeContact(const QString &contact_id);
    Q_INVOKABLE void cancelFriendRequest(const QString &contact_id);
    Q_INVOKABLE void accecptFriendRequest(const QString& id);
    Q_INVOKABLE void ignoreFriendRequest(const QString& id);
    Q_INVOKABLE bool deleteCurrentUser();
    Q_INVOKABLE bool sendDeleteConfirmation(const QString &code);
    Q_INVOKABLE void setUserProfile(const QString& name, const QString& avartaPath);
    Q_INVOKABLE void signoutDeice(const QString &device_id);
    Q_INVOKABLE void signoutAllDeices();
    Q_INVOKABLE void markAsCompromised(const QString &device_id);
    Q_INVOKABLE bool pkey_signup(const QString &address,const QString &username,const QString &signature);
    Q_INVOKABLE bool pkey_signin(const QString &address,const QString &username,const QString &signature);
    Q_INVOKABLE bool pkey_username_availability(const QString &username);
    Q_INVOKABLE void changePassword(const QString &oldpassword, const QString &newpassword);
    Q_INVOKABLE QVariant requestFriends(const QVariant emails);

    void checkForUpdate();
    QString get_pkey_nonce(const QString &address,const QString &username);
    QString pkey_manual_nonce(const QString &address, const QString &username, const QString &nonce, const QString &type = "none");
    bool pkey_delete_confirmation(const QString &signed_message);
    bool pkey_change_pkey(const QString &new_key, const QString &old_signed_message, const QString &new_signed_message);
    void recoverPassword(const QString& email, const QString& forgotToken, const QString& newpassword);
    void forgotPassword(const QString &email);
    void getMe();
    void getMepKey(const QString &public_address);
    //Frriend/Contacts
    DracoUser search(const QString& email);
    bool requestSingleFriend(const QString& email);
    bool requestMutipleFriend(const QStringList& email, QStringList &failed_emails);
    QList<DracoUser> getContacts();
    QList<DracoUser> getContactsSent();
    QList<DracoUser> getContactsReceived();
    QList<DracoUser> getRoomMembers(const QString& id);
    QList<DracoDevice> getDevices();
    QString Uid() const;
    void setUid(const QString &Uid);
    QString Pid() const;
    void setPid(const QString &pid);
    QString emailRequested() const;
    void setEmailRequested(const QString &value);
    int expireSec() const;
    void setExpireSec(int value);
    QString chatId() const;
    void setChatId(const QString &value);
    void reset();

    QString loginHalfToken() const;
    void setLoginHalfToken(const QString &value);
    QString deviceId() const;
    void setDeviceId(const QString &value);
    bool stayLoggedIn() const;
    void setStayLoggedIn(bool value);

    // USER_SUBSCRIPTION
    void getUserSubscriptions();
    bool getUserSubscriptionsTestnet();

    // ASSISTED_WALLETS
    bool getAssistedWallets(QJsonObject &output, QString &errormsg);
    bool assistedWalletCreateTx(const QString &wallet_id, const QString &psbt, const QString &memo);
    bool assistedWalletUpdateTx(const QString &wallet_id, const QString &txid, const QString &memo);
    QJsonObject assistedWalletSignTx(const QString &wallet_id, const QString &transaction_id, const QString &psbt, const QString &memo);
    bool assistedWalletCancelTx(const QString &wallet_id, const QString &transaction_id);
    QJsonObject assistedWalletGetTx(const QString &wallet_id, const QString &transaction_id);
    QJsonObject assistedWalletGetListTx(const QString &wallet_id);
    QJsonObject assistedWalletDeleteListTx(const QString &wallet_id, const int offset, const int limit);
    void assistedSyncTx(const QString &wallet_id, const QString &transaction_id, const QString &psbt, const QString &note);
    bool assistedRbfTx(const QString &wallet_id, const QString &transaction_id, const QString &psbt, QJsonObject &output, QString &errormsg);
    bool assistedWalletGetListKey(QJsonObject &output, QString &errormsg);
    bool assistedWalletAddKey(const QString &request_id, const QJsonObject& data, bool &isDuplicateKey, QString &errormsg);
    bool assistedWalletRemoveId(const QString &request_id);
    QJsonObject assistedGetWalletConfig();
    bool assistedWalletGetInfo(const QString &wallet_id, QJsonObject &output, QString &errormsg);
    bool assistedWalletUpdate(const QString &wallet_id, const QString &name, const QString &description, QJsonObject &output, QString &errormsg);
    bool assistedKeyUpdateName(const QString &fingerPrint, const QString &name, const QJsonObject& json = {});

    bool DeleteAssistedWallet(const QString& wallet_id, const QStringList &signatures, const QString& passwordToken, const QString& secQuesToken, QJsonObject &output, QString &errormsg);
    bool DeleteWalletRequiredSignatures(const QString &wallet_id, QJsonObject &output, QString &errormsg);

    bool verifyPasswordToken(const QString &password, const int action, QString &string_token, QString &errormsg);
    QString randomNonce();

    bool secQuesGet(QJsonObject& output, QString& errormsg);
    bool SecQuesUpdate(const QJsonObject& request_body,
                       const QStringList &signatures,
                       const QString& passwordToken,
                       const QString& secQuesToken,
                       const QString& confirmToken,
                       bool isDraft,
                       QJsonObject &output,
                       QString &errormsg);

    bool UpdateSecQuesRequiredSignatures(const QJsonObject& request_body,
                                         QJsonObject &output,
                                         QString &errormsg);

    bool secQuesAnswer(const QJsonArray& answers,
                       QString& secQuesToken,
                       int& correct_answer,
                       QString& errormsg);

    bool lockdownPeriods(QJsonArray& output, QString& errormsg);

    bool lockdownRequiredSignatures(const QString &period_id,
                                    const QString &wallet_id,
                                    QJsonObject &output,
                                    QString& errormsg);
    bool lockdownByAnswerSecQues(const QString& passwordToken,
                                 const QString& secQuesToken,
                                 const QString& period_id,
                                 const QString& wallet_id,
                                 QString& until_time,
                                 QString& errormsg);
    bool lockdownBySignDummyTx(const QStringList &signatures,
                               const QString& passwordToken,
                               const QString& period_id,
                               const QString& wallet_id,
                               QString& until_time,
                               QString& errormsg);

    bool UserKeysMarkAKeyAsVerified(const QString& xfpOrId,
                                    const QString& key_checksum,
                                    const QString& verification_type,
                                    QJsonObject& output,
                                    QString& errormsg);
    bool UserKeysRequestRecoveryKey(const QString& xfpOrId,
                                    const QStringList &signatures,
                                    const QString& passwordToken,
                                    const QString& secQuesToken,
                                    const QString& confirmToken,
                                    const QJsonObject &body,
                                    QJsonObject &output,
                                    QString &errormsg);
    bool UserKeysRecoveryKey(const QString& xfpOrId,
                             const QStringList &signatures,
                             const QString& passwordToken,
                             const QString& secQuesToken,
                             QJsonObject &output,
                             QString &errormsg);
    bool UserKeysMarkRecoverStatus(const QString& xfpOrId,
                                   QJsonObject &output,
                                   QString &errormsg);
    bool UserKeysCalculateRequiredSignatures(const QString& xfpOrId,
                                             QJsonObject &output,
                                             QString &errormsg);
    bool UserKeysUploadBackup();
    bool UserKeysGetListUserKey(const QString& passwordToken,
                                QJsonArray &output,
                                QString &errormsg);

    bool UserKeysGetUserKey(const QString& xfpOrId,
                            const QString& passwordToken,
                            QJsonObject &output,
                            QString &errormsg);

    bool UserKeysCreateUserCustomizedQuestion(const QString& question,
                            QJsonObject &output,
                            QString &errormsg);

    bool UserKeysDownloadBackup(const QString& verify_token,
                                const QString& xfpOrId,
                                const QJsonArray& answers,
                                QString& output,
                                QString& errormsg);

    bool getCurrencies(QJsonObject& output, QString& errormsg);
    void btcRates();

    bool inheritanceDownloadBackup(const QString& magic,
                                   const QStringList &backup_passwords,
                                   int& response_code,
                                   QJsonObject &output,
                                   QString& errormsg);

    bool inheritanceClaimRequest(const QString& magic,
                                 const QString& psbt,
                                 QJsonObject& output,
                                 QString& errormsg);

    bool inheritanceClaimStatus(const QJsonObject& data,
                                const QStringList& authos,
                                QJsonObject& output,
                                QString& errormsg);

    bool inheritanceCreateTx(const QJsonObject& data,
                             const QStringList& authos,
                             QJsonObject& output,
                             QString& errormsg);

    bool inheritanceCheck(const QString& magic,
                          const QString& environment,
                          QJsonObject& output,
                          QString& errormsg);

    bool inheritanceGetPlan(const QString &wallet_id,
                            const QString &group_id,
                            QJsonObject& output,
                            QString& errormsg);

    bool inheritancePlanCreate(const QStringList &signatures,
                               const QJsonObject data,
                               bool isDraft,
                               QJsonObject &output,
                               QString &errormsg);

    bool inheritancePlanUpdate(const QString &passwordToken,
                               const QString &secQuesToken,
                               const QStringList &signatures,
                               const QJsonObject data,
                               bool isDraft,
                               QJsonObject &output,
                               QString &errormsg);

    bool inheritancePlanCancel(const QString &passwordToken,
                               const QString &secQuesToken,
                               const QStringList &signatures,
                               const QJsonObject data,
                               bool isDraft,
                               QJsonObject &output,
                               QString &errormsg);

    bool inheritanceFakeUpdate();

    bool inheritancePlanRequiredSignatures(const QJsonObject& data, QJsonObject &output, QString& errormsg);

    bool inheritancePlanBufferPeriod(QJsonArray& output, QString& errormsg);

    bool InheritancePlanningRequestApprove(const QString &request_id,
                                           const QString &wallet_id,
                                           const QString &group_id,
                                           QString& errormsg);

    bool InheritancePlanningRequestDeny(const QString &request_id,
                                        const QString &wallet_id,
                                        const QString &group_id,
                                        QString& errormsg);

    bool ServerKeysGet(const QString& id_or_xfp, const QString &derivation_path, QJsonObject& output, QString& errormsg);

    bool ServerKeysUpdate(const QString& passwordToken,
                          const QString& secQuesToken,
                          const QString& id_or_xfp,
                          const QString& derivation_path,
                          const QStringList& signatures,
                          const QJsonObject& request_body,
                          QJsonObject& output,
                          QString& errormsg);

    bool ServerKeysRequiredSignature(const QString &key_id_or_xfp,
                                     const QString &derivation_path,
                                     const QJsonObject& request_body,
                                     QJsonObject& output, QString &errormsg);

    bool RequestConfirmationCode(const QString &action,
                                 const QJsonObject &requestBody,
                                 QJsonObject &output,
                                 QString &errormsg);
    bool VerifyConfirmationCode(const QString &code_id,
                                const QString &code,
                                QJsonObject &output,
                                QString &errormsg);

    bool GetAlerts(const QString wallet_id, QJsonObject &output, QString &errormsg);
    bool GetAlertsCount(const QString wallet_id, QJsonObject &output, QString &errormsg);
    bool DismissAlert(const QString wallet_id, const QString alert_id, QJsonObject &output, QString &errormsg);
    bool MarkAlertAsRead(const QString wallet_id, const QString alert_id, QJsonObject &output, QString &errormsg);

    bool HealthCheckForKey(const QString &wallet_id, const QString &xfp, QJsonObject &output, QString &errormsg);
    bool GetWalletHealthStatus(const QString &wallet_id, QJsonObject &output, QString &errormsg);

    // Dummy transaction
    bool GetDummyTransaction(const QString &wallet_id, const QString &txid, QJsonObject& output, QString &errormsg);
    bool UpdateDummyTransaction(const QString &wallet_id, const QStringList& authorizations, const QString &txid, QJsonObject& output, QString &errormsg);
    bool CancelDummyTransaction(const QString &wallet_id, const QString &txid, QJsonObject& output, QString &errormsg);
    bool FinalizeDummyTransaction(const QString &wallet_id, const QString &txid, QJsonObject& output, QString &errormsg);

    bool GetHistorySignerList(const QString &xfp, QJsonObject& output, QString &errormsg);

    bool GetCountryCodeList(QJsonObject &output, QString &errormsg);
    bool RequestOnboardingNoAdvisor(const QString &country_code, const QString &email, const QString &note, QString &errormsg);

    bool GetElectrumServers(QJsonObject &output, QString &errormsg);
    bool ChangeEmail(const QJsonObject& request_body,
                     const QStringList &signatures,
                     const QString& passwordToken,
                     const QString& secQuesToken,
                     const QString& confirmToken,
                     bool isDraft,
                     QJsonObject &output,
                     QString &errormsg);
    bool CalculateRequireSignaturesForChangingEmail(const QString &new_email, QJsonObject& output, QString &errormsg);

    bool GetKeyHealthReminder(const QString &wallet_id,
                              QJsonObject& output,
                              QString &errormsg);

    bool AddOrUpdateKeyHealthReminder(const QString &wallet_id,
                                      const QJsonObject& request_body,
                                      QJsonObject& output,
                                      QString &errormsg);

    bool DeleteKeyHealthReminder(const QString &wallet_id,
                                 const QStringList &xfps,
                                 QJsonObject& output,
                                 QString &errormsg);

    bool SkipKeyHealthReminder(const QString &wallet_id,
                                 const QString &xfp,
                                 QJsonObject& output,
                                 QString &errormsg);

    bool ConfigureWalletReplacement(const QString &wallet_id,
                                    QJsonObject& output,
                                    QString &errormsg);

    bool InitiateKeyReplacement(const QString &wallet_id,
                                const QString &xfp,
                                QJsonObject& output,
                                QString &errormsg);

    bool CancelKeyReplacement(const QString &wallet_id,
                              const QString &xfp,
                              QJsonObject& output,
                              QString &errormsg);

    bool ReplaceKey(const QString &wallet_id,
                    const QString &xfp,
                    const QString& passwordToken,
                    const QJsonObject& request_body,
                    QString &errormsg);

    bool FinalizeKeyReplacement(const QString &wallet_id,
                                QJsonObject& output,
                                QString &errormsg);

    bool GetKeyReplacementStatus(const QString &wallet_id,
                                QJsonObject& output,
                                QString &errormsg);

    bool ResetKeyReplacement(const QString &wallet_id,
                            QJsonObject& output,
                            QString &errormsg);

    //Saved address
    bool GetSavedAddress(QJsonObject &output, QString &errormsg);
    bool AddOrUpdateSavedAddress(const QString &label, const QString &address, QJsonObject &output, QString &errormsg);
    bool DeleteSavedAddress(const QString &label, const QString &address, QJsonObject &output, QString &errormsg);

    bool SignDummyTransactionToSignIn(const QString &dummy_transaction_id, const QStringList& signatures, QJsonObject &output, QString &errormsg);
    bool SignInUsingXPUBorWallet(const QString &bsms, QJsonObject &output, QString &errormsg);

private:
    Draco();
    ~Draco();
    static Draco* m_instance;
    QString m_uid;
    QString m_pid;
    QString m_emailRequested;
    int m_expireSec;
    QString m_ChatId;
    QString m_loginHalfToken;
    QString m_deviceId;
    bool m_stayLoggedIn;

signals:
    void uidChanged();
    void pidChanged();
    void emailRequestedChanged();
    void expireSecChanged();
    void dracoTokenChanged();
    void chatIdChanged();
    void deviceIdChanged();
    void loginHalfTokenChanged();
    void createAccountResult(int https_code, int error_code, QString error_msg);
    void singinResult(int https_code, int error_code, QString error_msg);
    void recoverPasswordResult(int https_code, int error_code, QString error_msg);
    void forgotPasswordResult(int https_code, int error_code, QString error_msg);
    void changePasswordResult(int https_code, int error_code, QString error_msg);
    void verifyNewDeviceResult(int https_code, int error_code, QString error_msg);
    void resendVerifyNewDeviceCodeResult(int https_code, int error_code, QString error_msg);
    void loggedInDeviceChanged(int https_code, int error_code, QString error_msg);
    void updateProfileResult(int https_code, int error_code, QString error_msg);
    void startCheckForUpdate(int result, const QString &title,const QString &message,const QString &doItLaterCTALbl);
    void signalpkey_signup(int https_code, int error_code, QString error_msg);
    void signalpkey_signin(int https_code, int error_code, QString error_msg);
    void stayLoggedInChanged();
};

#endif // DRACO_H
