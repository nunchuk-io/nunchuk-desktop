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
import QtQuick 2.0
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.12
import "../../origins"
import "../../customizes/Texts"
import "../../customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

Item {
    id: bubbleRoot
    property var sandboxWalletInfo: AppModel.walletInfo
    property bool isShowing: sandboxWalletInfo.showbubbleChat
    width: bubleType.width
    height: bubleType.height
    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onEntered: {
            bubbleRoot.focus = true
        }
        onClicked: {}
    }
    Loader {
        id: bubleType
        sourceComponent: isShowing ? bubleChatShown : bubleChatHidden
    }
    Component {
        id: bubleChatHidden
        Item {
            width: 56
            height: 56
            Item {
                width: 56
                height: 56
                layer.enabled: true
                layer.effect: DropShadow {
                    width: 56
                    height: 56
                    horizontalOffset: 3
                    verticalOffset: 3
                    radius: 8.0
                    samples: 17
                    color: "#80000000"
                }
                Rectangle {
                    width: 56
                    height: 56
                    color: "#031F2B"
                    radius: 44
                    QImage {
                        width: 24
                        height: 24
                        anchors.centerIn: parent
                        source: "qrc:/Images/Images/message-dark.png"
                    }
                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        onClicked: {
                            sandboxWalletInfo.showbubbleChat = true
                        }
                    }
                }
            }
            Rectangle {
                width: 24
                height: 24
                radius: 24
                color: "red"
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.topMargin: -height/3
                anchors.rightMargin: -width/3
                visible: sandboxWalletInfo.unreadMessage > 0
                QText {
                    anchors.centerIn: parent
                    text: sandboxWalletInfo.unreadMessage
                    color: "#FFFFFF"
                    font.family: "Lato"
                    font.pixelSize: 12
                }
            }
        }
    }

    Component {
        id: bubleChatShown
        Rectangle {
            id: contentItem
            width: 380
            height: 570
            radius: 24
            layer.enabled: true
            layer.effect: DropShadow {
                width: 380
                height: 570
                horizontalOffset: 3
                verticalOffset: 3
                radius: 8.0
                samples: 17
                color: "#80000000"
                source: contentItem
            }
            Column {
                anchors.fill: parent
                Item {
                    id: header
                    width: parent.width
                    height: 72
                    Row {
                        anchors.fill: parent
                        anchors.margins: 16
                        spacing: 33
                        Column {
                            width: 286
                            height: parent.height
                            spacing: 4
                            QLato {
                                text: "Chat"
                                font.pixelSize: 18
                                color: "#031F2B"
                                font.weight: Font.Bold
                                horizontalAlignment: Text.AlignLeft
                                verticalAlignment: Text.AlignVCenter
                            }
                            Row {
                                width: parent.width
                                height: 20
                                spacing: 4
                                QImage {
                                    width: 16
                                    height: 16
                                    source: "qrc:/Images/Images/Encrypted.png"
                                    anchors.verticalCenter: parent.verticalCenter
                                }
                                QLato {
                                    text: STR.STR_QML_675
                                    font.pixelSize: 12
                                    color: "#757575"
                                    horizontalAlignment: Text.AlignLeft
                                    verticalAlignment: Text.AlignVCenter
                                    anchors.verticalCenter: parent.verticalCenter
                                }
                            }
                        }
                        QImage {
                            width: 24
                            height: 24
                            source: "qrc:/Images/Images/CaretDown.png"
                            anchors.verticalCenter: parent.verticalCenter
                            MouseArea {
                                hoverEnabled: true
                                cursorShape: Qt.PointingHandCursor
                                anchors.fill: parent
                                onClicked: {
                                    sandboxWalletInfo.showbubbleChat = false
                                }
                            }
                        }
                    }
                    Rectangle {
                        width: parent.width
                        height: 1
                        color: "#DEDEDE"
                        anchors.bottom: parent.bottom
                    }
                }
                Item {
                    id: body
                    width: parent.width
                    height: parent.height - header.height - footer.height
                    QListView {
                        id: listView
                        width: parent.width - 32
                        height: parent.height
                        anchors.horizontalCenter: parent.horizontalCenter
                        clip: true
                        focus: true
                        model: sandboxWalletInfo.conversations
                        spacing: 16
                        cacheBuffer: 1000
                        // currentIndex: (count - 1)
                        // highlightFollowsCurrentItem: true
                        snapMode: ListView.NoSnap
                        delegate: QMessageBubbleDelegate {
                            width: listView.width
                            layoutDirection : isReceive ? Qt.LeftToRight : Qt.RightToLeft
                            isReceive       : model.group_message_receive
                            messageContent  : model.group_message_content
                            sendername      : model.group_message_signer
                        }
                        section
                        {
                            property: "group_message_time"
                            criteria: ViewSection.FullString
                            delegate: Item {
                                width: listView.width
                                height: 32
                                QLato {
                                    height: 16
                                    font.pixelSize: 12
                                    color: "#757575"
                                    text: section
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                }
                            }
                        }
                        ScrollBar.vertical: ScrollBar { id: scrollContact;
                            active: true;
                            function wheel(up)
                            {
                                if(up){
                                    //                        decrease();
                                }else{
                                    //                        increase();
                                    sandboxWalletInfo.markFiveMessagesAsRead()
                                }
                            }
                        }
                        onFlickEnded: {
                            if (listView.verticalVelocity > 0) {
                                sandboxWalletInfo.markFiveMessagesAsRead()
                            }
                        }
                        Component.onCompleted: {
                            listView.scrollToEnd()
                        }
                        Connections {
                            target: sandboxWalletInfo.conversations
                            function onGroupMessageAdded(index) {listView.scrollToEnd()}
                            function onGroupMessageDownloaded() {listView.scrollToEnd()}
                        }
                        function scrollToEnd() {
                            Qt.callLater(function() {
                                listView.forceLayout()
                                listView.positionViewAtIndex((listView.count - 1), ListView.End)
                                sandboxWalletInfo.markAllMessagesAsRead()
                            });
                        }
                    }
                }
                Item {
                    id: footer
                    width: parent.width
                    height: 72
                    Row {
                        anchors.centerIn: parent
                        spacing: 16
                        QTextField {
                            id: messageField
                            width: 301
                            height: 40
                            anchors.verticalCenter: parent.verticalCenter
                            placeholderText: "Type your message..."
                            clip: true
                            color: "#031F2B"
                            font.pixelSize: 16
                            selectByMouse: true
                            background: Rectangle {
                                anchors.fill: parent
                                radius: 8
                                border.color: "#DEDEDE"
                                color: "#FFFFFF"
                            }
                            onActiveFocusChanged:{ sandboxWalletInfo.markFiveMessagesAsRead() }
                            Keys.onReturnPressed:{messageField.sendRequest()}
                            Keys.onEnterPressed: {messageField.sendRequest()}
                            function sendRequest(){
                                sandboxWalletInfo.startSendGroupMessage(messageField.text)
                                messageField.text = ""
                            }
                        }
                        QImage {
                            width: 24
                            height: 24
                            source: messageField.text === "" ? "qrc:/Images/Images/SendMessage.png" : "qrc:/Images/Images/SendMessage-dark.png"
                            anchors.verticalCenter: parent.verticalCenter
                            MouseArea {
                                hoverEnabled: true
                                cursorShape: Qt.PointingHandCursor
                                anchors.fill: parent
                                onClicked: {
                                    if(listView.count === 0){
                                        if(sandboxWalletInfo.conversations){
                                            sandboxWalletInfo.startDownloadConversation()
                                        }
                                    }
                                    messageField.sendRequest()
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
