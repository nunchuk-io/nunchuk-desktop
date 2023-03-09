#ifndef MASTERSIGNERMODEL_H
#define MASTERSIGNERMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include "DeviceModel.h"
#include "QOutlog.h"
#include "qUtils.h"

class MasterSigner : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString masterSignerId      READ id              NOTIFY idChanged)
    Q_PROPERTY(QString masterSignername    READ name            WRITE setName      NOTIFY nameChanged)
    Q_PROPERTY(QDevice* masterSignerDevice  READ device          NOTIFY deviceChanged)
    Q_PROPERTY(int masterSignerHealth      READ health          WRITE setHealth    NOTIFY healthChanged)
    Q_PROPERTY(QString fingerPrint         READ fingerPrint     NOTIFY fingerPrintChanged)
    Q_PROPERTY(QString message             READ message         WRITE setMessage   NOTIFY messageToSignChanged)
    Q_PROPERTY(QString messageSHA256       READ messageSHA256                      NOTIFY messageToSignChanged)
    Q_PROPERTY(QString signature           READ signature       WRITE setSignature NOTIFY signatureChanged)
    Q_PROPERTY(QString masterSignerPath    READ path               NOTIFY pathChanged)
    Q_PROPERTY(bool needPinSent            READ needPinSent        NOTIFY needPinSentChanged)
    Q_PROPERTY(bool needPassphraseSent     READ needPassphraseSent NOTIFY needPassphraseSentChanged)
    Q_PROPERTY(QString deviceType          READ deviceType         NOTIFY deviceTypeChanged)
    Q_PROPERTY(bool checked                READ checked        WRITE setChecked    NOTIFY checkedChanged) // For add signer to wallet
    Q_PROPERTY(bool needXpub               READ needXpub       NOTIFY isNeedXpubChanged)
    Q_PROPERTY(int  signerType             READ signerType     NOTIFY signerTypeChanged)
    Q_PROPERTY(QString passphrase          READ passphrase     NOTIFY passphraseChanged)
    Q_PROPERTY(bool isPrimaryKey           READ isPrimaryKey   WRITE setIsPrimaryKey NOTIFY isPrimaryKeyChanged)
public:
    MasterSigner();
    MasterSigner(const QString& id,
                 const QString& name,
                 const QDevicePtr device,
                 const int health = -1,
                 const int signertype = 0,
                 const nunchuk::PrimaryKey &key = nunchuk::PrimaryKey());
    ~MasterSigner();
    QString id() const;
    QString name() const;
    QDevice* device() const;
    QDevicePtr devicePtr() const;
    int health() const;
    bool connected() const;
    void setId(const QString& d);
    void setName(const QString& d);
    void setDevice(const QDevicePtr &d);
    void setHealth(const int d);
    QString fingerPrint() const;
    QString message();
    QString messageSHA256();
    void setMessage(const QString &message);
    QString signature() const;
    void setSignature(const QString &signature);
    QString path() const;
    void setPath(const QString &path);
    bool checked() const;                   // For add signer to wallet
    void setChecked(const bool checked);    // For add signer to wallet
    bool needPinSent() const;
    void setNeedPinSent(bool value);
    bool needPassphraseSent() const;
    void setNeedPassphraseSent(bool value);
    QString deviceType() const;
    bool needXpub() const;
    void setNeedXpub(bool needXpub);
    int signerType() const;
    void setSignerType(int signer_type);
    QString passphrase() const;
    void setPassphrase(const QString &passphrase);
    bool isPrimaryKey() const;
    void setIsPrimaryKey(bool isPrimaryKey);
    nunchuk::PrimaryKey primaryKey() const;
private:
    QString id_;
    QString name_;
    QDevicePtr device_;
    int health_;
    QString messageToSign_;
    QString signature_;
    QString path_;
    bool checked_;          // For add signer to wallet
    bool isNeedXpub_;
    int signer_type_;
    QString passphrase_;
    bool isPrimaryKey_;
    nunchuk::PrimaryKey primaryKey_;
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
typedef QSharedPointer<MasterSigner> QMasterSignerPtr;

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
    void addMasterSigner(const QString& id,
                         const QString& name,
                         const QDevicePtr device,
                         const int health = -1,
                         const int signertype = 0);
    void addMasterSigner(const QMasterSignerPtr &d);
    void renameById(const QString &id, const QString &name);
    void updateDeviceStatus(const QString &fingerprint, const bool connected);
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
    bool hardwareContainsFingerPrint(const QString &fingerprint);
    void updateDeviceNeedPinSent(const QString &fingerprint, const bool needpin);
    void updateDeviceNeedPassphraseSent(const QString &fingerprint, const bool needpassphrase);
    void updateDeviceNeedPassphraseSentById(const QString &id, const bool needpassphrase);
    void updateMasterSignerNeedXpubByXfp(const QString &xfp, const bool value);
    void updateMasterSignerNeedXpubById(const QString &id, const bool value);
    void resetMasterSignerNeedXpub();
    QStringList getColdCardId() const;
    void requestSort(int role, int order);
    QList<QMasterSignerPtr> fullList() const;
    void cleardata();
    nunchuk::PrimaryKey containPrimaryKey(const QString& fingerprint);

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
    std::vector<nunchuk::PrimaryKey> primaryKeys;
};
typedef QSharedPointer<MasterSignerListModel> QMasterSignerListModelPtr;

#endif // MASTERSIGNERMODEL_H
