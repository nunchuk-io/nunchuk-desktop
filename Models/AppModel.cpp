/**************************************************************************
 * This file is part of the Nunchuk software (https://nunchuk.io/)        *
 * Copyright (C) 2020-2022 Enigmo								          *
 * Copyright (C) 2022 Nunchuk								              *
 *                                                                        *
 * This program is free software; you can redistribute it and/or          *
 * modify it under the terms of the GNU General Public License            *
 * as published by the Free Software Foundation; either version 3         *
 * of the License, or (at your option) any later version.                 *
 *                                                                        *
 * This program is distributed in the hope that it will be useful,        *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 * GNU General Public License for more details.                           *
 *                                                                        *
 * You should have received a copy of the GNU General Public License      *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
 *                                                                        *
 **************************************************************************/
#include "AppModel.h"
#include "AppSetting.h"
#include "bridgeifaces.h"
#include "QQuickViewer.h"
#include "ViewsEnums.h"
#include "Draco.h"
#include "localization/STR_CPP.h"
#include <QTimer>
#include "Chats/matrixbrigde.h"
#include "utils/enumconverter.hpp"

AppModel::AppModel(): inititalized_{false},
    walletList_(QWalletListModelPtr(new WalletListModel())),
    deviceList_(QDeviceListModelPtr(new DeviceListModel())),
    masterSignerList_(QMasterSignerListModelPtr(new MasterSignerListModel())),
    remoteSignerList_(QSingleSignerListModelPtr(new SingleSignerListModel())),
    masterSignerInfo_(QMasterSignerPtr(new MasterSigner())),
    singleSignerInfo_(QSingleSignerPtr(new SingleSigner())),
    walletInfo_(QWalletPtr(new Wallet())),
    transactionInfo_(QTransactionPtr(new Transaction())),
    transactionReplaceInfo_(NULL),
    transactionPending_(QTransactionListModelPtr(new TransactionListModel())),
    utxoList_(QUTXOListModelPtr(new UTXOListModel())),
    utxoInfo_(QUTXOPtr(new UTXO())),
    walletListCurrentIndex_(-1),
    chainTip_(0), addSignerStep_(-1), addSignerPercentage_(0), txidReplacing_(""),
    fastestFee_(1000), halfHourFee_(1000), hourFee_(1000), minFee_(1000),
    addressBalance_(0),mnemonic_(""),
    softwareSignerDeviceList_(QDeviceListModelPtr(new DeviceListModel())),
    nunchukMode_(LOCAL_MODE),
    m_tabIndex((int)ENUNCHUCK::TabSelection::CHAT_TAB),
    warningMessage_(QWarningMessagePtr(new QWarningMessage())),
    exchangeRates_(0), lasttime_checkEstimatedFee_(QDateTime::currentDateTime()),
    m_primaryKey(NULL),
    newKeySignMessage_("")
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
    connect(&timerRefreshHealthCheck_, SIGNAL(timeout()), this, SLOT(timerHealthCheckTimeHandle()));
    timerRefreshHealthCheck_.start(60000); // Every 1'
    connect(&timerFeeRates_, SIGNAL(timeout()), this, SLOT(timerFeeRatesHandle()));
    timerFeeRates_.start(300000); // Every 5'
    connect(&timerCheckAuthorized_, SIGNAL(timeout()), this, SLOT(timerCheckAuthorizedHandle()));
    qrExported_.clear();
    suggestMnemonics_.clear();

    connect(qApp, &QCoreApplication::aboutToQuit, this, [] {
        DBG_INFO << "APPLICATION ABOUT TO QUIT";
        bridge::stopNunchuk();
    });
}

AppModel::~AppModel(){
    timerRefreshHealthCheck_.stop();
    timerCheckAuthorized_.stop();
    walletList_.clear();
    deviceList_.clear();
    masterSignerList_.clear();
    remoteSignerList_.clear();
    masterSignerInfo_.clear();
    singleSignerInfo_.clear();
    newWalletInfo_.clear();
    walletInfo_.clear();
    transactionInfo_.clear();
    transactionReplaceInfo_.clear();
    utxoList_.clear();
    utxoInfo_.clear();
    destinationList_.clear();
    disconnect();
}

QStringList AppModel::getUserWallets() const
{
    return mUserWallets;
}

QString AppModel::newKeySignMessage() const
{
    return newKeySignMessage_;
}

QString AppModel::newKeySignMessageSHA256() const
{
    QByteArray bytes = QCryptographicHash::hash(newKeySignMessage_.toUtf8(), QCryptographicHash::Sha256);
    return QString(bytes.toHex());
}

void AppModel::setNewKeySignMessage(const QString &value)
{
    if(newKeySignMessage_ != value){
        newKeySignMessage_ = value;
        emit newKeySignMessageChanged();
    }
}

QString AppModel::lasttimeCheckEstimatedFee() const
{
    QDateTime in = lasttime_checkEstimatedFee_;
    QDateTime today = QDateTime::currentDateTime();
    qint64 secs = in.secsTo(today);
    const qint64 DAY = 86400;
    qint64 days = secs / DAY;
    QTime t = QTime(0,0).addSecs(secs % DAY);
    if(abs(days) > 0){
        return QString("%1 %2 ago").arg(abs(days)).arg(abs(days) > 1 ? "days" : "day");
    }
    else{
        if(t.hour() > 0){
            if(t.minute() > 0){
                return QString("%1 %2 %3 %4 ago").arg(t.hour()).arg(t.hour() > 1 ? "hours" : "hour").arg(t.minute()).arg(t.minute() > 1 ? "minutes" : "minute");
            }
            else{
                return QString("%1 %2ago").arg(t.hour()).arg(t.hour() > 1 ? "hours" : "hour");
            }
        }
        else{
            if(t.minute() == 0 && t.second() < 10){
                return QString("just now");
            }
            else if(t.minute() == 0 && t.second() >= 10){
                return QString("few seconds ago");
            }
            else{
                return QString("%1 %2 ago").arg(t.minute()).arg(t.minute() > 1 ? "minutes" : "minute");
            }
        }
    }
}

