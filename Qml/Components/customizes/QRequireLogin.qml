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
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.1
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.12
import HMIEVENTS 1.0
import EWARNING 1.0
import QRCodeItem 1.0
import DataPool 1.0
import DRACO_CODE 1.0
import "../../Components/customizes"
import "../../Components/origins"
import "../../../localization/STR_QML.js" as STR

Item {
    readonly property int eCREATE_ACCOUNT   : 0
    readonly property int eCONFIRM_CODE     : 1
    readonly property int eSET_PASSWORD     : 2
    property int whereAmI: eCREATE_ACCOUNT // 0: Create account - 1: Confirm Code - 2: Set password
    property var whereMyComponent: [createAccount, confirmCode, setPassword]
    property string confirmCodeStr: ""

    Rectangle{
        anchors.centerIn: parent
        width: contentLoader.width
        height: contentLoader.height
        color: "#FFFFFF"
        Loader {
            id: contentLoader
            sourceComponent: whereMyComponent[whereAmI]
        }
    }

    Component {
        id: createAccount
        Item {
            id: createAccountItem
            width: 400
            height: 524
            Column {
                id: inputUserEmail
                spacing: 16
                QText {
                    width: 400
                    text: STR.STR_QML_618
                    font.family: "Montserrat"
                    font.weight: Font.Medium
                    font.pixelSize: 30
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    color: "#031F2B"
                }
                QText {
                    width: 400
                    text: STR.STR_QML_619
                    font.family: "Lato"
                    font.weight: Font.Normal
                    font.pixelSize: 16
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    color: "#031F2B"
                }
                QTextInputBoxTypeB {
                    id: username
                    label: STR.STR_QML_620
                    boxWidth: 400
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
                    boxWidth: 400
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
                    width: 400
                    height: 48
                    font.family: "Lato"
                    font.pixelSize: 16
                    color: "#CF4018"
                    visible: false
                    wrapMode: Text.WordWrap
                }
                QTextButton {
                    width: 400
                    height: 48
                    label.text: STR.STR_QML_395
                    label.font.pixelSize: 16
                    type: eTypeA
                    enabled: username.textInputted !== "" && email.textInputted !== ""
                    onButtonClicked: {
                        commonError.visible = false
                        commonError.text = ""
                        if(username.textInputted !== "" && email.textInputted !== ""){
                            var newAccountInfo = {
                                "name"  : username.textInputted,
                                "email" : email.textInputted
                            };
                            Draco.createAccount(newAccountInfo.name,newAccountInfo.email)
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
                    width: 400
                    height: 48
                    label.text: STR.STR_QML_419
                    label.font.pixelSize: 16
                    type: eTypeB
                    onButtonClicked: {
                        commonError.visible = false
                        commonError.text = ""
                        QMLHandle.sendEvent(EVT.EVT_STARTING_APPLICATION_ONLINEMODE)
                    }
                }
                QText {
                    id: privacyTerms
                    width: 400
                    anchors.horizontalCenter: parent.horizontalCenter
                    property string privacylink: "https://www.nunchuk.io/privacy.html"
                    property string termslink: "https://www.nunchuk.io/terms.html"
                    text: STR.STR_QML_398.arg(privacyTerms.termslink).arg(privacyTerms.privacylink)
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
            function processingCreateAccountResult(https_code, error_code, error_msg){
                if(https_code !== DRACO_CODE.SUCCESSFULL || error_code === DRACO_CODE.REGISTER_EXCEPTION){
                    commonError.visible = true
                    commonError.text = qsTr("https_code[%1] error_code[%2] error_msg[%3], %4").arg(https_code).arg(error_code).arg(error_msg).arg(STR.STR_QML_399)
                }
                else if(https_code === DRACO_CODE.SUCCESSFULL && error_code === DRACO_CODE.RESPONSE_OK){
                    whereAmI = eCONFIRM_CODE
                }
                else{
                    email.isValid = false
                    email.errorText = error_msg
                    email.showError = true;
                }
            }
        }
    }
    Component {
        id: confirmCode
        Item {
            width: 400
            height: 340
            Column{
                spacing: 24
                Column{
                    spacing: 12
                    QText {
                        width: 400
                        height: 40
                        text: STR.STR_QML_424
                        font.family: "Montserrat"
                        font.weight: Font.Medium
                        font.pixelSize: 30
                        wrapMode: Text.WordWrap
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        color: "#031F2B"
                    }
                    QText {
                        width: 400
                        height: 56
                        wrapMode: Text.WordWrap
                        text: STR.STR_QML_621.arg(Draco.emailRequested)
                        font.family: "Lato"
                        font.pixelSize: 16
                        color: "#031F2B"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        onLinkActivated: {
                            Draco.resendVerifyNewDeviceCode()
                        }
                    }
                }
                QTextInputBoxTypeB {
                    id: changeCode
                    label: STR.STR_QML_426
                    isPassword: true
                    boxWidth: 400
                    boxHeight: 48
                    isValid: true
                    onTextInputtedChanged: {
                        if(!changeCode.isValid){
                            changeCode.isValid = true
                            changeCode.errorText = ""
                        }
                        confirmCodeStr = changeCode.textInputted
                    }
                }
                QTextButton {
                    width: 400
                    height: 48
                    label.text: STR.STR_QML_097
                    label.font.pixelSize: 16
                    type: eTypeA
                    enabled: changeCode.textInputted !== ""
                    onButtonClicked: {
                        whereAmI = eSET_PASSWORD
                    }
                }
                QButtonTextLink {
                    width: 400
                    height: 48
                    label: STR.STR_QML_397
                    fontPixelSize: 16
                    anchors.horizontalCenter: parent.horizontalCenter
                    onButtonClicked: {
                        whereAmI = eCREATE_ACCOUNT
                    }
                }
            }
        }
    }
    Component {
        id: setPassword
        Item {
            width: 400
            height: 436
            Column{
                spacing: 24
                Column{
                    spacing: 12
                    QText {
                        width: 400
                        height: 40
                        text: STR.STR_QML_622
                        font.family: "Montserrat"
                        font.weight: Font.Medium
                        font.pixelSize: 30
                        wrapMode: Text.WordWrap
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        color: "#031F2B"
                    }
                    QText {
                        width: 400
                        height: 56
                        wrapMode: Text.WordWrap
                        text: STR.STR_QML_623
                        font.family: "Lato"
                        font.pixelSize: 16
                        color: "#031F2B"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }
                QTextInputBoxTypeB {
                    id: newPass
                    label: STR.STR_QML_416
                    isPassword: true
                    boxWidth: 400
                    boxHeight: 48
                    isValid: true
                    onTextInputtedChanged: {
                        if(!newPass.isValid){
                            newPass.isValid = true
                            newPass.errorText = ""
                        }
                    }
                }
                QTextInputBoxTypeB {
                    id: confirmPass
                    label: STR.STR_QML_624
                    isPassword: true
                    boxWidth: 400
                    boxHeight: 48
                    isValid: newPass.textInputted == confirmPass.textInputted
                    onTextInputtedChanged: {
                        if(!confirmPass.isValid){
                            confirmPass.isValid = true
                            confirmPass.errorText = ""
                        }
                    }
                }
                QText {
                    id: commonError
                    width: 338
                    height: 48
                    font.family: "Lato"
                    font.pixelSize: 16
                    color: "#CF4018"
                    visible: false
                    wrapMode: Text.WordWrap
                }
                QTextButton {
                    width: 400
                    height: 48
                    label.text: STR.STR_QML_097
                    label.font.pixelSize: 16
                    type: eTypeA
                    enabled: newPass.textInputted !== "" && (newPass.textInputted === confirmPass.textInputted)
                    onButtonClicked: {
                        var oldPassword = confirmCodeStr
                        var newPassword = newPass.textInputted
                        Draco.changePassword(oldPassword, newPassword);
                    }
                }
                QButtonTextLink {
                    width: 400
                    height: 48
                    label: STR.STR_QML_397
                    fontPixelSize: 16
                    anchors.horizontalCenter: parent.horizontalCenter
                    onButtonClicked: {
                        commonError.visible = false
                        commonError.text = ""
                        whereAmI = eCONFIRM_CODE
                    }
                }
            }
            function processingChangePasswordResult(https_code, error_code, error_msg){
                if(https_code === DRACO_CODE.SUCCESSFULL && error_code === DRACO_CODE.RESPONSE_OK){
                    QMLHandle.sendEvent(EVT.EVT_STARTING_APPLICATION_ONLINEMODE)
                }
                else{
                    commonError.visible = true
                    commonError.text = qsTr("https_code[%1] error_code[%2] error_msg[%3], %4").arg(https_code).arg(error_code).arg(error_msg).arg(STR.STR_QML_407)
                }
            }
        }
    }

    Connections {
        target: Draco
        onCreateAccountResult: {
            contentLoader.item.processingCreateAccountResult(https_code, error_code, error_msg)
        }
        onChangePasswordResult: {
            contentLoader.item.processingChangePasswordResult(https_code, error_code, error_msg)
        }
    }

}
