#ifndef DEVICELISTMODEL_H
#define DEVICELISTMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include "QWarningMessage.h"
#include "QOutlog.h"

class Device : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString deviceName                 READ name                   WRITE setName                 NOTIFY nameChanged                 )
    Q_PROPERTY(QString deviceType                 READ type                   WRITE setType                 NOTIFY typeChanged                 )
    Q_PROPERTY(QString devicePath                 READ path                   WRITE setPath                 NOTIFY pathChanged                 )
    Q_PROPERTY(QString deviceModel                READ model                  WRITE setModel                NOTIFY modelChanged                )
    Q_PROPERTY(QString deviceMasterFingerPrint    READ masterFingerPrint      WRITE setMasterFingerPrint    NOTIFY masterFingerPrintChanged    )
    Q_PROPERTY(bool deviceConnected               READ connected              WRITE setConnected            NOTIFY connectedChanged            )
    Q_PROPERTY(bool deviceNeedsPassPhraseSent     READ needsPassPhraseSent    WRITE setNeedsPassPhraseSent  NOTIFY needsPassPhraseSentChanged  )
    Q_PROPERTY(bool deviceNeedsPinSent            READ needsPinSent           WRITE setNeedsPinSent         NOTIFY needsPinSentChanged         )
    Q_PROPERTY(QString masterSignerId             READ masterSignerId         WRITE setMasterSignerId       NOTIFY masterSignerIdChanged)
public:
    Device();
    Device(const QString& fingerprint);
    Device(const QString& name,
           const QString& type,
           const QString& path,
           const QString& model,
           const QString& master_fingerprint,
           bool connected,
           bool needs_pass_phrase_sent,
           bool needs_pin_sent,
           QString mastersigner_id = "");

    ~Device();
    QString name() const;
    QString type() const;
    QString path() const;
    QString model() const;
    QString masterFingerPrint() const;
    bool connected() const;
    bool needsPassPhraseSent() const;
    bool needsPinSent() const;

    void setName(const QString &name);
    void setType(const QString& d);
    void setPath(const QString& d);
    void setModel(const QString& d);
    void setMasterFingerPrint(const QString& d);
    void setConnected(const bool d);
    void setNeedsPassPhraseSent(const bool d);
    void setNeedsPinSent(const bool d);

    bool usableToSign() const;
    void setUsableToSign(bool usableToSign);

    bool usableToAdd() const;
    void setUsableToAdd(bool usableToAdd);
    QString masterSignerId() const;
    void setMasterSignerId(const QString &master_signer_id);

private:
    QString name_;
    QString type_;
    QString path_;
    QString model_;
    QString master_fingerprint_;
    bool connected_;
    bool needs_pass_phrase_sent_;
    bool needs_pin_sent_;

    //Use for tx only
    bool usableToSign_;

    //Use for add signer
    bool usableToAdd_;

    //Software signer devices
    QString master_signer_id_;

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

class DeviceListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QWarningMessage* warningMessage  READ warningMessage NOTIFY warningMessageChanged)
    Q_PROPERTY(bool containsSignable    READ containsSignable   NOTIFY containsSignableChanged)
    Q_PROPERTY(bool containsAddable     READ containsAddable    NOTIFY containsAddableChanged)
    Q_PROPERTY(int  count               READ deviceCount        NOTIFY deviceCountChanged)
public:
    DeviceListModel();
    ~DeviceListModel();
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QHash<int,QByteArray> roleNames() const;

    void addDevice(const QString& name,
                   const QString& type,
                   const QString& path,
                   const QString& model,
                   const QString& master_fingerprint,
                   bool connected,
                   bool needs_pass_phrase_sent,
                   bool needs_pin_sent,
                   QString mastersigner_id = "");
    void addDevice(const QSharedPointer<Device> &d);
    void updateDeviceList(const  QSharedPointer<DeviceListModel> &d);
    QSharedPointer<Device> getDeviceByIndex(const int index);
    QSharedPointer<Device> getDeviceByPath(const QString& path);
    QSharedPointer<Device> getDeviceByXFP(const QString& xfp);
    QSharedPointer<Device> getDeviceNeedPinSent();
    void updateDeviceConnected(QString xfp);
    void resetDeviceConnected();
    QString getDevicePathByIndex(const int index);
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
        device_usableToSign_role,
        device_usableToAdd_role,
        device_master_signer_id_role,
    };

    QWarningMessage* warningMessage() const;
    QSharedPointer<QWarningMessage> warningMessagePtr() const;
    void setWarningMessage(const QSharedPointer<QWarningMessage> &warningMessage);

    // for tx only
    void resetUsableToSign();
    bool containsSignable() const;
    void updateUsableToSign(const QString &fingerprint, bool value);

    void resetUsableToAdd();
    bool containsAddable() const;
    void updateUsableToAdd(const QString &fingerprint, bool value);

    // For verify addr
    QStringList getXFPList();
    bool containsNeedPinSent();
    void updateDeviceNeedPassphraseSentByMasterSignerId(const QString &id, bool needpassphrase);
    void notifySoftwareSignerRenamed(const QString &mastersignerid, const QString &newname);

    int deviceCount() const;
private:
    QList<QSharedPointer<Device>> d_;
    QSharedPointer<QWarningMessage> warningMessage_;
    bool contains(const QString &fingerprint);
signals:
    void warningMessageChanged();
    void containsSignableChanged();
    void containsAddableChanged();
    void deviceCountChanged();
};

#endif // DEVICELISTMODEL_H
