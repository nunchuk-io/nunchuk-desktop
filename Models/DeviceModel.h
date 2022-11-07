#ifndef DEVICELISTMODEL_H
#define DEVICELISTMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include "QOutlog.h"

class QDevice : public QObject
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
    QDevice();
    QDevice(const QString& fingerprint);
    QDevice(const QString& name,
           const QString& type,
           const QString& path,
           const QString& model,
           const QString& master_fingerprint,
           bool connected,
           bool needs_pass_phrase_sent,
           bool needs_pin_sent,
           QString mastersigner_id = "");

    ~QDevice();
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
typedef QSharedPointer<QDevice> QDevicePtr;

class DeviceListModel : public QAbstractListModel
{
    Q_OBJECT
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
    void addDevice(const QDevicePtr &d);
    void updateDeviceList(const  QSharedPointer<DeviceListModel> &d);
    QDevicePtr getDeviceByIndex(const int index);
    QDevicePtr getDeviceByPath(const QString& path);
    QDevicePtr getDeviceByXFP(const QString& xfp);
    QDevicePtr getDeviceNeedPinSent();
    void resetDeviceConnected();
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
    void resetUsableToAdd();
    bool containsAddable() const;
    void updateUsableToAdd(const QString &fingerprint, bool value);

    // For verify addr
    QStringList getXFPList();
    bool containsNeedPinSent();
    void updateDeviceNeedPassphraseSentByMasterSignerId(const QString &id, bool needpassphrase);
    void notifySoftwareSignerRenamed(const QString &mastersignerid, const QString &newname);
    void checkAndUnlockDevice();
    int deviceCount() const;
    int getDeviceIndexByXFP(const QString& xfp);
    QList<QDevicePtr> fullList() const;
    bool contains(const QString &fingerprint);
    bool needScanDevice();
private:
    QList<QDevicePtr> d_;
signals:
    void containsAddableChanged();
    void deviceCountChanged();
};
typedef QSharedPointer<DeviceListModel> QDeviceListModelPtr;

#endif // DEVICELISTMODEL_H
