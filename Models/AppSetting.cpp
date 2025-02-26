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
#include "AppSetting.h"
#include "AppModel.h"
#include "Servers/Draco.h"
#include "QOutlog.h"

NunchukSettings::NunchukSettings():
    QSettings(QSettings::NativeFormat, QSettings::UserScope, qApp->organizationName(), qApp->applicationName()),
    m_group("")
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

NunchukSettings::~NunchukSettings()
{

}

QString NunchukSettings::groupSetting() const
{
    return m_group;
}

void NunchukSettings::setGroupSetting(QString group)
{
    DBG_INFO << "Settings with [" << group << "]";
    m_group = group;
}

bool NunchukSettings::contains(const QString &key) const
{
    QString realkey = m_group == "" ? key : QString("%1/%2").arg(m_group).arg(key);
    return QSettings::contains(realkey);
}

void NunchukSettings::setValue(const QString &key, const QVariant &value)
{
    QString realkey = m_group == "" ? key : QString("%1/%2").arg(m_group).arg(key);
    QSettings::setValue(realkey, value);
}

void NunchukSettings::setValueCommon(const QString &key, const QVariant &value)
{
    QSettings::setValue(key, value);
}

QVariant NunchukSettings::value(const QString &key, const QVariant &defaultValue) const
{
    QString realkey = m_group == "" ? key : QString("%1/%2").arg(m_group).arg(key);
    return QSettings::value(realkey, defaultValue);
}

QVariant NunchukSettings::valueCommon(const QString &key, const QVariant &defaultValue) const
{
    return QSettings::value(key, defaultValue);
}

void NunchukSettings::removeKey(const QString &key)
{
    QString realkey = m_group == "" ? key : QString("%1/%2").arg(m_group).arg(key);
    QSettings::remove(realkey);
}

bool NunchukSettings::containsCommon(const QString &key) const
{
    return QSettings::contains(key);
}

void NunchukSettings::setCommonValue(const QString &key, const QVariant &value)
{
    QSettings::setValue(key, value);
}

QVariant NunchukSettings::commonValue(const QString &key, const QVariant &defaultValue) const
{
    return QSettings::value(key, defaultValue);
}

AppSetting::AppSetting() :
    unit_((int)Unit::BTC),
    mainnetServer_(MAINNET_SERVER),
    testnetServer_(TESTNET_SERVER),
    signetServer_(SIGNET_SERVER),
    enableDualServer_(false),
    enableCustomizeHWIDriver_(false),
    hwiPath_(HWI_PATH),
    enableDBEncryption_(false),
    enableTorProxy_(false),
    torProxyAddress_(LOCAL_ADDRESS),
    torProxyPort_(TOR_PORT),
    torProxyName_(""),
    torProxyPassword_(""),
    primaryServer_((int)Chain::MAIN),
    secondaryServer_(""),
    enableFixedPrecision_(false),
    storagePath_(""),
    connectionState_((int)ConnectionStatus::SYNCING),
    syncPercent_(0),
    enableCertificateFile_(false),
    certificateFile_(""),
    enableCoreRPC_(false),
    coreRPCAddress_(LOCAL_ADDRESS),
    coreRPCPort_(primaryServer_ == (int)Chain::TESTNET ? CORERPC_TESTNET_PORT : CORERPC_MAINNET_PORT),
    coreRPCName_(""),
    coreRPCPassword_(""),
    enableSignetStream_(false),
    signetStream_(GLOBAL_SIGNET_EXPLORER),
    enableDebugMode_(false),
    enableMultiDeviceSync_(false),
    isStarted_(false),
    isFirstTimeOnboarding_(false)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
    DBG_INFO << "Setting in:" << NunchukSettings::fileName();
    this->disconnect();
}

AppSetting::~AppSetting() {
    this->disconnect();
}

AppSetting *AppSetting::instance()
{
    static AppSetting mInstance;
    return &mInstance;
}

void AppSetting::refresh()
{
    emit unitChanged();
    emit currencyChanged();
}

void AppSetting::setGroupSetting(QString group) {
    NunchukSettings::setGroupSetting(group);
    refresh();
}

