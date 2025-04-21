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
import DRACO_CODE 1.0
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Chats"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../Components/customizes/Popups"
import "../../../localization/STR_QML.js" as STR

QScreen {
    property var primaryKeyUsername: ""
    property var primaryKeyPassphrase: ""
    property var primaryKeySignername: ""

    QOnScreenContent {
        id:_contentPrimaryKey
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        label.text: STR.STR_QML_638
        onCloseClicked: {
            QMLHandle.sendEvent(EVT.EVT_ONS_CLOSE_ALL_REQUEST)
        }
        Column{
            spacing: 24
            anchors {
                left: parent.left
                leftMargin: 36
                top: parent.top
                topMargin: 84
            }
            QText {
                width: 539
                height: 56
                text: STR.STR_QML_639
                color: "#031F2B"
                font.weight: Font.Normal
                wrapMode: Text.WrapAnywhere
                font.family: "Lato"
                font.pixelSize: 16
            }
            QTextInputBoxTypeB {
                id: primaryKey
                label: STR.STR_QML_640
                boxWidth: 539
                boxHeight: 48
                isValid: true
                textInputted: primaryKeyUsername
                onTextInputtedChanged: {
                    if(!primaryKey.isValid){
                        primaryKey.isValid = true
                        primaryKey.errorText = ""
                    }
                    primaryKey.showError = false;
                }
            }
        }

        QButtonTextLink {
            height: 24
            label: STR.STR_QML_059
            anchors {
                left: parent.left
                leftMargin: 36
                bottom: parent.bottom
                bottomMargin: 36
            }
            onButtonClicked: {
                QMLHandle.sendEvent(EVT.EVT_PRIMARY_KEY_CONFIGURATION_BACK)
            }
        }
        QTextButton {
            width: 200
            height: 48
            anchors {
                right: parent.right
                rightMargin: 36
                bottom: parent.bottom
                bottomMargin: 36
            }
            label.text: STR.STR_QML_097
            label.font.pixelSize: 16
            type: eTypeE
            onButtonClicked: {
                var signUp = {
                    "mnemonic": AppModel.mnemonic,
                    "username": primaryKey.textInputted,
                    "passphrase": primaryKeyPassphrase
                }
                QMLHandle.notifySendEvent(EVT.EVT_PRIMARY_KEY_SIGN_IN_REQUEST,signUp)
            }
        }
    }
    Connections{
        target: Draco
        onSignalpkey_signup:{
            signalpkey_signup(https_code, error_code, error_msg)
        }
    }
    function signalpkey_signup(https_code, error_code, error_msg){
        if(https_code === DRACO_CODE.SUCCESSFULL && error_code === DRACO_CODE.RESPONSE_OK){
            var signUp = {
                "signername": primaryKeySignername,
                "passphrase": primaryKeyPassphrase
            }
            QMLHandle.sendEvent(EVT.EVT_PRIMARY_KEY_SIGN_IN_SUCCEED,signUp)
        }
    }
}
