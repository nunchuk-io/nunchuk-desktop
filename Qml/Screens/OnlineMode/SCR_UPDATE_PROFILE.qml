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
import Qt.labs.platform 1.1
import QtGraphicalEffects 1.12
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import DRACO_CODE 1.0
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Chats"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../Components/customizes/Popups"
import "../../../localization/STR_QML.js" as STR
QScreen {
    id: roots
    popupWidth: 600
    popupHeight: 414

    QOnScreenContent {
        id: contenCenter
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        label.text: STR.STR_QML_450
        onCloseClicked: {
            QMLHandle.sendEvent(EVT.EVT_UPDATE_PROFILE_BACK)
        }
        Row {
            anchors.centerIn: parent
            spacing: 24
            QAvatar {
                id: avatar
                width: 194
                height: 194
                anchors.verticalCenter: parent.verticalCenter
                displayStatus: false
                avatarUrl: ClientController.user.avatar
                username:  ClientController.user.name
                avatarLocal: ClientController.user.avatar
                textSize: 72
            }
            Column {
                spacing: 24
                anchors.verticalCenter: parent.verticalCenter
                QTextInputBoxTypeB {
                    id: username
                    label: STR.STR_QML_451
                    boxWidth: 310
                    boxHeight: 48
                    isValid: true
                    textInputted: ClientController.user.name
                    onTextInputtedChanged: {
                        if(!username.isValid){
                            username.isValid = true
                            username.errorText = ""
                        }
                        username.showError = false
                    }
                }
                Row {
                    spacing: 16
                    QTextButton {
                        width: 130
                        height: 48
                        label.text: STR.STR_QML_452
                        label.font.pixelSize: 16
                        type: eTypeB
                        onButtonClicked: {
                            openfileDialog.open()
                        }
                    }
                    QButtonTextLink {
                        height: 48
                        label: STR.STR_QML_453
                        fontPixelSize: 16
                        displayIcon: false
                        onButtonClicked: {
                            avatar.avatarUrl = ""
                            avatar.avatarLocal = ""
                        }
                    }
                }
            }
        }
        QTextButton {
            id: buttonBottomR
            width: 128
            height: 48
            label.text: STR.STR_QML_454
            label.font.pixelSize: 16
            type: eTypeE
            enabled: ClientController.isMatrixLoggedIn
            anchors {
                right: parent.right
                rightMargin: 36
                bottom: parent.bottom
                bottomMargin: 36
            }
            onButtonClicked: {
                if(username.textInputted.length < 5){
                    username.isValid = false
                    username.errorText = STR.STR_QML_552
                    username.showError = true
                    return
                }
                console.log("username.textInputted", username.textInputted, ClientController.user.name)
                if(username.textInputted !== ClientController.user.name){
                    ClientController.setUserDisplayname(username.textInputted)
                }
                if(avatar.avatarLocal !== ClientController.user.avatar){
                    ClientController.setUserAvatar(avatar.avatarLocal)
                }
            }
        }
    }

    Connections {
        target: ClientController
        onUserChanged: {
            avatar.avatarUrl = ClientController.user.avatar
            avatar.avatarLocal = ClientController.user.avatar
            username.textInputted = ClientController.user.name
        }
    }

    FileDialog {
        id: openfileDialog
        fileMode: FileDialog.OpenFile
        nameFilters: ["Image files (*.png *.jpg)"]
        onAccepted: {
            avatar.avatarLocal = openfileDialog.file
        }
    }

    Connections{
        target: Draco
        onUpdateProfileResult:{
            if(https_code === DRACO_CODE.SUCCESSFULL && error_code === DRACO_CODE.RESPONSE_OK){
                _warning.open()
            }
        }
    }

    QPopupToast{
        id: _warning
        width: 528
        height: 60
        x:contenCenter.x + 36
        y:contenCenter.y + 318
        warningType: EWARNING.SUCCESS_MSG
        warningExplain: STR.STR_QML_586
    }
}
