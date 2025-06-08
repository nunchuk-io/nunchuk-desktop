#include "PrimaryKeyAccount.h"
#include "Chats/ClientController.h"
#include "Models/AppModel.h"
#include "Models/MasterSignerModel.h"
#include "Servers/Draco.h"
#include "ViewsEnums.h"
#include "bridgeifaces.h"
#include "localization/STR_CPP.h"

QVariant PrimaryKeyAccount::primaryKeyData() const {
    return QVariant::fromValue(m_primaryKeyData);
}

bool PrimaryKeyAccount::replacePrimaryKey(const QString &mnemonic, const QString &passphrase) {
    QMasterSignerPtr curKey = AppModel::instance()->getPrimaryKey();
    if (!curKey)
        return false;
    QString curAddress = QString::fromStdString(curKey->originPrimaryKey().get_address());
    QString curUsername = QString::fromStdString(curKey->originPrimaryKey().get_account());
    QString address = qUtils::GetPrimaryKeyAddress(mnemonic, passphrase);
    QString nonce = Draco::instance()->pkey_manual_nonce(curAddress, curUsername, address, "change_pkey");
    QString curSignature = bridge::SignLoginMessage(curKey->id(), nonce);
    QString new_signature = qUtils::SignLoginMessage(mnemonic, passphrase, nonce);
    return Draco::instance()->pkey_change_pkey(address, curSignature, new_signature);
}

void PrimaryKeyAccount::requestCreateSoftwareSignerPrimaryKey(const QString &signername, const QString &mnemonic, const QString &passphrase, bool replace) {
    clearPrimaryKeyData();
    QMap<QString, QVariant> dataMap;
    dataMap["type"] = "seed";
    dataMap["signername"] = signername;
    dataMap["passphrase"] = passphrase;
    dataMap["mnemonic"] = mnemonic;
    dataMap["replace"] = replace;

    qUtils::SetChain((nunchuk::Chain)AppSetting::instance()->primaryServer());
    QString fingerprint = qUtils::GetMasterFingerprint(mnemonic, passphrase);

    dataMap["username"] = fingerprint;
    updatePrimaryKeyData(dataMap);
    if (QEventProcessor::instance()->currentFlow() == (int)ENUNCHUCK::IN_FLOW::FLOW_REPLACE_PRIMARY_KEY) {
        if (replacePrimaryKey(mnemonic, passphrase)) {
            AppModel::instance()->startCreateSoftwareSigner(signername, mnemonic, passphrase);
        }
    } else {
        QEventProcessor::instance()->sendEvent(E::EVT_PRIMARY_KEY_CONFIGURATION_REQUEST);
    }
}

void PrimaryKeyAccount::requestCreateSoftwareSignerXprvPrimaryKey(const QString &signername, const QString &xprv, bool replace) {
    clearPrimaryKeyData();
    QMap<QString, QVariant> dataMap;
    dataMap["type"] = "xprv";
    dataMap["signername"] = signername;
    dataMap["xprv"] = xprv;
    dataMap["replace"] = replace;

    qUtils::SetChain((nunchuk::Chain)AppSetting::instance()->primaryServer());
    QString fingerprint = qUtils::GetMasterFingerprintFromMasterXprv(xprv);

    dataMap["username"] = fingerprint;    
    updatePrimaryKeyData(dataMap);
    if (QEventProcessor::instance()->currentFlow() == (int)ENUNCHUCK::IN_FLOW::FLOW_REPLACE_PRIMARY_KEY) {
        // if (replacePrimaryKey(mnemonic, passphrase)) {
        //     AppModel::instance()->startCreateSoftwareSignerXprv(name, xprv);
        // }
    } else {
        QEventProcessor::instance()->sendEvent(E::EVT_PRIMARY_KEY_CONFIGURATION_REQUEST);
    }
}

void PrimaryKeyAccount::loginPrimaryKeySeed() {
    AppModel::instance()->setMasterSignerInfo( QMasterSignerPtr(new QMasterSigner()));
    if (isBusy()) return;
    qApp->setOverrideCursor(QCursor(Qt::WaitCursor));
    runInConcurrent([this]() -> bool{
        QMap<QString,QVariant> maps = m_primaryKeyData;
        QString mnemonic = maps["mnemonic"].toString();
        QString username = maps["username"].toString();
        QString passphrase = maps["passphrase"].toString();
        DBG_INFO << username;
        QJsonObject errorObj {};
        bool isAvail = Draco::instance()->pkey_username_availability(username, errorObj);
        bool ret {false};
        if(!isAvail){
            QString address = qUtils::GetPrimaryKeyAddress(mnemonic,passphrase);
            QString nonce = Draco::instance()->get_pkey_nonce(address,username);
            QString message = QString("%1%2").arg(username).arg(nonce);
            QString signature = qUtils::SignLoginMessage(mnemonic,passphrase,message);
            if(Draco::instance()->pkey_signup(address,username,signature)){
                ret = true;
            }
        } else {
            int response_code = errorObj["code"].toInt();
            QString response_msg = errorObj["message"].toString();
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
        }
        DBG_INFO << ret << isAvail;
        return ret;
    },[](bool ret) {
        qApp->restoreOverrideCursor();
        if (ret) {
            QEventProcessor::instance()->sendEvent(E::EVT_PRIMARY_KEY_CONFIGURATION_FINISHED);
        }
    });
}

