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

#ifndef MASTERSIGNERMODEL_H
#define MASTERSIGNERMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include "DeviceModel.h"
#include "QOutlog.h"
#include "qUtils.h"

class QMasterSigner : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString masterSignerId      READ id                                      NOTIFY idChanged)
    Q_PROPERTY(QString masterSignername    READ name            WRITE setName           NOTIFY nameChanged)
    Q_PROPERTY(QDevice* masterSignerDevice READ device                                  NOTIFY deviceChanged)
    Q_PROPERTY(int masterSignerHealth      READ health          WRITE setHealth         NOTIFY healthChanged)
    Q_PROPERTY(QString fingerPrint         READ fingerPrint                             NOTIFY fingerPrintChanged)
    Q_PROPERTY(QString message             READ message         WRITE setMessage        NOTIFY messageToSignChanged)
    Q_PROPERTY(QString messageSHA256       READ messageSHA256                           NOTIFY messageToSignChanged)
    Q_PROPERTY(QString signature           READ signature       WRITE setSignature      NOTIFY signatureChanged)
    Q_PROPERTY(QString masterSignerPath    READ path                                    NOTIFY pathChanged)
    Q_PROPERTY(bool needPinSent            READ needPinSent                             NOTIFY needPinSentChanged)
    Q_PROPERTY(bool needPassphraseSent     READ needPassphraseSent                      NOTIFY needPassphraseSentChanged)
    Q_PROPERTY(QString deviceType          READ deviceType                              NOTIFY deviceTypeChanged)
    Q_PROPERTY(bool checked                READ checked         WRITE setChecked        NOTIFY checkedChanged) // For add signer to wallet
    Q_PROPERTY(bool needXpub               READ needXpub                                NOTIFY isNeedXpubChanged)
    Q_PROPERTY(int  signerType             READ signerType                              NOTIFY signerTypeChanged)
    Q_PROPERTY(QString passphrase          READ passphrase                              NOTIFY passphraseChanged)
    Q_PROPERTY(bool isPrimaryKey           READ isPrimaryKey    WRITE setIsPrimaryKey   NOTIFY isPrimaryKeyChanged)
public:
    QMasterSigner();
    QMasterSigner(const nunchuk::MasterSigner &signer);
    ~QMasterSigner();

    void convert(const nunchuk::MasterSigner& src);

    QString id() const;
    void setId(const QString& d);

    QString name() const;
    void setName(const QString& d);

    QDevice* device();
    QDevicePtr devicePtr();

    int health() const;
    void setHealth(const int d);

    QString fingerPrint() const;
    void setFingerPrint(const QString& d);

    QString message();
    QString messageSHA256();
    void setMessage(const QString &message);

    QString signature() const;
    void setSignature(const QString &signature);

    bool connected() const;

    QString path() const;
    void setPath(const QString &d);

    bool checked() const;                   // For add signer to wallet
    void setChecked(const bool checked);    // For add signer to wallet

    bool needPinSent() const;
    bool needPassphraseSent() const;

    QString deviceType() const;
    void setDeviceType(const QString& d);

    bool needXpub() const;
    void setNeedXpub(bool needXpub);

    int signerType() const;
    void setSignerType(int signer_type);

    QString passphrase() const;
    void setPassphrase(const QString &passphrase);

    bool isPrimaryKey() const;
    void setIsPrimaryKey(bool isPrimaryKey);

    void setSignerTags(std::vector<nunchuk::SignerTag> tags);

    nunchuk::PrimaryKey originPrimaryKey() const;

    nunchuk::MasterSigner originMasterSigner() const;
    void setOriginMasterSigner(const nunchuk::MasterSigner &signer);

private:
    QString id_ = "";
    QString name_ = "";
    QString xfp_ = "";
    int health_ = -1;
    QDevicePtr device_ = NULL;
    QString messageToSign_ = "";
    QString signature_ = "";
    QString path_ = "";
    QString deviceType_ = "";
    bool checked_ = false;          // For add signer to wallet
    bool isNeedXpub_ = false;
    int signer_type_ = (int)nunchuk::SignerType::UNKNOWN;
    QString passphrase_ = "";
    nunchuk::PrimaryKey primaryKey_;
    nunchuk::MasterSigner masterSigner_;
    bool isPrimaryKey_ = false;
    bool isDraft = false;
signals:
    void idChanged();
    void nameChanged();
    void deviceChanged();
    void healthChanged();
    void fingerPrintChanged();
    void messageToSignChanged();
    void signatureChanged();
    void warningMessageChanged();
    void pathChanged();
    void needPinSentChanged();
    void needPassphraseSentChanged();
    void deviceTypeChanged();
    void checkedChanged(); // For add signer to wallet
    void isNeedXpubChanged();
    void signerTypeChanged();
    void passphraseChanged();
    void isPrimaryKeyChanged();
};
typedef QSharedPointer<QMasterSigner> QMasterSignerPtr;

bool sortMasterSignerByNameAscending(const QMasterSignerPtr &v1, const QMasterSignerPtr &v2);
bool sortMasterSignerByNameDescending(const QMasterSignerPtr &v1, const QMasterSignerPtr &v2);

class MasterSignerListModel  : public QAbstractListModel
{
    Q_OBJECT
public:
    MasterSignerListModel();
    ~MasterSignerListModel();
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QHash<int,QByteArray> roleNames() const;
    void addMasterSigner(const QMasterSignerPtr &d);
    QMasterSignerPtr getMasterSignerByIndex(const int index);
    QMasterSignerPtr getMasterSignerById(const QString& id);
    QMasterSignerPtr getMasterSignerByXfp(const QString& xfp);
    QString getMasterSignerNameByFingerPrint(const QString &fingerprint);
    bool removeMasterSigner(const QMasterSignerPtr it);
    bool removeMasterSigner(const QString& masterSignerId);
    void resetUserChecked();
    void setUserChecked(const bool state, const int index);
    void setUserCheckedById(const bool state, const QString& id);
    void setUserCheckedByFingerprint(const bool state, const QString& xfp);
    bool contains(const QString &masterSignerId);
    bool containsFingerPrint(const QString &fingerprint);
    void updateMasterSignerNeedXpubByXfp(const QString &xfp, const bool value);
    void updateMasterSignerNeedXpubById(const QString &id, const bool value);
    void resetMasterSignerNeedXpub();
    QStringList getColdCardId() const;
    bool isColdCard(const QString& xfp);
    void requestSort(int role, int order);
    QList<QMasterSignerPtr> fullList() const;
    void cleardata();
    void reloadOriginMasterSignerById(const QString &id);

    enum MasterSignerRoles {
        master_signer_id_Role,
        master_signer_name_Role,
        master_signer_device_Role,
        master_signer_checked_Role,
        master_signer_fingerPrint_Role,
        master_signer_msg2Sign_Role,
        master_signer_deviceType_Role,
        master_signer_signerType_Role,
        master_signer_need_passphrase_Role,
        master_signer_need_pin_Role,
        master_signer_need_xpub_Role,
        master_signer_primary_key_Role
    };

public slots:
    int signerCount() const;
    int signerSelectedCount() const;
private:
    QList<QMasterSignerPtr> d_;
};
typedef QSharedPointer<MasterSignerListModel> QMasterSignerListModelPtr;

#endif // MASTERSIGNERMODEL_H