void AppModel::setLasttimeCheckEstimatedFee(const QDateTime &lasttime_checkEstimatedFee)
{
    if(lasttime_checkEstimatedFee_ == lasttime_checkEstimatedFee) {return;}
    lasttime_checkEstimatedFee_ = lasttime_checkEstimatedFee;
    emit lasttimeCheckEstimatedFeeChanged();
}

void AppModel::startCheckAuthorize()
{
    timerCheckAuthorized_.stop();
    timerCheckAuthorized_.start(120000); // Every 2'
}

void AppModel::stopCheckAuthorize()
{
    timerCheckAuthorized_.stop();
}

bool AppModel::makeInstanceForAccount(const QVariant msg, const QString &dbPassphrase)
{
    bool ret = true;
    qApp->setOverrideCursor(QCursor(Qt::WaitCursor));
    QWarningMessage nunchukMsg;
    bridge::nunchukSetCurrentMode(ONLINE_MODE);
    QString account = Draco::instance()->Uid();
    bridge::nunchukMakeInstanceForAccount(account,
                                          dbPassphrase,
                                          nunchukMsg);
    if((int)EWARNING::WarningType::NONE_MSG == nunchukMsg.type()){
        ret = true;
        QString device_id = QString("%1%2").arg(Draco::instance()->deviceId()).arg(Draco::instance()->chatId());
        Draco::instance()->getMe();
        Draco::instance()->getCurrentUserSubscription();
        CLIENT_INSTANCE->requestLogin();
        CLIENT_INSTANCE->saveStayLoggedInData();
        QQuickViewer::instance()->sendEvent(E::EVT_GOTO_HOME_CHAT_TAB);

        QWarningMessage matrixMsg;
        matrixbrigde::makeMatrixInstance(account,
                                         device_id,
                                         matrixMsg);
        if((int)EWARNING::WarningType::NONE_MSG != matrixMsg.type()){
            int code = matrixMsg.code();
            QString what = matrixMsg.what();
            int type = matrixMsg.type();
            QTimer::singleShot(500, [code, what, type]() {
                AppModel::instance()->showToast(code,
                                                what,
                                                (EWARNING::WarningType)type,
                                                "ERROR");
            });
        }
        AppModel::instance()->requestInitialData();
    }
    else if((int)EWARNING::WarningType::EXCEPTION_MSG == nunchukMsg.type() && nunchuk::NunchukException::INVALID_PASSPHRASE == nunchukMsg.code()){
        DBG_INFO << "COULD NOT MAKE NUNCHUCK INSTANCE" << nunchukMsg.code();
        ret = false;
        QList<uint> states = QQuickViewer::instance()->getCurrentStates();
        if(!states.isEmpty() && states.last() != (uint)E::STATE_ID_SCR_UNLOCK_DB)
        {
            QTimer::singleShot(500,[msg](){
                QQuickViewer::instance()->sendEvent(E::EVT_LOGIN_DB_REQUEST, msg);
            });
        }
        else{
            int code = nunchukMsg.code();
            QString what = nunchukMsg.what();
            int type = nunchukMsg.type();
            QTimer::singleShot(500, [code, what, type]() {
                AppModel::instance()->showToast(code,
                                                what,
                                                (EWARNING::WarningType)type,
                                                "ERROR");
            });
        }
    }
    else{
        DBG_INFO << "COULD NOT MAKE NUNCHUCK INSTANCE" << nunchukMsg.code();
        ret = false;
        int code = nunchukMsg.code();
        QString what = nunchukMsg.what();
        int type = nunchukMsg.type();
        QTimer::singleShot(500, [code, what, type]() {
            AppModel::instance()->showToast(code,
                                            what,
                                            (EWARNING::WarningType)type,
                                            "ERROR");
        });
    }
    qApp->restoreOverrideCursor();
    return ret;
}

bool AppModel::makeNunchukInstanceForAccount(const QVariant msg, const QString &dbPassphrase)
{
    bool ret = true;
    qApp->setOverrideCursor(QCursor(Qt::WaitCursor));
    QWarningMessage warningmsg;
    bridge::nunchukSetCurrentMode(ONLINE_MODE);
    QString account = Draco::instance()->Uid();
    bridge::nunchukMakeInstanceForAccount(account,
                                          dbPassphrase,
                                          warningmsg);
    if((int)EWARNING::WarningType::NONE_MSG == warningmsg.type()){
        ret = true;
        AppModel::instance()->requestInitialData();
    }
    else if((int)EWARNING::WarningType::EXCEPTION_MSG == warningmsg.type() && nunchuk::NunchukException::INVALID_PASSPHRASE == warningmsg.code()){
        DBG_INFO << "COULD NOT MAKE NUNCHUCK INSTANCE" << warningmsg.code();
        ret = false;
        QList<uint> states = QQuickViewer::instance()->getCurrentStates();
        if(!states.isEmpty() && states.last() != (uint)E::STATE_ID_SCR_UNLOCK_DB)
        {
            QTimer::singleShot(500,[msg](){
                QQuickViewer::instance()->sendEvent(E::EVT_LOGIN_DB_REQUEST, msg);
            });
        }
        else{
            int code = warningmsg.code();
            QString what = warningmsg.what();
            int type = warningmsg.type();
            QTimer::singleShot(500, [code, what, type]() {
                AppModel::instance()->showToast(code,
                                                what,
                                                (EWARNING::WarningType)type,
                                                "ERROR");
            });
        }
    }
    else{
        DBG_INFO << "COULD NOT MAKE NUNCHUCK INSTANCE" << warningmsg.code();
        ret = false;
        int code = warningmsg.code();
        QString what = warningmsg.what();
        int type = warningmsg.type();
        QTimer::singleShot(500, [code, what, type]() {
            AppModel::instance()->showToast(code,
                                            what,
                                            (EWARNING::WarningType)type,
                                            "ERROR");
        });
    }
    qApp->restoreOverrideCursor();
    return ret;
}