void AppSetting::resetSetting()
{
    this->setUnit((int)Unit::BTC);
    this->setMainnetServer(MAINNET_SERVER);
    this->setTestnetServer(TESTNET_SERVER);
    this->setSignetServer(SIGNET_SERVER);
    this->setEnableDualServer(false);
    this->setEnableCustomizeHWIDriver(false);
    this->setHwiPath(HWI_PATH);
    this->setEnableDBEncryption(false);
    this->setEnableTorProxy(false);
    this->setTorProxyAddress(LOCAL_ADDRESS);
    this->setTorProxyPort(TOR_PORT);
    this->setTorProxyName("");
    this->setTorProxyPassword("");
    this->setPrimaryServer((int)Chain::TESTNET);
    this->setSecondaryServer("");
    this->setEnableFixedPrecision(false);
    this->setEnableCertificateFile(false);
    this->setCertificateFile("");
    this->setEnableCoreRPC(false);
    this->setCoreRPCAddress(LOCAL_ADDRESS);
    this->setCoreRPCPort(primaryServer_ == (int)Chain::TESTNET ? CORERPC_TESTNET_PORT : CORERPC_MAINNET_PORT);
    this->setCoreRPCName("");
    this->setCoreRPCPassword("");
    this->setSignetStream(GLOBAL_SIGNET_EXPLORER);
    this->setEnableMultiDeviceSync(false);
    this->setCurrency("USD");
}

void AppSetting::updateUnit()
{
    if(AppModel::instance()->walletList()){
        AppModel::instance()->walletList()->notifyUnitChanged();
    }
    if(AppModel::instance()->walletInfo()){
        if(AppModel::instance()->walletInfo()->transactionHistory()){
            AppModel::instance()->walletInfo()->transactionHistory()->notifyUnitChanged();
        }
        if(AppModel::instance()->walletInfo()->utxoList()){
            AppModel::instance()->walletInfo()->utxoList()->notifyUnitChanged();
        }
    }
    emit currencyChanged();
}

int AppSetting::unit()
{
    if(NunchukSettings::contains("unit")){
        unit_ = NunchukSettings::value("unit").toInt();
    }
    else{
        NunchukSettings::setValue("unit", unit_);
    }
    return unit_;
}

void AppSetting::setUnit(int unit)
{
    if(unit_ != unit){
        unit_ = unit;
        NunchukSettings::setValue("unit", unit_);
        emit unitChanged();
        updateUnit();
    }
}

QString AppSetting::mainnetServer()
{
    if(NunchukSettings::contains("mainnestServer")){
        mainnetServer_ = NunchukSettings::value("mainnestServer").toString();
    }
    else{
        NunchukSettings::setValue("mainnestServer", mainnetServer_);
    }
    return mainnetServer_;
}

void AppSetting::setMainnetServer(const QString &mainnestServer)
{
    if(mainnetServer_ != mainnestServer){
        mainnetServer_ = mainnestServer;
        NunchukSettings::setValue("mainnestServer", mainnetServer_);
        emit mainnetServerChanged();
    }
}

QString AppSetting::testnetServer()
{
    if(NunchukSettings::contains("testnetServer")){
        testnetServer_ = NunchukSettings::value("testnetServer").toString();
    }
    else{
        NunchukSettings::setValue("testnetServer", testnetServer_);
    }
    return testnetServer_;
}

void AppSetting::setTestnetServer(const QString &testnetServer)
{
    if(testnetServer_ != testnetServer){
        testnetServer_ = testnetServer;
        NunchukSettings::setValue("testnetServer", testnetServer_);
        emit testnetServerChanged();
    }
}

QString AppSetting::signetServer()
{
    if(NunchukSettings::contains("signetServer")){
        signetServer_ = NunchukSettings::value("signetServer").toString();
    }
    else{
        NunchukSettings::setValue("signetServer", signetServer_);
    }
    return signetServer_;
}

void AppSetting::setSignetServer(const QString &signetServer)
{
    if (signetServer_ != signetServer){
        signetServer_ = signetServer;
        NunchukSettings::setValue("signetServer", signetServer_);
        emit signetServerChanged();
    }
}

