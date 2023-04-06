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
#include "MasterSignerModel.h"
#include <QQmlEngine>
#include "bridgeifaces.h"

QMasterSigner::QMasterSigner(): isPrimaryKey_(false), isDraft(true)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

QMasterSigner::QMasterSigner(const nunchuk::MasterSigner &signer) : masterSigner_(signer), isPrimaryKey_(false)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

QMasterSigner::QMasterSigner(const nunchuk::PrimaryKey &key) : primaryKey_(key), isPrimaryKey_(true)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

QMasterSigner::~QMasterSigner(){

}

QString QMasterSigner::id() const{
    if(isDraft){
        return id_;
    }
    else{
        return QString::fromStdString(masterSigner_.get_id());
    }
}

void QMasterSigner::setId(const QString &d){
    if(isDraft && d != id_){
        id_ = d;
        emit idChanged();
    }
}

QString QMasterSigner::name() const {
    if(isDraft){
        return name_;
    }
    else{
        return QString::fromStdString(masterSigner_.get_name());
    }
}

void QMasterSigner::setName(const QString &d) {
    if(isDraft){
        name_ = d;
    }
    else{
        masterSigner_.set_name(d.toStdString());
    }
    emit nameChanged();
}

QDevice *QMasterSigner::device() {
    if(!device_){
        if(isDraft){
            device_ = QDevicePtr(new QDevice());
        }
        else{
            device_ = QDevicePtr(new QDevice(masterSigner_.get_device()));
        }
    }
    return device_.data();
}

QDevicePtr QMasterSigner::devicePtr()
{
    if(!device_){
        if(isDraft){
            device_ = QDevicePtr(new QDevice());
        }
        else{
            device_ = QDevicePtr(new QDevice(masterSigner_.get_device()));
        }
    }
    return device_;
}

int QMasterSigner::health() const {
    return health_;
}

void QMasterSigner::setHealth(const int d)
{
    if(d != health_){
        health_ = d;
        emit healthChanged();
    }
}

bool QMasterSigner::connected() const
{
    if(!device_){
        return false;
    }
    else{
        return masterSigner_.get_device().connected();
    }
}

bool QMasterSigner::checked() const
{
    return checked_;
}

void QMasterSigner::setChecked(const bool checked)
{
    if(checked != checked_){
        checked_ = checked;
        emit checkedChanged();
    }
}

bool QMasterSigner::needPinSent() const
{
    return masterSigner_.get_device().needs_pin_sent();
}

bool QMasterSigner::needPassphraseSent() const
{
    return masterSigner_.get_device().needs_pass_phrase_sent();
}

QString QMasterSigner::deviceType() const
{
    if(isDraft){
        return deviceType_;
    }
    else{
        return QString::fromStdString(masterSigner_.get_device().get_type());
    }
}

void QMasterSigner::setDeviceType(const QString &d)
{
    if(isDraft){
        deviceType_ = d;
        emit deviceTypeChanged();
    }
}

bool QMasterSigner::needXpub() const
{
    return isNeedXpub_;
}

void QMasterSigner::setNeedXpub(bool isNeedXpub)
{
    isNeedXpub_ = isNeedXpub;
}

QString QMasterSigner::fingerPrint() const
{
    if(isDraft){
        return xfp_;
    }
    else{
        return QString::fromStdString(masterSigner_.get_device().get_master_fingerprint());
    }
}

void QMasterSigner::setFingerPrint(const QString &d)
{
    if(isDraft){
        xfp_ = d;
        emit fingerPrintChanged();
    }
}

QString QMasterSigner::message()
{
    if(messageToSign_.isEmpty() || messageToSign_.isNull() || messageToSign_ == ""){
        messageToSign_ = qUtils::QGenerateRandomMessage();
    }
    return messageToSign_;
}

QString QMasterSigner::messageSHA256()
{
    return qUtils::GetSHA256(messageToSign_);
}

void QMasterSigner::setMessage(const QString &messageToSign)
{
    if(messageToSign_ != messageToSign){
        messageToSign_ = messageToSign;
        emit messageToSignChanged();
    }
}

QString QMasterSigner::signature() const
{
    return signature_;
}

void QMasterSigner::setSignature(const QString &signature)
{
    if(signature_ != signature){
        signature_ = signature;
        emit signatureChanged();
    }
}

QString QMasterSigner::path() const
{
    return path_;
}

void QMasterSigner::setPath(const QString &d)
{
    if(path_ != d){
        path_ = d;
        emit pathChanged();
    }
}

int QMasterSigner::signerType() const
{
    if(isDraft){
        return signer_type_;
    }
    else{
        return (int)masterSigner_.get_type();
    }
}

