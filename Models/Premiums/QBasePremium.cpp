#include "QBasePremium.h"
#include "AppModel.h"
#include "WalletModel.h"
#include "Chats/ClientController.h"
#include "Premiums/QGroupDashboard.h"
#include "Premiums/QWalletDummyTx.h"
#include "Premiums/QGroupWalletDummyTx.h"
#include "Premiums/QUserWalletDummyTx.h"
#include "Premiums/QGroupWalletHealthCheck.h"
#include "Premiums/QServerKey.h"
#include "Premiums/QInheritancePlan.h"
#include "Premiums/QRecurringPayment.h"
#include "Premiums/QWalletServicesTag.h"
#include "Premiums/QUserWallets.h"

QBasePremium::QBasePremium(WalletId wallet_id)
    :m_wallet_id(wallet_id)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

QString QBasePremium::wallet_id() const
{
    return m_wallet_id;
}

QWalletPtr QBasePremium::walletInfoPtr() const
{
    if (auto wallet = AppModel::instance()->walletListPtr()->getWalletById(wallet_id())) {
        return wallet;
    }
    else {
        return QWalletPtr(new Wallet);
    }
}

QGroupDashboardPtr QBasePremium::dashBoardPtr() const
{
    return QGroupDashboard::information<QGroupDashboardPtr>(wallet_id());
}

QServerKeyPtr QBasePremium::serverKeyPtr() const
{
    return QServerKey::information<QServerKeyPtr>(wallet_id());
}

QInheritancePlanPtr QBasePremium::inheritancePlanPtr() const
{
    return QInheritancePlan::information<QInheritancePlanPtr>(wallet_id());
}

QGroupWalletHealthCheckPtr QBasePremium::healthPtr() const
{
    return QGroupWalletHealthCheck::information<QGroupWalletHealthCheckPtr>(wallet_id());
}

QGroupWalletDummyTxPtr QBasePremium::groupDummyTxPtr() const
{
    if (auto dummy = QWalletDummyTx::information<QWalletDummyTxPtr>(wallet_id())) {
        return dummy->get<QGroupWalletDummyTxPtr>();
    }
    return {};
}

QRecurringPaymentPtr QBasePremium::recurringPaymentPtr() const
{
    return QRecurringPayment::information<QRecurringPaymentPtr>(wallet_id());
}

QWalletServicesTagPtr QBasePremium::servicesTagPtr() const
{
    if (auto w = walletInfoPtr())
    {
        return w->servicesTagPtr();
    }
    return {};
}

QStringList QBasePremium::slugs() const
{
    return walletInfoPtr() ? walletInfoPtr()->slugs() : QStringList();
}

bool QBasePremium::isDraftWallet() const
{
    auto dash = dynamic_cast<const QGroupDashboard*>(this);
    if (dash) {
        if (auto draft = QUserWallets::instance()->dashboardInfoPtr()) {
            return qUtils::strCompare(dash->groupId(), draft->groupId());
        }
    }
    return false;
}