bool AppSetting::enableDualServer()
{
    if(NunchukSettings::contains("enableDualServer")){
        enableDualServer_ = NunchukSettings::value("enableDualServer").toBool();
    }
    else{
        NunchukSettings::setValue("enableDualServer", enableDualServer_);
    }
    return enableDualServer_;
}

void AppSetting::setEnableDualServer(bool enableDualServer)
{
    if(enableDualServer_ != enableDualServer){
        enableDualServer_ = enableDualServer;
        NunchukSettings::setValue("enableDualServer", enableDualServer_);
        emit enableDualServerChanged();
    }
}

bool AppSetting::enableCustomizeHWIDriver()
{
    if(NunchukSettings::contains("enableCustomizeHWIDriver")){
        enableCustomizeHWIDriver_ = NunchukSettings::value("enableCustomizeHWIDriver").toBool();
    }
    else{
        NunchukSettings::setValue("enableCustomizeHWIDriver", enableCustomizeHWIDriver_);
    }
    return enableCustomizeHWIDriver_;
}

void AppSetting::setEnableCustomizeHWIDriver(bool enableCustomizeHWIDriver)
{
    if(enableCustomizeHWIDriver_ != enableCustomizeHWIDriver){
        enableCustomizeHWIDriver_ = enableCustomizeHWIDriver;
        NunchukSettings::setValue("enableCustomizeHWIDriver", enableCustomizeHWIDriver_);
        emit enableCustomizeHWIDriverChanged();
    }
}

QString AppSetting::hwiPath()
{
    if(NunchukSettings::contains("hwiPath")){
        hwiPath_ = NunchukSettings::value("hwiPath").toString();
    }
    else{
        NunchukSettings::setValue("hwiPath", hwiPath_);
    }
    return hwiPath_;
}

void AppSetting::setHwiPath(const QString &hwiPath)
{
    if(hwiPath_ != hwiPath){
        hwiPath_ = hwiPath;
        NunchukSettings::setValue("hwiPath", hwiPath_);
        emit hwiPathChanged();
    }
}

bool AppSetting::enableDBEncryption()
{
    if(NunchukSettings::contains("enableDBEncryption")){
        enableDBEncryption_ = NunchukSettings::value("enableDBEncryption").toBool();
    }
    else{
        NunchukSettings::setValue("enableDBEncryption", enableDBEncryption_);
    }
    return enableDBEncryption_;
}

void AppSetting::setEnableDBEncryption(bool enableDBEncryption)
{
    enableDBEncryption_ = enableDBEncryption;
    NunchukSettings::setValue("enableDBEncryption", enableDBEncryption_);
    emit enableDBEncryptionChanged();
}

bool AppSetting::enableTorProxy()
{
    if(NunchukSettings::contains("enableTorProxy")){
        enableTorProxy_ = NunchukSettings::value("enableTorProxy").toBool();
    }
    else{
        NunchukSettings::setValue("enableTorProxy", enableTorProxy_);
    }
    return enableTorProxy_;
}

void AppSetting::setEnableTorProxy(bool enableTorProxy)
{
    if(enableTorProxy_ != enableTorProxy){
        enableTorProxy_ = enableTorProxy;
        NunchukSettings::setValue("enableTorProxy", enableTorProxy_);
        emit enableTorProxyChanged();
    }
}

QString AppSetting::torProxyAddress()
{
    if(NunchukSettings::contains("torProxyAddress")){
        torProxyAddress_ = NunchukSettings::value("torProxyAddress").toString();
    }
    else{
        NunchukSettings::setValue("torProxyAddress", torProxyAddress_);
    }
    return torProxyAddress_;
}

void AppSetting::setTorProxyAddress(const QString &torProxyAddress)
{
    if(torProxyAddress_ != torProxyAddress){
        torProxyAddress_ = torProxyAddress;
        NunchukSettings::setValue("torProxyAddress", torProxyAddress_);
        emit torProxyAddressChanged();
    }
}

int AppSetting::torProxyPort()
{
    if(NunchukSettings::contains("torProxyPort")){
        torProxyPort_ = NunchukSettings::value("torProxyPort").toInt();
    }
    else{
        NunchukSettings::setValue("torProxyPort", torProxyPort_);
    }
    return torProxyPort_;
}