void AppModel::makeMatrixInstanceForAccount()
{
    qApp->setOverrideCursor(QCursor(Qt::WaitCursor));
    QString account = Draco::instance()->Uid();
    QString device_id = QString("%1%2").arg(Draco::instance()->deviceId()).arg(Draco::instance()->chatId());
    QWarningMessage warningmsg2;
    matrixbrigde::makeMatrixInstance(account,
                                     device_id,
                                     warningmsg2);
    if((int)EWARNING::WarningType::NONE_MSG == warningmsg2.type()){
        CLIENT_INSTANCE->requestLogin();
        CLIENT_INSTANCE->saveStayLoggedInData();
    }
    else {
        int code = warningmsg2.code();
        QString what = warningmsg2.what();
        int type = warningmsg2.type();
        QTimer::singleShot(500, [code, what, type]() {
            AppModel::instance()->showToast(code,
                                            what,
                                            (EWARNING::WarningType)type,
                                            "ERROR");
        });
    }
    qApp->restoreOverrideCursor();
}

bool AppModel::makeNunchukInstance(const QVariant makeInstanceData, const QString &dbPassphrase)
{
    qApp->setOverrideCursor(QCursor(Qt::WaitCursor));
    bool ret = false;
    QWarningMessage warningmsg;
    bridge::nunchukMakeInstance(dbPassphrase, warningmsg);
    if((int)EWARNING::WarningType::NONE_MSG == warningmsg.type()){
        AppModel::instance()->requestInitialData();
        ret = true;
    }
    else if((int)EWARNING::WarningType::EXCEPTION_MSG == warningmsg.type() && nunchuk::NunchukException::INVALID_PASSPHRASE == warningmsg.code()){
        ret = false;
        QList<uint> states = QQuickViewer::instance()->getCurrentStates();
        if(!states.isEmpty() && states.last() != (uint)E::STATE_ID_SCR_UNLOCK_DB)
        {
            QQuickViewer::instance()->sendEvent(E::EVT_LOGIN_DB_REQUEST, makeInstanceData);
        }
        else{
            int code = warningmsg.code();
            QString what = warningmsg.what();
            int type = warningmsg.type();
            QTimer::singleShot(500, [code, what, type]() {
                AppModel::instance()->showToast(code,
                                                what,
                                                (EWARNING::WarningType)type,
                                                "ERROR");
            });
        }
    }
    else{
        ret = false;
        int code = warningmsg.code();
        QString what = warningmsg.what();
        int type = warningmsg.type();
        QTimer::singleShot(500, [code, what, type]() {
            AppModel::instance()->showToast(code,
                                            what,
                                            (EWARNING::WarningType)type,
                                            "ERROR");
        });
    }
    qApp->restoreOverrideCursor();
    return ret;
}

void AppModel::loginNunchuk(QVariant msg)
{
    qApp->setOverrideCursor(QCursor(Qt::WaitCursor));
    QMap<QString,QVariant> maps = msg.toMap();
    QString emailInput = maps.value("email").toString();
    QString passwordInput = maps.value("password").toString();
    Draco::instance()->singin(emailInput, passwordInput);
    qApp->restoreOverrideCursor();
}

void AppModel::create_account(QVariant msg)
{
    QMap<QString,QVariant> maps = msg.toMap();
    QString nameInput = maps.value("name").toString();
    QString emailInput =  maps.value("email").toString();
    Draco::instance()->createAccount(nameInput, emailInput);
}

int AppModel::tabIndex() const
{
    return m_tabIndex;
}

void AppModel::setTabIndex(int tabIndex)
{
    if(m_tabIndex == tabIndex) return;
    m_tabIndex = tabIndex;
    emit tabIndexChanged();
}

int AppModel::nunchukMode() const
{
    return nunchukMode_;
}

void AppModel::setNunchukMode(int nunchukMode)
{
    nunchukMode_ = nunchukMode;
    emit nunchukModeChanged();
}

bool AppModel::inititalized() const
{
    return inititalized_[nunchukMode_];
}

void AppModel::setInititalized(bool inititalized)
{
    inititalized_[nunchukMode_] = inititalized;
}

bool AppModel::limitSoftwareSigner() const
{
#if defined(ENABLE_LIMIT)
    return true;
#else
    return false;
#endif
}

QStringList AppModel::getSuggestMnemonics() const
{
    return suggestMnemonics_;
}

void AppModel::setSuggestMnemonics(const QStringList &suggestMnemonics)
{
    suggestMnemonics_ = suggestMnemonics;
    emit suggestMnemonicsChanged();
}

QString AppModel::getMnemonic() const
{
    return mnemonic_;
}

void AppModel::setMnemonic(const QString &mnemonic)
{
    if(mnemonic_ != mnemonic){
        mnemonic_ = mnemonic;
        emit mnemonicChanged();
    }
}

DeviceListModel *AppModel::softwareSignerDeviceList() const
{
    return softwareSignerDeviceList_.data();
}

QDeviceListModelPtr AppModel::softwareSignerDeviceListPtr() const
{
    return softwareSignerDeviceList_;
}

void AppModel::setSoftwareSignerDeviceList(const QDeviceListModelPtr &value)
{
    softwareSignerDeviceList_ = value;
    emit softwareSignerDeviceListChanged();
}

QString AppModel::hourFeeUSD() const
{
    double exRates = this->getExchangeRates()/100000000;
    double fee = (double)hourFee_/1000;
    double feeusd = exRates*fee*140;
    QLocale locale(QLocale::English);
    return locale.toString(feeusd, 'f', 2);
}

QString AppModel::hourFee() const
{    
    double fee = (double)hourFee_/1000;
    QLocale locale(QLocale::English);
    return locale.toString(fee, 'f', 2);
}

void AppModel::setHourFee(qint64 fee)
{
    if(hourFee_ != fee){
        hourFee_ = fee;
        emit hourFeeChanged();
    }
}

QString AppModel::minFeeUSD() const
{
    double exRates = this->getExchangeRates()/100000000;
    double fee = (double)minFee_/1000;
    double feeusd = exRates*fee*140;
    QLocale locale(QLocale::English);
    return locale.toString(feeusd, 'f', 2);
}

QString AppModel::minFee() const
{
    double fee = (double)minFee_/1000;
    QLocale locale(QLocale::English);
    return locale.toString(fee, 'f', 2);
}

void AppModel::setMinFee(qint64 fee)
{
    if(minFee_ != fee){
        minFee_ = fee;
        emit minFeeChanged();
    }
}

