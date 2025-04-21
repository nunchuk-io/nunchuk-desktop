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
#include "STATE_ID_SCR_ONLINE_ADD_CONTACTS.h"
#include "QEventProcessor.h"
#include "Servers/Draco.h"
#include "Chats/ClientController.h"

void SCR_ONLINE_ADD_CONTACTS_Entry(QVariant msg) {
    CLIENT_INSTANCE->refreshContacts();
}

void SCR_ONLINE_ADD_CONTACTS_Exit(QVariant msg) {

}

void EVT_ONLINE_ADD_CONTACTS_SEARCH_ID_HANDLER(QVariant msg) {

}

void EVT_ONLINE_ADD_CONTACTS_SEND_INVITATION_HANDLER(QVariant msg) {
    QStringList emails = msg.toStringList();
    if(emails.count() <= 0){
        return;
    }
    else if(emails.count() == 1){
        bool ret = Draco::instance()->requestSingleFriend(emails.at(0));
        if(ret){
            Draco::instance()->refreshContacts();
            QEventProcessor::instance()->sendEvent(E::EVT_ONLINE_ADD_CONTACTS_BACK);
        }
    }
    else{
        bool ret = Draco::instance()->requestSingleFriend(emails.at(0));
        if(ret){
            Draco::instance()->refreshContacts();
            QEventProcessor::instance()->sendEvent(E::EVT_ONLINE_ADD_CONTACTS_BACK);
        }
    }
}

void EVT_ONLINE_ADD_CONTACTS_BACK_HANDLER(QVariant msg) {

}