void AppSetting::setTorProxyPort(const int torProxyPort)
{
    if(torProxyPort_ != torProxyPort){
        torProxyPort_ = torProxyPort;
        NunchukSettings::setValue("torProxyPort", torProxyPort_);
        emit torProxyPortChanged();
    }
}

QString AppSetting::torProxyName()
{
    if(NunchukSettings::contains("torProxyName")){
        torProxyName_ = NunchukSettings::value("torProxyName").toString();
    }
    else{
        NunchukSettings::setValue("torProxyName", torProxyName_);
    }
    return torProxyName_;
}

void AppSetting::setTorProxyName(const QString &torProxyName)
{
    if(torProxyName_ != torProxyName){
        torProxyName_ = torProxyName;
        NunchukSettings::setValue("torProxyName", torProxyName_);
        emit torProxyNameChanged();
    }
}

QString AppSetting::torProxyPassword()
{
    if(NunchukSettings::contains("torProxyPassword")){
        torProxyPassword_ = NunchukSettings::value("torProxyPassword").toString();
    }
    else{
        NunchukSettings::setValue("torProxyPassword", torProxyPassword_);
    }
    return torProxyPassword_;
}

void AppSetting::setTorProxyPassword(const QString &torProxyPassword)
{
    if(torProxyPassword_ != torProxyPassword){
        torProxyPassword_ = torProxyPassword;
        NunchukSettings::setValue("torProxyPassword", torProxyPassword_);
        emit torProxyPasswordChanged();
    }
}

int AppSetting::primaryServer()
{
    if(NunchukSettings::contains("primaryServer")){
        primaryServer_ = NunchukSettings::valueCommon("primaryServer").toInt();
    }
    else{
        NunchukSettings::setValueCommon("primaryServer", primaryServer_);
    }
    return primaryServer_;
}

void AppSetting::setPrimaryServer(int primaryServer)
{
    if(primaryServer_ != primaryServer){
        primaryServer_ = primaryServer;
        NunchukSettings::setValueCommon("primaryServer", primaryServer_);
        emit primaryServerChanged();
    }
}

QString AppSetting::secondaryServer()
{
    if(NunchukSettings::contains("secondaryServer")){
        secondaryServer_ = NunchukSettings::value("secondaryServer").toString();
    }
    else{
        NunchukSettings::setValue("secondaryServer", secondaryServer_);
    }
    return secondaryServer_;
}

void AppSetting::setSecondaryServer(const QString &secondaryServer)
{
    if(secondaryServer_ != secondaryServer){
        secondaryServer_ = secondaryServer;
        NunchukSettings::setValue("secondaryServer", secondaryServer_);
        emit secondaryServerChanged();
    }
}

bool AppSetting::enableFixedPrecision()
{
    if(NunchukSettings::contains("enableFixedPrecision")){
        enableFixedPrecision_ = NunchukSettings::value("enableFixedPrecision").toBool();
    }
    else{
        NunchukSettings::setValue("enableFixedPrecision", enableFixedPrecision_);
    }
    return enableFixedPrecision_;
}

void AppSetting::setEnableFixedPrecision(bool enableFixedPrecision)
{
    if(enableFixedPrecision_ != enableFixedPrecision){
        enableFixedPrecision_ = enableFixedPrecision;
        NunchukSettings::setValue("enableFixedPrecision", enableFixedPrecision_);
        emit enableFixedPrecisionChanged();
        if((int)Unit::BTC == unit()){
            updateUnit();
        }
    }
}

QString AppSetting::certificateFile()
{
    if(NunchukSettings::contains("certificateFile")){
        certificateFile_ = NunchukSettings::value("certificateFile").toString();
        if(certificateFile_.isEmpty() || certificateFile_.isNull() || (certificateFile_ =="")){
            setEnableCertificateFile(false);
        }
    }
    else{
        NunchukSettings::setValue("certificateFile", certificateFile_);
    }
    return certificateFile_;
}

void AppSetting::setCertificateFile(const QString &certificateFile)
{
    if(certificateFile_ != certificateFile){
        certificateFile_ = certificateFile;
        NunchukSettings::setValue("certificateFile", certificateFile_);
        emit certificateFileChanged();
    }
}