void AppModel::checkDeviceUsableToSign()
{
    if( transactionInfo() && transactionInfo()->singleSignersAssigned()){
        if(deviceList()){
            transactionInfo()->singleSignersAssigned()->resetSignerReadyToSign();
            for (QDevicePtr signer : deviceList()->fullList()) {
                transactionInfo()->singleSignersAssigned()->updateSignerReadyToSign(signer.data()->masterFingerPrint());
            }
        }
    }
}

void AppModel::checkDeviceUsableToAdd()
{
    if(deviceList() && masterSignerList()){
        deviceList()->resetUsableToAdd();
        for (int i = 0; i < deviceList()->rowCount(); i++) {
            QDevicePtr dv = deviceList()->getDeviceByIndex(i);
            if(dv){
                bool usable = masterSignerList()->containsFingerPrint(dv.data()->masterFingerPrint());
                deviceList()->updateUsableToAdd(dv.data()->masterFingerPrint(), !usable);
            }
        }
    }
}

void AppModel::resetSignersChecked()
{
    if(masterSignerList_){
        masterSignerList_.data()->resetUserChecked();
    }
    if(remoteSignerList_){
        remoteSignerList_.data()->resetUserChecked();
    }
}

QString AppModel::halfHourFeeUSD() const
{
    double exRates = this->getExchangeRates()/100000000;
    double fee = (double)halfHourFee_/1000;
    double feeusd = exRates*fee*140;
    QLocale locale(QLocale::English);
    return locale.toString(feeusd, 'f', 2);
}

QString AppModel::halfHourFee() const
{
    double fee = (double)halfHourFee_/1000;
    QLocale locale(QLocale::English);
    return locale.toString(fee, 'f', 2);
}

void AppModel::setHalfHourFee(qint64 fee)
{
    if(halfHourFee_ != fee){
        halfHourFee_ = fee;
        emit halfHourFeeChanged();
    }
}

QString AppModel::fastestFeeUSD() const
{
    double exRates = this->getExchangeRates()/100000000;
    double fee = (double)fastestFee_/1000;
    double feeusd = exRates*fee*140;
    QLocale locale(QLocale::English);
    return locale.toString(feeusd, 'f', 2);
}

QString AppModel::fastestFee() const
{
    double fee = (double)fastestFee_/1000;
    QLocale locale(QLocale::English);
    return locale.toString(fee, 'f', 2);
}

void AppModel::setFastestFee(qint64 fee)
{
    if(fastestFee_ != fee){
        fastestFee_ = fee;
        emit fastestFeeChanged();
    }
}

QString AppModel::getTxidReplacing() const
{
    return txidReplacing_;
}

void AppModel::setTxidReplacing(const QString &txidReplacing)
{
    txidReplacing_ = txidReplacing;
}

int AppModel::getAddSignerPercentage() const
{
    return addSignerPercentage_;
}

void AppModel::setAddSignerPercentage(int addSignerPercentage)
{
    if(addSignerPercentage_ != addSignerPercentage){
        addSignerPercentage_ = addSignerPercentage;
        emit addSignerPercentageChanged();
    }
}

int AppModel::getAddSignerStep() const
{
    return addSignerStep_;
}

void AppModel::setAddSignerStep(int addSignerStep)
{
    if(addSignerStep_ != addSignerStep){
        addSignerStep_ = addSignerStep;
        emit addSignerStepChanged();
    }
}

QTransactionPtr AppModel::getTransactionReplaceInfo() const
{
    return transactionReplaceInfo_;
}

void AppModel::setTransactionReplaceInfo(const QTransactionPtr &transactionReplaceInfo)
{
    transactionReplaceInfo_ = transactionReplaceInfo;
}

UTXO *AppModel::utxoInfo() const
{
    return utxoInfo_.data();
}

QUTXOPtr AppModel::utxoInfoPtr() const
{
    return utxoInfo_;
}

void AppModel::setUtxoInfo(const QUTXOPtr &utxoInfo)
{
    utxoInfo_ = utxoInfo;
    emit utxoInfoChanged();
}

int AppModel::chainTip() const
{
    return chainTip_;
}

void AppModel::setChainTip(int chainTip)
{
    if(chainTip_ != chainTip){
        chainTip_ = chainTip;
        emit chainTipChanged();
    }
}

int AppModel::walletListCurrentIndex() const
{
    return walletListCurrentIndex_;
}

void AppModel::setWalletListCurrentIndex(int walletListCurrentIndex)
{
    DBG_INFO << walletListCurrentIndex;
    if(walletListCurrentIndex == -1){
        walletListCurrentIndex_ = 0;
    }
    else{
        walletListCurrentIndex_ = walletListCurrentIndex;
    }
    setWalletInfoByIndex(walletListCurrentIndex_);
    if(walletInfo() && walletInfo()->singleSignersAssigned()){
        for (int j = 0; j < walletInfo()->singleSignersAssigned()->rowCount(); j++) {
            QSingleSignerPtr signer = walletInfo()->singleSignersAssigned()->getSingleSignerByIndex(j);
            if(signer && (int)ENUNCHUCK::SignerType::HARDWARE == signer.data()->signerType()){
                if(masterSignerList() && masterSignerList()->hardwareContainsFingerPrint(signer.data()->masterFingerPrint())){
                    walletInfo()->setContainsHWSigner(true);
                    break;
                }
            }
        }
        QString wallet_id = walletInfo()->id();
        QtConcurrent::run([wallet_id]() {
            bridge::nunchukSetSelectedWallet(wallet_id);
        });
    }
    emit walletListCurrentIndexChanged();
}

AppModel *AppModel::instance() {
    static AppModel mInstance;
    return &mInstance;
}

void AppModel::requestInitialData()
{
    if(ONLINE_MODE == bridge::nunchukCurrentMode()){
        requestCreateUserWallets();
    }
//    requestGetSigners();
//    requestGetWallets();
    QtConcurrent::run([this]() {
        startReloadWallets();
        startReloadMasterSigners();
        startReloadRemoteSigners();
    });
}

void AppModel::requestGetSigners()
{
    DBG_INFO << "IN MODE: " << bridge::nunchukCurrentMode();
    QMasterSignerListModelPtr mastersigners = bridge::nunchukGetMasterSigners();
    if(mastersigners){
        setMasterSignerList(mastersigners);
    }
    QSingleSignerListModelPtr remoteSigners = bridge::nunchukGetRemoteSigners();
    if(remoteSigners){
        setRemoteSignerList(remoteSigners);
    }
}

