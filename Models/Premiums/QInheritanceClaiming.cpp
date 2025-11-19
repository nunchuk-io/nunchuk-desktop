#include "QInheritanceClaiming.h"

#include <QLocale>
#include <QJsonDocument>
#include "Servers/Draco.h"
#include "ViewsEnums.h"
#include "QEventProcessor.h"
#include "bridgeifaces.h"
#include "qUtils.h"
#include "Models/AppModel.h"
#include "AppSetting.h"

QInheritanceClaiming::QInheritanceClaiming()
    : QStateFlow()
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
    m_inheritanceCheckStatus = {};
    m_inheritanceClaimPlan = {};
    m_claimInheritanceCustomAmount = 0;
    // Set default flags
    updateInheritanceCheckStatus("isWithdrawBitcoin", false);
}

QInheritanceClaiming::~QInheritanceClaiming() = default;

void QInheritanceClaiming::clearClaimAnInheritance() {
    clearInheritanceCheckStatus();
    setInheritanceClaimPlan({});
}

bool QInheritanceClaiming::inheritanceCheck(const QString &magic, const QString &environment) {
    QJsonObject result;
    QString errormsg;
    bool ret = Draco::instance()->inheritanceCheck(magic, environment, result, errormsg);
    if (ret) {
        DBG_INFO << result;
        updateInheritanceCheckStatus(result);
        bool is_paid = result["is_paid"].toBool();
        if (!is_paid) {
            emit notPaidAlert();
        }
    }
    return ret;
}

int QInheritanceClaiming::inheritanceDownloadBackup(const QString &magic, const QString &backup_password, const QString &backup_password_two) {
    QStringList backup_keys;
    backup_keys.clear();
    if (!backup_password.isEmpty()) {
        backup_keys.push_back(backup_password);
    }
    if (!backup_password_two.isEmpty()) {
        backup_keys.push_back(backup_password_two);
    }
    DBG_INFO << magic;
    QJsonObject result;
    QString errormsg;
    int response_code = DRACO_CODE::RESPONSE_OK;
    bool ret = Draco::instance()->inheritanceDownloadBackup(magic, backup_keys, response_code, result, errormsg);
    DBG_INFO << ret << errormsg;
    if (ret) {
        QJsonArray keys = result["keys"].toArray();
        master_signers.clear();
        single_signers.clear();
        if (keys.size() != backup_keys.size()) {
            // ERROR: Missing Backup Passwords
            AppModel::instance()->showToast(0, "Missing Backup Passwords", EWARNING::WarningType::ERROR_MSG);
            return DRACO_CODE::INHERITANCE_801;
        }

        auto downloadKey = [this](const QJsonObject &key, const QString &backup_key) {
            QWarningMessage msg;
            QString base64 = key["key_backup_base64"].toString();
            QByteArray ba;
            ba.append(base64);
            QByteArray base64bin = QByteArray::fromBase64(ba);
            std::vector<unsigned char> base64vec(base64bin.begin(), base64bin.end());
            QString key_name = key["key_name"].toString();
            QString derivation_path = key["derivation_path"].toString();
            auto master_signer_ptr = bridge::ImportBackupKey(base64vec, backup_key, key_name, false, msg);
            if (master_signer_ptr) {
                msg.resetWarningMessage();
                auto master_signer = master_signer_ptr->originMasterSigner();
                // update tag
                master_signer.set_tags({nunchuk::SignerTag::INHERITANCE});
                bridge::UpdateMasterSigner(master_signer, msg);
                master_signers.push_back(master_signer);
                single_signers.push_back(bridge::GetSignerFromMasterSigner(QString::fromStdString(master_signer.get_id()), derivation_path, msg));
            }
        };

        for (QJsonValue js : keys) {
            QJsonObject key = js.toObject();
            // Try with all backup_keys (Backup Password)
            for (auto backup_key : backup_keys) {
                downloadKey(key, backup_key);
            }
        }
        if (keys.size() != (int)master_signers.size()) {
            // ERROR: Backup Password is incorrect
            // Remove master_singer
            AppModel::instance()->showToast(0, STR_CPP_115, EWARNING::WarningType::ERROR_MSG);
            for (auto m : master_signers) {
                bridge::nunchukDeleteMasterSigner(QString::fromStdString(m.get_id()));
            }
            return DRACO_CODE::INHERITANCE_801;
        }

        inheritanceClaimStatus(magic);
    } else {
        switch (response_code) {
        case DRACO_CODE::INHERITANCE_801:
            // show popup: Invalid phrase. Please try again
            AppModel::instance()->showToast(0, STR_CPP_115, EWARNING::WarningType::ERROR_MSG);
            break;
        case DRACO_CODE::INHERITANCE_802:
            // show popup: It looks like the original subscription has expired for more than 3 years.
            //             To claim the inheritance, please reactivate the plan first by visiting nunchuk.io/claim.
            emit isExpiredAlert();
            break;
        case DRACO_CODE::INHERITANCE_803:
            // show popup: Your inheritance plan has not been activated yet. Please try again later.
            emit hasNotBeenActivatedYetAlert();
            break;
        case DRACO_CODE::INHERITANCE_829:
            // show popup: Please come back later
            emit comeBackLaterAlert(errormsg);
            break;
        case DRACO_CODE::INHERITANCE_830:
            // show popup: Security deposit required
            emit securityDepositRequiredAlert(errormsg);
            break;
        default:
            AppModel::instance()->showToast(response_code, errormsg, EWARNING::WarningType::EXCEPTION_MSG);
            break;
        }
    }
    return response_code;
}

