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
import "../../Components/customizes/QRCodes"
import "../OnlineMode/Inheritances"
import "../../../localization/STR_QML.js" as STR

QScreen {
    property int flow: ServiceSetting.claimInheritanceFlow
    readonly property var map_flow: [
        {flow_action: ServiceType.WITHDRAW_TO_ADDRESS,               screen_component: _widthdraw_to_address},
        {flow_action: ServiceType.WITHDRAW_TO_NUNCHUK_WALLET,        screen_component: _widthdraw_to_wallet},
    ]
    property var inheritanceInfo: ServiceSetting.servicesTag.inheritanceInfo
    function isValid()
    {
        var balance = inheritanceInfo.balanceSats // sats
        var minFee = parseInt(AppModel.minFee)*1000; // sats
        return balance <= minFee
    }
    Loader {
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        sourceComponent: map_flow.find(function(e) {if (e.flow_action === flow) return true; else return false}).screen_component
    }
    Component {
        id: _widthdraw_to_address
        QInheritanceWidthdrawToAddress {

        }
    }

    Component {
        id: _widthdraw_to_wallet
        QInheritanceWidthdrawToWallet {

        }
    }
}
