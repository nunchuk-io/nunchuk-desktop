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

#ifndef BYZANTINE_H
#define BYZANTINE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QObject>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QQmlEngine>
#include <QJsonArray>
#include "DracoDefines.h"
#include "QRest.h"

class Byzantine : public QRest
{
    Q_OBJECT
private:
    Byzantine();
    ~Byzantine();
public:
    static Byzantine *instance();
    bool GetListGroupWallets(QJsonObject& output, QString &errormsg);

    bool GetAllGroupWallets(QJsonObject& output, QString &errormsg);
    bool GetOneGroupWallets(const QString group_id, QJsonObject& output, QString &errormsg);
    bool AcceptGroupWallet(const QString group_id, QJsonObject& output, QString &errormsg);
    bool DenyGroupWallet(const QString group_id, QJsonObject& output, QString &errormsg);
    bool ResetGroupWallet(const QString group_id, QJsonObject& output, QString &errormsg);

    bool DeleteGroupWallet(const QString& wallet_id, const QString& group_id, const QStringList &signatures, const QString& passwordToken, const QString& secQuesToken, QJsonObject &output, QString &errormsg);
    bool DeleteGroupWalletRequiredSignatures(const QString &wallet_id, const QString &group_id, QJsonObject &output, QString &errormsg);

    bool GetGroupAlerts(const QString group_id, QJsonObject& output, QString &errormsg);
    bool GetGroupAlertsCount(const QString group_id, QJsonObject& output, QString &errormsg);
    bool DismissGroupAlert(const QString group_id, const QString alert_id, QJsonObject& output, QString &errormsg);
    bool MarkGroupAlertAsRead(const QString group_id, const QString alert_id, QJsonObject& output, QString &errormsg);

    bool GetAllPermissions(QJsonObject& output, QString &errormsg);
    bool GetOnePermissions(const QString slug, QJsonObject& output, QString &errormsg);
    bool GetDefaultPermissions(QJsonObject& output, QString &errormsg);

    bool DraftWalletAddKey(const QString group_id, const QString request_id, const QJsonObject data, bool &isDuplicateKey, QString &errormsg);

    bool GetCurrentGroupWallet(const QString group_id, QJsonObject& output, QString &errormsg);
    bool GetCurrentGroupDraftWallet(const QString group_id, QJsonObject& output, QString &errormsg);
    bool GetAllListRequestAddKey(const QString &group_id, QJsonObject &output, QString &errormsg);
    bool DeleteRequestAddKey(const QString &group_id, const QString &request_id);
    bool UpdateWallet(const QString &group_id, const QString &wallet_id, const QString &name, const QString &description, QJsonObject& output, QString &errormsg);
    bool GetWalletAlias(const QString &group_id, const QString &wallet_id, QJsonObject& output, QString &errormsg);
    bool UpdateWalletAlias(const QString &group_id, const QString &wallet_id, const QString& alias, QJsonObject& output, QString &errormsg);
    bool DeleteWalletAlias(const QString &group_id, const QString &wallet_id, QJsonObject& output, QString &errormsg);
    bool UpdateWalletPrimaryOwner(const QString &group_id, const QString &wallet_id, const QString& primary_membership_id, QJsonObject& output, QString &errormsg);

    bool GetCurrentGroupChat(const QString &group_id, QJsonObject& output, QString &errormsg);
    bool UpdateCurrentGroupChat(const QString &group_id, QJsonObject& output, QString &errormsg);
    bool DeleteCurrentGroupChat(const QString &group_id, QJsonObject& output, QString &errormsg);
    bool CreateCurrentGroupChat(const QString &group_id, QJsonObject& output, QString &errormsg);
    bool GetCurrentGroupChatHistory(const QString &group_id, QJsonObject& output, QString &errormsg);

    QString nonce();
    // Health check
    bool RequestHealthCheckForKey(const QString &group_id, const QString &wallet_id, const QString &xfp, QJsonObject& output, QString &errormsg);
    bool HealthCheckForKey(const QString &group_id, const QString &wallet_id, const QString &xfp, QJsonObject& output, QString &errormsg);
    bool GetWalletHealthStatus(const QString &group_id, const QString &wallet_id, QJsonObject& output, QString &errormsg);

