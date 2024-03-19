#ifndef QBASEPREMIUM_H
#define QBASEPREMIUM_H

#include <QObject>
#include "TypeDefine.h"
#include "QWalletManagement.h"

class QBasePremium : public QObject
{
    Q_OBJECT
public:
    explicit QBasePremium(WalletId wallet_id);
    static int mode();
    template<typename T>
    static T information(const QString &wallet_id) {
        return WalletsMng->data<T>(wallet_id);
    }

    virtual QString wallet_id() const;
    virtual QWalletPtr walletInfoPtr() const;
    virtual QGroupDashboardPtr dashBoardPtr() const;
    virtual QServerKeyPtr serverKeyPtr() const;
    virtual QInheritancePlanPtr inheritancePlanPtr() const;
    virtual QGroupWalletHealthCheckPtr healthPtr() const;

    virtual QGroupWalletDummyTxPtr groupDummyTxPtr() const;
    virtual QRecurringPaymentPtr recurringPaymentPtr() const;
private:
    WalletId m_wallet_id;
};

#endif // QBASEPREMIUM_H
