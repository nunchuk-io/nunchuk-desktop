#ifndef SANDBOXWALLET_H
#define SANDBOXWALLET_H

#include <QObject>
#include "AssistedWallet.h"
#include "Premiums/GroupSandboxModel.h"

class SandboxWallet : public AssistedWallet
{
    Q_OBJECT
    // Group wallet (global)
    Q_PROPERTY(bool                 isGlobalGroupWallet             READ isGlobalGroupWallet                                    NOTIFY globalGroupWalletChanged)
    Q_PROPERTY(QGroupSandbox*       sandbox                         READ groupSandbox                                           NOTIFY groupSandboxChanged)
    Q_PROPERTY(QGroupMessageModel*  conversations                   READ conversations                                          NOTIFY walletChanged)
    Q_PROPERTY(int                  retentionDays                   READ retentionDays              WRITE setRetentionDays      NOTIFY walletConfigChanged)
    Q_PROPERTY(int                  unreadMessage                   READ unreadMessage                                          NOTIFY unreadMessageChanged)
    Q_PROPERTY(int                  numberOnline                    READ numberOnline                                           NOTIFY numberOnlineChanged)
    Q_PROPERTY(bool                 showbubbleChat                  READ showbubbleChat             WRITE setShowbubbleChat     NOTIFY showbubbleChatChanged)
    Q_PROPERTY(bool                 isReplaceGroupWallet            READ isReplaceGroupWallet                                   CONSTANT)
    Q_PROPERTY(QVariantList         replaceGroups                   READ replaceGroups                                          NOTIFY replaceGroupsChanged)
    
public:
    SandboxWallet(const nunchuk::Wallet &w);
    ~SandboxWallet() override = default;
    void convert(const nunchuk::Wallet w) override;
    bool isReplaced() const override;
    bool isGlobalGroupWallet() const;
    bool isReplaceGroupWallet() const;
    QGroupSandbox* groupSandbox();
    QGroupSandboxPtr groupSandboxPtr();
    void updateGroupSandbox(const nunchuk::GroupSandbox &value);
    void setGroupSandbox(QGroupSandboxPtr sandbox);
    
    // Groupwallet
    void GetGroupWalletConfig();
    int  numberOnline();
    void setNumberOnline(int number);
    int  unreadMessage();
    void setUnreadMessage(int number);
    int  retentionDays();
    void setRetentionDays(const int days);
    QGroupMessageModel *conversations();
    time_t lastTime();
    bool showbubbleChat() const;
    void setShowbubbleChat(bool data);
    void markAsReadMessage(const QString&msg_id);
    Q_INVOKABLE void markFiveMessagesAsRead();
    Q_INVOKABLE void markAllMessagesAsRead();

    void GetReplaceGroups();
    QVariantList replaceGroups();
public slots:
    // Group message
    void startGetUnreadMessage();
    void startDownloadConversation();
    void startSendGroupMessage(const QString &message);
    void requestAcceptReplaceGroup(const QString &sandbox_id);
    void requestDeclineReplaceGroup(const QString &sandbox_id);
signals:
    void groupSandboxChanged();
    void globalGroupWalletChanged();
    void showbubbleChatChanged();
    void replaceGroupsChanged();
    void numberOnlineChanged();
    void unreadMessageChanged();
private:
    // Groupwallet
    QGroupSandboxPtr            m_sandbox;
    nunchuk::GroupWalletConfig  m_nunchukConfig;
    QGroupMessageModelPtr       m_conversations;

    QJsonArray                  m_replaceGroups {};
    mutable int     m_numberOnline {0};
    mutable int     m_unreadMessage {0};
    mutable bool    m_showbubbleChat{false};
    bool            m_isGlobalGroupWallet {false};
};

#endif // SANDBOXWALLET_H
