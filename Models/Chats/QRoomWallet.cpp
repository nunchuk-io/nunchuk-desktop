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
#include "QRoomWallet.h"
#include "QOutlog.h"
#include <QJsonDocument>
#include "AppModel.h"
#include "ifaces/bridgeifaces.h"

QRoomWallet::QRoomWallet():
    m_isCreator(true),
    m_walletm(0),
    m_walletn(0),
    m_walletname(""),
    m_walletDescription(""),
    m_walletaddressType(QString::number((int)ENUNCHUCK::AddressType::NATIVE_SEGWIT)), // Default is NATIVE_SEGWIT
    m_walletescrow(false),
    m_signers(QWalletSignersModelPtr(new QWalletSignersModel())),
    m_walletInfo(NULL)
{

}

QRoomWallet::QRoomWallet(nunchuk::RoomWallet wl):
    m_roomwallet(wl),
    m_isCreator(true),
    m_walletm(0),
    m_walletn(0),
    m_walletname(""),
    m_walletDescription(""),
    m_walletaddressType(QString::number((int)ENUNCHUCK::AddressType::NATIVE_SEGWIT)), // Default is NATIVE_SEGWIT
    m_walletescrow(false),
    m_signers(QWalletSignersModelPtr(new QWalletSignersModel())),
    m_walletInfo(NULL)
{

}

QRoomWallet::~QRoomWallet()
{

}

nunchuk::RoomWallet QRoomWallet::nunchukRoomWallet() const
{
    return m_roomwallet;
}

QString QRoomWallet::get_room_id() const
{
    return QString::fromStdString(m_roomwallet.get_room_id());
}

QString QRoomWallet::get_wallet_id() const
{
    return QString::fromStdString(m_roomwallet.get_wallet_id());
}

void QRoomWallet::set_wallet_id(const QString &value)
{
    m_roomwallet.set_wallet_id(value.toStdString());
}

QString QRoomWallet::get_init_event_id() const
{
    return QString::fromStdString(m_roomwallet.get_init_event_id());
}

void QRoomWallet::set_init_event_id(const QString &value)
{
    m_roomwallet.set_init_event_id(value.toStdString());
}

QList<QString> QRoomWallet::get_join_event_ids() const
{
    QList<QString> ret;
    ret.clear();
    std::vector<std::string> v = m_roomwallet.get_join_event_ids();
    foreach (std::string i, v) {
        ret.append(QString::fromStdString(i));
    }
    return ret;
}

void QRoomWallet::add_join_event_id(const QString &value)
{
    m_roomwallet.add_join_event_id(value.toStdString());
}

void QRoomWallet::set_join_event_ids(const QList<QString> &value)
{
    std::vector<std::string> v;
    if(v.size() > 0) v.clear();
    foreach (QString i, value) {
        v.push_back(i.toStdString());
    }
    m_roomwallet.set_join_event_ids(v);
}

QList<QString> QRoomWallet::get_leave_event_ids() const
{
    QList<QString> ret;
    ret.clear();
    std::vector<std::string> v = m_roomwallet.get_leave_event_ids();
    foreach (std::string i, v) {
        ret.append(QString::fromStdString(i));
    }
    return ret;
}

void QRoomWallet::add_leave_event_id(const QString &value)
{
    m_roomwallet.add_leave_event_id(value.toStdString());
}

void QRoomWallet::set_leave_event_ids(const QList<QString> &value)
{
    std::vector<std::string> v;
    if(v.size() > 0) v.clear();
    foreach (QString i, value) {
        v.push_back(i.toStdString());
    }
    m_roomwallet.set_leave_event_ids(v);
}

QString QRoomWallet::get_finalize_event_id() const
{
    return QString::fromStdString(m_roomwallet.get_finalize_event_id());
}

void QRoomWallet::set_finalize_event_id(const QString &value)
{
    m_roomwallet.set_finalize_event_id(value.toStdString());
    emit walletReadyChanged();
}

QString QRoomWallet::get_cancel_event_id() const
{
    return QString::fromStdString(m_roomwallet.get_cancel_event_id());
}

void QRoomWallet::set_cancel_event_id(const QString &value)
{
    m_roomwallet.set_cancel_event_id(value.toStdString());
}

QString QRoomWallet::get_ready_event_id() const
{
    return QString::fromStdString(m_roomwallet.get_ready_event_id());
}

void QRoomWallet::set_ready_event_id(const QString &value)
{
    m_roomwallet.set_ready_event_id(value.toStdString());
}

QString QRoomWallet::get_json_content() const
{
    return QString::fromStdString(m_roomwallet.get_json_content());
}

