#include "AppSetting.h"
#include "AppModel.h"
#include "QOutlog.h"

AppSetting::AppSetting() : unit_((int)Unit::BTC),
    mainnetServer_(MAINNET_SERVER),
    testnetServer_(TESTNET_SERVER),
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
    changePassphraseResult_((int)ChangePassphraseResult::NOT_YET_SET),
    enableCertificateFile_(false),
    certificateFile_(""),
    enableCoreRPC_(false),
    coreRPCAddress_(LOCAL_ADDRESS),
    coreRPCPort_(primaryServer_ == (int)Chain::TESTNET ? CORERPC_TESTNET_PORT : CORERPC_MAINNET_PORT),
    coreRPCName_(""),
    coreRPCPassword_(""),
    firstTimeCoreRPC_(true),
    firstTimePassPhrase_(true),
    settings_(QSettings::NativeFormat, QSettings::UserScope, qApp->organizationName(), qApp->applicationName())
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
    DBG_INFO << "Setting in:" << settings_.fileName();
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

void AppSetting::resetSetting()
{
    this->setUnit((int)Unit::BTC);
    this->setMainnetServer(MAINNET_SERVER);
    this->setTestnetServer(TESTNET_SERVER);
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
}

void AppSetting::updateUnit()
{
    if(AppModel::instance()->walletList()){
        AppModel::instance()->walletList()->notifyUnitChanged();
    }
    if(AppModel::instance()->transactionHistory()){
        AppModel::instance()->transactionHistory()->notifyUnitChanged();
    }
    if(AppModel::instance()->utxoList()){
        AppModel::instance()->utxoList()->notifyUnitChanged();
    }
}

int AppSetting::unit()
{
    if(settings_.contains("unit")){
        unit_ = settings_.value("unit").toInt();
    }
    else{
        settings_.setValue("unit", unit_);
    }
    return unit_;
}

void AppSetting::setUnit(int unit)
{
    if(unit_ != unit){
        unit_ = unit;
        settings_.setValue("unit", unit_);
        emit unitChanged();
        updateUnit();
    }
}

QString AppSetting::mainnetServer()
{
    if(settings_.contains("mainnestServer")){
        mainnetServer_ = settings_.value("mainnestServer").toString();
    }
    else{
        settings_.setValue("mainnestServer", mainnetServer_);
    }
    return mainnetServer_;
}

void AppSetting::setMainnetServer(const QString &mainnestServer)
{
    if(mainnetServer_ != mainnestServer){
        mainnetServer_ = mainnestServer;
        settings_.setValue("mainnestServer", mainnetServer_);
        emit mainnetServerChanged();
    }
}

QString AppSetting::testnetServer()
{
    if(settings_.contains("testnetServer")){
        testnetServer_ = settings_.value("testnetServer").toString();
    }
    else{
        settings_.setValue("testnetServer", testnetServer_);
    }
    return testnetServer_;
}

void AppSetting::setTestnetServer(const QString &testnetServer)
{
    if(testnetServer_ != testnetServer){
        testnetServer_ = testnetServer;
        settings_.setValue("testnetServer", testnetServer_);
        emit testnetServerChanged();
    }
}

bool AppSetting::enableDualServer()
{
    if(settings_.contains("enableDualServer")){
        enableDualServer_ = settings_.value("enableDualServer").toBool();
    }
    else{
        settings_.setValue("enableDualServer", enableDualServer_);
    }
    return enableDualServer_;
}

void AppSetting::setEnableDualServer(bool enableDualServer)
{
    if(enableDualServer_ != enableDualServer){
        enableDualServer_ = enableDualServer;
        settings_.setValue("enableDualServer", enableDualServer_);
        emit enableDualServerChanged();
    }
}

bool AppSetting::enableCustomizeHWIDriver()
{
    if(settings_.contains("enableCustomizeHWIDriver")){
        enableCustomizeHWIDriver_ = settings_.value("enableCustomizeHWIDriver").toBool();
    }
    else{
        settings_.setValue("enableCustomizeHWIDriver", enableCustomizeHWIDriver_);
    }
    return enableCustomizeHWIDriver_;
}

void AppSetting::setEnableCustomizeHWIDriver(bool enableCustomizeHWIDriver)
{
    if(enableCustomizeHWIDriver_ != enableCustomizeHWIDriver){
        enableCustomizeHWIDriver_ = enableCustomizeHWIDriver;
        settings_.setValue("enableCustomizeHWIDriver", enableCustomizeHWIDriver_);
        emit enableCustomizeHWIDriverChanged();
    }
}

