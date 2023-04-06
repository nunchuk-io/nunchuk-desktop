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
