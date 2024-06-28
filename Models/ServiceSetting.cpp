#include "ServiceSetting.h"
#include "Servers/Draco.h"
#include "nunchuckiface.h"
#include "ProfileSetting.h"
#include "Premiums/QUserWallets.h"
#include "Premiums/QGroupWallets.h"
#include "Chats/ClientController.h"
#include "Premiums/QWalletServicesTag.h"
#include "Premiums/QGroupWalletDummyTx.h"

ServiceSetting::ServiceSetting(QObject *parent)
    : QObject(parent)
    , walletInfo_(QWalletPtr(new Wallet()))
{

}

ServiceSetting *ServiceSetting::instance() {
    static ServiceSetting mInstance;
    return &mInstance;
}

int ServiceSetting::claimInheritanceFlow() const
{
    return m_claimInheritanceFlow;
}

void ServiceSetting::setClaimInheritanceFlow(int flow)
{
    if (m_claimInheritanceFlow == flow)
        return;

    m_claimInheritanceFlow = flow;
    emit claimInheritanceFlowChanged();
}

bool ServiceSetting::existHardware(const QString &tag)
{
    if (QAssistedDraftWallets::IsByzantine()) {
        QGroupWallets::instance()->MixMasterSignerAndSingleSigner(tag);
        if (QGroupWallets::instance()->signerExistList().size() > 0) {
            return true;
        }
    } else {
        QUserWallets::instance()->MixMasterSignerAndSingleSigner(tag);
        if (QUserWallets::instance()->signerExistList().size() > 0) {
            return true;
        }
    }
    return false;
}

int ServiceSetting::optionIndex() const
{
    return m_optionIndex;
}

void ServiceSetting::setOptionIndex(int index)
{
    if (m_optionIndex == index)
        return;

    m_optionIndex = index;
    emit optionIndexChanged();
}

QVariant ServiceSetting::servicesTag() const
{
    return QVariant::fromValue(servicesTagPtr().data());
}

QWalletServicesTagPtr ServiceSetting::servicesTagPtr() const
{
    if (walletInfo()) {
        return walletInfo()->servicesTagPtr();
    } else {
        if (ClientController::instance()->isMultiSubscriptions()) {
            return QGroupWallets::instance()->servicesTagPtr();
        } else {
            return QBasePremium::mode() == USER_WALLET ? QUserWallets::instance()->servicesTagPtr() : QGroupWallets::instance()->servicesTagPtr();
        }
    }
}

Wallet *ServiceSetting::walletInfo() const
{
    return walletInfo_.data();
}

QWalletPtr ServiceSetting::walletInfoPtr() const
{
    return walletInfo_;
}

void ServiceSetting::setWalletInfo(const QWalletPtr &d)
{
    if(d){
        walletInfo_ = bridge::convertWallet(d->wallet());
        if (walletInfo_) {
            QGroupWallets::instance()->setDashboardInfo(walletInfo_);
            if (auto dummy = walletInfo_->groupDummyTxPtr()) {
                dummy->setCurrentXfp("");
            }
        }
        emit walletInfoChanged();
    }
}

void ServiceSetting::clearWalletInfo()
{
    walletInfo_.clear();
    emit walletInfoChanged();
}
