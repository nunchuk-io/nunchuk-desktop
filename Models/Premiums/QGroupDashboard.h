#ifndef QGROUPDASHBOARD_H
#define QGROUPDASHBOARD_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include "TypeDefine.h"
#include <nunchuk.h>
#include "Premiums/QBasePremium.h"
#include <QTimer>
#include "UseCase/TimeLockUseCase.h"

class AlertEnum : public QObject
{
    Q_OBJECT
public:
    Q_ENUMS(E_Alert_t)
    enum class E_Alert_t : int{
        GROUP_WALLET_PENDING = 0,
        GROUP_WALLET_SETUP,
        DRAFT_WALLET_KEY_ADDED,
        UPDATE_SERVER_KEY,
        UPDATE_SERVER_KEY_SUCCESS,
        CREATE_INHERITANCE_PLAN,
        UPDATE_INHERITANCE_PLAN,
        CANCEL_INHERITANCE_PLAN,
        CREATE_INHERITANCE_PLAN_SUCCESS,
        UPDATE_INHERITANCE_PLAN_SUCCESS,
        CANCEL_INHERITANCE_PLAN_SUCCESS,
        REQUEST_INHERITANCE_PLANNING,
        REQUEST_INHERITANCE_PLANNING_APPROVED,
        REQUEST_INHERITANCE_PLANNING_DENIED,
        KEY_RECOVERY_REQUEST,
        KEY_RECOVERY_APPROVED,
        KEY_RECOVERY_SUCCESS,
        RECURRING_PAYMENT_REQUEST,
        RECURRING_PAYMENT_CANCELATION_PENDING,
        RECURRING_PAYMENT_APPROVED,
        RECURRING_PAYMENT_DENIED,
        RECURRING_PAYMENT_CANCELED,
        HEALTH_CHECK_REQUEST,
        HEALTH_CHECK_PENDING,
        HEALTH_CHECK_COMPLETED,
        GROUP_WALLET_INVITATION_DENIED,
        TRANSACTION_SIGNATURE_REQUEST,
        UPDATE_SECURITY_QUESTIONS,
        WALLET_PENDING,

        // other enum
        CANCEL_RECURRING_PAYMENT,
        CREATE_RECURRING_PAYMENT,
        CHANGE_EMAIL_REQUEST,
        CHANGE_EMAIL_SUCCESS,
        // not alert
        HEALTH_CHECK_STATUS,
        // use for service tag
        SERVICE_TAG_POLICY_UPDATE,
        SERVICE_TAG_INHERITANCE_PLAN_CREATE,
        SERVICE_TAG_INHERITANCE_PLAN_UPDATE,
        SERVICE_TAG_INHERITANCE_PLAN_CANCEL,
        SERVICE_TAG_SHARE_YOUR_SECRET,
        SERVICE_TAG_UPDATE_SECURITY_QUESTION,

        MANAGE_GROUP_CHAT_HISTORY,
        CHANGE_EMAIL,
        HEALTH_CHECK_REMINDER,

        KEY_REPLACEMENT_PENDING,
        KEY_REPLACEMENT_COMPLETED,
        TRANSFER_FUNDS,
        WALLET_LOCKED,
        SETUP_INHERITANCE_PLAN,

        // Health Check Reminder
        HEALTH_CHECK_REMINDER_EMPTY_STATE,
        HEALTH_CHECK_SELECT_KEY_WANT_TO_REMINDER,
        HEALTH_CHECK_FREQUENCY_REPEAT,
        HEALTH_CHECK_REMINDER_POPULATED,

        WELCOME_SIGN_IN_SIGNATURE_XPUB,

        // New for HB onchain alert types
        BACKUP_WALLET,
        CHANGE_TIMELOCK_TYPE,
        KEY_TIMELOCK_UPDATE_PENDING,

        MAX_ALERT,
    };
};

int StringToInt(const QString &type);
QString IntToString(const int type);

class QGroupDashboard : public QBasePremium
{
    Q_OBJECT
    Q_PROPERTY(QString groupStatus                READ groupStatus                              NOTIFY groupInfoChanged)
    Q_PROPERTY(QString myRole                     READ myRole                                   NOTIFY groupInfoChanged)
    Q_PROPERTY(bool    accepted                   READ accepted                                 NOTIFY groupInfoChanged)
    Q_PROPERTY(QString groupId                    READ groupId                                  NOTIFY groupInfoChanged)
    Q_PROPERTY(QString userName                   READ userName                                 NOTIFY groupInfoChanged)
    Q_PROPERTY(QString userEmail                  READ userEmail                                NOTIFY groupInfoChanged)

    Q_PROPERTY(QVariantList members               READ members                                  NOTIFY groupInfoChanged)
    Q_PROPERTY(QVariantList memberSignatures      READ memberSignatures                         NOTIFY groupInfoChanged)
    Q_PROPERTY(QVariant inviter                   READ inviter                                  NOTIFY groupInfoChanged)
    Q_PROPERTY(QVariant myInfoModel               READ myInfoModel                              NOTIFY groupInfoChanged)
    Q_PROPERTY(bool isLocked                      READ isLocked                                 NOTIFY groupInfoChanged)
    Q_PROPERTY(QString walletType                 READ walletType                               NOTIFY draftWalletChanged)