bool QInheritanceClaiming::inheritanceClaimRequest(const nunchuk::Wallet wallet, const nunchuk::Transaction txSigned, const QString &magic) {
    QJsonObject result;
    QString errormsg;
    bool ret = Draco::instance()->inheritanceClaimRequest(magic, QString::fromStdString(txSigned.get_psbt()), result, errormsg);
    DBG_INFO << QString::fromStdString(txSigned.get_psbt());
    DBG_INFO << result;
    if (ret) {
        // HANDLE RESULT
        QJsonObject transaction = result["transaction"].toObject();
        QString status = transaction.value("status").toString();
        QString psbt = transaction.value("psbt").toString();
        QString wallet_local_id = transaction.value("wallet_local_id").toString();
        if (status == "PENDING_CONFIRMATION" || status == "CONFIRMED") {
            QWarningMessage _msg;
            bridge::nunchukImportPsbt(wallet_local_id, psbt, _msg);
            QString id = transaction.value("id").toString();
            QString hex = transaction.value("hex").toString();
            QString reject_msg = transaction.value("reject_msg").toString();
            std::string walletid = "";
            try {
                walletid = wallet.get_id();
            }
            catch (const nunchuk::BaseException &ex) {
                DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
            }
            catch (std::exception &e) {
                DBG_INFO << "THROW EXCEPTION" << e.what();
            }
            bridge::nunchukUpdateTransaction(QString::fromStdString(walletid), QString::fromStdString(txSigned.get_txid()), id, hex, reject_msg, _msg);
        }
        QJsonObject current = m_inheritanceClaimPlan;
        current["balance"] = 0;
        setInheritanceClaimPlan(current); // reset
        emit inheritanceInfoChanged();
        AppModel::instance()->requestCreateUserWallets();
    }
    return ret;
}

bool QInheritanceClaiming::inheritanceClaimStatus(const QString &magic) {
    QWarningMessage msg;
    QJsonObject body;
    body["magic"] = magic;

    QJsonObject data;
    data["nonce"] = Draco::instance()->randomNonce();
    data["body"] = body;
    QJsonDocument doc(data);
    QString user_data(doc.toJson());
    QString messages_to_sign = qUtils::GetHealthCheckMessage(user_data, msg); // user_data in json string

    QStringList authos;
    for (auto signer : single_signers) {
        QString signature = bridge::SignHealthCheckMessage(signer, messages_to_sign, msg);
        QString authorization = qUtils::CreateRequestToken(signature, QString::fromStdString(signer.get_master_fingerprint()), msg);
        authos.append(authorization);
    }

    QJsonObject result;
    QString errormsg;
    bool ret = Draco::instance()->inheritanceClaimStatus(data, authos, result, errormsg);
    DBG_INFO << result;
    if (ret) {
        // HANDLE RESULT
        setInheritanceClaimPlan(result);
    }
    return ret;
}

