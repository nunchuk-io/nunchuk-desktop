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
import NUNCHUCKTYPE 1.0
import "../../Components/customizes"
import "../../Components/origins"
import "../../Components/customizes/Chats"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../../localization/STR_QML.js" as STR
QScreen {
    id: roots
    popupWidth: 600
    popupHeight: 540

    QOnScreenContent {
        id: contenCenter
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        label.text: STR.STR_QML_354
        onCloseClicked: {
            QMLHandle.sendEvent(EVT.EVT_PENDING_REQUEST_BACK)
        }
        TabView {
            id: tabselect
            width: 528
            height: 420
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 100

            Tab {
                title: STR.STR_QML_355
                Item {
                    Rectangle {
                        width: 528
                        height: 352
                        anchors.bottom: parent.bottom
                        anchors.horizontalCenter: parent.horizontalCenter
                        radius: 8
                        color: "Transparent"
                        border.color: "#EAEAEA"
                        QListView {
                            id: contactReceivedList
                            width: 504
                            height: 336
                            anchors.bottom: parent.bottom
                            anchors.horizontalCenter: parent.horizontalCenter
                            model: ClientController.contactsReceived
                            clip: true
                            spacing: 12
                            delegate: QContactReceivedDelegate {
                                contactname: model.name
                                contactemail: model.email
                                onItemAcceptClicked: {
                                    ClientController.acceptFriendRequest(model.id)
                                }
                                onItemIgnoreClicked: {
                                    ClientController.ignoreFriendRequest(model.id)
                                }
                            }
                            ScrollBar.vertical: ScrollBar { active: true }
                        }
                    }
                }
            }
            Tab {
                title: STR.STR_QML_356
                Item {
                    Rectangle {
                        width: 528
                        height: 352
                        anchors.bottom: parent.bottom
                        anchors.horizontalCenter: parent.horizontalCenter
                        radius: 8
                        color: "Transparent"
                        border.color: "#EAEAEA"
                        QListView {
                            id: contactSendList
                            width: 504
                            height: 336
                            anchors.bottom: parent.bottom
                            anchors.horizontalCenter: parent.horizontalCenter
                            model: ClientController.contactsSent
                            clip: true
                            spacing: 12
                            delegate: QContactSentDelegate {
                                contactname: model.name
                                contactemail: model.email
                                onItemIgnoreClicked: {
                                    ClientController.cancelFriendRequest(model.id)
                                }
                            }
                            ScrollBar.vertical: ScrollBar { active: true }
                        }
                    }
                }
            }

            style: TabViewStyle {
                frameOverlap: 1
                tab: Rectangle {
                    implicitWidth: tabselect.width/2
                    implicitHeight: 60
                    color: "transparent"
                    QText {
                        id: text
                        anchors.centerIn: parent
                        text: styleData.title
                        color: "#031F2B"
                        font.pixelSize: 24
                        font.weight: styleData.selected ? Font.ExtraBold: Font.Normal
                        font.family: "Lato"
                    }

                    Rectangle {
                        color: styleData.selected ? "#031F2B" : "#EAEAEA"
                        implicitWidth: tabselect.width/2
                        height: styleData.selected ? 2 : 1
                        anchors.bottom: parent.bottom
                    }
                }

                frame: Rectangle { color: "transparent" }
            }
        }
    }
}
