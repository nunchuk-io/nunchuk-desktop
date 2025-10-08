/**************************************************************************
 * This file is part of the Nunchuk software (https://nunchuk.io/)        *
 * Copyright (C) 2020-2022 Enigmo
 ** Copyright (C) 2022 Nunchuk
 **
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

#include "Servers/Draco.h"
#include "ViewsEnums.h"
#include "bridgeifaces.h"

QMasterSigner::QMasterSigner() : isDraft(true) {
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

QMasterSigner::QMasterSigner(const nunchuk::MasterSigner &signer) : masterSigner_(signer) {
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

QMasterSigner::~QMasterSigner() {}

void QMasterSigner::convert(const nunchuk::MasterSigner &src) {
    isDraft = false;
    masterSigner_ = src;
}

QString QMasterSigner::id() const {
    if (isDraft) {
        return id_;
    } else {
        return QString::fromStdString(masterSigner_.get_id());
    }
}

void QMasterSigner::setId(const QString &d) {
    if (isDraft && d != id_) {
        id_ = d;
        emit idChanged();
    }
}

QString QMasterSigner::name() const {
    if (isDraft) {
        return name_;
    } else {
        return QString::fromStdString(masterSigner_.get_name());
    }
}

void QMasterSigner::setName(const QString &d) {
    if (isDraft) {
        name_ = d;
    } else {
        masterSigner_.set_name(d.toStdString());
    }
    emit nameChanged();
}

QDevice *QMasterSigner::device() {
    return devicePtr().data();
}

QDevicePtr QMasterSigner::devicePtr() {
    if (!device_) {
        device_ = QDevicePtr(new QDevice(masterSigner_.get_device()));
    }
    return device_;
}

int QMasterSigner::health() const {
    return health_;
}

void QMasterSigner::setHealth(const int d) {
    if (d != health_) {
        health_ = d;
        emit healthChanged();
    }
}

bool QMasterSigner::connected() const {
    if (!device_) {
        return false;
    } else {
        return masterSigner_.get_device().connected();
    }
}

bool QMasterSigner::checked() const {
    return checked_;
}

void QMasterSigner::setChecked(const bool checked) {
    if (checked != checked_) {
        checked_ = checked;
        emit checkedChanged();
    }
}

bool QMasterSigner::needPinSent() const {
    return masterSigner_.get_device().needs_pin_sent();
}

bool QMasterSigner::needPassphraseSent() const {
    return masterSigner_.get_device().needs_pass_phrase_sent();
}

QString QMasterSigner::deviceType() const {
    if (isDraft) {
        return deviceType_;
    } else {
        return QString::fromStdString(masterSigner_.get_device().get_type());
    }
}

void QMasterSigner::setDeviceType(const QString &d) {
    if (isDraft) {
        deviceType_ = d;
        emit deviceTypeChanged();
    }
}

bool QMasterSigner::needXpub() const {
    return isNeedXpub_;
}

void QMasterSigner::setNeedXpub(bool isNeedXpub) {
    isNeedXpub_ = isNeedXpub;
}

QString QMasterSigner::fingerPrint() const {
    if (isDraft) {
        return xfp_;
    } else {
        return QString::fromStdString(masterSigner_.get_device().get_master_fingerprint());
    }
}

void QMasterSigner::setFingerPrint(const QString &d) {
    if (isDraft) {
        xfp_ = d;
        emit fingerPrintChanged();
    }
}

QString QMasterSigner::message() {
    if (messageToSign_.isEmpty() || messageToSign_.isNull() || messageToSign_ == "") {
        messageToSign_ = qUtils::QGenerateRandomMessage();
    }
    return messageToSign_;
}

QString QMasterSigner::messageSHA256() {
    return qUtils::GetSHA256(messageToSign_);
}

void QMasterSigner::setMessage(const QString &messageToSign) {
    if (messageToSign_ != messageToSign) {
        messageToSign_ = messageToSign;
        emit messageToSignChanged();
    }
}

QString QMasterSigner::signature() const {
    return signature_;
}

void QMasterSigner::setSignature(const QString &signature) {
    if (signature_ != signature) {
        signature_ = signature;
        emit signatureChanged();
    }
}

QString QMasterSigner::path() const {
    return path_;
}

void QMasterSigner::setPath(const QString &d) {
    if (path_ != d) {
        path_ = d;
        emit pathChanged();
    }
}

int QMasterSigner::signerType() const {
    if (isDraft) {
        return signer_type_;
    } else {
        return (int)masterSigner_.get_type();
    }
}

void QMasterSigner::setSignerType(int signer_type) {
    if (isDraft) {
        signer_type_ = signer_type;
        emit signerTypeChanged();
    }
}

QString QMasterSigner::passphrase() const {
    return passphrase_;
}

void QMasterSigner::setPassphrase(const QString &passphrase) {
    if (passphrase_ != passphrase) {
        passphrase_ = passphrase;
        emit passphraseChanged();
    }
}

bool QMasterSigner::isPrimaryKey() const {
    return qUtils::isPrimaryKey(fingerPrint());
}

void QMasterSigner::setSignerTags(std::vector<nunchuk::SignerTag> tags) {
    if (!isDraft) {
        masterSigner_.set_tags(tags);
    }
}

nunchuk::PrimaryKey QMasterSigner::originPrimaryKey() {
    if (isPrimaryKey()) {
        if (primaryKey_.get_master_fingerprint().empty()) {
            primaryKey_ = AppModel::instance()->findPrimaryKey(fingerPrint());
        }
    }
    return primaryKey_;
}

nunchuk::MasterSigner QMasterSigner::originMasterSigner() const {
    return masterSigner_;
}

void QMasterSigner::setOriginMasterSigner(const nunchuk::MasterSigner &signer) {
    masterSigner_ = signer;
}

nunchuk::SignerTag QMasterSigner::signerTag() const {
    std::vector<nunchuk::SignerTag> tags = masterSigner_.get_tags();
    tags.erase(
        std::remove_if(tags.begin(), tags.end(), [](const nunchuk::SignerTag& tag) {
            return tag == nunchuk::SignerTag::INHERITANCE;
        }),
        tags.end()
    );
    if(tags.empty()) {
        return static_cast<nunchuk::SignerTag>(-1); // Default tag
    }
    return tags.front();
}

QString QMasterSigner::tag() const {
    QStringList list = tags();
    list.removeOne("INHERITANCE");
    return list.isEmpty() ? "" : list.first();
}

QStringList QMasterSigner::tags() const {
    QStringList list;
    for (auto tag : masterSigner_.get_tags()) {
        list.append(QString::fromStdString(SignerTagToStr(tag)));
    }
    return list;
}

QString QMasterSigner::address() const {
    return m_address;
}

void QMasterSigner::setAddress(const QString &address) {
    if (m_address == address)
        return;
    m_address = address;
    emit addressChanged();
}

QVariantList QMasterSigner::getWalletList() {
    QVariantList ret;
    for (auto wallet : AppModel::instance()->walletListPtr()->fullList()) {
        if (wallet->isContainKey(fingerPrint()) && wallet->isAssistedWallet()) {
            auto walletQml = WalletListModel::useQml(wallet);
            ret.append(QVariant::fromValue(walletQml));
        }
    }
    return ret;
}

nunchuk::SingleSigner QMasterSigner::cloneSingleSigner(const nunchuk::WalletType& wallet_type, const nunchuk::AddressType& address_type) {

    QString masterId = id().isEmpty() ? fingerPrint() : id();
    QWarningMessage msg;    
    nunchuk::SingleSigner signer = bridge::nunchukGetDefaultSignerFromMasterSigner(masterId,
                                                                                wallet_type,
                                                                                address_type,
                                                                                msg);

    if((int)EWARNING::WarningType::NONE_MSG != msg.type()){
        signer = qUtils::toSingleSigner(masterSigner_);
    }
    return signer;
}

bool QMasterSigner::isMine() const {
    QWarningMessage msg;
    std::vector<nunchuk::MasterSigner> signers = bridge::nunchukGetOriginMasterSigners(msg);
    auto it = std::find_if(signers.begin(), signers.end(), [this](nunchuk::MasterSigner signer) {
        return signer.get_device().get_master_fingerprint() == fingerPrint().toStdString() ||
               signer.get_id() == fingerPrint().toStdString();
    });
    return it != signers.end();
}

void QMasterSigner::setDeviceIndex(int newDeviceIndex) {
    m_deviceIndex = newDeviceIndex;
}

int QMasterSigner::deviceIndex() const {
    return m_deviceIndex;
}

bool QMasterSigner::needBackup() {
    return masterSigner_.need_backup();
}

void QMasterSigner::setNeedBackup(bool val) {
    masterSigner_.set_need_backup(val);
}

bool QMasterSigner::allowAssignToWallet() const {
    auto walletList = AppModel::instance()->walletListPtr();
    if (!walletList) {
        return true;
    }
    for (const auto& wallet : walletList->fullList()) {
        bool isContain = wallet->isContainKey(fingerPrint());
        bool needBackup = wallet->keyNeedBackup();
        if (isContain && needBackup) {
            return false;
        }
    }
    return true;
}

MasterSignerListModel::MasterSignerListModel() {
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

MasterSignerListModel::~MasterSignerListModel() {
    d_.clear();
}

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
        return qVariantFromValue((QDevice *)d_[index.row()]->device());
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
    case master_signer_tag_Role:
        return d_[index.row()]->tag();
    case master_signer_needBackup_Role:
        return d_[index.row()]->needBackup();
    default:
        return QVariant();
    }
}

bool MasterSignerListModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (role == master_signer_checked_Role) {
        d_[index.row()]->setChecked(value.toBool());
    } else {
    }
    emit dataChanged(index, index, {role});
    return true;
}

QHash<int, QByteArray> MasterSignerListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[master_signer_id_Role] = "master_signer_id";
    roles[master_signer_name_Role] = "master_signer_name";
    roles[master_signer_device_Role] = "master_signer_device";
    roles[master_signer_checked_Role] = "master_signer_checked";
    roles[master_signer_fingerPrint_Role] = "master_signer_fingerPrint";
    roles[master_signer_msg2Sign_Role] = "master_signer_messageToSign";
    roles[master_signer_deviceType_Role] = "master_signer_deviceType";
    roles[master_signer_signerType_Role] = "master_signer_type";
    roles[master_signer_need_passphrase_Role] = "master_signer_need_passphrase";
    roles[master_signer_need_pin_Role] = "master_signer_need_pin";
    roles[master_signer_need_xpub_Role] = "master_signer_need_xpub";
    roles[master_signer_primary_key_Role] = "master_signer_primary_key";
    roles[master_signer_tag_Role] = "master_signer_tag";
    roles[master_signer_needBackup_Role] = "master_signer_needBackup";
    return roles;
}

void MasterSignerListModel::addMasterSigner(const QMasterSignerPtr &d) {
    beginResetModel();
    if (!contains(d.data()->id())) {
        d_.append(d);
    }
    endResetModel();
    emit signerListChanged();
}

QMasterSignerPtr MasterSignerListModel::getMasterSignerByIndex(const int index) {
    if (index < 0 || index >= d_.count()) {
        DBG_INFO << "Index out of range";
        return QMasterSignerPtr(NULL);
    } else {
        return d_.at(index);
    }
}

QMasterSignerPtr MasterSignerListModel::getMasterSignerById(const QString &id) {
    foreach (QMasterSignerPtr it, d_) {
        if (it.data()->id() == id) {
            return it;
        }
    }
    return QMasterSignerPtr(NULL);
}

QMasterSignerPtr MasterSignerListModel::getMasterSignerByXfp(const QString &xfp) {
    foreach (QMasterSignerPtr it, d_) {
        if (qUtils::strCompare(xfp, it.data()->fingerPrint())) {
            return it;
        }
    }
    return NULL;
}

QMasterSignerPtr MasterSignerListModel::getMasterSignerByXfpName(const QString &xfp,
                                                                 const QString &name) {
    foreach (QMasterSignerPtr it, d_) {
        if (qUtils::strCompare(xfp, it.data()->fingerPrint()) &&
            qUtils::strCompare(name, it.data()->name())) {
            return it;
        }
    }
    return NULL;
}

QString MasterSignerListModel::getMasterSignerNameByFingerPrint(const QString &fingerprint) {
    foreach (QMasterSignerPtr it, d_) {
        if (qUtils::strCompare(fingerprint, it.data()->fingerPrint())) {
            return it.data()->name();
        }
    }
    return "";
}

int MasterSignerListModel::getIndexNameByFingerPrint(const QString &fingerprint) {
    for (int i = 0; i < d_.count(); i++) {
        if (qUtils::strCompare(fingerprint, d_[i].data()->fingerPrint())) {
            return i;
        }
    }
    return -1;
}

bool MasterSignerListModel::removeMasterSigner(const QString &masterSignerId) {
    beginResetModel();
    foreach (QMasterSignerPtr it, d_) {
        if (it.data() && it.data()->id() == masterSignerId) {
            d_.removeAll(it);
            return true;
        }
    }
    endResetModel();
    emit signerListChanged();
    return false;
}

void MasterSignerListModel::resetUserChecked() {
    beginResetModel();
    foreach (QMasterSignerPtr it, d_) {
        it.data()->setChecked(false);
    }
    endResetModel();
}

void MasterSignerListModel::setUserChecked(const bool state, const int index) {
    if (index < 0 || index >= d_.count()) {
        DBG_INFO << "Index out of range";
    } else {
        beginResetModel();
        d_.at(index)->setChecked(state);
        endResetModel();
    }
}

void MasterSignerListModel::setUserCheckedById(const bool state, const QString &id) {
    beginResetModel();
    foreach (QMasterSignerPtr it, d_) {
        if (qUtils::strCompare(id, it.data()->id())) {
            it.data()->setChecked(state);
        }
    }
    endResetModel();
}

void MasterSignerListModel::setUserCheckedByFingerprint(const bool state, const QString &xfp) {
    beginResetModel();
    foreach (QMasterSignerPtr it, d_) {
        if (qUtils::strCompare(xfp, it.data()->fingerPrint())) {
            it.data()->setChecked(state);
        }
    }
    endResetModel();
}

bool MasterSignerListModel::removeMasterSigner(const QMasterSignerPtr it) {
    beginResetModel();
    d_.removeAll(it);
    endResetModel();
    emit signerListChanged();
    return true;
}

bool MasterSignerListModel::contains(const QString &masterSignerId) {
    foreach (QMasterSignerPtr i, d_) {
        if (qUtils::strCompare(masterSignerId, i.data()->id())) {
            return true;
        }
    }
    return false;
}

bool MasterSignerListModel::containsFingerPrint(const QString &fingerprint) {
    foreach (QMasterSignerPtr i, d_) {
        if (qUtils::strCompare(fingerprint, i.data()->fingerPrint())) {
            return true;
        }
    }
    return false;
}

void MasterSignerListModel::updateMasterSignerNeedXpubById(const QString &id, const bool value) {
    beginResetModel();
    foreach (QMasterSignerPtr i, d_) {
        if (qUtils::strCompare(id, i.data()->id())) {
            i.data()->setNeedXpub(value);
        }
    }
    endResetModel();
}

void MasterSignerListModel::updateMasterSignerNeedXpubByXfp(const QString &xfp, const bool value) {
    beginResetModel();
    foreach (QMasterSignerPtr i, d_) {
        if (qUtils::strCompare(xfp, i.data()->fingerPrint())) {
            i.data()->setNeedXpub(value);
        }
    }
    endResetModel();
}

void MasterSignerListModel::resetMasterSignerNeedXpub() {
    beginResetModel();
    foreach (QMasterSignerPtr i, d_) {
        i.data()->setNeedXpub(false);
    }
    endResetModel();
}

QStringList MasterSignerListModel::getColdCardId() const {
    QStringList ret;
    ret.clear();
    foreach (QMasterSignerPtr i, d_) {
        if (i.data()->device() && qUtils::strCompare("coldcard", i.data()->device()->type())) {
            ret << i.data()->id();
        }
    }
    return ret;
}

bool MasterSignerListModel::isColdCard(const QString &xfp) {
    foreach (QMasterSignerPtr i, d_) {
        if (i.data()->signerType() == (int)ENUNCHUCK::SignerType::COLDCARD_NFC) {
            return true;
        } else {
            if (i.data()->device() && i.data()->device()->type().contains("coldcard")) {
                return true;
            }
        }
    }
    return false;
}

bool MasterSignerListModel::isPrimaryKey(const QString &xfp) {
    foreach (QMasterSignerPtr i, d_) {
        if (qUtils::strCompare(xfp, i->fingerPrint())) {
            return i.data()->isPrimaryKey();
        }
    }
    return false;
}

void MasterSignerListModel::requestSort(int role, int order) {
    beginResetModel();
    if (d_.count() > 1) {
        switch (role) {
        case master_signer_name_Role: {
            if (Qt::DescendingOrder == order) {
                qSort(d_.begin(), d_.end(), sortMasterSignerByNameDescending);
            } else {
                qSort(d_.begin(), d_.end(), sortMasterSignerByNameAscending);
            }
        } break;
        default:
            break;
        }
    }
    endResetModel();
}

QList<QMasterSignerPtr> MasterSignerListModel::fullList() const {
    return d_;
}

void MasterSignerListModel::cleardata() {
    beginResetModel();
    d_.clear();
    endResetModel();
}

int MasterSignerListModel::signerCount() const {
    return d_.count();
}

int MasterSignerListModel::signerSelectedCount() const {
    int ret = 0;
    foreach (QMasterSignerPtr it, d_) {
        if (it.data()->checked()) {
            ret++;
        }
    }
    return ret;
}

void MasterSignerListModel::reloadOriginMasterSignerById(const QString &id) {
    beginResetModel();
    foreach (QMasterSignerPtr i, d_) {
        if (qUtils::strCompare(id, i.data()->id())) {
            QWarningMessage msg;
            nunchuk::MasterSigner signer = bridge::nunchukGetOriginMasterSigner(id, msg);
            if ((int)EWARNING::WarningType::NONE_MSG == msg.type()) {
                i.data()->setOriginMasterSigner(signer);
            }
        }
    }
    endResetModel();
}

int MasterSignerListModel::getHotKeyIndex() const {
    int index = 0;
    foreach (QMasterSignerPtr i, d_) {
        if (i->name().contains("My key")) {
            index++;
        }
    }
    return (index + 1);
}

int MasterSignerListModel::pendingBackupCount() const {
    int count = 0;
    foreach (QMasterSignerPtr i, d_) {
        if (i->needBackup()) {
            count++;
        }
    }
    return count;
}

bool sortMasterSignerByNameAscending(const QMasterSignerPtr &v1, const QMasterSignerPtr &v2) {
    return (QString::compare((v1.data()->name()), (v2.data()->name())) < 0);
}

bool sortMasterSignerByNameDescending(const QMasterSignerPtr &v1, const QMasterSignerPtr &v2) {
    return (QString::compare((v1.data()->name()), (v2.data()->name())) > 0);
}
