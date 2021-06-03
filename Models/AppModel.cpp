#include "AppModel.h"
#include "AppSetting.h"
#include <QTimer>

AppModel::AppModel(): walletList_(QSharedPointer<WalletListModel>(new WalletListModel())),
    deviceList_(QSharedPointer<DeviceListModel>(new DeviceListModel())),
    masterSignerList_(QSharedPointer<MasterSignerListModel>(new MasterSignerListModel())),
    remoteSignerList_(QSharedPointer<SingleSignerListModel>(new SingleSignerListModel())),
    masterSignerInfo_(QSharedPointer<MasterSigner>(new MasterSigner())),
    singleSignerInfo_(QSharedPointer<SingleSigner>(new SingleSigner())),
    walletInfo_(QSharedPointer<Wallet>(new Wallet())),
    transactionInfo_(QSharedPointer<Transaction>(new Transaction())),
    transactionReplaceInfo_(NULL),
    transactionHistory_(QSharedPointer<TransactionListModel>(new TransactionListModel())),
    transactionHistoryShort_(QSharedPointer<TransactionListModel>(new TransactionListModel())),
    utxoList_(QSharedPointer<UTXOListModel>(new UTXOListModel())),
    utxoInfo_(QSharedPointer<UTXO>(new UTXO())),
    walletListCurrentIndex_(-1),
    chainTip_(0), addSignerStep_(0), cacheXpubsPercentage_(0), addSignerPercentage_(0), txidReplacing_(""),
    estimateFeePriority_(0), estimateFeeStandard_(0), estimateFeeEconomical_(0),addressBalance_(0),mnemonic_(""),
    softwareSignerDeviceList_(QSharedPointer<DeviceListModel>(new DeviceListModel()))
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
    connect(&timer_, SIGNAL(timeout()), this, SLOT(timer1sChanged()));
    timer_.start(10000);
    qrExported_.clear();
    suggestMnemonics_.clear();
}

