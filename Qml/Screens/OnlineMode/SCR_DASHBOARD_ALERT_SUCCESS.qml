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
import "../OnlineMode/SetupWallets"
import "../OnlineMode/AddHardwareKeys"
import "../OnlineMode/ServerKeys"
import "../OnlineMode/Healths"
import "../OnlineMode/Inheritances"
import "../../Components/customizes/services"
import "../../../localization/STR_QML.js" as STR

QScreen {
    property int flow: AppModel.walletInfo.flow
    property string config_flow: GroupWallet.dashboardInfo.configFlow
    readonly property var map_flow: [
        {flow_action: AlertType.KEY_RECOVERY_REQUEST,        screen_component: _key_recovery_approved},
        {flow_action: AlertType.KEY_RECOVERY_APPROVED,       screen_component: _key_recovery_approved},
        {flow_action: AlertType.KEY_RECOVERY_SUCCESS,        screen_component: _tapsigner_recovered},
    ]

    Loader {
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        sourceComponent: map_flow.find(function(e) {if (e.flow_action === flow) return true; else return false}).screen_component
    }
    Component {
        id: _key_recovery_approved
        QKeyRecoveryResult {
            title: STR.STR_QML_1054
            description: STR.STR_QML_1055
        }
    }
    Component {
        id: _tapsigner_recovered
        QTapsignerRecovered {
            onSignalGotIt: {
                AppModel.showToast(0, STR.STR_QML_739, EWARNING.SUCCESS_MSG);
                closeTo(NUNCHUCKTYPE.CURRENT_TAB)
            }
        }
    }

}
