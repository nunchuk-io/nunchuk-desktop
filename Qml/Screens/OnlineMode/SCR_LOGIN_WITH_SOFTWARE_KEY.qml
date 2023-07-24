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
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../../localization/STR_QML.js" as STR

QScreen {
    property var primary_key: ""
    Connections {
        target: AppModel
        onSignalShowToast:{
            _warning.open()
        }
    }
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
                width: 64
                height: 64
                anchors.horizontalCenter: parent.horizontalCenter
                source: "qrc:/Images/Images/logo-light.svg"
            }
            Column{
                spacing: 16
                QTextInputBoxTypeB {
                    id: primary_key_name
                    label: STR.STR_QML_656
                    boxWidth: 350
                    boxHeight: 48
                    isValid: true
                    enabled: false
                    textInputted: primary_key.name ? primary_key.name : ""
                }
                QTextInputBoxTypeB {
                    id: _passphrase
                    label: STR.STR_QML_645
                    isPassword: true
                    boxWidth: 350
                    boxHeight: 48
                    isValid: true
                    onTextInputtedChanged: {
                        if(!_passphrase.isValid){
                            _passphrase.isValid = true
                            _passphrase.errorText = ""
                        }
                        primary_key_name.showError = false;
                    }
                    onEnterKeyRequest: {
                        commonError.visible = false
                        commonError.text = ""
                        var signIn = {"primary_key":primary_key,
                            "passphrase":_passphrase.textInputted
                        }
                        QMLHandle.sendEvent(EVT.EVT_PRIMARY_KEY_SIGNIN_ACCOUNT_REQUEST, signIn)
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
                Item{
                    width: 125
                    height: 28
                    anchors.horizontalCenter: parent.horizontalCenter
                    Row {
                        id: staySignedin
                        spacing: 4
                        QImage {
                            width: 18
                            height: 18
                            anchors.verticalCenter: parent.verticalCenter
                            source: Draco.stayLoggedIn ? "qrc:/Images/Images/Checked_n.png" : "qrc:/Images/Images/UnChecked_n.png"
                            MouseArea {
                                anchors.fill: parent
                                hoverEnabled: true
                                cursorShape: Qt.PointingHandCursor
                                onClicked: {Draco.stayLoggedIn = !Draco.stayLoggedIn}
                            }
                        }
                        QText {
                            width: 98
                            height: 28
                            anchors.verticalCenter: parent.verticalCenter
                            text: STR.STR_QML_417
                            font.family: "Lato"
                            font.pixelSize: 16
                            font.weight: Font.Normal
                            MouseArea {
                                anchors.fill: parent
                                hoverEnabled: true
                                cursorShape: Qt.PointingHandCursor
                                onClicked: {Draco.stayLoggedIn = !Draco.stayLoggedIn}
                            }
                        }
                    }
                }

                QTextButton {
                    width: 350
                    height: 48
                    label.text: STR.STR_QML_419
                    label.font.pixelSize: 16
                    type: eTypeB
                    onButtonClicked: {
                        commonError.visible = false
                        commonError.text = ""
                        processing = true
                        var signIn = {"primary_key":primary_key,
                            "passphrase":_passphrase.textInputted
                        }

                        QMLHandle.sendEvent(EVT.EVT_PRIMARY_KEY_SIGNIN_ACCOUNT_REQUEST, signIn)
                        processing = false
                    }
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
                QMLHandle.sendEvent(EVT.EVT_LOGIN_WITH_SOFTWARE_KEY_BACK)
            }
        }

        QPopupToast{
            id:_warning
            x:36
            y:520
            warningType:AppModel.warningMessage.type
            warningCode: AppModel.warningMessage.code
            warningWhat: AppModel.warningMessage.what
            warningContent: AppModel.warningMessage.contentDisplay
            warningExplain: AppModel.warningMessage.explaination
        }
    }
    function processingSigninResult(https_code, error_code, error_msg){
        if(https_code !== DRACO_CODE.SUCCESSFULL || error_code === DRACO_CODE.SIGN_IN_EXCEPTION){
            primary_key_name.isValid = false
            primary_key_name.errorText = STR.STR_QML_423
            primary_key_name.showError = true;
        }
        else {
            if(https_code === DRACO_CODE.SUCCESSFULL){
                if (error_code === DRACO_CODE.RESPONSE_OK){
                    QMLHandle.sendEvent(EVT.EVT_LOGIN_WITH_SOFTWARE_KEY_SUCCEED)
                }
                else if (error_code === DRACO_CODE.LOGIN_NEW_DEVICE){
                    commonError.visible = false
                    commonError.text = ""
                }
                else{
                    _passphrase.isValid = false
                    _passphrase.errorText = STR.STR_QML_667
                    _passphrase.showError = true
                }
            }
            else{
                _passphrase.isValid = false
                _passphrase.errorText = error_msg
            }
        }
    }
    Connections {
        target: Draco
        onSignalpkey_signin: {
            processingSigninResult(https_code, error_code, error_msg)
        }
    }
}