void PrimaryKeyAccount::loginPrimaryKeyXprv() {
    AppModel::instance()->setMasterSignerInfo( QMasterSignerPtr(new QMasterSigner()));
    if (isBusy()) return;
    qApp->setOverrideCursor(QCursor(Qt::WaitCursor));
    runInConcurrent([this]() -> bool{
        QMap<QString,QVariant> maps = m_primaryKeyData;
        QString xprv = maps["xprv"].toString();
        QString username = maps["username"].toString();
        DBG_INFO << username;
        QJsonObject errorObj {};
        bool isAvail = Draco::instance()->pkey_username_availability(username, errorObj);
        bool ret {false};
        if(!isAvail){
            QString address = qUtils::GetPrimaryKeyAddressFromMasterXprv(xprv);
            QString nonce = Draco::instance()->get_pkey_nonce(address,username);
            QString message = QString("%1%2").arg(username).arg(nonce);
            QString signature = qUtils::SignLoginMessageWithMasterXprv(xprv,message);
            if(Draco::instance()->pkey_signup(address,username,signature)){
                ret = true;
            }
        } else {
            int response_code = errorObj["code"].toInt();
            QString response_msg = errorObj["message"].toString();
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
        }
        DBG_INFO << ret << isAvail;
        return ret;
    },[](bool ret) {
        qApp->restoreOverrideCursor();
        if (ret) {
            QEventProcessor::instance()->sendEvent(E::EVT_PRIMARY_KEY_CONFIGURATION_FINISHED);
        }
    });
}

void PrimaryKeyAccount::loginPrimaryKey() {
    QString type = m_primaryKeyData.value("type").toString();
    if (type == "seed") {
        loginPrimaryKeySeed();
    } else if (type == "xprv") {
        loginPrimaryKeyXprv();
    } else {
    }
}

void PrimaryKeyAccount::loginPrimaryKeySeedSuccess() {
    QMap<QString,QVariant> maps = m_primaryKeyData;
    QString signername = maps.value("signername").toString();
    QString passphrase = maps.value("passphrase").toString();
    QString mnemonic = maps.value("mnemonic").toString();

    QMap<QString, QVariant> makeInstanceData;
    makeInstanceData["state_id"] = E::STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION;
    makeInstanceData["signername"] = signername;
    makeInstanceData["passphrase"] = passphrase;
    makeInstanceData["mnemonic"] = mnemonic;

    bool ret = AppModel::instance()->makeInstanceForAccount(makeInstanceData,"");
    if(ret){
        AppModel::instance()->startCreateSoftwareSigner(signername, mnemonic, passphrase);
    }
}

void PrimaryKeyAccount::loginPrimaryKeyXprvSuccess() {
    QMap<QString,QVariant> maps = m_primaryKeyData;
    QString signername = maps.value("signername").toString();
    QString xprv = maps.value("xprv").toString();

    QMap<QString, QVariant> makeInstanceData;
    makeInstanceData["state_id"] = E::STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION;
    makeInstanceData["signername"] = signername;
    makeInstanceData["xprv"] = xprv;

    bool ret = AppModel::instance()->makeInstanceForAccount(makeInstanceData,"");
    if(ret){
        AppModel::instance()->startCreateSoftwareSignerXprv(signername, xprv);
    }
}

void PrimaryKeyAccount::loginPrimaryKeySuccess() {
    QString type = m_primaryKeyData.value("type").toString();
    if (type == "seed") {
        loginPrimaryKeySeedSuccess();
    } else if (type == "xprv") {
        loginPrimaryKeyXprvSuccess();
    } else {
    }
}