void QMasterSigner::setSignerType(int signer_type)
{
    if(isDraft){
        signer_type_ = signer_type;
        emit signerTypeChanged();
    }
}

QString QMasterSigner::passphrase() const
{
    return passphrase_;
}

void QMasterSigner::setPassphrase(const QString &passphrase){
    if(passphrase_ != passphrase){
        passphrase_ = passphrase;
        emit passphraseChanged();
    }
}

bool QMasterSigner::isPrimaryKey() const
{
    return isPrimaryKey_;
}

void QMasterSigner::setIsPrimaryKey(bool isPrimaryKey)
{
    if (isPrimaryKey_ == isPrimaryKey)
        return;

    isPrimaryKey_ = isPrimaryKey;
    setIsPrimaryKey(true);
    emit isPrimaryKeyChanged();
}

nunchuk::PrimaryKey QMasterSigner::originPrimaryKey() const
{
    return primaryKey_;
}

nunchuk::MasterSigner QMasterSigner::originMasterSigner() const
{
    return masterSigner_;
}

void QMasterSigner::setOriginMasterSigner(const nunchuk::MasterSigner &signer)
{
    masterSigner_ = signer;
}

MasterSignerListModel::MasterSignerListModel() {
    primaryKeys = qUtils::GetPrimaryKeys(AppSetting::instance()->storagePath(),(nunchuk::Chain)AppSetting::instance()->primaryServer());
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
        return qVariantFromValue((QDevice*) d_[index.row()]->device());
    case master_signer_checked_Role:
        return d_[index.row()]->checked();
    case master_signer_fingerPrint_Role:
        return d_[index.row()]->fingerPrint();
    case master_signer_msg2Sign_Role:
        return d_[index.row()]->message();
    case master_signer_deviceType_Role:
        return d_[index.row()]->deviceType();
    case master_signer_signerType_Role:
        return d_[index.row()]->signerType();
    case master_signer_need_passphrase_Role:
        return d_[index.row()]->needPassphraseSent();
    case master_signer_need_pin_Role:
        return d_[index.row()]->needPinSent();
    case master_signer_need_xpub_Role:
        return d_[index.row()]->needXpub();
    case master_signer_primary_key_Role:
        return d_[index.row()]->isPrimaryKey();
    default:
        return QVariant();
    }
}

bool MasterSignerListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role == master_signer_checked_Role) {
        d_[index.row()]->setChecked(value.toBool());
    }
    else{ }
    emit dataChanged(index, index, { role } );
    return true;
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
    roles[master_signer_signerType_Role]    = "master_signer_type";
    roles[master_signer_need_passphrase_Role]    = "master_signer_need_passphrase";
    roles[master_signer_need_pin_Role]      = "master_signer_need_pin";
    roles[master_signer_need_xpub_Role]     = "master_signer_need_xpub";
    roles[master_signer_primary_key_Role]   = "master_signer_primary_key";
    return roles;
}

void MasterSignerListModel::addMasterSigner(const QMasterSignerPtr &d)
{
    beginResetModel();
    if(!contains(d.data()->id())){
        d_.append(d);
    }
    endResetModel();
}

QMasterSignerPtr MasterSignerListModel::getMasterSignerByIndex(const int index) {
    if(index < 0 || index >= d_.count()){
        DBG_INFO << "Index out of range";
        return QMasterSignerPtr(NULL);
    }
    else {
        return d_.at(index);
    }
}

QMasterSignerPtr MasterSignerListModel::getMasterSignerById(const QString &id)
{
    foreach (QMasterSignerPtr it, d_) {
        if(it.data()->id() == id){
            return it;
        }
    }
    return QMasterSignerPtr(NULL);
}

QMasterSignerPtr MasterSignerListModel::getMasterSignerByXfp(const QString &xfp)
{
    foreach (QMasterSignerPtr it, d_) {
        if(0 == QString::compare(xfp, it.data()->fingerPrint(), Qt::CaseInsensitive)){
            return it;
        }
    }
    return NULL;
}

QString MasterSignerListModel::getMasterSignerNameByFingerPrint(const QString &fingerprint)
{
    foreach (QMasterSignerPtr it, d_) {
        if(0 == QString::compare(fingerprint, it.data()->fingerPrint(), Qt::CaseInsensitive)){
            return it.data()->name();
        }
    }
    return "";
}