    // Dummy transaction
    bool GetDummyTransaction(const QString &group_id, const QString &wallet_id, const QString &txid, QJsonObject& output, QString &errormsg);
    bool UpdateDummyTransaction(const QString &group_id, const QString &wallet_id, const QStringList& authorizations, const QString &txid, QJsonObject& output, QString &errormsg);
    bool CancelDummyTransaction(const QString &group_id, const QString &wallet_id, const QString &txid, QJsonObject& output, QString &errormsg);
    bool FinalizeDummyTransaction(const QString &group_id, const QString &wallet_id, const QString &txid, QJsonObject& output, QString &errormsg);

    // Transaction operation
    bool GetAllTransaction(const QString &group_id, const QString &wallet_id, QJsonObject& output, QString &errormsg);
    bool GetAllCancelledTransaction(const QString &group_id, const QString &wallet_id, const int offset, const int limit, QJsonObject& output, QString &errormsg);
    bool GetAllTransactionNotes(const QString &group_id, const QString &wallet_id, QJsonObject& output, QString &errormsg);
    bool GetOneTransaction(const QString &group_id, const QString &wallet_id, const QString &txid, QJsonObject& output, QString &errormsg);
    bool UpdateTransaction(const QString &group_id, const QString &wallet_id, const QString &txid, const QString &note, QJsonObject& output, QString &errormsg);
    bool CancelTransaction(const QString &group_id, const QString &wallet_id, const QString &txid, QJsonObject& output, QString &errormsg);
    bool SyncTransaction(const QString &group_id, const QString &wallet_id, const QString &txid, const QString &psbt, const QString &note, QJsonObject& output, QString &errormsg);
    bool RbfTransaction(const QString &group_id, const QString &wallet_id, const QString &txid, const QString &psbt, QJsonObject& output, QString &errormsg);
    bool SignTransaction(const QString &group_id, const QString &wallet_id, const QString &txid, const QString &psbt, const QString &note, QJsonObject& output, QString &errormsg);
    bool CreateTransaction(const QString &group_id, const QString &wallet_id, const QString &psbt, const QString &note, QJsonObject& output, QString &errormsg);
    bool requestSignature(const QString &group_id, const QString &wallet_id, const QString &txid, const QString &membership_id, QJsonObject& output, QString &errormsg);

    // Server keys
    bool ServerKeysGet(const QString &group_id, const QString &key_id_or_xfp, const QString &derivation_path, QJsonObject& output, QString &errormsg);
    bool ServerKeysUpdate(const QString &group_id,
                          const QString &passwordToken,
                          const QString &secQuesToken,
                          const QString &key_id_or_xfp,
                          const QString &derivation_path,
                          const QStringList &signatures,
                          const QJsonObject& request_body,
                          bool isDraft,
                          QJsonObject &output,
                          QString &errormsg);
    bool ServerKeysRequiredSignature(const QString &group_id,
                                     const QString &key_id_or_xfp,
                                     const QString &derivation_path,
                                     const QJsonObject& request_body,
                                     QJsonObject& output,
                                     QString &errormsg);

    bool lockdownPeriods(QJsonArray& output, QString& errormsg);
    bool lockdownRequiredSignatures(const QString &group_id,
                                    const QString &period_id,
                                    const QString &wallet_id,
                                    QJsonObject &output,
                                    QString& errormsg);
    bool lockdownByAnswerSecQues(const QString &group_id,
                                 const QString &passwordToken,
                                 const QString &secQuesToken,
                                 const QString &period_id,
                                 const QString &wallet_id,
                                 QString &until_time,
                                 QString &errormsg);

    bool lockdownByConfirmationCode(const QString &passwordToken,
                                 const QString &confirmToken,
                                 const QJsonObject &requestBody,
                                 QString &until_time,
                                 QString &errormsg);

    bool lockdownBySignDummyTx(const QString &group_id,
                               const QStringList &signatures,
                               const QString &passwordToken,
                               const QString &period_id,
                               const QString &wallet_id,
                               QString &until_time,
                               QString &errormsg);

    QJsonObject assistedGetWalletConfig();

    // Group chat
    bool GetGroupChat(const QString &group_id, QJsonObject& output, QString &errormsg);
    bool UpdateGroupChat(const QString &group_id, const QString &history_period_id, QJsonObject& output, QString &errormsg);
    bool DeleteGroupChat(const QString &group_id, QJsonObject& output, QString &errormsg);
    bool CreateGroupChat(const QString &group_id, const QString &room_id, QJsonObject& output, QString &errormsg);
    bool GetAllGroupChat(QJsonObject& output, QString &errormsg);
    bool GetHistoryPeriodsGroupChat(QJsonObject& output, QString &errormsg);

