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
#include "AppModel.h"
#include <QQmlEngine>

QDevice::QDevice(): m_isDraft(true)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

QDevice::QDevice(const nunchuk::Device &device) : m_device(device)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

QDevice::~QDevice(){}

QString QDevice::name()
{
    QString name = "";
    QString xfp = QString::fromStdString(m_device.get_master_fingerprint());
    if(AppModel::instance()->masterSignerList()){
        name = AppModel::instance()->masterSignerList()->getMasterSignerNameByFingerPrint(xfp);
    }
    if(name == ""){
        name = QString::fromStdString(m_device.get_type());
    }
    return name;
}

QString QDevice::type() const {
    return QString::fromStdString(m_device.get_type());
}

QString QDevice::path() const {
    return QString::fromStdString(m_device.get_path());
}

QString QDevice::model() const {
    return QString::fromStdString(m_device.get_model());
}

QString QDevice::masterFingerPrint() const {
    return QString::fromStdString(m_device.get_master_fingerprint());
}

bool QDevice::connected() const {
    return m_device.connected();
}

bool QDevice::needsPassPhraseSent() const {
    return m_device.needs_pass_phrase_sent();
}

bool QDevice::needsPinSent() const {
    return m_device.needs_pin_sent();
}

bool QDevice::usableToAdd()
{
    bool used = false;
    if(AppModel::instance()->masterSignerList()){
        used = AppModel::instance()->masterSignerList()->containsFingerPrint(masterFingerPrint());
    }
    if(!used && AppModel::instance()->remoteSignerList()){
        used = AppModel::instance()->remoteSignerList()->containsFingerPrint(masterFingerPrint());
    }
    return !used;
}

QString QDevice::masterSignerId() const
{
    QString id = "";
    if(AppModel::instance()->masterSignerList()){
        QMasterSignerPtr signer = AppModel::instance()->masterSignerList()->getMasterSignerByXfp(masterFingerPrint());
        if(signer){
            id = signer.data()->id();
        }
    }
    return id;
}

QString QDevice::cardId() const
{
    return m_cardId;
}

void QDevice::setCardId(const QString &card_id)
{
    m_cardId = card_id;
}

nunchuk::Device QDevice::originDevice() const
{
    return m_device;
}

void QDevice::setOriginDevice(const nunchuk::Device &device)
{
    m_device = device;
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

void DeviceListModel::addDevice(const QDevicePtr &device){
    beginResetModel();
    if(device && !contains(device.data()->masterFingerPrint())){
        d_.append(device);
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

bool DeviceListModel::containsFingerPrint(const QString &xfp)
{
    foreach (QDevicePtr it, d_) {
        if(0 == QString::compare(xfp, it.data()->masterFingerPrint(), Qt::CaseInsensitive)){
            return true;
        }
    }
    return false;
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

QStringList DeviceListModel::getXFPList()
{
    QStringList ret;
    ret.clear();
    foreach (QDevicePtr i , d_ ){
        ret.append(i.data()->masterFingerPrint());
    }
    return ret;
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