AppModel::~AppModel(){
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
    transactionHistory_.clear();
    transactionHistoryShort_.clear();
    utxoList_.clear();
    utxoInfo_.clear();
    destinationList_.clear();
    this->disconnect();
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

QSharedPointer<DeviceListModel> AppModel::softwareSignerDeviceListPtr() const
{
    return softwareSignerDeviceList_;
}

void AppModel::setSoftwareSignerDeviceList(const QSharedPointer<DeviceListModel> &value)
{
    softwareSignerDeviceList_ = value;
    emit softwareSignerDeviceListChanged();
}

QString AppModel::getEstimateFeeEconomical() const
{
    return QString::number((double)estimateFeeEconomical_/1000);
}

void AppModel::setEstimateFeeEconomical(int estimateFeeEconomical)
{
    if(estimateFeeEconomical_ != estimateFeeEconomical){
        estimateFeeEconomical_ = estimateFeeEconomical;
        emit estimateFeeEconomicalChanged();
    }
}

void AppModel::checkDeviceUsableToSign()
{
    if( this->transactionInfo() && this->transactionInfo()->singleSignersAssigned()){
        if(this->deviceList()){
            this->deviceList()->resetUsableToSign();
            for (int i = 0; i < this->deviceList()->rowCount(); i++) {
                QSharedPointer<Device> dv = this->deviceList()->getDeviceByIndex(i);
                if(dv){
                    QString xfp = dv.data()->masterFingerPrint();
                    bool usable = this->transactionInfo()->singleSignersAssigned()->checkUsableToSign(xfp);
                    this->deviceList()->updateUsableToSign(xfp, usable);
                }
            }
        }
        if(this->softwareSignerDeviceList()){
            this->softwareSignerDeviceList()->resetUsableToSign();
            for (int i = 0; i < this->softwareSignerDeviceList()->rowCount(); i++) {
                QSharedPointer<Device> dv = this->softwareSignerDeviceList()->getDeviceByIndex(i);
                if(dv){
                    QString xfp = dv.data()->masterFingerPrint();
                    bool usable = this->transactionInfo()->singleSignersAssigned()->checkUsableToSign(xfp);
                    this->softwareSignerDeviceList()->updateUsableToSign(xfp, usable);
                }
            }
        }
    }
}


void AppModel::checkDeviceUsableToAdd()
{
    if(this->deviceList() && this->masterSignerList()){
        this->deviceList()->resetUsableToAdd();
        for (int i = 0; i < this->deviceList()->rowCount(); i++) {
            QSharedPointer<Device> dv = this->deviceList()->getDeviceByIndex(i);
            if(dv){
                bool usable = this->masterSignerList()->containsFingerPrint(dv.data()->masterFingerPrint());
                this->deviceList()->updateUsableToAdd(dv.data()->masterFingerPrint(), !usable);
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

QString AppModel::getEstimateFeeStandard() const
{
    return QString::number((double)estimateFeeStandard_/1000);
}

void AppModel::setEstimateFeeStandard(int estimateFeeStandard)
{
    if(estimateFeeStandard_ != estimateFeeStandard){
        estimateFeeStandard_ = estimateFeeStandard;
        emit estimateFeeStandardChanged();
    }
}

QString AppModel::getEstimateFeePriority() const
{
    return QString::number((double)estimateFeePriority_/1000);
}

void AppModel::setEstimateFeePriority(int estimateFeePriority)
{
    if(estimateFeePriority_ != estimateFeePriority){
        estimateFeePriority_ = estimateFeePriority;
        emit estimateFeePriorityChanged();
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

int AppModel::getCacheXpubsPercentage() const
{
    return cacheXpubsPercentage_;
}

void AppModel::setCacheXpubsPercentage(int cacheXpubsPercentage)
{
    if(cacheXpubsPercentage_ != cacheXpubsPercentage){
        cacheXpubsPercentage_ = cacheXpubsPercentage;
        emit cacheXpubsPercentageChanged();
    }
}

int AppModel::getAddSignerStep() const
{
    return addSignerStep_;
}

void AppModel::setAddSignerStep(int addSignerStep)
{
    DBG_INFO << addSignerStep;
    if(addSignerStep_ != addSignerStep){
        addSignerStep_ = addSignerStep;
        emit addSignerStepChanged();
    }
}

QSharedPointer<Transaction> AppModel::getTransactionReplaceInfo() const
{
    return transactionReplaceInfo_;
}

void AppModel::setTransactionReplaceInfo(const QSharedPointer<Transaction> &transactionReplaceInfo)
{
    transactionReplaceInfo_ = transactionReplaceInfo;
}

UTXO *AppModel::utxoInfo() const
{
    return utxoInfo_.data();
}

QSharedPointer<UTXO> AppModel::utxoInfoPtr() const
{
    return utxoInfo_;
}

void AppModel::setUtxoInfo(const QSharedPointer<UTXO> &utxoInfo)
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
    if(walletListCurrentIndex == -1){
        walletListCurrentIndex_ = 0;
    }
    else{
        walletListCurrentIndex_ = walletListCurrentIndex;
    }
    setWalletInfo(walletListCurrentIndex_);

    if(this->walletInfo()->singleSignersAssigned()){
        this->softwareSignerDeviceList()->resetDeviceConnected();
        for (int i = 0; i < this->walletInfo()->singleSignersAssigned()->rowCount(); i++) {
            QString xfp = this->walletInfo()->singleSignersAssigned()->getMasterSignerXfpByIndex(i);
            this->softwareSignerDeviceList()->updateDeviceConnected(xfp);
        }

        for (int j = 0; j < this->walletInfo()->singleSignersAssigned()->rowCount(); j++) {
            QString xfp = this->walletInfo()->singleSignersAssigned()->getMasterSignerXfpByIndex(j);
            //Check contains hardware signer
            if(masterSignerList()){
                if(masterSignerList()->hardwareContainsFingerPrint(xfp)){
                    walletInfo()->setContainsHWSigner(true);
                    break;
                }
            }
        }
    }
    emit walletListCurrentIndexChanged();
}

AppModel *AppModel::instance() {
    static AppModel mInstance;
    return &mInstance;
}

QSharedPointer<WalletListModel> AppModel::walletListPtr() const
{
    return walletList_;
}

WalletListModel* AppModel::walletList() const {
    return walletList_.data();
}

DeviceListModel* AppModel::deviceList() const {
    return deviceList_.data();
}

QSharedPointer<DeviceListModel> AppModel::deviceListPtr() const
{
    return deviceList_;
}

void AppModel::setWalletList(const QSharedPointer<WalletListModel> &d){
    walletList_ = d;
    if(walletList_){
        walletList_.data()->requestSort(WalletListModel::WalletRoles::wallet_Name_Role, Qt::AscendingOrder);
    }
    emit walletListChanged();
}

void AppModel::setDeviceList(const QSharedPointer<DeviceListModel> &d){
    if(deviceList_.data()){
        deviceList_.data()->updateDeviceList(d);
        emit deviceListChanged();
    }
}

MasterSignerListModel *AppModel::masterSignerList() const {
    return masterSignerList_.data();
}

QSharedPointer<MasterSignerListModel> AppModel::masterSignerListPtr() const
{
    return masterSignerList_;
}

void AppModel::setMasterSignerList(const QSharedPointer<MasterSignerListModel> &d)
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

QSharedPointer<SingleSignerListModel> AppModel::remoteSignerListPtr() const
{
    return remoteSignerList_;
}

void AppModel::setRemoteSignerList(const QSharedPointer<SingleSignerListModel> &d)
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

QSharedPointer<MasterSigner> AppModel::masterSignerInfoPtr() const
{
    return masterSignerInfo_;
}

void AppModel::setMasterSignerInfo(const QSharedPointer<MasterSigner> &d)
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
        masterSignerList_.data()->updateMasterSignerName(masterSignerInfo_.data()->id(), name);
    }
    if(walletList_ && masterSignerInfo_) {
        walletList_.data()->notifyMasterSignerRenamed(masterSignerInfo_->id(), name);
    }
    if(softwareSignerDeviceList_ && masterSignerInfo_) {
        softwareSignerDeviceList_.data()->notifySoftwareSignerRenamed(masterSignerInfo_->id(), name);
    }
}

void AppModel::updateSingleSignerInfoName(const QString &name)
{
    if(remoteSignerList_ && singleSignerInfo_){
        remoteSignerList_.data()->notifyRemoteSignerRenamed(singleSignerInfo_.data()->masterFingerPrint(), name);
    }
    if(walletList_ && singleSignerInfo_) {
        walletList_.data()->notifyMasterSignerRenamed(singleSignerInfo_->masterFingerPrint(), name);
    }
}


SingleSigner *AppModel::singleSignerInfo()
{
    return singleSignerInfo_.data();
}

QSharedPointer<SingleSigner> AppModel::singleSignerInfoPtr() const
{
    return singleSignerInfo_;
}

void AppModel::setSingleSignerInfo(const QSharedPointer<SingleSigner> &d)
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

QSharedPointer<Wallet> AppModel::newWalletInfoPtr() const
{
    return newWalletInfo_;
}

void AppModel::setNewWalletInfo(const QSharedPointer<Wallet> &d)
{
    newWalletInfo_ = d;
    emit newWalletInfoChanged();
}

Wallet *AppModel::walletInfo() const
{
    return walletInfo_.data();
}

QSharedPointer<Wallet> AppModel::walletInfoPtr() const
{
    return walletInfo_;
}

void AppModel::setWalletInfo(const QSharedPointer<Wallet> &d)
{
    walletInfo_ = d;
    emit walletInfoChanged();
}

void AppModel::setWalletInfo(const int index)
{
    if(walletList_.data()){
        walletInfo_ = walletList_.data()->getWalletByIndex(index);
    }

    if(!walletInfo_){
        walletInfo_ = QSharedPointer<Wallet>(new Wallet());
    }

    DBG_INFO << walletInfo_.data()->name();

    emit walletInfoChanged();
}

Transaction *AppModel::transactionInfo() const
{
    return transactionInfo_.data();
}

QSharedPointer<Transaction> AppModel::transactionInfoPtr() const
{
    return transactionInfo_;
}

void AppModel::setTransactionInfo(const QSharedPointer<Transaction>& d)
{
    if(d){
        transactionInfo_ = d;
    }
    else {
        transactionInfo_ = QSharedPointer<Transaction>(new Transaction());
    }
    emit transactionInfoChanged();
}

TransactionListModel *AppModel::transactionHistory() const{
    if(transactionHistory_.data()){
        return transactionHistory_.data();
    }
    return NULL;
}

TransactionListModel *AppModel::transactionHistoryShort() const
{
    if(transactionHistoryShort_.data()){
        return transactionHistoryShort_.data();
    }
    return NULL;
}

QSharedPointer<Transaction> AppModel::getTransactionByIndex(const int index) const
{
    if(transactionHistory_.data()){
         return transactionHistory_.data()->getTransactionByIndex(index);
    }
    return NULL;
}

QSharedPointer<Transaction> AppModel::getTransactionByTxid(const QString &txid) const
{
    if(transactionHistory_.data()){
         return transactionHistory_.data()->getTransactionByTxid(txid);
    }
    return NULL;
}

QSharedPointer<TransactionListModel> AppModel::transactionHistoryPtr() const {
    return transactionHistory_;
}

void AppModel::setTransactionHistory(const QSharedPointer<TransactionListModel> &d) {
    transactionHistory_ = d;
    if(transactionHistory_.data()){
        transactionHistory_.data()->requestSort(TransactionListModel::TransactionRoles::transaction_blocktime_role, Qt::DescendingOrder);
    }
    setTransactionHistoryShort(d);
    emit transactionHistoryChanged();
}

void AppModel::setTransactionHistoryShort(const QSharedPointer<TransactionListModel> &d) {
    transactionHistoryShort_ = QSharedPointer<TransactionListModel>(d.data()->getTransactionShortList());
    if(transactionHistoryShort_.data()){
        transactionHistoryShort_.data()->requestSort(TransactionListModel::TransactionRoles::transaction_blocktime_role, Qt::DescendingOrder);
    }
    emit transactionHistoryShortChanged();
}


UTXOListModel *AppModel::utxoList() const
{
    return utxoList_.data();
}

QSharedPointer<UTXOListModel> AppModel::utxoListPtr() const
{
    return utxoList_;
}

void AppModel::setUtxoList(const QSharedPointer<UTXOListModel> &utxoList)
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

QSharedPointer<DestinationListModel> AppModel::destinationListPtr() const
{
    return destinationList_;
}

void AppModel::setDestinationList(const QSharedPointer<DestinationListModel> &destinationList)
{
    destinationList_ = destinationList;
}

QString AppModel::addressBalance() const
{
    if(1 == AppSetting::instance()->unit()){
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

void AppModel::timer1sChanged()
{
    if(walletList()){
        walletList()->updateHealthCheckTime();
    }
    if(remoteSignerList()){
        remoteSignerList()->updateHealthCheckTime();
    }
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
