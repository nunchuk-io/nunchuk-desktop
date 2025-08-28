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
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Chats"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../Components/customizes/Signers"
import "../../../Components/customizes/Wallets"
import "../../../Components/customizes/Popups"
import "../../../Components/customizes/Wallets/miniscript"
import "../../../../localization/STR_QML.js" as STR

Loader {
    property var walletInfo: AppModel.walletInfo
    property var transactionInfo: AppModel.transactionInfo
    property string flow_screen: transactionInfo.screenFlow
    readonly property var map_flow: [
        {screen: "choose-signing-path",         screen_component: choose_signing_path},
        {screen: "choose-signing-policy",       screen_component: choose_signing_policy},
    ]
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    sourceComponent: {
        var itemScreen = map_flow.find(function(e) {if (e.screen === flow_screen) return true; else return false})
        if (itemScreen) {
            return itemScreen.screen_component
        } else {
            return add_destination
        }
    }
    property var destinationDataInput
    Component {
        id: add_destination
        QNewTransactionAddDestination {
            onSelectedScriptPath: {
                transactionInfo.screenFlow = "choose-signing-path"
            }
            onSelectedScriptPolicy: {
                if(transactionInfo.numberOfPolices() >= 2) {
                    transactionInfo.screenFlow = "choose-signing-policy"
                } else {
                    QMLHandle.sendEvent(EVT.EVT_SEND_CREATE_TRANSACTION_REQUEST, destinationDataInput)
                }
            }
        }
    }

    Component {
        id: choose_signing_path
        QNewTransactionChooseSigningPath {
            onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
            onPrevClicked: transactionInfo.backScreen()
            onNextClicked: {
                if (selectPath === "keypath") {
                    destinationDataInput["use_script_path"] = false
                    QMLHandle.sendEvent(EVT.EVT_SEND_CREATE_TRANSACTION_REQUEST, destinationDataInput)
                } else {
                    if(transactionInfo.numberOfPolices() >= 2) {
                        transactionInfo.screenFlow = "choose-signing-policy"
                    } else {
                        QMLHandle.sendEvent(EVT.EVT_SEND_CREATE_TRANSACTION_REQUEST, destinationDataInput)
                    }
                }
            }
        }
    }
    Component {
        id: choose_signing_policy
        QNewTransactionChooseSigningPolicy {
            onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
            onPrevClicked: transactionInfo.backScreen()
            onNextClicked: {
                destinationDataInput["use_script_path"] = true
                QMLHandle.sendEvent(EVT.EVT_SEND_CREATE_TRANSACTION_REQUEST, destinationDataInput)
            }
        }
    }
}
