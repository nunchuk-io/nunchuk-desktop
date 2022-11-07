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
#include "QWalletSignersModel.h"
#include "QOutlog.h"
#include "AppSetting.h"

QWalletSignersModel::QWalletSignersModel()
{
    primaryKeys = qUtils::GetPrimaryKeys(AppSetting::instance()->storagePath(),(nunchuk::Chain)AppSetting::instance()->primaryServer());
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

QWalletSignersModel::~QWalletSignersModel()
{

}

int QWalletSignersModel::rowCount(const QModelIndex &parent) const
{
    return m_data.count();
}

QVariant QWalletSignersModel::data(const QModelIndex &index, int role) const
{
    switch (role) {
    case role_name:
        return m_data[index.row()].name;
    case role_xfp:
        return m_data[index.row()].xfp;
    case role_xpub:
        return m_data[index.row()].xpub;
    case role_bip32:
        return m_data[index.row()].bip32path;
    case role_joid_id:
        return m_data[index.row()].joid_id;
    case role_is_localuser:
        return m_data[index.row()].is_localuser;
    case role_signer_type:
        return m_data[index.row()].type;
    case role_signer_primary_key:
        return m_data[index.row()].isPrimaryKey;
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> QWalletSignersModel::roleNames() const
{
    QHash<int, QByteArray> names;
    names[role_name]  = "signer_name";
    names[role_xfp]   = "signer_xfp";
    names[role_xpub]  = "signer_xpub";
    names[role_bip32] = "signer_bip32";
    names[role_joid_id]      = "signer_joid_id";
    names[role_is_localuser] = "signer_is_localuser";
    names[role_signer_type] = "signer_type";
    names[role_signer_primary_key] ="signer_primary_key";
    return names;
}

int QWalletSignersModel::signersAssignedCount()
{
    int ret = 0;
    foreach (SignerAssigned it, m_data) {
        if(it.xfp != ""){
            ret++;
        }
    }
    return ret;
}

void QWalletSignersModel::addSigner(SignerAssigned signer)
{
    beginResetModel();
    if(signer.xfp == "" || !constains(signer.xfp)){
        nunchuk::PrimaryKey key = containPrimaryKey(signer.xfp);
        if(key.get_master_fingerprint() != ""){
            signer.isPrimaryKey = true;
        }
        m_data.append(signer);
    }
    qSort(m_data.begin(), m_data.end(), sortWalletSigners);
    endResetModel();
}

QString QWalletSignersModel::getJoinEventId(const QString &xfp)
{
    foreach (SignerAssigned it, m_data) {
        if(0 == QString::compare(xfp, it.xfp, Qt::CaseInsensitive)){
            return it.joid_id;
        }
    }
    return "";
}

QStringList QWalletSignersModel::xfplist()
{
    QStringList ret;
    ret.clear();
    foreach (SignerAssigned it, m_data) {
        ret.append(it.xfp.toUpper());
    }
    return ret;
}

bool QWalletSignersModel::constains(const QString &xfp)
{
    foreach (SignerAssigned it, m_data) {
        if(0 == QString::compare(xfp, it.xfp, Qt::CaseInsensitive)){
            return true;
        }
    }
    return false;
}

SignerAssigned QWalletSignersModel::getSignerByIndex(const int index)
{
    if(index < 0 || index >= m_data.count()){
        DBG_INFO << "Index out of range";
        return SignerAssigned();
    }
    else {
        return m_data.at(index);
    }
}

QList<SignerAssigned> QWalletSignersModel::fullList() const
{
    return m_data;
}

nunchuk::PrimaryKey QWalletSignersModel::containPrimaryKey(const QString &fingerprint)
{
    for(nunchuk::PrimaryKey key: primaryKeys){
        if(key.get_master_fingerprint() == fingerprint.toStdString()){
            return key;
        }
    }
    return nunchuk::PrimaryKey();
}

bool sortWalletSigners(const SignerAssigned &v1, const SignerAssigned &v2)
{
    return v1.is_localuser > v2.is_localuser;
}
