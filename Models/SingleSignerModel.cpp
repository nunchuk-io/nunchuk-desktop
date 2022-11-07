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
#include "SingleSignerModel.h"
#include "qUtils.h"
#include "bridgeifaces.h"
#include <QQmlEngine>

SingleSigner::SingleSigner() :  name_(""),
    xpub_(""),
    public_key_(""),
    derivation_path_(""),
    master_fingerprint_(""),
    master_signer_id_(""),
    last_health_check_(QDateTime::fromTime_t(0)),
    signer_type_(0),
    signed_(false),
    isValid_(false),
    message_(qUtils::QGenerateRandomMessage()),
    signature_(""),
    health_(-1),
    isColdCard_(false),
    checked_(false),
    readyToSign_(false),
    isLocalSigner_(true),
    isPrimaryKey_(false)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

SingleSigner::SingleSigner(const QString &pr_name,
                           const QString &pr_xpub,
                           const QString &pr_public_key,
                           const QString &pr_derivation_path,
                           const QString &pr_master_fingerprint,
                           const QString &pr_master_signer_id,
                           const QDateTime& pr_last_health_check,
                           const nunchuk::PrimaryKey &key)
    : name_(pr_name),
      xpub_(pr_xpub),
      public_key_(pr_public_key),
      derivation_path_(pr_derivation_path),
      master_fingerprint_(pr_master_fingerprint),
      master_signer_id_(pr_master_signer_id),
      last_health_check_(pr_last_health_check),
      signer_type_(0),
      signed_(false),
      isValid_(false),
      message_(qUtils::QGenerateRandomMessage()),
      signature_(""),
      health_(-1),
      isColdCard_(false),
      checked_(false),
      readyToSign_(false),
      isLocalSigner_(true),
      isPrimaryKey_(false),
      primaryKey_(key)
{
    if(key.get_master_fingerprint() != ""){
        isPrimaryKey_ = true;
    }
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

SingleSigner::~SingleSigner() {

}

QString SingleSigner::name() const {return name_;}

QString SingleSigner::xpub() const {return xpub_;}

QString SingleSigner::publickey() const {return public_key_;}

QString SingleSigner::derivationPath() const {return derivation_path_;}

QString SingleSigner::masterFingerPrint() const {return  master_fingerprint_;}

QString SingleSigner::masterSignerId() const {return master_signer_id_;}

QString SingleSigner::lastHealthCheck() {
    if(0 == last_health_check_.toTime_t()){
        return "N/A"; // There is no time
    }
    return this->timeGapCalculationShort(last_health_check_); // last_health_check_.toString( "MM/dd/yyyy hh:mm AP" );
}

QDateTime SingleSigner::lastHealthCheckDateTime() const {
    return last_health_check_;
}

QString SingleSigner::timeGapCalculationShort(QDateTime in)
{
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
                return QString("Just now");
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

QString SingleSigner::timeGapCalculation(QDateTime in)
{
    QDateTime today = QDateTime::currentDateTime();
    qint64 secs = in.secsTo(today);
    const qint64 DAY = 86400;
    qint64 days = secs / DAY;
    QTime t = QTime(0,0).addSecs(secs % DAY);
    if(abs(days) > 0){
        if(t.hour() > 0){
            return QString("%1 %2 %3 %4 ago").arg(abs(days)).arg(abs(days) > 1 ? "days" : "day").arg(t.hour()).arg(t.hour() > 1 ? "hours" : "hour");
        }
        else{
            return QString("%1 %2 ago").arg(abs(days)).arg(abs(days) > 1 ? "days" : "day");
        }
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
                return QString("Just now");
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

bool SingleSigner::signerSigned() const
{
    return signed_;
}

void SingleSigner::setName(const QString &d) {
    if(d != name_){
        name_ = d;
        emit nameChanged();
    }
}

void SingleSigner::setXpub(const QString &d) {
    if(d != xpub_){
        xpub_ = d;
        emit xpubChanged();
    }
}

void SingleSigner::setPublickey(const QString &d) {
    if(d != public_key_){
        public_key_ = d;
        emit publickeyChanged();
    }
}

void SingleSigner::setDerivationPath(const QString &d) {
    if(d != derivation_path_){
        derivation_path_ = d;
        emit derivationPathChanged();
    }
}

void SingleSigner::setMasterFingerPrint(const QString &d) {
    if(d != master_fingerprint_){
        master_fingerprint_ = d;
        emit masterFingerPrintChanged();
    }
}

void SingleSigner::setMasterSignerId(const QString &d) {
    if(d != master_signer_id_){
        master_signer_id_ = d;
        emit masterSignerIdChanged();
    }
}

void SingleSigner::setlastHealthCheck(const QDateTime& d) {
    if(d != last_health_check_){
        last_health_check_ = d;
        emit lastHealthCheckChanged();
    }
}

void SingleSigner::setSignerSigned(const bool d)
{
    if(d != signed_){
        signed_ = d;
        emit signerSignedChanged();
    }
}

bool SingleSigner::isValid() const
{
    return isValid_;
}

void SingleSigner::setIsValid(bool isValid)
{
    if(isValid_ != isValid){
        isValid_ = isValid;
        emit isValidChanged();
    }
}

QString SingleSigner::message() const
{
    return message_;
}

void SingleSigner::setMessage(const QString &message)
{
    if(message_ != message){
        message_ = message;
        emit messageChanged();
    }
}

QString SingleSigner::signature() const
{
    return signature_;
}

void SingleSigner::setSignature(const QString &signature)
{
    if(signature_ != signature){
        signature_ = signature;
        emit signatureChanged();
    }
}

int SingleSigner::health() const
{
    return health_;
}

void SingleSigner::setHealth(int health)
{
    if(health_ != health){
        health_ = health;
        emit healthChanged();
    }
}

bool SingleSigner::isColdCard() const
{
    return isColdCard_;
}

void SingleSigner::setIsColdCard(const bool isColdCard)
{
    if(isColdCard_ != isColdCard){
        isColdCard_ = isColdCard;
        emit isColdCardChanged();
    }
}

bool SingleSigner::checked() const
{
    return checked_;
}

void SingleSigner::setChecked(const bool checked)
{
    if(checked != checked_){
        checked_ = checked;
        emit checkedChanged();
    }
}

bool SingleSigner::readyToSign() const
{
    return readyToSign_;
}

void SingleSigner::setReadyToSign(bool available)
{
    if(readyToSign_ != available){
        readyToSign_ = available;
        emit readyToSignChanged();
    }
}

int SingleSigner::signerType() const
{
    return signer_type_;
}

void SingleSigner::setSignerType(int signer_type)
{
    if(signer_type_ != signer_type){
        signer_type_ = signer_type;
        emit signerTypeChanged();
    }
}

bool SingleSigner::isLocalSigner() const
{
    return isLocalSigner_;
}

void SingleSigner::setIsLocalSigner(bool isLocalSigner)
{
    if(isLocalSigner_ != isLocalSigner){
        isLocalSigner_ = isLocalSigner;
        emit isLocalSignerChanged();
    }
}

bool SingleSigner::isPrimaryKey() const
{
    return isPrimaryKey_;
}

void SingleSigner::setIsPrimaryKey(bool isPrimaryKey)
{
    isPrimaryKey_ = isPrimaryKey;
}

nunchuk::PrimaryKey SingleSigner::primaryKey() const
{
    return primaryKey_;
}

void SingleSigner::setPrimaryKey(const nunchuk::PrimaryKey &primaryKey)
{
    primaryKey_ = primaryKey;
}

SingleSignerListModel::SingleSignerListModel(){
    primaryKeys = qUtils::GetPrimaryKeys(AppSetting::instance()->storagePath(),(nunchuk::Chain)AppSetting::instance()->primaryServer());
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

SingleSignerListModel::~SingleSignerListModel(){d_.clear();}

int SingleSignerListModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return d_.count();
}

QVariant SingleSignerListModel::data(const QModelIndex &index, int role) const
{
    switch (role) {
    case single_signer_name_Role:
        return d_[index.row()]->name();
    case single_signer_xpub_Role:
        return d_[index.row()]->xpub();
    case single_signer_public_key_Role:
        return d_[index.row()]->publickey();
    case single_signer_derivation_path_Role:
        return d_[index.row()]->derivationPath();
    case single_signer_master_fingerprint_Role:
        return d_[index.row()]->masterFingerPrint();
    case single_signer_master_signer_id_Role:
        return d_[index.row()]->masterSignerId();
    case single_signer_last_health_check_Role:
        return d_[index.row()]->lastHealthCheck();
    case single_signer_signed_status_Role:
        return d_[index.row()]->signerSigned();
    case single_signer_type_Role:
        return d_[index.row()]->signerType();
    case single_signer_signed_isValid_Role:
        return d_[index.row()]->isValid();
    case single_signer_signed_message_Role:
        return d_[index.row()]->message();
    case single_signer_signed_signature_Role:
        return d_[index.row()]->signature();
    case single_signer_health_Role:
        return d_[index.row()]->health();
    case single_signer_isColdcard_Role:
        return d_[index.row()]->isColdCard();
    case single_signer_checked_Role:
        return d_[index.row()]->checked();
    case single_signer_readyToSign_Role:
        return d_[index.row()]->readyToSign();
    case single_signer_is_local_Role:
        return d_[index.row()]->isLocalSigner();
    case single_signer_primary_key_Role:
        return d_[index.row()]->isPrimaryKey();
    default:
        return QVariant();
    }
}

bool SingleSignerListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role == single_signer_signed_message_Role) {
        d_[index.row()]->setMessage(value.toString());
    }
    else if(role == single_signer_signed_signature_Role){
        d_[index.row()]->setSignature(value.toString());
    }
    else if(role == single_signer_checked_Role){
        d_[index.row()]->setChecked(value.toBool());
    }
    else{ }
    emit dataChanged(index, index, { role } );
    return true;
}

QHash<int, QByteArray> SingleSignerListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[single_signer_name_Role]                = "singleSigner_name";
    roles[single_signer_xpub_Role]                = "singleSigner_xpub";
    roles[single_signer_public_key_Role]          = "singleSigner_publickey";
    roles[single_signer_derivation_path_Role]     = "singleSigner_derivationPath";
    roles[single_signer_master_fingerprint_Role]  = "singleSigner_masterFingerPrint";
    roles[single_signer_master_signer_id_Role]    = "singleSigner_masterSignerId";
    roles[single_signer_last_health_check_Role]   = "singleSigner_lastHealthCheck";
    roles[single_signer_signed_status_Role]       = "single_signer_signed_status";
    roles[single_signer_type_Role]                = "single_signer_type";
    roles[single_signer_signed_isValid_Role]      = "single_signer_signed_isValid";
    roles[single_signer_signed_message_Role]      = "single_signer_signed_message";
    roles[single_signer_signed_signature_Role]    = "single_signer_signed_signature";
    roles[single_signer_health_Role]              = "single_signer_signed_health";
    roles[single_signer_isColdcard_Role]          = "single_signer_isColdcard";
    roles[single_signer_checked_Role]             = "single_signer_checked";
    roles[single_signer_readyToSign_Role]         = "single_signer_readyToSign";
    roles[single_signer_is_local_Role]            = "single_signer_is_local";
    roles[single_signer_primary_key_Role]         = "single_signer_primary_key";
    return roles;
}

void SingleSignerListModel::replaceSingleSigner(int index, const QSingleSignerPtr &value)
{
    if(index < 0 || index >= d_.count()){
        DBG_INFO << "Index out of range";
    }
    else {
        beginResetModel();
        d_.replace(index, value);
        endResetModel();
    }
}

void SingleSignerListModel::addSingleSigner(const QSingleSignerPtr &d)
{
    beginResetModel();
    if(d && !containsSigner(d.data()->masterFingerPrint(), d.data()->derivationPath())){
        nunchuk::PrimaryKey key = containPrimaryKey(d->masterFingerPrint());
        if(key.get_master_fingerprint() != ""){
            d->setPrimaryKey(key);
            d->setIsPrimaryKey(true);
        }
        d_.append(d);
    }
    endResetModel();
}

void SingleSignerListModel::initSignatures()
{
    beginResetModel();
    for (int var = 0; var < d_.count(); var++) {
        d_[var]->setSignerSigned(false);
    }
    endResetModel();
}


void SingleSignerListModel::updateSignatures(const QString &masterfingerprint, const bool value)
{
    for (int var = 0; var < d_.count(); var++) {
        if(0 == QString::compare(masterfingerprint, d_.at(var).data()->masterFingerPrint(), Qt::CaseInsensitive)){
            d_[var]->setSignerSigned(value);
        }
    }
}

bool SingleSignerListModel::needTopUpXpubs() const
{
    foreach (QSingleSignerPtr i , d_ ){
        if(((int)ENUNCHUCK::SignerType::AIRGAP != i.data()->signerType()) && (!i.data()->isValid())){
            return true;
        }
    }
    return false;
}

void SingleSignerListModel::initIsColdCard()
{
    beginResetModel();
    for (int var = 0; var < d_.count(); var++) {
        d_[var]->setIsColdCard(false);
    }
    endResetModel();
}

void SingleSignerListModel::setIsColdCard(const QString &id)
{
    beginResetModel();
    if(id != ""){
        foreach (QSingleSignerPtr i , d_ ){
            if(i.data()->masterSignerId() == id){
                i.data()->setIsColdCard(true);
            }
        }
    }
    endResetModel();
}

QSingleSignerPtr SingleSignerListModel::getSingleSignerByIndex(const int index) {
    if(index < 0 || index >= d_.count()){
        DBG_INFO << "Index out of range";
        QSingleSignerPtr fail(NULL);
        return fail;
    }
    else {
        return d_.at(index);
    }
}

bool SingleSignerListModel::containsMasterSignerId(const QString &masterSignerId) {
    foreach (QSingleSignerPtr i , d_ ){
        if(0 == QString::compare(masterSignerId, i.data()->masterSignerId(), Qt::CaseInsensitive)){
            return true;
        }
    }
    return false;
}

bool SingleSignerListModel::removeSingleSignerByIndex(const int index)
{
    beginResetModel();
    d_.removeAt(index);
    endResetModel();
    return false;
}

bool SingleSignerListModel::removeSingleSignerByFingerprint(const QString &fp)
{
    bool ret = false;
    beginResetModel();
    foreach (QSingleSignerPtr it, d_) {
        if(0 == QString::compare(fp, it.data()->masterFingerPrint(), Qt::CaseInsensitive)){
            d_.removeAll(it);
            ret = true;
            break;
        }
    }
    endResetModel();
    return ret;
}

QString SingleSignerListModel::getMasterSignerIdByIndex(const int index)
{
    QString ret = "";
    if(index < 0 || index >= d_.count()){
        DBG_INFO << "Index out of range";
    }
    else {
        ret = d_.at(index)->masterSignerId();
    }

    return ret;
}

QString SingleSignerListModel::getMasterSignerXfpByIndex(const int index)
{
    QString ret = "";
    if(index < 0 || index >= d_.count()){
        DBG_INFO << "Index out of range";
    }
    else {
        ret = d_.at(index)->masterFingerPrint();
    }
    return ret;
}

QSingleSignerPtr SingleSignerListModel::getSingleSignerByFingerPrint(const QString &fp) {

    foreach (QSingleSignerPtr i , d_ ){
        if(0 == QString::compare(fp, i.data()->masterFingerPrint(), Qt::CaseInsensitive)){
            return i;
        }
    }
    return NULL;
}

int SingleSignerListModel::getnumberSigned() {
    int ret = 0;
    foreach (QSingleSignerPtr i , d_ ){
        if(true == i.data()->signerSigned()){
            ret++;
        }
    }
    return ret;
}

bool SingleSignerListModel::containsSigner(const QString &xfp, const QString &path)
{
    foreach (QSingleSignerPtr i , d_ ){
        if(0 == QString::compare(path, i.data()->derivationPath(), Qt::CaseInsensitive)
                && 0 == QString::compare(xfp, i.data()->masterFingerPrint(), Qt::CaseInsensitive))
        {
            return true;
        }
    }
    return false;
}

bool SingleSignerListModel::contains(const QString &masterFingerPrint)
{
    foreach (QSingleSignerPtr i , d_ ){
        if(0 == QString::compare(masterFingerPrint, i.data()->masterFingerPrint(), Qt::CaseInsensitive)){
            return true;
        }
    }
    return false;
}

bool SingleSignerListModel::checkUsableToSign(const QString &masterFingerPrint)
{
    bool isContains = contains(masterFingerPrint);
    if(isContains){
        foreach (QSingleSignerPtr i , d_ ){
            if(0 == QString::compare(masterFingerPrint, i.data()->masterFingerPrint(), Qt::CaseInsensitive)) {
                if(!i.data()->signerSigned()){
                    return true;
                }
            }
        }
    }
    return false;
}

void SingleSignerListModel::updateSignerHealthStatus(const QString &masterSignerId, const int status, const time_t time)
{
    beginResetModel();
    foreach (QSingleSignerPtr i , d_ ){
        if(i.data() && masterSignerId == i.data()->masterSignerId()){
            i.data()->setHealth(status);
            i.data()->setlastHealthCheck(QDateTime::fromTime_t(time));
        }
    }
    endResetModel();
}

void SingleSignerListModel::notifyMasterSignerDeleted(const QString &masterSignerId)
{
    beginResetModel();
    foreach (QSingleSignerPtr i , d_ ){
        if(i.data() && masterSignerId == i.data()->masterSignerId()){
            i.data()->setSignerType((int)ENUNCHUCK::SignerType::AIRGAP);
        }
    }
    endResetModel();
}

void SingleSignerListModel::notifyMasterSignerRenamed(const QString &masterSignerId, const QString &newname)
{
    beginResetModel();
    foreach (QSingleSignerPtr i , d_ ){
        if(i.data() && masterSignerId == i.data()->masterSignerId()){
            i.data()->setName(newname);
        }
    }
    endResetModel();
}

void SingleSignerListModel::notifyRemoteSignerRenamed(const QString &fingerprint, const QString &newname)
{
    beginResetModel();
    foreach (QSingleSignerPtr i , d_ ){
        if(0 == QString::compare(fingerprint, i.data()->masterFingerPrint(), Qt::CaseInsensitive)){
            i.data()->setName(newname);
        }
    }
    endResetModel();
}

void SingleSignerListModel::notifyRemoteLocalSigner(const QString &fingerprint)
{
    beginResetModel();
    foreach (QSingleSignerPtr i , d_ ){
        if(0 == QString::compare(fingerprint, i.data()->masterFingerPrint(), Qt::CaseInsensitive)){
            i.data()->setIsLocalSigner(false);
        }
    }
    requestSort(SingleSignerListModel::SingleSignerRoles::single_signer_is_local_Role, Qt::AscendingOrder);
    endResetModel();
}

void SingleSignerListModel::resetUserChecked()
{
    beginResetModel();
    foreach (QSingleSignerPtr it, d_) {
        it.data()->setChecked(false);
    }
    endResetModel();
}

void SingleSignerListModel::setUserCheckedByFingerprint(const bool state, const QString fp)
{
    beginResetModel();
    foreach (QSingleSignerPtr it, d_) {
        if(0 == QString::compare(fp, it.data()->masterFingerPrint(), Qt::CaseInsensitive)){
            it.data()->setChecked(state);
        }
    }
    endResetModel();
}

void SingleSignerListModel::setUserChecked(const bool state, const int index)
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

void SingleSignerListModel::updateHealthCheckTime()
{
    beginResetModel();
    foreach (QSingleSignerPtr it, d_) {
        it.data()->lastHealthCheckChanged();
    }
    endResetModel();
}

bool SingleSignerListModel::containsDevicesConnected(const QStringList xfp)
{
    for (int i = 0; i < xfp.count(); i++) {
        foreach (QSingleSignerPtr it, d_) {
            if(((int)ENUNCHUCK::SignerType::HARDWARE == it.data()->signerType()) && (0 == QString::compare(xfp.at(i), it.data()->masterFingerPrint(), Qt::CaseInsensitive))){
                return true;
            }
        }
    }
    return false;
}

void SingleSignerListModel::updateSignerReadyToSign(const QString &xfp)
{
    for (int i = 0; i < d_.count(); i++) {
        if(0 == QString::compare(xfp, d_.at(i)->masterFingerPrint(), Qt::CaseInsensitive)){
            d_[i]->setReadyToSign(true);
            emit dataChanged(index(i),index(i));
        }
    }
#if 0
    beginResetModel();
    foreach (QSingleSignerPtr it, d_) {
        if(((int)ENUNCHUCK::SignerType::AIRGAP == it.data()->signerType()) || ((int)ENUNCHUCK::SignerType::SOFTWARE == it.data()->signerType())){
            it.data()->setReadyToSign(true);
        }
        else{
            if(0 == QString::compare(xfp, it.data()->masterFingerPrint(), Qt::CaseInsensitive)){
                it.data()->setReadyToSign(true);
            }
            else{
                it.data()->setReadyToSign(false);
            }
        }
    }
    endResetModel();
#endif
}

void SingleSignerListModel::updateSignerIsLocalAndReadyToSign(const QString &xfp,int signerType)
{
    for (int i = 0; i < d_.count(); i++) {
        if(0 == QString::compare(xfp, d_.at(i)->masterFingerPrint(), Qt::CaseInsensitive)){
            d_[i]->setIsLocalSigner(true);
            d_[i]->setReadyToSign(true);
            d_[i]->setSignerType(signerType);
            emit dataChanged(index(i),index(i));
        }
    }
}

void SingleSignerListModel::resetSignerReadyToSign()
{
    beginResetModel();
    foreach (QSingleSignerPtr it, d_) {
        if(((int)ENUNCHUCK::SignerType::AIRGAP == it.data()->signerType())
                || ((int)ENUNCHUCK::SignerType::SOFTWARE == it.data()->signerType())
                || it.data()->signerSigned()){
            it.data()->setReadyToSign(true);
        }
        else{
            it.data()->setReadyToSign(false);
        }
    }
    endResetModel();
}

nunchuk::PrimaryKey SingleSignerListModel::containPrimaryKey(const QString &fingerprint)
{
    for(nunchuk::PrimaryKey key: primaryKeys){
        if(key.get_master_fingerprint() == fingerprint.toStdString()){
            return key;
        }
    }
    return nunchuk::PrimaryKey();
}

void SingleSignerListModel::requestSort(int role, int order)
{
    beginResetModel();
    if(d_.count() > 1){
        switch (role) {
        case single_signer_name_Role:
        {
            if(Qt::DescendingOrder == order){
                qSort(d_.begin(), d_.end(), sortSingleSignerByNameDescending);
            }
            else{
                qSort(d_.begin(), d_.end(), sortSingleSignerByNameAscending);
            }
        }
            break;
        case single_signer_is_local_Role:
        {
            qSort(d_.begin(), d_.end(), sortSingleSignerByLocalAscending);
        }
            break;
        default:
            break;
        }
    }
    endResetModel();
}

QList<QSingleSignerPtr > SingleSignerListModel::fullList() const
{
    return d_;
}

QSharedPointer<SingleSignerListModel> SingleSignerListModel::clone() const
{
    QSharedPointer<SingleSignerListModel> clone = QSharedPointer<SingleSignerListModel>(new SingleSignerListModel());
    for (QSingleSignerPtr signer : d_) {
        QSingleSignerPtr ret = QSingleSignerPtr(new SingleSigner());
        ret.data()->setName(signer.data()->name());
        ret.data()->setXpub(signer.data()->xpub());
        ret.data()->setPublickey(signer.data()->publickey());
        ret.data()->setDerivationPath(signer.data()->derivationPath());
        ret.data()->setMasterFingerPrint(signer.data()->masterFingerPrint());
        ret.data()->setMasterSignerId(signer.data()->masterSignerId());
        ret.data()->setlastHealthCheck(signer.data()->lastHealthCheckDateTime());
        ret.data()->setSignerType(signer.data()->signerType());
        ret.data()->setIsColdCard(signer.data()->isColdCard());
        ret.data()->setIsLocalSigner(signer.data()->isLocalSigner());
        ret.data()->setMessage(signer.data()->message());
        ret.data()->setSignature(signer.data()->signature());
        ret.data()->setHealth(signer.data()->health());
        ret.data()->setIsPrimaryKey(signer.data()->isPrimaryKey());
        //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        clone.data()->addSingleSigner(ret);
    }
    return clone;
}

void SingleSignerListModel::cleardata()
{
    beginResetModel();
    d_.clear();
    endResetModel();
}

int SingleSignerListModel::signerCount() const
{
    return d_.count();
}

int SingleSignerListModel::signerSelectedCount() const
{
    int ret = 0;
    foreach (QSingleSignerPtr it, d_) {
        if(it.data()->checked()) { ret++;}
    }
    return ret;
}

bool sortSingleSignerByNameAscending(const QSingleSignerPtr &v1, const QSingleSignerPtr &v2)
{
    return (QString::compare((v1.data()->name()), (v2.data()->name())) < 0);
}

bool sortSingleSignerByNameDescending(const QSingleSignerPtr &v1, const QSingleSignerPtr &v2)
{
    return (QString::compare((v1.data()->name()), (v2.data()->name())) > 0);
}

bool sortSingleSignerByLocalAscending(const QSingleSignerPtr &v1, const QSingleSignerPtr &v2)
{
    return (v1.data()->isLocalSigner() > v2.data()->isLocalSigner());
}