    Q_PROPERTY(QVariantList keys                  READ keys                                     NOTIFY draftWalletChanged)
    Q_PROPERTY(QVariantList replaceKeys           READ replaceKeys                              NOTIFY replacementKeysChanged)

    Q_PROPERTY(bool hasWallet                     READ hasWallet                                NOTIFY groupInfoChanged)
    Q_PROPERTY(QVariantList alerts                READ alerts                                   NOTIFY alertInfoChanged)
    Q_PROPERTY(QVariant alert                     READ alert                                    NOTIFY alertInfoChanged)
    Q_PROPERTY(bool isShowDashBoard               READ showDashBoard  WRITE setShowDashBoard    NOTIFY showDashBoardChanged)
    Q_PROPERTY(int mInfo                          READ mInfo                                    NOTIFY draftWalletChanged)
    Q_PROPERTY(int nInfo                          READ nInfo                                    NOTIFY draftWalletChanged)
    Q_PROPERTY(QVariant health                    READ health                                   NOTIFY groupInfoChanged)
    Q_PROPERTY(QString configFlow                 READ configFlow                               NOTIFY configFlowChanged)
    Q_PROPERTY(QString walletunUsedAddress        READ walletunUsedAddress                      NOTIFY walletunUsedAddressChanged)
    Q_PROPERTY(bool    isPremierGroup             READ isPremierGroup                           NOTIFY groupInfoChanged)
    Q_PROPERTY(bool    allowInheritance           READ allowInheritance                         NOTIFY draftWalletChanged)
    Q_PROPERTY(int inheritanceCount               READ inheritanceCount                         NOTIFY inheritanceCountChanged)
    Q_PROPERTY(QString historyPeriodId            READ historyPeriodId                          NOTIFY historyPeriodIdChanged)
    Q_PROPERTY(bool groupChatExisted              READ groupChatExisted                         NOTIFY groupChatExistedChanged)
    Q_PROPERTY(QVariantList editMembers           READ editMembers                              NOTIFY editMembersChanged)
    Q_PROPERTY(QString  timeLock                  READ timeLock                                 NOTIFY draftWalletChanged)
    Q_PROPERTY(QVariant timeLockSet               READ timeLockSet                              NOTIFY timeLockSetChanged)
    Q_PROPERTY(QString  timelockReplacementDisp   READ timelockReplacementDisp                  NOTIFY timelockReplacementChanged)
    Q_PROPERTY(QVariant timelockReplacement       READ timelockReplacement                      NOTIFY timelockReplacementChanged)

public:
    QGroupDashboard(const QString& wallet_id);
    ~QGroupDashboard();

    GroupId groupId() const;
    QString walletType() const;

    QJsonObject groupInfo() const;
    void setGroupInfo(const QJsonObject &groupInfo);
    QString groupStatus() const;
    QString myRole() const;
    QString walletname() const;
    QString walletStatus() const;
    bool    isReplaced();
    bool    accepted() const;
    QString userName() const;
    QString userEmail() const;
    QVariantList members() const;
    QVariantList memberSignatures();
    QVariant inviter() const;
    QJsonObject walletJson() const;
    QJsonObject walletDraftJson() const;
    void GetMemberInfo();
    void GetAlertsInfo();
    bool MarkAlertAsRead(const QString &alert_id);
    bool DismissAlert(const QString &alert_id);
    void GetWalletInfo();
    void checkInheritanceWallet();
    void GetDraftWalletInfo();
    void GetHealthCheckInfo();
    QString getOurId() const;
    QJsonObject createOrUpdateSignerInfo(const QJsonObject &info, int index);
    void UpdateKeys(const QJsonObject &data);
    QJsonObject CreateAccountIndexAndKeyIndices(const QJsonArray &signers, int index);
    void UpdateReplacementKeys(const QJsonObject &data);
    bool GetKeyReplacementStatus();
    QJsonObject GetSigerReplacement(const QString &xfp);
    QMap<QString, QVariant> requestBodyUploadBackupFile(const QString &xfp, const QString &filePath);
    bool ReplacementUploadBackupFile(const QString &xfp, const QString &filePath);
    bool DraftWalletUploadBackupFile(const QString &xfp, const QString &filePath);

    QVariantList alerts() const;
    QVariant alert() const;
    QJsonObject alertJson() const;
    QVariantList keys() const;
    QVariantList replaceKeys() const;

    bool hasWallet() const;

    bool showDashBoard();
    void setShowDashBoard(bool showDashBoard);
    int mInfo() const;
    int nInfo() const;

    QJsonObject GetDraftSigner(const QString& xfp) const;
    QJsonObject GetSigner(const QString& xfp) const;

    int flow() const;
    void setFlow(int flow);

    QString configFlow() const;
    void setConfigFlow(const QString &configFlow);

    QString walletunUsedAddress() const;
    void setWalletunUsedAddress(const QString &walletunUsedAddress);

