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
import QtQuick.Controls 1.4
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.12
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../Components/customizes/Chats"
import "../../Components/customizes/Signers"
import "../../../localization/STR_QML.js" as STR

QScreen {
    id: rootAddsignerToWallet
    property bool isPrimaryKeyFlow: NUNCHUCKTYPE.FLOW_PRIMARY_KEY === QMLHandle.currentFlow || NUNCHUCKTYPE.FLOW_REPLACE_PRIMARY_KEY === QMLHandle.currentFlow
    property string flow_screen: SignerManagement.screenFlow
    readonly property var map_flow: [
        {screen: "add-a-key",                           screen_component: add_a_key},
        {screen: "before-start-hardware",               screen_component: beforeStartHardware},
        {screen: "before-start-software",               screen_component: beforeStartSoftware},
    ]
    Loader {
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        sourceComponent: map_flow.find(function(e) {if (e.screen === flow_screen) return true; else return false}).screen_component
    }

    Component {
        id: add_a_key
        QAddAKey {

        }
    }

    Component {
        id: beforeStartHardware
        QBeforeStartHardware {

        }
    }

    Component {
        id: beforeStartSoftware
        QBeforeStartSoftware {
            onNextClicked: {
                if (optionSelected === "create-and-back-up-key-now") {
                    QMLHandle.sendEvent(EVT.EVT_ADD_NEW_SIGNER_SOFTWARE_SIGNER_NEW_SEED)
                } else if (optionSelected === "create-key-now-back-up_later") {
                    SignerManagement.createHotKey()
                } if (optionSelected === "recover-exist-seed") {
                    GlobalData.recoverSoftwareType = "seed"
                    QMLHandle.sendEvent(EVT.EVT_ADD_NEW_SIGNER_SOFTWARE_SIGNER_EXIST_SEED)
                } else if (optionSelected === "recover-key-via-xprv") {
                    GlobalData.recoverSoftwareType = "xprv"
                    QMLHandle.sendEvent(EVT.EVT_ADD_NEW_SIGNER_SOFTWARE_SIGNER_EXIST_SEED)
                }
            }
        }
    }
}