bool AppSetting::enableCertificateFile()
{
    if(NunchukSettings::contains("enableCertificateFile")){
        enableCertificateFile_ = NunchukSettings::value("enableCertificateFile").toBool();
    }
    else{
        NunchukSettings::setValue("enableCertificateFile", enableCertificateFile_);
    }
    return enableCertificateFile_;
}

void AppSetting::setEnableCertificateFile(bool enableCertificateFile)
{
    if(enableCertificateFile_ != enableCertificateFile){
        enableCertificateFile_ = enableCertificateFile;
        NunchukSettings::setValue("enableCertificateFile", enableCertificateFile_);
        emit enableCertificateFileChanged();
    }
}

QString AppSetting::storagePath()
{
    auto path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (path.isEmpty()) {
        DBG_INFO << "Cannot determine settings storage location";
    }
    else {
        QDir d{ path };
        if (d.mkpath(d.absolutePath()) && QDir::setCurrent(d.absolutePath())) {
            storagePath_ = QDir::currentPath();
        }
    }
    return storagePath_;
}

QString AppSetting::executePath() const
{
    return qApp->applicationDirPath();
}

int AppSetting::connectionState() const
{
    return connectionState_;
}

void AppSetting::setConnectionState(int connectionState)
{
    if(connectionState_ != connectionState){
        connectionState_ = connectionState;
        emit connectionStateChanged();
    }
}

bool AppSetting::enableCoreRPC()
{
    if(NunchukSettings::contains("enableCoreRPC")){
        enableCoreRPC_ = NunchukSettings::value("enableCoreRPC").toBool();
    }
    else{
        NunchukSettings::setValue("enableCoreRPC", enableCoreRPC_);
    }
    return enableCoreRPC_;
}

void AppSetting::setEnableCoreRPC(bool enableCoreRPC)
{
    if(enableCoreRPC_ != enableCoreRPC){
        enableCoreRPC_ = enableCoreRPC;
        NunchukSettings::setValue("enableCoreRPC", enableCoreRPC_);
        emit enableCoreRPCChanged();
    }
}

QString AppSetting::coreRPCAddress()
{
    if(NunchukSettings::contains("coreRPCAddress")){
        coreRPCAddress_ = NunchukSettings::value("coreRPCAddress").toString();
    }
    else{
        NunchukSettings::setValue("coreRPCAddress", coreRPCAddress_);
    }
    return coreRPCAddress_;
}

void AppSetting::setCoreRPCAddress(const QString &coreRPCAddress)
{
    if(coreRPCAddress_ != coreRPCAddress){
        coreRPCAddress_ = coreRPCAddress;
        NunchukSettings::setValue("coreRPCAddress", coreRPCAddress_);
        emit coreRPCAddressChanged();
    }
}

int AppSetting::coreRPCPort()
{
    if(NunchukSettings::contains("coreRPCPort")){
        coreRPCPort_ = NunchukSettings::value("coreRPCPort").toInt();
    }
    else{
        NunchukSettings::setValue("coreRPCPort", coreRPCPort_);
    }
    return coreRPCPort_;
}

void AppSetting::setCoreRPCPort(const int coreRPCPort)
{
    if(coreRPCPort_ != coreRPCPort){
        coreRPCPort_ = coreRPCPort;
        NunchukSettings::setValue("coreRPCPort", coreRPCPort_);
        emit coreRPCPortChanged();
    }
}

QString AppSetting::coreRPCName()
{
    if(NunchukSettings::contains("coreRPCName")){
        coreRPCName_ = NunchukSettings::value("coreRPCName").toString();
    }
    else{
        NunchukSettings::setValue("coreRPCName", coreRPCName_);
    }
    return coreRPCName_;
}

void AppSetting::setCoreRPCName(const QString &coreRPCName)
{
    if(coreRPCName_ != coreRPCName){
        coreRPCName_ = coreRPCName;
        NunchukSettings::setValue("coreRPCName", coreRPCName_);
        emit coreRPCNameChanged();
    }
}

QString AppSetting::coreRPCPassword()
{
    if(NunchukSettings::contains("coreRPCPassword")){
        coreRPCPassword_ = NunchukSettings::value("coreRPCPassword").toString();
    }
    else{
        NunchukSettings::setValue("coreRPCPassword", coreRPCPassword_);
    }
    return coreRPCPassword_;
}