void AppModel::requestGetWallets()
{
    DBG_INFO << "DATA IN MODE: " << bridge::nunchukCurrentMode();
    QWalletListModelPtr wallets = bridge::nunchukGetWallets();
    if(wallets){
        setWalletList(wallets);
        if(wallets->rowCount() > 0){
            QString lastWalletId = bridge::nunchukGetSelectedWallet();
            int lastIndex = -1;
            if(lastWalletId != ""){
                lastIndex = walletList()->getWalletIndexById(lastWalletId);
            }
            setWalletListCurrentIndex(lastIndex);
        }
    }
    requestSyncSharedWallets();
}

void AppModel::requestCreateUserWallets()
{
    if(CLIENT_INSTANCE->getSubCur().isEmpty()) return;
    QJsonObject data = Draco::instance()->getAssistedWallets();
    if(!data.isEmpty()){
        QJsonArray wallets = data["wallets"].toArray();
        mUserWallets.clear();
        for(QJsonValue jv_wallet : wallets){
            QJsonObject js_wallet = jv_wallet.toObject();
            QString wallet_id = js_wallet["local_id"].toString();
            QString status = js_wallet["status"].toString();
            mUserWallets.append(wallet_id);
            if(status == "ACTIVE" && !bridge::nunchukHasWallet(wallet_id)){
                QJsonArray signers = js_wallet["signers"].toArray();
                for(QJsonValue jv_signer : signers){
                    QJsonObject js_signer = jv_signer.toObject();
                    QJsonObject tapsigner = js_signer["tapsigner"].toObject();
                    QString name = js_signer["name"].toString();
                    QString xfp = js_signer["xfp"].toString();
                    if(!tapsigner.isEmpty()){
                        QString card_id = tapsigner["card_id"].toString();
                        QString version = tapsigner["version"].toString();
                        int birth_height = tapsigner["birth_height"].toInt();
                        bool is_testnet = tapsigner["is_testnet"].toBool();
                        bridge::AddTapsigner(card_id,xfp,name,version,birth_height,is_testnet);
                    }
                    else {
                        QString xpub = js_signer["xpub"].toString();
                        QString pubkey = js_signer["pubkey"].toString();
                        QString derivation_path = js_signer["derivation_path"].toString();
                        QString type = js_signer["type"].toString();
                        nunchuk::SingleSigner signer(name.toStdString(), xpub.toStdString(), pubkey.toStdString(), derivation_path.toStdString(), xfp.toStdString(), std::time(0));
                        if(!bridge::nunchukHasSinger(signer)){
                            bridge::nunchukCreateSigner(name, xpub, pubkey, derivation_path, xfp, type);
                        }
                    }
                }
                QWarningMessage msg;
                QString bsms = js_wallet["bsms"].toString();
                nunchuk::Wallet w = qUtils::ParseWalletDescriptor(bsms,msg);
                QString name = js_wallet["name"].toString();
                QString description = js_wallet["description"].toString();
                w.set_name(name.toStdString());
                w.set_description(description.toStdString());
                bridge::nunchukCreateWallet(w, true,msg);
            } else if (status == "DELETED" && bridge::nunchukHasWallet(wallet_id)){
                bridge::nunchukDeleteWallet(wallet_id);
            }
            QWarningMessage msg;
            nunchuk::Wallet w = nunchukiface::instance()->GetWallet(wallet_id.toStdString(), msg);
            std::vector<nunchuk::SingleSigner> local_signers = w.get_signers();
            QJsonArray signers = js_wallet["signers"].toArray();
            for(QJsonValue jv_signer : signers){
                QJsonObject js_signer = jv_signer.toObject();
                QString xfp = js_signer["xfp"].toString();
                QJsonArray wtags = js_signer["tags"].toArray();
                std::vector<nunchuk::SingleSigner>::iterator local_signer = std::find_if(local_signers.begin(), local_signers.end(), [&](const nunchuk::SingleSigner &local){
                    return local.get_master_fingerprint() == xfp.toStdString();
                });
                if(wtags.size() != 0){
                    QWarningMessage msgIn;
                    if (local_signer != local_signers.end() && local_signer->has_master_signer()) {
                        nunchuk::MasterSigner m = nunchukiface::instance()->GetMasterSigner(local_signer->get_master_signer_id(),msgIn);
                        std::vector<nunchuk::SignerTag> tags; // get tags from api signer.tags
                        for (QJsonValue tag : wtags) {
                            QString js_tag = tag.toString();
                            tags.push_back(SignerTagFromStr(js_tag.toStdString()));
                        }
                        // Do update
                        m.set_tags(tags);
                        nunchukiface::instance()->UpdateMasterSigner(m,msgIn);
                    } else {
                        // update for single signer
                        // skip for now
                    }
                }
            }

            if (!wallet_id.isEmpty()) {
                QJsonObject data = Draco::instance()->assistedWalletGetListTx(wallet_id);
                QJsonArray transactions = data.value("transactions").toArray();
                for(QJsonValue js_value : transactions){
                    QJsonObject transaction = js_value.toObject();
                    QString status = transaction.value("status").toString();
                    QString psbt = transaction.value("psbt").toString();
                    QString note = transaction.value("note").toString();
                    QString type = transaction.value("type").toString();
                    if (status == "READY_TO_BROADCAST" || status == "PENDING_SIGNATURES" ) {
                        QWarningMessage _msg;
                        QTransactionPtr tran = bridge::nunchukImportPsbt(wallet_id, psbt, _msg);
                        bridge::nunchukUpdateTransactionMemo(wallet_id,
                                                             tran->txid(),
                                                             note);
                        long int broadcast_time_milis = static_cast<long int>(transaction.value("broadcast_time_milis").toInt());
                        // honey badger feature: schedule broadcast
                        long int current_time_stamp_milis = static_cast<long int>(std::time(nullptr)) * 1000;

                        if(type == "SCHEDULED" &&
                                broadcast_time_milis > current_time_stamp_milis) {
                            bridge::nunchukUpdateTransactionSchedule(wallet_id, tran->txid(), broadcast_time_milis/1000,msg);
                        }
                    }
                }
            }

        }
    }
}

