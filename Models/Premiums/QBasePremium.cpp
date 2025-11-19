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
#include "Premiums/QGroupWallets.h"

QBasePremium::QBasePremium(WalletId wallet_id)
    :m_wallet_id(wallet_id),
    globalWallet(QWalletPtr(new Wallet()))
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

QString QBasePremium::wallet_id() const
{
    return m_wallet_id;
}

QWalletPtr QBasePremium::walletInfoPtr() const
{
    auto list = AppModel::instance()->walletListPtr();
    if (!list) {
        return globalWallet;
    }
    QWalletPtr wallet = list->getWalletById(wallet_id());
    if (wallet) {
        return wallet;
    }
    return globalWallet;
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
    return QWalletServicesTag::instance();
}

QWalletServicesTagPtr QBasePremium::accountServicesTagPtr() const
{
    if (ClientController::instance()->isMultiSubscriptions()) {
        return QGroupWallets::instance()->servicesTagPtr();
    }
    else {
        return ClientController::instance()->isUserWallet() ? QUserWallets::instance()->servicesTagPtr() : QGroupWallets::instance()->servicesTagPtr();
    }
}

QAssistedDraftWallets* QBasePremium::DraftWallet() const
{
    if (ClientController::instance()->isMultiSubscriptions()) {
        return dynamic_cast<QAssistedDraftWallets*>(QGroupWallets::instance());
    }
    else {
        return ClientController::instance()->isUserWallet() ? dynamic_cast<QAssistedDraftWallets*>(QUserWallets::instance()) : dynamic_cast<QAssistedDraftWallets*>(QGroupWallets::instance());
    }
}

QStringList QBasePremium::slugs() const
{
    auto info = walletInfoPtr();
    return info ? info->slugs() : QStringList();
}

bool QBasePremium::isUserDraftWallet() const
{
    auto dash = dynamic_cast<const QGroupDashboard*>(this);
    if (dash) {
        if (auto draft = QUserWallets::instance()->dashboardInfoPtr()) {
            return qUtils::strCompare(dash->groupId(), draft->groupId());
        }
    }
    return false;
}

Wallet *QBasePremium::walletInfo() const
{
    return walletInfoPtr().data();
}