#include "QSignerManagement.h"
#include "Chats/ClientController.h"
#include "Models/AppModel.h"
#include "Models/MasterSignerModel.h"
#include "Servers/Draco.h"
#include "ViewsEnums.h"
#include "bridgeifaces.h"
#include "localization/STR_CPP.h"
#include "Models/OnBoardingModel.h"

QSignerManagement::QSignerManagement() {
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

QSignerManagement::~QSignerManagement() {}

QSignerManagement *QSignerManagement::instance() {
    static QSignerManagement mInstance;
    return &mInstance;
}

void QSignerManagement::registerCreateMasterSigner(Execute func) {
    m_executeMaster = func;
}

bool QSignerManagement::finishCreateMasterSigner() {
    if (m_executeMaster != nullptr) {
        if (m_executeMaster()) {
            if (auto s = AppModel::instance()->masterSignerInfoPtr()) {
                QString msg = QString("<b>%1</b> has been added successfully!").arg(s->name());
                AppModel::instance()->showToast(0, msg, EWARNING::WarningType::SUCCESS_MSG);
            }
        }
        m_executeMaster = nullptr;
        return true;
    }
    return false;
}

void QSignerManagement::registerCreateRemoteSigner(Execute func) {
    m_executeRemote = func;
}

bool QSignerManagement::finishCreateRemoteSigner() {
    if (m_executeRemote != nullptr) {
        if (m_executeRemote()) {
            if (auto s = AppModel::instance()->singleSignerInfoPtr()) {
                QString msg = QString("<b>%1</b> has been added successfully!").arg(s->name());
                AppModel::instance()->showToast(0, msg, EWARNING::WarningType::SUCCESS_MSG);
            }
        }
        m_executeRemote = nullptr;
        return true;
    }
    return false;
}

void QSignerManagement::registerCreateSoftwareSigner(Execute func) {
    m_executeSoftware = func;
}

bool QSignerManagement::finishCreateSoftwareSigner() {
    if (m_executeSoftware != nullptr) {
        if (m_executeSoftware()) {
            if (auto s = AppModel::instance()->masterSignerInfoPtr()) {
                QString msg = QString("<b>%1</b> has been added successfully!").arg(s->name());
                AppModel::instance()->showToast(0, msg, EWARNING::WarningType::SUCCESS_MSG);
            }
        }
        m_executeSoftware = nullptr;
        return true;
    }
    return false;
}

void QSignerManagement::registerCreateSoftwareSignerXprv(Execute func) {
    m_executeSoftXprv = func;
}

bool QSignerManagement::finishCreateSoftwareSignerXprv() {
    if (m_executeSoftXprv != nullptr) {
        if (m_executeSoftXprv()) {
            if (auto s = AppModel::instance()->masterSignerInfoPtr()) {
                QString msg = QString("<b>%1</b> has been added successfully!").arg(s->name());
                AppModel::instance()->showToast(0, msg, EWARNING::WarningType::SUCCESS_MSG);
            }
        }
        m_executeSoftXprv = nullptr;
        return true;
    }
    return false;
}

void QSignerManagement::clearExecute() {
    DBG_INFO << "clearExecute";
    if (m_executeMaster != nullptr) {
        m_executeMaster = nullptr;
    }
    if (m_executeRemote != nullptr) {
        m_executeRemote = nullptr;
    }
    if (m_executeSoftware != nullptr) {
        m_executeSoftware = nullptr;
    }
    if (m_executeSoftXprv != nullptr) {
        m_executeSoftXprv = nullptr;
    }
}

void QSignerManagement::requestCreateSoftwareSigner(const QString &signername, const QString &mnemonic, const QString &passphrase, bool replace) {
    if (QEventProcessor::instance()->currentFlow() != (int)ENUNCHUCK::IN_FLOW::FLOW_PRIMARY_KEY &&
        QEventProcessor::instance()->currentFlow() != (int)ENUNCHUCK::IN_FLOW::FLOW_REPLACE_PRIMARY_KEY) {
        DBG_INFO << "signername" << signername << replace;
        AppModel::instance()->startCreateSoftwareSigner(signername, mnemonic, passphrase, replace);
    } else {
        requestCreateSoftwareSignerPrimaryKey(signername, mnemonic, passphrase, replace);
    }
}
void QSignerManagement::requestCreateSoftwareSignerXprv(const QString &name, const QString &xprv, bool replace) {
    if (QEventProcessor::instance()->currentFlow() != (int)ENUNCHUCK::IN_FLOW::FLOW_PRIMARY_KEY &&
        QEventProcessor::instance()->currentFlow() != (int)ENUNCHUCK::IN_FLOW::FLOW_REPLACE_PRIMARY_KEY) {
        DBG_INFO << "signername" << name << replace;
        AppModel::instance()->startCreateSoftwareSignerXprv(name, xprv);
    } else {
        requestCreateSoftwareSignerXprvPrimaryKey(name, xprv, replace);
    }
}

void QSignerManagement::requestCreateSignerFromHomeScreen() {
    QSignerManagement::instance()->clearExecute();
    AppModel::instance()->setNewWalletInfo(nullptr);
    auto softwareFunc = []() -> bool {
        QEventProcessor::instance()->sendEvent(E::EVT_ADD_SOFTWARE_SIGNER_RESULT);
        return true;
    };
    QString mnemonic = qUtils::GenerateMnemonic();
    AppModel::instance()->setMnemonic(mnemonic);
    QSignerManagement::instance()->registerCreateSoftwareSigner(softwareFunc);
    QSignerManagement::instance()->registerCreateSoftwareSignerXprv(softwareFunc);
    // request show screen
    QEventProcessor::instance()->sendEvent(E::EVT_HOME_ADD_NEW_SIGNER_REQUEST);
}

void QSignerManagement::requestCreateSignerFromReplaceScreen() {
    QSignerManagement::instance()->clearExecute();
    auto softwareFunc = []() -> bool {
        if (auto w = AppModel::instance()->newWalletInfoPtr()) {
            auto newsigner = AppModel::instance()->masterSignerInfoPtr();
            if (newsigner) {
                w->updateNewKeyReplace(newsigner->fingerPrint());
                AppModel::instance()->showToast(0, STR_CPP_062, EWARNING::WarningType::SUCCESS_MSG);
            }
        }
        return true;
    };
    QString mnemonic = qUtils::GenerateMnemonic();
    AppModel::instance()->setMnemonic(mnemonic);
    QSignerManagement::instance()->registerCreateSoftwareSigner(softwareFunc);
    QSignerManagement::instance()->registerCreateSoftwareSignerXprv(softwareFunc);
    // request show screen
    QEventProcessor::instance()->sendEvent(E::EVT_HOME_ADD_NEW_SIGNER_REQUEST);
}

void QSignerManagement::requestCreateSignerFromClaimScreen() {
    QSignerManagement::instance()->clearExecute();
    AppModel::instance()->setNewWalletInfo(nullptr);
    auto softwareFunc = []() -> bool {
        OnBoardingModel::instance()->setScreenFlow("claimAddAWallet");
        QEventProcessor::instance()->sendEvent(E::EVT_ONBOARDING_REQUEST);
        return true;
    };
    QString mnemonic = qUtils::GenerateMnemonic();
    AppModel::instance()->setMnemonic(mnemonic);
    QSignerManagement::instance()->registerCreateSoftwareSigner(softwareFunc);
    QSignerManagement::instance()->registerCreateSoftwareSignerXprv(softwareFunc);
    QSignerManagement::instance()->registerCreateMasterSigner(softwareFunc);
    // request show screen
    QEventProcessor::instance()->sendEvent(E::EVT_HOME_ADD_NEW_SIGNER_REQUEST);
}
  