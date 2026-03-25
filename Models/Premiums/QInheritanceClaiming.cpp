#include "QInheritanceClaiming.h"

#include <QLocale>
#include <QJsonDocument>
#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include "Servers/Draco.h"
#include "ViewsEnums.h"
#include "QEventProcessor.h"
#include "bridgeifaces.h"
#include "qUtils.h"
#include "Models/AppModel.h"
#include "AppSetting.h"
#include "Premiums/QGroupDashboard.h"
#include "Premiums/QAssistedDraftWallets.h"
#include "Premiums/QGroupWallets.h"
#include "Premiums/QUserWallets.h"
#include "Signers/QSignerManagement.h"
#include "generated_qml_keys.hpp"
#include "features/claiming/flows/OnChainClaimingFlow.h"
#include "app/AppContext.h"
#include "core/ui/UiServices.inc"

QInheritanceClaiming::QInheritanceClaiming()
    : QStateFlow()
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
    m_inheritanceCheckStatus = {};
    m_inheritanceClaimPlan = {};
    // Set default flags
}

QInheritanceClaiming::~QInheritanceClaiming() = default;

void QInheritanceClaiming::clearClaimAnInheritance() {
    clearInheritanceCheckStatus();
    setInheritanceClaimPlan({});
}

bool QInheritanceClaiming::inheritanceCheck(const QString &magic, const QString &environment) {
    QJsonObject result;
    bool ret = Draco::instance()->inheritanceCheck(magic, environment, result);
    if (ret) {
        DBG_INFO << result;
        bool is_paid = result["is_paid"].toBool();
        if (!is_paid) {
            emit notPaidAlert();
        }
    }
    return ret;
}

QVariant QInheritanceClaiming::inheritanceInfo() const {
    static const int64_t COIN = 100000000;
    QJsonObject inheritance = m_inheritanceClaimPlan["inheritance"].toObject();
    QMap<QString, QVariant> maps;
    double balance = m_inheritanceClaimPlan["balance"].toDouble(); // BTC
    int balanceSats = balance * COIN;
    QLocale locale(QLocale::English);
    if ((int)AppSetting::Unit::SATOSHI == AppSetting::instance()->unit()) {
        maps["balance"] = locale.toString(balanceSats); // sats
    } else {
        maps["balance"] = locale.toString(balance, 'f', qUtils::Precision(balance));
    }
    maps["balanceSats"] = balanceSats;
    maps["balanceCurrency"] = qUtils::currencyLocale(balanceSats);
    maps["note"] = inheritance["note"].toString();
    return QVariant::fromValue(maps);
}

QVariant QInheritanceClaiming::inheritanceCheckStatus() const {
    return QVariant::fromValue(m_inheritanceCheckStatus);
}

void QInheritanceClaiming::clearInheritanceCheckStatus() {
    m_inheritanceCheckStatus = {};
}

QVariant QInheritanceClaiming::inheritanceClaimPlan() const {
    return QVariant::fromValue(m_inheritanceClaimPlan);
}

void QInheritanceClaiming::setInheritanceClaimPlan(const QJsonObject &claim) {
    if (m_inheritanceClaimPlan == claim)
        return;

    m_inheritanceClaimPlan = claim;
    emit inheritanceClaimPlanChanged();
}

void QInheritanceClaiming::clearBufferPeriodCountdown() {
    QJsonObject data = m_inheritanceClaimPlan;
    data["buffer_period_countdown"] = {};
    DBG_INFO << data;
    setInheritanceClaimPlan(data);
}

void QInheritanceClaiming::clearInheritance() {
    QJsonObject data = m_inheritanceClaimPlan;
    data["inheritance"] = {};
    DBG_INFO << data;
    setInheritanceClaimPlan(data);
}

void QInheritanceClaiming::clearInheritanceKeys() {
    m_inheritanceKeys = QJsonArray{};
}

void QInheritanceClaiming::setClaimInit(const InitializeInheritanceClaimingResult& init) {
    m_claimInit = init;
}

void QInheritanceClaiming::claimInitSaveBsms(const QString& bsms, const nunchuk::Wallet& wallet) {
    m_walletInfoClaim["bsms"] = bsms;
    mInheritance.wallet = wallet;
}

QJsonArray QInheritanceClaiming::inheritanceKeys() const { 
    return m_inheritanceKeys; 
}

