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
import QtQuick.Controls 1.4
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.12
import HMIEVENTS 1.0
import EWARNING 1.0
import QRCodeItem 1.0
import DataPool 1.0
import NUNCHUCKTYPE 1.0
import "../../origins"
import "../../customizes"
import "../../customizes/Chats"
import "../../customizes/Texts"
import "../../customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

Rectangle {
    anchors.fill: parent
    color: "#FFFFFF"
    property var curUser: {
        var index = ClientController.contacts.currentIndex
        if(index > (ClientController.contacts.count - 1) || index < 0){
            index = 0
        }
        return ClientController.contacts.get(index)
    }
    signal send_a_messageClicked(var user)
    signal send_removeContact(var id)
    Column {
        id: coll
        width: 600
        anchors{
            top: parent.top
            topMargin: 96
            horizontalCenter: parent.horizontalCenter
        }
        spacing: 24
        QAvatar {
            id: avatarAccount
            anchors.horizontalCenter: parent.horizontalCenter
            width: 96
            height: 96
            avatarUrl: curUser.avatar
            username: curUser.name
            displayStatus: false
        }

        Item{
            width: Math.max(_name.paintedWidth,_email.paintedWidth)
            height: _name.paintedHeight + _email.paintedHeight
            anchors.horizontalCenter: parent.horizontalCenter
            Column{
                spacing: 4
                anchors.horizontalCenter: parent.horizontalCenter
                QText {
                    id:_name
                    height: paintedHeight
                    width: paintedWidth
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.family: "Montserrat"
                    font.pixelSize: 32
                    horizontalAlignment: Text.AlignHCenter
                    text: curUser.name
                    color: "#031F2B"
                }
                QText {
                    id:_email
                    height: paintedHeight
                    width: paintedWidth
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.family: "Lato"
                    font.pixelSize: 16
                    horizontalAlignment: Text.AlignHCenter
                    text: curUser.email
                    color: "#031F2B"
                }
            }
        }
        Item {
            width: row1.childrenRect.width
            height: _col1.childrenRect.height
            anchors.horizontalCenter: parent.horizontalCenter
            Row{
                id:row1
                spacing: 16
                Item{
                    width: 142
                    height: _col1.childrenRect.height
                    Column{
                        id:_col1
                        spacing: 8
                        anchors.horizontalCenter: parent.horizontalCenter
                        QIconButton{
                            id:_message
                            anchors.horizontalCenter: parent.horizontalCenter
                            width: 48
                            height: 48
                            icon: "qrc:/Images/Images/send_a_message.svg"
                            onClicked: {
                                send_a_messageClicked(curUser)
                            }
                        }
                        QText {
                            height: paintedHeight
                            width: paintedWidth
                            anchors.horizontalCenter: parent.horizontalCenter
                            font.family: "Lato"
                            font.pixelSize: 16
                            horizontalAlignment: Text.AlignHCenter
                            text: STR.STR_QML_602
                            color: "#031F2B"
                        }
                    }
                }
                Item{
                    width: 142
                    height: _col2.childrenRect.height
                    Column{
                        id:_col2
                        spacing: 8
                        anchors.horizontalCenter: parent.horizontalCenter
                        QIconButton{
                            id:_more
                            anchors.horizontalCenter: parent.horizontalCenter
                            width: 48
                            height: 48
                            icon: "qrc:/Images/Images/more_options.svg"
                            onClicked: {
                                optionMenu.popup()
                            }
                        }
                        QText {
                            height: paintedHeight
                            width: paintedWidth
                            anchors.horizontalCenter: parent.horizontalCenter
                            font.family: "Lato"
                            font.pixelSize: 16
                            horizontalAlignment: Text.AlignHCenter
                            text: STR.STR_QML_444
                            color: "#031F2B"
                        }
                    }
                }
            }
        }

    }
    QContextMenu {
        id: optionMenu
        menuWidth: 180
        icons: [
            "qrc:/Images/Images/Delete.svg"
        ]
        labels: [
            STR.STR_QML_374
        ]
        colors: [
            "#CF4018"
        ]
        onItemClicked: {
            switch(index){
            case 0:
            default:
                send_removeContact(curUser.id)
                break;
            }
        }
    }
}
