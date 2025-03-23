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
import "../../customizes"
import "../../origins"
import "../../customizes/Texts"
import "../../customizes/Buttons"

Flickable {
    id: flickerRooms

    property int globalCurrentIndex: 0
    property int totalCount: roomlist.count + groupwallets.count
    signal itemClicked(var index)
    signal itemGroupWalletClick(var index)

    clip: true
    flickableDirection: Flickable.VerticalFlick
    interactive: true
    contentHeight: roomsDisplay.height
    ScrollBar.vertical: ScrollBar { active: true }
    Column {
        id: roomsDisplay
        width: parent.width
        QListView {
            id: roomlist
            clip: true
            width: parent.width
            height: contentHeight
            model: ClientController.rooms
            interactive: false
            currentIndex: ClientController.rooms ? 0 : ClientController.rooms.currentIndex
            delegate: QRoomDelegate {
                contactAvt: model.room_avatar
                userCount: model.users_count
                contactname: model.name
                roomJoinState: model.joinstate
                unReadCount: model.unreadCount
                isCurrentItem: index === flickerRooms.globalCurrentIndex
                lasttimestamp: model.lasttimestamp
                walletReady: model.wallet_ready
                usersTyping: model.typing_users
                lastmessage: model.last_message
                isEncrypted: model.is_encrypted
                onItemClicked: {
                    flickerRooms.globalCurrentIndex = index
                    ClientController.rooms.currentIndex = index
                    flickerRooms.itemClicked(index)
                }
            }
        }
        QListView {
            id: groupwallets
            clip: true
            width: parent.width
            height: contentHeight
            model: AppModel.groupWalletList
            currentIndex: 0
            interactive: false
            delegate: QRoomSandboxDelegate {
                userCount: model.wallet_N
                contactname: model.wallet_name
                unReadCount: model.wallet_unreadMessage
                isCurrentItem: index + roomlist.count === flickerRooms.globalCurrentIndex
                lasttimestamp: model.wallet_conversation.lasttimestamp
                lastmessage: model.wallet_conversation.lastMessage
                isEncrypted: false
                onItemClicked: {
                    flickerRooms.globalCurrentIndex = index + roomlist.count
                    AppModel.groupWalletList.currentIndex = index
                    itemGroupWalletClick(index)
                }
            }
        }
    }
}