void QRoomWallet::updateSignerInfo(const QJsonObject &json)
{
    SignerAssigned signer;
    signer.name = json["name"].toString();
    signer.xfp = json["xfp"].toString();
    signer.xpub = json["xpub"].toString();
    signer.bip32path = json["bip32path"].toString();
    signer.joid_id = json["joid_id"].toString();
    signer.is_localuser = json["is_localuser"].toBool();
    signer.type = json["signer_type"].toInt();
    signer.username = json["username"].toString();
    signer.avatar = json["avatar"].toString();
    // Check mine
    if(signer.is_localuser && signer.xfp != ""){
        if((int)ENUNCHUCK::SignerType::SOFTWARE == signer.type || (int)ENUNCHUCK::SignerType::HARDWARE == signer.type){
            if(AppModel::instance()->masterSignerList()){
                QMasterSignerPtr master = AppModel::instance()->masterSignerList()->getMasterSignerByXfp(signer.xfp);
                if(master){
                    signer.type = master.data()->signerType();
                }
            }
        }
        else if((int)ENUNCHUCK::SignerType::AIRGAP == signer.type){
            if(AppModel::instance()->remoteSignerList()){
                QSingleSignerPtr remote = AppModel::instance()->remoteSignerList()->getSingleSignerByFingerPrint(signer.xfp);
                if(remote){
                    signer.type = (int)ENUNCHUCK::SignerType::AIRGAP;
                }
            }
        }
        else{ }
    }

    walletSigners()->addSigner(signer);
}

void QRoomWallet::updateWalletInfo(const QJsonObject &json)
{
    QJsonObject body = json["body"].toObject();
    QString msgtype = json["msgtype"].toString();
    setWalletm(body["m"].toInt());
    setWalletn(body["n"].toInt());
    setWalletname(body["name"].toString());
    setWalletDescription(body["description"].toString());
    setWalletescrow(body["is_escrow"].toBool());
    setWalletaddressType(addr_map[body["address_type"].toString()]);
}

bool QRoomWallet::walletReady()
{
    return get_finalize_event_id() != "";
}

int QRoomWallet::walletM() const
{
    return m_walletm;
}

void QRoomWallet::setWalletm(int walletm)
{
    if(m_walletm == walletm) return;
    m_walletm = walletm;
    emit walletMChanged();
}

int QRoomWallet::walletN() const
{
    return m_walletn;
}

void QRoomWallet::setWalletn(int walletn)
{
    if(m_walletn == walletn) return;
    m_walletn = walletn;
    emit walletNChanged();
}

int QRoomWallet::walletSignersCount() const
{
    return m_signers ? m_signers.data()->signersAssignedCount() : 0;
}

QString QRoomWallet::walletName() const
{
    return m_walletname;
}

void QRoomWallet::setWalletname(const QString &walletname)
{
    if(m_walletname == walletname) return;
    m_walletname = walletname;
    emit walletNameChanged();
}

QString QRoomWallet::walletAddressType() const
{
    return m_walletaddressType;
}

void QRoomWallet::setWalletaddressType(const QString &walletaddressType)
{
    if(m_walletaddressType == walletaddressType) return;
    m_walletaddressType = walletaddressType;
    emit walletAddressTypeChanged();
}

bool QRoomWallet::walletEscrow() const
{
    return m_walletescrow;
}

void QRoomWallet::setWalletescrow(bool walletescrow)
{
    if(m_walletescrow == walletescrow) return;
    m_walletescrow = walletescrow;
    emit walletEscrowChanged();
}

QString QRoomWallet::walletDescription() const
{
    return m_walletDescription;
}

void QRoomWallet::setWalletDescription(const QString &walletDescription)
{
    if(m_walletDescription == walletDescription) return;
    m_walletDescription = walletDescription;
    emit walletDescriptionChanged();
}

QWalletSignersModel *QRoomWallet::walletSigners() const
{
    return m_signers.data();
}

QWalletSignersModelPtr QRoomWallet::walletSignersPtr() const
{
    return m_signers;
}

void QRoomWallet::setWalletSigners(const QWalletSignersModelPtr &signers)
{
    m_signers = signers;
    emit walletSignersChanged();
}

bool QRoomWallet::isCreator() const
{
    return m_isCreator;
}

void QRoomWallet::setIsCreator(bool isCreator)
{
    if(m_isCreator == isCreator) return;
    m_isCreator = isCreator;
    emit isCreatorChanged();
}

Wallet *QRoomWallet::walletInfo() const
{
    return m_walletInfo.data();
}

QWalletPtr QRoomWallet::walletInfoPtr() const
{
    return m_walletInfo;
}

void QRoomWallet::setWalletInfo(const QWalletPtr &d)
{
    m_walletInfo = d;
    emit walletInfoChanged();
}
