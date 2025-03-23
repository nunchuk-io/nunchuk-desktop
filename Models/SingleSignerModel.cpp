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
    QStringList list = tags();
    list.removeOne("INHERITANCE");
    return list.isEmpty() ? "" : list.first();
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

bool QSingleSigner::isReplaced() const
{
    return m_isReplaced;
}

void QSingleSigner::setIsReplaced(bool newIsReplaced)
{
    m_isReplaced = newIsReplaced;
}

QSingleSignerPtr QSingleSigner::keyReplaced() const
{
    return m_keyReplaced;
}

void QSingleSigner::setKeyReplaced(const QSingleSignerPtr &keyReplaced)
{
    m_keyReplaced = keyReplaced;
}

bool QSingleSigner::taprootSupported()
{
    bool ret = true;
    QSet<int> types = Draco::instance()->GetTaprootSupportedCached();
    if(types.size() > 0){
        ret = types.contains(signerType());
    }
    return ret;
}

int QSingleSigner::keysetIndex() const
{
    return m_keyset_index;
}

void QSingleSigner::setKeysetIndex(const int index)
{
    if(m_keyset_index != index){
        m_keyset_index = index;
        emit keysetIndexChanged();
    }
}

int QSingleSigner::keysetStatus() const
{
    return m_keyset_status;
}

void QSingleSigner::setKeysetStatus(const int status)
{
    if(m_keyset_status != status){
        m_keyset_status = status;
        emit keysetStatusChanged();
    }
}

int QSingleSigner::keysetPendingNumber() const
{
    return m_keyset_pendingnumber;
}

void QSingleSigner::setKeysetPendingNumber(const int number)
{
    if(m_keyset_pendingnumber != number){
        m_keyset_pendingnumber = number;
        emit keysetPendingNumberChanged();
    }
}

bool QSingleSigner::valueKey() const
{
    return m_valuekey;
}

void QSingleSigner::setValueKey(const bool data)
{
    if(m_valuekey != data){
        m_valuekey = data;
        emit valueKeyChanged();
    }
}

bool QSingleSigner::isValid()
{
    return !masterFingerPrint().isEmpty();
}

SingleSignerListModel::SingleSignerListModel(){
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

SingleSignerListModel::~SingleSignerListModel(){m_data.clear();}

int SingleSignerListModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return m_data.count();
}

QVariant SingleSignerListModel::data(const QModelIndex &index, int role) const
{
    switch (role) {
    case single_signer_name_Role:
        return m_data[index.row()]->name();
    case single_signer_xpub_Role:
        return m_data[index.row()]->xpub();
    case single_signer_public_key_Role:
        return m_data[index.row()]->publickey();
    case single_signer_derivation_path_Role:
        return m_data[index.row()]->derivationPath();
    case single_signer_master_fingerprint_Role:
        return m_data[index.row()]->masterFingerPrint();
    case single_signer_master_signer_id_Role:
        return m_data[index.row()]->masterSignerId();
    case single_signer_last_health_check_Role:
        return m_data[index.row()]->lastHealthCheck();
    case single_signer_signed_status_Role:
        return m_data[index.row()]->signerSigned();
    case single_signer_type_Role:
        return m_data[index.row()]->signerType();
    case single_signer_need_Topup_Xpub_Role:
        return m_data[index.row()]->needTopUpXpub();
    case single_signer_signed_message_Role:
        return m_data[index.row()]->message();
    case single_signer_signed_signature_Role:
        return m_data[index.row()]->signature();
    case single_signer_health_Role:
        return m_data[index.row()]->health();
    case single_signer_isColdcard_Role:
        return m_data[index.row()]->isColdCard();
    case single_signer_checked_Role:
        return m_data[index.row()]->checked();
    case single_signer_readyToSign_Role:
        return m_data[index.row()]->readyToSign();
    case single_signer_is_local_Role:
        return m_data[index.row()]->isLocalSigner();
    case single_signer_primary_key_Role:
        return m_data[index.row()]->isPrimaryKey();
    case single_signer_devicetype_Role:
        return m_data[index.row()]->devicetype();
    case single_signer_device_cardid_Role:
        return m_data[index.row()]->cardId();
    case single_signer_tag_Role:
        return m_data[index.row()]->tag();
    case single_signer_has_sign_btn_Role:
        return m_data[index.row()]->hasSignBtn();
    case single_signer_account_index_Role:
        return m_data[index.row()]->accountIndex();
    case single_signer_isReplaced_Role:
        return m_data[index.row()]->isReplaced();
    case single_signer_keyReplaced_Role:
        return QVariant::fromValue(m_data[index.row()]->keyReplaced().data());
    case single_signer_taproot_supported:
        return m_data[index.row()]->taprootSupported();
    case single_signer_keyset_index:
        return m_data[index.row()]->keysetIndex();
    case single_signer_keyset_status:
        return m_data[index.row()]->keysetStatus();
    case single_signer_keyset_remaining:
        return m_data[index.row()]->keysetPendingNumber();
    case single_signer_value_key:
        return m_data[index.row()]->valueKey();
    default:
        return QVariant();
    }
}

