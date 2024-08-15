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
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Chats"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../Components/customizes/Signers"
import "../OnlineMode/EditMembers"
import "../../Components/customizes/services"
import "../../../localization/STR_QML.js" as STR

QScreen {
    property var dashboardInfo: GroupWallet.dashboardInfo
    property string config_flow: "edit_members"
    readonly property var map_screens: [
        {screen_name: "edit_members",         screen_component: _edit_members},
        {screen_name: "edit_members_success", screen_component: _edit_members_success},
    ]

    Loader {
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        sourceComponent: map_screens.find(function(e) {if (e.screen_name === config_flow) return true; else return false}).screen_component
    }
    Connections {
        target: dashboardInfo
        onEditMembersSuccessChanged: {
            config_flow = "edit_members_success"
        }
    }
    Component {
        id: _edit_members
        QEditMembers {}
    }
    Component {
        id: _edit_members_success
        QEditMemberSuccess {

        }
    }
}
