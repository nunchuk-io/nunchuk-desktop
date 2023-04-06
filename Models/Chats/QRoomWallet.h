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

#ifndef QROOMWALLET_H
#define QROOMWALLET_H

#include <QObject>
#include <QJsonObject>
#include <QMap>
#include "QWalletSignersModel.h"
#include "WalletModel.h"
#include "nunchukmatrix.h"

class QRoomWallet : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString  walletId                READ  get_wallet_id     CONSTANT)
    Q_PROPERTY(QString  walletInitId            READ  get_init_event_id CONSTANT)
    Q_PROPERTY(bool     walletReady             READ  walletReady       NOTIFY walletReadyChanged)
    Q_PROPERTY(int      walletM                 READ walletM            WRITE setWalletm        NOTIFY walletMChanged)
    Q_PROPERTY(int      walletN                 READ walletN            WRITE setWalletn        NOTIFY walletNChanged)
    Q_PROPERTY(int      walletSignersCount      READ  walletSignersCount   NOTIFY walletSignersCountChanged)
    Q_PROPERTY(QString  walletName              READ walletName         WRITE setWalletname         NOTIFY walletNameChanged)
    Q_PROPERTY(QString  walletDescription       READ walletDescription  WRITE setWalletDescription  NOTIFY walletDescriptionChanged)
    Q_PROPERTY(QString  walletAddressType       READ walletAddressType  WRITE setWalletaddressType  NOTIFY walletAddressTypeChanged)
    Q_PROPERTY(bool     walletEscrow            READ walletEscrow       WRITE setWalletescrow       NOTIFY walletEscrowChanged)
    Q_PROPERTY(QWalletSignersModel*             walletSigners           READ walletSigners          NOTIFY walletSignersChanged)
    Q_PROPERTY(bool     isCreator               READ isCreator          NOTIFY isCreatorChanged)
    Q_PROPERTY(Wallet*  info                    READ walletInfo         NOTIFY walletInfoChanged)

public:
    QRoomWallet();
    QRoomWallet(nunchuk::RoomWallet wl);
    ~QRoomWallet();

    QMap<QString, QString> addr_map
    {
        {"NOT_SET"      , "0"},
        {"LEGACY"       , "1"},
        {"NESTED_SEGWIT", "2"},
        {"NATIVE_SEGWIT", "3"}
    };

    nunchuk::RoomWallet nunchukRoomWallet() const;
    QString get_room_id() const;
    QString get_wallet_id() const;
    void set_wallet_id(const QString& value);
    QString get_init_event_id() const;
    void set_init_event_id(const QString& value);
    QList<QString> get_join_event_ids() const;
    void add_join_event_id(const QString& value);
    void set_join_event_ids(const QList<QString>& value);
    QList<QString> get_leave_event_ids() const;
    void add_leave_event_id(const QString& value);
    void set_leave_event_ids(const QList<QString>& value);
    QString get_finalize_event_id() const;
    void set_finalize_event_id(const QString& value);
    QString get_cancel_event_id() const;
    void set_cancel_event_id(const QString& value);
    QString get_ready_event_id() const;
    void set_ready_event_id(const QString& value);
    QString get_json_content() const;
    void updateSignerInfo(const QJsonObject& json);
    void updateWalletInfo(const QJsonObject& json);
    bool walletReady();
    int walletM() const;
    void setWalletm(int walletm);
    int walletN() const;
    void setWalletn(int walletn);
    int walletSignersCount() const;
    QString walletName() const;
    void setWalletname(const QString &walletname);
    QString walletAddressType() const;
    void setWalletaddressType(const QString &walletaddressType);
    bool walletEscrow() const;
    void setWalletescrow(bool walletescrow);
    QString walletDescription() const;
    void setWalletDescription(const QString &walletDescription);
    QWalletSignersModel *walletSigners() const;
    QWalletSignersModelPtr walletSignersPtr() const;
    void setWalletSigners(const QWalletSignersModelPtr &walletSigners);
    bool isCreator() const;
    void setIsCreator(bool isCreator);
    Wallet *walletInfo() const;
    QWalletPtr walletInfoPtr() const;
    void setWalletInfo(const QWalletPtr &d);
private:
    nunchuk::RoomWallet m_roomwallet;
    bool m_isCreator;
    int m_walletm;
    int m_walletn;
    QString m_walletname;
    QString m_walletDescription;
    QString m_walletaddressType;
    bool m_walletescrow;
    QWalletSignersModelPtr m_signers;
    QWalletPtr m_walletInfo;

signals:
    void isCreatorChanged();
    void walletReadyChanged();
    void walletMChanged();
    void walletNChanged();
    void walletNameChanged();
    void walletDescriptionChanged();
    void walletAddressTypeChanged();
    void walletEscrowChanged();
    void walletSignersCountChanged();
    void walletSignersChanged();
    void walletInfoChanged();
};

typedef QSharedPointer<QRoomWallet> QRoomWalletPtr;

#endif // QROOMWALLET_H
