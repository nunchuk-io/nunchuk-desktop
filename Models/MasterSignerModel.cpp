#include "MasterSignerModel.h"
#include "qUtils.h"
#include <QQmlEngine>

MasterSigner::MasterSigner():
    id_(""),
    name_("UNKNOW"),
    device_(new Device()),
    health_(-1),
    messageToSign_(qUtils::QGenerateRandomMessage()),
    signature_(""),
    warningMessage_(QSharedPointer<QWarningMessage>(new QWarningMessage())),
    path_(""),
    checked_(false),
    isSoftwareSigner_(false)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

MasterSigner::MasterSigner(const QString &id, const QString& name, const QSharedPointer<Device> device, const int health, const bool software) :
    id_(id),
    name_(name),
    device_(device),
    health_(health),
    messageToSign_(qUtils::QGenerateRandomMessage()),
    signature_(""),
    warningMessage_(QSharedPointer<QWarningMessage>(new QWarningMessage())),
    path_(""),
    checked_(false),
    isSoftwareSigner_(software)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

MasterSigner::~MasterSigner(){
    warningMessage_.clear();
    device_.clear();
}

QString MasterSigner::id() const{
    return id_;
}

QString MasterSigner::name() const { return name_;}

Device *MasterSigner::device() const {
    return device_.data();
}

QSharedPointer<Device> MasterSigner::devicePtr() const
{
    return device_;
}

int MasterSigner::health() const {return health_;}

bool MasterSigner::connected() const
{
    if(device_.data()){
        return device_.data()->connected();
    }
    return false;
}

void MasterSigner::setId(const QString &d){
    if(d != id_){
        id_ = d;
        emit idChanged();
    }
}

void MasterSigner::setName(const QString &d) {
    if(d != name_){
        name_ = d;
        emit nameChanged();
    }
}

void MasterSigner::setDevice(const QSharedPointer<Device> &d) {
    device_ = d;
    emit deviceChanged();
    emit fingerPrintChanged();
}

void MasterSigner::setHealth(const int d)
{
    if(d != health_){
        health_ = d;
        emit healthChanged();
    }
}

bool MasterSigner::checked() const
{
    return checked_;
}

void MasterSigner::setChecked(const bool checked)
{
    if(checked != checked_){
        checked_ = checked;
        emit checkedChanged();
    }
}

bool MasterSigner::isSoftwareSigner() const
{
    return isSoftwareSigner_;
}

void MasterSigner::setIsSoftwareSigner(bool isSoftwareSigner)
{
    if(isSoftwareSigner_ != isSoftwareSigner){
        isSoftwareSigner_ = isSoftwareSigner;
        emit isSoftwareSignerChanged();
    }
}

bool MasterSigner::needPinSent() const
{
    return device_ ? device_.data()->needsPinSent() : false;
}

void MasterSigner::setNeedPinSent(bool value)
{
    if(device_){
        device_.data()->setNeedsPinSent(value);
        emit needPinSentChanged();
    }
}

bool MasterSigner::needPassphraseSent() const
{
    return device_ ? device_.data()->needsPassPhraseSent() : false;
}

void MasterSigner::setNeedPassphraseSent(bool value)
{
    if(device_){
        device_.data()->setNeedsPassPhraseSent(value);
        emit needPassphraseSentChanged();
    }
}

QString MasterSigner::deviceType() const
{
    return device_ ? device_.data()->type() : "";
}

QString MasterSigner::fingerPrint() const
{
    if(device_){
        return device_.data()->masterFingerPrint();
    }
    else{
        return "";
    }
}

QString MasterSigner::message() const
{
    return messageToSign_;
}

void MasterSigner::setMessage(const QString &messageToSign)
{
    if(messageToSign_ != messageToSign){
        messageToSign_ = messageToSign;
        emit messageToSignChanged();
    }
}

QString MasterSigner::signature() const
{
    return signature_;
}

void MasterSigner::setSignature(const QString &signature)
{
    if(signature_ != signature){
        signature_ = signature;
        emit signatureChanged();
    }
}

QSharedPointer<QWarningMessage> MasterSigner::warningMessagePtr() const
{
    return warningMessage_;
}

QWarningMessage* MasterSigner::warningMessage() const
{
    return warningMessage_.data();
}

void MasterSigner::setWarningMessage(const QSharedPointer<QWarningMessage> &warningMessage)
{
    warningMessage_ = warningMessage;
    emit warningMessageChanged();
}

QString MasterSigner::path() const
{
    return path_;
}

void MasterSigner::setPath(const QString &path)
{
    if(path_ != path){
        path_ = path;
        emit pathChanged();
    }
}

MasterSignerListModel::MasterSignerListModel() {
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

MasterSignerListModel::~MasterSignerListModel() {d_.clear();}

int MasterSignerListModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return d_.count();
}

