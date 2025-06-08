#ifndef SHAREDWALLET_H
#define SHAREDWALLET_H

#include <QObject>
#include "HotWallet.h"

class SharedWallet : public HotWallet
{
    Q_OBJECT
    Q_PROPERTY(bool         isSharedWallet                          READ isSharedWallet                                         NOTIFY isSharedWalletChanged)
    Q_PROPERTY(int          collabWalletN                           READ collabWalletN              WRITE setCollabWalletN      NOTIFY walletChanged)
public:
    SharedWallet(const nunchuk::Wallet &w);
    ~SharedWallet() override = default;

    int     collabWalletN() const;
    void    setCollabWalletN(int data);

    bool isSharedWallet() const;
    void setIsSharedWallet(bool isShared);

    void syncCollabKeyname();
    QString roomId() const;
    void setRoomId(const QString &roomId);

    QString initEventId() const;
    void setInitEventId(const QString &initEventId);

    QWalletPtr clone() const override;
public slots:
    void slotSyncCollabKeyname(QList<DracoUser> users);
signals:
    void isSharedWalletChanged();
    void signalSyncCollabKeyname(QList<DracoUser> users);
    void roomIdChanged();
    void initEventIdChanged();
private:
    bool m_isSharedWallet {};
    QString m_roomId {};
    QString m_initEventId {};
    QList<DracoUser> m_roomMembers {};
    mutable int     m_collabWalletN {0};
};

#endif // SHAREDWALLET_H