bool MasterSignerListModel::removeMasterSigner(const QString &masterSignerId)
{
    beginResetModel();
    foreach (QMasterSignerPtr it, d_) {
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
    foreach (QMasterSignerPtr it, d_) {
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
    foreach (QMasterSignerPtr it, d_) {
        if(0 == QString::compare(id, it.data()->id(), Qt::CaseInsensitive)){
            it.data()->setChecked(state);
        }
    }
    endResetModel();
}

void MasterSignerListModel::setUserCheckedByFingerprint(const bool state, const QString &xfp)
{
    beginResetModel();
    foreach (QMasterSignerPtr it, d_) {
        if(0 == QString::compare(xfp, it.data()->fingerPrint(), Qt::CaseInsensitive)){
            it.data()->setChecked(state);
        }
    }
    endResetModel();
}

bool MasterSignerListModel::removeMasterSigner(const QMasterSignerPtr it)
{
    beginResetModel();
    d_.removeAll(it);
    endResetModel();
    return true;
}

bool MasterSignerListModel::contains(const QString &masterSignerId)
{
    foreach (QMasterSignerPtr i , d_ ){
        if(0 == QString::compare(masterSignerId, i.data()->id(), Qt::CaseInsensitive)){
            return true;
        }
    }
    return false;
}

bool MasterSignerListModel::containsFingerPrint(const QString &fingerprint)
{
    foreach (QMasterSignerPtr i , d_ ){
        if(0 == QString::compare(fingerprint, i.data()->fingerPrint(), Qt::CaseInsensitive)){
            return true;
        }
    }
    return false;
}

void MasterSignerListModel::updateMasterSignerNeedXpubById(const QString &id, const bool value)
{
    beginResetModel();
    foreach (QMasterSignerPtr i , d_ ){
        if(0 == QString::compare(id, i.data()->id(), Qt::CaseInsensitive)){
            i.data()->setNeedXpub(value);
        }
    }
    endResetModel();
}

void MasterSignerListModel::updateMasterSignerNeedXpubByXfp(const QString &xfp, const bool value)
{
    beginResetModel();
    foreach (QMasterSignerPtr i , d_ ){
        if(0 == QString::compare(xfp, i.data()->fingerPrint(), Qt::CaseInsensitive)){
            i.data()->setNeedXpub(value);
        }
    }
    endResetModel();
}

void MasterSignerListModel::resetMasterSignerNeedXpub()
{
    beginResetModel();
    foreach (QMasterSignerPtr i , d_ ){
        i.data()->setNeedXpub(false);
    }
    endResetModel();
}

QStringList MasterSignerListModel::getColdCardId() const
{
    QStringList ret;
    ret.clear();
    foreach (QMasterSignerPtr i , d_ ){
        if(i.data()->device() && 0 == QString::compare("coldcard", i.data()->device()->type(), Qt::CaseInsensitive)){
            ret << i.data()->id();
        }
    }
    return ret;
}

bool MasterSignerListModel::isColdCard(const QString &xfp)
{
    foreach (QMasterSignerPtr i , d_ ){
        if(i.data()->signerType() == (int)ENUNCHUCK::SignerType::COLDCARD_NFC){
            return true;
        }
        else{
            if(i.data()->device() && 0 == QString::compare("coldcard", i.data()->device()->type(), Qt::CaseInsensitive)){
                return true;
            }
        }
    }
    return false;
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

QList<QMasterSignerPtr> MasterSignerListModel::fullList() const
{
    return d_;
}

void MasterSignerListModel::cleardata()
{
    beginResetModel();
    d_.clear();
    endResetModel();
}

int MasterSignerListModel::signerCount() const
{
    return d_.count();
}

int MasterSignerListModel::signerSelectedCount() const
{
    int ret = 0;
    foreach (QMasterSignerPtr it, d_) {
        if(it.data()->checked()) { ret++;}
    }
    return ret;
}

nunchuk::PrimaryKey MasterSignerListModel::containPrimaryKey(const QString &fingerprint)
{
    for(nunchuk::PrimaryKey key: primaryKeys){
        if(key.get_master_fingerprint() == fingerprint.toStdString()){
            return key;
        }
    }
    return nunchuk::PrimaryKey();
}

void MasterSignerListModel::reloadOriginMasterSignerById(const QString &id)
{
    beginResetModel();
    foreach (QMasterSignerPtr i , d_ ){
        if(0 == QString::compare(id, i.data()->id(), Qt::CaseInsensitive)){
            QWarningMessage msg;
            nunchuk::MasterSigner signer = bridge::nunchukGetOriginMasterSigner(id, msg);
            if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
                i.data()->setOriginMasterSigner(signer);
            }
        }
    }
    endResetModel();
}

bool sortMasterSignerByNameAscending(const QMasterSignerPtr &v1, const QMasterSignerPtr &v2)
{
    return (QString::compare((v1.data()->name()), (v2.data()->name())) < 0);
}

bool sortMasterSignerByNameDescending(const QMasterSignerPtr &v1, const QMasterSignerPtr &v2)
{
    return (QString::compare((v1.data()->name()), (v2.data()->name())) > 0);
}
