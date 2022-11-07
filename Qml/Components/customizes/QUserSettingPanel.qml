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
import Qt.labs.platform 1.1
import HMIEVENTS 1.0
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Chats"
import "../../../localization/STR_QML.js" as STR

Column {
    width: 304
    spacing: 12
    QAvatar {
        width: 64
        height: 64
        avatarUrl: ClientController.user.avatar
        username:  ClientController.user.name
        displayStatus: false
        anchors.horizontalCenter: parent.horizontalCenter
    }
    Column {
        spacing: 0
        anchors.horizontalCenter: parent.horizontalCenter
        QText {
            anchors.horizontalCenter: parent.horizontalCenter
            horizontalAlignment: Text.AlignHCenter
            text: ClientController.user.name
            color: "#FFFFFF"
            font.family: "Lato"
            font.weight: Font.DemiBold
            font.pixelSize: 20
        }
        QText {
            anchors.horizontalCenter: parent.horizontalCenter
            horizontalAlignment: Text.AlignHCenter
            text: ClientController.user.email
            color: "#FFFFFF"
            font.family: "Lato"
            font.pixelSize: 16
            visible: !ClientController.user.isPrimaryKey
        }
        Item{
            width: 176
            height: 16
            anchors.horizontalCenter: parent.horizontalCenter
            visible: ClientController.user.isPrimaryKey
            Row{
                spacing: 6
                Rectangle{
                    width: 79
                    height: 16
                    radius: 20
                    color: "#FDD95C"
                    anchors.verticalCenter: parent.verticalCenter
                    QText {
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        text: STR.STR_QML_641
                        color: "#031F2B"
                        font.family: "Lato"
                        font.pixelSize: 10
                        font.weight: Font.DemiBold
                    }
                }
                QText {
                    anchors.verticalCenter: parent.verticalCenter
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    text: "XFP: " + ClientController.user.master_fingerprint
                    color: "#FFFFFF"
                    font.family: "Lato"
                    font.pixelSize: 16
                    font.capitalization: Font.AllUppercase
                }
            }
        }
    }
    QTextButton {
        width: 115
        height: 48
        anchors.horizontalCenter: parent.horizontalCenter
        label.text: STR.STR_QML_512
        label.font.pixelSize: 16
        type: eTypeC
        onButtonClicked: {
            QMLHandle.sendEvent(EVT.EVT_ROOT_UPDATE_PROFILE)
        }
    }
}