void AppModel::requestSyncSharedWallets()
{
    if(ONLINE_MODE == bridge::nunchukCurrentMode()){
        if(CLIENT_INSTANCE->isNunchukLoggedIn() && CLIENT_INSTANCE->isMatrixLoggedIn()){
            if(!CLIENT_INSTANCE->rooms() || !walletList()) return;
            QList<QRoomWalletPtr> roomWallets = CLIENT_INSTANCE->rooms()->getRoomWallets();
            for(QRoomWalletPtr roomWwallet : roomWallets){
                if(!roomWwallet) continue;
                QString wallet_id = roomWwallet.data()->get_wallet_id();
                QString room_id = roomWwallet.data()->get_room_id();
                QString init_event_id = roomWwallet.data()->get_init_event_id();
                QString wallet_name = roomWwallet.data()->walletName();
                QWalletPtr final = QWalletPtr(new Wallet());
                if(!walletList()->containsId(wallet_id)){
                    final = bridge::nunchukGetWallet(wallet_id);
                }
                else{
                    final = walletList()->getWalletById(wallet_id);
                }
                if(final){
                    final.data()->setIsSharedWallet(true);
                    final.data()->setRoomId(room_id);
                    final.data()->setInitEventId(init_event_id);
                    final.data()->setName(wallet_name);
                    if(roomWwallet.data()->walletSigners() && final.data()->singleSignersAssigned()){
                        QList<SignerAssigned> signers = roomWwallet.data()->walletSigners()->fullList();
                        for(SignerAssigned signer : signers){
                            final.data()->singleSignersAssigned()->renameByXfp(signer.xfp, signer.name);
                            final.data()->singleSignersAssigned()->updateIsLocalSigner(signer.xfp, signer.is_localuser);
                            final.data()->singleSignersAssigned()->updateSignerType(signer.xfp, signer.type);
                        }
                    }
                    walletList()->addSharedWallet(final);
                }
            }
        }
    }
}

void AppModel::requestClearData()
{
    if(walletList()){
        walletList()->cleardata();
    }
    if(masterSignerList()){
        masterSignerList()->cleardata();
    }
    if(remoteSignerList()){
        remoteSignerList()->cleardata();
    }
    AppSetting::instance()->setSyncPercent(0);
}

QWalletListModelPtr AppModel::walletListPtr() const
{
    return walletList_;
}

WalletListModel* AppModel::walletList() const {
    return walletList_.data();
}

DeviceListModel* AppModel::deviceList() const {
    return deviceList_.data();
}

QDeviceListModelPtr AppModel::deviceListPtr() const
{
    return deviceList_;
}

void AppModel::setWalletList(const QWalletListModelPtr &d){
    walletList_ = d;
    if(walletList_){
        walletList_.data()->requestSort(WalletListModel::WalletRoles::wallet_Name_Role, Qt::AscendingOrder);
    }
    emit walletListChanged();
}

void AppModel::setDeviceList(const QDeviceListModelPtr &d){
    if(deviceList_.data()){
        deviceList_.data()->updateDeviceList(d);
        emit deviceListChanged();
    }
}

MasterSigner *AppModel::primaryKey() const
{
    return m_primaryKey.data();
}

void AppModel::setPrimaryKey(const QString &userName)
{
    QList<QMasterSignerPtr> list = masterSignerList()->fullList();
    for(QMasterSignerPtr ptr: list){
        if(ptr){
            QString account = QString::fromStdString(ptr->primaryKey().get_account());
            if(account.localeAwareCompare(userName) == 0){
                m_primaryKey = ptr;
                DBG_INFO << "Found key needPassphraseSent " << ptr->needPassphraseSent();
                return;
            }
        }
    }
    DBG_INFO << "Login in manually " << list.size();
    m_primaryKey = NULL;
}

QMasterSignerPtr AppModel::getPrimaryKey() const
{
    return m_primaryKey;
}

MasterSignerListModel *AppModel::masterSignerList() const {
    return masterSignerList_.data();
}

QMasterSignerListModelPtr AppModel::masterSignerListPtr() const
{
    return masterSignerList_;
}

void AppModel::setMasterSignerList(const QMasterSignerListModelPtr &d)
{
    masterSignerList_ = d;
    if(masterSignerList_){
        masterSignerList_.data()->requestSort(MasterSignerListModel::MasterSignerRoles::master_signer_name_Role, Qt::AscendingOrder);
    }
    emit masterSignerListChanged();
}

SingleSignerListModel* AppModel::remoteSignerList() const
{
    return remoteSignerList_.data();
}

QSingleSignerListModelPtr AppModel::remoteSignerListPtr() const
{
    return remoteSignerList_;
}

void AppModel::setRemoteSignerList(const QSingleSignerListModelPtr &d)
{
    remoteSignerList_ = d;
    if(remoteSignerList_){
        remoteSignerList_.data()->requestSort(SingleSignerListModel::SingleSignerRoles::single_signer_name_Role, Qt::AscendingOrder);
    }
    emit remoteSignerListChanged();
}


MasterSigner *AppModel::masterSignerInfo() const {
    return masterSignerInfo_.data();
}

QMasterSignerPtr AppModel::masterSignerInfoPtr() const
{
    return masterSignerInfo_;
}

void AppModel::setMasterSignerInfo(const QMasterSignerPtr &d)
{
    masterSignerInfo_ = d;
    emit masterSignerInfoChanged();
}

void AppModel::setMasterSignerInfo(const int index) {
    if(masterSignerList_.data()){
        masterSignerInfo_ = masterSignerList_.data()->getMasterSignerByIndex(index);
        emit masterSignerInfoChanged();
    }
}

void AppModel::updateMasterSignerInfoName(const QString &name)
{
    if(masterSignerList_ && masterSignerInfo_){
        masterSignerList_.data()->renameById(masterSignerInfo_.data()->id(), name);
    }
    if(walletList_ && masterSignerInfo_) {
        walletList_.data()->renameSignerById(masterSignerInfo_->id(), name);
    }
    if(softwareSignerDeviceList_ && masterSignerInfo_) {
        softwareSignerDeviceList_.data()->renameById(masterSignerInfo_->id(), name);
    }
}

