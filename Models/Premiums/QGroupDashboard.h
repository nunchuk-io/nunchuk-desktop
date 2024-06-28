#ifndef QGROUPDASHBOARD_H
#define QGROUPDASHBOARD_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include "TypeDefine.h"
#include <nunchuk.h>
#include "Premiums/QBasePremium.h"

class AlertEnum : public QObject
{
    Q_OBJECT
public:
    Q_ENUMS(E_Alert_t)
    enum class E_Alert_t : int{
        GROUP_WALLET_PENDING,
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

    Q_PROPERTY(QVariantList keys                  READ keys                                     NOTIFY draftWalletChanged)
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

public:
    QGroupDashboard(const QString& wallet_id);
    ~QGroupDashboard();

    GroupId groupId() const;

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
    void GetMemberInfo();
    void GetAlertsInfo();
    bool MarkAlertAsRead(const QString &alert_id);
    bool DismissAlert(const QString &alert_id);
    bool DismissAlert();
    void GetWalletInfo();
    void checkInheritanceWallet();
    void GetDraftWalletInfo();
    void GetHealthCheckInfo();
    void UpdateKeys(const QJsonObject &data);

    QVariantList alerts() const;
    QVariant alert() const;
    QJsonObject alertJson() const;
    QVariantList keys() const;

    bool hasWallet() const;

    bool showDashBoard() const;
    void setShowDashBoard(bool showDashBoard);
    int mInfo() const;
    int nInfo() const;

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

    bool canReplaceKey();

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

public slots:
    bool requestStartKeyReplacement(const QString &tag);
    void requestHealthCheck(const QString &xfp);
    bool requestByzantineChat();

    void byzantineRoomCreated(QString room_id, QString group_id, bool existed);
    void byzantineRoomDeleted(QString room_id, QString group_id);
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

private:
    QJsonObject m_groupInfo {};
    QJsonObject m_alertInfo {};
    QJsonObject m_walletInfo {};
    QJsonObject m_myInfo {};
    QJsonObject m_inviterInfo {};
    QJsonArray m_signerInfo {};
    QString m_alertId {};
    QVariantList m_keys {};
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
};
#endif // QGROUPDASHBOARD_H
