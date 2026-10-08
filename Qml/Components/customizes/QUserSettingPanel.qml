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
import "../origins"
import "../customizes"
import "../customizes/Chats"
import "../customizes/Texts"
import "../customizes/Buttons"
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
        spacing: 1
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
        Item{
            width: 97
            height: 24 + 8
            anchors.horizontalCenter: parent.horizontalCenter
            visible: ClientController.user.isSubscribedUser
            Rectangle{
                width: _row.childrenRect.width + 10*2
                height: 24
                radius: 20
                color: "#EAEAEA"
                anchors{
                    horizontalCenter: parent.horizontalCenter
                    bottom: parent.bottom
                    bottomMargin: 0
                }
                Row {
                    id:_row
                    anchors.centerIn: parent
                    spacing: 4
                    QIcon {
                        iconSize: 12
                        source: "qrc:/Images/Images/subscription-dark.svg"
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    QText{
                        font.family: "Lato"
                        font.pixelSize: 12
                        color: "#031F2B"
                        text: {
                            var user = ClientController.user
                            if (user.isMultiSubscriptions) {
                                return STR.STR_QML_1294
                            }
                            else if (user.isIronHandUser) {
                                return STR.STR_QML_680
                            }
                            else if (user.isHoneyBadgerUser) {
                                return STR.STR_QML_681
                            }
                            else if (user.isHoneyBadgerPlusUser) {
                                return STR.STR_QML_1503
                            }
                            else if (user.isHoneyBadgerPremierUser) {
                                return STR.STR_QML_1534
                            }
                            else if (user.isByzantineUserStandard) {
                                return STR.STR_QML_1003
                            }
                            else if (user.isByzantineUserPro) {
                                return STR.STR_QML_1003_Pro
                            }
                            else if (user.isByzantineUserPremier) {
                                return STR.STR_QML_1003_Premier
                            }
                            else if (user.isFinneyUserPro) {
                                return STR.STR_QML_1287
                            }
                            else if (user.isFinneyUserStandard) {
                                return STR.STR_QML_1286
                            }
                            return ""
                        }
                        font.weight: Font.Normal
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
            }
        }
    }
    QTextButton {
        width: 289
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
