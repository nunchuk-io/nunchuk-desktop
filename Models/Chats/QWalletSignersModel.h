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

#ifndef QWALLETSIGNERSMODEL_H
#define QWALLETSIGNERSMODEL_H

#include <QAbstractListModel>
#include <QQmlEngine>
#include "qUtils.h"

struct SignerAssigned {
    QString name = "";
    QString xfp = "";
    QString xpub = "";
    QString bip32path = "";
    QString joid_id = "";
    QString username = "";
    QString avatar = "";
    int     type = 0;
    bool    isPrimaryKey = false;
    bool is_localuser = false;
};

class QWalletSignersModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)
public:
    QWalletSignersModel();
    ~QWalletSignersModel();
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QHash<int, QByteArray> roleNames() const;
    int signersAssignedCount();
    void addSigner(SignerAssigned signer);
    QString getJoinEventId(const QString& xfp);
    QStringList xfplist();
    bool constains(const QString& xfp);
    SignerAssigned getSignerByIndex(const int index);
    QList<SignerAssigned> fullList() const;
    enum ConversationRoles {
        role_name,
        role_xfp,
        role_xpub,
        role_bip32,
        role_joid_id,
        role_is_localuser,
        role_signer_type,
        role_signer_primary_key,
        role_username,
        role_avatar,
    };
private:
    QList<SignerAssigned> m_data;
signals:
    void countChanged();
};
typedef QSharedPointer<QWalletSignersModel> QWalletSignersModelPtr;

bool sortWalletSigners(const SignerAssigned &v1, const SignerAssigned &v2);

#endif // QWALLETSIGNERSMODEL_H