QString AppSetting::hwiPath()
{
    if(settings_.contains("hwiPath")){
        hwiPath_ = settings_.value("hwiPath").toString();
    }
    else{
        settings_.setValue("hwiPath", hwiPath_);
    }
    return hwiPath_;
}

void AppSetting::setHwiPath(const QString &hwiPath)
{
    if(hwiPath_ != hwiPath){
        hwiPath_ = hwiPath;
        settings_.setValue("hwiPath", hwiPath_);
        emit hwiPathChanged();
    }
}

bool AppSetting::enableDBEncryption()
{
    if(settings_.contains("enableDBEncryption")){
        enableDBEncryption_ = settings_.value("enableDBEncryption").toBool();
    }
    else{
        settings_.setValue("enableDBEncryption", enableDBEncryption_);
    }
    return enableDBEncryption_;
}

void AppSetting::setEnableDBEncryption(bool enableDBEncryption)
{
    if(enableDBEncryption_ != enableDBEncryption){
        enableDBEncryption_ = enableDBEncryption;
        settings_.setValue("enableDBEncryption", enableDBEncryption_);
        emit enableDBEncryptionChanged();
    }
}

bool AppSetting::enableTorProxy()
{
    if(settings_.contains("enableTorProxy")){
        enableTorProxy_ = settings_.value("enableTorProxy").toBool();
    }
    else{
        settings_.setValue("enableTorProxy", enableTorProxy_);
    }
    return enableTorProxy_;
}

void AppSetting::setEnableTorProxy(bool enableTorProxy)
{
    if(enableTorProxy_ != enableTorProxy){
        enableTorProxy_ = enableTorProxy;
        settings_.setValue("enableTorProxy", enableTorProxy_);
        emit enableTorProxyChanged();
    }
}

QString AppSetting::torProxyAddress()
{
    if(settings_.contains("torProxyAddress")){
        torProxyAddress_ = settings_.value("torProxyAddress").toString();
    }
    else{
        settings_.setValue("torProxyAddress", torProxyAddress_);
    }
    return torProxyAddress_;
}

void AppSetting::setTorProxyAddress(const QString &torProxyAddress)
{
    if(torProxyAddress_ != torProxyAddress){
        torProxyAddress_ = torProxyAddress;
        settings_.setValue("torProxyAddress", torProxyAddress_);
        emit torProxyAddressChanged();
    }
}

int AppSetting::torProxyPort()
{
    if(settings_.contains("torProxyPort")){
        torProxyPort_ = settings_.value("torProxyPort").toInt();
    }
    else{
        settings_.setValue("torProxyPort", torProxyPort_);
    }
    return torProxyPort_;
}

void AppSetting::setTorProxyPort(const int torProxyPort)
{
    if(torProxyPort_ != torProxyPort){
        torProxyPort_ = torProxyPort;
        settings_.setValue("torProxyPort", torProxyPort_);
        emit torProxyPortChanged();
    }
}

QString AppSetting::torProxyName()
{
    if(settings_.contains("torProxyName")){
        torProxyName_ = settings_.value("torProxyName").toString();
    }
    else{
        settings_.setValue("torProxyName", torProxyName_);
    }
    return torProxyName_;
}

void AppSetting::setTorProxyName(const QString &torProxyName)
{
    if(torProxyName_ != torProxyName){
        torProxyName_ = torProxyName;
        settings_.setValue("torProxyName", torProxyName_);
        emit torProxyNameChanged();
    }
}

QString AppSetting::torProxyPassword()
{
    if(settings_.contains("torProxyPassword")){
        torProxyPassword_ = settings_.value("torProxyPassword").toString();
    }
    else{
        settings_.setValue("torProxyPassword", torProxyPassword_);
    }
    return torProxyPassword_;
}

void AppSetting::setTorProxyPassword(const QString &torProxyPassword)
{
    if(torProxyPassword_ != torProxyPassword){
        torProxyPassword_ = torProxyPassword;
        settings_.setValue("torProxyPassword", torProxyPassword_);
        emit torProxyPasswordChanged();
    }
}