void AppSetting::setCoreRPCPassword(const QString &coreRPCPassword)
{
    if(coreRPCPassword_ != coreRPCPassword){
        coreRPCPassword_ = coreRPCPassword;
        NunchukSettings::setValue("coreRPCPassword", coreRPCPassword_);
        emit coreRPCPasswordChanged();
    }
}

int AppSetting::syncPercent() const
{
    return syncPercent_;
}

void AppSetting::setSyncPercent(int syncPercent)
{
    if(syncPercent_ != syncPercent){
        syncPercent_ = syncPercent;
        emit syncPercentChanged();
    }
}

QString AppSetting::signetStream()
{
    if(NunchukSettings::contains("signetStream")){
        signetStream_ = NunchukSettings::value("signetStream").toString();
    }
    else{
        NunchukSettings::setValue("signetStream", signetStream_);
    }
    return signetStream_;
}

void AppSetting::setSignetStream(const QString &signetStream)
{
    if (signetStream_ != signetStream){
        signetStream_ = signetStream;
        NunchukSettings::setValue("signetStream", signetStream_);
        emit signetStreamChanged();
    }
}

bool AppSetting::enableSignetStream()
{
    if(NunchukSettings::contains("enableSignetStream")){
        enableSignetStream_ = NunchukSettings::value("enableSignetStream").toBool();
    }
    else{
        NunchukSettings::setValue("enableSignetStream", enableSignetStream_);
    }
    return enableSignetStream_;
}

void AppSetting::setEnableSignetStream(const bool &enableSignetStream)
{
    if (enableSignetStream_ != enableSignetStream){
        enableSignetStream_ = enableSignetStream;
        NunchukSettings::setValue("enableSignetStream", enableSignetStream_);
        emit enableSignetStreamChanged();
    }
}

bool AppSetting::enableDebug()
{
#if 0
    if(NunchukSettings::contains("enableDebugMode")){
        enableDebugMode_ = NunchukSettings::value("enableDebugMode").toBool();
    }
    else{
        NunchukSettings::setValue("enableDebugMode", enableDebugMode_);
    }
#endif
    return enableDebugMode_;
}

void AppSetting::setEnableDebug(bool enableDebugMode)
{
    if (enableDebugMode_ != enableDebugMode){
        enableDebugMode_ = enableDebugMode;
#if 0
        //    NunchukSettings::setValue("enableDebugMode", enableDebugMode_);
#endif
        emit enableDebugChanged();
    }
}

bool AppSetting::enableColab()
{
    if(NunchukSettings::contains("enableColab")){
        enableColab_ = NunchukSettings::value("enableColab").toBool();
    }
    else{
        NunchukSettings::setValue("enableColab", enableColab_);
    }
    return enableColab_;
}

void AppSetting::setEnableColab(bool enableColab)
{
    if (enableColab_ != enableColab){
        enableColab_ = enableColab;
        NunchukSettings::setValue("enableColab", enableColab_);
        emit enableColabChanged();
    }
}

bool AppSetting::isStarted()
{
    return isStarted_ || NunchukSettings::commonValue("isStarted").toBool();
}

void AppSetting::setIsStarted(bool isStarted, bool isSetting)
{
    if(isSetting){
        NunchukSettings::setCommonValue("isStarted", isStarted);
        emit isStartedChanged();
    }
    else{
        isStarted_ = isStarted;
    }
}

bool AppSetting::enableMultiDeviceSync()
{
    // Hide for all user
    if(NunchukSettings::contains("enableMultiDeviceSync")){
        enableMultiDeviceSync_ = NunchukSettings::value("enableMultiDeviceSync").toBool();
        if(enableMultiDeviceSync_){
            setEnableMultiDeviceSync(false);
        }
    }
    return false; // Hide for all user
#if 0
    if(NunchukSettings::contains("enableMultiDeviceSync")){
        enableMultiDeviceSync_ = NunchukSettings::value("enableMultiDeviceSync").toBool();
    }
    else{
        NunchukSettings::setValue("enableMultiDeviceSync", enableMultiDeviceSync_);
    }
    return enableMultiDeviceSync_;
#endif
}

