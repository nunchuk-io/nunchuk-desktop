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
import DRACO_CODE 1.0
import "../../Components/origins"
import "../../Components/customizes"
import "../../../localization/STR_QML.js" as STR

QScreen {
    QOnScreenContent {
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        label.text: ""
        onCloseClicked: {
            QMLHandle.sendEvent(EVT.EVT_ONS_CLOSE_ALL_REQUEST)
        }
        Column{
            anchors.centerIn: parent
            spacing: 24
            QImage {
                width: 105
                height: 105
                anchors.horizontalCenter: parent.horizontalCenter
                source: "qrc:/Images/Images/logo-light.svg"
            }
            Column{
                spacing: 16
                QTextInputBoxTypeB {
                    id: email
                    label: STR.STR_QML_394
                    boxWidth: 350
                    boxHeight: 48
                    isValid: true
                    onTextInputtedChanged: {
                        if(!email.isValid){
                            email.isValid = true
                            email.errorText = ""
                        }
                        email.showError = false;
                    }
                }
                QTextInputBoxTypeB {
                    id: _password
                    label: STR.STR_QML_416
                    isPassword: true
                    boxWidth: 350
                    boxHeight: 48
                    isValid: true
                    onTextInputtedChanged: {
                        if(!_password.isValid){
                            _password.isValid = true
                            _password.errorText = ""
                        }
                        email.showError = false;
                    }
                }
                QText {
                    id: commonError
                    width: 350
                    height: 48
                    font.family: "Lato"
                    font.pixelSize: 16
                    color: "#CF4018"
                    visible: false
                    wrapMode: Text.WordWrap
                }
            }
            Column{
                spacing: 16
                QTextButton {
                    width: 350
                    height: 48
                    label.text: STR.STR_QML_419
                    label.font.pixelSize: 16
                    type: eTypeE
                    enabled: email.textInputted !== "" && _password.textInputted !== ""
                    onButtonClicked: {
                        commonError.visible = false
                        commonError.text = ""
                        processing = true
                        var requestBody = {
                            "email"      : email.textInputted,
                            "password"   : _password.textInputted,
                        };
                        QMLHandle.sendEvent(EVT.EVT_SIGN_IN_PASSWORD_REQUEST, requestBody)
                        processing = false
                    }
                }
                QTextButton {
                    width: 350
                    height: 48
                    label.text: STR.STR_QML_643
                    label.font.pixelSize: 16
                    type: eTypeB
                    onButtonClicked: {
                        var isSignIn = true;
                        QMLHandle.sendEvent(EVT.EVT_SIGN_IN_PRIMARY_KEY_REQUEST,isSignIn)
                    }
                }
            }
        }
    }
    function processingSigninResult(https_code, error_code, error_msg){
        if(https_code !== DRACO_CODE.SUCCESSFULL || error_code === DRACO_CODE.SIGN_IN_EXCEPTION){
            if (error_code === DRACO_CODE.SIGN_IN_EXCEPTION) {
                errorInfo.contentText = error_msg
                errorInfo.open()
            } else {
                email.isValid = false
                email.errorText = error_msg
                email.showError = true;
            }
        }
        else {
            if(https_code === DRACO_CODE.SUCCESSFULL){
                if (error_code === DRACO_CODE.RESPONSE_OK){
                    QMLHandle.sendEvent(EVT.EVT_NUNCHUK_LOGIN_SUCCEEDED)
                }
                else if (error_code === DRACO_CODE.LOGIN_NEW_DEVICE){
                    commonError.visible = false
                    commonError.text = ""
                }
                else{
                    _password.isValid = false
                    _password.errorText = error_msg
                    _password.showError = true
                }
            }
            else{
                _password.isValid = false
                _password.errorText = error_msg
            }
        }
    }
    QPopupInfo{
        id: errorInfo
        coverColor: Qt.rgba(255, 255, 255, 0)
        onGotItClicked: {
            close()
        }
    }
    Connections {
        target: Draco
        onSinginResult: {
            processingSigninResult(https_code, error_code, error_msg)
        }
    }
}