bool SingleSignerListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role == single_signer_signed_message_Role) {
        m_data[index.row()]->setMessage(value.toString());
    }
    else if(role == single_signer_signed_signature_Role){
        m_data[index.row()]->setSignature(value.toString());
    }
    else if(role == single_signer_checked_Role){
        m_data[index.row()]->setChecked(value.toBool());
        emit signerSelectedCountChanged();
    }
    else if(role == single_signer_value_key){
        m_data[index.row()]->setValueKey(value.toBool());
        emit signerValueKeyCountChanged();
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
    roles[single_signer_isReplaced_Role]          = "single_signer_isReplaced";
    roles[single_signer_keyReplaced_Role]         = "single_signer_keyReplaced";
    roles[single_signer_taproot_supported]        = "single_signer_taproot_supported";
    roles[single_signer_keyset_index]             = "single_signer_keyset_index";
    roles[single_signer_keyset_status]            = "single_signer_keyset_status";
    roles[single_signer_keyset_remaining]         = "single_signer_keyset_remaining";
    roles[single_signer_value_key]                = "single_signer_value_key";
    return roles;
}

void SingleSignerListModel::replaceSingleSigner(int index, const QSingleSignerPtr &value)
{
    if(index < 0 || index >= m_data.count()){
        DBG_INFO << "Index out of range";
    }
    else {
        beginResetModel();
        m_data.replace(index, value);
        endResetModel();
    }
}

void SingleSignerListModel::addSingleSigner(const QSingleSignerPtr &d)
{
    if(d && !containsSigner(d.data()->masterFingerPrint(), d.data()->derivationPath())){
        nunchuk::PrimaryKey key = AppModel::instance()->findPrimaryKey(d->masterFingerPrint());
        if(key.get_master_fingerprint() != ""){
            d->setOriginPrimaryKey(key);
        }
        beginResetModel();
        m_data.append(d);
        endResetModel();
    }
}

void SingleSignerListModel::addKeysetSigner(const QSingleSignerPtr &signer, const int keyset_index)
{
    if(signer){
        QString xfp = signer.data()->masterFingerPrint();
        if(!containsKeyset(xfp, keyset_index)){
            beginResetModel();
            m_data.append(signer);
            endResetModel();
        }
    }
}

void SingleSignerListModel::updateKeysetPendingnumber(const int keyset_index, const int number)
{
    for (int i = 0; i < m_data.count(); i++) {
        if(keyset_index == m_data.at(i).data()->keysetIndex()){
            m_data.at(i).data()->setKeysetPendingNumber(number);
            emit dataChanged(index(i), index(i) );
        }
    }
}

void SingleSignerListModel::initSignatures()
{
    beginResetModel();
    for (int var = 0; var < m_data.count(); var++) {
        m_data[var]->setSignerSigned(false);
    }
    endResetModel();
}