bool QInheritanceClaiming::inheritanceCreateTx(const QJsonObject &data, const QStringList &authos) {
    QJsonObject result;
    QString errormsg;
    DBG_INFO << data << authos;
    bool ret = Draco::instance()->inheritanceCreateTx(data, authos, result, errormsg);
    if (ret) {
        QJsonObject transaction = result["transaction"].toObject();
        QString psbt = transaction["psbt"].toString();
        QString sub_amount = QString("%1").arg(result["sub_amount"].toDouble());
        QString fee = QString("%1").arg(result["fee"].toDouble());
        QString fee_rate = QString("%1").arg(result["fee_rate"].toDouble());
        int change_pos = result["change_pos"].toInt();
        qint64 tx_fee = qUtils::QAmountFromValue(fee);               // fee in BTC
        qint64 tx_fee_rate = qUtils::QAmountFromValue(fee_rate);     // fee_rate in BTC
        qint64 tx_sub_amount = qUtils::QAmountFromValue(sub_amount); // sub amount in BTC
        // HANDLE RESULT
        mInheritance.wallet = nunchuk::Wallet("", 1, 1, single_signers, nunchuk::AddressType::NATIVE_SEGWIT, false, 0, true);
        QWarningMessage msg;
        mInheritance.tx = qUtils::DecodeTx(mInheritance.wallet, psbt, tx_sub_amount, tx_fee, tx_fee_rate, msg);
        if ((int)EWARNING::WarningType::NONE_MSG == msg.type()) {
            mInheritance.tx.set_change_index(change_pos); // set change index to i

            std::string walletid = "";
            try {
                walletid = mInheritance.wallet.get_id();
            }
            catch (const nunchuk::BaseException &ex) {
                DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
            }
            catch (std::exception &e) {
                DBG_INFO << "THROW EXCEPTION" << e.what();
            }

            QTransactionPtr trans = bridge::convertTransaction(mInheritance.tx, QString::fromStdString(walletid));
            if (trans) {
                trans->setIsClaimTx(true);
                AppModel::instance()->setTransactionInfo(trans);
                QList<uint> states = QEventProcessor::instance()->getCurrentStates();
                if (!states.isEmpty() && states.last() == E::STATE_ID_SCR_INHERITANCE_WITHDRAW_BALANCE) {
                    DBG_INFO << "Entry here ";
                    QEventProcessor::instance()->sendEvent(E::EVT_INHERITANCE_CONFIRM_TRANSACTION_REQUEST);
                }
            }
        } else {
            AppModel::instance()->showToast(msg.code(), msg.what(), (EWARNING::WarningType)msg.type());
        }
    }
    return ret;
}

void QInheritanceClaiming::setInheritanceAddress(const QString &to_wallet_id) {
    QWalletPtr ptr = AppModel::instance()->walletList()->getWalletById(to_wallet_id);
    if (!ptr.isNull()) {
        ServiceSetting::instance()->setWalletInfo(ptr);
        QString wallet_id = ptr->walletId();
        QStringList addrs = bridge::nunchukGetUnusedAddresses(wallet_id, false);
        if (addrs.size() > 0) {
            mInheritance.m_destinationAddress = addrs.first();
        } else {
            mInheritance.m_destinationAddress = bridge::nunchukGenNewAddresses(wallet_id, false);
        }
        DBG_INFO << mInheritance.m_destinationAddress;
        AppModel::instance()->setWalletInfo(ptr);
    }
}

void QInheritanceClaiming::setInheritanceAddressNewTransaction(const QString &address) {
    mInheritance.m_destinationAddress = address;
}

bool QInheritanceClaiming::inheritanceCreateDraftTransaction(double fee_rate, bool anti_fee_sniping) {
    const constexpr auto NONE_MSG = (int)EWARNING::WarningType::NONE_MSG;
    int64_t amount = m_claimInheritanceCustomAmount;
    QJsonObject inheritance = m_inheritanceClaimPlan["inheritance"].toObject();

    QJsonObject body;
    body["magic"] = inheritance["magic"].toString();
    body["address"] = mInheritance.m_destinationAddress;
    body["fee_rate"] = qUtils::QValueFromAmount(fee_rate);
    if (anti_fee_sniping) {
        body["anti_fee_sniping"] = anti_fee_sniping;
    }
    if (amount > 0) {
        body["amount"] = qUtils::QValueFromAmount(amount);
    }

    QJsonObject data;
    data["nonce"] = Draco::instance()->randomNonce();
    data["body"] = body;
    QJsonDocument doc(data);
    QString user_data(doc.toJson());
    QWarningMessage msg;
    QString messages_to_sign = qUtils::GetHealthCheckMessage(user_data, msg);
    QStringList authos;
    for (auto signer : single_signers) {
        QString signature = bridge::SignHealthCheckMessage(signer, messages_to_sign, msg);
        QString authorization = qUtils::CreateRequestToken(signature, QString::fromStdString(signer.get_master_fingerprint()), msg);
        authos.append(authorization);
    }

    if (NONE_MSG != msg.type()) {
        AppModel::instance()->showToast(msg.code(), msg.what(), (EWARNING::WarningType)msg.type());
        return false;
    }
    return inheritanceCreateTx(data, authos);
}

