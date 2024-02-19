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
import NUNCHUCKTYPE 1.0
import QRCodeItem 1.0
import DataPool 1.0
import DRACO_CODE 1.0
import EWARNING 1.0
import "../../../origins"
import "../../../customizes"
import "../../../customizes/Chats"
import "../../../customizes/Texts"
import "../../../customizes/Buttons"
import "../../../customizes/Popups"
import "../../../../../localization/STR_QML.js" as STR

Item {
    property var highestPermissionAccount: GroupWallet.highestPermissionAccount
    property string role: highestPermissionAccount.role
    function hasUpdateSecurityQuestion()
    {
        var user = ClientController.user
        if (user.isIronHandUser || user.isHoneyBadgerUser) {
            return true
        } else if (user.isByzantineUser) {
            var highestPermissionAccount = GroupWallet.highestPermissionAccount
            var role = highestPermissionAccount.role
            return role === "MASTER"
        }
        return false;
    }

    function hasWalletAssited()
    {
        return AppModel.walletList.hasAssistedWallet()
    }

    Column {
        anchors.fill: parent
        anchors.margins: 24
        spacing: 10
        QText {
            font.family: "Lato"
            font.pixelSize: 28
            color: "#031F2B"
            font.weight: Font.Bold
            text: STR.STR_QML_698
        }
        Row {
            spacing: 0
            height: 48
            QText {
                width: 627-24
                text: STR.STR_QML_708
                color: "#031F2B"
                font.pixelSize: 16
                font.family: "Lato"
                anchors.verticalCenter: parent.verticalCenter
            }
            QIcon {
                iconSize: 24
                source: "qrc:/Images/Images/right-arrow-dark.svg"
                scale: arrow_key_recovery.containsMouse ? 1.1 : 1
                anchors.verticalCenter: parent.verticalCenter
                MouseArea {
                    id: arrow_key_recovery
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    onClicked: {
                        var screenObj = { "state_id" : EVT.STATE_ID_SCR_KEY_RECOVERY }
                        QMLHandle.sendEvent(EVT.EVT_REENTER_YOUR_PASSWORD_REQUEST,screenObj)
                    }
                }
            }
        }
        Row {
            spacing: 0
            height: 48
            visible: hasUpdateSecurityQuestion()
            QText {
                width: 627-24
                text: STR.STR_QML_1060
                color: "#031F2B"
                font.pixelSize: 16
                font.family: "Lato"
                anchors.verticalCenter: parent.verticalCenter
            }
            QIcon {
                iconSize: 24
                source: "qrc:/Images/Images/right-arrow-dark.svg"
                scale: arrow_update_security.containsMouse ? 1.1 : 1
                anchors.verticalCenter: parent.verticalCenter
                MouseArea {
                    id: arrow_update_security
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    onClicked: {
                        if (hasWalletAssited()) {
                            var screenObj = { "state_id" : EVT.STATE_ID_SCR_SETUP_SECURITY_QUESTION }
                            QMLHandle.sendEvent(EVT.EVT_REENTER_YOUR_PASSWORD_REQUEST,screenObj)
                            return;
                        }
                        _info1.title = STR.STR_QML_339
                        _info1.contentText = STR.STR_QML_839
                        _info1.open()
                    }
                }
            }
        }
    }
}
