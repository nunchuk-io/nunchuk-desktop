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
#include "QNunchukRoomUsers.h"
#include "connection.h"
#include "uriresolver.h"
#include "csapi/joining.h"
#include "csapi/leaving.h"
#include "csapi/room_send.h"
#include "events/reactionevent.h"
#include "events/redactionevent.h"
#include "events/simplestateevents.h"
#include "events/roommessageevent.h"
#include <functional>
#include "QOutlog.h"

QNunchukRoomUsers::QNunchukRoomUsers(Room *r): m_room(r)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
    if(m_room){
        QQmlEngine::setObjectOwnership(m_room, QQmlEngine::CppOwnership);
        connect(m_room, &Room::userAdded, this, &QNunchukRoomUsers::refresh);
        connect(m_room, &Room::userRemoved, this, &QNunchukRoomUsers::refresh);
    }
}

QNunchukRoomUsers::~QNunchukRoomUsers()
{

}

int QNunchukRoomUsers::rowCount(const QModelIndex &parent) const
{
    return m_room ? m_room->users().count() : 0;
}

QVariant QNunchukRoomUsers::data(const QModelIndex &index, int role) const
{
    switch (role) {
    case user_id:
        return m_room ? m_room->users().at(index.row())->id() : QVariant();
    case user_name:
        return m_room ? m_room->users().at(index.row())->displayname(m_room) : QVariant();
    case user_avatar:
        return m_room ? m_room->users().at(index.row())->avatarMediaId() : QVariant();
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> QNunchukRoomUsers::roleNames() const
{
    QHash<int, QByteArray> names;
    names[user_id]     = "id";
    names[user_name]   = "name";
    names[user_avatar] = "avatar";
    return names;
}

QVariant QNunchukRoomUsers::get(int row) {
    QHash<int,QByteArray> names = roleNames();
    QHashIterator<int, QByteArray> i(names);
    QVariantMap res;
    while (i.hasNext()) {
        i.next();
        QModelIndex idx = index(row, 0);
        QVariant data = idx.data(i.key());
        res[i.value()] = data;
    }
    return QVariant::fromValue(res);
}

QString QNunchukRoomUsers::change() const
{
    return "";
}

void QNunchukRoomUsers::refresh()
{
    beginResetModel();
    endResetModel();
    emit changeChanged();
}

bool QNunchukRoomUsers::HasContact(const QString &id)
{
    for(User* user:m_room->users()){
        if(user && user->id().contains(id)){
            return true;
        }
    }
    return false;
}
