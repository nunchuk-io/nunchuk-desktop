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
import QtQuick.Controls 2.3
import QtGraphicalEffects 1.12
import Qt.labs.platform 1.1
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Chats"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../Components/customizes/QRCodes"
import "../../Components/customizes/Popups"
import "../../../localization/STR_QML.js" as STR

QScreen {
    id:_screen
    property bool isEditing: false
    QOnScreenContentTypeB {
        id:_content
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        minWidth: 539
        label.text: isEditing ? STR.STR_QML_1297: STR.STR_QML_1295
        extraHeader: Item {}
        onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
        content: Item {
            Rectangle {
                anchors.top: parent.top
                anchors.topMargin: 8
                border.width: 1
                border.color: "#EAEAEA"
                width: 539
                height: 468
                radius: 12
                QListView {
                    id: servers
                    anchors.fill: parent
                    model: ProfileSetting.mainnetServers
                    clip: true
                    ScrollBar.vertical: ScrollBar { active: true }
                    spacing: 16
                    delegate: Item {
                        width: 539
                        height: 48
                        Row {
                            anchors {
                                verticalCenter: parent.verticalCenter
                                left: parent.left
                                leftMargin: 18
                            }
                            spacing: 4
                            QIcon {
                                iconSize: 24
                                source: "qrc:/Images/Images/accounts-settings-dark.svg"
                                anchors.verticalCenter: parent.verticalCenter
                                visible: modelData.isLocal ? modelData.isLocal : false
                            }
                            QLato {
                                text: modelData.name
                                anchors.verticalCenter: parent.verticalCenter
                            }
                        }
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                ProfileSetting.mainnetIndex = index
                                AppSetting.mainnetServerSelected(modelData.url)
                                closeTo(NUNCHUCKTYPE.CURRENT_TAB)
                            }
                        }

                        QIcon{
                            anchors {
                                verticalCenter: parent.verticalCenter
                                right: parent.right
                                rightMargin: 12
                            }
                            iconSize: 24
                            source: {
                                var imgCheck = ProfileSetting.mainnetIndex === index ? "qrc:/Images/Images/check-dark.svg": ""
                                if (modelData.isLocal) {
                                    return isEditing ? "qrc:/Images/Images/close-dark.svg": imgCheck
                                } else {
                                    return imgCheck
                                }
                            }
                            visible: source !== ""
                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    if (isEditing) {
                                        var _input = {
                                            type: "enter-remove-server",
                                            serverAddress: modelData.url,
                                            serverName: modelData.name
                                        }
                                        QMLHandle.notifySendEvent(EVT.EVT_SELECT_SERVER_ENTER, _input)
                                    }
                                }
                            }
                        }
                    }
                }

            }
        }
        onPrevClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
        bottomRight: Row {
            spacing: 12
            QTextButton {
                width: 61
                height: 48
                label.text: isEditing ? STR.STR_QML_835 : STR.STR_QML_849
                label.font.pixelSize: 16
                type: eTypeB
                enabled: {
                    if (isEditing) return true
                    for(var i = 0; i < ProfileSetting.mainnetServers.length; i++) {
                        if (ProfileSetting.mainnetServers[i].isLocal) {
                            return true
                        }
                    }
                    return false
                }
                onButtonClicked: {
                    if (isEditing == false) {
                        isEditing = true
                    } else {
                        isEditing = false
                        var _input = {
                            type: "save-enter-edit"
                        }
                        QMLHandle.notifySendEvent(EVT.EVT_SELECT_SERVER_ENTER, _input)
                    }
                }
            }
            QTextButton {
                width: 111
                height: 48
                label.text: STR.STR_QML_1298
                label.font.pixelSize: 16
                type: eTypeB
                onButtonClicked: {
                    enterServerAddress.clearText()
                    enterServerAddress.open()
                }
            }
        }
    }

    QPopupEnterServerAddress {
        id: enterServerAddress
        onEnterServerAddress: {
            var _input = {
                type: "enter-add-server",
                serverAddress: address
            }
            QMLHandle.notifySendEvent(EVT.EVT_SELECT_SERVER_ENTER, _input)
        }
    }
}