void PrimaryKeyAccount::requestCreateSignerFromCreatePrimaryKeyScreen() {
    clearExecute();
    auto softwareFunc = []() -> bool {
        uint last = QEventProcessor::instance()->getCurrentStates().last();
        if (last == (int)E::STATE_ID_SCR_UNLOCK_DB) {
            QEventProcessor::instance()->sendEvent(E::EVT_GOTO_HOME_WALLET_TAB);
            AppModel::instance()->makeMatrixInstanceForAccount();
            QMasterSignerPtr pKey = AppModel::instance()->getPrimaryKey();
            if (pKey) {
                timeoutHandler(3000, [pKey]() {
                    AppModel::instance()->showToast(0, STR_CPP_108.arg(pKey->name()), EWARNING::WarningType::SUCCESS_MSG);
                    QWarningMessage msg;
                    bridge::nunchukClearSignerPassphrase(pKey->fingerPrint(), msg);
                });
            }
        }
        return true;
    };
    QString mnemonic = qUtils::GenerateMnemonic();
    AppModel::instance()->setMnemonic(mnemonic);
    registerCreateSoftwareSigner(softwareFunc);
    registerCreateSoftwareSignerXprv(softwareFunc);
}

void PrimaryKeyAccount::requestSignInFromImportPrimaryKey() {
    clearExecute();
    auto softwareFunc = []() -> bool {
        uint last = QEventProcessor::instance()->getCurrentStates().last();
        if (last == (int)E::STATE_ID_SCR_UNLOCK_DB) {
            QEventProcessor::instance()->sendEvent(E::EVT_GOTO_HOME_WALLET_TAB);
            AppModel::instance()->makeMatrixInstanceForAccount();
            QMasterSignerPtr pKey = AppModel::instance()->getPrimaryKey();
            if (pKey) {
                timeoutHandler(3000, [pKey]() {
                    AppModel::instance()->showToast(0, STR_CPP_108.arg(pKey->name()), EWARNING::WarningType::SUCCESS_MSG);
                    QWarningMessage msg;
                    bridge::nunchukClearSignerPassphrase(pKey->fingerPrint(), msg);
                });
            }
        } else {
            QEventProcessor::instance()->sendEvent(E::EVT_PRIMARY_KEY_ENTER_PASSPHRASE_SUCCEED);
        }
        return true;
    };
    registerCreateSoftwareSigner(softwareFunc);
    registerCreateSoftwareSignerXprv(softwareFunc);
}

void PrimaryKeyAccount::requestSignInByManuallyPrimaryKey() {
    clearExecute();
    auto softwareFunc = []() -> bool {
        uint last = QEventProcessor::instance()->getCurrentStates().last();
        if (last == (int)E::STATE_ID_SCR_UNLOCK_DB) {
            QEventProcessor::instance()->sendEvent(E::EVT_GOTO_HOME_WALLET_TAB);
            AppModel::instance()->makeMatrixInstanceForAccount();
            QMasterSignerPtr pKey = AppModel::instance()->getPrimaryKey();
            if (pKey) {
                timeoutHandler(3000, [pKey]() {
                    AppModel::instance()->showToast(0, STR_CPP_108.arg(pKey->name()), EWARNING::WarningType::SUCCESS_MSG);
                    QWarningMessage msg;
                    bridge::nunchukClearSignerPassphrase(pKey->fingerPrint(), msg);
                });
            }
        }
        return true;
    };
    registerCreateSoftwareSigner(softwareFunc);
    registerCreateSoftwareSignerXprv(softwareFunc);
}

void PrimaryKeyAccount::requestReplacePrimaryKey() {
    clearExecute();
    auto softwareFunc = []() -> bool {
        QEventProcessor::instance()->sendEvent(E::EVT_ADD_SOFTWARE_SIGNER_RESULT);
        AppModel::instance()->setPrimaryKey(ClientController::instance()->getMe().username);
        return true;
    };
    QString mnemonic = qUtils::GenerateMnemonic();
    AppModel::instance()->setMnemonic(mnemonic);
    registerCreateSoftwareSigner(softwareFunc);
    registerCreateSoftwareSignerXprv(softwareFunc);
}

void PrimaryKeyAccount::updatePrimaryKeyData(const QString &key, const QVariant &value) {
    if (m_primaryKeyData[key] == value)
        return;
        
    m_primaryKeyData[key] = value;
    emit primaryKeyDataChanged();
}

void PrimaryKeyAccount::updatePrimaryKeyData(const QMap<QString, QVariant> &dataMap) {
    for (auto it = dataMap.constBegin(); it != dataMap.constEnd(); ++it) {
        m_primaryKeyData[it.key()] = it.value();
    }
    emit primaryKeyDataChanged();
}

void PrimaryKeyAccount::clearPrimaryKeyData() {
    m_primaryKeyData.clear();
    emit primaryKeyDataChanged();
}