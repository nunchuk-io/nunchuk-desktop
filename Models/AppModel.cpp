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
#include "QEventProcessor.h"
#include "ViewsEnums.h"
#include "Servers/Draco.h"
#include "localization/STR_CPP.h"
#include <QTimer>
#include "Chats/matrixbrigde.h"
#include "utils/enumconverter.hpp"
#include "Premiums/QUserWallets.h"
#include "Premiums/QGroupWallets.h"
#include "Premiums/QWalletServicesTag.h"
#include "Premiums/QSharedWallets.h"
#include "ServiceSetting.h"
#include "OnBoardingModel.h"
#include "QThreadForwarder.h""

class QEmpyTask : public QRunnable {
public:
    void run() override {
        DBG_INFO << "Running task on thread:" << QThread::currentThread();
    }
};

AppModel::AppModel(): m_inititalized{false},
    m_walletList(QWalletListModelPtr(new WalletListModel())),
    m_deviceList(QDeviceListModelPtr(new DeviceListModel())),
    m_masterSignerList(QMasterSignerListModelPtr(new MasterSignerListModel())),
    m_remoteSignerList(QSingleSignerListModelPtr(new SingleSignerListModel())),
    m_masterSignerInfo(QMasterSignerPtr(new QMasterSigner())),
    m_singleSignerInfo(QSingleSignerPtr(new QSingleSigner())),
    m_walletInfo(QWalletPtr(new Wallet())),
    m_transactionInfo(QTransactionPtr(new Transaction())),
    m_transactionReplaceInfo(NULL),
    m_transactionPending(QTransactionListModelPtr(new TransactionListModel())),
    m_walletListCurrentIndex(-1),
    m_chainTip(0), m_addSignerStep(-1), m_addSignerPercentage(0), m_txidReplacing(""),
    m_fastestFee(1000), m_halfHourFee(1000), m_hourFee(1000), m_minFee(1000),
    m_addressBalance(0),m_mnemonic(""),
    m_softwareSignerDeviceList(QDeviceListModelPtr(new DeviceListModel())),
    m_nunchukMode(LOCAL_MODE),
    m_tabIndex((int)ENUNCHUCK::TabSelection::WALLET_TAB),
    m_warningMessage(QWarningMessagePtr(new QWarningMessage())),
    m_exchangeRates(0), m_btcRates(0),m_lasttimeCheckEstimatedFee(QDateTime::currentDateTime()),
    m_primaryKey(NULL),
    m_newKeySignMessage(""),
    m_timeLogging(QDateTime::currentDateTime())
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
    connect(&m_timerRefreshHealthCheck, &QTimer::timeout, this, &AppModel::timerHealthCheckTimeHandle, Qt::QueuedConnection);
    connect(&m_timerFeeRates, &QTimer::timeout, this, &AppModel::timerFeeRatesHandle, Qt::QueuedConnection);
    connect(&m_timerCheckAuthorized, &QTimer::timeout, this, &AppModel::timerCheckAuthorizedHandle, Qt::QueuedConnection);
    connect(this, &AppModel::forwardToast, this, &AppModel::recieveToast, Qt::QueuedConnection);
    connect(this, &AppModel::signViaSingature, this, &AppModel::slotSignViaSingature, Qt::QueuedConnection);

    m_timerRefreshHealthCheck.start(60000);  // Every 1'
    m_timerFeeRates.start(300000);           // Every 5'
    m_qrExported.clear();
    m_suggestMnemonics.clear();

    connect(qApp, &QCoreApplication::aboutToQuit, this, [] {
        DBG_INFO << "APPLICATION ABOUT TO QUIT" << QThreadPool::globalInstance()->activeThreadCount();
        bridge::stopAllNunchuk();
        QThreadPool::globalInstance()->clear();
        QThreadPool::globalInstance()->setMaxThreadCount(0);
    });
}

AppModel::~AppModel(){
    m_timerRefreshHealthCheck.stop();
    m_timerCheckAuthorized.stop();
    m_walletList.clear();
    m_deviceList.clear();
    m_masterSignerList.clear();
    m_remoteSignerList.clear();
    m_masterSignerInfo.clear();
    m_singleSignerInfo.clear();
    m_newWalletInfo.clear();
    m_walletInfo.clear();
    m_transactionInfo.clear();
    m_transactionReplaceInfo.clear();
    m_destinationList.clear();
    disconnect();
}

