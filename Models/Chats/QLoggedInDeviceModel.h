#ifndef QLOGGEDINDEVICEMODEL_H
#define QLOGGEDINDEVICEMODEL_H


#include <QAbstractListModel>
#include "DracoDefines.h"
class QLoggedInDeviceModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)
    Q_PROPERTY(int count        READ count                              NOTIFY countChanged)
public:
    QLoggedInDeviceModel();
    ~QLoggedInDeviceModel();
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QHash<int, QByteArray> roleNames() const;

    void syncDevices(QList<DracoDevice> data);
    void removeAll();
    enum DeviceRoles {
        Device_id,
        Device_name,
        Device_lastIP,
        Device_lastTs,
        Device_thisId,
    };
    int currentIndex() const;
    void setCurrentIndex(int currentIndex);
    int count() const;
private:
    int m_currentIndex;
    QList<DracoDevice> m_data;

signals:
    void currentIndexChanged();
    void countChanged();
};

typedef QSharedPointer<QLoggedInDeviceModel> QLoggedInDeviceModelPtr;

#endif // QLOGGEDINDEVICEMODEL_H
