#ifndef MASTERSIGNERMODEL_H
#define MASTERSIGNERMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include "DeviceModel.h"
#include "QWarningMessage.h"
#include "QOutlog.h"

class MasterSigner : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString masterSignerId           READ id                            NOTIFY idChanged)
    Q_PROPERTY(QString masterSignername         READ name       WRITE setName      NOTIFY nameChanged)
    Q_PROPERTY(Device* masterSignerDevice       READ device                        NOTIFY deviceChanged)
    Q_PROPERTY(int masterSignerHealth           READ health     WRITE setHealth    NOTIFY healthChanged)
    Q_PROPERTY(QString fingerPrint              READ fingerPrint                   NOTIFY fingerPrintChanged)
    Q_PROPERTY(QString message                  READ message        WRITE setMessage        NOTIFY messageToSignChanged)
    Q_PROPERTY(QString signature                READ signature      WRITE setSignature      NOTIFY signatureChanged)
    Q_PROPERTY(QWarningMessage* warningMessage  READ warningMessage     NOTIFY warningMessageChanged)
    Q_PROPERTY(QString masterSignerPath         READ path               NOTIFY pathChanged)
    Q_PROPERTY(bool isSoftwareSigner            READ isSoftwareSigner   NOTIFY isSoftwareSignerChanged)
    Q_PROPERTY(bool needPinSent                 READ needPinSent        NOTIFY needPinSentChanged)
    Q_PROPERTY(bool needPassphraseSent          READ needPassphraseSent NOTIFY needPassphraseSentChanged)
    Q_PROPERTY(QString deviceType               READ deviceType         NOTIFY deviceTypeChanged)

    // For add signer to wallet
    Q_PROPERTY(bool masterSignerChecked         READ checked        WRITE setChecked        NOTIFY checkedChanged)
public:
    MasterSigner();
    MasterSigner(const QString& id, const QString& name, const QSharedPointer<Device> device, const int health = -1, const bool software = false);
    ~MasterSigner();

    QString id() const;
    QString name() const;
    Device* device() const;
    QSharedPointer<Device> devicePtr() const;
    int health() const;
    bool connected() const;
    void setId(const QString& d);
    void setName(const QString& d);
    void setDevice(const QSharedPointer<Device> &d);
    void setHealth(const int d);
    QString fingerPrint() const;

    QString message() const;
    void setMessage(const QString &message);

    QString signature() const;
    void setSignature(const QString &signature);

    QWarningMessage* warningMessage() const;
    QSharedPointer<QWarningMessage> warningMessagePtr() const;
    void setWarningMessage(const QSharedPointer<QWarningMessage> &warningMessage);

    QString path() const;
    void setPath(const QString &path);

    // For add signer to wallet
    bool checked() const;
    void setChecked(const bool checked);

    bool isSoftwareSigner() const;
    void setIsSoftwareSigner(bool isSoftwareSigner);

    bool needPinSent() const;
    void setNeedPinSent(bool value);
    bool needPassphraseSent() const;
    void setNeedPassphraseSent(bool value);
    QString deviceType() const;

private:
    QString id_;
    QString name_;
    QSharedPointer<Device> device_;
    int health_;
    QString messageToSign_;
    QString signature_;
    QSharedPointer<QWarningMessage> warningMessage_;
    QString path_;

    // For add signer to wallet
    bool checked_;
    bool isSoftwareSigner_;
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
    void isSoftwareSignerChanged();
    void needPinSentChanged();
    void needPassphraseSentChanged();
    void deviceTypeChanged();
    // For add signer to wallet
    void checkedChanged();
};

bool sortMasterSignerByNameAscending(const QSharedPointer<MasterSigner> &v1, const QSharedPointer<MasterSigner> &v2);

bool sortMasterSignerByNameDescending(const QSharedPointer<MasterSigner> &v1, const QSharedPointer<MasterSigner> &v2);

class MasterSignerListModel  : public QAbstractListModel
{
    Q_OBJECT
public:
    MasterSignerListModel();
    ~MasterSignerListModel();
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QHash<int,QByteArray> roleNames() const;
    void addMasterSigner(const QString& id, const QString& name, const QSharedPointer<Device> device, const int health = -1, const bool software = false);
    void addMasterSigner(const QSharedPointer<MasterSigner> &d);
    void updateMasterSignerList(const QSharedPointer<MasterSignerListModel> &d);
    void updateMasterSignerName(const QString &signerId, const QString &name);
    void updateDeviceStatus(const QString &fingerprint, const bool connected);
    QSharedPointer<MasterSigner> getMasterSignerByIndex(const int index);
    QSharedPointer<MasterSigner> getMasterSignerById(const QString& id);
    QString getMasterSignerNameByFingerPrint(const QString &fingerprint);
    bool removeMasterSigner(const QSharedPointer<MasterSigner> it);
    bool removeMasterSigner(const QString& masterSignerId);
    void resetUserChecked();
    void setUserChecked(const bool state, const int index);
    void setUserCheckedById(const bool state, const QString& id);
    bool contains(const QString &masterSignerId);
    bool containsFingerPrint(const QString &fingerprint);
    bool hardwareContainsFingerPrint(const QString &fingerprint);
    void updateDeviceNeedPinSent(const QString &fingerprint, const bool needpin);
    void updateDeviceNeedPassphraseSent(const QString &fingerprint, const bool needpassphrase);
    void updateDeviceNeedPassphraseSentById(const QString &id, const bool needpassphrase);
    QStringList getColdCardId() const;
    void requestSort(int role, int order);

    enum MasterSignerRoles {
        master_signer_id_Role,
        master_signer_name_Role,
        master_signer_device_Role,
        master_signer_checked_Role,
        master_signer_fingerPrint_Role,
        master_signer_msg2Sign_Role,
        master_signer_deviceType_Role,
        master_signer_isSoftware_Role,
        master_signer_need_passphrase_Role,
        master_signer_need_pin_Role
    };
private:
    QList<QSharedPointer<MasterSigner>> d_;
};

#endif // MASTERSIGNERMODEL_H
