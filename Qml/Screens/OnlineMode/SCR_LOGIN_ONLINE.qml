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
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../Components/customizes/Popups"
import "../../../localization/STR_QML.js" as STR
QScreen {
    id: rootlogin

    readonly property int eACCOUNT_AVAILABILITY     : 0
    readonly property int eSIGN_IN_OLD_ACCOUNT      : 1
    readonly property int eSIGN_IN_NEW_ACCOUNT      : 2
    readonly property int eRESET_PASSWORD           : 3
    readonly property int eCHANGE_TEMPO_PASSWORD_OLD: 4
    readonly property int eCHANGE_TEMPO_PASSWORD_NEW: 5
    readonly property int eRECOVER_PASSWORD         : 6
    readonly property int eVERIFY_NEWDEVICE         : 7
    readonly property int eCREATE_ACCOUNT           : 8
             property int whereAmI      : eACCOUNT_AVAILABILITY
    readonly property int eRIGHT_WIDTH  : 386
    readonly property int eLEFT_WIDTH   : rootlogin.width - eRIGHT_WIDTH

    property var whereMyComponent: [
        accountAvailability,        // 0: Check account
        signinOldAccount,           // 1: Sign in with old account
        signinNewAccount,           // 2: Sign in with new account created
        resetPassword,              // 3: Reset/Forgot password
        changeTempoPasswordOld,     // 4: Change password when user create new account
        changeTempoPasswordNew,     // 5: Change password when user did not change tempo password
        recoverPassword,            // 6: Change password when user forgot password
        verifyNewDevice,            // 7: Verify new device
        createAccount               // 8: Create new account
    ]

    Row {
        id: getStarted
        visible: !AppSetting.isStarted
        Item {
            width: rootlogin.width
            height: rootlogin.height
            QPicture {
                source: "qrc:/Images/Images/Gradient_bg.png"
                anchors.fill: parent
            }
            QPicture {
                source: "qrc:/Images/Images/Gradient_overlay.png"
                anchors.fill: parent
            }
            Column {
                spacing: 36
                anchors.centerIn: parent
                QIcon {
                    iconSize: 120
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
                        AppSetting.setIsStarted(true,false)
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
            width: eLEFT_WIDTH
            height: rootlogin.height
            QPicture {
                source: "qrc:/Images/Images/Gradient_bg.png"
                anchors.fill: parent
            }
            QPicture {
                source: "qrc:/Images/Images/Gradient_overlay.png"
                anchors.fill: parent
            }
            Column {
                spacing: 36
                anchors.centerIn: parent
                QIcon {
                    iconSize: 120
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
            width: eRIGHT_WIDTH
            height: rootlogin.height
            color: "#FFFFFF"
            // border.color: "red"
            Loader {
                id: contentLoader
                anchors.verticalCenter: parent.verticalCenter
                sourceComponent: whereMyComponent[whereAmI]
            }
        }
    }

    Component {
        id: accountAvailability
        Item {
            width: eRIGHT_WIDTH
            height: childrenRect.height
            Column {
                id: inputUserEmail
                spacing: 16
                anchors.horizontalCenter: parent.horizontalCenter
                QTextInputBoxTypeB {
                    id: emailaddrs
                    label: STR.STR_QML_394
                    boxWidth: 338
                    boxHeight: 48
                    input.placeholderText: STR.STR_QML_1500
                    onEnterKeyRequest: {
                        btnSignin.buttonClicked(null)
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
                id: buttonGroupSignin
                spacing: 16
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: inputUserEmail.bottom
                anchors.topMargin: 24
                QTextButton {
                    id: btnSignin
                    width: 338
                    height: 48
                    label.text: STR.STR_QML_097
                    label.font.pixelSize: 16
                    type: eTypeE
                    enabled: emailaddrs.textInputted !== "" && !processing
                    onButtonClicked: {
                        processing = true
                        var requestBody = {
                            "action"     : "account-availability",
                            "email"      : emailaddrs.textInputted
                        };
                        QMLHandle.sendEvent(EVT.EVT_LOGIN_ONLINE_SIGN_IN, requestBody)
                        processing = false
                    }
                }
                QTextButton {
                    id: btnGoogleSignin
                    width: 338
                    height: 48
                    label.text: "Sign in with Google"
                    label.font.pixelSize: 16
                    type: eTypeB
                    iconVisible: true
                    iconSource: [
                        "qrc:/Images/Images/GoogleLogo.png",
                        "qrc:/Images/Images/GoogleLogo.png"
                    ]
                    onButtonClicked: {
                        processing = true
                        var requestBody = {
                            "action"     : "account-availability-google",
                            "email"      : emailaddrs.textInputted
                        };
                        QMLHandle.sendEvent(EVT.EVT_LOGIN_ONLINE_SIGN_IN, requestBody)
                        processing = false
                    }
                }
                QTextButton {
                    id: btnAppleSignin
                    width: 338
                    height: 48
                    label.text: "Sign in with Apple"
                    label.font.pixelSize: 16
                    type: eTypeB
                    iconVisible: true
                    iconSource: [
                        "qrc:/Images/Images/AppleLogo.png",
                        "qrc:/Images/Images/AppleLogo.png"
                    ]
                    onButtonClicked: {
                        processing = true
                        var requestBody = {
                            "action"     : "account-availability-apple",
                            "email"      : emailaddrs.textInputted
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
                        QMLHandle.sendEvent(EVT.EVT_ROOT_SIGN_IN_PRIMARY_KEY_REQUEST)
                        processing = false
                    }
                }

                Item {
                    height: 24
                    width: 338
                    Row {
                        anchors.centerIn: parent
                        Rectangle {
                            height: 1
                            width: 150
                            color: "#C4C4C4"
                            anchors.verticalCenter: parent.verticalCenter
                        }
                        Item {
                            height: 24
                            width: 38
                            anchors.verticalCenter: parent.verticalCenter
                            QText {
                                font.family: "Lato"
                                font.pixelSize: 12
                                color: "#595959"
                                text: "OR"
                                anchors.centerIn: parent
                            }
                        }
                        Rectangle {
                            height: 1
                            width: 150
                            color: "#C4C4C4"
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }
                }

                QTextButton {
                    width: 338
                    height: 48
                    label.text: STR.STR_QML_420
                    label.font.pixelSize: 16
                    type: eTypeB
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
            }
            function processingAccountAvailabilityResult(https_code, error_code, error_msg){
                console.log(https_code, error_code, error_msg)
                if(https_code === DRACO_CODE.SUCCESSFULL){
                    if (error_code === DRACO_CODE.SIGN_IN_EXCEPTION) {
                        errorInfo.contentText = error_msg
                        errorInfo.open()
                    }
                    else if (error_code === DRACO_CODE.NOT_FOUND) {
                        // Username is not registed
                        // Create new account
                        if(emailaddrs.textInputted !== ""){
                            var newAccountInfo = {
                                "name"  : emailaddrs.textInputted,
                                "email" : emailaddrs.textInputted
                            };
                            QMLHandle.sendEvent(EVT.EVT_LOGIN_ONLINE_CREATE_ACCOUNT, newAccountInfo)
                        }
                    }
                    else if(error_code === DRACO_CODE.ACCOUNT_NOT_ACTIVATED) {
                        // User did not changes tempo password
                        // request change tempo password
                        whereAmI = eCHANGE_TEMPO_PASSWORD_OLD
                    }
                    else {
                        // User able to signin, request signin (with password input)
                        whereAmI = eSIGN_IN_OLD_ACCOUNT
                    }
                }
                else {
                    inputUserEmail.isValid = false
                    inputUserEmail.errorText = error_msg
                    inputUserEmail.showError = true;
                }
            }
            function processingCreateAccountResult(https_code, error_code, error_msg){
                if(https_code !== DRACO_CODE.SUCCESSFULL || error_code === DRACO_CODE.REGISTER_EXCEPTION){
                    commonError.visible = true
                    commonError.text = qsTr("https_code[%1] error_code[%2] error_msg[%3], %4").arg(https_code).arg(error_code).arg(error_msg).arg(STR.STR_QML_399)
                }
                else if(https_code === DRACO_CODE.SUCCESSFULL && error_code === DRACO_CODE.RESPONSE_OK){
                    whereAmI = eCHANGE_TEMPO_PASSWORD_NEW
                }
                else{

                }
            }
        }
    }
    Component {
        id: signinOldAccount
        Item {
            width: eRIGHT_WIDTH
            height: childrenRect.height
            QText {
                id: title
                width: 343
                anchors.horizontalCenter: parent.horizontalCenter
                verticalAlignment: Text.AlignVCenter
                text: STR.STR_QML_415
                font.family: "Lato"
                font.weight: Font.Bold
                font.pixelSize: 20
                color: "#031F2B"
            }
            Column {
                id: inputUserEmail
                spacing: 16
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: title.bottom
                anchors.topMargin: 24
                QTextInputBoxTypeB {
                    id: emailaddrs
                    label: STR.STR_QML_394
                    boxWidth: 338
                    boxHeight: 48
                    enabled: false
                    textInputted: Draco.emailRequested
                    input.placeholderText: STR.STR_QML_1500
                }
                QTextInputBoxTypeB {
                    id: passwd
                    label: STR.STR_QML_416
                    isPassword: true
                    boxWidth: 338
                    boxHeight: 48
                    isValid: true
                    input.placeholderText: STR.STR_QML_1501
                    onTextInputtedChanged: {
                        if(!passwd.isValid){
                            passwd.isValid = true
                            passwd.errorText = ""
                        }
                        passwd.showError = false
                    }
                    onEnterKeyRequest: {
                        btnSignin.buttonClicked(null)
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
                QIcon {
                    iconSize: 18
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
                    font.weight: Font.DemiBold
                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        onClicked: {Draco.stayLoggedIn = !Draco.stayLoggedIn}
                    }
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
                        whereAmI = eRESET_PASSWORD
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
                    type: eTypeE
                    enabled: emailaddrs.textInputted !== "" && passwd.textInputted !== "" && !processing
                    onButtonClicked: {
                        processing = true
                        var requestBody = {
                            "action"     : "login",
                            "email"      : emailaddrs.textInputted,
                            "password"   : passwd.textInputted,
                        };
                        QMLHandle.sendEvent(EVT.EVT_LOGIN_ONLINE_SIGN_IN, requestBody)
                        processing = false
                    }
                }
                QTextButton {
                    id: btnSigninDigitalKey
                    width: 338
                    height: 48
                    label.text: STR.STR_QML_1356
                    label.font.pixelSize: 16
                    type: eTypeB
                    visible: Draco.isSubscribed
                    onButtonClicked: {
                        processing = true
                        QMLHandle.sendEvent(EVT.EVT_SIGN_IN_VIA_XPUB_REQUEST)
                        processing = false
                    }
                }
                QButtonTextLink {
                    height: 20
                    label: STR.STR_QML_059
                    direction: eLEFT
                    anchors.horizontalCenter: parent.horizontalCenter
                    onButtonClicked: {
                        whereAmI = eACCOUNT_AVAILABILITY
                        passwd.errorText = ""
                        passwd.showError = false
                        backHandler(eSIGN_IN_OLD_ACCOUNT)
                    }
                }
            }

            function processingSigninResult(https_code, error_code, error_msg){
                if(https_code !== DRACO_CODE.SUCCESSFULL || error_code === DRACO_CODE.SIGN_IN_EXCEPTION){
                    if (error_code === DRACO_CODE.SIGN_IN_EXCEPTION) {
                        errorInfo.contentText = error_msg
                        errorInfo.open()
                    }
                    else {
                        emailaddrs.isValid = false
                        emailaddrs.errorText = error_msg
                        emailaddrs.showError = true;
                    }
                } else {
                    if(https_code === DRACO_CODE.SUCCESSFULL) {
                        if (error_code === DRACO_CODE.RESPONSE_OK) {
                            QMLHandle.sendEvent(EVT.EVT_NUNCHUK_LOGIN_SUCCEEDED)
                        }
                        else if (error_code === DRACO_CODE.LOGIN_NEW_DEVICE) {
                            commonError.visible = false
                            commonError.text = ""
                            whereAmI = eVERIFY_NEWDEVICE
                        }
                        else {
                            passwd.isValid = false
                            passwd.errorText = error_msg
                            passwd.showError = true
                        }
                    }
                    else {
                        passwd.isValid = false
                        passwd.errorText = error_msg
                    }
                }
            }
        }
    }
    Component {
        id: signinNewAccount
        Item {
            width: eRIGHT_WIDTH
            height: childrenRect.height
            Column {
                id: signinNewAccountItem
                readonly property int eINPUT_USERPASS: 0
                readonly property int eINPUT_USERNAME: 1
                         property int inputStep: eINPUT_USERPASS
                width: 343
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 24
                QText {
                    id: title
                    width: 343
                    anchors.horizontalCenter: parent.horizontalCenter
                    verticalAlignment: Text.AlignVCenter
                    text: STR.STR_QML_415
                    font.family: "Lato"
                    font.weight: Font.Bold
                    font.pixelSize: 20
                    color: "#031F2B"
                    visible: signinNewAccountItem.inputStep === signinNewAccountItem.eINPUT_USERPASS
                }
                Column {
                    width: parent.width
                    spacing: 16
                    visible: signinNewAccountItem.inputStep === signinNewAccountItem.eINPUT_USERPASS
                    QTextInputBoxTypeB {
                        id: emailaddrs
                        label: STR.STR_QML_394
                        boxWidth: 338
                        boxHeight: 48
                        enabled: false
                        textInputted: Draco.emailRequested
                    }
                    QTextInputBoxTypeB {
                        id: passwd
                        label: STR.STR_QML_416
                        isPassword: true
                        boxWidth: 338
                        boxHeight: 48
                        input.placeholderText: STR.STR_QML_1501
                        onTextInputtedChanged: {
                            if(!passwd.isValid){
                                passwd.isValid = true
                                passwd.errorText = ""
                            }
                            passwd.showError = false
                        }
                        onEnterKeyRequest: {
                            btnSignin.buttonClicked(null)
                        }
                    }

                    Row {
                        width: parent.width
                        Item {
                            width: parent.width/2
                            height: childrenRect.height
                            Row {
                                id: staySignedin
                                property bool isStaySignedIn: false
                                anchors.left: parent.left
                                spacing: 4
                                anchors.verticalCenter: parent.verticalCenter
                                QIcon {
                                    iconSize: 18
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
                                    font.weight: Font.DemiBold
                                    MouseArea {
                                        anchors.fill: parent
                                        hoverEnabled: true
                                        cursorShape: Qt.PointingHandCursor
                                        onClicked: {Draco.stayLoggedIn = !Draco.stayLoggedIn}
                                    }
                                }
                            }
                        }
                        Item {
                            width: parent.width/2
                            height: childrenRect.height
                            QText {
                                width: 98
                                height: 28
                                anchors.right: parent.right
                                anchors.verticalCenter: parent.verticalCenter
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
                                        whereAmI = eRESET_PASSWORD
                                    }
                                }
                            }
                        }
                    }
                }
                QTextInputBoxTypeB {
                    id: yourname
                    label: STR.STR_QML_393
                    boxWidth: 338
                    boxHeight: 48
                    visible: signinNewAccountItem.inputStep === signinNewAccountItem.eINPUT_USERNAME
                    input.placeholderText: STR.STR_QML_1502
                    onTextInputtedChanged: {
                        if(!passwd.isValid){
                            passwd.isValid = true
                            passwd.errorText = ""
                        }
                        passwd.showError = false
                    }
                    onEnterKeyRequest: {
                        btnContiue.buttonClicked(null)
                    }
                }

                Column {
                    id: buttonGroupSignin
                    spacing: 16
                    anchors.horizontalCenter: parent.horizontalCenter
                    QTextButton {
                        id: btnSignin
                        width: 338
                        height: 48
                        label.text: STR.STR_QML_419
                        label.font.pixelSize: 16
                        type: eTypeE
                        enabled: emailaddrs.textInputted !== "" && passwd.textInputted !== "" && !processing
                        visible: signinNewAccountItem.inputStep === signinNewAccountItem.eINPUT_USERPASS
                        onButtonClicked: {
                            signinNewAccountItem.inputStep = signinNewAccountItem.eINPUT_USERNAME
                        }
                    }
                    QTextButton {
                        id: btnContiue
                        width: 338
                        height: 48
                        label.text: STR.STR_QML_097
                        label.font.pixelSize: 16
                        type: eTypeE
                        enabled: emailaddrs.textInputted !== "" && passwd.textInputted !== "" && yourname.textInputted !== "" && !processing
                        visible: signinNewAccountItem.inputStep === signinNewAccountItem.eINPUT_USERNAME
                        onButtonClicked: {
                            processing = true
                            var requestBody = {
                                "action"     : "login",
                                "email"      : emailaddrs.textInputted,
                                "password"   : passwd.textInputted,
                                "username"   : yourname.textInputted
                            };
                            QMLHandle.sendEvent(EVT.EVT_LOGIN_ONLINE_SIGN_IN, requestBody)
                            processing = false
                        }
                    }
                    QButtonTextLink {
                        height: 20
                        label: STR.STR_QML_059
                        direction: eLEFT
                        anchors.horizontalCenter: parent.horizontalCenter
                        onButtonClicked: {
                            if(signinNewAccountItem.inputStep === signinNewAccountItem.eINPUT_USERNAME){
                                signinNewAccountItem.inputStep = signinNewAccountItem.eINPUT_USERPASS
                            }
                            else{
                                whereAmI = eACCOUNT_AVAILABILITY
                                passwd.errorText = ""
                                passwd.showError = false
                                backHandler(eSIGN_IN_NEW_ACCOUNT)
                            }
                        }
                    }
                }
            }
            function processingSigninResult(https_code, error_code, error_msg){
                if(https_code === DRACO_CODE.SUCCESSFULL && error_code === DRACO_CODE.RESPONSE_OK) {
                    QMLHandle.sendEvent(EVT.EVT_NUNCHUK_LOGIN_SUCCEEDED)
                }
            }
        }
    }
    Component {
        id: resetPassword
        Item {
            width: eRIGHT_WIDTH
            height: childrenRect.height
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
                onEnterKeyRequest: {
                    btnResetPassword.buttonClicked(null)
                }
            }
            Column {
                id: buttonGroupResetPassword
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
                    id: btnResetPassword
                    width: 338
                    height: 48
                    label.text: STR.STR_QML_410
                    label.font.pixelSize: 16
                    type: eTypeE
                    enabled: emailbackup.textInputted !== ""
                    onButtonClicked: {
                        commonError.visible = false
                        commonError.text = ""
                        QMLHandle.sendEvent(EVT.EVT_LOGIN_ONLINE_FORGOT_PASSWORD, emailbackup.textInputted)
                    }
                }
                QButtonTextLink {
                    height: 20
                    label: STR.STR_QML_059
                    direction: eLEFT
                    anchors.horizontalCenter: parent.horizontalCenter
                    onButtonClicked: {
                        commonError.visible = false
                        commonError.text = ""
                        whereAmI = eSIGN_IN_OLD_ACCOUNT
                        backHandler(eRESET_PASSWORD)
                    }
                }
            }

            function processingResetPasswordResult(https_code, error_code, error_msg){
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
        id: changeTempoPasswordOld
        Item {
            width: eRIGHT_WIDTH
            height: childrenRect.height
            QText {
                id: title
                width: 343
                anchors.horizontalCenter: parent.horizontalCenter
                verticalAlignment: Text.AlignVCenter
                text: STR.STR_QML_516
                font.family: "Lato"
                font.weight: Font.Bold
                font.pixelSize: 20
                color: "#031F2B"
            }
            QText {
                id: notifySentEmail
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: title.bottom
                anchors.topMargin: 12
                width: 338
                text: STR.STR_QML_400.arg(Draco.emailRequested)
                font.family: "Lato"
                font.pixelSize: 16
                color: "#031F2B"
                wrapMode: Text.WordWrap
                onLinkActivated: {
                    var requestBody = {
                        "action"  : "resend-password",
                        "email"   : Draco.emailRequested,
                    };
                    QMLHandle.sendEvent(EVT.EVT_LOGIN_ONLINE_SIGN_IN, requestBody)
                }
                MouseArea {
                    anchors.fill: parent
                    cursorShape: notifySentEmail.hoveredLink ? Qt.PointingHandCursor : Qt.ArrowCursor
                    acceptedButtons: Qt.NoButton
                }
            }
            Column {
                id: tempoPassword
                spacing: 16
                anchors.top: notifySentEmail.bottom
                anchors.topMargin: 24
                anchors.horizontalCenter: parent.horizontalCenter
                QTextInputBoxTypeB {
                    id: oldPassword
                    label: STR.STR_QML_401
                    isPassword: true
                    boxWidth: 338
                    boxHeight: 48
                }
                QTextInputBoxTypeB {
                    id: newPassword
                    label: STR.STR_QML_402
                    isPassword: true
                    boxWidth: 338
                    boxHeight: 48
                    passwordValidate: true
                    isValid: passwordValidateResult
                }
                QTextInputBoxTypeB {
                    id: confirmPassword
                    label: STR.STR_QML_403
                    isPassword: true
                    boxWidth: 338
                    boxHeight: 48
                    isValid: (textInputted === "") || (textInputted === newPassword.textInputted)
                    errorText: STR.STR_QML_404
                    onEnterKeyRequest: {
                        btnChangePassword.buttonClicked(null)
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
                id: buttonGroupChangePassword
                spacing: 16
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: tempoPassword.bottom
                anchors.topMargin: 24
                QTextButton {
                    id: btnChangePassword
                    width: 338
                    height: 48
                    label.text: STR.STR_QML_405
                    label.font.pixelSize: 16
                    type: eTypeE
                    enabled: (newPassword.textInputted !== "") && (newPassword.textInputted === confirmPassword.textInputted) && newPassword.isValid
                    onButtonClicked: {
                        var requestBody = {
                            "action"      : "login-tempo",
                            "email"       : Draco.emailRequested,
                            "oldPassword" : oldPassword.textInputted,
                            "newPassword" : newPassword.textInputted,
                        };
                        QMLHandle.sendEvent(EVT.EVT_LOGIN_ONLINE_CHANGE_PASSWORD, requestBody)
                    }
                }
                QButtonTextLink {
                    height: 20
                    label: STR.STR_QML_059
                    direction: eLEFT
                    anchors.horizontalCenter: parent.horizontalCenter
                    onButtonClicked: {
                        commonError.visible = false
                        commonError.text = ""
                        whereAmI = eACCOUNT_AVAILABILITY
                        backHandler(eCHANGE_TEMPO_PASSWORD_OLD)
                    }
                }
            }
            function processingChangePasswordResult(https_code, error_code, error_msg){
                if(https_code === DRACO_CODE.SUCCESSFULL && error_code === DRACO_CODE.RESPONSE_OK){
                    var requestBody = {
                        "action"      : "logout-tempo",
                        "email"       : Draco.emailRequested,
                    };
                    QMLHandle.sendEvent(EVT.EVT_LOGIN_ONLINE_CHANGE_PASSWORD, requestBody)
                    whereAmI = eSIGN_IN_OLD_ACCOUNT
                }
                else{
                    commonError.visible = true
                    commonError.text = qsTr("https_code[%1] error_code[%2] error_msg[%3], %4").arg(https_code).arg(error_code).arg(error_msg).arg(STR.STR_QML_407)
                }
            }
            function processingSigninResult(https_code, error_code, error_msg){
                if(https_code === DRACO_CODE.SUCCESSFULL && error_code === DRACO_CODE.RESPONSE_OK) {
                    var requestBody = {
                        "action"      : "change-password",
                        "oldPassword" : oldPassword.textInputted,
                        "newPassword" : newPassword.textInputted,
                    };
                    QMLHandle.sendEvent(EVT.EVT_LOGIN_ONLINE_CHANGE_PASSWORD, requestBody)
                }
            }
        }
    }
    Component {
        id: changeTempoPasswordNew
        Item {
            width: eRIGHT_WIDTH
            height: childrenRect.height
            QText {
                id: title
                width: 343
                anchors.horizontalCenter: parent.horizontalCenter
                verticalAlignment: Text.AlignVCenter
                text: STR.STR_QML_516
                font.family: "Lato"
                font.weight: Font.Bold
                font.pixelSize: 20
                color: "#031F2B"
            }
            QText {
                id: notifySentEmail
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: title.bottom
                anchors.topMargin: 12
                width: 338
                text: STR.STR_QML_422.arg(Draco.emailRequested)
                font.family: "Lato"
                font.pixelSize: 16
                color: "#031F2B"
                wrapMode: Text.WordWrap
                onLinkActivated: {
                    var requestBody = {
                        "action"  : "resend-password",
                        "email"   : Draco.emailRequested,
                    };
                    QMLHandle.sendEvent(EVT.EVT_LOGIN_ONLINE_SIGN_IN, requestBody)
                }
                MouseArea {
                    anchors.fill: parent
                    cursorShape: notifySentEmail.hoveredLink ? Qt.PointingHandCursor : Qt.ArrowCursor
                    acceptedButtons: Qt.NoButton
                }
            }
            Column {
                id: tempoPassword
                spacing: 16
                anchors.top: notifySentEmail.bottom
                anchors.topMargin: 24
                anchors.horizontalCenter: parent.horizontalCenter
                QTextInputBoxTypeB {
                    id: oldPassword
                    label: STR.STR_QML_401
                    isPassword: true
                    boxWidth: 338
                    boxHeight: 48
                }
                QTextInputBoxTypeB {
                    id: newPassword
                    label: STR.STR_QML_402
                    isPassword: true
                    boxWidth: 338
                    boxHeight: 48
                    passwordValidate: true
                    isValid: passwordValidateResult
                }
                QTextInputBoxTypeB {
                    id: confirmPassword
                    label: STR.STR_QML_403
                    isPassword: true
                    boxWidth: 338
                    boxHeight: 48
                    isValid: (textInputted === "") || (textInputted === newPassword.textInputted)
                    errorText: STR.STR_QML_404
                    onEnterKeyRequest: {
                        btnChangePassword.buttonClicked(null)
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
                id: buttonGroupChangePassword
                spacing: 16
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: tempoPassword.bottom
                anchors.topMargin: 24
                QTextButton {
                    id: btnChangePassword
                    width: 338
                    height: 48
                    label.text: STR.STR_QML_405
                    label.font.pixelSize: 16
                    type: eTypeE
                    enabled: (newPassword.textInputted !== "") && (newPassword.textInputted === confirmPassword.textInputted) && newPassword.isValid
                    onButtonClicked: {
                        var requestBody = {
                            "action"      : "change-password",
                            "enail"       : Draco.emailRequested,
                            "oldPassword" : oldPassword.textInputted,
                            "newPassword" : newPassword.textInputted,
                        };
                        QMLHandle.sendEvent(EVT.EVT_LOGIN_ONLINE_CHANGE_PASSWORD, requestBody)
                    }
                }
                QButtonTextLink {
                    height: 20
                    label: STR.STR_QML_059
                    direction: eLEFT
                    anchors.horizontalCenter: parent.horizontalCenter
                    onButtonClicked: {
                        commonError.visible = false
                        commonError.text = ""
                        whereAmI = eACCOUNT_AVAILABILITY
                        backHandler(eCHANGE_TEMPO_PASSWORD_NEW)
                    }
                }
            }
            function processingChangePasswordResult(https_code, error_code, error_msg){
                if(https_code === DRACO_CODE.SUCCESSFULL && error_code === DRACO_CODE.RESPONSE_OK){
                    whereAmI = eSIGN_IN_NEW_ACCOUNT
                }
                else{
                    commonError.visible = true
                    commonError.text = qsTr("https_code[%1] error_code[%2] error_msg[%3], %4").arg(https_code).arg(error_code).arg(error_msg).arg(STR.STR_QML_407)
                }
            }
        }
    }
    Component {
        id: recoverPassword
        Item {
            width: eRIGHT_WIDTH
            height: childrenRect.height
            QText {
                id: title
                width: 343
                anchors.horizontalCenter: parent.horizontalCenter
                verticalAlignment: Text.AlignVCenter
                text: STR.STR_QML_516
                font.family: "Lato"
                font.weight: Font.Bold
                font.pixelSize: 20
                color: "#031F2B"
            }
            QText {
                id: notifySentEmail
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: title.bottom
                anchors.topMargin: 12
                width: 338
                text: STR.STR_QML_400.arg(Draco.emailRequested)
                font.family: "Lato"
                font.pixelSize: 16
                color: "#031F2B"
                wrapMode: Text.WordWrap
                onLinkActivated: {
                    var requestBody = {
                        "action"  : "recover-password",
                        "email"   : Draco.emailRequested,
                    };
                    QMLHandle.sendEvent(EVT.EVT_LOGIN_ONLINE_SIGN_IN, requestBody)
                }
                MouseArea {
                    anchors.fill: parent
                    cursorShape: notifySentEmail.hoveredLink ? Qt.PointingHandCursor : Qt.ArrowCursor
                    acceptedButtons: Qt.NoButton
                }
            }
            Column {
                id: tempoPassword
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
                }
                QTextInputBoxTypeB {
                    id: newPassword
                    label: STR.STR_QML_402
                    isPassword: true
                    boxWidth: 338
                    boxHeight: 48
                    passwordValidate: true
                    isValid: passwordValidateResult
                }
                QTextInputBoxTypeB {
                    id: confirmPassword
                    label: STR.STR_QML_403
                    isPassword: true
                    boxWidth: 338
                    boxHeight: 48
                    isValid: textInputted === "" || textInputted === newPassword.textInputted
                    errorText: STR.STR_QML_404
                    onEnterKeyRequest: {
                        buttonRecoverPassword.buttonClicked(null)
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
                id: buttonGroupChangePassword
                spacing: 16
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: tempoPassword.bottom
                anchors.topMargin: 24
                QTextButton {
                    id: buttonRecoverPassword
                    width: 338
                    height: 48
                    label.text: STR.STR_QML_405
                    label.font.pixelSize: 16
                    type: eTypeE
                    enabled:  tokenPassword.textInputted !== "" && newPassword.textInputted !== "" && newPassword.textInputted === confirmPassword.textInputted
                    onButtonClicked: {
                        var requestBody = {
                            "token"   : tokenPassword.textInputted,
                            "passw"   : newPassword.textInputted,
                        };
                        QMLHandle.sendEvent(EVT.EVT_LOGIN_ONLINE_RECOVER_PASSWORD, requestBody)
                    }
                }
                QButtonTextLink {
                    height: 20
                    label: STR.STR_QML_059
                    direction: eLEFT
                    anchors.horizontalCenter: parent.horizontalCenter
                    onButtonClicked: {
                        commonError.visible = false
                        commonError.text = ""
                        whereAmI = eACCOUNT_AVAILABILITY
                        backHandler(eRECOVER_PASSWORD)
                    }
                }
            }

            function processingRecoverPasswordResult(https_code, error_code, error_msg){
                if(https_code !== DRACO_CODE.SUCCESSFULL || error_code === DRACO_CODE.RECOVER_PASSWORD_RESET_EXCEPTION || error_code === DRACO_CODE.RECOVER_PASSWORD_RESET_FAIL){
                    commonError.visible = true
                    commonError.text = qsTr("https_code[%1] error_code[%2] error_msg[%3], %4").arg(https_code).arg(error_code).arg(error_msg).arg(STR.STR_QML_414)
                }
                else if(https_code === DRACO_CODE.SUCCESSFULL && error_code === DRACO_CODE.RESPONSE_OK){
                    whereAmI = eSIGN_IN_OLD_ACCOUNT
                }
                else{
                    tokenPassword.isValid = false
                    tokenPassword.errorText = error_msg
                }
            }
            function processingResetPasswordResult(https_code, error_code, error_msg){
                // NOTHING
            }
        }
    }
    Component {
        id: verifyNewDevice
        Item {
            width: eRIGHT_WIDTH
            height: childrenRect.height
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
                onEnterKeyRequest: {
                    btnVerifyNewDevice.buttonClicked(null)
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
                    id: btnVerifyNewDevice
                    width: 338
                    height: 48
                    label.text: STR.STR_QML_427
                    label.font.pixelSize: 16
                    type: eTypeE
                    enabled: confirmCode.textInputted !== ""
                    onButtonClicked: {
                        commonError.visible = false
                        commonError.text = ""
                        QMLHandle.sendEvent(EVT.EVT_LOGIN_ONLINE_VERIFY_NEW_DEVICE, confirmCode.textInputted)
                    }
                }
                QButtonTextLink {
                    height: 20
                    label: STR.STR_QML_059
                    direction: eLEFT
                    anchors.horizontalCenter: parent.horizontalCenter
                    onButtonClicked: {
                        commonError.visible = false
                        commonError.text = ""
                        whereAmI = eSIGN_IN_OLD_ACCOUNT
                        backHandler(eVERIFY_NEWDEVICE)
                    }
                }
            }
            function processingVerifyNewDeviceResult(https_code, error_code, error_msg){
                if(https_code === DRACO_CODE.SUCCESSFULL && error_code === DRACO_CODE.RESPONSE_OK){
                    QMLHandle.sendEvent(EVT.EVT_NUNCHUK_LOGIN_SUCCEEDED)
                }
                else{
                    commonError.visible = true
                    commonError.text = qsTr("https_code[%1] error_code[%2] error_msg[%3]").arg(https_code).arg(error_code).arg(error_msg)
                }
            }
        }
    }
    Component {
        id: createAccount
        Item {
            id: createAccountItem
            width: eRIGHT_WIDTH
            height: childrenRect.height
            Column {
                id: inputUserEmail
                spacing: 16
                anchors.horizontalCenter: parent.horizontalCenter
                QTextInputBoxTypeB {
                    id: email
                    label: STR.STR_QML_394
                    boxWidth: 338
                    boxHeight: 48
                    isValid: true
                    anchors.horizontalCenter: parent.horizontalCenter
                    input.placeholderText: STR.STR_QML_1500
                    onTextInputtedChanged: {
                        if(!email.isValid){
                            email.isValid = true
                            email.errorText = ""
                        }
                        email.showError = false;
                    }
                    onEnterKeyRequest: {
                        btnCreateAccount.buttonClicked(null)
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
                    id: btnCreateAccount
                    width: 338
                    height: 48
                    label.text: STR.STR_QML_097
                    label.font.pixelSize: 16
                    type: eTypeE
                    enabled: email.textInputted !== ""
                    onButtonClicked: {
                        commonError.visible = false
                        commonError.text = ""
                        var newAccountInfo = {
                            "name"  : email.textInputted,
                            "email" : email.textInputted
                        };
                        QMLHandle.sendEvent(EVT.EVT_LOGIN_ONLINE_CREATE_ACCOUNT, newAccountInfo)
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
                        QMLHandle.sendEvent(EVT.EVT_ROOT_CREATE_PRIMARY_KEY_REQUEST,false)
                        processing = false
                    }
                }
                QTextButton {
                    width: 338
                    height: 48
                    label.text: STR.STR_QML_420
                    label.font.pixelSize: 16
                    type: eTypeB
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
            }
            function processingCreateAccountResult(https_code, error_code, error_msg){
                if(https_code !== DRACO_CODE.SUCCESSFULL || error_code === DRACO_CODE.REGISTER_EXCEPTION){
                    commonError.visible = true
                    commonError.text = qsTr("https_code[%1] error_code[%2] error_msg[%3], %4").arg(https_code).arg(error_code).arg(error_msg).arg(STR.STR_QML_399)
                }
                else if(https_code === DRACO_CODE.SUCCESSFULL && error_code === DRACO_CODE.RESPONSE_OK){
                    whereAmI = eCHANGE_TEMPO_PASSWORD_NEW
                }
                else{
                    email.isValid = false
                    email.errorText = error_msg
                    email.showError = true;
                }
            }
        }
    }
    QPopupInfo{
        id: errorInfo
    }
    Connections {
        target: Draco
        function onCreateAccountResult(https_code, error_code, error_msg) {
            contentLoader.item.processingCreateAccountResult(https_code, error_code, error_msg)
        }
        function onForgotPasswordResult(https_code, error_code, error_msg) {
            contentLoader.item.processingResetPasswordResult(https_code, error_code, error_msg)
        }
        function onChangePasswordResult(https_code, error_code, error_msg) {
            contentLoader.item.processingChangePasswordResult(https_code, error_code, error_msg)
        }
        function onSigninResult(https_code, error_code, error_msg) {
            contentLoader.item.processingSigninResult(https_code, error_code, error_msg)
        }
        function onRecoverPasswordResult(https_code, error_code, error_msg) {
            contentLoader.item.processingRecoverPasswordResult(https_code, error_code, error_msg)
        }
        function onVerifyNewDeviceResult(https_code, error_code, error_msg) {
            contentLoader.item.processingVerifyNewDeviceResult(https_code, error_code, error_msg)
        }
        function onResendVerifyNewDeviceCodeResult(https_code, error_code, error_msg) {
            console.log("onResendVerifyNewDeviceCodeResult", https_code, error_code, error_msg)
        }
        function onAccountAvailabilityResult(https_code, error_code, error_msg) {
            contentLoader.item.processingAccountAvailabilityResult(https_code, error_code, error_msg)
        }
        function onRequestCreateAccount() {
            whereAmI = eCREATE_ACCOUNT
        }
    }

    function backHandler(from){
        switch(from){
            case eSIGN_IN_OLD_ACCOUNT:
                Draco.isSubscribed = false
                Draco.emailRequested = ""
                break
            case eVERIFY_NEWDEVICE:
            case eRESET_PASSWORD:
            case eCHANGE_TEMPO_PASSWORD_OLD:
            case eCHANGE_TEMPO_PASSWORD_NEW:
            case eRECOVER_PASSWORD:
            case eONBOARDING_CREATE_ACCOUNT:
            case eONBOARDING_SIGN_IN:
            default:
                break
        }
    }
}
