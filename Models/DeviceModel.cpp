#include "DeviceModel.h"
#include <QQmlEngine>

Device::Device() :
    name_("UNKNOWN"),
    type_("UNKNOWN"),
    path_("UNKNOWN"),
    model_("UNKNOWN"),
    master_fingerprint_("UNKNOWN"),
    connected_(false),
    needs_pass_phrase_sent_(false),
    needs_pin_sent_(false),
    usableToSign_(true),
    usableToAdd_(true),
    master_signer_id_("")
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

Device::Device(const QString &fingerprint) :
    name_(""),
    type_(""),
    path_(""),
    model_(""),
    master_fingerprint_(fingerprint),
    connected_(false),
    needs_pass_phrase_sent_(false),
    needs_pin_sent_(false),
    usableToSign_(true),
    usableToAdd_(true),
    master_signer_id_("")
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

Device::Device(const QString &name,
               const QString &type,
               const QString &path,
               const QString &model,
               const QString &master_fingerprint,
               bool connected,
               bool needs_pass_phrase_sent,
               bool needs_pin_sent,
               QString mastersigner_id) :
    name_(name),
    type_(type),
    path_(path),
    model_(model),
    master_fingerprint_(master_fingerprint),
    connected_(connected),
    needs_pass_phrase_sent_(needs_pass_phrase_sent),
    needs_pin_sent_(needs_pin_sent),
    usableToSign_(true),
    usableToAdd_(true),
    master_signer_id_(mastersigner_id)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

Device::~Device(){}

QString Device::name() const
{
    if(name_ == "" || name_.isEmpty() || name_.isNull()){
        return type_;
    }
    return name_;
}

QString Device::type() const { return type_;}

QString Device::path() const { return path_;}

QString Device::model() const { return model_;}

QString Device::masterFingerPrint() const { return master_fingerprint_;}

bool Device::connected() const { return connected_;}

bool Device::needsPassPhraseSent() const { return needs_pass_phrase_sent_;}

bool Device::needsPinSent() const { return needs_pin_sent_;}

void Device::setName(const QString &name)
{
    if(name_ != name){
        name_ = name;
        emit nameChanged();
    }
}

void Device::setType(const QString &d) {
    if(d != type_){
        type_ = d;
        emit typeChanged();
    }
}

void Device::setPath(const QString &d) {
    if(d != path_){
        path_ = d;
        emit pathChanged();
    }
}

void Device::setModel(const QString &d) {
    if(d != model_){
        model_ = d;
        emit modelChanged();
    }
}

void Device::setMasterFingerPrint(const QString &d) {
    if(d != master_fingerprint_){
        master_fingerprint_ = d;
        emit masterFingerPrintChanged();
    }
}

void Device::setConnected(const bool d) {
    if(d != connected_){
        connected_ = d;
        emit connectedChanged();
    }
}

void Device::setNeedsPassPhraseSent(const bool d) {
    if(d != needs_pass_phrase_sent_){
        needs_pass_phrase_sent_ = d;
        emit needsPassPhraseSentChanged();
    }
}

void Device::setNeedsPinSent(const bool d) {
    if(d != needs_pin_sent_){
        needs_pin_sent_ = d;
        emit needsPinSentChanged();
    }
}

bool Device::usableToSign() const
{
    return usableToSign_;
}

void Device::setUsableToSign(bool usable)
{
    if(usableToSign_ != usable){
        usableToSign_ = usable;
        emit usableToSignChanged();
    }
}

bool Device::usableToAdd() const
{
    return usableToAdd_;
}

void Device::setUsableToAdd(bool usableToAdd)
{
    if(usableToAdd_ != usableToAdd){
        usableToAdd_ = usableToAdd;
        emit usableToAddChanged();
    }
}

QString Device::masterSignerId() const
{
    return master_signer_id_;
}

void Device::setMasterSignerId(const QString &master_signer_id)
{
    if(master_signer_id_ != master_signer_id){
        master_signer_id_ = master_signer_id;
        emit masterSignerIdChanged();
    }
}

DeviceListModel::DeviceListModel(): warningMessage_(QSharedPointer<QWarningMessage>(new QWarningMessage()))
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

DeviceListModel::~DeviceListModel(){
    d_.clear();
    warningMessage_.clear();
}

int DeviceListModel::rowCount(const QModelIndex &parent) const{
    Q_UNUSED(parent);
    return d_.count();
}

