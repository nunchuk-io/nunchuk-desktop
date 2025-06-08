#include "ServiceSetting.h"
#include "Chats/ClientController.h"
#include "OnBoardingModel.h"
#include "Premiums/QGroupWalletDummyTx.h"
#include "Premiums/QGroupWallets.h"
#include "Premiums/QUserWallets.h"
#include "Premiums/QWalletServicesTag.h"
#include "ProfileSetting.h"
#include "Servers/Draco.h"
#include "ViewsEnums.h"
#include "nunchuckiface.h"

ServiceSetting::ServiceSetting(QObject *parent) : QStateFlow(), walletInfo_(QWalletPtr(new Wallet())) {}

ServiceSetting *ServiceSetting::instance() {
    static ServiceSetting mInstance;
    return &mInstance;
}

bool ServiceSetting::existHardware(const QString &tag) {
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

int ServiceSetting::optionIndex() const {
    return m_optionIndex;
}

void ServiceSetting::setOptionIndex(int index) {
    if (m_optionIndex == index)
        return;

    m_optionIndex = index;
    emit optionIndexChanged();
}

QVariant ServiceSetting::servicesTag() const {
    return QVariant::fromValue(servicesTagPtr().data());
}

QWalletServicesTagPtr ServiceSetting::servicesTagPtr() const {
    return QWalletServicesTag::instance();
}

Wallet *ServiceSetting::walletInfo() const {
    return walletInfo_.data();
}

QWalletPtr ServiceSetting::walletInfoPtr() const {
    return walletInfo_;
}

void ServiceSetting::setWalletInfo(const QWalletPtr &d) {
    if (d) {
        walletInfo_ = bridge::convertWallet(d->nunchukWallet());
        if (walletInfo_) {
            QGroupWallets::instance()->setDashboardInfo(walletInfo_);
            if (auto dummy = walletInfo_->groupDummyTxPtr()) {
                dummy->setCurrentXfp("");
            }
        }
        emit walletInfoChanged();
    }
}

void ServiceSetting::requestStartAddHardwareKey(const QString &tag) {
    if (QAssistedDraftWallets::IsByzantine()) {
        QGroupWallets::instance()->setDashboardInfo(QGroupWallets::instance()->request().mGroupId);
    } else {
        QGroupWallets::instance()->SignInDashBoard(QUserWallets::instance()->dashboardInfoPtr());
    }
    if (auto dash = QGroupWallets::instance()->dashboardInfoPtr()) {
        dash->requestStartKeyReplacement(tag);
    }
}

void ServiceSetting::clearWalletInfo() {
    walletInfo_.clear();
    emit walletInfoChanged();
}

void ServiceSetting::handleClaimInheritance(const QVariant &msg) {
    const auto maps = msg.toMap();
    const QString type = maps.value("type").toString();
    const bool isShowScreen = maps.value("isShowScreen").toBool();

    if (type == "withdraw-a-custom-amount") {
        ServiceSetting::instance()->setScreenFlow(type);
        QEventProcessor::instance()->sendEvent(E::EVT_INHERITANCE_WITHDRAW_BALANCE_REQUEST);
        return;
    }

    const int64_t amount = maps.value("amount").toLongLong();
    DBG_INFO << " Claim inheritance amount: " << amount;
    servicesTagPtr()->setClaimInheritanceCustomAmount(amount);

    if (type == "withdraw-to-a-nunchuk-wallet") {
        const int keyCount = AppModel::instance()->masterSignerList()->rowCount() + AppModel::instance()->remoteSignerList()->rowCount();
        const int walletCount = AppModel::instance()->walletList()->rowCount();

        if (walletCount == 0 && keyCount <= 0) {
            ServiceSetting::instance()->setScreenFlow("inheritance-claim-empty-key");
            if (isShowScreen) {
                QEventProcessor::instance()->sendEvent(E::EVT_INHERITANCE_WITHDRAW_BALANCE_REQUEST);
            }
            return;
        }

        if (walletCount == 0) {
            OnBoardingModel::instance()->setScreenFlow("claimAddAWallet");
            QEventProcessor::instance()->sendEvent(E::EVT_ONBOARDING_REQUEST);
            DBG_INFO << "Request show screen: You don't have a wallet yet.";
            return;
        }
    }

    ServiceSetting::instance()->setScreenFlow(type);
    if (isShowScreen) {
        QEventProcessor::instance()->sendEvent(E::EVT_INHERITANCE_WITHDRAW_BALANCE_REQUEST);
    }    
}