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
import "./../../origins"
import "./../../customizes"
import "./../../customizes/Chats"
import "./../../customizes/Texts"
import "./../../customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

Item {
    anchors.fill: parent
    Flickable {
        anchors.fill: parent
        contentWidth: parent.width
        contentHeight: _netAcc.childrenRect.height
        visible: ClientController.isNunchukLoggedIn === true
        interactive: contentHeight > height
        clip: true
        flickableDirection: Flickable.VerticalFlick
        ScrollBar.vertical: ScrollBar { active: true }

        Column{
            id:_netAcc
            anchors{
                top:parent.top
                topMargin: 24
            }
            spacing: 24
            QText {
                anchors{
                    left: parent.left
                    leftMargin: 24
                }
                font.family: "Lato"
                font.pixelSize: 28
                font.weight: Font.Bold
                text: STR.STR_QML_537
            }
            QLine {
                width: 627
                anchors{
                    left: parent.left
                    leftMargin: 24
                }
                visible: ClientController.user.isSubscribedUser
            }
            Item {
                width: 627
                height: 48
                anchors{
                    left: parent.left
                    leftMargin: 24
                }
                visible: ClientController.user.isSubscribedUser
                QLato {
                    anchors {
                        left: parent.left
                        verticalCenter: parent.verticalCenter
                    }
                    font.pixelSize: 20
                    font.weight: Font.Bold
                    text: STR.STR_QML_1303
                }
                QTextButton {
                    anchors {
                        right: parent.right
                        verticalCenter: parent.verticalCenter
                    }
                    width: 128
                    height: 48
                    label.text: STR.STR_QML_1304
                    label.font.pixelSize: 16
                    type: eTypeB
                    onButtonClicked: {
                        var screenObj = { "state_id" : EVT.STATE_ID_SCR_CHANGE_EMAIL }
                        QMLHandle.sendEvent(EVT.EVT_REENTER_YOUR_PASSWORD_REQUEST,screenObj)
                    }
                }
            }

            QLine {
                width: 627
                anchors{
                    left: parent.left
                    leftMargin: 24
                }
                visible: ClientController.user.isSubscribedUser
            }

            Item{
                visible: ClientController.user.isPrimaryKey
                width: 627
                height: 48 + 24
                anchors{
                    left: parent.left
                    leftMargin: 24
                }
                QText {
                    anchors{
                        bottom: parent.bottom
                        bottomMargin: 10
                    }
                    font.family: "Lato"
                    font.pixelSize: 20
                    font.weight: Font.Bold
                    text: STR.STR_QML_641
                }
                QTextButton {
                    id: _replaceBtn
                    anchors{
                        bottom: parent.bottom
                        bottomMargin: 0
                    }
                    width: 200
                    height: 48
                    label.text: STR.STR_QML_657
                    label.font.pixelSize: 16
                    type: eTypeB
                    anchors.right: parent.right
                    enabled: AppModel.primaryKey
                    onButtonClicked: {
                        QMLHandle.sendEvent(EVT.EVT_SHOW_REPLACE_PRIMARY_KEY_REQUEST)
                    }
                }
            }

            Item{
                id: changePassword
                visible: !ClientController.user.isPrimaryKey
                width: 651
                height: 28+ 20 + currentpassword.height + newpassword.height + confirmPassword.height + updateBtn.height + 10 * 5 + 24*2
                Column {
                    anchors.fill: parent
                    anchors.margins: 24
                    spacing: 10
                    QText {
                        font.family: "Lato"
                        font.pixelSize: 20
                        font.weight: Font.Bold
                        text: STR.STR_QML_525
                    }
                    QTextInputBoxTypeB {
                        id: currentpassword
                        label: STR.STR_QML_526
                        isPassword: true
                        boxWidth: 627
                        boxHeight: 48
                        isValid: true
                        onTextInputtedChanged: {
                            if(!currentpassword.isValid){
                                currentpassword.isValid = true
                                currentpassword.errorText = ""
                            }
                            currentpassword.showError = false
                        }
                    }
                    QTextInputBoxTypeB {
                        id: newpassword
                        label: STR.STR_QML_402
                        isPassword: true
                        boxWidth: 627
                        boxHeight: 48
                        isValid: true
                        onTextInputtedChanged: {
                            if(!newpassword.isValid){
                                newpassword.isValid = true
                                newpassword.errorText = ""
                            }
                            newpassword.showError = false
                        }
                    }
                    QTextInputBoxTypeB {
                        id: confirmPassword
                        label: STR.STR_QML_403
                        isPassword: true
                        boxWidth: 627
                        boxHeight: 48
                        isValid: textInputted === "" || textInputted === newpassword.textInputted
                        errorText: STR.STR_QML_404
                    }
                    QTextButton {
                        id: updateBtn
                        width: 200
                        height: 48
                        label.text: STR.STR_QML_527
                        label.font.pixelSize: 16
                        type: eTypeE
                        anchors.right: parent.right
                        onButtonClicked: {
                            if(newpassword.textInputted === currentpassword.textInputted ||
                                    newpassword.textInputted === "" ||
                                    currentpassword.textInputted === ""){
                                if(newpassword.textInputted === ""){
                                    newpassword.isValid = false
                                    newpassword.errorText = STR.STR_QML_396
                                    newpassword.showError = true
                                }
                                if(currentpassword.textInputted === ""){
                                    currentpassword.isValid = false
                                    currentpassword.errorText = STR.STR_QML_396
                                    currentpassword.showError = true
                                }
                                else if(newpassword.textInputted === currentpassword.textInputted){
                                    newpassword.isValid = false
                                    currentpassword.isValid = false
                                    currentpassword.errorText = STR.STR_QML_406
                                    currentpassword.showError = true
                                }
                            }
                            else{
                                var requestBody = {
                                    "oldPassword" : currentpassword.textInputted,
                                    "newPassword" : newpassword.textInputted,
                                };
                                QMLHandle.sendEvent(EVT.EVT_SETTING_ACCOUNT_CHANGE_PASSWORD, requestBody)
                            }
                        }
                    }
                }
                function processingChangePasswordResult(https_code, error_code, error_msg){
                    if(https_code === DRACO_CODE.SUCCESSFULL && error_code === DRACO_CODE.RESPONSE_OK){
                        AppModel.showToast(0, STR.STR_QML_545, EWARNING.SUCCESS_MSG);
                    }
                    else{
                        if(https_code === DRACO_CODE.SUCCESSFULL){
                            currentpassword.isValid = false
                            currentpassword.errorText = error_msg
                            currentpassword.showError = true
                        }
                    }
                }
            }
            Rectangle {
                width: 627
                height: 1
                color: "#EAEAEA"
                anchors.left: parent.left
                anchors.leftMargin: 24
                visible: false
            }
            Item {
                id: loggedInDevice
                width: 651
                height: loggedBox.height + 74

                Column {
                    anchors.fill: parent
                    anchors.margins: 24
                    spacing: 14
                    Item{
                        width: 627
                        height: 20
                        QText {
                            anchors.left: parent.left
                            font.family: "Lato"
                            font.pixelSize: 20
                            font.weight: Font.Bold
                            text: STR.STR_QML_517
                        }
                        QText {

                            anchors.right: parent.right
                            font.family: "Lato"
                            font.pixelSize: 16
                            font.weight: Font.Bold
                            color: "#CF4018"
                            text: STR.STR_QML_547
                            scale: btnSignOutAll.containsMouse ? 1.1 : 1.0
                            MouseArea {
                                id: btnSignOutAll
                                hoverEnabled: true
                                cursorShape: Qt.PointingHandCursor
                                anchors.fill: parent
                                onClicked: {
                                    loggedBox.typePopup = loggedBox._SignOutAll
                                    _loggerDevice.contentText = STR.STR_QML_578
                                    _loggerDevice.open()
                                }
                            }
                        }
                    }

                    QLoggedInDevicesBox{
                        id: loggedBox
                        width: 627
                        boxHeight: 48*5
                        onSignOutClicked: {
                            loggedBox.device_id = id
                            loggedBox.device_name = name
                            loggedBox.typePopup = _SignOutSingle
                            _loggerDevice.contentText = STR.STR_QML_576.arg(name)
                            _loggerDevice.open()
                        }
                        onMarkClicked: {
                            loggedBox.device_id = id
                            loggedBox.device_name = name
                            loggedBox.typePopup = _MarkAsCompromised
                            _loggerDevice.contentText = STR.STR_QML_577.arg(name)
                            _loggerDevice.open()

                        }
                    }
                }

                function loggedInDeviceChanged(https_code, error_code, error_msg){
                    if(https_code === DRACO_CODE.SUCCESSFULL && error_code === DRACO_CODE.RESPONSE_OK){
                        var ret = ""
                        switch(loggedBox.typePopup){
                        case loggedBox._SignOutSingle:
                            ret = STR.STR_QML_581.arg(loggedBox.device_name)
                            break
                        case loggedBox._SignOutAll:
                            ret = STR.STR_QML_580
                            break
                        case loggedBox._MarkAsCompromised:
                            ret = STR.STR_QML_579.arg(loggedBox.device_name)
                            break
                        default:break
                        }
                        if(ret !== ""){
                            AppModel.showToast(0, ret, EWARNING.SUCCESS_MSG);
                        }


                    }
                    else{
                        if(https_code === DRACO_CODE.SUCCESSFULL){

                        }
                    }
                }
            }
            Rectangle {
                width: 627
                height: 1
                color: "#EAEAEA"
                anchors.left: parent.left
                anchors.leftMargin: 24
                visible: false // Hide for all user
            }

            Item{
                width: 627
                height: 25*2 + 48
                anchors.left: parent.left
                anchors.leftMargin: 24
                visible: false // Hide for all user
                QText {
                    width: 309
                    height: 28
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    horizontalAlignment: Text.AlignLeft
                    text: STR.STR_QML_593
                    font.family: "Lato"
                    font.pixelSize: 20
                    font.weight: Font.Bold
                    color: "#031F2B"
                }

                QSwitchTypeB {
                    id: multiSyncDevice
                    anchors.right: parent.right
                    width: 84
                    height: 48
                    switchOn: AppSetting.enableMultiDeviceSync
                    anchors.verticalCenter: parent.verticalCenter
                    onSwitchOnChanged: {
                        AppSetting.enableMultiDeviceSync = multiSyncDevice.switchOn
                    }
                }
            }

            Rectangle {
                width: 627
                height: 1
                color: "#EAEAEA"
                anchors.left: parent.left
                anchors.leftMargin: 24
            }
            Item {
                id: deleteAccount
                width: 651
                height: 124
                Column {
                    anchors.fill: parent
                    anchors.margins: 24
                    spacing: 16
                    QText {
                        font.family: "Lato"
                        font.pixelSize: 20
                        font.weight: Font.Bold
                        text: STR.STR_QML_539
                    }
                    Item{
                        width: parent.width
                        height: 28
                        QText {
                            width: 309
                            height: 28
                            anchors.left: parent.left
                            horizontalAlignment: Text.AlignLeft
                            text: STR.STR_QML_540
                            font.family: "Lato"
                            font.pixelSize: 16
                            color: "#031F2B"
                        }
                        QText {
                            width: 108
                            height: 20
                            anchors{
                                right: parent.right
                                verticalCenter: parent.verticalCenter
                            }
                            horizontalAlignment: Text.AlignLeft
                            text: STR.STR_QML_539
                            font.family: "Lato"
                            font.pixelSize: 16
                            color: "#CF4018"
                            enabled: ClientController.user.isPrimaryKey ? AppModel.primaryKey : true
                            opacity: enabled ? 1.0 : 0.3
                            MouseArea {
                                id: btnMouseDeleteccount
                                hoverEnabled: true
                                cursorShape: Qt.PointingHandCursor
                                anchors.fill: parent
                                onClicked: {
                                    if(ClientController.user.isPrimaryKey){
                                        accountConfirmDeletePrimaryKey.open()
                                    }
                                    else{
                                        accountConfirmDelete.open()
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    Item {
        id: addAccount
        width: 651
        height: 124
        anchors.top:parent.top
        visible: ClientController.isNunchukLoggedIn === false
        Column {
            anchors.fill: parent
            anchors.margins: 24
            spacing: 24
            QText {
                font.family: "Lato"
                font.pixelSize: 28
                font.weight: Font.Bold
                text: STR.STR_QML_537
            }
            QText {
                font.family: "Lato"
                font.pixelSize: 20
                font.weight: Font.Bold
                text: STR.STR_QML_538
            }
            Item{
                width: parent.width
                height: 28
                QText {
                    width: 309
                    height: 28
                    anchors.left: parent.left
                    horizontalAlignment: Text.AlignLeft
                    text: STR.STR_QML_541
                    font.family: "Lato"
                    font.pixelSize: 16
                    color: "#031F2B"
                }
                QText {
                    width: 108
                    height: 20
                    anchors{
                        right: parent.right
                        verticalCenter: parent.verticalCenter
                    }
                    horizontalAlignment: Text.AlignLeft
                    text: STR.STR_QML_538
                    font.family: "Lato"
                    font.pixelSize: 16
                    color: "#CF4018"
                    MouseArea {
                        id: btnMouseAddccount
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        anchors.fill: parent
                        onClicked: {
                            QMLHandle.sendEvent(EVT.EVT_LOGIN_MATRIX_REQUEST)
                        }
                    }
                }
            }
        }
    }

    QConfirmYesNoPopup {
        id: _loggerDevice
        width: rootSettings.width
        height: rootSettings.width
        anchors.centerIn: rootSettings
        contentText: ""
        onConfirmNo: close()
        onConfirmYes: {
            close()
            switch(loggedBox.typePopup){
            case loggedBox._SignOutSingle:
                Draco.signoutDeice(loggedBox.device_id)
                break
            case loggedBox._SignOutAll:
                Draco.signoutAllDeices()
                break
            case loggedBox._MarkAsCompromised:
                Draco.markAsCompromised(loggedBox.device_id)
                break
            default:break
            }
        }
    }
    Connections {
        target: Draco

        onChangePasswordResult: {
            changePassword.processingChangePasswordResult(https_code, error_code, error_msg)
        }
        onLoggedInDeviceChanged:{
            loggedInDevice.loggedInDeviceChanged(https_code, error_code, error_msg)
        }
    }
}
