#include "TransactionModel.h"
#include "AppSetting.h"
#include "qUtils.h"
#include "QOutlog.h"
#include "bridgeifaces.h"
#include <QQmlEngine>

Destination::Destination(): address_(""), amount_(0){
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

Destination::Destination(const QString &address, const qint64 amount) : address_(address), amount_(amount){
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

Destination::~Destination() {}

QString Destination::amountDisplay() const
{
    if(1 == AppSetting::instance()->unit()){
        QLocale locale(QLocale::English);
        return locale.toString(amountSats());
    }
    else{
        return amountBTC();
    }
}

QString Destination::amountBTC() const
{
    return qUtils::QValueFromAmount(amount_);
}

qint64 Destination::amountSats() const
{
    return amount_;
}

QString Destination::address() const {
    return address_;
}

void Destination::setAddress(const QString &value){
    if(value != address_){
        address_ = value;
        emit addressChanged();
    }
}

void Destination::setAmount(const qint64 value){
    if(value != amount_){
        amount_ = value;
        emit amountChanged();
    }
}

DestinationListModel::DestinationListModel(){
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

DestinationListModel::~DestinationListModel(){d_.clear();}

int DestinationListModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return d_.count();
}

QVariant DestinationListModel::data(const QModelIndex &index, int role) const {
    switch (role) {
    case destination_address_role:
        return d_[index.row()]->address();
    case destination_amount_role:
        return d_[index.row()]->amountDisplay();
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> DestinationListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[destination_address_role] = "destination_address";
    roles[destination_amount_role] = "destination_amount";
    return roles;
}

void DestinationListModel::addDestination(const QString &address, const qint64 amount) {
    qint64 am = 0;
    if(!contains(address, am)){
        beginResetModel();
        d_.append(QSharedPointer<Destination>(new Destination(address, amount)));
        endResetModel();
    }
}

QMap<QString, qint64> DestinationListModel::getOutputs() const
{
    QMap<QString, qint64> outputs;
    foreach (QSharedPointer<Destination> i , d_ ){
        outputs[i->address()] = i->amountSats();
    }
    return outputs;
}

bool DestinationListModel::contains(const QString &address, qint64 &amount)
{
    foreach (QSharedPointer<Destination> i , d_ ){
        if(address == i.data()->address()){
            amount = i.data()->amountSats();
            return true;
        }
    }
    return false;
}

qint64 DestinationListModel::getAmountByAddress(const QString &address)
{
    foreach (QSharedPointer<Destination> i , d_ ){
        if(address == i.data()->address()){
            return i.data()->amountSats();
        }
    }
    return 0;
}

void DestinationListModel::notifyUnitChanged()
{
    beginResetModel();
    foreach (QSharedPointer<Destination> it, d_) {
        if(it.data()){
            it.data()->amountChanged();
        }
    }
    endResetModel();
}


Transaction::Transaction() :txid_(""),
                            memo_(""),
                            status_(-1),
                            fee_(0),
                            m_(0),
                            hasChange_(false),
                            destinationList_(QSharedPointer<DestinationListModel>(new DestinationListModel())),
                            change_(QSharedPointer<Destination>(new Destination())),
                            singleSignersAssigned_(QSharedPointer<SingleSignerListModel>(new (SingleSignerListModel))),
                            subtotal_(0),
                            total_(0),
                            numberSigned_(0),
                            blocktime_(-2),
                            height_(0),
                            isReceiveTx_(false),
                            subtractFromFeeAmount_(false),
                            feeRate_(0),
                            warningMessage_(QSharedPointer<QWarningMessage>(new QWarningMessage())),
                            replacedTxid_("")
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

Transaction::~Transaction() {
    destinationList_.clear();
    change_.clear() ;
    singleSignersAssigned_.clear() ;
    warningMessage_.clear() ;
}

QString Transaction::txid() const { return txid_; }

QString Transaction::memo() const {
    if(memo_.isEmpty() || memo_ == ""){
        return "--";
    }
    else{
        return memo_;
    }
}

int Transaction::status() const { return status_; }

QString Transaction::feeDisplay() const
{
    if(1 == AppSetting::instance()->unit()){
        QLocale locale(QLocale::English);
        return locale.toString(feeSats());
    }
    else{
        return feeBTC();
    }
}

QString Transaction::feeBTC() const
{
    return qUtils::QValueFromAmount(fee_);
}

qint64 Transaction::feeSats() const
{
    return fee_;
}

int Transaction::m() const { return m_; }

bool Transaction::hasChange() const { return hasChange_; }

DestinationListModel *Transaction::destinationList() const { return destinationList_.data(); }

QSharedPointer<DestinationListModel> Transaction::destinationListPtr() const { return destinationList_; }

Destination *Transaction::change() const { return change_.data(); }

QSharedPointer<Destination> Transaction::changePtr() const { return change_; }

SingleSignerListModel *Transaction::singleSignersAssigned() const { return singleSignersAssigned_.data(); }

QSharedPointer<SingleSignerListModel> Transaction::singleSignersAssignedPtr() const { return singleSignersAssigned_; }

void Transaction::setTxid(const QString &txid)
{
    if(txid_ != txid){
        txid_ = txid;
        emit txidChanged();
    }
}

void Transaction::setMemo(const QString &memo)
{
    if(memo_ != memo){
        memo_ = memo;
        emit memoChanged();
    }
}

void Transaction::setStatus(int status)
{
    if(status_ != status){
        status_ = status;
        emit statusChanged();
    }
}

void Transaction::setFee(const qint64 fee)
{
    if(fee_ != fee){
        fee_ = fee;
        emit feeChanged();
    }
}

void Transaction::setM(int m)
{
    if(m_ != m){
        m_ = m;
        emit mChanged();
    }
}

void Transaction::setHasChange(bool hasChange)
{
    if(hasChange_ != hasChange){
        hasChange_ = hasChange;
        emit hasChangeChanged();
    }
}

void Transaction::setDestinationList(const QSharedPointer<DestinationListModel> &destinationList)
{
    destinationList_ = destinationList;
    emit destinationListChanged();
}

void Transaction::setChange(const QSharedPointer<Destination> &change)
{
    change_ = change;
    emit changeChanged();
}

void Transaction::setSingleSignersAssigned(const QSharedPointer<SingleSignerListModel> &singleSignersAssigned)
{
    for (int var = 0; var < singleSignersAssigned->rowCount(); var++) {
        QSharedPointer<SingleSigner> temp = singleSignersAssigned.data()->getSingleSignerByIndex(var);
        singleSignersAssigned_.data()->addSingleSigner(temp.data()->name(),
                                                       temp.data()->xpub(),
                                                       temp.data()->publickey(),
                                                       temp.data()->derivationPath(),
                                                       temp.data()->masterFingerPrint(),
                                                       temp.data()->masterSignerId(),
                                                       temp->lastHealthCheckDateTime());
    }

    singleSignersAssigned_.data()->initSignatures();
    emit singleSignerAssignedChanged();
}

QString Transaction::subtotalDisplay() const
{
    if(1 == AppSetting::instance()->unit()){
        QLocale locale(QLocale::English);
        return locale.toString(subtotalSats());
    }
    else{
        return subtotalBTC();
    }
}

QString Transaction::subtotalBTC() const
{
    return qUtils::QValueFromAmount(subtotal_);
}

qint64 Transaction::subtotalSats() const
{
    return subtotal_;
}

void Transaction::setSubtotal(const qint64 subtotal)
{
    if(subtotal_ != subtotal){
        subtotal_ = subtotal;
        emit subtotalChanged();
    }
}

QString Transaction::totalDisplay() const
{
    if(1 == AppSetting::instance()->unit()){
        QLocale locale(QLocale::English);
        return locale.toString(totalSats());
    }
    else{
        return totalBTC();
    }
}

qint64 Transaction::totalSats() const
{
    return total_;
}

QString Transaction::totalBTC() const
{
    return qUtils::QValueFromAmount(total_);
}

void Transaction::setTotal(const qint64 total)
{
    if(total_ != total){
        total_ = total;
        emit totalChanged();
    }
}

void Transaction::setNumberSigned(int numberSigned)
{
    numberSigned_ = numberSigned;
    emit numberSignedChanged();
}

QString Transaction::blocktimeDisplay() const
{
    if(0 >= blocktime_){
        return "--/--/----"; // There is no time
    }
    else{
//        return QDateTime::fromTime_t(blocktime_).toOffsetFromUtc(QDateTime::currentDateTime().offsetFromUtc()).toString(Qt::ISODate);
        return QDateTime::fromTime_t(blocktime_).toString( "MM/dd/yyyy hh:mm AP");
    }
}

time_t Transaction::blocktime() const
{
    return blocktime_;
}

void Transaction::setBlocktime(const time_t blocktime)
{
    if(blocktime_ != blocktime){
        blocktime_ = blocktime;
        emit blocktimeChanged();
    }
}

int Transaction::height() const
{
    return height_;
}

void Transaction::setHeight(int height)
{
    if(height_ != height){
        height_ = height;
        emit heightChanged();
    }
}

bool Transaction::isReceiveTx() const
{
    return isReceiveTx_;
}

void Transaction::setIsReceiveTx(bool isReceiveTx)
{
    if(isReceiveTx_ != isReceiveTx){
        isReceiveTx_ = isReceiveTx;
        emit isReceiveTxChanged();
    }
}

bool Transaction::subtractFromFeeAmount() const
{
    return subtractFromFeeAmount_;
}

void Transaction::setSubtractFromFeeAmount(bool subtractFromFeeAmount)
{
    if(subtractFromFeeAmount_ != subtractFromFeeAmount){
        subtractFromFeeAmount_ = subtractFromFeeAmount;
        emit subtractFromFeeAmountChanged();
    }
}

QString Transaction::feeRate() const
{
    DBG_INFO << "CURRENT FEE RATE " << QString::number((double)feeRate_/1000);
    return QString::number((double)feeRate_/1000);
}

void Transaction::setFeeRate(qint64 feeRate)
{
    if(feeRate_ != feeRate){
        feeRate_ = feeRate;
        emit feeRateChanged();
    }
}

QSharedPointer<QWarningMessage> Transaction::warningMessagePtr() const
{
    return warningMessage_;
}

QWarningMessage* Transaction::warningMessage() const
{
    return warningMessage_.data();
}

void Transaction::setWarningMessage(const QSharedPointer<QWarningMessage> &warningMessage)
{
    warningMessage_ = warningMessage;
    emit warningMessageChanged();
}

QString Transaction::replacedTxid() const
{
    return replacedTxid_;
}

void Transaction::setReplacedTxid(const QString &replacedTxid)
{
    if(replacedTxid_ != replacedTxid){
        replacedTxid_ = replacedTxid;
        emit replacedTxidChanged();
    }
}

int Transaction::numberSigned() const
{
    return numberSigned_;
}

TransactionListModel::TransactionListModel() {
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

TransactionListModel::~TransactionListModel() {d_.clear();}

int TransactionListModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return d_.count();
}

QVariant TransactionListModel::data(const QModelIndex &index, int role) const {
    switch (role) {
    case transaction_txid_role:
        return d_[index.row()]->txid();
    case transaction_memo_role:
        return d_[index.row()]->memo();
    case transaction_status_role:
        return d_[index.row()]->status();
    case transaction_fee_role:
        return d_[index.row()]->feeDisplay();
    case transaction_m_role:
        return d_[index.row()]->m();
    case transaction_hasChange_role:
        return d_[index.row()]->hasChange();
    case transaction_destinationList_role:
        return qVariantFromValue(d_[index.row()]->destinationList());
    case transaction_change_role:
        return qVariantFromValue(d_[index.row()]->change());
    case transaction_singleSignersAssigned_role:
        return qVariantFromValue(d_[index.row()]->singleSignersAssigned());
    case transaction_subtotal_role:
        return d_[index.row()]->subtotalDisplay();
    case transaction_total_role:
        return d_[index.row()]->totalDisplay();
    case transaction_numberSigned_role:
        return d_[index.row()]->numberSigned();
    case transaction_blocktime_role:
        return d_[index.row()]->blocktimeDisplay();
    case transaction_height_role:
        return d_[index.row()]->height();
    case transaction_isReceiveTx_role:
        return d_[index.row()]->isReceiveTx();
    case transaction_replacedTx_role:
        return d_[index.row()]->replacedTxid();
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> TransactionListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[transaction_txid_role]            = "transaction_txid";
    roles[transaction_memo_role]            = "transaction_memo";
    roles[transaction_status_role]          = "transaction_status";
    roles[transaction_fee_role]             = "transaction_fee";
    roles[transaction_m_role]               = "transaction_m";
    roles[transaction_hasChange_role]       = "transaction_hasChange";
    roles[transaction_destinationList_role] = "transaction_destinationList";
    roles[transaction_change_role]          = "transaction_change_role";
    roles[transaction_singleSignersAssigned_role]  = "transaction_singleSignersAssigned";
    roles[transaction_subtotal_role]        = "transaction_subtotal";
    roles[transaction_total_role]           = "transaction_total";
    roles[transaction_numberSigned_role]    = "transaction_numberSigned";
    roles[transaction_blocktime_role]       = "transaction_blocktime";
    roles[transaction_height_role]          = "transaction_height";
    roles[transaction_isReceiveTx_role]     = "transaction_isReceiveTx";
    roles[transaction_replacedTx_role]      = "transaction_replacedTx";
    return roles;
}

QSharedPointer<Transaction> TransactionListModel::getTransactionByIndex(const int index)
{
    if(index < 0 || index >= d_.count()){
        DBG_INFO << "Index out of range";
        return NULL;
    }
    else {
        return d_.at(index);
    }
}

QSharedPointer<Transaction> TransactionListModel::getTransactionByTxid(const QString &txid)
{
    foreach (QSharedPointer<Transaction> i , d_ ){
        if(txid == i.data()->txid()){
            return i;
        }
    }
    return NULL;
}

TransactionListModel *TransactionListModel::getTransactionShortList(const int cnt)
{
    TransactionListModel* result = new TransactionListModel();
    int max = min(d_.count(), cnt);
    for (int var = 0; var < max; var++) {
        result->addTransaction(d_[var]);
    }
    return result;
}

void TransactionListModel::addTransaction(const QSharedPointer<Transaction> &d){
    if(d){
        if(!contains(d.data()->txid())){
            beginResetModel();
            d_.append(d);
            endResetModel();
        }
    }
}

void TransactionListModel::updateTransactionStatus(const QString &tx_id, const int status)
{
    foreach (QSharedPointer<Transaction> i , d_ ){
        if(tx_id == i.data()->txid()){
            beginResetModel();
            i.data()->setStatus(status);
            endResetModel();
        }
    }
    requestSort(TransactionListModel::TransactionRoles::transaction_blocktime_role, Qt::DescendingOrder);
}

void TransactionListModel::updateTransactionMemo(const QString &tx_id, const QString &memo)
{
    beginResetModel();
    foreach (QSharedPointer<Transaction> i , d_ ){
        if(tx_id == i.data()->txid()){
            i.data()->setMemo(memo);
        }
    }
    endResetModel();
}

void TransactionListModel::replaceTransaction(const QString &tx_id, const QSharedPointer<Transaction> &value)
{
    if(value){
        bool isExist = false;
        for (int i = 0; i < d_.count(); i++) {
            if(d_.at(i) && d_.at(i).data()->txid() == tx_id){
                beginResetModel();
                d_.replace(i, value);
                endResetModel();
                isExist = true;
                break;
            }
        }

        if(false == isExist){
            DBG_INFO << "New transaction" << value.data()->txid();
            if(!contains(value.data()->txid())){
                beginResetModel();
                d_.append(value);
                endResetModel();
            }
        }
    }
    requestSort(TransactionListModel::TransactionRoles::transaction_blocktime_role, Qt::DescendingOrder);
}

void TransactionListModel::removeTransaction(const QString &tx_id)
{
    foreach (QSharedPointer<Transaction> it, d_) {
        if(it.data() && it.data()->txid() == tx_id){
            beginResetModel();
            d_.removeAll(it);
            endResetModel();
            break;
        }
    }
}

bool TransactionListModel::contains(const QString &tx_id)
{
    foreach (QSharedPointer<Transaction> i , d_ ){
        if(tx_id == i.data()->txid()){
            return true;
        }
    }
    return false;
}

void TransactionListModel::requestSort(int role, int order)
{
    beginResetModel();
    if(d_.count() > 1){
        switch (role) {
        case transaction_txid_role:
            break;
        case transaction_memo_role:
        {
            if(Qt::DescendingOrder == order){
                qSort(d_.begin(), d_.end(), sortTXsByMemoDescending);
            }
            else{
                qSort(d_.begin(), d_.end(), sortTXsByMemoAscending);
            }
        }
            break;
        case transaction_status_role:
        {
            if(Qt::DescendingOrder == order){
                qSort(d_.begin(), d_.end(), sortTXsByStatusDescending);
            }
            else{
                qSort(d_.begin(), d_.end(), sortTXsByStatusAscending);
            }
        }
            break;
        case transaction_subtotal_role:
        case transaction_total_role:
        {
            if(Qt::DescendingOrder == order){
                qSort(d_.begin(), d_.end(), sortTXsByAmountDescending);
            }
            else{
                qSort(d_.begin(), d_.end(), sortTXsByAmountAscending);
            }
        }
            break;
        case transaction_blocktime_role:
        {
            qSort(d_.begin(), d_.end(), sortTXsByBlocktimeAscending);
            if(Qt::DescendingOrder == order){
                qSort(d_.begin(), d_.end(), sortTXsByBlocktimeDescendingSkipZero);
            }
            linkingReplacedTransactions();
        }
            break;
        default:
            break;
        }
    }
    endResetModel();
}

void TransactionListModel::notifyUnitChanged()
{
    beginResetModel();
    foreach (QSharedPointer<Transaction> it, d_) {
        if(it.data()){
            emit it.data()->feeChanged();
            emit it.data()->totalChanged();
            emit it.data()->subtotalChanged();
            if(it.data()->destinationList()){
                it.data()->destinationList()->notifyUnitChanged();
            }
            if(it.data()->change()){
                it.data()->change()->amountChanged();
            }
        }
    }
    endResetModel();
}

void TransactionListModel::linkingReplacedTransactions()
{
    QMap<QString, QString> replaces;
    for (int i = 0; i < d_.count(); i++) {
        if(d_.at(i) && (int)nunchuk::TransactionStatus::REPLACED == d_.at(i).data()->status()){
            replaces[d_.at(i).data()->txid()] = d_.at(i).data()->replacedTxid();
        }
    }

    int from_index = -1;
    int to_index = -1;
    for (int j = 0; j < replaces.keys().count(); j++) {
        from_index = -1;
        to_index = -1;
        for (int k = 0; k < d_.count(); k++) {
            if(d_.at(k)){
                if(d_.at(k).data()->txid() == replaces.keys()[j]){ // old tx replaced
                    from_index = k;
                }
                if(d_.at(k).data()->txid() == replaces[replaces.keys()[j]]){ // Find new tx
                    to_index = k;
                }
            }
        }

        if((-1 != from_index) && (-1 != to_index) && (to_index != from_index)){
            d_.move(from_index, to_index);
        }
    }
}

bool sortTXsByBlocktimeAscending(const QSharedPointer<Transaction> &v1, const QSharedPointer<Transaction> &v2)
{
    return v1.data()->blocktime() < v2.data()->blocktime();
}

bool sortTXsByBlocktimeDescending(const QSharedPointer<Transaction> &v1, const QSharedPointer<Transaction> &v2)
{
    return v1.data()->blocktime() > v2.data()->blocktime();
}

bool sortTXsByBlocktimeDescendingSkipZero(const QSharedPointer<Transaction> &v1, const QSharedPointer<Transaction> &v2)
{
    if(v1.data()->blocktime() <= 0 && v2.data()->blocktime() <= 0){ return v1.data()->blocktime() < v2.data()->blocktime();}
    else if(v1.data()->blocktime() <= 0 && v2.data()->blocktime() > 0){ return true;}
    else if(v1.data()->blocktime() > 0 && v2.data()->blocktime() <= 0){ return false;}
    else {return v1.data()->blocktime() > v2.data()->blocktime();}
}

bool sortTXsByAmountAscending(const QSharedPointer<Transaction> &v1, const QSharedPointer<Transaction> &v2)
{
    return ((v1.data()->subtotalBTC() + v1.data()->totalBTC()) < (v2.data()->subtotalBTC() + v2.data()->totalBTC()));
}

bool sortTXsByAmountDescending(const QSharedPointer<Transaction> &v1, const QSharedPointer<Transaction> &v2)
{
    return ((v1.data()->subtotalBTC() + v1.data()->totalBTC()) > (v2.data()->subtotalBTC() + v2.data()->totalBTC()));
}

bool sortTXsByMemoAscending(const QSharedPointer<Transaction> &v1, const QSharedPointer<Transaction> &v2)
{
    return (QString::compare((v1.data()->memo()), (v2.data()->memo())) < 0);
}

bool sortTXsByMemoDescending(const QSharedPointer<Transaction> &v1, const QSharedPointer<Transaction> &v2)
{
    return (QString::compare((v1.data()->memo()), (v2.data()->memo())) > 0);
}

bool sortTXsByStatusAscending(const QSharedPointer<Transaction> &v1, const QSharedPointer<Transaction> &v2)
{
    return ((v1.data()->status()) < (v2.data()->status()));
}

bool sortTXsByStatusDescending(const QSharedPointer<Transaction> &v1, const QSharedPointer<Transaction> &v2)
{
    return ((v1.data()->status()) > (v2.data()->status()));
}