    // Recurring payment
    bool GetListRecurringPayment(const QString &group_id, const QString &wallet_id, QJsonObject& output, QString &errormsg);
    bool CreateRecurringPayment(const QString &group_id,
                                const QString &wallet_id,
                                const QStringList &signatures,
                                const QJsonObject& request_body,
                                const bool isDraft,
                                QJsonObject& output,
                                QString &errormsg);

    bool GetOneRecurringPayment(const QString &group_id,
                                const QString &wallet_id,
                                const QString &recurring_payment_id,
                                QJsonObject& output,
                                QString &errormsg);

    bool CancelRecurringPayment(const QString &group_id,
                                const QString &wallet_id,
                                const QString &recurring_payment_id,
                                const QStringList &signatures,
                                const QJsonObject& request_body,
                                const bool isDraft,
                                QJsonObject& output,
                                QString &errormsg);

    bool GetKeyHealthReminder(const QString &group_id,
                              const QString &wallet_id,
                              QJsonObject& output,
                              QString &errormsg);

    bool AddOrUpdateKeyHealthReminder(const QString &group_id,
                                const QString &wallet_id,
                                const QJsonObject& request_body,
                                QJsonObject& output,
                                QString &errormsg);

    bool DeleteKeyHealthReminder(const QString &group_id,
                                 const QString &wallet_id,
                                 const QStringList &xfps,
                                 QJsonObject& output,
                                 QString &errormsg);

    bool SkipKeyHealthReminder(const QString &group_id,
                                 const QString &wallet_id,
                                 const QString &xfp,
                                 QJsonObject& output,
                                 QString &errormsg);

    bool ConfigureWalletReplacement(const QString &group_id,
                                    const QString &wallet_id,
                                    const QJsonObject& request_body,
                                    QJsonObject& output,
                                    QString &errormsg);

    bool InitiateKeyReplacement(const QString &group_id,
                                const QString &wallet_id,
                                const QString &xfp,
                                QJsonObject& output,
                                QString &errormsg);

    bool CancelKeyReplacement(const QString &group_id,
                              const QString &wallet_id,
                              const QString &xfp,
                              QJsonObject& output,
                              QString &errormsg);

    bool ReplaceKey(const QString &group_id,
                    const QString &wallet_id,
                    const QString &xfp,
                    const QString& passwordToken,
                    const QJsonObject& request_body,
                    QString &errormsg);

    bool FinalizeKeyReplacement(const QString &group_id,
                                const QString &wallet_id,
                                QJsonObject& output,
                                QString &errormsg);

    bool GetKeyReplacementStatus(const QString &group_id,
                                 const QString &wallet_id,
                                 QJsonObject& output,
                                 QString &errormsg);

    bool ResetKeyReplacement(const QString &group_id,
                             const QString &wallet_id,
                             QJsonObject& output,
                             QString &errormsg);

    bool EditGroupMembers(const QString &group_id,
                          const QJsonObject& request_body,
                          const QStringList &signatures,
                          const QString& passwordToken,
                          const QString& secQuesToken,
                          const QString& confirmToken,
                          QJsonObject &output,
                          QString &errormsg);

    bool CalculateRequireSignaturesForEditingMembers(const QString &group_id,
                                                     const QJsonObject& request_body,
                                                     QJsonObject& output,
                                                     QString &errormsg);

    // Coin control
    bool GetCoinControl(const QString &group_id, const QString &wallet_id, QJsonObject& output, QString &errormsg);
    bool UpdateCoinControl(const QString &group_id, const QString &wallet_id, const QString& data, QJsonObject& output, QString &errormsg);

    bool ReplacementDownloadBackupFile(const QString& group_id, const QString& wallet_id, const QString& xfp, const QString& passwordToken, QJsonObject& output, QString& errormsg);
    bool ReplacementUploadBackupFile(const QString& group_id, const QString& wallet_id, const QString& passwordToken, const QMap<QString, QVariant>& requestBody, QJsonObject& output, QString& errormsg);
    bool DraftWalletDownloadBackupFile(const QString& group_id, const QString& xfp, QJsonObject& output, QString& errormsg);
    bool DraftWalletUploadBackupFile(const QString& group_id, const QMap<QString, QVariant>& requestBody, QJsonObject& output, QString& errormsg);

};

#endif // BYZANTINE_H
