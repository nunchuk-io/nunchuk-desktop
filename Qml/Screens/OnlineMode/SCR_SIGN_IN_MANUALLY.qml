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
import DRACO_CODE 1.0
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Chats"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../../localization/STR_QML.js" as STR

QScreen {
    readonly property int _USERNAME: 0
    readonly property int _SIGNATURE: 1
    property var _SCREENS: [enterUserName,enterSignature]
    property var whereIn: _USERNAME
    property var username: ""
    property var challengemessage:""
    Connections {
        target: AppModel
        onSignalShowToast:{
            _warning.open()
        }
    }
    Connections{
        target: Draco
        onSignalpkey_signin:{
            loadercontent.item.signalpkey_signin(https_code, error_code, error_msg)
        }
    }

    Loader {
        id: loadercontent
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        sourceComponent: _SCREENS[whereIn]
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

    Component{
        id:enterUserName
        QOnScreenContent {
            label.text: STR.STR_QML_633
            onCloseClicked: {
                QMLHandle.sendEvent(EVT.EVT_ONS_CLOSE_ALL_REQUEST)
            }
            QTextInputBoxTypeB {
                id: signername
                label: STR.STR_QML_647
                boxWidth: 540
                boxHeight: 48
                maxLength: 20
                enableLengthLimit: true
                textInputted: username
                anchors {
                    left: parent.left
                    leftMargin: 36
                    top: parent.top
                    topMargin: 100
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
                    QMLHandle.sendEvent(EVT.EVT_ONLINE_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_SIGN_IN_MANUALLY)
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
                enabled: signername.textInputted !== ""
                onButtonClicked: {
                    username = signername.textInputted
                    QMLHandle.sendEvent(EVT.EVT_PRIMARY_KEY_CHECK_USERNAME_REQUEST,username)
                }
            }
            function signalpkey_signin(https_code, error_code, error_msg){
                if(https_code === DRACO_CODE.SUCCESSFULL && error_code === DRACO_CODE.RESPONSE_OK){
                    QMLHandle.sendEvent(EVT.EVT_PRIMARY_KEY_ENTER_PASSPHRASE_SUCCEED)
                }
                else{
                    AppModel.setToast(-1,
                                   error_msg,
                                   EWARNING.ERROR_MSG,
                                   "");
                }
            }
        }
    }

    Component{
        id:enterSignature
        QOnScreenContent {
            width: popupWidth
            height: popupHeight
            anchors.centerIn: parent
            label.text: STR.STR_QML_633
            onCloseClicked: {
                QMLHandle.sendEvent(EVT.EVT_ONS_CLOSE_ALL_REQUEST)
            }
            Column{
                spacing: 24
                anchors {
                    left: parent.left
                    leftMargin: 36
                    top: parent.top
                    topMargin: 100
                }
                Row{
                    height:96
                    spacing: 13
                    QTextInputBoxTypeC  {
                        id: signatureInput
                        boxWidth: 539
                        boxHeight: 96
                        label: STR.STR_QML_648
                        enableLengthLimit:true
                        isDimmed: true
                        maxLength:80
                        mode:eREADONLY_MODE
                        textInputted: challengemessage
                        onCopyCompleted: {
                            AppModel.setToast(0,
                                              STR.STR_QML_668,
                                              EWARNING.SUCCESS_MSG,
                                              STR.STR_QML_668);
                        }
                    }
                    Item{
                        id:_item
                        width: 117
                        height: 48
                        anchors.verticalCenter: signatureInput.verticalCenter
                        anchors.verticalCenterOffset: 20
                        property bool loading: false
                        Row{
                            anchors.verticalCenter: parent.verticalCenter
                            spacing: 7
                            QText {
                                width: 55
                                font.family: "Lato"
                                font.pixelSize: 16
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                                text: STR.STR_QML_652
                            }
                            QImage{
                                height: 24
                                width: 24
                                source: "qrc:/Images/Images/cached_24px.png"
                                RotationAnimator on rotation {
                                    from: 0;
                                    to: 360;
                                    duration: 1000
                                    running: _item.loading
                                }
                            }
                        }
                        MouseArea{
                            anchors.fill: parent
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            onClicked: {
                                _item.loading = true
                                QMLHandle.sendEvent(EVT.EVT_CHALLENGE_MESSAGE_REFRESH_REQUEST,username)
                                _item.loading = false
                            }
                        }
                    }
                }


                QTextInputBoxTypeD  {
                    id: _signature
                    label: STR.STR_QML_649
                    boxWidth: 539
                    boxHeight: 178
                    isValid: true
                    onTypingFinished: {
                        if(!_signature.isValid){
                            _signature.isValid = true
                            _signature.errorText = ""
                        }
                        _signature.showError = false;
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
                    whereIn = _USERNAME
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
                label.text: STR.STR_QML_650
                label.font.pixelSize: 16
                type: eTypeE
                onButtonClicked: {
                    var manually = {
                        "username": username,
                        "signature": _signature.textInputted
                    }
                    QMLHandle.sendEvent(EVT.EVT_ADD_PRIMARY_KEY_ACCOUNT_REQUEST,manually)
                }
            }

            function signalpkey_signin(https_code, error_code, error_msg){
                if(https_code === DRACO_CODE.SUCCESSFULL && error_code === DRACO_CODE.RESPONSE_OK){
                    QMLHandle.sendEvent(EVT.EVT_ADD_PRIMARY_KEY_ACCOUNT_SUCCEED)
                }
                else{
                    AppModel.setToast(-1,
                                   error_msg,
                                   EWARNING.ERROR_MSG,
                                   "");
                }
            }
        }
    }

}