void AppSetting::setEnableMultiDeviceSync(bool enableMultiDeviceSync)
{
    if(enableMultiDeviceSync_ != enableMultiDeviceSync){
        enableMultiDeviceSync_ = enableMultiDeviceSync;
        NunchukSettings::setValue("enableMultiDeviceSync", enableMultiDeviceSync_);
        emit enableMultiDeviceSyncChanged();
        AppModel::instance()->startMultiDeviceSync(enableMultiDeviceSync_);
    }
}

QString AppSetting::currencySymbol()
{
    if(qUtils::strCompare(currency(), "USD")){
        return "$";
    }
    else{
        return "";
    }
}

QString AppSetting::currency()
{
    if(NunchukSettings::contains("currency")){
        return NunchukSettings::value("currency").toString();
    }
    else{
        return "USD";
    }
}

void AppSetting::setCurrency(QString currency)
{
    NunchukSettings::setValue("currency", currency);
    emit currencyChanged();
}

void AppSetting::setWalletCached(QString id, QWalletCached<QString, QString, QString, QString> data)
{
    QByteArray dataByteArray;
    QDataStream stream(&dataByteArray, QIODevice::WriteOnly);
    stream << data.first << data.second << data.third;
    NunchukSettings::setValue(id, dataByteArray);
}

bool AppSetting::getwalletCached(QString id, QWalletCached<QString, QString, QString, QString> &result)
{
    if(NunchukSettings::contains(id)){
        QByteArray dataByteArray = NunchukSettings::value(id).toByteArray();
        QDataStream stream(&dataByteArray, QIODevice::ReadOnly);
        stream >> result.first >> result.second >> result.third >> result.fourth;
        DBG_INFO << "first:" << result.first
                 << "second:" << result.second
                 << "third:" << result.third
                 << "fourth:" << result.fourth;
        return true;
    }
    return false;
}

void AppSetting::deleteWalletCached(QString id)
{
    NunchukSettings::removeKey(id);
}

bool AppSetting::isFirstTimeOnboarding()
{
    isFirstTimeOnboarding_ = false;
    if(NunchukSettings::contains("isFirstTimeOnboarding")){
        isFirstTimeOnboarding_ = NunchukSettings::value("isFirstTimeOnboarding").toBool();
    }
    else{
        NunchukSettings::setValue("isFirstTimeOnboarding", isFirstTimeOnboarding_);
    }
    return isFirstTimeOnboarding_;
}

void AppSetting::setIsFirstTimeOnboarding(bool isFirstTime)
{
    if (isFirstTimeOnboarding_ != isFirstTime){
        isFirstTimeOnboarding_ = isFirstTime;
        NunchukSettings::setValue("isFirstTimeOnboarding", isFirstTimeOnboarding_);
        emit isFirstTimeOnboardingChanged();
    }
}

QVariant AppSetting::getMainnetList()
{
    return NunchukSettings::commonValue("mainnetList");
}

void AppSetting::setMainnetList(const QVariant &mainnetList)
{
    NunchukSettings::setCommonValue("mainnetList", mainnetList);
}

QVariant AppSetting::getReminderStates()
{
    return NunchukSettings::value("reminderStates");
}

void AppSetting::setReminderStates(const QVariant &states)
{
    NunchukSettings::setValue("reminderStates", states);
}

QStringList AppSetting::favoriteAddresses()
{
    return m_favoriteAddresses;
}

void AppSetting::loadFavoriteAddresses()
{
    qApp->setOverrideCursor(Qt::WaitCursor);
    if(CLIENT_INSTANCE->isSubscribed()){
        QJsonObject output;
        QString errormsg = "";
        bool ret = Draco::instance()->GetSavedAddress(output, errormsg);
        DBG_INFO << "Load favorite addresses from server" << output << errormsg;
        if(ret){
            QJsonArray addresses = output["addresses"].toArray();
            QStringList favoriteAddresses;
            foreach (QJsonValue value, addresses) {
                QString label = value.toObject()["label"].toString();
                QString address = value.toObject()["address"].toString();
                favoriteAddresses.append(QString("%1[split]%2").arg(label).arg(address));
            }
            m_favoriteAddresses = favoriteAddresses;
        }
    }
    else{
        QString key = QString("%1/favoriteAddresses").arg(AppSetting::instance()->primaryServer());
        if(NunchukSettings::contains(key)){
            m_favoriteAddresses = NunchukSettings::value(key).toStringList();
        }
        else{
            NunchukSettings::setValue(key, m_favoriteAddresses);
        }
    }
    qApp->restoreOverrideCursor();
    emit favoriteAddressesChanged();
}