void SingleSignerListModel::updateSignatures(const QString &masterfingerprint, const bool value, const QString& signature)
{
    for (int var = 0; var < m_data.count(); var++) {
        if(qUtils::strCompare(masterfingerprint, m_data.at(var).data()->masterFingerPrint())){
            m_data[var]->setSignerSigned(value);
            m_data[var]->setSignature(signature);
        }
    }
}

bool SingleSignerListModel::needTopUpXpubs() const
{
    foreach (QSingleSignerPtr i , m_data ){
        if(i.data()->needTopUpXpub()){
            return true;
        }
    }
    return false;
}

QSingleSignerPtr SingleSignerListModel::getSingleSignerByIndex(const int index) {
    if(index < 0 || index >= m_data.count()){
        DBG_INFO << "Index out of range";
        return NULL;
    }
    else {
        return m_data.at(index);
    }
}

bool SingleSignerListModel::containsMasterSignerId(const QString &masterSignerId) {
    foreach (QSingleSignerPtr i , m_data ){
        if(qUtils::strCompare(masterSignerId, i.data()->masterSignerId())){
            return true;
        }
    }
    return false;
}

bool SingleSignerListModel::removeSingleSignerByIndex(const int index)
{
    beginResetModel();
    m_data.removeAt(index);
    endResetModel();
    return false;
}

bool SingleSignerListModel::removeSingleSignerByType(const int type)
{
    beginResetModel();
    m_data.erase(std::remove_if(m_data.begin(), m_data.end(), [type](const auto& item) {
                                return item.data()->signerType() == type;
                            }), m_data.end());

    endResetModel();
    return false;
}

QString SingleSignerListModel::getMasterSignerIdByIndex(const int index)
{
    QString ret = "";
    if(index < 0 || index >= m_data.count()){
        DBG_INFO << "Index out of range";
    }
    else {
        ret = m_data.at(index)->masterSignerId();
    }

    return ret;
}

QString SingleSignerListModel::getMasterSignerXfpByIndex(const int index)
{
    QString ret = "";
    if(index < 0 || index >= m_data.count()){
        DBG_INFO << "Index out of range";
    }
    else {
        ret = m_data.at(index)->masterFingerPrint();
    }
    return ret;
}

QSingleSignerPtr SingleSignerListModel::getSingleSignerByFingerPrint(const QString &xfp) {

    foreach (QSingleSignerPtr i , m_data ){
        if(qUtils::strCompare(xfp, i.data()->masterFingerPrint())){
            return i;
        }
    }
    return NULL;
}

QSingleSignerPtr SingleSignerListModel::getSingleSignerByFingerPrint(const QString &xfp, const QString &name)
{
    foreach (QSingleSignerPtr i , m_data ){
        if(qUtils::strCompare(xfp, i.data()->masterFingerPrint()) && qUtils::strCompare(name, i.data()->name())){
            return i;
        }
    }
    return NULL;
}

int SingleSignerListModel::getIndexByFingerPrint(const QString &fingerprint)
{
    for (int i = 0; i < m_data.count(); i++) {
        if(qUtils::strCompare(fingerprint, m_data[i].data()->masterFingerPrint())){
            return i;
        }
    }
    return -1;
}

int SingleSignerListModel::getnumberSigned() {
    int ret = 0;
    foreach (QSingleSignerPtr i , m_data ){
        if(true == i.data()->signerSigned()){
            ret++;
        }
    }
    return ret;
}

bool SingleSignerListModel::containsHardwareKey()
{
    foreach (QSingleSignerPtr i , m_data ){
        if(i.data()->signerType() == (int)ENUNCHUCK::SignerType::HARDWARE) {
            return true;
        }
    }
    return false;
}

bool SingleSignerListModel::containsSigner(const QString &xfp, const QString &path)
{
    foreach (QSingleSignerPtr i , m_data ){
        if (path.isEmpty() && i.data()->derivationPath().isEmpty()
            && qUtils::strCompare(xfp, i.data()->masterFingerPrint()))
        {
            return true;
        } else if(qUtils::strCompare(path, i.data()->derivationPath())
                   && qUtils::strCompare(xfp, i.data()->masterFingerPrint()))
        {
            return true;
        }
    }
    return false;
}