void AppModel::updateSingleSignerInfoName(const QString &name)
{
    if(remoteSignerList_ && singleSignerInfo_){
        remoteSignerList_.data()->renameByXfp(singleSignerInfo_.data()->masterFingerPrint(), name);
    }
    if(walletList_ && singleSignerInfo_) {
        walletList_.data()->renameSignerByXfp(singleSignerInfo_->masterFingerPrint(), name);
    }
}


SingleSigner *AppModel::singleSignerInfo()
{
    return singleSignerInfo_.data();
}

QSingleSignerPtr AppModel::singleSignerInfoPtr() const
{
    return singleSignerInfo_;
}

void AppModel::setSingleSignerInfo(const QSingleSignerPtr &d)
{
    singleSignerInfo_ = d;
    emit singleSignerInfoChanged();
}

QStringList AppModel::walletsUsingSigner() const {
    return walletsUsingSigner_;
}

void AppModel::setWalletsUsingSigner(QStringList d){
    walletsUsingSigner_ = d;
    emit walletsUsingSigner();
}

bool AppModel::removeMasterSigner(const QString &masterSignerId)
{
    bool ret = false;
    if(masterSignerList_.data()){
        ret = masterSignerList_.data()->removeMasterSigner(masterSignerId);
        if(ret) emit masterSignerListChanged();
    }
    return ret;
}

void AppModel::removeWallet(const QString &wallet_id)
{
    if(walletList_.data()){
        walletList_.data()->removeWallet(wallet_id);
    }
    emit walletListChanged();
}

Wallet *AppModel::newWalletInfo() const
{
    return newWalletInfo_.data();
}

QWalletPtr AppModel::newWalletInfoPtr() const
{
    return newWalletInfo_;
}

void AppModel::setNewWalletInfo(const QWalletPtr &d)
{
    newWalletInfo_ = d;
    emit newWalletInfoChanged();
}

Wallet *AppModel::walletInfo() const
{
    return walletInfo_.data();
}

QWalletPtr AppModel::walletInfoPtr() const
{
    return walletInfo_;
}

void AppModel::setWalletInfo(const QWalletPtr &d)
{
    walletInfo_ = d;
    if(walletInfo_){
        walletInfo_->setContainsHWSigner(false);
        startGetUsedAddresses(walletInfo_->id());
        startGetUnusedAddresses(walletInfo_->id());
        startGetTransactionHistory(walletInfo_->id());
    }
    emit walletInfoChanged();
}

void AppModel::setWalletInfoByIndex(const int index)
{
    if(walletList_){
        QWalletPtr newWallet = walletList_.data()->getWalletByIndex(index);
        if(!newWallet){
            newWallet = QWalletPtr(new Wallet());
        }
        setWalletInfo(newWallet);
    }
}

Transaction *AppModel::transactionInfo() const
{
    return transactionInfo_.data();
}

QTransactionPtr AppModel::transactionInfoPtr() const
{
    return transactionInfo_;
}

void AppModel::setTransactionInfo(const QTransactionPtr& d)
{
    if(d){
        transactionInfo_ = d;
    }
    else {
        transactionInfo_ = QTransactionPtr(new Transaction());
    }
    emit transactionInfoChanged();
}

UTXOListModel *AppModel::utxoList() const
{
    return utxoList_.data();
}

QUTXOListModelPtr AppModel::utxoListPtr() const
{
    return utxoList_;
}

void AppModel::setUtxoList(const QUTXOListModelPtr &utxoList)
{
    utxoList_ = utxoList;
    if(utxoList_.data()){
        utxoList_.data()->requestSort(UTXOListModel::UTXORoles::utxo_amount_role,  Qt::DescendingOrder);
    }
    emit utxoListChanged();
}

DestinationListModel *AppModel::destinationList() const
{
    return destinationList_.data();
}

QDestinationListModelPtr AppModel::destinationListPtr() const
{
    return destinationList_;
}

void AppModel::setDestinationList(const QDestinationListModelPtr &destinationList)
{
    destinationList_ = destinationList;
}

QString AppModel::addressBalance() const
{
    if((int)AppSetting::Unit::SATOSHI == AppSetting::instance()->unit()){
        QLocale locale(QLocale::English);
        return locale.toString(addressBalance_);
    }
    else{
        return qUtils::QValueFromAmount(addressBalance_);
    }
}

void AppModel::setAddressBalance(const qint64 &addressBalance)
{
    if(addressBalance_ != addressBalance){
        addressBalance_ = addressBalance;
        emit addressBalanceChanged();
    }
}

void AppModel::timerHealthCheckTimeHandle()
{
    if(walletList()){
        walletList()->updateHealthCheckTime();
    }
    if(remoteSignerList()){
        remoteSignerList()->updateHealthCheckTime();
    }
    emit lasttimeCheckEstimatedFeeChanged();
}

void AppModel::timerFeeRatesHandle()
{
    Draco::instance()->exchangeRates();
    this->startGetEstimatedFee();
}

void AppModel::timerCheckAuthorizedHandle()
{
    Draco::instance()->getMe();
    Draco::instance()->getCurrentUserSubscription();
}

QString AppModel::parseKeystoneSigner(QString qr)
{
    return bridge::nunchukParseKeystoneSigner(qr);
}

bool AppModel::parseKeystoneWallet(const QString name, const QString desc, const QStringList qrtags)
{
    QStringList in = qrtags;
    in.removeDuplicates();
    if(in.isEmpty()){
        return false;
    }
    QWarningMessage importmsg;
    QWalletPtr walletImported = bridge::nunchukImportKeystoneWallet(in, desc, importmsg);
    if(walletImported && importmsg.type() == (int)EWARNING::WarningType::NONE_MSG){
        walletImported.data()->setCreationMode((int)Wallet::CreationMode::CREATE_BY_IMPORT_QRCODE);
        this->walletList()->addWallet(walletImported);
        this->resetSignersChecked();
        this->walletList()->requestSort(WalletListModel::WalletRoles::wallet_Name_Role, Qt::AscendingOrder);
        int index = this->walletList()->getWalletIndexById(walletImported.data()->id());
        if(-1 != index){
            this->setWalletListCurrentIndex(index);
        }
        return true;
    }
    else{
        return false;
    }
}