int AppSetting::primaryServer()
{
    if(settings_.contains("primaryServer")){
        primaryServer_ = settings_.value("primaryServer").toInt();
    }
    else{
        settings_.setValue("primaryServer", primaryServer_);
    }
    return primaryServer_;
}

void AppSetting::setPrimaryServer(int primaryServer)
{
    if(primaryServer_ != primaryServer){
        primaryServer_ = primaryServer;
        settings_.setValue("primaryServer", primaryServer_);
        emit primaryServerChanged();
    }
}

QString AppSetting::secondaryServer()
{
    if(settings_.contains("secondaryServer")){
        secondaryServer_ = settings_.value("secondaryServer").toString();
    }
    else{
        settings_.setValue("secondaryServer", secondaryServer_);
    }
    return secondaryServer_;
}

void AppSetting::setSecondaryServer(const QString &secondaryServer)
{
    if(secondaryServer_ != secondaryServer){
        secondaryServer_ = secondaryServer;
        settings_.setValue("secondaryServer", secondaryServer_);
        emit secondaryServerChanged();
    }
}

bool AppSetting::enableFixedPrecision()
{
    if(settings_.contains("enableFixedPrecision")){
        enableFixedPrecision_ = settings_.value("enableFixedPrecision").toBool();
    }
    else{
        settings_.setValue("enableFixedPrecision", enableFixedPrecision_);
    }
    return enableFixedPrecision_;
}

void AppSetting::setEnableFixedPrecision(bool enableFixedPrecision)
{
    if(enableFixedPrecision_ != enableFixedPrecision){
        enableFixedPrecision_ = enableFixedPrecision;
        settings_.setValue("enableFixedPrecision", enableFixedPrecision_);
        emit enableFixedPrecisionChanged();
        if((int)Unit::BTC == unit()){
            updateUnit();
        }
    }
}

QString AppSetting::certificateFile()
{
    if(settings_.contains("certificateFile")){
        certificateFile_ = settings_.value("certificateFile").toString();
        if(certificateFile_.isEmpty() || certificateFile_.isNull() || (certificateFile_ =="")){
            setEnableCertificateFile(false);
        }
    }
    else{
        settings_.setValue("certificateFile", certificateFile_);
    }
    return certificateFile_;
}

void AppSetting::setCertificateFile(const QString &certificateFile)
{
    if(certificateFile_ != certificateFile){
        certificateFile_ = certificateFile;
        settings_.setValue("certificateFile", certificateFile_);
        emit certificateFileChanged();
    }
}

bool AppSetting::enableCertificateFile()
{
    if(settings_.contains("enableCertificateFile")){
        enableCertificateFile_ = settings_.value("enableCertificateFile").toBool();
    }
    else{
        settings_.setValue("enableCertificateFile", enableCertificateFile_);
    }
    return enableCertificateFile_;
}

void AppSetting::setEnableCertificateFile(bool enableCertificateFile)
{
    if(enableCertificateFile_ != enableCertificateFile){
        enableCertificateFile_ = enableCertificateFile;
        settings_.setValue("enableCertificateFile", enableCertificateFile_);
        emit enableCertificateFileChanged();
    }
}

QString AppSetting::storagePath() const
{
    return storagePath_;
}

void AppSetting::setStoragePath(const QString &storagePath)
{
    if(storagePath_ != storagePath){
        storagePath_ = storagePath;
        emit storagePathChanged();
    }
}

QString AppSetting::executePath() const
{
    return executePath_;
}