bool SingleSignerListModel::containsColdcard()
{
    foreach (QSingleSignerPtr i , m_data ){
        if(i.data()->isColdCard()){
            return true;
        }
    }
    return false;
}

bool SingleSignerListModel::containsFingerPrint(const QString &masterFingerPrint)
{
    foreach (QSingleSignerPtr i , m_data ){
        if(qUtils::strCompare(masterFingerPrint, i.data()->masterFingerPrint())){
            return true;
        }
    }
    return false;
}

bool SingleSignerListModel::containsKeyset(const QString &xfp, const int keyset_index)
{
    foreach (QSingleSignerPtr i , m_data ){
        if((i.data()->keysetIndex() == keyset_index) && qUtils::strCompare(xfp, i.data()->masterFingerPrint())){
            return true;
        }
    }
    return false;
}

bool SingleSignerListModel::checkUsableToSign(const QString &masterFingerPrint)
{
    bool isContains = containsFingerPrint(masterFingerPrint);
    if(isContains){
        foreach (QSingleSignerPtr i , m_data ){
            if(qUtils::strCompare(masterFingerPrint, i.data()->masterFingerPrint())) {
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
    foreach (QSingleSignerPtr i , m_data ){
        if(i.data() && qUtils::strCompare(xfp, i.data()->masterFingerPrint())){
            i.data()->setHealth(status);
        }
    }
    endResetModel();
}

void SingleSignerListModel::notifyMasterSignerDeleted(const QString &masterSignerId)
{
    beginResetModel();
    foreach (QSingleSignerPtr i , m_data ){
        if(i.data() && masterSignerId == i.data()->masterSignerId()){
            i.data()->setSignerType((int)ENUNCHUCK::SignerType::AIRGAP);
        }
    }
    endResetModel();
}

void SingleSignerListModel::updateSignerOfRoomWallet(const SignerAssigned &signer)
{
    beginResetModel();
    foreach (QSingleSignerPtr i , m_data ){
        if(!i.data()->isLocalSigner() && qUtils::strCompare(signer.xfp, i.data()->masterFingerPrint())){
            i.data()->setName(signer.name);
            i.data()->setSignerType(signer.type);
        }
    }
    endResetModel();
}

void SingleSignerListModel::resetUserChecked()
{
    beginResetModel();
    foreach (QSingleSignerPtr it, m_data) {
        it.data()->setChecked(false);
    }
    endResetModel();
}

void SingleSignerListModel::setUserCheckedByFingerprint(const bool state, const QString fp)
{
    beginResetModel();
    foreach (QSingleSignerPtr it, m_data) {
        if(qUtils::strCompare(fp, it.data()->masterFingerPrint())){
            it.data()->setChecked(state);
        }
    }
    endResetModel();
}

void SingleSignerListModel::setUserChecked(const bool state, const int index)
{
    if(index < 0 || index >= m_data.count()){
        DBG_INFO << "Index out of range";
    }
    else {
        beginResetModel();
        m_data.at(index)->setChecked(state);
        endResetModel();
    }
}

void SingleSignerListModel::updateHealthCheckTime()
{
    beginResetModel();
    foreach (QSingleSignerPtr it, m_data) {
        it.data()->lastHealthCheckChanged();
    }
    endResetModel();
}

void SingleSignerListModel::syncNunchukEmail(QList<DracoUser> users)
{
    for (int i = 0; i < m_data.count(); i++) {
        if(m_data.at(i) && !m_data.at(i).data()->isLocalSigner()){
            foreach (DracoUser user, users) {
                if(qUtils::strCompare(m_data.at(i).data()->name(), user.chat_id)){
                    m_data.at(i).data()->setEmail(user.email);
                    emit dataChanged(index(i), index(i) );
                }
            }
        }
    }
}

bool SingleSignerListModel::needSyncNunchukEmail()
{
    bool ret = false;
    for (QSingleSignerPtr p : m_data) {
        if(!p.data()->isLocalSigner() && p.data()->email() == ""){
            return true;
        }
    }
    return ret;
}

void SingleSignerListModel::requestSort()
{
    beginResetModel();
    if(m_data.count() > 1){
        qSort(m_data.begin(), m_data.end(), sortSingleSignerByNameAscending);
    }
    endResetModel();
}

void SingleSignerListModel::requestSortKeyset()
{
    beginResetModel();
    if(m_data.count() > 1){
        qSort(m_data.begin(), m_data.end(), sortSingleSignerByKeysetIndexAscending);
    }
    endResetModel();
}

QList<QSingleSignerPtr > SingleSignerListModel::fullList() const
{
    return m_data;
}

std::vector<nunchuk::SingleSigner> SingleSignerListModel::signers() const
{
    std::vector<nunchuk::SingleSigner> signerList;
    for (QSingleSignerPtr p : m_data) {
        signerList.push_back(p->originSingleSigner());
    }
    return signerList;
}

std::vector<nunchuk::SingleSigner> SingleSignerListModel::localSigners() const
{
    std::vector<nunchuk::SingleSigner> signerList;
    for (QSingleSignerPtr p : m_data) {
        if(p.data()->isLocalSigner()){
            signerList.push_back(p->originSingleSigner());
        }
    }
    return signerList;
}

QSharedPointer<SingleSignerListModel> SingleSignerListModel::clone() const
{
    QSharedPointer<SingleSignerListModel> clone = QSharedPointer<SingleSignerListModel>(new SingleSignerListModel());
    for (QSingleSignerPtr signer : m_data) {
        QSingleSignerPtr ret = QSingleSignerPtr(new QSingleSigner(signer.data()->originSingleSigner()));
        if(ret){
            // username
            ret.data()->setEmail(signer.data()->email());
            // tapsigner
            ret.data()->setCardId(signer.data()->cardId());
            //primary key
            ret.data()->setOriginPrimaryKey(signer.data()->originPrimaryKey());
            // Add to data
            clone.data()->addSingleSigner(ret);
        }
    }
    return clone;
}

QSharedPointer<SingleSignerListModel> SingleSignerListModel::cloneKeysets(std::vector<nunchuk::KeysetStatus> keyset_status) const
{
    QSharedPointer<SingleSignerListModel> clone = QSharedPointer<SingleSignerListModel>(new SingleSignerListModel());
    for (int i = 0; i < keyset_status.size(); i++){
        nunchuk::KeysetStatus keyset = keyset_status[i];
        nunchuk::TransactionStatus tx_status = keyset.first;
        nunchuk::KeyStatus keystatus = keyset.second;
        int pending_number = 0;
        for (std::map<std::string, bool>::iterator it = keystatus.begin(); it != keystatus.end(); it++){
            QString xfp = QString::fromStdString(it->first);
            bool    signedStatus = it->second;
            //FIXME - TRICK -->>
            std::set<int> valid_numbers = {(int)nunchuk::TransactionStatus::CONFIRMED, (int)nunchuk::TransactionStatus::READY_TO_BROADCAST, (int)nunchuk::TransactionStatus::PENDING_CONFIRMATION};
            if (valid_numbers.find((int)tx_status) != valid_numbers.end()) {
                signedStatus = true;
            }
            //FIXME - TRICK <<--

            if(!signedStatus) {
                pending_number++;
            }
            for (QSingleSignerPtr signer : m_data) {
                if(qUtils::strCompare(xfp, signer.data()->masterFingerPrint())){
                    QSingleSignerPtr ret = QSingleSignerPtr(new QSingleSigner(signer.data()->originSingleSigner()));
                    if(ret){
                        // username
                        ret.data()->setEmail(signer.data()->email());
                        // tapsigner
                        ret.data()->setCardId(signer.data()->cardId());
                        //primary key
                        ret.data()->setOriginPrimaryKey(signer.data()->originPrimaryKey());
                        // keyset
                        ret.data()->setSignerSigned(signedStatus);
                        ret.data()->setKeysetIndex(i);
                        ret.data()->setKeysetStatus((int)tx_status);
                        ret.data()->setValueKey(signer.data()->valueKey());
                        // Add to data
                        clone.data()->addKeysetSigner(ret, i);
                    }
                }
            }
        }
        clone.data()->updateKeysetPendingnumber(i, pending_number);
    }
    return clone;
}

QSharedPointer<SingleSignerListModel> SingleSignerListModel::cloneFinalSigners(std::map<std::string, bool> final_signers) const
{
    QSharedPointer<SingleSignerListModel> clone = QSharedPointer<SingleSignerListModel>(new SingleSignerListModel());

    for (auto it = final_signers.begin(); it != final_signers.end(); ++it) {
        QString xfp = QString::fromStdString(it->first);
        bool    signedStatus = it->second;
        for (QSingleSignerPtr signer : m_data) {
            if(qUtils::strCompare(xfp, signer.data()->masterFingerPrint())){
                QSingleSignerPtr ret = QSingleSignerPtr(new QSingleSigner(signer.data()->originSingleSigner()));
                if(ret){
                    // username
                    ret.data()->setEmail(signer.data()->email());
                    // tapsigner
                    ret.data()->setCardId(signer.data()->cardId());
                    //primary key
                    ret.data()->setOriginPrimaryKey(signer.data()->originPrimaryKey());
                    // sign status
                    ret.data()->setSignerSigned(signedStatus);
                    ret.data()->setValueKey(signer.data()->valueKey());
                    // Add to data
                    clone.data()->addSingleSigner(ret);
                }
            }
        }
    }
    return clone;
}

void SingleSignerListModel::cleardata()
{
    beginResetModel();
    m_data.clear();
    endResetModel();
}

QStringList SingleSignerListModel::getKeyNames()
{
    QStringList ret;
    foreach (QSingleSignerPtr it, m_data) {
        if(it) {
            ret << it.data()->name();
        }
    }
    return ret;
}

void SingleSignerListModel::setCardIDList(const QMap<QString, QString> &card_ids)
{
    foreach (QSingleSignerPtr it, m_data) {
        if(it) {
            QString card_id = card_ids.value(it.data()->masterFingerPrint());
            it.data()->setCardId(card_id);
        }
    }
}

int SingleSignerListModel::signerCount() const
{
    return m_data.count();
}

int SingleSignerListModel::signerSelectedCount() const
{
    int ret = 0;
    foreach (QSingleSignerPtr it, m_data) {
        if(it.data()->checked()) { ret++;}
    }
    return ret;
}


int SingleSignerListModel::keyinfo() const
{
    return 0;
}

int SingleSignerListModel::signerValueKeyCount()
{
    int ret = 0;
    foreach (QSingleSignerPtr it, m_data) {
        if(it.data()->valueKey()) {
            ret++;
        }
    }
    return ret;
}

int SingleSignerListModel::keysetStatus(int keyset_index)
{
    DBG_INFO << keyset_index;
    foreach (QSingleSignerPtr it, m_data) {
        if(it && (it.data()->keysetIndex() == keyset_index)) {
            return it.data()->keysetStatus();
        }
    }
    return (int)nunchuk::TransactionStatus::PENDING_NONCE;
}

int SingleSignerListModel::keysetRemaingSignarure(int keyset_index)
{
    DBG_INFO << keyset_index;
    int remaining = 0;
    foreach (QSingleSignerPtr it, m_data) {
        if(it && (it.data()->keysetIndex() == keyset_index) && (!it.data()->signerSigned())) {
            remaining += 1;
        }
    }
    return remaining;
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

bool sortSingleSignerByKeysetIndexAscending(const QSingleSignerPtr &v1, const QSingleSignerPtr &v2)
{
    return (v1->keysetIndex() < v2->keysetIndex());
}