bool AppModel::parseKeystoneCollabWallet(const QStringList qrtags)
{
    QStringList in = qrtags;
    in.removeDuplicates();
    if (in.isEmpty()) {
        return false;
    }
    if (CLIENT_CURRENTROOM) {
        QRoomWalletPtr wl = CLIENT_CURRENTROOM->roomWalletPtr();
        if (!wl.isNull()) {
            QWarningMessage msgwarning;
            matrixbrigde::ImportWalletQR(CLIENT_CURRENTROOM->id(),
                wl->walletName(),
                wl->walletDescription(),
                (nunchuk::Chain)AppSetting::instance()->primaryServer(),
                qrtags,
                msgwarning);
            if (msgwarning.type() == (int)EWARNING::WarningType::NONE_MSG) {
                return true;
            }
            else {
                return false;
            }
        }
    }
    return false;
}

bool AppModel::parseKeystoneTransaction(const QStringList qrtags)
{
    QStringList in = qrtags;
    in.removeDuplicates();
    if(in.isEmpty()){
        return false;
    }
    if(this->transactionInfo()){
        QString wallet_id = this->transactionInfo()->walletId();
        QWarningMessage msgwarning;
        QTransactionPtr trans = bridge::nunchukImportKeystoneTransaction(wallet_id,
                                                                         in,
                                                                         msgwarning);
        if((int)EWARNING::WarningType::NONE_MSG == msgwarning.type() && trans){
            wallet_id = this->transactionInfo()->walletId();
            this->setTransactionInfo(trans);
            this->startGetTransactionHistory(wallet_id);
            this->startGetUsedAddresses(wallet_id);
            this->startGetUnusedAddresses(wallet_id);
            return true;
        }
    }
    return false;
}

QString AppModel::parsePassportSigners(QStringList qrtags)
{
    QStringList in = qrtags;
    in.removeDuplicates();
    return bridge::nunchukParsePassportSigner(in);
}

QString AppModel::parseQRSigners(QStringList qrtags)
{
    QStringList in = qrtags;
    in.removeDuplicates();
    return bridge::nunchukParseQRSigners(in);
}

QString AppModel::parseJSONSigners(QString fileName)
{
    QString file_path = qUtils::QGetFilePath(fileName);
    return bridge::nunchukParseJSONSigners(file_path);
}

bool AppModel::parsePassportTransaction(const QStringList qrtags)
{
    QStringList in = qrtags;
    in.removeDuplicates();
    if(in.isEmpty()){
        return false;
    }
    if(this->transactionInfo()){
        QString wallet_id = this->transactionInfo()->walletId();
        QWarningMessage msgwarning;
        QTransactionPtr trans = bridge::nunchukImportPassportTransaction(wallet_id,
                                                                         in,
                                                                         msgwarning);
        if((int)EWARNING::WarningType::NONE_MSG == msgwarning.type() && trans){
            wallet_id = this->transactionInfo()->walletId();
            this->setTransactionInfo(trans);
            this->startGetTransactionHistory(wallet_id);
            this->startGetUsedAddresses(wallet_id);
            this->startGetUnusedAddresses(wallet_id);
            return true;
        }
    }
    return false;
}

bool AppModel::updateSettingRestartRequired()
{
    QWarningMessage warningmsg;
    bridge::nunchukUpdateAppSettings(warningmsg);
    if((int)EWARNING::WarningType::EXCEPTION_MSG != warningmsg.type()){
        return false;
    }
    else {
        return (nunchuk::NunchukException::APP_RESTART_REQUIRED == warningmsg.code());
    }
}

QString AppModel::getFilePath(const QString in)
{
    return qUtils::QGetFilePath(in);
}

bool AppModel::enableDatabaseEncryption(const QString in)
{
    bool ret = true;
    qApp->setOverrideCursor(QCursor(Qt::WaitCursor));
    QWarningMessage warningmsg;
    bridge::nunchukSetPassphrase(in, warningmsg);
    if((int)EWARNING::WarningType::NONE_MSG != warningmsg.type()){
        ret = false;
        int code = warningmsg.code();
        QString what = warningmsg.what();
        int type = warningmsg.type();
        QTimer::singleShot(500, [code, what, type]() {
            AppModel::instance()->showToast(code,
                                            what,
                                            (EWARNING::WarningType)type,
                                            STR_CPP_089);
        });
    }
    else{
        QTimer::singleShot(500, []() {
            AppModel::instance()->showToast(0,
                                            STR_CPP_090,
                                            EWARNING::WarningType::SUCCESS_MSG,
                                            STR_CPP_090);
        });
    }
    qApp->restoreOverrideCursor();
    return ret;
}

QStringList AppModel::qrExported() const
{
    return qrExported_;
}

void AppModel::setQrExported(const QStringList &qrExported)
{
    qrExported_ = qrExported;
    emit qrExportedChanged();
}

QWarningMessagePtr AppModel::warningMessagePtr() const
{
    return warningMessage_;
}

QWarningMessage* AppModel::warningMessage() const
{
    return warningMessage_.data();
}

void AppModel::setWarningMessage(const QWarningMessagePtr &warningMessage)
{
    warningMessage_ = warningMessage;
    emit warningMessageChanged();
}

double AppModel::getExchangeRates() const
{
    return exchangeRates_;
}

void AppModel::setExchangeRates(double exchangeRates)
{
    if(exchangeRates_ != exchangeRates && exchangeRates != 0){
        exchangeRates_ = exchangeRates;
        emit exchangeRatesChanged();
    }
}

void AppModel::showToast(int code, const QString &what, EWARNING::WarningType type, const QString& explain, POPUP::PopupType popup){
    if(!warningMessage()){
        setWarningMessage(QWarningMessagePtr(new QWarningMessage()));
    }
    warningMessage()->setWarningMessage(code, what, type, explain);
    warningMessage()->setPopupType((int)popup);
    QQuickViewer::instance()->sendEvent(E::EVT_SHOW_TOAST_MESSAGE);
}

void AppModel::setToast(int code, const QString &what, EWARNING::WarningType type, const QString &explain, POPUP::PopupType popup)
{
    if(!warningMessage()){
        setWarningMessage(QWarningMessagePtr(new QWarningMessage()));
    }
    warningMessage()->setWarningMessage(code, what, type, explain);
    warningMessage()->setPopupType((int)popup);
    emit signalShowToast();
}