    QJsonObject inviterInfo() const;
    void setInviterInfo(const QJsonObject &inviterInfo);

    QVariant myInfoModel() const;
    QString role() const;
    QJsonObject myInfo() const;
    void setMyInfo(const QJsonObject &myInfo);

    bool register_wallet();
    bool isLocked() const;
    bool registerKeyDone();
    bool registerKeyNext();

    QString alertId() const;
    void setAlertId(const QString &alertId);
    void setAlertId(const QJsonObject &alert);
    void setDummyTxAlert(const QJsonObject &dummy_transaction);

    bool CancelRecoveryKey();

    bool CancelKeyReplacement();
    bool FinishKeyReplacement(const QJsonObject &requestBody);

    Q_INVOKABLE bool canReplaceKey();
    Q_INVOKABLE bool isInheritance();

    QStringList getNameSameTag(const QString &tag);
    QString createName(const QString &tag, int &index);
    QString generateName(const QStringList &tags);

    bool canEntryClickAlert();

    bool allowInheritance() const;

    int inheritanceCount() const;
    void setInheritanceCount(int count);

    void GetGroupChat();
    void UpdateGroupChat(const QString &history_period_id);

    QString historyPeriodId() const;
    void setHistoryPeriodId(const QString &newHistoryPeriodId);

    void getChatInfo();
    bool groupChatExisted();
    void setGroupChatExisted(bool existed);

    bool EditGroupMembers();
    bool CalculateRequireSignaturesForEditingMembers();
    QVariantList editMembers() const;
    void initMembers();
    bool addMember(const QJsonObject &aEditMember);
    bool removeMember(const QJsonObject &aEditMember);
    bool editMembers(const QJsonObject &aEditMember, int index);
    bool containEditMeber(const QJsonObject &aEditMember);
    bool RequestConfirmationCodeEditMembers();
    QJsonObject bodyEditMembers();
    QString timeLock() const;
    QVariant timeLockSet() const;
    QString  timelockReplacementDisp() const;
    QVariant timelockReplacement() const;
    void setTimelockReplacement(const QJsonObject &timelock);
    void clearTimeLock();

public slots:
    void draftWalletSetupTimeLock(const QVariant &datetime, bool isPutServer = false);
    void walletSetupTimeLock(const QVariant &datetime, bool isPutServer = false);
    bool requestStartKeyReplacement(const QString &tag);
    bool requestStartKeyCreate(const QString &tag, bool isFirst = false);
    
    void requestHealthCheck(const QString &xfp);
    bool requestByzantineChat();

    void byzantineRoomCreated(QString room_id, QString group_id, bool existed);
    void byzantineRoomDeleted(QString room_id, QString group_id);

    void requestShowLetAddYourKeys();
    void requestShowReplacementKey();
    bool isDowngrade(QString email_or_username, QString roleNew);
    void updateProgress();
    void updateSuccess();
    void updateFail();
    void markRead();
    void requestBackupColdcard(QVariant msg);

    void startAddKeyAtIndex(int index);
    void startReplaceKeyAtIndex(int index);
    bool enoughKeyAdded(const QString& xfp);
    bool dismissAlert();

    void preparingKeyTobeReplaced();
    void CorrectCurrentSignerInfoForReplacementKey(const QString &xfp);

private:
    bool deviceExport(const QStringList tags, nunchuk::SignerType type);
    bool xfpExport(const QString xfp);
    QVariant health() const;
signals:
    void groupInfoChanged();
    void draftWalletChanged();
    void alertInfoChanged();
    void showDashBoardChanged();
    void configFlowChanged();
    void walletunUsedAddressChanged();
    void inheritanceCountChanged();
    void historyPeriodIdChanged();
    void groupChatExistedChanged();

    void editMembersChanged();
    void editMembersSuccessChanged();
    void timeLockSetChanged();
    void timelockReplacementChanged();
    void replacementKeysChanged();
private:
    QJsonObject m_groupInfo {};
    QJsonObject m_alertInfo {};
    QJsonObject m_walletInfo {};
    QJsonObject m_myInfo {};
    QJsonObject m_inviterInfo {};
    QJsonArray m_signerInfo {};
    QJsonObject m_walletDraftInfo {};
    QJsonArray m_keys {};
    QJsonArray m_replaceKeys {};
    bool m_showDashBoard {false};
    int m_mInfo {0};
    int m_nInfo {0};
    bool m_allow_inheritance{false};
    QString m_configFlow;
    QString m_walletunUsedAddress;
    QJsonObject m_currentAlertInfo {};
    QStringList m_registered_key_xfps {};
    int m_inheritanceCount {0};
    QString mHistoryPeriodId {};
    bool m_groupChatExisted {false};
    QJsonArray m_editMembers;
    QTimer *mTimer {nullptr};
    QJsonObject m_timeLockSet;
    QJsonObject m_keyReplacementInfo;
    QJsonObject m_timeLockReplacement;
    TimeLockUseCase m_timeLockUseCase;
};
void registerAddKey();
#endif // QGROUPDASHBOARD_H
