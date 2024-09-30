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
import QtQuick 2.4
import QtQuick.Controls 2.1
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.0
import NUNCHUCKTYPE 1.0
import "../origins"
import "../customizes"
import "../customizes/Chats"
import "../customizes/Texts"
import "../customizes/Buttons"
import "../../../localization/STR_QML.js" as STR

Rectangle {
    id: menubarroot
    width: 72
    height: 910
    color: "#031F2B"
    property alias labels: psContextMenu.labels
    property alias enables: psContextMenu.enables
    property string username: "Guest"
    property int currentId: AppModel.tabIndex
    property var icons: {
        var ls = [];
        ls.push({id:NUNCHUCKTYPE.WALLET_TAB,icon:"qrc:/Images/Images/wallet-light.svg"})
        ls.push({id:NUNCHUCKTYPE.SERVICE_TAB,icon:"qrc:/Images/Images/services-light.svg"})
        ls.push({id:NUNCHUCKTYPE.CHAT_TAB,icon:"qrc:/Images/Images/message-light.svg"})
        return ls
    }
    signal onlineModeRequest()
    signal localModeRequest()
    signal serviceRequest()
    signal openSettingRequest()
    signal signoutRequest()
    signal signinRequest()
    signal updateProfileRequest()
    signal createAccountRequest()
    Rectangle {
        width: 2
        height: menubarroot.height
        color: "#595959"
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
    }
    QImage {
        id: logo
        width: 32
        height: 32
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 24
        source: "qrc:/Images/Images/logo-light.svg"
    }
    Column {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: logo.bottom
        anchors.topMargin: 35
        Repeater {
            id: chatlist
            model: icons.length
            QMenuBarItem {
                width: 48
                height: 48
                isTinyShow: {
                    var show = false
                    try{
                        show = ClientController.rooms.totalUnread > 0 || ClientController.contactsReceived.count > 0
                    }catch(e){}
                    return icons[index].id === NUNCHUCKTYPE.CHAT_TAB && show
                }

                icon: icons[index].icon
                isCurrentItem: currentId === icons[index].id
                onButtonClicked: {
                    switch (icons[index].id) {
                    case NUNCHUCKTYPE.CHAT_TAB:
                        onlineModeRequest()
                        break;
                    case NUNCHUCKTYPE.WALLET_TAB:
                        localModeRequest()
                        break;
                    case NUNCHUCKTYPE.SERVICE_TAB:
                        serviceRequest()
                        break;
                    default:
                        break;
                    }
                }
            }
        }
    }
    Rectangle {
        id: avatarRect
        width: 48
        height: 48
        radius: 48
        color: userMouse.containsMouse ? Qt.rgba(255, 255, 255, 0.1) : "transparent"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 12
        QAvatar {
            id: userAvt
            width: 36
            height: 36
            avatarUrl: ClientController.isNunchukLoggedIn ? ClientController.user.avatar : ""
            username:  ClientController.isNunchukLoggedIn ? ClientController.user.name : STR.STR_QML_535
            displayStatus: false
            anchors.centerIn: parent
            MouseArea {
                id: userMouse
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                onClicked: {
                    psContextMenu.x = avatarRect.x + mouseX
                    psContextMenu.y = avatarRect.y - psContextMenu.height + mouseY
                    psContextMenu.open()
                }
            }
        }
    }
    QContextMenu {
        id: psContextMenu
        labels: [
            STR.STR_QML_460, // Update profile
            STR.STR_QML_461,
            ClientController.isNunchukLoggedIn ? STR.STR_QML_462 : STR.STR_QML_533,
            STR.STR_QML_412
        ]
        enables: [
            true,
            true,
            true,
            !ClientController.isNunchukLoggedIn,
        ]
        visibles:  [
            true,
            true,
            true,
            !ClientController.isNunchukLoggedIn,
        ]

        onItemClicked: {
            switch(index){
            case 0:// Update profile
                updateProfileRequest()
                break;
            case 1:// Setting
                openSettingRequest()
                break;
            case 2:// Signin/Signout
                ClientController.isNunchukLoggedIn ? signoutRequest() : signinRequest()
                break;
            case 3:// Create an account
                createAccountRequest()
                break;
            default:
                break;
            }
        }
    }
}