QVariant MasterSignerListModel::data(const QModelIndex &index, int role) const {
    switch (role) {
    case master_signer_id_Role:
        return d_[index.row()]->id();
    case master_signer_name_Role:
        return d_[index.row()]->name();
    case master_signer_device_Role:
        return qVariantFromValue((Device*) d_[index.row()]->device());
    case master_signer_checked_Role:
        return d_[index.row()]->checked();
    case master_signer_fingerPrint_Role:
        return d_[index.row()]->fingerPrint();
    case master_signer_msg2Sign_Role:
        return d_[index.row()]->message();
    case master_signer_deviceType_Role:
        return d_[index.row()]->deviceType();
    case master_signer_isSoftware_Role:
        return d_[index.row()]->isSoftwareSigner();
    case master_signer_need_passphrase_Role:
        return d_[index.row()]->needPassphraseSent();
    case master_signer_need_pin_Role:
        return d_[index.row()]->needPinSent();
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> MasterSignerListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[master_signer_id_Role]        = "master_signer_id";
    roles[master_signer_name_Role]      = "master_signer_name";
    roles[master_signer_device_Role]    = "master_signer_device";
    roles[master_signer_checked_Role]   = "master_signer_checked";
    roles[master_signer_fingerPrint_Role]   = "master_signer_fingerPrint";
    roles[master_signer_msg2Sign_Role]      = "master_signer_messageToSign";
    roles[master_signer_deviceType_Role]    = "master_signer_deviceType";
    roles[master_signer_isSoftware_Role]    = "master_signer_isSoftware";
    roles[master_signer_need_passphrase_Role]    = "master_signer_need_passphrase";
    roles[master_signer_need_pin_Role]      = "master_signer_need_pin";
    return roles;
}

void MasterSignerListModel::addMasterSigner(const QString &id, const QString& name, const QSharedPointer<Device> device, const int health , const bool software){
    beginResetModel();
    if(!contains(id)){
        d_.append(QSharedPointer<MasterSigner>(new MasterSigner(id, name, device, health, software)));
    }
    endResetModel();
}

void MasterSignerListModel::addMasterSigner(const QSharedPointer<MasterSigner> &d)
{
    beginResetModel();
    if(!contains(d.data()->id())){
        d_.append(d);
    }
    endResetModel();
}

void MasterSignerListModel::updateMasterSignerList(const QSharedPointer<MasterSignerListModel> &d)
{
    beginResetModel();
    d_.clear();
    for (int var = 0; var < d.data()->rowCount(); var++) {
        d_.append(d.data()->getMasterSignerByIndex(var));
    }
    endResetModel();
}

void MasterSignerListModel::updateMasterSignerName(const QString &signerId, const QString &name){
    foreach (QSharedPointer<MasterSigner> it, d_) {
        if(it.data()->id() == signerId){
            beginResetModel();
            it.data()->setName(name);
            endResetModel();
        }
    }
}

void MasterSignerListModel::updateDeviceStatus(const QString &fingerprint, const bool connected)
{
    foreach (QSharedPointer<MasterSigner> it, d_) {
        if(it.data()->device()){
            if(it.data()->device()->masterFingerPrint() == fingerprint){
                it.data()->device()->setConnected(connected);
                emit dataChanged(this->index(d_.indexOf(it)),this->index(d_.indexOf(it)));
            }
        }
    }
}

QSharedPointer<MasterSigner> MasterSignerListModel::getMasterSignerByIndex(const int index) {
    if(index < 0 || index >= d_.count()){
        DBG_INFO << "Index out of range";
        return QSharedPointer<MasterSigner>(NULL);
    }
    else {
        return d_.at(index);
    }
}

QSharedPointer<MasterSigner> MasterSignerListModel::getMasterSignerById(const QString &id)
{
    foreach (QSharedPointer<MasterSigner> it, d_) {
        if(it.data()->id() == id){
            return it;
        }
    }
    return QSharedPointer<MasterSigner>(NULL);
}

QString MasterSignerListModel::getMasterSignerNameByFingerPrint(const QString &fingerprint)
{
    foreach (QSharedPointer<MasterSigner> it, d_) {
        if(it.data() && it.data()->device()){
            if(0 == QString::compare(fingerprint, it.data()->fingerPrint(), Qt::CaseInsensitive)){
                return it.data()->name();
            }
        }
    }
    return "";
}

bool MasterSignerListModel::removeMasterSigner(const QString &masterSignerId)
{
    beginResetModel();
    foreach (QSharedPointer<MasterSigner> it, d_) {
        if(it.data() && it.data()->id() == masterSignerId){
            d_.removeAll(it);
            return true;
        }
    }
    endResetModel();
    return false;
}

void MasterSignerListModel::resetUserChecked()
{
    beginResetModel();
    foreach (QSharedPointer<MasterSigner> it, d_) {
        it.data()->setChecked(false);
    }
    endResetModel();
}

void MasterSignerListModel::setUserChecked(const bool state, const int index)
{
    if(index < 0 || index >= d_.count()){
        DBG_INFO << "Index out of range";
    }
    else {
        beginResetModel();
        d_.at(index)->setChecked(state);
        endResetModel();
    }
}

void MasterSignerListModel::setUserCheckedById(const bool state, const QString &id)
{
    beginResetModel();
    foreach (QSharedPointer<MasterSigner> it, d_) {
        if(0 == QString::compare(id, it.data()->id(), Qt::CaseInsensitive)){
            DBG_INFO << it.data()->id() << id << state;
            it.data()->setChecked(state);
        }
    }
    endResetModel();
}

bool MasterSignerListModel::removeMasterSigner(const QSharedPointer<MasterSigner> it)
{
    beginResetModel();
    d_.removeAll(it);
    endResetModel();
    return true;
}

bool MasterSignerListModel::contains(const QString &masterSignerId)
{
    foreach (QSharedPointer<MasterSigner> i , d_ ){
        if(0 == QString::compare(masterSignerId, i.data()->id(), Qt::CaseInsensitive)){
            return true;
        }
    }
    return false;
}

bool MasterSignerListModel::containsFingerPrint(const QString &fingerprint)
{
    foreach (QSharedPointer<MasterSigner> i , d_ ){
        if(0 == QString::compare(fingerprint, i.data()->fingerPrint(), Qt::CaseInsensitive)){
            return true;
        }
    }
    return false;
}

bool MasterSignerListModel::hardwareContainsFingerPrint(const QString &fingerprint)
{
    foreach (QSharedPointer<MasterSigner> i , d_ ){
        if(i.data()->isSoftwareSigner()){
            continue;
        }
        else{
            DBG_INFO << fingerprint << i.data()->fingerPrint();
            if(0 == QString::compare(fingerprint, i.data()->fingerPrint(), Qt::CaseInsensitive)){
                return true;
            }
        }
    }
    return false;
}

void MasterSignerListModel::updateDeviceNeedPinSent(const QString &fingerprint, const bool needpin)
{
    beginResetModel();
    foreach (QSharedPointer<MasterSigner> i , d_ ){
        if(0 == QString::compare(fingerprint, i.data()->fingerPrint(), Qt::CaseInsensitive)){
            i.data()->device()->setNeedsPinSent(needpin);
        }
    }
}

void MasterSignerListModel::updateDeviceNeedPassphraseSent(const QString &fingerprint, const bool needpassphrase)
{
    DBG_INFO << fingerprint << needpassphrase;
    beginResetModel();
    foreach (QSharedPointer<MasterSigner> i , d_ ){
        if(0 == QString::compare(fingerprint, i.data()->fingerPrint(), Qt::CaseInsensitive)){
            i.data()->setNeedPassphraseSent(needpassphrase);
        }
    }
    endResetModel();
}

void MasterSignerListModel::updateDeviceNeedPassphraseSentById(const QString &id, const bool needpassphrase)
{
    beginResetModel();
    foreach (QSharedPointer<MasterSigner> i , d_ ){
        if(0 == QString::compare(id, i.data()->id(), Qt::CaseInsensitive)){
            i.data()->setNeedPassphraseSent(needpassphrase);
        }
    }
    endResetModel();
}

QStringList MasterSignerListModel::getColdCardId() const
{
    QStringList ret;
    ret.clear();
    foreach (QSharedPointer<MasterSigner> i , d_ ){
        if(i.data()->device() && i.data()->device()->type() == "coldcard"){
            ret << i.data()->id();
        }
    }
    return ret;
}

void MasterSignerListModel::requestSort(int role, int order)
{
    beginResetModel();
    if(d_.count() > 1){
        switch (role) {
        case master_signer_name_Role:
        {
            if(Qt::DescendingOrder == order){
                qSort(d_.begin(), d_.end(), sortMasterSignerByNameDescending);
            }
            else{
                qSort(d_.begin(), d_.end(), sortMasterSignerByNameAscending);
            }
        }
            break;
        default:
            break;
        }
    }
    endResetModel();
}

bool sortMasterSignerByNameAscending(const QSharedPointer<MasterSigner> &v1, const QSharedPointer<MasterSigner> &v2)
{
    return (QString::compare((v1.data()->name()), (v2.data()->name())) < 0);
}

bool sortMasterSignerByNameDescending(const QSharedPointer<MasterSigner> &v1, const QSharedPointer<MasterSigner> &v2)
{
    return (QString::compare((v1.data()->name()), (v2.data()->name())) > 0);
}
