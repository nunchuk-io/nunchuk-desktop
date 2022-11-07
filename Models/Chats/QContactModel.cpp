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
#include "QContactModel.h"
#include "QOutlog.h"
#include "Draco.h"

QContactModel::QContactModel(): m_currentIndex(0)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

QContactModel::~QContactModel()
{

}

int QContactModel::rowCount(const QModelIndex &parent) const
{
    return m_data.count();
}

QVariant QContactModel::data(const QModelIndex &index, int role) const
{
    switch (role) {
    case Contact_id:
        return m_data[index.row()].id;
    case Contact_name:
        return m_data[index.row()].name;
    case Contact_email:
        return m_data[index.row()].email;
    case Contact_gender:
        return m_data[index.row()].gender;
    case Contact_avatar:
        return m_data[index.row()].avatar;
    case Contact_status:
        return m_data[index.row()].status;
    case Contact_status_text:
        return m_data[index.row()].status_text;
    case Contact_chat_id:
        return m_data[index.row()].chat_id;
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> QContactModel::roleNames() const
{
    QHash<int, QByteArray> names;
    names[Contact_id]           = "id";
    names[Contact_name]         = "name";
    names[Contact_email]        = "email";
    names[Contact_gender]       = "gender";
    names[Contact_avatar]       = "avatar";
    names[Contact_status]       = "status";
    names[Contact_status_text]  = "status_text";
    names[Contact_chat_id]      = "chat_id";
    return names;
}

void QContactModel::syncContacts(QList<DracoUser> data)
{
    beginResetModel();
    m_data.clear();
    m_data = data;
    endResetModel();
    emit countChanged();
}

void QContactModel::addContact(const DracoUser user)
{
    beginResetModel();
    m_data.append(user);
    endResetModel();
    emit countChanged();
}

DracoUser QContactModel::getContactByIndex(const int index)
{
    if(0 <= index && index < m_data.count() ){
        return m_data[index];
    }
    else{
        return DracoUser();
    }
}

QStringList QContactModel::contactsByStringList()
{
    QStringList ret;
    ret.clear();
    foreach (DracoUser it, m_data) {
        ret << QString("%1|%2|%3|%4").arg(it.id).arg(it.name).arg(it.email).arg(it.chat_id);
    }
    return ret;
}

QString QContactModel::getContactNameByChatId(const QString &id)
{
    foreach (DracoUser it, m_data) {
        if(0 == QString::compare(it.chat_id, id, Qt::CaseInsensitive)){
            return it.name;
        }
    }
    return "";
}

void QContactModel::removeContact(const QString &contact_id)
{
    if(contact_id != ""){
        Draco::instance()->removeContact(contact_id);
        Draco::instance()->refreshContacts();
    }
}

void QContactModel::removeAll()
{
    beginResetModel();
    m_data.clear();
    emit countChanged();
}

DracoUser QContactModel::currentUser()
{
    if(0 <= m_currentIndex && m_currentIndex < m_data.count() ){
        return m_data[m_currentIndex];
    }
    else{
        return DracoUser();
    }
}

QVariant QContactModel::get(int row)
{
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

void QContactModel::acceptFriendRequest(const QString& contact_id)
{
    if(contact_id != ""){
        Draco::instance()->accecptFriendRequest(contact_id);
        Draco::instance()->refreshContacts();
    }
}

void QContactModel::ignoreFriendRequest(const QString &contact_id)
{
    if(contact_id != ""){
        Draco::instance()->ignoreFriendRequest(contact_id);
        Draco::instance()->refreshContacts();
    }
}

void QContactModel::cancelFriendRequest(const QString &contact_id)
{
    if(contact_id != ""){
        Draco::instance()->cancelFriendRequest(contact_id);
        Draco::instance()->refreshContacts();
    }
}

bool QContactModel::containsUserId(const QString &userid)
{
    foreach (DracoUser it, m_data) {
        if(0 == QString::compare(it.chat_id, userid, Qt::CaseInsensitive)){
            return true;
        }
    }
    return false;
}

int QContactModel::currentIndex() const
{
    return m_currentIndex;
}

void QContactModel::setCurrentIndex(int currentIndex)
{
    if(m_currentIndex != currentIndex){
        m_currentIndex = currentIndex;
        emit currentIndexChanged();
    }
}

int QContactModel::count() const
{
    return rowCount();
}
