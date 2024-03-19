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

#ifndef DEVICELISTMODEL_H
#define DEVICELISTMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include "QOutlog.h"
#include "qUtils.h"

class QDevice : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString deviceName                 READ name                   NOTIFY nameChanged                 )
    Q_PROPERTY(QString deviceType                 READ type                   NOTIFY typeChanged                 )
    Q_PROPERTY(QString devicePath                 READ path                   NOTIFY pathChanged                 )
    Q_PROPERTY(QString deviceModel                READ model                  NOTIFY modelChanged                )
    Q_PROPERTY(QString deviceMasterFingerPrint    READ masterFingerPrint      NOTIFY masterFingerPrintChanged    )
    Q_PROPERTY(bool deviceConnected               READ connected              NOTIFY connectedChanged            )
    Q_PROPERTY(bool deviceNeedsPassPhraseSent     READ needsPassPhraseSent    NOTIFY needsPassPhraseSentChanged  )
    Q_PROPERTY(bool deviceNeedsPinSent            READ needsPinSent           NOTIFY needsPinSentChanged         )
    Q_PROPERTY(QString masterSignerId             READ masterSignerId         NOTIFY masterSignerIdChanged       )
    Q_PROPERTY(QString cardId                     READ cardId         CONSTANT)
public:
    QDevice();
    QDevice(const nunchuk::Device& device);
    ~QDevice();
    QString name();
    QString type() const;
    QString path() const;
    QString model() const;
    QString masterFingerPrint() const;
    bool connected() const;
    void setConnected(bool);
    bool needsPassPhraseSent() const;
    bool needsPinSent() const;
    bool usableToAdd() const;
    QString masterSignerId() const;
    QString cardId() const;
    void setCardId(const QString &card_id);

    nunchuk::Device originDevice() const;
    void setOriginDevice(const nunchuk::Device &device);

private:
    bool m_isDraft = true;
    QString m_cardId = "";
    nunchuk::Device m_device;
    bool isConnected {false};
signals:
    void typeChanged();
    void pathChanged();
    void modelChanged();
    void masterFingerPrintChanged();
    void connectedChanged();
    void needsPassPhraseSentChanged();
    void needsPinSentChanged();
    void nameChanged();
    void usableToSignChanged();
    void usableToAddChanged();
    void masterSignerIdChanged();
};
typedef QSharedPointer<QDevice> QDevicePtr;

class DeviceListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int  count               READ deviceCount        NOTIFY deviceCountChanged)
public:
    DeviceListModel();
    ~DeviceListModel();
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QHash<int,QByteArray> roleNames() const;
    void addDevice(const QDevicePtr &device);
    void updateDeviceList(const  QSharedPointer<DeviceListModel> &d);
    QDevicePtr getDeviceByIndex(const int index);
    QDevicePtr getDeviceByPath(const QString& path);
    QDevicePtr getDeviceByXfp(const QString& xfp);
    QDevicePtr getDeviceNeedPinSent();
    QString getDevicePathByIndex(const int index);
    QString getDevicePathByXfp(const QString& xfp);
    bool deviceNeedPinSent(const QString& xfp);
    bool deviceNeedPinSent(const QString& deviceModel, const QString& deviceType);
    enum DeviceRoles {
        device_name_role,
        device_type_role,
        device_path_role,
        device_model_role,
        device_master_fingerprint_role,
        device_connected_role,
        device_needs_pass_phrase_sent_role,
        device_needs_pin_sent_role,
        device_usableToAdd_role,
        device_master_signer_id_role,
    };
    bool removeDevice(const QString xfp);

    void clearList();

    // For verify addr
    QStringList getXFPList();
    void checkAndUnlockDevice();
    int deviceCount() const;
    int getDeviceIndexByXfp(const QString& xfp);
    QList<QDevicePtr> fullList() const;
    bool contains(const QString &fingerprint);
    bool needScanDevice();
private:
    QList<QDevicePtr> d_;
signals:
    void deviceCountChanged();
};
typedef QSharedPointer<DeviceListModel> QDeviceListModelPtr;

#endif // DEVICELISTMODEL_H