void AppSetting::setExecutePath(const QString &executePath)
{
    if(executePath_ != executePath){
        executePath_ = executePath;
        emit executePathChanged();
    }
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

int AppSetting::changePassphraseResult() const
{
    return changePassphraseResult_;
}

void AppSetting::setChangePassphraseResult(int changePassphraseResult)
{
    if(changePassphraseResult_ != changePassphraseResult){
        changePassphraseResult_ = changePassphraseResult;
        emit changePassphraseResultChanged();
    }
}

bool AppSetting::enableCoreRPC()
{
    if(settings_.contains("enableCoreRPC")){
        enableCoreRPC_ = settings_.value("enableCoreRPC").toBool();
    }
    else{
        settings_.setValue("enableCoreRPC", enableCoreRPC_);
    }
    return enableCoreRPC_;
}

void AppSetting::setEnableCoreRPC(bool enableCoreRPC)
{
    if(enableCoreRPC_ != enableCoreRPC){
        enableCoreRPC_ = enableCoreRPC;
        settings_.setValue("enableCoreRPC", enableCoreRPC_);
        setFirstTimeCoreRPC(false);
        emit enableCoreRPCChanged();
    }
}

QString AppSetting::coreRPCAddress()
{
    if(settings_.contains("coreRPCAddress")){
        coreRPCAddress_ = settings_.value("coreRPCAddress").toString();
    }
    else{
        settings_.setValue("coreRPCAddress", coreRPCAddress_);
    }
    return coreRPCAddress_;
}

void AppSetting::setCoreRPCAddress(const QString &coreRPCAddress)
{
    if(coreRPCAddress_ != coreRPCAddress){
        coreRPCAddress_ = coreRPCAddress;
        settings_.setValue("coreRPCAddress", coreRPCAddress_);
        emit coreRPCAddressChanged();
    }
}

int AppSetting::coreRPCPort()
{
    if(settings_.contains("coreRPCPort")){
        coreRPCPort_ = settings_.value("coreRPCPort").toInt();
    }
    else{
        settings_.setValue("coreRPCPort", coreRPCPort_);
    }
    return coreRPCPort_;
}

void AppSetting::setCoreRPCPort(const int coreRPCPort)
{
    if(coreRPCPort_ != coreRPCPort){
        coreRPCPort_ = coreRPCPort;
        settings_.setValue("coreRPCPort", coreRPCPort_);
        emit coreRPCPortChanged();
    }
}

QString AppSetting::coreRPCName()
{
    if(settings_.contains("coreRPCName")){
        coreRPCName_ = settings_.value("coreRPCName").toString();
    }
    else{
        settings_.setValue("coreRPCName", coreRPCName_);
    }
    return coreRPCName_;
}

void AppSetting::setCoreRPCName(const QString &coreRPCName)
{
    if(coreRPCName_ != coreRPCName){
        coreRPCName_ = coreRPCName;
        settings_.setValue("coreRPCName", coreRPCName_);
        emit coreRPCNameChanged();
    }
}

QString AppSetting::coreRPCPassword()
{
    if(settings_.contains("coreRPCPassword")){
        coreRPCPassword_ = settings_.value("coreRPCPassword").toString();
    }
    else{
        settings_.setValue("coreRPCPassword", coreRPCPassword_);
    }
    return coreRPCPassword_;
}

void AppSetting::setCoreRPCPassword(const QString &coreRPCPassword)
{
    if(coreRPCPassword_ != coreRPCPassword){
        coreRPCPassword_ = coreRPCPassword;
        settings_.setValue("coreRPCPassword", coreRPCPassword_);
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

bool AppSetting::firstTimePassPhrase()
{
    if(settings_.contains("firstTimePassPhrase")){
        firstTimePassPhrase_ = settings_.value("firstTimePassPhrase").toBool();
    }
    else{
        settings_.setValue("firstTimePassPhrase", firstTimePassPhrase_);
    }
    return firstTimePassPhrase_;
}

void AppSetting::setFirstTimePassPhrase(bool firstTimePassPhrase)
{
    if(firstTimePassPhrase_ != firstTimePassPhrase){
        firstTimePassPhrase_ = firstTimePassPhrase;
        settings_.setValue("firstTimePassPhrase", firstTimePassPhrase_);
        emit firstTimePassPhraseChanged();
    }
}

bool AppSetting::firstTimeCoreRPC()
{
    if(settings_.contains("firstTimeCoreRPC")){
        firstTimeCoreRPC_ = settings_.value("firstTimeCoreRPC").toBool();
        if(firstTimeCoreRPC_ && enableCoreRPC_){
            firstTimeCoreRPC_ = false;
        }
    }
    else{
        firstTimeCoreRPC_ = true;
        settings_.setValue("firstTimeCoreRPC", firstTimeCoreRPC_);
    }
    DBG_INFO <<  firstTimeCoreRPC_;
    return firstTimeCoreRPC_;
}

void AppSetting::setFirstTimeCoreRPC(bool firstTimeCoreRPC)
{
    if(firstTimeCoreRPC_ != firstTimeCoreRPC){
        firstTimeCoreRPC_ = firstTimeCoreRPC;
        settings_.setValue("firstTimeCoreRPC", firstTimeCoreRPC_);
        emit firstTimeCoreRPCChanged();
    }
}
