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
#include "Servers/Draco.h"
#include "localization/STR_CPP.h"
#include <QTimer>
#include "Chats/matrixbrigde.h"
#include "utils/enumconverter.hpp"
#include "Premiums/QUserWallets.h"
#include "Premiums/QGroupWallets.h"
#include "Premiums/QWalletServicesTag.h"
#include "ServiceSetting.h"

AppModel::AppModel(): inititalized_{false},
    walletList_(QWalletListModelPtr(new WalletListModel())),
    deviceList_(QDeviceListModelPtr(new DeviceListModel())),
    masterSignerList_(QMasterSignerListModelPtr(new MasterSignerListModel())),
    remoteSignerList_(QSingleSignerListModelPtr(new SingleSignerListModel())),
    masterSignerInfo_(QMasterSignerPtr(new QMasterSigner())),
    singleSignerInfo_(QSingleSignerPtr(new QSingleSigner())),
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
    exchangeRates_(0), btcRates_(0),lasttime_checkEstimatedFee_(QDateTime::currentDateTime()),
    m_primaryKey(NULL),
    newKeySignMessage_("")
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
    connect(&timerRefreshHealthCheck_, &QTimer::timeout, this, &AppModel::timerHealthCheckTimeHandle, Qt::QueuedConnection);
    connect(&timerFeeRates_, &QTimer::timeout, this, &AppModel::timerFeeRatesHandle, Qt::QueuedConnection);
    connect(&timerCheckAuthorized_, &QTimer::timeout, this, &AppModel::timerCheckAuthorizedHandle, Qt::QueuedConnection);
    connect(this, &AppModel::forwardToast, this, &AppModel::recieveToast, Qt::QueuedConnection);

    timerRefreshHealthCheck_.start(60000);  // Every 1'
    timerFeeRates_.start(300000);           // Every 5'
    qrExported_.clear();
    suggestMnemonics_.clear();

    connect(qApp, &QCoreApplication::aboutToQuit, this, [this] {
        DBG_INFO << "APPLICATION ABOUT TO QUIT" << QThreadPool::globalInstance()->activeThreadCount();
        QThreadPool::globalInstance()->clear();
        QThreadPool::globalInstance()->waitForDone();
        this->disconnect();
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

QString AppModel::newKeySignMessage() const
{
    return newKeySignMessage_;
}

QString AppModel::newKeySignMessageSHA256() const
{
    return qUtils::GetSHA256(newKeySignMessage_);
}

void AppModel::setNewKeySignMessage(const QString &value)
{
    if(newKeySignMessage_ != value){
        newKeySignMessage_ = value;
        emit newKeySignMessageChanged();
    }
}

const std::vector<nunchuk::PrimaryKey> &AppModel::primaryKeys()
{
    if (m_primaryKeys.size() == 0) {
        m_primaryKeys = qUtils::GetPrimaryKeys(AppSetting::instance()->storagePath(),
                                               (nunchuk::Chain)AppSetting::instance()->primaryServer());
    }
    return m_primaryKeys;
}

nunchuk::PrimaryKey AppModel::findPrimaryKey(const QString &fingerprint)
{
    for(nunchuk::PrimaryKey key: primaryKeys()){
        if(key.get_master_fingerprint() == fingerprint.toStdString()){
            return key;
        }
    }
    return nunchuk::PrimaryKey();
}

void AppModel::clearPrimaryKeyList()
{
    m_primaryKeys.clear();
}

void AppModel::setAddSignerWizard(int index)
{
    if (m_addSignerWizard == index)
        return;

    m_addSignerWizard = index;
    emit addSignerWizardChanged();
}

int AppModel::addSignerWizard() const
{
    return m_addSignerWizard;
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
    FuncTime f(__PRETTY_FUNCTION__);
    bool ret = true;
    QWarningMessage nunchukMsg;
    bridge::nunchukSetCurrentMode(ONLINE_MODE);
    QString account = Draco::instance()->Uid();
    bridge::nunchukMakeInstanceForAccount(account,
                                          dbPassphrase,
                                          nunchukMsg);
    if((int)EWARNING::WarningType::NONE_MSG == nunchukMsg.type()){
        ret = true;
        AppModel::instance()->requestInitialData();
        AppModel::instance()->timerFeeRatesHandle();
        timeoutHandler(500,[account](){
            CLIENT_INSTANCE->requestLogin();
            CLIENT_INSTANCE->saveStayLoggedInData();
            QString device_id = QString("%1%2").arg(Draco::instance()->deviceId()).arg(Draco::instance()->chatId());
            QWarningMessage matrixMsg;
            matrixbrigde::makeMatrixInstance(account, device_id, matrixMsg);
            if((int)EWARNING::WarningType::NONE_MSG != matrixMsg.type()){
                AppModel::instance()->showToast(matrixMsg.code(), matrixMsg.what(), (EWARNING::WarningType)matrixMsg.type());
            }
        });
    }
    else if((int)EWARNING::WarningType::EXCEPTION_MSG == nunchukMsg.type() && nunchuk::NunchukException::INVALID_PASSPHRASE == nunchukMsg.code()){
        DBG_INFO << "COULD NOT MAKE NUNCHUCK INSTANCE" << nunchukMsg.code();
        ret = false;
        QList<uint> states = QQuickViewer::instance()->getCurrentStates();
        if(!states.isEmpty() && states.last() != (uint)E::STATE_ID_SCR_UNLOCK_DB)
        {
            timeoutHandler(500,[msg](){
                QQuickViewer::instance()->sendEvent(E::EVT_LOGIN_DB_REQUEST, msg);
            });
        }
        else{
            AppModel::instance()->showToast(nunchukMsg.code(), nunchukMsg.what(), (EWARNING::WarningType)nunchukMsg.type());
        }
    }
    else{
        ret = false;
        AppModel::instance()->showToast(nunchukMsg.code(), nunchukMsg.what(), (EWARNING::WarningType)nunchukMsg.type());
    }
    return ret;
}

bool AppModel::makeNunchukInstanceForAccount(const QVariant msg, const QString &dbPassphrase)
{
    FuncTime f(__PRETTY_FUNCTION__);
    bool ret = true;
    QWarningMessage warningmsg;
    bridge::nunchukSetCurrentMode(ONLINE_MODE);
    QString account = Draco::instance()->Uid();
    bridge::nunchukMakeInstanceForAccount(account,
                                          dbPassphrase,
                                          warningmsg);
    if((int)EWARNING::WarningType::NONE_MSG == warningmsg.type()){
        ret = true;
        AppModel::instance()->requestInitialData();
        AppModel::instance()->timerFeeRatesHandle();
    }
    else if((int)EWARNING::WarningType::EXCEPTION_MSG == warningmsg.type() && nunchuk::NunchukException::INVALID_PASSPHRASE == warningmsg.code()){
        DBG_INFO << "COULD NOT MAKE NUNCHUCK INSTANCE" << warningmsg.code();
        ret = false;
        QList<uint> states = QQuickViewer::instance()->getCurrentStates();
        if(!states.isEmpty() && states.last() != (uint)E::STATE_ID_SCR_UNLOCK_DB)
        {
            timeoutHandler(500,[msg](){
                QQuickViewer::instance()->sendEvent(E::EVT_LOGIN_DB_REQUEST, msg);
            });
        }
        else{
            AppModel::instance()->showToast(warningmsg.code(), warningmsg.what(), (EWARNING::WarningType)warningmsg.type());
        }
    }
    else{
        DBG_INFO << "COULD NOT MAKE NUNCHUCK INSTANCE" << warningmsg.code();
        ret = false;
        AppModel::instance()->showToast(warningmsg.code(), warningmsg.what(), (EWARNING::WarningType)warningmsg.type());
    }
    return ret;
}

void AppModel::makeMatrixInstanceForAccount()
{
    QString account = Draco::instance()->Uid();
    QString device_id = QString("%1%2").arg(Draco::instance()->deviceId()).arg(Draco::instance()->chatId());
    QWarningMessage warningmsg;
    matrixbrigde::makeMatrixInstance(account,
                                     device_id,
                                     warningmsg);

    if((int)EWARNING::WarningType::NONE_MSG == warningmsg.type()){
        CLIENT_INSTANCE->requestLogin();
        CLIENT_INSTANCE->saveStayLoggedInData();
        AppModel::instance()->timerFeeRatesHandle();
    }
    else {
        AppModel::instance()->showToast(warningmsg.code(), warningmsg.what(), (EWARNING::WarningType)warningmsg.type());
    }
}

bool AppModel::makeNunchukInstance(const QVariant makeInstanceData, const QString &dbPassphrase)
{
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
            AppModel::instance()->showToast(warningmsg.code(), warningmsg.what(), (EWARNING::WarningType)warningmsg.type());
        }
    }
    else{
        ret = false;
        AppModel::instance()->showToast(warningmsg.code(), warningmsg.what(), (EWARNING::WarningType)warningmsg.type());
    }
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

QString AppModel::hourFeeCurrency() const
{
    double fee = (double)hourFee_/1000;
    double btcRate = btcRates()/100000000;
    double feeCurrency = btcRate*fee*140;
    QLocale locale(QLocale::English);
    return locale.toString(feeCurrency, 'f', 2);
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

QString AppModel::minFeeCurrency() const
{
    double btcRate = btcRates()/100000000;
    double fee = (double)minFee_/1000;
    double feeCurrency = btcRate*fee*140;
    QLocale locale(QLocale::English);
    return locale.toString(feeCurrency, 'f', 2);
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

void AppModel::resetSignersChecked()
{
    if(masterSignerList_){
        masterSignerList_.data()->resetUserChecked();
    }
    if(remoteSignerList_){
        remoteSignerList_.data()->resetUserChecked();
    }
}

QString AppModel::halfHourFeeCurrency() const
{
    double btcRate = btcRates()/100000000;
    double fee = (double)halfHourFee_/1000;
    double feeCurrency = btcRate*fee*140;
    QLocale locale(QLocale::English);
    return locale.toString(feeCurrency, 'f', 2);
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

QString AppModel::fastestFeeCurrency() const
{
    double btcRate = btcRates()/100000000;
    double fee = (double)fastestFee_/1000;
    double feeCurrency = btcRate*fee*140;
    QLocale locale(QLocale::English);
    return locale.toString(feeCurrency, 'f', 2);
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
    emit txidReplacingChanged();
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

void AppModel::setWalletListCurrentIndex(int index)
{
    if(walletListCurrentIndex_ != index){
        walletListCurrentIndex_ = index;
    }
    emit walletListCurrentIndexChanged();
    setWalletInfoByIndex(walletListCurrentIndex_);
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
    else{
        startReloadUserDb();
    }
}

void AppModel::requestSyncWalletDb(const QString &wallet_id)
{
    if(wallet_id != ""){
        startSyncWalletDb(wallet_id);
    }
}

void AppModel::requestCreateUserWallets()
{
    if(CLIENT_INSTANCE->subscription().isEmpty()) {
        QtConcurrent::run([]() {
            WalletsMng->GetListWallet(GROUP_WALLET);
            AppModel::instance()->startReloadUserDb();
        });
    }
    else {
        QtConcurrent::run([]() {
            WalletsMng->GetListWallet(USER_WALLET);
            WalletsMng->GetListWallet(GROUP_WALLET);
            AppModel::instance()->startReloadUserDb();
        });
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
                            final.data()->singleSignersAssigned()->updateSignerOfRoomWallet(signer);
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
    setWalletListCurrentIndex(-1);
    AppSetting::instance()->setSyncPercent(0);
    QUserWallets::instance()->reset();
    QGroupWallets::instance()->reset();
    WalletsMng->clear();
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
        walletList_.data()->requestSort(WalletListModel::WalletRoles::wallet_createDate_Role, Qt::AscendingOrder);
    }
    emit walletListChanged();
}

void AppModel::setDeviceList(const QDeviceListModelPtr &d){
    if(deviceList_.data()){
        deviceList_.data()->updateDeviceList(d);
        emit deviceListChanged();
    }
}

QMasterSigner *AppModel::originPrimaryKey() const
{
    return m_primaryKey.data();
}

void AppModel::setPrimaryKey(const QString &userName)
{
    QList<QMasterSignerPtr> list = masterSignerList()->fullList();
    for(QMasterSignerPtr ptr: list){
        if(ptr){
            QString account = QString::fromStdString(ptr->originPrimaryKey().get_account());
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
        remoteSignerList_.data()->requestSort();
    }
    emit remoteSignerListChanged();
}


QMasterSigner *AppModel::masterSignerInfo() const {
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

QSingleSigner *AppModel::singleSignerInfo()
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

void AppModel::setWalletInfo(WalletId wallet_id)
{
    auto w = AppModel::instance()->walletListPtr()->getWalletById(wallet_id);
    setWalletInfo(w);
}

void AppModel::setWalletInfo(const QWalletPtr &d)
{
    if(d && 0 != QString::compare(d.data()->id(), walletInfo_->id(), Qt::CaseInsensitive)){
        walletInfo_ = d;
        if(walletInfo_){
            QString wallet_id = walletInfo_.data()->id();
            if(wallet_id != ""){
                requestSyncWalletDb(wallet_id);
                QtConcurrent::run([wallet_id]() {
                    bridge::nunchukSetSelectedWallet(wallet_id);
                });
            }
        }
        emit walletInfoChanged();
    }
}

void AppModel::setWalletInfoByIndex(const int index)
{
    if(index == -1){
        setWalletInfo(QWalletPtr(new Wallet()));
    }
    else{
        if(walletList_){
            QWalletPtr newWallet = walletList_.data()->getWalletByIndex(index);
            if(!newWallet){
                newWallet = QWalletPtr(new Wallet());
            }
            setWalletInfo(newWallet);
        }
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
    QtConcurrent::run([this]() {
        Draco::instance()->btcRates();
        Draco::instance()->exchangeRates(AppSetting::instance()->currency());
        startGetEstimatedFee();
    });
}

void AppModel::timerCheckAuthorizedHandle()
{
    Draco::instance()->getMe();
    QGroupWallets::instance()->GetAllGroups();
    QUserWallets::instance()->GetListAllRequestAddKey();
}

bool AppModel::parseQRWallet(const QString name, const QString desc, const QStringList qrtags)
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
        walletImported.data()->setName(name);
        walletList()->addWallet(walletImported);
        resetSignersChecked();
        walletList()->requestSort(WalletListModel::WalletRoles::wallet_createDate_Role, Qt::AscendingOrder);
        int index = walletList()->getWalletIndexById(walletImported.data()->id());
        if(-1 != index){
            setWalletListCurrentIndex(index);
        }
        return true;
    }
    else{
        return false;
    }
}

bool AppModel::parseQRCollabWallet(const QStringList qrtags)
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
        AppModel::instance()->showToast(warningmsg.code(), warningmsg.what(), (EWARNING::WarningType)warningmsg.type());
    }
    else{
        AppModel::instance()->showToast(0, STR_CPP_090, EWARNING::WarningType::SUCCESS_MSG);
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

double AppModel::exchangeRates() const
{
    return exchangeRates_;
}

void AppModel::setExchangeRates(double exchangeRates)
{
    if(exchangeRates_ != exchangeRates && exchangeRates > 0){
        exchangeRates_ = exchangeRates;
        emit btcRatesChanged();
        emit exchangeRatesChanged();
    }
}

double AppModel::btcRates() const
{
    return btcRates_;
}

void AppModel::setBtcRates(double btcRates)
{
    if(btcRates_ != btcRates && btcRates > 0){
        btcRates_ = btcRates;
        emit btcRatesChanged();
        emit exchangeRatesChanged();
    }
}

QVariantList AppModel::listMessage() const
{
    return listMessage_;
}

void AppModel::showToast(int code, const QString &what, EWARNING::WarningType type){
    emit forwardToast(code, what, type);
}

void AppModel::recieveToast(int code, const QString &what, EWARNING::WarningType type)
{
    timeoutHandler(500, [this, code, what, type]() {
        QWarningMessage message;
        message.setWarningMessage(code, what, type);
        QJsonObject data;
        data["contentDisplay"] = message.contentDisplay();
        data["explaination"  ] = message.explaination();
        data["what"          ] = message.what();
        data["code"          ] = message.code();
        data["type"          ] = message.type();
        data["popupType"     ] = message.popupType();
        QQuickViewer::instance()->sendToastMessage(QVariant::fromValue(data));
    });
}
