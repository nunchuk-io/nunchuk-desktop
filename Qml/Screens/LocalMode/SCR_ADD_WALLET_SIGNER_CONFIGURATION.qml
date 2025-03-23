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
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../Components/customizes/Popups"
import "../../Components/customizes/Signers"
import "../../Components/customizes/Wallets"
import "../../../localization/STR_QML.js" as STR

QScreen {
    readonly property var newWalletInfo: AppModel.newWalletInfo
    readonly property int walletOptType: AppModel.newWalletInfo.walletOptType
    readonly property var map_opt: [
        {walletOptType: NUNCHUCKTYPE.E_PERSONAL_WALLET,     screen_component: funPersonalWallet()},
    ]

    function funPersonalWallet() {
        if (newWalletInfo.walletAddressType === NUNCHUCKTYPE.TAPROOT) {
            return taproot_personal_Wallet
        } else {
            return personal_Wallet
        }
    }

    Loader {
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        sourceComponent: map_opt.find(function(e) {if (e.walletOptType === walletOptType) return true; else return false}).screen_component
    }

    Component {
        id: personal_Wallet
        QSetupPersonalWallet {

        }
    }
    Component {
        id: taproot_personal_Wallet
        QSetupPersonalTaprootWallet {

        }
    }
}
