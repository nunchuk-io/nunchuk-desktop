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
#include "QLoggedInDeviceModel.h"
#include "QOutlog.h"
#include "Servers/Draco.h"

QLoggedInDeviceModel::QLoggedInDeviceModel(): m_currentIndex(0)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

QLoggedInDeviceModel::~QLoggedInDeviceModel()
{

}

int QLoggedInDeviceModel::rowCount(const QModelIndex &parent) const
{
    return m_data.count();
}

QVariant QLoggedInDeviceModel::data(const QModelIndex &index, int role) const
{
    switch (role) {
    case Device_id:
        return m_data[index.row()].id;
    case Device_name:
        return m_data[index.row()].name;
    case Device_lastIP:
        return m_data[index.row()].lastIP;
    case Device_lastTs:
        return m_data[index.row()].lastTs;
    case Device_thisId:
        return m_data[index.row()].thisId;
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> QLoggedInDeviceModel::roleNames() const
{
    QHash<int, QByteArray> names;
    names[Device_id]           = "id";
    names[Device_name]         = "name";
    names[Device_lastIP]       = "lastIP";
    names[Device_lastTs]       = "lastTs";
    names[Device_thisId]       = "thisId";
    return names;
}

void QLoggedInDeviceModel::syncDevices(QList<DracoDevice> data)
{
    beginResetModel();
    m_data.clear();
    m_data = data;
    endResetModel();
    emit countChanged();
}

void QLoggedInDeviceModel::removeAll()
{
    beginResetModel();
    m_data.clear();
    emit countChanged();
}

int QLoggedInDeviceModel::currentIndex() const
{
    return m_currentIndex;
}

void QLoggedInDeviceModel::setCurrentIndex(int currentIndex)
{
    if(m_currentIndex != currentIndex){
        m_currentIndex = currentIndex;
        emit currentIndexChanged();
    }
}

int QLoggedInDeviceModel::count() const
{
    return rowCount();
}
