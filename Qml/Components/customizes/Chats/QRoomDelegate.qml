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
import QtQuick.Controls 2.1
import QtGraphicalEffects 1.0
import HMIEVENTS 1.0
import EWARNING 1.0
import QRCodeItem 1.0
import "../../customizes"
import "../../origins"
import "../../customizes/Texts"
import "../../customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

Rectangle {
    id: roomRoot
    width: 321
    height: roomDelegateContent.height + 20

    property int  userCount: 6
    property int  unReadCount: 0
    property int  roomJoinState: 0
    property bool isCurrentItem: false
    property string contactAvt: ""
    property string contactname: "name"
    property string lasttimestamp: "lasttime"
    property string lastmessage: "lastmessage"
    property bool isEncrypted: false
    property bool walletReady: false
    property string usersTyping: ""
    property bool isTyping: usersTyping !== ""

    color: isCurrentItem ? Qt.rgba(255, 255, 255, 0.3) : "transparent"
    Rectangle {
        width: 4
        height: parent.height
        color: isCurrentItem ? "#FDD95C" : "transparent"
    }
    Row {
        id: roomDelegateContent
        width: parent.width
        anchors.verticalCenter: parent.verticalCenter
        spacing: 8
        Item { width: 8; height: parent.height}
        QAvatar {
            id: avatar
            width: 36
            height: 36
            userCount: roomRoot.userCount
            avatarUrl: contactAvt
            username: contactname
            displayStatus: userCount > 2
            anchors.verticalCenter: parent.verticalCenter
        }
        Column {
            width: parent.width - 90
            spacing: 4
            Item {
                width: parent.width
                height: 20
                QText{
                    width: parent.width*2/3
                    height: parent.height
                    font.family: "Lato"
                    text: contactname
                    font.pixelSize: 16
                    color: "#FFFFFF"
                    font.weight: Font.Bold
                    elide: Text.ElideRight
                    anchors.verticalCenter: parent.verticalCenter
                }
                QText{
                    anchors.right: parent.right
                    horizontalAlignment: Text.AlignRight
                    font.family: "Lato"
                    font.pixelSize: 12
                    color: "#FFFFFF"
                    text: lasttimestamp
                    anchors.verticalCenter: parent.verticalCenter
                    Rectangle{
                        width: 24
                        height: 24
                        radius: 24
                        color: "#CF4018"
                        visible: unReadCount > 0
                        anchors.right: parent.right
                        anchors.top: parent.bottom
                        anchors.topMargin: 4
                        QText{
                            font.family: "Lato"
                            font.pixelSize: 12
                            color: "#FFFFFF"
                            text: unReadCount
                            anchors.centerIn: parent
                        }
                    }
                }
            }
            Item {
                width: parent.width
                height: 24
                visible: walletReady || isEncrypted
                Row{
                    spacing: 4
                    height: 24
                    Rectangle{
                        width: 70
                        height: parent.height
                        radius: 20
                        color: "#EAEAEA"
                        anchors.verticalCenter: parent.verticalCenter
                        visible: walletReady
                        Row {
                            anchors.centerIn: parent
                            spacing: 4
                            QIcon {
                                iconSize: 12
                                source: "qrc:/Images/Images/collab-wallet-dark.svg"
                            }
                            QText{
                                font.family: "Lato"
                                font.pixelSize: 10
                                color: "#031F2B"
                                text: STR.STR_QML_438
                                font.weight: Font.Bold
                                anchors.verticalCenter: parent.verticalCenter
                            }
                        }
                    }
                    Rectangle{
                        width: 70
                        height: parent.height
                        radius: 20
                        color: "#EAEAEA"
                        anchors.verticalCenter: parent.verticalCenter
                        visible: false//isEncrypted
                        Row {
                            anchors.centerIn: parent
                            spacing: 4
                            QIcon {
                                iconSize: 12
                                source: "qrc:/Images/Images/secured-dark.svg"
                            }
                            QText{
                                font.family: "Lato"
                                font.pixelSize: 10
                                color: "#031F2B"
                                text: STR.STR_QML_675
                                font.weight: Font.Bold
                                anchors.verticalCenter: parent.verticalCenter
                            }
                        }
                    }
                }
            }
            Item {
                width: parent.width
                height: 16
                Loader {
                    width: parent.width*3/4
                    height: parent.height
                    sourceComponent: isTyping ? typingmessageComp : lastmessageComp
                }
            }
        }
    }
    Component {
        id: lastmessageComp
        QText {
            width: parent.width*3/4
            height: parent.height
            font.family: "Lato"
            font.pixelSize: 12
            color: "#FFFFFF"
            elide: Text.ElideRight
            text: lastmessage
            anchors.verticalCenter: parent.verticalCenter
        }
    }
    Component {
        id: typingmessageComp
        QText {
            width: parent.width*2/3
            height: parent.height
            font.family: "Lato"
            font.pixelSize: 12
            color: "#FFFFFF"
            elide: Text.ElideRight
            text: usersTyping
            anchors.verticalCenter: parent.verticalCenter
        }
    }
    signal itemClicked()
    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        onClicked: {
            if (mouse.button === Qt.RightButton) {
                optionMenu.functions = [function(){
                    if (walletReady) {
                        confirmDeleteRoom.indexRequest = index
                        confirmDeleteRoom.open()
                    } else {
                        ClientController.leaveRoom(index)
                    }                    
                }]
                optionMenu.popup()
            }
            else{
                itemClicked()
            }
        }
    }

    QContextMenu {
        id: optionMenu
        menuWidth: 180
        labels: [
            "Delete"
        ]
        colors: [
            "Red"
        ]
        icons: [
            "qrc:/Images/Images/Delete.svg"
        ]
        onItemClicked: {
            functions[index]()
        }
    }
}
