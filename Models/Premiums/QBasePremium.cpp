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

QBasePremium::QBasePremium(WalletId wallet_id)
    :m_wallet_id(wallet_id)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

int QBasePremium::mode()
{
    if (ClientController::instance()->isByzantine()) {
        return GROUP_WALLET;
    }
    else if (ClientController::instance()->isHoneyBadger() || ClientController::instance()->isIronHand()) {
        return USER_WALLET;
    }
    else {
        return FREE_USER;
    }
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
    if (auto w = walletInfoPtr())
    {
        return w->dashboard();
    }
    return {};
}

QServerKeyPtr QBasePremium::serverKeyPtr() const
{
    if (auto w = walletInfoPtr())
    {
        return w->serverKeyPtr();
    }
    return {};
}

QInheritancePlanPtr QBasePremium::inheritancePlanPtr() const
{
    if (auto w = walletInfoPtr())
    {
        return w->inheritancePlanPtr();
    }
    return {};
}

QGroupWalletHealthCheckPtr QBasePremium::healthPtr() const
{
    if (auto w = walletInfoPtr())
    {
        return w->healthPtr();
    }
    return {};
}

QGroupWalletDummyTxPtr QBasePremium::groupDummyTxPtr() const
{
    if (auto w = walletInfoPtr())
    {
        return w->groupDummyTxPtr();
    }
    return {};
}

QRecurringPaymentPtr QBasePremium::recurringPaymentPtr() const
{
    if (auto w = walletInfoPtr())
    {
        return w->recurringPaymentPtr();
    }
    return {};
}
