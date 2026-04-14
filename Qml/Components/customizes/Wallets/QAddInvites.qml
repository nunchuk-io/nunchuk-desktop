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
import "./../../origins"
import "./../../customizes"
import "./../../customizes/Chats"
import "./../../customizes/Texts"
import "./../../customizes/Buttons"
import "./../../customizes/Wallets/miniscript"
import "./../../../core/buttons"

Column {
    id: _invite
    spacing: 8
    Item {
        width: 360
        height: 24
        QLato {
            font.weight: Font.Bold
            font.pixelSize: 12
            text: STR.STR_QML_2143
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
        }
        QPlusButton {
            id: plusBtn
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            plusEnabled: true
            onAddClicked: {
                vm.onAddInvitesClicked()
            }
        }
    }
    function getColor(enabled, pressed) {
        return enabled ? (pressed ? "#595959" : "#031F2B") : "#B5B5B5";
    }
    
    Flickable {
        id: flick
        width: _invite.width
        height: 200
        flickableDirection: Flickable.VerticalFlick
        ScrollBar.vertical: ScrollBar { active: true }
        contentHeight: _colWalletConfig.childrenRect.height + 50
        contentWidth: _invite.width
        clip: true
        Column {
            id: _colWalletConfig
            width: _invite.width
            spacing: 8
            Repeater {
                id: repeater
                model: vm.invitesListJson
                Item {
                    width: 360
                    height: 44
                    QLato {
                        font.pixelSize: 14
                        text: modelData.recipient_email
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        color: "#595959"
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    QTextLink {
                        id: _link
                        font.underline: false
                        anchors.right: parent.right
                        anchors.verticalCenter: parent.verticalCenter
                        text: STR.STR_QML_1134
                        onTextClicked: {
                            vm.onRemoveInviteClicked(modelData.id)
                        }
                    }                    
                }
            }
        }
    }
}
