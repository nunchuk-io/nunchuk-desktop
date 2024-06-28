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
#include "AppModel.h"
#include "qUtils.h"
#include "bridgeifaces.h"
#include <QQmlEngine>
#include "utils/enumconverter.hpp"
#include "Servers/Draco.h"
#include "AppModel.h"
#include "ViewsEnums.h"

QSingleSigner::QSingleSigner()
    : isPrimaryKey_(false)
    , isDraft(true)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

QSingleSigner::QSingleSigner(const nunchuk::SingleSigner& singleKey):
    singleSigner_(singleKey),
    isPrimaryKey_(false),
    isDraft(false)
{
    nunchuk::PrimaryKey key = AppModel::instance()->findPrimaryKey(QString::fromStdString(singleKey.get_master_fingerprint()));
    if(key.get_master_fingerprint() != ""){
        isPrimaryKey_ = true;
        primaryKey_ = key;
    }
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

QSingleSigner::~QSingleSigner() {}

void QSingleSigner::convert(const nunchuk::SingleSigner &src)
{
    isDraft = false;
    singleSigner_ = src;
}

nunchuk::SingleSigner QSingleSigner::originSingleSigner() const
{
    return singleSigner_;
}

void QSingleSigner::setOriginSingleSigner(const nunchuk::SingleSigner signer)
{
    singleSigner_ = signer;
}

nunchuk::PrimaryKey QSingleSigner::originPrimaryKey() const
{
    return primaryKey_;
}

void QSingleSigner::setOriginPrimaryKey(const nunchuk::PrimaryKey key)
{
    primaryKey_ = key;
    setIsPrimaryKey(true);
}

QString QSingleSigner::name() {
    if(email() != ""){
        return email();
    }
    else{
        return QString::fromStdString(singleSigner_.get_name());
    }
}

void QSingleSigner::setName(const QString &d) {
    singleSigner_.set_name(d.toStdString());
    emit nameChanged();
}

QString QSingleSigner::xpub() {
    DBG_INFO << singleSigner_.get_xpub();
    if(isDraft){
        return xpub_;
    }
    else{
        return QString::fromStdString(singleSigner_.get_xpub());
    }
}

void QSingleSigner::setXpub(const QString &d) {
    if(isDraft && d != xpub_){
        xpub_ = d;
        emit xpubChanged();
    }
}

QString QSingleSigner::publickey() {
    if(isDraft){
        return public_key_;
    }
    else{
        return QString::fromStdString(singleSigner_.get_public_key());
    }
}

void QSingleSigner::setPublickey(const QString &d) {
    if(isDraft && d != public_key_){
        public_key_ = d;
        emit publickeyChanged();
    }
}

QString QSingleSigner::derivationPath() {
    if(isDraft){
        return derivation_path_;
    }
    else {
        return QString::fromStdString(singleSigner_.get_derivation_path());
    }
}

void QSingleSigner::setDerivationPath(const QString &d) {
    if(isDraft && d != derivation_path_){
        derivation_path_ = d;
        emit derivationPathChanged();
    }
}

QString QSingleSigner::fingerPrint() const
{
    return masterFingerPrint();
}

QString QSingleSigner::masterFingerPrint() const {
    if(isDraft){
        return  master_fingerprint_;
    }
    else{
        return QString::fromStdString(singleSigner_.get_master_fingerprint());
    }
}

void QSingleSigner::setMasterFingerPrint(const QString &d) {
    if(isDraft && d != master_fingerprint_){
        master_fingerprint_ = d;
        emit masterFingerPrintChanged();
    }
}

QString QSingleSigner::masterSignerId() {
    if(isDraft){
        return master_signer_id_;
    }
    else{
        return QString::fromStdString(singleSigner_.get_master_signer_id());
    }
}

void QSingleSigner::setMasterSignerId(const QString &d)
{
    if(isDraft && d != master_signer_id_){
        master_signer_id_ = d;
        emit masterSignerIdChanged();
    }
}

QString QSingleSigner::lastHealthCheck() {
    time_t time = singleSigner_.get_last_health_check();
    if(0 == time){
        return "N/A"; // There is no time
    }
    return this->timeGapCalculationShort(QDateTime::fromTime_t(time)); // last_health_check_.toString( "MM/dd/yyyy hh:mm AP" );
}

QDateTime QSingleSigner::lastHealthCheckDateTime() const {
    return QDateTime::fromTime_t(singleSigner_.get_last_health_check());
}

bool QSingleSigner::signerSigned() const
{
    return signed_;
}

void QSingleSigner::setSignerSigned(const bool d)
{
    if(d != signed_){
        signed_ = d;
        emit signerSignedChanged();
    }
}

bool QSingleSigner::needTopUpXpub()
{
    if((int)ENUNCHUCK::SignerType::HARDWARE == signerType() || (int)ENUNCHUCK::SignerType::SOFTWARE == signerType()){
        return needTopUpXpub_;
    }
    else{
        return false;
    }
}

void QSingleSigner::setNeedTopUpXpub(bool needTopUpXpub)
{
    if(needTopUpXpub_ != needTopUpXpub){
        needTopUpXpub_ = needTopUpXpub;
        emit needTopUpXpubChanged();
    }
}

QString QSingleSigner::message() const
{
    return message_;
}

void QSingleSigner::setMessage(const QString &message)
{
    if(message_ != message){
        message_ = message;
        emit messageChanged();
    }
}

QString QSingleSigner::signature() const
{
    return signature_;
}

void QSingleSigner::setSignature(const QString &signature)
{
    if(signature_ != signature){
        signature_ = signature;
        emit signatureChanged();
    }
}

int QSingleSigner::health() const
{
    return health_;
}

void QSingleSigner::setHealth(int health)
{
    if(health_ != health){
        health_ = health;
        emit healthChanged();
    }
}

bool QSingleSigner::checked() const
{
    return checked_;
}

void QSingleSigner::setChecked(const bool checked)
{
    if(checked != checked_){
        checked_ = checked;
        emit checkedChanged();
    }
}

bool QSingleSigner::readyToSign() const
{
    bool signAble = false;
    if((int)ENUNCHUCK::SignerType::HARDWARE == signerType() || (int)ENUNCHUCK::SignerType::COLDCARD_NFC == signerType()){
        if(auto devices = AppModel::instance()->deviceListPtr()) {
            return devices->contains(masterFingerPrint());
        }
        else{
            return false;
        }
    }
    else if((int)ENUNCHUCK::SignerType::SOFTWARE == signerType()){
        signAble = true;
    }
    else{
        signAble = false;
    }
    return signAble;
}

int QSingleSigner::signerType() const
{
    return (int)singleSigner_.get_type();
}

void QSingleSigner::setSignerType(int signer_type)
{
    singleSigner_.set_type((nunchuk::SignerType)signer_type);
    emit signerTypeChanged();
}

std::vector<nunchuk::SignerTag> QSingleSigner::signerTags() const
{
    return singleSigner_.get_tags();
}

void QSingleSigner::setSignerTags(const std::vector<nunchuk::SignerTag> signer_tags)
{
    singleSigner_.set_tags(signer_tags);
}

bool QSingleSigner::isColdCard()
{
    DBG_INFO << tag();
    bool ret = false;
    if((int)ENUNCHUCK::SignerType::COLDCARD_NFC == signerType()){
        ret = true;
    }
    else if((int)ENUNCHUCK::SignerType::HARDWARE == signerType()){
        if(tag().contains("COLDCARD")){
            ret = true;
        }
        else{
            if(AppModel::instance()->masterSignerList()){
                ret = AppModel::instance()->masterSignerList()->isColdCard(masterFingerPrint());
            }
        }
    }
    else{ }
    return ret;
}

bool QSingleSigner::isLocalSigner()
{
    bool isLocal = false;
    if(signerType() != (int)ENUNCHUCK::SignerType::SERVER){
        if(AppModel::instance()->masterSignerList()){
            isLocal = AppModel::instance()->masterSignerList()->containsFingerPrint(masterFingerPrint());
        }
        if(!isLocal && AppModel::instance()->remoteSignerList()){
            isLocal = AppModel::instance()->remoteSignerList()->containsFingerPrint(masterFingerPrint());
        }
    }
    return isLocal;
}

bool QSingleSigner::isMine() const
{
    QWarningMessage msg;
    std::vector<nunchuk::SingleSigner> signers = bridge::nunchukGetOriginRemoteSigners(msg);
    auto it = std::find_if(signers.begin(), signers.end(), [this](const nunchuk::SingleSigner& signer) {
        return signer.get_master_fingerprint() == masterFingerPrint().toStdString();
    });
    return it != signers.end();
}

bool QSingleSigner::isPrimaryKey() const
{
    return isPrimaryKey_;
}

void QSingleSigner::setIsPrimaryKey(bool isPrimaryKey)
{
    isPrimaryKey_ = isPrimaryKey;
    emit isPrimaryKeyChanged();
}

QString QSingleSigner::devicetype() const
{
    return m_devicetype;
}

void QSingleSigner::setDevicetype(QString devicetype)
{
    if (m_devicetype == devicetype)
        return;

    m_devicetype = devicetype;
    emit devicetypeChanged();
}

QString QSingleSigner::cardId()
{
    if((int)ENUNCHUCK::SignerType::NFC == signerType()){
        if (cardId_.isEmpty()) {
            nunchuk::TapsignerStatus tapsigner = bridge::GetTapsignerStatusFromMasterSigner(masterFingerPrint());
            cardId_ = QString::fromStdString(tapsigner.get_card_ident());
        }
    }
    return cardId_;
}

void QSingleSigner::setCardId(const QString &card_id)
{
    cardId_ = card_id;
}

nunchuk::SingleSigner QSingleSigner::singleSigner() const
{
    return singleSigner_;
}

QString QSingleSigner::email() const
{
    return m_email;
}

void QSingleSigner::setEmail(const QString &email)
{
    if(m_email != email){
        m_email = email;
        emit nameChanged();
    }
}

QString QSingleSigner::tag() const
{
    if (singleSigner_.get_tags().size() > 0) {
        nunchuk::SignerTag tag = singleSigner_.get_tags().front();
        return QString::fromStdString(SignerTagToStr(tag));
    }
    return "";
}

QStringList QSingleSigner::tags() const
{
    QStringList list;
    for (auto tag : singleSigner_.get_tags()) {
        list.append(QString::fromStdString(SignerTagToStr(tag)));
    }
    return list;
}

bool QSingleSigner::hasSignBtn() const
{
    return m_hasSignBtn;
}

void QSingleSigner::setHasSignBtn(bool hasSignBtn)
{
    m_hasSignBtn = hasSignBtn;
}

int QSingleSigner::accountIndex()
{
    return qUtils::GetIndexFromPath(derivationPath());
}

QString QSingleSigner::address() const
{
    return m_address;
}

void QSingleSigner::setAddress(const QString &address)
{
    if (m_address == address)
        return;
    m_address = address;
    emit addressChanged();
}

QString QSingleSigner::descriptor() const
{
    DBG_INFO << singleSigner_.get_descriptor();
    DBG_INFO << singleSigner_.get_xpub();
    DBG_INFO << singleSigner_.get_public_key();
    return QString::fromStdString(singleSigner_.get_descriptor());
}

QString QSingleSigner::timeGapCalculationShort(QDateTime in)
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

QString QSingleSigner::timeGapCalculation(QDateTime in)
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

QVariantList QSingleSigner::getWalletList()
{
    QVariantList ret;
    for (auto wallet : AppModel::instance()->walletListPtr()->fullList()) {
        if (wallet->isContainKey(masterFingerPrint())) {
            ret.append(QVariant::fromValue(wallet.data()));
        }
    }
    return ret;
}

SingleSignerListModel::SingleSignerListModel(){
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
    case single_signer_need_Topup_Xpub_Role:
        return d_[index.row()]->needTopUpXpub();
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
    case single_signer_devicetype_Role:
        return d_[index.row()]->devicetype();
    case single_signer_device_cardid_Role:
        return d_[index.row()]->cardId();
    case single_signer_tag_Role:
        return d_[index.row()]->tag();
    case single_signer_has_sign_btn_Role:
        return d_[index.row()]->hasSignBtn();
    case single_signer_account_index_Role:
        return d_[index.row()]->accountIndex();
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
        DBG_INFO;
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
    roles[single_signer_need_Topup_Xpub_Role]     = "single_signer_need_Topup_Xpub";
    roles[single_signer_signed_message_Role]      = "single_signer_signed_message";
    roles[single_signer_signed_signature_Role]    = "single_signer_signed_signature";
    roles[single_signer_health_Role]              = "single_signer_signed_health";
    roles[single_signer_isColdcard_Role]          = "single_signer_isColdcard";
    roles[single_signer_checked_Role]             = "single_signer_checked";
    roles[single_signer_readyToSign_Role]         = "single_signer_readyToSign";
    roles[single_signer_is_local_Role]            = "single_signer_is_local";
    roles[single_signer_primary_key_Role]         = "single_signer_primary_key";
    roles[single_signer_devicetype_Role]          = "single_signer_devicetype";
    roles[single_signer_device_cardid_Role]       = "single_signer_device_cardid";
    roles[single_signer_tag_Role]                 = "single_signer_tag";
    roles[single_signer_has_sign_btn_Role]        = "single_signer_has_sign_btn";
    roles[single_signer_account_index_Role]       = "single_signer_account_index";
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
        nunchuk::PrimaryKey key = AppModel::instance()->findPrimaryKey(d->masterFingerPrint());
        if(key.get_master_fingerprint() != ""){
            d->setOriginPrimaryKey(key);
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


void SingleSignerListModel::updateSignatures(const QString &masterfingerprint, const bool value, const QString& signature)
{
    for (int var = 0; var < d_.count(); var++) {
        if(0 == QString::compare(masterfingerprint, d_.at(var).data()->masterFingerPrint(), Qt::CaseInsensitive)){
            d_[var]->setSignerSigned(value);
            d_[var]->setSignature(signature);
        }
    }
}

bool SingleSignerListModel::needTopUpXpubs() const
{
    foreach (QSingleSignerPtr i , d_ ){
        if(i.data()->needTopUpXpub()){
            return true;
        }
    }
    return false;
}

QSingleSignerPtr SingleSignerListModel::getSingleSignerByIndex(const int index) {
    if(index < 0 || index >= d_.count()){
        DBG_INFO << "Index out of range";
        return NULL;
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

QSingleSignerPtr SingleSignerListModel::getSingleSignerByFingerPrint(const QString &xfp) {

    foreach (QSingleSignerPtr i , d_ ){
        if(0 == QString::compare(xfp, i.data()->masterFingerPrint(), Qt::CaseInsensitive)){
            return i;
        }
    }
    return NULL;
}

int SingleSignerListModel::getIndexByFingerPrint(const QString &fingerprint)
{
    for (int i = 0; i < d_.count(); i++) {
        if(0 == QString::compare(fingerprint, d_[i].data()->masterFingerPrint(), Qt::CaseInsensitive)){
            return i;
        }
    }
    return -1;
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

bool SingleSignerListModel::containsHardwareKey()
{
    foreach (QSingleSignerPtr i , d_ ){
        if(i.data()->signerType() == (int)ENUNCHUCK::SignerType::HARDWARE) {
            return true;
        }
    }
    return false;
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

bool SingleSignerListModel::containsColdcard()
{
    foreach (QSingleSignerPtr i , d_ ){
        if(i.data()->isColdCard()){
            return true;
        }
    }
    return false;
}

bool SingleSignerListModel::containsFingerPrint(const QString &masterFingerPrint)
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
    bool isContains = containsFingerPrint(masterFingerPrint);
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

void SingleSignerListModel::updateSignerHealthStatus(const QString &xfp, const int status, const time_t time)
{
    beginResetModel();
    foreach (QSingleSignerPtr i , d_ ){
        if(i.data() && qUtils::strCompare(xfp, i.data()->masterFingerPrint())){
            i.data()->setHealth(status);
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

void SingleSignerListModel::updateSignerOfRoomWallet(const SignerAssigned &signer)
{
    beginResetModel();
    foreach (QSingleSignerPtr i , d_ ){
        if(!i.data()->isLocalSigner() && 0 == QString::compare(signer.xfp, i.data()->masterFingerPrint(), Qt::CaseInsensitive)){
            i.data()->setName(signer.name);
            i.data()->setSignerType(signer.type);
        }
    }
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

void SingleSignerListModel::syncNunchukEmail(QList<DracoUser> users)
{
    for (int i = 0; i < d_.count(); i++) {
        if(d_.at(i) && !d_.at(i).data()->isLocalSigner()){
            foreach (DracoUser user, users) {
                if(0 == QString::compare(d_.at(i).data()->name(), user.chat_id, Qt::CaseInsensitive)){
                    d_.at(i).data()->setEmail(user.email);
                    emit dataChanged(index(i), index(i) );
                }
            }
        }
    }
}

bool SingleSignerListModel::needSyncNunchukEmail()
{
    bool ret = false;
    for (QSingleSignerPtr p : d_) {
        if(!p.data()->isLocalSigner() && p.data()->email() == ""){
            return true;
        }
    }
    return ret;
}

void SingleSignerListModel::requestSort()
{
    beginResetModel();
    if(d_.count() > 1){
        qSort(d_.begin(), d_.end(), sortSingleSignerByNameAscending);
    }
    endResetModel();
}

QList<QSingleSignerPtr > SingleSignerListModel::fullList() const
{
    return d_;
}

std::vector<nunchuk::SingleSigner> SingleSignerListModel::signers() const
{
    std::vector<nunchuk::SingleSigner> signerList;
    for (QSingleSignerPtr p : d_) {
        signerList.push_back(p->originSingleSigner());
    }
    return signerList;
}

QSharedPointer<SingleSignerListModel> SingleSignerListModel::clone() const
{
    QSharedPointer<SingleSignerListModel> clone = QSharedPointer<SingleSignerListModel>(new SingleSignerListModel());
    for (QSingleSignerPtr signer : d_) {
        QSingleSignerPtr ret = QSingleSignerPtr(new QSingleSigner(signer.data()->originSingleSigner()));
        if(ret){
            ret.data()->setEmail(signer.data()->email());
            clone.data()->addSingleSigner(ret);
        }
    }
    return clone;
}

void SingleSignerListModel::cleardata()
{
    beginResetModel();
    d_.clear();
    endResetModel();
}

QStringList SingleSignerListModel::getKeyNames()
{
    QStringList ret;
    foreach (QSingleSignerPtr it, d_) {
        if(it) {
            ret << it.data()->name();
        }
    }
    return ret;
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
    if (!v1->isLocalSigner() || v1->signerType() == (int)ENUNCHUCK::SignerType::SERVER) {
        return false;
    }
    else if (!v2->isLocalSigner() || v2->signerType() == (int)ENUNCHUCK::SignerType::SERVER) {
        return true;
    }
    else {
        return (QString::compare((v1.data()->name()), (v2.data()->name())) < 0);
    }
}
