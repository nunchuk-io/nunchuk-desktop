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
import "./origins"
import "./customizes"
import "./customizes/Chats"
import "./customizes/Texts"
import "./customizes/Buttons"
import "./LeftPannel"
import "../../localization/STR_QML.js" as STR

Item {
    id: pannel
    property Component left_pannel
    width: 386
    height: parent.height
    transformOrigin: Item.Left
    Rectangle {
        id: bgColor
        anchors.fill: parent
        color: "#031F2B"
        visible: false
    }
    DropShadow {
        id: dropshodow1
        anchors.fill: bgColor
        radius: 24
        samples: 16
        color: Qt.rgba(0, 0, 0, 0.14)
        source: bgColor
    }
    Row {
        QMenuBar {
            width: 72
            height: pannel.height
            anchors.verticalCenter: parent.verticalCenter
            enables: [
                ClientController.isNunchukLoggedIn,
                true,
                true
            ]
            onLocalModeRequest: {
                if (closeRequest(NUNCHUCKTYPE.WALLET_TAB)) {
                    if(0 === AppModel.nunchukMode){
                        QMLHandle.sendEvent(EVT.EVT_STARTING_APPLICATION_LOCALMODE)
                    }
                    else{
                        QMLHandle.sendEvent(EVT.EVT_STARTING_APPLICATION_ONLINE_HWL)
                    }
                }
            }
            onOnlineModeRequest: {
                if (closeRequest(NUNCHUCKTYPE.CHAT_TAB)) {
                    QMLHandle.sendEvent(EVT.EVT_GOTO_HOME_CHAT_TAB)
                }
            }
            onOpenSettingRequest: {
                QMLHandle.sendEvent(EVT.EVT_GOTO_APP_SETTINGS_TAB)
            }
            onSignoutRequest: {
                ClientController.requestSignout()
            }
            onUpdateProfileRequest: {
                QMLHandle.sendEvent(EVT.EVT_ROOT_UPDATE_PROFILE)
            }
            onSigninRequest: {
                if (closeRequest(NUNCHUCKTYPE.CHAT_TAB)) {
                    QMLHandle.sendEvent(EVT.EVT_STARTING_APPLICATION_ONLINEMODE)
                }
            }
            onServiceRequest: {
                if (closeRequest(NUNCHUCKTYPE.SERVICE_TAB)) {
                    QMLHandle.sendEvent(EVT.EVT_GOTO_SERVICE_SETTING_TAB)
                }
            }
        }
        Loader {
            width: 314
            height: pannel.height
            anchors.verticalCenter: parent.verticalCenter
            sourceComponent: left_pannel
        }
    }
}