void AppModel::confirmSyncingWalletFromServer(bool yes, bool no)
{
    QString fingerPrint = WalletsMng->UpdateSyncWalletFlows(yes, no);
    if (yes) {
        int index = masterSignerList()->getIndexNameByFingerPrint(fingerPrint);
        if (index >= 0) {
            QEventProcessor::instance()->sendEvent(E::EVT_HOME_MASTER_SIGNER_INFO_REQUEST, fingerPrint);
            emit syncingConfirmWalletRemoveKey(fingerPrint);
        }
    }
}

void AppModel::setTimeLogging(const QDateTime &newTimeLogging)
{
    m_timeLogging = newTimeLogging;
}

void AppModel::slotSignViaSingature()
{
    AppModel::instance()->requestClearData();
    timeoutHandler(100, [this](){
        makeInstanceForAccount({}, "");
    });
}

QString AppModel::newKeySignMessage() const
{
    return m_newKeySignMessage;
}

QString AppModel::newKeySignMessageSHA256() const
{
    return qUtils::GetSHA256(m_newKeySignMessage);
}

void AppModel::setNewKeySignMessage(const QString &value)
{
    if(m_newKeySignMessage != value){
        m_newKeySignMessage = value;
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

bool AppModel::checkAutoSignOut()
{
    QDateTime now = QDateTime::currentDateTime();
    qint64 secs = m_timeLogging.secsTo(now);
    qint64 _7days = 7 * 24 * 60 * 60;
    if(secs > _7days){
        ClientController::instance()->requestSignout();
        m_timeLogging = now;
        return true;
    }
    return false;
}

QString AppModel::lasttimeCheckEstimatedFee() const
{
    QDateTime in = m_lasttimeCheckEstimatedFee;
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
    if(m_lasttimeCheckEstimatedFee == lasttime_checkEstimatedFee) {return;}
    m_lasttimeCheckEstimatedFee = lasttime_checkEstimatedFee;
    emit lasttimeCheckEstimatedFeeChanged();
}

void AppModel::startCheckAuthorize()
{
    m_timerCheckAuthorized.stop();
    m_timerCheckAuthorized.start(120000); // Every 2'
}

void AppModel::stopCheckAuthorize()
{
    m_timerCheckAuthorized.stop();
}

bool AppModel::makeInstanceForAccount(const QVariant msg, const QString &dbPassphrase)
{
    QFunctionTime f(__PRETTY_FUNCTION__);
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
        QList<uint> states = QEventProcessor::instance()->getCurrentStates();
        if(!states.isEmpty() && states.last() != (uint)E::STATE_ID_SCR_UNLOCK_DB)
        {
            timeoutHandler(500,[msg](){
                QEventProcessor::instance()->sendEvent(E::EVT_LOGIN_DB_REQUEST, msg);
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
    QFunctionTime f(__PRETTY_FUNCTION__);
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
        QList<uint> states = QEventProcessor::instance()->getCurrentStates();
        if(!states.isEmpty() && states.last() != (uint)E::STATE_ID_SCR_UNLOCK_DB)
        {
            timeoutHandler(500,[msg](){
                QEventProcessor::instance()->sendEvent(E::EVT_LOGIN_DB_REQUEST, msg);
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
        QList<uint> states = QEventProcessor::instance()->getCurrentStates();
        if(!states.isEmpty() && states.last() != (uint)E::STATE_ID_SCR_UNLOCK_DB)
        {
            QEventProcessor::instance()->sendEvent(E::EVT_LOGIN_DB_REQUEST, makeInstanceData);
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
    QString emailInput      = maps.value("email").toString();
    QString passwordInput   = maps.value("password").toString();
    Draco::instance()->singin(emailInput, passwordInput);

    if(maps.contains("username")){
        QString username   = maps.value("username").toString();
        Draco::instance()->setUserProfile(username, "");
    }
    qApp->restoreOverrideCursor();
}

void AppModel::create_account(QVariant msg)
{
    QMap<QString,QVariant> maps = msg.toMap();
    QString nameInput = maps.value("name").toString();
    QString emailInput =  maps.value("email").toString();
    Draco::instance()->createAccount(nameInput, emailInput);
}

void AppModel::checkAccountAvailability(QVariant msg)
{
    qApp->setOverrideCursor(QCursor(Qt::WaitCursor));
    QMap<QString,QVariant> maps = msg.toMap();
    QString emailInput = maps.value("email").toString();
    Draco::instance()->checkAccountAvailability(emailInput);
    qApp->restoreOverrideCursor();
}

void AppModel::resendTempoPassword(QVariant msg)
{
    qApp->setOverrideCursor(QCursor(Qt::WaitCursor));
    QMap<QString,QVariant> maps = msg.toMap();
    QString emailInput = maps.value("email").toString();
    Draco::instance()->resendPassword(emailInput);
    qApp->restoreOverrideCursor();
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
    return m_nunchukMode;
}

void AppModel::setNunchukMode(int nunchukMode)
{
    m_nunchukMode = nunchukMode;
    emit nunchukModeChanged();
}

bool AppModel::inititalized() const
{
    return m_inititalized[m_nunchukMode];
}

void AppModel::setInititalized(bool inititalized)
{
    m_inititalized[m_nunchukMode] = inititalized;
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
    return m_suggestMnemonics;
}

void AppModel::setSuggestMnemonics(const QStringList &suggestMnemonics)
{
    m_suggestMnemonics = suggestMnemonics;
    emit suggestMnemonicsChanged();
}

QString AppModel::getMnemonic() const
{
    return m_mnemonic;
}

void AppModel::setMnemonic(const QString &mnemonic)
{
    if(m_mnemonic != mnemonic){
        m_mnemonic = mnemonic;
        emit mnemonicChanged();
    }
}

DeviceListModel *AppModel::softwareSignerDeviceList() const
{
    return m_softwareSignerDeviceList.data();
}

QDeviceListModelPtr AppModel::softwareSignerDeviceListPtr() const
{
    return m_softwareSignerDeviceList;
}

void AppModel::setSoftwareSignerDeviceList(const QDeviceListModelPtr &value)
{
    m_softwareSignerDeviceList = value;
    emit softwareSignerDeviceListChanged();
}

qint64 AppModel::hourFeeOrigin() const
{
    return m_hourFee;
}

QString AppModel::hourFeeCurrency() const
{
    double fee = (double)m_hourFee/1000;
    double btcRate = btcRates()/100000000;
    double feeCurrency = btcRate*fee*140;
    QLocale locale(QLocale::English);
    return locale.toString(feeCurrency, 'f', 2);
}

QString AppModel::hourFee() const
{    
    double fee = (double)m_hourFee/1000;
    QLocale locale(QLocale::English);
    return locale.toString(fee, 'f', 2);
}

void AppModel::setHourFee(qint64 fee)
{
    if(m_hourFee != fee){
        m_hourFee = fee;
        emit hourFeeChanged();
    }
}

qint64 AppModel::minFeeOrigin() const
{
    return m_minFee;
}

QString AppModel::minFeeCurrency() const
{
    double btcRate = btcRates()/100000000;
    double fee = (double)m_minFee/1000;
    double feeCurrency = btcRate*fee*140;
    QLocale locale(QLocale::English);
    return locale.toString(feeCurrency, 'f', 2);
}

QString AppModel::minFee() const
{
    double fee = (double)m_minFee/1000;
    QLocale locale(QLocale::English);
    return locale.toString(fee, 'f', 2);
}

void AppModel::setMinFee(qint64 fee)
{
    if(m_minFee != fee){
        m_minFee = fee;
        emit minFeeChanged();
    }
}

void AppModel::resetSignersChecked()
{
    if(m_masterSignerList){
        m_masterSignerList.data()->resetUserChecked();
    }
    if(m_remoteSignerList){
        m_remoteSignerList.data()->resetUserChecked();
    }
}

qint64 AppModel::halfHourFeeOrigin() const
{
    return m_halfHourFee;
}

QString AppModel::halfHourFeeCurrency() const
{
    double btcRate = btcRates()/100000000;
    double fee = (double)m_halfHourFee/1000;
    double feeCurrency = btcRate*fee*140;
    QLocale locale(QLocale::English);
    return locale.toString(feeCurrency, 'f', 2);
}

QString AppModel::halfHourFee() const
{
    double fee = (double)m_halfHourFee/1000;
    QLocale locale(QLocale::English);
    return locale.toString(fee, 'f', 2);
}

void AppModel::setHalfHourFee(qint64 fee)
{
    if(m_halfHourFee != fee){
        m_halfHourFee = fee;
        emit halfHourFeeChanged();
    }
}

qint64 AppModel::fastestFeeOrigin() const
{
    return m_fastestFee;
}

QString AppModel::fastestFeeCurrency() const
{
    double btcRate = btcRates()/100000000;
    double fee = (double)m_fastestFee/1000;
    double feeCurrency = btcRate*fee*140;
    QLocale locale(QLocale::English);
    return locale.toString(feeCurrency, 'f', 2);
}

QString AppModel::fastestFee() const
{
    double fee = (double)m_fastestFee/1000;
    QLocale locale(QLocale::English);
    return locale.toString(fee, 'f', 2);
}

void AppModel::setFastestFee(qint64 fee)
{
    if(m_fastestFee != fee){
        m_fastestFee = fee;
        emit fastestFeeChanged();
    }
}

QString AppModel::getTxidReplacing() const
{
    return m_txidReplacing;
}

void AppModel::setTxidReplacing(const QString &txidReplacing)
{
    m_txidReplacing = txidReplacing;
    emit txidReplacingChanged();
}

int AppModel::getAddSignerPercentage() const
{
    return m_addSignerPercentage;
}

void AppModel::setAddSignerPercentage(int addSignerPercentage)
{
    if(m_addSignerPercentage != addSignerPercentage){
        m_addSignerPercentage = addSignerPercentage;
        emit addSignerPercentageChanged();
    }
}

int AppModel::getAddSignerStep() const
{
    return m_addSignerStep;
}

void AppModel::setAddSignerStep(int addSignerStep)
{
    if(m_addSignerStep != addSignerStep){
        m_addSignerStep = addSignerStep;
        emit addSignerStepChanged();
    }
}

QTransactionPtr AppModel::getTransactionReplaceInfo() const
{
    return m_transactionReplaceInfo;
}

void AppModel::setTransactionReplaceInfo(const QTransactionPtr &transactionReplaceInfo)
{
    m_transactionReplaceInfo = transactionReplaceInfo;
}

int AppModel::chainTip() const
{
    return m_chainTip;
}

void AppModel::setChainTip(int chainTip)
{
    if(m_chainTip != chainTip){
        m_chainTip = chainTip;
        emit chainTipChanged();
    }
}

int AppModel::walletListCurrentIndex() const
{
    return m_walletListCurrentIndex;
}

void AppModel::setWalletListCurrentIndex(int index, bool force)
{
    if(m_walletListCurrentIndex != index){
        m_walletListCurrentIndex = index;
    }
    emit walletListCurrentIndexChanged();
    setWalletInfoByIndex(m_walletListCurrentIndex, force);
    emit QGroupWallets::instance()->dashboardInfoChanged();
}

AppModel *AppModel::instance() {
    static AppModel mInstance;
    return &mInstance;
}

void AppModel::requestInitialData()
{
    //For Group wallet
    bridge::EnableGroupWallet();
    QSharedWallets::instance()->GetGroupDeviceUID();
    // Order
    bridge::StartConsumeGroupEvent();

    if(ONLINE_MODE == bridge::nunchukCurrentMode()){
        requestCreateUserWallets();
    }
    else{
        startReloadUserDb();
    }
    requestSyncGroups();
}

void AppModel::requestSyncWalletDb(const QString &wallet_id)
{
    if(wallet_id != ""){
        startSyncWalletDb(wallet_id);
    }
}

void AppModel::requestCreateUserWallets()
{
    QtConcurrent::run([]() {
        WalletsMng->GetListWallet(USER_WALLET);
        WalletsMng->GetListWallet(GROUP_WALLET);
        AppModel::instance()->startReloadUserDb();
    });
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
                    final.data()->setWalletName(wallet_name);
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
    if(groupWalletList()){
        groupWalletList()->cleardata();
    }
    WalletsMng->clear();
    SharedWalletsMng->clear();
    setWalletListCurrentIndex(-1);
    AppSetting::instance()->setSyncPercent(0);
    QUserWallets::instance()->reset();
    QGroupWallets::instance()->reset();
}

void AppModel::requestOnboarding()
{
    timeoutHandler(1000,[=, this]{
        if (CLIENT_INSTANCE->isSubscribed() || QGroupWallets::instance()->existGroupPending() || AppModel::instance()->walletListPtr()->existGroupWallet()) {
            AppSetting::instance()->setIsFirstTimeOnboarding(true);
        } else {
            DBG_INFO << "Checking Onboarding " << AppSetting::instance()->isFirstTimeOnboarding();
            if (!AppSetting::instance()->isFirstTimeOnboarding()) {
                OnBoardingModel::instance()->setState("onboarding");
                QEventProcessor::instance()->sendEvent(E::EVT_ONBOARDING_REQUEST);
            }
        }
    });
}

void AppModel::requestSyncGroups()
{
    QUserWallets::instance()->GetListAllRequestAddKey();
    QGroupWallets::instance()->GetAllGroups();
    QSharedWallets::instance()->GetAllGroups();
}

QWalletListModelPtr AppModel::walletListPtr() const
{
    return m_walletList;
}

WalletListModel* AppModel::walletList() const {
    return m_walletList.data();
}

DeviceListModel* AppModel::deviceList() const {
    return m_deviceList.data();
}

QDeviceListModelPtr AppModel::deviceListPtr() const
{
    return m_deviceList;
}

void AppModel::setWalletList(const QWalletListModelPtr &d){
    m_walletList = d;
    if(m_walletList){
        m_walletList.data()->requestSort(WalletListModel::WalletRoles::wallet_createDate_Role, Qt::AscendingOrder);
    }
    emit walletListChanged();
}

WalletListModel *AppModel::groupWalletList() const
{
    return m_groupWalletList.data();
}

QWalletListModelPtr AppModel::groupWalletListPtr() const
{
    return m_groupWalletList;
}

void AppModel::setGroupWalletList(const QWalletListModelPtr &d)
{
    m_groupWalletList = d;
    if (m_groupWalletList.isValid()) {
        m_groupWalletList->startAllConversation();
    }
    emit groupWalletListChanged();
}

void AppModel::setDeviceList(const QDeviceListModelPtr &d){
    if(m_deviceList.data()){
        m_deviceList.data()->updateDeviceList(d);
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
    return m_masterSignerList.data();
}

QMasterSignerListModelPtr AppModel::masterSignerListPtr() const
{
    return m_masterSignerList;
}

void AppModel::setMasterSignerList(const QMasterSignerListModelPtr &d)
{
    m_masterSignerList = d;
    if(m_masterSignerList){
        m_masterSignerList.data()->requestSort(MasterSignerListModel::MasterSignerRoles::master_signer_name_Role, Qt::AscendingOrder);
    }
    emit masterSignerListChanged();
}

SingleSignerListModel* AppModel::remoteSignerList() const
{
    return m_remoteSignerList.data();
}

QSingleSignerListModelPtr AppModel::remoteSignerListPtr() const
{
    return m_remoteSignerList;
}

void AppModel::setRemoteSignerList(const QSingleSignerListModelPtr &d)
{
    m_remoteSignerList = d;
    if(m_remoteSignerList){
        m_remoteSignerList.data()->requestSort();
    }
    emit remoteSignerListChanged();
}


QMasterSigner *AppModel::masterSignerInfo() const {
    return m_masterSignerInfo.data();
}

QMasterSignerPtr AppModel::masterSignerInfoPtr() const
{
    return m_masterSignerInfo;
}

void AppModel::setMasterSignerInfo(const QMasterSignerPtr &d)
{
    m_masterSignerInfo = d;
    emit masterSignerInfoChanged();
}

void AppModel::setMasterSignerInfo(const int index) {
    if(m_masterSignerList.data()){
        m_masterSignerInfo = m_masterSignerList.data()->getMasterSignerByIndex(index);
        emit masterSignerInfoChanged();
    }
}

QSingleSigner *AppModel::singleSignerInfo()
{
    return m_singleSignerInfo.data();
}

QSingleSignerPtr AppModel::singleSignerInfoPtr() const
{
    return m_singleSignerInfo;
}

void AppModel::setSingleSignerInfo(const QSingleSignerPtr &d)
{
    m_singleSignerInfo = d;
    emit singleSignerInfoChanged();
}

QVariantList AppModel::walletsUsingSigner() const {
    return m_walletsUsingSigner;
}

void AppModel::setWalletsUsingSigner(QVariantList d){
    m_walletsUsingSigner = d;
    emit walletsUsingSigner();
}

bool AppModel::removeMasterSigner(const QString &masterSignerId)
{
    bool ret = false;
    if(m_masterSignerList.data()){
        ret = m_masterSignerList.data()->removeMasterSigner(masterSignerId);
        if(ret) emit masterSignerListChanged();
    }
    return ret;
}

void AppModel::removeWallet(const QString &wallet_id)
{
    if(m_walletList){
        m_walletList.data()->removeWallet(wallet_id);
    }
    if(m_groupWalletList){
        m_groupWalletList.data()->removeWallet(wallet_id);
    }
    emit walletListChanged();
}

Wallet *AppModel::newWalletInfo() const
{
    return m_newWalletInfo.data();
}

QWalletPtr AppModel::newWalletInfoPtr() const
{
    return m_newWalletInfo;
}

void AppModel::setNewWalletInfo(const QWalletPtr &d)
{
    m_newWalletInfo = d;
    emit newWalletInfoChanged();
}

Wallet *AppModel::walletInfo() const
{
    return m_walletInfo.data();
}

QWalletPtr AppModel::walletInfoPtr() const
{
    return m_walletInfo;
}

void AppModel::setWalletInfo(const QWalletPtr &d, bool force)
{
    bool allow = force || !qUtils::strCompare(d.data()->walletId(), m_walletInfo->walletId());
    if(d && allow){
        m_walletInfo = d;
        if(m_walletInfo){
            QString wallet_id = m_walletInfo.data()->walletId();
            if(wallet_id != ""){
                requestSyncWalletDb(wallet_id);
                QtConcurrent::run([wallet_id]() {
                    bridge::nunchukSetSelectedWallet(wallet_id);
                });
                m_walletInfo.data()->getChatInfo();
            }
        }
        emit walletInfoChanged();
    }
    if(m_walletInfo){
        DBG_INFO << "Force set:" << allow
                 << "Wallet Id:" << m_walletInfo.data()->walletId()
                 << "Wallet Role:" << m_walletInfo.data()->myRole()
                 << "Wallet Slug:" << m_walletInfo.data()->slug()
                 << "Wallet Status:" << m_walletInfo.data()->status();
        QGroupWallets::instance()->setDashboardInfo(m_walletInfo);
        m_walletInfo.data()->RequestGetCoins();
    }
}

void AppModel::setWalletInfoByIndex(const int index, bool force)
{
    if(index == -1){
        setWalletInfo(QWalletPtr(new Wallet()), force);
    }
    else{
        if(m_walletList){
            QWalletPtr newWallet = m_walletList.data()->getWalletByIndex(index);
            if(!newWallet){
                newWallet = QWalletPtr(new Wallet());
            }
            setWalletInfo(newWallet, force);
        }
    }
}

Transaction *AppModel::transactionInfo() const
{
    return m_transactionInfo.data();
}

QTransactionPtr AppModel::transactionInfoPtr() const
{
    return m_transactionInfo;
}

void AppModel::setTransactionInfo(const QTransactionPtr& d)
{
    if(d){
        m_transactionInfo = d;
    }
    else {
        m_transactionInfo = QTransactionPtr(new Transaction());
    }
    emit transactionInfoChanged();
}

DestinationListModel *AppModel::destinationList() const
{
    return m_destinationList.data();
}

QDestinationListModelPtr AppModel::destinationListPtr() const
{
    return m_destinationList;
}

void AppModel::setDestinationList(const QDestinationListModelPtr &destinationList)
{
    m_destinationList = destinationList;
}

QString AppModel::addressBalance() const
{
    if((int)AppSetting::Unit::SATOSHI == AppSetting::instance()->unit()){
        QLocale locale(QLocale::English);
        return locale.toString(m_addressBalance);
    }
    else{
        return qUtils::QValueFromAmount(m_addressBalance);
    }
}

void AppModel::setAddressBalance(const qint64 addressBalance)
{
    if(m_addressBalance != addressBalance){
        m_addressBalance = addressBalance;
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

        //Get taproot supported
        Draco::instance()->GetTaprootSupportedCached(true);
    });
}

void AppModel::timerCheckAuthorizedHandle()
{
    checkAutoSignOut();
    Draco::instance()->getMe();
    requestSyncGroups();
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
        walletImported.data()->setWalletName(name);
        walletList()->addWallet(walletImported);
        resetSignersChecked();
        walletList()->requestSort(WalletListModel::WalletRoles::wallet_createDate_Role, Qt::AscendingOrder);
        int index = walletList()->getWalletIndexById(walletImported.data()->walletId());
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

bool AppModel::isValidXPRV(const QString &xprv)
{
    QWarningMessage msg;
    bool ret =  qUtils::isValidXPRV(xprv, msg);
    if(!ret) {
        if((int)EWARNING::WarningType::EXCEPTION_MSG == msg.type()){
            showToast(msg.code(), msg.what(), (EWARNING::WarningType)msg.type());
        }
        // else {
        //     QString what = "Invalid XPRV";
        //     showToast(0, what, EWARNING::WarningType::EXCEPTION_MSG);
        // }
    }
    return ret;
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
    return m_qrExported;
}

void AppModel::setQrExported(const QStringList &qrExported)
{
    m_qrExported = qrExported;
    emit qrExportedChanged();
}

QWarningMessagePtr AppModel::warningMessagePtr() const
{
    return m_warningMessage;
}

QWarningMessage* AppModel::warningMessage() const
{
    return m_warningMessage.data();
}

void AppModel::setWarningMessage(const QWarningMessagePtr &warningMessage)
{
    m_warningMessage = warningMessage;
    emit warningMessageChanged();
}

double AppModel::exchangeRates() const
{
    return m_exchangeRates;
}

void AppModel::setExchangeRates(double exchangeRates)
{
    if(m_exchangeRates != exchangeRates && exchangeRates > 0){
        m_exchangeRates = exchangeRates;
        emit btcRatesChanged();
        emit exchangeRatesChanged();
    }
}

double AppModel::btcRates() const
{
    return m_btcRates;
}

void AppModel::setBtcRates(double btcRates)
{
    if(m_btcRates != btcRates && btcRates > 0){
        m_btcRates = btcRates;
        emit btcRatesChanged();
        emit exchangeRatesChanged();
    }
}

QVariantList AppModel::listMessage() const
{
    return m_listMessage;
}

void AppModel::showToast(int code, const QString &what, EWARNING::WarningType type){
    emit forwardToast(code, what, type);
}

void AppModel::recieveToast(int code, const QString &what, EWARNING::WarningType type)
{
    QThreadForwarder::instance()->forwardInQueuedConnection([code, type, what](){
        timeoutHandler(500, [code, what, type]() {
            QWarningMessage message;
            message.setWarningMessage(code, what, type);
            QJsonObject data;
            data["contentDisplay"] = message.contentDisplay();
            data["explaination"  ] = message.explaination();
            data["what"          ] = message.what();
            data["code"          ] = message.code();
            data["type"          ] = message.type();
            data["popupType"     ] = message.popupType();
            QEventProcessor::instance()->sendToastMessage(QVariant::fromValue(data));
        });
    });
}

qint64 AppModel::qAmountFromBTC(const QString &btcValue)
{
    return qUtils::QAmountFromValue(btcValue);
}

QString AppModel::qBTCFromAmount(const qint64 amount)
{
    return qUtils::QValueFromAmount(amount);
}

QString AppModel::qCurrencyFromAmount(const qint64 amountSats)
{
    return qUtils::currencyLocale(amountSats);
}

qint64 AppModel::qAmountFromCurrency(const QString &currency)
{
    return qUtils::QAmountFromCurrency(currency);
}
