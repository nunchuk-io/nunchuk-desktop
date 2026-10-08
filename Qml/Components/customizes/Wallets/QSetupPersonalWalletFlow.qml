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
// Qt imports
import QtQuick 2.4
import QtQuick.Controls 2.3
import Qt.labs.platform 1.1
import QtGraphicalEffects 1.12

// Application-specific imports
import "../../../../localization/STR_QML.js" as STR
import "../../customizes"
import "../../customizes/Buttons"
import "../../customizes/Popups"
import "../../customizes/Signers"
import "../../customizes/Texts"
import "../../origins"

// Third-party or additional module imports
import DataPool 1.0
import EWARNING 1.0
import HMIEVENTS 1.0
import NUNCHUCKTYPE 1.0

Loader {
    property var newWalletInfo: AppModel.newWalletInfo
    property string flow_screen: newWalletInfo.screenFlow
    readonly property var map_flow: [
        {screen: "review-wallet",             screen_component: review_wallet},
    ]
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    sourceComponent: {
        var itemScreen = map_flow.find(function(e) {if (e.screen === flow_screen) return true; else return false})
        if (itemScreen) {
            return itemScreen.screen_component
        } else {
            return personal_Wallet
        }
    }
    function switchReviewWallet() {
        newWalletInfo.screenFlow = "review-wallet";
    }
    Component {
        id: personal_Wallet
        QSetupPersonalWallet {
            onNextClicked: {
                if (newWalletInfo.confirmAndAssignKeysToWallet()) {
                    switchReviewWallet();
                }
            }
        }
    }
    Component {
        id: review_wallet
        QDraftWalletReviewWallet {
            onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
            onPrevClicked: {
                newWalletInfo.backScreen()
            }
            nextEnabled: !newWalletInfo.walletSingleSignerAssigned.needTopUpXpubs
            onNextClicked: {
                AppModel.startCreateWallet(false, "");
                closeTo(NUNCHUCKTYPE.CURRENT_TAB)
            }
        }
    }
}