QVariant DeviceListModel::data(const QModelIndex &index, int role) const {
    switch (role) {
    case device_name_role:
        return d_[index.row()]->name();
    case device_type_role:
        return d_[index.row()]->type();
    case device_path_role:
        return d_[index.row()]->path();
    case device_model_role:
        return d_[index.row()]->model();
    case device_master_fingerprint_role:
        return d_[index.row()]->masterFingerPrint();
    case device_connected_role:
        return d_[index.row()]->connected();
    case device_needs_pass_phrase_sent_role:
        return d_[index.row()]->needsPassPhraseSent();
    case device_needs_pin_sent_role:
        return d_[index.row()]->needsPinSent();
    case device_usableToSign_role:
        return d_[index.row()]->usableToSign();
    case device_usableToAdd_role:
        return d_[index.row()]->usableToAdd();
    case device_master_signer_id_role:
        return d_[index.row()]->masterSignerId();
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> DeviceListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[device_name_role] = "device_name";
    roles[device_type_role] = "device_type";
    roles[device_path_role] = "device_path";
    roles[device_model_role] = "device_model";
    roles[device_master_fingerprint_role] = "device_master_fingerprint";
    roles[device_connected_role] = "device_connected";
    roles[device_needs_pass_phrase_sent_role] = "device_needs_pass_phrase_sent";
    roles[device_needs_pin_sent_role] = "device_needs_pin_sent";
    roles[device_usableToSign_role] = "device_usable_to_sign";
    roles[device_usableToAdd_role] = "device_usable_to_add";
    roles[device_master_signer_id_role] = "device_master_signer_id";

    return roles;
}

void DeviceListModel::addDevice(const QString &name,
                                const QString &type,
                                const QString &path,
                                const QString &model,
                                const QString &master_fingerprint,
                                bool connected,
                                bool needs_pass_phrase_sent,
                                bool needs_pin_sent,
                                QString mastersigner_id)
{
    beginResetModel();
    if(!contains(master_fingerprint)){
        d_.append(QSharedPointer<Device>(new Device(name, type, path, model, master_fingerprint, connected, needs_pass_phrase_sent, needs_pin_sent, mastersigner_id)));
    }
    endResetModel();
    emit deviceCountChanged();
}

void DeviceListModel::addDevice(const QSharedPointer<Device> &d){
    beginResetModel();
    if(d && !contains(d.data()->masterFingerPrint())){
        d_.append(d);
    }
    endResetModel();
    emit deviceCountChanged();
}

void DeviceListModel::updateDeviceList(const QSharedPointer<DeviceListModel> &d)
{
    beginResetModel();
    if(d){
        d_.clear();
        for (int var = 0; var < d.data()->rowCount(); var++) {
            d_.append(d.data()->getDeviceByIndex(var));
        }
    }
    endResetModel();
    emit deviceCountChanged();
}

QString DeviceListModel::getDevicePathByIndex(const int index){
    if(0 <= index && index < d_.count() ){
        return d_.at(index)->path();
    }
    else{
        return "";
    }
}

bool DeviceListModel::deviceNeedPinSent(const QString &xfp)
{
    bool needPin = false;
    foreach (QSharedPointer<Device> it, d_) {
        if(0 == QString::compare(xfp, it.data()->masterFingerPrint(), Qt::CaseInsensitive)){
            needPin = it.data()->needsPinSent();
        }
    }
    return needPin;
}

bool DeviceListModel::deviceNeedPinSent(const QString &deviceModel, const QString &deviceType)
{
    bool needPin = false;
     foreach (QSharedPointer<Device> it, d_) {
         if((0 == QString::compare(deviceModel, it.data()->model(), Qt::CaseInsensitive)) &&
            (0 == QString::compare(deviceType, it.data()->type(), Qt::CaseInsensitive)))
         {
             needPin = it.data()->needsPinSent();
         }
     }
     return needPin;
}

QSharedPointer<Device> DeviceListModel::getDeviceByIndex(const int index){
    if(0 <= index && index < d_.count() ){
        return d_.at(index);
    }
    else{
        return QSharedPointer<Device>(NULL);
    }
}

QSharedPointer<Device> DeviceListModel::getDeviceByPath(const QString& path){
    foreach (QSharedPointer<Device> it, d_) {
        if(0 == QString::compare(path, it.data()->path(), Qt::CaseInsensitive)){
            return it;
        }
    }
    return QSharedPointer<Device>(NULL);
}

QSharedPointer<Device> DeviceListModel::getDeviceByXFP(const QString& xfp){
    foreach (QSharedPointer<Device> it, d_) {
        if(0 == QString::compare(xfp, it.data()->masterFingerPrint(), Qt::CaseInsensitive)){
            return it;
        }
    }
    return QSharedPointer<Device>(NULL);
}

QSharedPointer<Device> DeviceListModel::getDeviceNeedPinSent(){
    foreach (QSharedPointer<Device> it, d_) {
        if(it.data()->needsPinSent()){
            return it;
        }
    }
    return QSharedPointer<Device>(NULL);
}

void DeviceListModel::updateDeviceConnected(QString xfp)
{
    foreach (QSharedPointer<Device> i, d_) {
        if(0 == QString::compare(xfp, i.data()->masterFingerPrint(), Qt::CaseInsensitive)){
            i.data()->setConnected(true);
        }
        emit dataChanged(this->index(d_.indexOf(i)),this->index(d_.indexOf(i)));
    }
}

void DeviceListModel::resetDeviceConnected()
{
    foreach (QSharedPointer<Device> i, d_) {
        i.data()->setConnected(false);
        emit dataChanged(this->index(d_.indexOf(i)),this->index(d_.indexOf(i)));
    }
}

QSharedPointer<QWarningMessage> DeviceListModel::warningMessagePtr() const
{
    return warningMessage_;
}

QWarningMessage* DeviceListModel::warningMessage() const
{
    return warningMessage_.data();
}

void DeviceListModel::setWarningMessage(const QSharedPointer<QWarningMessage> &warningMessage)
{
    warningMessage_ = warningMessage;
    emit warningMessageChanged();
}

void DeviceListModel::resetUsableToSign()
{
    beginResetModel();
    foreach (QSharedPointer<Device> it, d_) {
        it.data()->setUsableToSign(true);
    }
    endResetModel();
}

void DeviceListModel::resetUsableToAdd()
{
    beginResetModel();
    foreach (QSharedPointer<Device> it, d_) {
        it.data()->setUsableToAdd(false);
    }
    endResetModel();
}

void DeviceListModel::updateUsableToSign(const QString &fingerprint, bool value)
{
    foreach (QSharedPointer<Device> it, d_) {
        if(0 == QString::compare(fingerprint, it.data()->masterFingerPrint(), Qt::CaseInsensitive)){
            it.data()->setUsableToSign(value);
            emit dataChanged(this->index(d_.indexOf(it)),this->index(d_.indexOf(it)));
        }
    }
    emit containsSignableChanged();
}

void DeviceListModel::updateUsableToAdd(const QString &fingerprint, bool value)
{
    foreach (QSharedPointer<Device> it, d_) {
        if(0 == QString::compare(fingerprint, it.data()->masterFingerPrint(), Qt::CaseInsensitive)){
            it.data()->setUsableToAdd(value);
            emit dataChanged(this->index(d_.indexOf(it)),this->index(d_.indexOf(it)));
        }
    }
    emit containsAddableChanged();
}

bool DeviceListModel::contains(const QString &fingerprint)
{
    foreach (QSharedPointer<Device> i , d_ ){
        if(0 == QString::compare(fingerprint, i.data()->masterFingerPrint(), Qt::CaseInsensitive)){
            return true;
        }
    }
    return false;
}

bool DeviceListModel::containsAddable() const
{
    foreach (QSharedPointer<Device> i , d_ ){
        if(i.data()->usableToAdd()){
            return true;
        }
    }
    return false;
}

bool DeviceListModel::containsSignable() const
{
    foreach (QSharedPointer<Device> i , d_ ){
        if(i.data()->usableToSign()){
            return true;
        }
    }
    return false;
}

QStringList DeviceListModel::getXFPList()
{
    QStringList ret;
    ret.clear();
    foreach (QSharedPointer<Device> i , d_ ){
        ret.append(i.data()->masterFingerPrint());
    }
    return ret;
}

bool DeviceListModel::containsNeedPinSent()
{
    foreach (QSharedPointer<Device> i , d_ ){
        if(i.data()->needsPinSent()){
            return true;
        }
    }
    return false;
}

void DeviceListModel::updateDeviceNeedPassphraseSentByMasterSignerId(const QString &id, bool needpassphrase)
{
    beginResetModel();
    foreach (QSharedPointer<Device> it, d_) {
        if(0 == QString::compare(id, it.data()->masterSignerId(), Qt::CaseInsensitive)){
            it.data()->setUsableToAdd(needpassphrase);
        }
    }
    endResetModel();
}

void DeviceListModel::notifySoftwareSignerRenamed(const QString& mastersignerid, const QString &newname)
{
    beginResetModel();
    foreach (QSharedPointer<Device> i , d_ ){
        if(0 == QString::compare(mastersignerid, i.data()->masterSignerId(), Qt::CaseInsensitive)){
            i.data()->setName(newname);
        }
    }
    endResetModel();
}

int DeviceListModel::deviceCount() const
{
    return d_.count();
}
