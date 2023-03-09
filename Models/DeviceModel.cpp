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
#include "DeviceModel.h"
#include <QQmlEngine>

QDevice::QDevice() :
    name_("UNKNOWN"),
    type_("UNKNOWN"),
    path_("UNKNOWN"),
    model_("UNKNOWN"),
    master_fingerprint_("UNKNOWN"),
    connected_(false),
    needs_pass_phrase_sent_(false),
    needs_pin_sent_(false),
    usableToAdd_(true),
    master_signer_id_(""),
    cardId_("")
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

QDevice::QDevice(const QString &fingerprint) :
    name_(""),
    type_(""),
    path_(""),
    model_(""),
    master_fingerprint_(fingerprint),
    connected_(false),
    needs_pass_phrase_sent_(false),
    needs_pin_sent_(false),
    usableToAdd_(true),
    master_signer_id_(""),
    cardId_("")
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

QDevice::QDevice(const QString &name,
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
    usableToAdd_(true),
    master_signer_id_(mastersigner_id),
    cardId_("")
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

QDevice::~QDevice(){}

QString QDevice::name() const
{
    if(name_ == "" || name_.isEmpty() || name_.isNull()){
        return type_;
    }
    return name_;
}

QString QDevice::type() const { return type_;}

QString QDevice::path() const { return path_;}

QString QDevice::model() const { return model_;}

QString QDevice::masterFingerPrint() const { return master_fingerprint_;}

bool QDevice::connected() const { return connected_;}

bool QDevice::needsPassPhraseSent() const { return needs_pass_phrase_sent_;}

bool QDevice::needsPinSent() const { return needs_pin_sent_;}

void QDevice::setName(const QString &name)
{
    if(name_ != name){
        name_ = name;
        emit nameChanged();
    }
}

void QDevice::setType(const QString &d) {
    if(d != type_){
        type_ = d;
        emit typeChanged();
    }
}

void QDevice::setPath(const QString &d) {
    if(d != path_){
        path_ = d;
        emit pathChanged();
    }
}

void QDevice::setModel(const QString &d) {
    if(d != model_){
        model_ = d;
        emit modelChanged();
    }
}

void QDevice::setMasterFingerPrint(const QString &d) {
    if(d != master_fingerprint_){
        master_fingerprint_ = d;
        emit masterFingerPrintChanged();
    }
}

void QDevice::setConnected(const bool d) {
    if(d != connected_){
        connected_ = d;
        emit connectedChanged();
    }
}

void QDevice::setNeedsPassPhraseSent(const bool d) {
    if(d != needs_pass_phrase_sent_){
        DBG_INFO << d;
        needs_pass_phrase_sent_ = d;
        emit needsPassPhraseSentChanged();
    }
}

void QDevice::setNeedsPinSent(const bool d) {
    if(d != needs_pin_sent_){
        needs_pin_sent_ = d;
        emit needsPinSentChanged();
    }
}

bool QDevice::usableToAdd() const
{
    return usableToAdd_;
}

void QDevice::setUsableToAdd(bool usableToAdd)
{
    if(usableToAdd_ != usableToAdd){
        usableToAdd_ = usableToAdd;
        emit usableToAddChanged();
    }
}

QString QDevice::masterSignerId() const
{
    return master_signer_id_;
}

void QDevice::setMasterSignerId(const QString &master_signer_id)
{
    if(master_signer_id_ != master_signer_id){
        master_signer_id_ = master_signer_id;
        emit masterSignerIdChanged();
    }
}

QString QDevice::cardId() const
{
    return cardId_;
}

QDevice& QDevice::setCardId(const QString &card_id)
{
    cardId_ = card_id;
    return *this;
}

DeviceListModel::DeviceListModel()
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

DeviceListModel::~DeviceListModel(){
    d_.clear();
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
        d_.append(QDevicePtr(new QDevice(name, type, path, model, master_fingerprint, connected, needs_pass_phrase_sent, needs_pin_sent, mastersigner_id)));
    }
    endResetModel();
    emit deviceCountChanged();
}