bool QInheritanceClaiming::inheritanceSignTransaction() {
    QJsonObject inheritance = m_inheritanceClaimPlan["inheritance"].toObject();
    DBG_INFO << inheritance;
    QWarningMessage msgwarning;
    nunchuk::Transaction tx = mInheritance.tx;
    for (auto master_signer : master_signers) {
        tx = bridge::SignTransaction(mInheritance.wallet, tx, nunchuk::Device(master_signer.get_id()), msgwarning);
    }

    if ((int)EWARNING::WarningType::NONE_MSG == msgwarning.type()) {
        QTransactionPtr trans = bridge::convertTransaction(tx, "");
        if (trans) {
            trans->setIsClaimTx(true);
            AppModel::instance()->setTransactionInfo(trans);
        }
        return inheritanceClaimRequest(mInheritance.wallet, tx, inheritance["magic"].toString());
    } else {
        AppModel::instance()->showToast(msgwarning.code(), msgwarning.what(), (EWARNING::WarningType)msgwarning.type());
    }
    return false;
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

qint64 QInheritanceClaiming::claimInheritanceCustomAmount() const {
    return m_claimInheritanceCustomAmount;
}

void QInheritanceClaiming::setClaimInheritanceCustomAmount(qint64 amountSats) {
    if (m_claimInheritanceCustomAmount == amountSats) return;
    m_claimInheritanceCustomAmount = amountSats;
    emit claimInheritanceCustomAmountChanged();
}

QVariant QInheritanceClaiming::inheritanceCheckStatus() const {
    return QVariant::fromValue(m_inheritanceCheckStatus);
}

void QInheritanceClaiming::updateInheritanceCheckStatus(const QJsonObject &status) {
    for (auto it = status.constBegin(); it != status.constEnd(); ++it) {
        m_inheritanceCheckStatus[it.key()] = it.value();
    }
    emit inheritanceCheckStatusChanged();
}

void QInheritanceClaiming::updateInheritanceCheckStatus(const QString &key, const QVariant &value) {
    QJsonValue jsonValue = QJsonValue::fromVariant(value);
    if (m_inheritanceCheckStatus[key] == jsonValue)
        return;

    m_inheritanceCheckStatus[key] = jsonValue;
    emit inheritanceCheckStatusChanged();
}

void QInheritanceClaiming::clearInheritanceCheckStatus() {
    m_inheritanceCheckStatus = {};
    updateInheritanceCheckStatus("isWithdrawBitcoin", false);
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

QVariant QInheritanceClaiming::inheritanceClaimInit(const QString& magic) {
    QJsonObject result;
    QString errormsg;
    bool ret = Draco::instance()->inheritanceClaimInit(magic, result, errormsg);
    DBG_INFO << result;
    if (ret) {
        return QVariant::fromValue(result);
    } else {
        int response_code = result.value("code").toInt();
        switch (response_code) {
        case DRACO_CODE::INHERITANCE_801:
            // show popup: Invalid phrase. Please try again
            AppModel::instance()->showToast(0, STR_CPP_115, EWARNING::WarningType::ERROR_MSG);
            break;
        case DRACO_CODE::INHERITANCE_802:
            // show popup: It looks like the original subscription has expired for more than 3 years.
            //             To claim the inheritance, please reactivate the plan first by visiting nunchuk.io/claim.
            emit isExpiredAlert();
            break;
        case DRACO_CODE::INHERITANCE_803:
            // show popup: Your inheritance plan has not been activated yet. Please try again later.
            emit hasNotBeenActivatedYetAlert();
            break;
        case DRACO_CODE::INHERITANCE_829:
            // show popup: Please come back later
            emit comeBackLaterAlert(errormsg);
            break;
        case DRACO_CODE::INHERITANCE_830:
            // show popup: Security deposit required
            emit securityDepositRequiredAlert(errormsg);
            break;
        default:
            AppModel::instance()->showToast(response_code, "Invalid Magic Phrase", EWARNING::WarningType::EXCEPTION_MSG);
            break;
        }
        return QVariant::fromValue(result);
    }
}