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
import HMIEVENTS 1.0
import EWARNING 1.0
import DRACO_CODE 1.0
import QRCodeItem 1.0
import "../../Components/origins"
import "../../Components/customizes"
import "../../../localization/STR_QML.js" as STR
QScreen {
    id: rootlogin
    readonly property int eCREATE_ACCOUNT   : 0
    readonly property int eSIGN_IN          : 1
    readonly property int eFORGOT_PASSWORD  : 2
    readonly property int eRECOVER_PASSWORD : 3
    readonly property int eCHANGE_PASSWORD  : 4
    readonly property int eVERIFY_NEWDEVICE : 5
    property int whereAmI: eSIGN_IN // 0: Create account - 1: Sign-in - 2: Forgot password - 3: Change password
    property var whereMyComponent: [createAccount, signin, forgotPassword, recoverPassword, changePassword, verifyNewDevice]

    Row {
        id: getStarted
        visible: !AppSetting.isStarted
        Item {
            width: rootlogin.width
            height: rootlogin.height
            QImage {
                source: "qrc:/Images/Images/OnlineMode/Gradient_bg.png"
                anchors.fill: parent
            }
            QImage {
                source: "qrc:/Images/Images/OnlineMode/Gradient_overlay.png"
                anchors.fill: parent
            }
            Column {
                spacing: 36
                anchors.centerIn: parent
                QImage {
                    width: 120
                    height: 120
                    anchors.horizontalCenter: parent.horizontalCenter
                    source: "qrc:/Images/Images/logo-light.svg"
                }
                Column{
                    height: 100
                    spacing: 8
                    QText {
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: STR.STR_QML_390
                        font.family: "Lato"
                        font.weight: Font.DemiBold
                        font.pixelSize: 52
                        horizontalAlignment: Text.AlignHCenter
                        color: "#FFFFFF"

                    }
                    QText {
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: STR.STR_QML_391
                        font.family: "Lato"
                        font.weight: Font.Normal
                        font.pixelSize: 20
                        horizontalAlignment: Text.AlignHCenter
                        color: "#FFFFFF"
                    }
                }
                QTextButton {
                    width: 120
                    height: 36
                    anchors.horizontalCenter: parent.horizontalCenter
                    label.text: STR.STR_QML_592
                    label.font.pixelSize: 16
                    type: eTypeF
                    onButtonClicked: {
                        AppSetting.isStarted = true
                        QMLHandle.sendEvent(EVT.EVT_STARTING_APPLICATION_LOCALMODE)
                    }
                }
            }
        }
    }
    Row {
        id: login
        visible: AppSetting.isStarted
        Item {
            width: rootlogin.width - loginpanel.width
            height: rootlogin.height
            QImage {
                source: "qrc:/Images/Images/OnlineMode/Gradient_bg.png"
                anchors.fill: parent
            }
            QImage {
                source: "qrc:/Images/Images/OnlineMode/Gradient_overlay.png"
                anchors.fill: parent
            }
            Column {
                spacing: 36
                anchors.centerIn: parent
                QImage {
                    width: 120
                    height: 120
                    anchors.horizontalCenter: parent.horizontalCenter
                    source: "qrc:/Images/Images/logo-light.svg"
                }
                QText {
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: STR.STR_QML_390
                    font.family: "Lato"
                    font.weight: Font.DemiBold
                    font.pixelSize: 52
                    horizontalAlignment: Text.AlignHCenter
                    color: "#FFFFFF"
                    QText {
                        anchors.top: parent.bottom
                        anchors.topMargin: 12
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: STR.STR_QML_391
                        font.family: "Lato"
                        font.weight: Font.Normal
                        font.pixelSize: 20
                        horizontalAlignment: Text.AlignHCenter
                        color: "#FFFFFF"
                    }
                }
            }
        }
        Rectangle {
            id: loginpanel
            width: 386
            height: rootlogin.height
            color: "#FFFFFF"
            Loader {
                id: contentLoader
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: 170
                sourceComponent: whereMyComponent[whereAmI]
            }
        }
    }
    Component {
        id: createAccount
        Item {
            id: createAccountItem
            QText {
                id: title
                anchors.top: parent.bottom
                anchors.topMargin: 12
                anchors.horizontalCenter: parent.horizontalCenter
                text: STR.STR_QML_392
                font.family: "Lato"
                font.weight: Font.Bold
                font.pixelSize: 20
                horizontalAlignment: Text.AlignHCenter
                color: "#031F2B"
            }
            Column {
                id: inputUserEmail
                spacing: 16
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: title.bottom
                anchors.topMargin: 36
                QTextInputBoxTypeB {
                    id: username
                    label: STR.STR_QML_393
                    boxWidth: 338
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
                    boxWidth: 338
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
                    width: 338
                    height: 48
                    font.family: "Lato"
                    font.pixelSize: 16
                    color: "#CF4018"
                    visible: false
                    wrapMode: Text.WordWrap
                }
            }
            Column {
                id: buttonCreateSignin
                spacing: 16
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: inputUserEmail.bottom
                anchors.topMargin: 24
                QTextButton {
                    width: 338
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
                            QMLHandle.sendEvent(EVT.EVT_LOGIN_ONLINE_CREATE_ACCOUNT, newAccountInfo)
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
                    id: btnCreatePrimaryKey
                    width: 338
                    height: 48
                    label.text: STR.STR_QML_635
                    label.font.pixelSize: 16
                    type: eTypeB
                    onButtonClicked: {
                        processing = true
                        QMLHandle.sendEvent(EVT.EVT_STARTING_APPLICATION_LOCALMODE, 2)
                        processing = false
                    }
                }
                QButtonTextLink {
                    height: 48
                    label: STR.STR_QML_397
                    fontPixelSize: 16
                    anchors.horizontalCenter: parent.horizontalCenter
                    onButtonClicked: {
                        commonError.visible = false
                        commonError.text = ""
                        whereAmI = eSIGN_IN
                    }
                }
                QText {
                    id: privacyTerms
                    width: parent.width
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
                    whereAmI = eCHANGE_PASSWORD
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
        id: changePassword
        Item {
            Rectangle {
                id: notifySentEmail
                anchors.horizontalCenter: parent.horizontalCenter
                width: 338
                height: 108
                color: "#EAEAEA"
                radius: 8
                Row {
                    height: 84
                    spacing: 11
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    QImage {
                        anchors.verticalCenter: parent.verticalCenter
                        source: "qrc:/Images/Images/tooltip.png"
                        width: 30
                        height: 30
                    }
                    QText {
                        width: 241
                        height: 84
                        text: STR.STR_QML_400.arg(Draco.emailRequested)
                        font.family: "Lato"
                        font.pixelSize: 16
                        color: "#031F2B"
                        wrapMode: Text.WordWrap
                    }
                }
            }
            Column {
                id: inputpassword
                spacing: 16
                anchors.top: notifySentEmail.bottom
                anchors.topMargin: 24
                anchors.horizontalCenter: parent.horizontalCenter
                QTextInputBoxTypeB {
                    id: changePasswordOldPassword
                    label: STR.STR_QML_401
                    isPassword: true
                    boxWidth: 338
                    boxHeight: 48
                    isValid: true
                    onTextInputtedChanged: {
                        if(!changePasswordOldPassword.isValid){
                            changePasswordOldPassword.isValid = true
                            changePasswordOldPassword.errorText = ""
                        }
                    }
                }
                QTextInputBoxTypeB {
                    id: changePasswordNewPassword
                    label: STR.STR_QML_402
                    isPassword: true
                    boxWidth: 338
                    boxHeight: 48
                    isValid: true
                    onTextInputtedChanged: {
                        if(!changePasswordNewPassword.isValid){
                            changePasswordNewPassword.isValid = true
                            changePasswordNewPassword.errorText = ""
                        }
                    }
                }
                QTextInputBoxTypeB {
                    id: changePasswordConfirmPassword
                    label: STR.STR_QML_403
                    isPassword: true
                    boxWidth: 338
                    boxHeight: 48
                    isValid: textInputted === "" || textInputted === changePasswordNewPassword.textInputted
                    errorText: STR.STR_QML_404
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
            }
            Column {
                id: buttonGroupChangePassword
                spacing: 16
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: inputpassword.bottom
                anchors.topMargin: 24
                QTextButton {
                    width: 338
                    height: 48
                    label.text: STR.STR_QML_405
                    label.font.pixelSize: 16
                    type: eTypeA
                    enabled: changePasswordNewPassword.textInputted !== "" && (changePasswordNewPassword.textInputted === changePasswordConfirmPassword.textInputted)
                    onButtonClicked: {
                        if(changePasswordNewPassword.textInputted === changePasswordOldPassword.textInputted){
                            changePasswordNewPassword.isValid = false
                            changePasswordOldPassword.isValid = false
                            if(changePasswordNewPassword.textInputted === ""){
                                changePasswordNewPassword.errorText = STR.STR_QML_396
                                changePasswordOldPassword.errorText = STR.STR_QML_396
                            }
                            else{
                                changePasswordOldPassword.errorText = STR.STR_QML_406
                            }
                        }
                        else{
                            var requestBody = {
                                "oldPassword" : changePasswordOldPassword.textInputted,
                                "newPassword" : changePasswordNewPassword.textInputted,
                            };
                            QMLHandle.sendEvent(EVT.EVT_LOGIN_ONLINE_CHANGE_PASSWORD, requestBody)
                        }
                    }
                }
                QButtonTextLink {
                    height: 48
                    label: STR.STR_QML_397
                    fontPixelSize: 16
                    anchors.horizontalCenter: parent.horizontalCenter
                    onButtonClicked: {
                        commonError.visible = false
                        commonError.text = ""
                        whereAmI = eSIGN_IN
                    }
                }
            }
            function processingChangePasswordResult(https_code, error_code, error_msg){
                if(https_code === DRACO_CODE.SUCCESSFULL && error_code === DRACO_CODE.RESPONSE_OK){
                    whereAmI = eSIGN_IN
                }
                else{
                    commonError.visible = true
                    commonError.text = qsTr("https_code[%1] error_code[%2] error_msg[%3], %4").arg(https_code).arg(error_code).arg(error_msg).arg(STR.STR_QML_407)
                }
            }
        }
    }
    Component {
        id: forgotPassword
        Item {
            Column {
                id: textGuide
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 8
                QText {
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: STR.STR_QML_408
                    font.family: "Lato"
                    font.pixelSize: 20
                    color: "#031F2B"
                    font.weight: Font.Bold
                }
                QText {
                    width: 338
                    height: 84
                    wrapMode: Text.WordWrap
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: STR.STR_QML_409
                    font.family: "Lato"
                    font.pixelSize: 16
                    color: "#031F2B"
                    horizontalAlignment: Text.AlignHCenter
                }
            }
            QTextInputBoxTypeB {
                id: emailbackup
                anchors.top: textGuide.bottom
                anchors.topMargin: 36
                anchors.horizontalCenter: parent.horizontalCenter
                label: STR.STR_QML_394
                boxWidth: 338
                boxHeight: 48
                onTextInputtedChanged: {
                    if(!emailbackup.isValid){
                        emailbackup.isValid = true
                        emailbackup.errorText = ""
                    }
                }
            }
            Column {
                id: buttonGroupForgotPassword
                spacing: 16
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: emailbackup.bottom
                anchors.topMargin: 24
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
                    width: 338
                    height: 48
                    label.text: STR.STR_QML_410
                    label.font.pixelSize: 16
                    type: eTypeA
                    enabled: emailbackup.textInputted !== ""
                    onButtonClicked: {
                        commonError.visible = false
                        commonError.text = ""
                        QMLHandle.sendEvent(EVT.EVT_LOGIN_ONLINE_FORGOT_PASSWORD, emailbackup.textInputted)
                    }
                }
                QButtonTextLink {
                    height: 48
                    label: STR.STR_QML_397
                    fontPixelSize: 16
                    anchors.horizontalCenter: parent.horizontalCenter
                    onButtonClicked: {
                        commonError.visible = false
                        commonError.text = ""
                        whereAmI = eSIGN_IN
                    }
                }
            }

            function processingForgotPasswordResult(https_code, error_code, error_msg){
                if(https_code !== DRACO_CODE.SUCCESSFULL || error_code === DRACO_CODE.FORGOT_PASSWORD_EXCEPTION){
                    commonError.visible = true
                    commonError.text = qsTr("https_code[%1] error_code[%2] error_msg[%3], %4").arg(https_code).arg(error_code).arg(error_msg).arg(STR.STR_QML_411)
                }
                else if(https_code === DRACO_CODE.SUCCESSFULL && error_code === DRACO_CODE.RESPONSE_OK){
                    whereAmI = eRECOVER_PASSWORD
                }
                else{
                    emailbackup.isValid = false
                    emailbackup.errorText = error_msg
                }
            }
        }
    }
    Component {
        id: recoverPassword
        Item {
            Rectangle {
                id: notifySentEmail
                anchors.horizontalCenter: parent.horizontalCenter
                width: 338
                height: 108
                color: "#EAEAEA"
                radius: 8
                Row {
                    height: 84
                    spacing: 11
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    QImage {
                        anchors.verticalCenter: parent.verticalCenter
                        source: "qrc:/Images/Images/tooltip.png"
                        width: 30
                        height: 30
                    }
                    QText {
                        width: 241
                        height: 84
                        text: STR.STR_QML_412.arg(Draco.emailRequested)
                        font.family: "Lato"
                        font.pixelSize: 16
                        color: "#031F2B"
                        wrapMode: Text.WordWrap
                    }
                }
            }
            Column {
                id: inputpassword
                spacing: 16
                anchors.top: notifySentEmail.bottom
                anchors.topMargin: 24
                anchors.horizontalCenter: parent.horizontalCenter
                QTextInputBoxTypeB {
                    id: tokenPassword
                    label: STR.STR_QML_413
                    isPassword: true
                    boxWidth: 338
                    boxHeight: 48
                    isValid: true
                    onTextInputtedChanged: {
                        if(!tokenPassword.isValid){
                            tokenPassword.isValid = true
                            tokenPassword.errorText = ""
                        }
                    }
                }
                QTextInputBoxTypeB {
                    id: newpassword
                    label: STR.STR_QML_402
                    isPassword: true
                    boxWidth: 338
                    boxHeight: 48
                    isValid: true
                    onTextInputtedChanged: {
                        if(!newpassword.isValid){
                            newpassword.isValid = true
                            newpassword.errorText = ""
                        }
                    }
                }
                QTextInputBoxTypeB {
                    id: confirmPassword
                    label: STR.STR_QML_403
                    isPassword: true
                    boxWidth: 338
                    boxHeight: 48
                    isValid: textInputted === "" || textInputted === newpassword.textInputted
                    errorText: STR.STR_QML_404
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
            }
            Column {
                id: buttonGroupChangePassword
                spacing: 16
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: inputpassword.bottom
                anchors.topMargin: 24
                QTextButton {
                    width: 338
                    height: 48
                    label.text: STR.STR_QML_405
                    label.font.pixelSize: 16
                    type: eTypeA
                    enabled:  tokenPassword.textInputted !== "" && newpassword.textInputted !== "" && newpassword.textInputted === confirmPassword.textInputted
                    onButtonClicked: {
                        var requestBody = {
                            "token"   : tokenPassword.textInputted,
                            "passw"   : newpassword.textInputted,
                        };
                        QMLHandle.sendEvent(EVT.EVT_LOGIN_ONLINE_RECOVER_PASSWORD, requestBody)
                    }
                }
                QButtonTextLink {
                    height: 48
                    label: STR.STR_QML_397
                    fontPixelSize: 16
                    anchors.horizontalCenter: parent.horizontalCenter
                    onButtonClicked: {
                        commonError.visible = false
                        commonError.text = ""
                        whereAmI = eSIGN_IN
                    }
                }
            }

            function processingRecoverPasswordResult(https_code, error_code, error_msg){
                if(https_code !== DRACO_CODE.SUCCESSFULL || error_code === DRACO_CODE.RECOVER_PASSWORD_RESET_EXCEPTION || error_code === DRACO_CODE.RECOVER_PASSWORD_RESET_FAIL){
                    commonError.visible = true
                    commonError.text = qsTr("https_code[%1] error_code[%2] error_msg[%3], %4").arg(https_code).arg(error_code).arg(error_msg).arg(STR.STR_QML_414)
                }
                else if(https_code === DRACO_CODE.SUCCESSFULL && error_code === DRACO_CODE.RESPONSE_OK){
                    var requestBody = {
                        "email"      : Draco.emailRequested,
                        "password"   : newpassword.textInputted,
                    };
                    QMLHandle.sendEvent(EVT.EVT_LOGIN_ONLINE_SIGN_IN, requestBody)
                }
                else{
                    tokenPassword.isValid = false
                    tokenPassword.errorText = error_msg
                }
            }
            function processingSigninResult(https_code, error_code, error_msg){
                if(https_code === DRACO_CODE.SUCCESSFULL && error_code === DRACO_CODE.RESPONSE_OK){
                    QMLHandle.sendEvent(EVT.EVT_LOGIN_ONLINE_LOGIN_SUCCEED)
                }
            }
        }
    }
    Component {
        id: signin
        Item {
            QText {
                id: title
                anchors.top: parent.bottom
                anchors.topMargin: 12
                anchors.horizontalCenter: parent.horizontalCenter
                text: STR.STR_QML_415
                font.family: "Lato"
                font.weight: Font.Bold
                font.pixelSize: 20
                horizontalAlignment: Text.AlignHCenter
                color: "#031F2B"
            }
            Column {
                id: inputUserEmail
                spacing: 16
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: title.bottom
                anchors.topMargin: 36
                QTextInputBoxTypeB {
                    id: emailaddrs
                    label: STR.STR_QML_394
                    boxWidth: 338
                    boxHeight: 48
                    isValid: true
                    onTextInputtedChanged: {
                        if(!emailaddrs.isValid){
                            emailaddrs.isValid = true
                            emailaddrs.errorText = ""
                        }
                        emailaddrs.showError = false
                        passwd.showError = false
                    }
                }
                QTextInputBoxTypeB {
                    id: passwd
                    label: STR.STR_QML_416
                    isPassword: true
                    boxWidth: 338
                    boxHeight: 48
                    isValid: true
                    onTextInputtedChanged: {
                        if(!passwd.isValid){
                            passwd.isValid = true
                            passwd.errorText = ""
                        }
                        emailaddrs.showError = false
                        passwd.showError = false
                    }
                    onEnterKeyRequest: {
                        if(emailaddrs.textInputted !== "" && passwd.textInputted !== ""){
                            btnSignin.buttonClicked()
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
            }
            Row {
                id: staySignedin
                property bool isStaySignedIn: false
                anchors.left: inputUserEmail.left
                anchors.top: inputUserEmail.bottom
                anchors.topMargin: 14
                spacing: 4
                QImage {
                    width: 18
                    height: 18
                    anchors.verticalCenter: parent.verticalCenter
                    source: Draco.stayLoggedIn ? "qrc:/Images/Images/Checked_n.png" : "qrc:/Images/Images/UnChecked_n.png"
                    MouseArea {
                        anchors.fill: parent
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
                    font.weight: Font.DemiBold
                }
            }
            QText {
                width: 98
                height: 28
                anchors.right: inputUserEmail.right
                anchors.top: inputUserEmail.bottom
                anchors.topMargin: 14
                horizontalAlignment: Text.AlignRight
                text: STR.STR_QML_418
                font.family: "Lato"
                font.pixelSize: 16
                font.underline : true
                color: btnMouseForgotPwd.containsMouse ? "#35ABEE" : "#031F2B"
                MouseArea {
                    id: btnMouseForgotPwd
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    anchors.fill: parent
                    onClicked: {
                        whereAmI = eFORGOT_PASSWORD
                    }
                }
            }
            Column {
                id: buttonGroupSignin
                spacing: 16
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: inputUserEmail.bottom
                anchors.topMargin: 64
                QTextButton {
                    id: btnSignin
                    width: 338
                    height: 48
                    label.text: STR.STR_QML_419
                    label.font.pixelSize: 16
                    type: eTypeA
                    enabled: emailaddrs.textInputted !== "" && passwd.textInputted !== "" && !processing
                    onButtonClicked: {
                        processing = true
                        var requestBody = {
                            "email"      : emailaddrs.textInputted,
                            "password"   : passwd.textInputted,
                        };
                        QMLHandle.sendEvent(EVT.EVT_LOGIN_ONLINE_SIGN_IN, requestBody)
                        processing = false
                    }
                }
                QTextButton {
                    id: btnSigninPrimaryKey
                    width: 338
                    height: 48
                    label.text: STR.STR_QML_643
                    label.font.pixelSize: 16
                    type: eTypeB
                    onButtonClicked: {
                        processing = true
                        QMLHandle.sendEvent(EVT.EVT_STARTING_APPLICATION_LOCALMODE, 1)
                        processing = false
                    }
                }
                QTextButton {
                    width: 338
                    height: 48
                    label.text: STR.STR_QML_395
                    label.font.pixelSize: 16
                    type: eTypeB
                    onButtonClicked: {
                        whereAmI = eCREATE_ACCOUNT
                    }
                }
                QButtonTextLink {
                    height: 20
                    label: STR.STR_QML_420
                    direction: eRIGHT
                    anchors.horizontalCenter: parent.horizontalCenter
                    onButtonClicked: {
                        QMLHandle.sendEvent(EVT.EVT_STARTING_APPLICATION_LOCALMODE)
                    }
                }
                QText {
                    id: privacyTerms
                    width: parent.width
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
                QText {
                    width: parent.width
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: STR.STR_QML_422
                    font.family: "Lato"
                    font.pixelSize: 12
                    horizontalAlignment: Text.AlignHCenter
                    color: "#031F2B"
                    lineHeightMode: Text.FixedHeight
                    lineHeight: 16
                    wrapMode: Text.WordWrap
                }
            }
            function processingSigninResult(https_code, error_code, error_msg){
                if(https_code !== DRACO_CODE.SUCCESSFULL || error_code === DRACO_CODE.SIGN_IN_EXCEPTION){
                    emailaddrs.isValid = false
                    emailaddrs.errorText = STR.STR_QML_423
                    emailaddrs.showError = true;
                }
                else {
                    if(https_code === DRACO_CODE.SUCCESSFULL){
                        if (error_code === DRACO_CODE.RESPONSE_OK){
                            QMLHandle.sendEvent(EVT.EVT_LOGIN_ONLINE_LOGIN_SUCCEED)
                        }
                        else if (error_code === DRACO_CODE.LOGIN_NEW_DEVICE){
                            commonError.visible = false
                            commonError.text = ""
                            whereAmI = eVERIFY_NEWDEVICE
                        }
                        else{
                            passwd.isValid = false
                            passwd.errorText = error_msg
                            passwd.showError = true
                        }
                    }
                    else{
                        passwd.isValid = false
                        passwd.errorText = error_msg
                    }
                }
            }
        }
    }
    Component {
        id: verifyNewDevice
        Item {
            Column {
                id: textGuide
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 8
                QText {
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: STR.STR_QML_424
                    font.family: "Lato"
                    font.pixelSize: 20
                    color: "#031F2B"
                    font.weight: Font.Bold
                }
                QText {
                    width: 338
                    height: 84
                    wrapMode: Text.WordWrap
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: STR.STR_QML_425
                    font.family: "Lato"
                    font.pixelSize: 16
                    color: "#031F2B"
                    horizontalAlignment: Text.AlignHCenter
                    onLinkActivated: {
                        QMLHandle.sendEvent(EVT.EVT_LOGIN_ONLINE_RESEND_CONFIRM_CODE)
                    }
                }
            }
            QTextInputBoxTypeB {
                id: confirmCode
                anchors.top: textGuide.bottom
                anchors.topMargin: 36
                anchors.horizontalCenter: parent.horizontalCenter
                label: STR.STR_QML_426
                boxWidth: 338
                boxHeight: 48
                onTextInputtedChanged: {
                    if(!confirmCode.isValid){
                        confirmCode.isValid = true
                        confirmCode.errorText = ""
                    }
                }
            }
            Column {
                id: buttonGroupVerifyDevice
                spacing: 16
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: confirmCode.bottom
                anchors.topMargin: 24
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
                    width: 338
                    height: 48
                    label.text: STR.STR_QML_427
                    label.font.pixelSize: 16
                    type: eTypeA
                    enabled: confirmCode.textInputted !== ""
                    onButtonClicked: {
                        commonError.visible = false
                        commonError.text = ""
                        QMLHandle.sendEvent(EVT.EVT_LOGIN_ONLINE_VERIFY_NEW_DEVICE, confirmCode.textInputted)
                    }
                }
                QButtonTextLink {
                    height: 48
                    label: STR.STR_QML_397
                    fontPixelSize: 16
                    anchors.horizontalCenter: parent.horizontalCenter
                    onButtonClicked: {
                        commonError.visible = false
                        commonError.text = ""
                        whereAmI = eSIGN_IN
                    }
                }
            }
            function processingVerifyNewDeviceResult(https_code, error_code, error_msg){
                if(https_code === DRACO_CODE.SUCCESSFULL && error_code === DRACO_CODE.RESPONSE_OK){
                    QMLHandle.sendEvent(EVT.EVT_LOGIN_ONLINE_LOGIN_SUCCEED)
                }
                else{
                    commonError.visible = true
                    commonError.text = qsTr("https_code[%1] error_code[%2] error_msg[%3]").arg(https_code).arg(error_code).arg(error_msg)
                }
            }
        }
    }

    Connections {
        target: Draco
        onCreateAccountResult: {
            contentLoader.item.processingCreateAccountResult(https_code, error_code, error_msg)
        }
        onForgotPasswordResult: {
            contentLoader.item.processingForgotPasswordResult(https_code, error_code, error_msg)
        }
        onChangePasswordResult: {
            contentLoader.item.processingChangePasswordResult(https_code, error_code, error_msg)
        }
        onSinginResult: {
            contentLoader.item.processingSigninResult(https_code, error_code, error_msg)
        }
        onRecoverPasswordResult: {
            contentLoader.item.processingRecoverPasswordResult(https_code, error_code, error_msg)
        }
        onVerifyNewDeviceResult: {
            contentLoader.item.processingVerifyNewDeviceResult(https_code, error_code, error_msg)
        }
        onResendVerifyNewDeviceCodeResult: {
            //TBD
        }
    }
    Component.onCompleted: {
        ClientController.guestMode = false
    }
}
