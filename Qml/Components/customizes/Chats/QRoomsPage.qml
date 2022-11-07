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
import QtQuick 2.12
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0
import HMIEVENTS 1.0
import EWARNING 1.0
import QRCodeItem 1.0
import "../../../Components/customizes"
import "../../../Components/origins"

QListView {
    id: roomlist
    clip: true
    model: ClientController.rooms
    currentIndex: ClientController.rooms.currentIndex
    signal itemClicked(var index)
    delegate: QRoomDelegate {
        contactAvt: model.room_avatar
        userCount: model.users_count
        contactname: model.name
        roomJoinState: model.joinstate
        unReadCount: model.unreadCount
        isCurrentItem: index === roomlist.currentIndex
        lasttimestamp: model.lasttimestamp
        walletReady: model.wallet_ready
        usersTyping: model.typing_users
        lastmessage: model.last_message
        isEncrypted: model.is_encrypted
        onItemClicked: {
            ClientController.rooms.currentIndex = index
            roomlist.itemClicked(index)
        }
    }
    ScrollBar.vertical: ScrollBar { id: scrollContact; active: true ; function wheel(up){if(up){decrease()}else{increase()}}}
    MouseArea { anchors.fill: parent;z: 10;propagateComposedEvents: true;onWheel: { scrollContact.wheel(wheel.angleDelta.y > 0);}}
}
