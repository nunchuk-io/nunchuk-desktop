#pragma once
#include <QObject>
#include "CoinWallet.h"

class HotWallet : public CoinWallet {
    Q_OBJECT
    Q_PROPERTY(bool         keyNeedBackup                           READ keyNeedBackup              WRITE setKeyNeedBackup      NOTIFY walletChanged)
public:
    HotWallet(const nunchuk::Wallet &w);
    ~HotWallet() override = default;

    bool keyNeedBackup() const; // Hotkey ??
    void setKeyNeedBackup(const bool);
public slots:
    bool allowBackup();
};
