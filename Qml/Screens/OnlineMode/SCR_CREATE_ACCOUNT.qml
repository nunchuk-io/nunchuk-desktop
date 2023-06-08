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
                    id: username
                    label: STR.STR_QML_393
                    boxWidth: 350
                    boxHeight: 48
                    isValid: true
                    onTextInputtedChanged: {
                        if(!username.isValid){
                            username.isValid = true
                            username.errorText = ""
                        }
                        email.showError = false;
                    }
                }
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
                    label.text: STR.STR_QML_395
                    label.font.pixelSize: 16
                    type: eTypeE
                    enabled: username.textInputted !== "" && email.textInputted !== ""
                    onButtonClicked: {
                        commonError.visible = false
                        commonError.text = ""
                        if(username.textInputted !== "" && email.textInputted !== ""){
                            var newAccountInfo = {
                                "name"  : username.textInputted,
                                "email" : email.textInputted
                            };
                            QMLHandle.sendEvent(EVT.EVT_CREATE_ACCOUNT_REQUEST, newAccountInfo)
                        }
                        else{
                            if(username.textInputted === ""){
                                username.isValid = false
                                username.errorText = STR.STR_QML_396
                            }
                            if(email.textInputted === ""){
                                email.isValid = false
                                email.errorText = STR.STR_QML_396
                            }
                        }
                    }
                }
                QTextButton {
                    width: 350
                    height: 48
                    label.text: STR.STR_QML_635
                    label.font.pixelSize: 16
                    type: eTypeB
                    onButtonClicked: {
                        var isSignIn = false;
                        QMLHandle.sendEvent(EVT.EVT_CREATE_PRIMARY_KEY_REQUEST,isSignIn)
                    }
                }
                QText {
                    id: privacyTerms
                    width: 350
                    height: 32
                    anchors.horizontalCenter: parent.horizontalCenter
                    property string privacylink: "https://www.nunchuk.io/privacy.html"
                    property string termslink: "https://www.nunchuk.io/terms.html"
                    text: STR.STR_QML_421.arg(privacyTerms.termslink).arg(privacyTerms.privacylink)
                    font.family: "Lato"
                    font.pixelSize: 12
                    horizontalAlignment: Text.AlignHCenter
                    color: "#031F2B"
                    lineHeightMode: Text.FixedHeight
                    lineHeight: 16
                    wrapMode: Text.WordWrap
                    onLinkActivated: Qt.openUrlExternally(link)
                    MouseArea {
                        anchors.fill: parent
                        cursorShape: privacyTerms.hoveredLink ? Qt.PointingHandCursor : Qt.ArrowCursor
                        acceptedButtons: Qt.NoButton
                    }
                }
            }
        }
    }
    function processingCreateAccountResult(https_code, error_code, error_msg){
        if(https_code !== DRACO_CODE.SUCCESSFULL || error_code === DRACO_CODE.REGISTER_EXCEPTION){
            commonError.visible = true
            commonError.text = qsTr("https_code[%1] error_code[%2] error_msg[%3], %4").arg(https_code).arg(error_code).arg(error_msg).arg(STR.STR_QML_399)
        }
        else if(https_code === DRACO_CODE.SUCCESSFULL && error_code === DRACO_CODE.RESPONSE_OK){
            QMLHandle.sendEvent(EVT.EVT_CHANGE_PASSWORD_SHOW_REQUEST)
        }
        else{
            email.isValid = false
            email.errorText = error_msg
            email.showError = true;
        }
    }
    Connections {
        target: Draco
        onCreateAccountResult: {
            processingCreateAccountResult(https_code, error_code, error_msg)
        }
    }
}