bool QInheritanceClaiming::requestDownloadWalletWithIndexAsync(const QString &xfp) {
    runInConcurrent([this, xfp]() ->bool{
        nunchuk::SingleSigner signer;
        auto draftWallet = QAssistedDraftWallets::IsByzantine() ? dynamic_cast<QAssistedDraftWallets*>(QGroupWallets::instance()) : dynamic_cast<QAssistedDraftWallets*>(QUserWallets::instance());
        bool ret = draftWallet->checkAndGetSingleSigner(xfp, 0, signer);
        if (ret) {
            return requestClaimInheritanceFromSigner(signer);
        } else {
            DBG_INFO << "Cannot find signer with xfp:" << xfp;
        }
        return ret;
    },[](bool ret) {
    });
    return false;
}

bool QInheritanceClaiming::requestDownloadWalletViaQR(const QStringList &qr_data) {
    runInConcurrent([this, qr_data]() ->bool{
        QWarningMessage msg;
        nunchuk::SingleSigner signer = bridge::nunchukParseQRSigners(qr_data, 0, msg);
        if ((int)EWARNING::WarningType::NONE_MSG == msg.type()) {
            bool ret = requestClaimInheritanceFromSigner(signer);
            return ret;
        } else {
            AppModel::instance()->showToast(msg.code(), msg.what(), EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    },[](bool ret) {
    });
    return false;
}
 
bool QInheritanceClaiming::requestDownloadWalletViaImportFile(const QString &fileName) {
    runInConcurrent([this, fileName]() ->bool{
        QWarningMessage msg;
        QString file_path = qUtils::QGetFilePath(fileName);
        nunchuk::SingleSigner fileSigner = bridge::nunchukParseJSONSigners(file_path, 0, nunchuk::SignerType::AIRGAP, msg);
        if ((int)EWARNING::WarningType::NONE_MSG == msg.type()) {
            bool ret = requestClaimInheritanceFromSigner(fileSigner);
            return ret;
        } else {
            return false;
        }
    },[](bool ret) {
    });
    return false;
}

bool QInheritanceClaiming::requestClaimInheritanceFromSigner(const nunchuk::SingleSigner &signer) {
    auto keyObj = qUtils::SingleSignertoJsonObject(signer);
    auto contains = [this, keyObj]() -> bool {
        for (const auto& existingKey : m_inheritanceKeys) {
            if (existingKey.toObject().value("xfp") == keyObj.value("xfp")) {
                return true;
            }
        }
        return false;
    };
    if (contains()) {
        DBG_INFO << "Key already added:" << keyObj;
        emit resultClaimInheritanceAlert("CLAIM_INHERITANCE_SECOND_KEY");
        return false;
    } else {
        DBG_INFO << "Adding key:" << keyObj;
    }
    m_inheritanceKeys.append(keyObj);
    int inheritance_key_added_count = inheritanceKeys().size();
    DBG_INFO << "inheritance_key_count:" << m_claimInit.inheritance_key_count << "inheritance_key_added_count:" << inheritance_key_added_count;
    if (inheritance_key_added_count == 1 && m_claimInit.inheritance_key_count == 2) {
        // add more keys
        QEventProcessor::instance()->sendEvent(E::EVT_ONS_CLOSE_ALL_REQUEST);
        emit resultClaimInheritanceAlert("CLAIM_INHERITANCE_SECOND_KEY");
    }
    else if (inheritance_key_added_count == m_claimInit.inheritance_key_count) {
        return requestDownloadWallet();
    }
    return false;
}

using features::claiming::flows::OnChainClaimingFlow;
void QInheritanceClaiming::onChainProcessErrorCode(int error_code, const QString &error_msg) {
    auto flowMng = app::AppContext::instance()->flowManager();
    auto subMng = app::AppContext::instance()->subScreenManager();
    switch (error_code) {
    case DRACO_CODE::INHERITANCE_801:
        // show popup: Invalid phrase. Please try again
        AppModel::instance()->showToast(0, STR_CPP_115, EWARNING::WarningType::ERROR_MSG);
        break;
    case DRACO_CODE::INHERITANCE_802:
        // show popup: It looks like the original subscription has expired for more than 3 years.
        //             To claim the inheritance, please reactivate the plan first by visiting nunchuk.io/claim.
        emit isExpiredAlert();
        break;
    case DRACO_CODE::INHERITANCE_831:
    case DRACO_CODE::INHERITANCE_803: {
        // show popup: Your inheritance plan has not been activated yet. Please try again later.
        auto flow = flowMng->startFlow<OnChainClaimingFlow>();
        flow->passClaimingError(error_code, error_msg);
        subMng->show(qml::screens::onlinemode::claiming::claiminheritanceplanresult);
    }    break;
    case DRACO_CODE::INHERITANCE_829:
        // show popup: Please come back later
        emit comeBackLaterAlert(error_msg);
        break;
    case DRACO_CODE::INHERITANCE_830:
        // show popup: Security deposit required
        emit securityDepositRequiredAlert(error_msg);
        break;
    default:
        AppModel::instance()->showToast(error_code, "Invalid Magic Phrase", EWARNING::WarningType::EXCEPTION_MSG);
        break;
    }
}

bool QInheritanceClaiming::requestDownloadWallet() {
    bool ret{false};
    QJsonObject output;
    QJsonObject body;
    body["magic"] = m_claimInit.magic;
    body["keys"] = inheritanceKeys();
    ret = Draco::instance()->inheritanceClaimDownloadWallet(body, output);
    DBG_INFO << "body:" << body << "output:" << output;
    if (!ret) {
        int code = output.value("code").toInt();
        QString message = output.value("message").toString();
        onChainProcessErrorCode(code, message);
    } else {
        m_walletInfoClaim = output.value("wallet").toObject();
        QString bsms = m_walletInfoClaim.value("bsms").toString();
        if (!bsms.isEmpty()) {
            QWarningMessage msg;
            mInheritance.wallet = qUtils::ParseWalletDescriptor(bsms, msg);
            mInheritance.wallet.set_name("Inheritance Wallet");
            bridge::nunchukCreateWallet(mInheritance.wallet, true, msg);
            QString wallet_id = QString::fromStdString(mInheritance.wallet.get_id());
            {
                QWalletCached cachedData;
                cachedData.groupId = m_walletInfoClaim.value("group_id").toString();
                cachedData.slug = m_walletInfoClaim.value("slug").toString();
                cachedData.myRole = m_walletInfoClaim.value("role").toString();
                cachedData.status = m_walletInfoClaim.value("status").toString();
                cachedData.backedup = true;
                cachedData.hideFiatCurrency = m_walletInfoClaim.value("hide_fiat_currency").toBool();
                cachedData.registered = true;
                cachedData.isClaimed = true;
                AppSetting::instance()->setWalletCached(wallet_id, cachedData);
            }

            if(msg.type() == (int)EWARNING::WarningType::NONE_MSG){
                QEventProcessor::instance()->sendEvent(E::EVT_ONS_CLOSE_ALL_REQUEST);
                AppModel::instance()->startReloadWallets();
                
            }
            auto wallet = bridge::convertWallet(mInheritance.wallet);
            if (wallet.isNull()) {
                DBG_INFO << "Failed to convert wallet from descriptor";
                return false;
            }
            ServiceSetting::instance()->setWalletInfo(wallet);
            QEventProcessor::instance()->sendEvent(E::EVT_ONS_CLOSE_ALL_REQUEST);
            emit resultClaimInheritanceAlert("CLAIM_INHERITANCE_PLAN_RESULT_SUCCESS");
            auto flowMng = app::AppContext::instance()->flowManager();
            auto flow = flowMng->startFlow<OnChainClaimingFlow>();
            flow->setnunWallet(mInheritance.wallet);
            flow->setmagicWord(m_claimInit.magic);
            flow->setbsms(bsms);                    
            flow->claimStatus();
        } else {
            onChainProcessErrorCode(DRACO_CODE::INHERITANCE_831, "No inheritance plan found");
        }
        
    }
    return ret;
}

nunchuk::SingleSigner QInheritanceClaiming::RecoverAnExistingSeed(const QString &mnemonic) {
    QString fingerprint = qUtils::GetMasterFingerprint(mnemonic.trimmed(), "");
    nunchuk::SingleSigner outSigner("","","","",{}, "", 0,"");
    auto draftWallet = QAssistedDraftWallets::IsByzantine() ? dynamic_cast<QAssistedDraftWallets*>(QGroupWallets::instance()) : dynamic_cast<QAssistedDraftWallets*>(QUserWallets::instance());
    bool ret = draftWallet->checkAndGetSingleSigner(fingerprint, 0, outSigner);
    return outSigner;
}

bool QInheritanceClaiming::requestDownloadWalletViaSeedPhrase(const QString &mnemonic) {
    DBG_INFO << mnemonic;
    runInConcurrent([this, mnemonic]() ->bool{
        bool checkmnemonic = qUtils::CheckMnemonic(mnemonic.trimmed());
        if(checkmnemonic){
            auto signer = RecoverAnExistingSeed(mnemonic.trimmed());
            if (!signer.get_master_fingerprint().empty()) {
                return requestClaimInheritanceFromSigner(signer);
            }
            else {
                AppModel::instance()->setMnemonic("-101");
                AppModel::instance()->showToast(0, STR_CPP_081, EWARNING::WarningType::ERROR_MSG);
            }        
        }
        else{
            AppModel::instance()->setMnemonic("-101");
            AppModel::instance()->showToast(0, STR_CPP_081, EWARNING::WarningType::ERROR_MSG);
        }
        return checkmnemonic;
    },[](bool ret) {
    });
    return false;
}