void AppSetting::setFavoriteAddresses(const QStringList &newFavoriteAddresses)
{
    m_favoriteAddresses = newFavoriteAddresses;
    if(!CLIENT_INSTANCE->isSubscribed()){
        QString key = QString("%1/favoriteAddresses").arg(AppSetting::instance()->primaryServer());
        NunchukSettings::setValue(key, m_favoriteAddresses);
    }
    emit favoriteAddressesChanged();
}

void AppSetting::removeFavoriteAddress(const QString &label, const QString &address)
{
    qApp->setOverrideCursor(Qt::WaitCursor);
    if(CLIENT_INSTANCE->isSubscribed()){
        QJsonObject output;
        QString errormsg = "";
        bool ret = Draco::instance()->DeleteSavedAddress(label, address, output, errormsg);
        if(ret){
            QJsonArray addresses = output["addresses"].toArray();
            QStringList favoriteAddresses;
            foreach (QJsonValue value, addresses) {
                QString labelValue = value.toObject()["label"].toString();
                QString addressValue = value.toObject()["address"].toString();
                favoriteAddresses.append(QString("%1[split]%2").arg(labelValue).arg(addressValue));
            }
            setFavoriteAddresses(favoriteAddresses);
        }
    }
    else {
        QString newaddress = QString("%1[split]%2").arg(label).arg(address);
        loadFavoriteAddresses();
        QStringList addresses = favoriteAddresses();
        if(addresses.contains(newaddress)){
            addresses.removeAll(newaddress);
            setFavoriteAddresses(addresses);
        }
    }
    qApp->restoreOverrideCursor();
}

void AppSetting::addFavoriteAddress(const QString &label, const QString &address)
{
    qApp->setOverrideCursor(Qt::WaitCursor);
    if(CLIENT_INSTANCE->isSubscribed()){
        QJsonObject output;
        QString errormsg = "";
        bool ret = Draco::instance()->AddOrUpdateSavedAddress(label, address, output, errormsg);
        if(ret){
            emit favoriteAddressesChanged();
        }
    }
    else{
        QString newaddress = QString("%1[split]%2").arg(label).arg(address);
        loadFavoriteAddresses();
        QStringList addresses = favoriteAddresses();
        if(!addresses.contains(newaddress)){
            addresses.append(newaddress);
            setFavoriteAddresses(addresses);
        }
    }
    qApp->restoreOverrideCursor();
}

void AppSetting::updateFavoriteAddress(const QString &label, const QString &address)
{
    qApp->setOverrideCursor(Qt::WaitCursor);
    if(CLIENT_INSTANCE->isSubscribed()){
        QJsonObject output;
        QString errormsg = "";
        bool ret = Draco::instance()->AddOrUpdateSavedAddress(label, address, output, errormsg);
        if(ret){
            emit favoriteAddressesChanged();
        }
    }
    else{
        QString newaddress = QString("%1[split]%2").arg(label).arg(address);
        loadFavoriteAddresses();
        QStringList addresses = favoriteAddresses();
        QStringList addresses_updated;
        foreach (QString item, addresses) {
            QString value = item.split("[split]")[1];
            if(qUtils::strCompare(value, address)){
                addresses_updated.append(newaddress);
            }
            else{
                addresses_updated.append(item);
            }
        }
        setFavoriteAddresses(addresses_updated);
    }
    qApp->restoreOverrideCursor();
}

bool AppSetting::validateAddress(const QString &address)
{
    bool ret =qUtils::IsValidAddress(address);;
    // if(!ret){
    //     QString message = QString("Invalid address: %1").arg(address);
    //     AppModel::instance()->showToast(0, message, EWARNING::WarningType::EXCEPTION_MSG);
    // }
    return ret;
}
