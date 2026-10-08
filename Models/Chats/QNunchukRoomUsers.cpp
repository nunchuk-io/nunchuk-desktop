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
        connect(m_room, &Room::memberListChanged, this, &QNunchukRoomUsers::refresh);
    }
}

QNunchukRoomUsers::~QNunchukRoomUsers()
{

}

int QNunchukRoomUsers::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_room ? m_room->joinedMembers().count() : 0;
}

QVariant QNunchukRoomUsers::data(const QModelIndex &index, int role) const
{
    if(!m_room || !index.isValid()){
        return {};
    }
    const auto members = m_room->joinedMembers();
    if(index.row() < 0 || index.row() >= members.count()){
        return {};
    }
    const RoomMember& member = members.at(index.row());
    switch (role) {
    case user_id:
        return member.id();
    case user_name:
        return member.displayName();
    case user_avatar:
        return member.avatarMediaId();
    case user_is_local:
        return member.id() == m_room->localMember().id();
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
    names[user_is_local] = "isLocal";
    return names;
}

QVariant QNunchukRoomUsers::get(int row) {
    if(row < 0 || row >= rowCount()){
        return {};
    }
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
    if(!m_room){
        return false;
    }
    for(const RoomMember& user : m_room->joinedMembers()){
        if(user.id() == id){
            return true;
        }
    }
    return false;
}