void DeviceListModel::addDevice(const QDevicePtr &d){
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
    d_.clear();
    if(d){
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

QString DeviceListModel::getDevicePathByXfp(const QString &xfp)
{
    foreach (QDevicePtr it, d_) {
        if(0 == QString::compare(xfp, it.data()->masterFingerPrint(), Qt::CaseInsensitive)){
            return it.data()->path();
        }
    }
    return "";
}

bool DeviceListModel::deviceNeedPinSent(const QString &xfp)
{
    bool needPin = false;
    foreach (QDevicePtr it, d_) {
        if(0 == QString::compare(xfp, it.data()->masterFingerPrint(), Qt::CaseInsensitive)){
            needPin = it.data()->needsPinSent();
        }
    }
    return needPin;
}

bool DeviceListModel::deviceNeedPinSent(const QString &deviceModel, const QString &deviceType)
{
    bool needPin = false;
     foreach (QDevicePtr it, d_) {
         if((0 == QString::compare(deviceModel, it.data()->model(), Qt::CaseInsensitive)) &&
            (0 == QString::compare(deviceType, it.data()->type(), Qt::CaseInsensitive)))
         {
             needPin = it.data()->needsPinSent();
         }
     }
     return needPin;
}

QDevicePtr DeviceListModel::getDeviceByIndex(const int index){
    if(0 <= index && index < d_.count() ){
        return d_.at(index);
    }
    else{
        return QDevicePtr(NULL);
    }
}

QDevicePtr DeviceListModel::getDeviceByPath(const QString& path){
    foreach (QDevicePtr it, d_) {
        if(0 == QString::compare(path, it.data()->path(), Qt::CaseInsensitive)){
            return it;
        }
    }
    return QDevicePtr(NULL);
}

QDevicePtr DeviceListModel::getDeviceByXfp(const QString& xfp){
    foreach (QDevicePtr it, d_) {
        if(0 == QString::compare(xfp, it.data()->masterFingerPrint(), Qt::CaseInsensitive)){
            return it;
        }
    }
    return NULL;
}

QDevicePtr DeviceListModel::getDeviceNeedPinSent(){
    foreach (QDevicePtr it, d_) {
        if(it.data()->needsPinSent()){
            return it;
        }
    }
    return QDevicePtr(NULL);
}

void DeviceListModel::resetDeviceConnected()
{
    foreach (QDevicePtr i, d_) {
        i.data()->setConnected(false);
        emit dataChanged(this->index(d_.indexOf(i)),this->index(d_.indexOf(i)));
    }
}

void DeviceListModel::resetUsableToAdd()
{
    beginResetModel();
    foreach (QDevicePtr it, d_) {
        it.data()->setUsableToAdd(false);
    }
    endResetModel();
}

void DeviceListModel::updateUsableToAdd(const QString &fingerprint, bool value)
{
    foreach (QDevicePtr it, d_) {
        if(0 == QString::compare(fingerprint, it.data()->masterFingerPrint(), Qt::CaseInsensitive)){
            it.data()->setUsableToAdd(value);
            emit dataChanged(this->index(d_.indexOf(it)),this->index(d_.indexOf(it)));
        }
    }
    emit containsAddableChanged();
}

bool DeviceListModel::contains(const QString &fingerprint)
{
    foreach (QDevicePtr i , d_ ){
        if(0 == QString::compare(fingerprint, i.data()->masterFingerPrint(), Qt::CaseInsensitive)){
            return true;
        }
    }
    return false;
}

bool DeviceListModel::needScanDevice()
{
    if(d_.count() == 0){
        return true;
    }
    foreach (QDevicePtr i , d_ ){
        DBG_INFO << i.data()->needsPinSent() << i.data()->needsPassPhraseSent();
        if(i.data()->needsPinSent() || i.data()->needsPassPhraseSent()){
            return true;
        }
    }
    return false;
}

bool DeviceListModel::containsAddable() const
{
    foreach (QDevicePtr i , d_ ){
        if(i.data()->usableToAdd()){
            return true;
        }
    }
    return false;
}

QStringList DeviceListModel::getXFPList()
{
    QStringList ret;
    ret.clear();
    foreach (QDevicePtr i , d_ ){
        ret.append(i.data()->masterFingerPrint());
    }
    return ret;
}

bool DeviceListModel::containsNeedPinSent()
{
    foreach (QDevicePtr i , d_ ){
        if(i.data()->needsPinSent()){
            return true;
        }
    }
    return false;
}

void DeviceListModel::updateNeedPassphraseSentById(const QString &id, bool needpassphrase)
{
    beginResetModel();
    foreach (QDevicePtr it, d_) {
        if(0 == QString::compare(id, it.data()->masterSignerId(), Qt::CaseInsensitive)){
            it.data()->setUsableToAdd(needpassphrase);
        }
    }
    endResetModel();
}

void DeviceListModel::renameById(const QString& id, const QString &newname)
{
    beginResetModel();
    foreach (QDevicePtr i , d_ ){
        if(0 == QString::compare(id, i.data()->masterSignerId(), Qt::CaseInsensitive)){
            i.data()->setName(newname);
        }
    }
    endResetModel();
}

void DeviceListModel::checkAndUnlockDevice()
{

}

int DeviceListModel::deviceCount() const
{
    return d_.count();
}

int DeviceListModel::getDeviceIndexByXfp(const QString &xfp)
{
    for (int i = 0; i < d_.count(); i++) {
        if(0 == QString::compare(xfp, d_.at(i).data()->masterFingerPrint(), Qt::CaseInsensitive)){
            return i;
        }
    }
    return -1;
}

QList<QDevicePtr> DeviceListModel::fullList() const
{
    return d_;
}
