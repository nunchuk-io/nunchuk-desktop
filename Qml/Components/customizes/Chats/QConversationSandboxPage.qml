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
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.12
import DataPool 1.0
import "../../origins"
import "../../customizes/Texts"
import "../../customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

Rectangle {
    id: contentItem
    width: 380
    height: 570
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
    property var sandboxWalletInfo: AppModel.groupWalletList.currentWallet
    Column {
        anchors.fill: parent
        Item {
            id: header
            width: parent.width
            height: 72
            Row {
                anchors.fill: parent
                anchors.margins: 16
                spacing: 12
                QIcon {
                    id: avatar
                    width: 36
                    height: 36
                    source: "qrc:/Images/Images/backgroundAvatar.svg"
                    anchors.verticalCenter: parent.verticalCenter
                    QIcon {
                        anchors.centerIn: parent
                        iconSize: 18
                        source: "qrc:/Images/Images/groups-dark.svg"
                    }
                    Rectangle {
                        anchors {
                            right: parent.right
                            bottom: parent.bottom
                            bottomMargin: -2
                        }
                        width: 16
                        height: 16
                        radius: 8
                        border.width: 1
                        border.color: "#FFFFFF"
                        color: "#031F2B"
                        QLato {
                            color: "#FFFFFF"
                            font.pixelSize: 10
                            font.weight: Font.Black
                            anchors.centerIn: parent
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            text: qsTr("%1").arg(sandboxWalletInfo.walletN)
                        }
                    }
                }
                Column {
                    width: 286
                    height: parent.height
                    anchors.verticalCenter: parent.verticalCenter
                    QLato {
                        text: sandboxWalletInfo.walletName
                        font.pixelSize: 18
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
            }
            Rectangle {
                width: 213
                height: 52
                radius: 8
                color: "#F5F5F5"
                anchors {
                    right: parent.right
                    rightMargin: 24
                    verticalCenter: parent.verticalCenter
                }

                Row {
                    anchors {
                        left: parent.left
                        leftMargin: 16
                    }
                    height: parent.height
                    width: parent.width
                    spacing: 12
                    QIcon {
                        iconSize: 20
                        source: "qrc:/Images/Images/fav_wallet-dark.svg"
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    Column {
                        width: _balance.paintedHeight
                        anchors.verticalCenter: parent.verticalCenter
                        QLato {
                            id: _balance
                            width: paintedHeight
                            text: qsTr("%1%2").arg(sandboxWalletInfo.walletBalance).arg(RoomWalletData.unitValue)
                            font.pixelSize: 16
                            font.weight: Font.Bold
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter
                        }

                        QLato {
                            width: paintedHeight
                            text: qsTr("%1/%2 %3").arg(sandboxWalletInfo.walletM).arg(sandboxWalletInfo.walletN).arg(STR.STR_QML_069);
                            font.pixelSize: 12
                            font.weight: Font.Normal
                            color: "#757575"
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                }
                QIcon {
                    iconSize: 20
                    source: "qrc:/Images/Images/arrow-right.svg"
                    anchors{
                        verticalCenter: parent.verticalCenter
                        right: parent.right
                        rightMargin: 16
                    }
                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        onClicked: {
                            SharedWallet.changeToWallet()
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
            height: 80
            QTextField {
                id: messageField
                width: parent.width - 24*2
                height: 48
                anchors {
                    horizontalCenter: parent.horizontalCenter
                    verticalCenter: parent.verticalCenter
                }
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
                    console.log(sandboxWalletInfo, messageField.text)
                    sandboxWalletInfo.startSendGroupMessage(messageField.text)
                    messageField.text = ""
                }
            }
        }
    }
}

