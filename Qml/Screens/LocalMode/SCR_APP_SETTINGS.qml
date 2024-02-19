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
import QtQuick.Controls.Styles 1.3
import QtGraphicalEffects 1.0
import Qt.labs.platform 1.1
import HMIEVENTS 1.0
import NUNCHUCKTYPE 1.0
import QRCodeItem 1.0
import DataPool 1.0
import DRACO_CODE 1.0
import EWARNING 1.0
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Chats"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../Components/customizes/Popups"
import "../../Components"
import "../../Components/LeftPannel"
import "../../Components/RightPannel"
import "../../../localization/STR_QML.js" as STR

QScreen {
    id: rootSettings
    readonly property int _DISPLAY_UNIT: 0
    readonly property int _LOCAL_CURRENCY: 1
    readonly property int _ACCOUNT_SETTINGS: 2
    readonly property int _NETWORK_SETTINGS: 3
    readonly property int _HARDWARE_DRIVER: 4
    readonly property int _DATABASE_ENCRYTION: 5
    readonly property int _DEVELOPER_SETTINGS: 6
    readonly property int _ABOUT: 7


    function closeRequest(to) {
        if(pannel_right.rightContent.anyChanged){
            modelConfirmUpdateSetting.requestTo = to
            modelConfirmUpdateSetting.open()
            return false
        }
        return true
    }
    function applySettings() {
        pannel_right.rightContent.applySettings()
    }
    Row {
        anchors.fill: parent
        QLeftPannel {
            id: pannel_left
            left_pannel: QSettingLeftPannel {}
        }
        QRightPannel {
            id: pannel_right
            width: parent.width - pannel_left.width
            right_pannel: QSettingRightPannel {}
        }
    }

    QConfirmDeletingTypeA {
        id: accountConfirmDelete
        modalWidth: 488
        modalHeight: 317
        label: STR.STR_QML_024
        content: STR.STR_QML_542
        leftBtnLabel: STR.STR_QML_432
        rightBtnLabel: STR.STR_QML_433
        onDeleteRequest: {
            QMLHandle.sendEvent(EVT.EVT_APP_SETTING_DELETE_ACCOUNT_REQUEST,ClientController.user.isPrimaryKey)
            if(!ClientController.user.isPrimaryKey){
                accountConfirmCodeDelete.open()
            }
        }
    }

    QConfirmDeletingTypeB {
        id: accountConfirmDeletePrimaryKey
        modalWidth: 320
        modalHeight: 386
        label: STR.STR_QML_024
        content_1: STR.STR_QML_672
        content_2: STR.STR_QML_673
        leftBtnLabel: STR.STR_QML_035
        rightBtnLabel: STR.STR_QML_427
        onDeleteRequest: {
            QMLHandle.sendEvent(EVT.EVT_APP_SETTING_DELETE_ACCOUNT_REQUEST,ClientController.user.isPrimaryKey)
            accountConfirmDeletePrimaryKey.close()
        }
    }

    QConfirmCodeDeletion {
        id: accountConfirmCodeDelete
        modalWidth: 600
        modalHeight: 416
        label: STR.STR_QML_539
        content: STR.STR_QML_544.arg(ClientController.user.email)
        leftBtnLabel: STR.STR_QML_245
        rightBtnLabel: STR.STR_QML_543
        placeholderText: ""
        onDeleteRequest: {
            if(Draco.sendDeleteConfirmation(emailCode.textInputted)){
                accountConfirmCodeDelete.close()
                ClientController.requestSignout()
            }else{
                emailCode.isValid = false
                errorConfirm.open()
            }
        }
    }

    QPopupInfo{
        id: errorConfirm
        title: STR.STR_QML_594
        contentText: STR.STR_QML_595
        btnLabel: STR.STR_QML_596
    }

    Popup {
        id: pleasewait
        width: parent.width
        height: parent.height
        modal: true
        focus: true
        background: Item{}
        Rectangle {
            id: pleasewaitContent
            width: 340
            height: 56
            color: "#FFFFFF"
            radius: 8
            anchors.centerIn: parent
            QText {
                anchors.centerIn: parent
                horizontalAlignment: Text.AlignHCenter
                text: STR.STR_QML_190
                color: "#031F2B"
                font.weight: Font.DemiBold
                font.family: "Montserrat"
                font.pixelSize: 14
            }
        }
        DropShadow {
            anchors.fill: pleasewaitContent
            horizontalOffset: 3
            verticalOffset: 3
            radius: 8.0
            samples: 17
            color: "#80000000"
            source: pleasewaitContent
        }
    }
    // ModelConfirm settting
    Popup {
        id: modelConfirmUpdateSetting
        property int requestTo: -1
        width: parent.width
        height: parent.height
        modal: true
        focus: true
        background: Item{}
        Rectangle {
            id: modelConfirmUpdateSettingmask
            width: 488
            height: 250
            radius: 8
            color: "#FFFFFF"
            anchors.centerIn: parent
            QText {
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: 51
                horizontalAlignment: Text.AlignHCenter
                text: STR.STR_QML_191
                color: "#031F2B"
                font.family: "Montserrat"
                font.weight: Font.DemiBold
                font.pixelSize: 24
            }
            QText {
                width: 355
                height: 63
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: 91
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignHCenter
                text: STR.STR_QML_192
                font.family: "Lato"
                font.pixelSize: 14
            }
            Row {
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 50
                spacing: 16
                QTextButton {
                    width: 100
                    height: 32
                    label.text: STR.STR_QML_193
                    label.font.pixelSize: 10
                    type: eTypeB
                    onButtonClicked: {
                        modelConfirmUpdateSetting.close()
                        if(modelConfirmUpdateSetting.requestTo === AppModel.tabIndex){
                            QMLHandle.sendEvent(EVT.EVT_APP_SETTING_BACK_REQUEST)
                        }
                        else{
                            if(NUNCHUCKTYPE.CHAT_TAB === modelConfirmUpdateSetting.requestTo){
                                QMLHandle.sendEvent(EVT.EVT_STARTING_APPLICATION_ONLINEMODE)
                            }
                            else {
                                if(0 === AppModel.nunchukMode){
                                    QMLHandle.sendEvent(EVT.EVT_STARTING_APPLICATION_LOCALMODE)
                                }
                                else{
                                    QMLHandle.sendEvent(EVT.EVT_STARTING_APPLICATION_ONLINE_HWL)
                                }
                            }
                        }
                    }
                }
                QTextButton {
                    width: 100
                    height: 32
                    label.text: STR.STR_QML_194
                    label.font.pixelSize: 10
                    type: eTypeA
                    onButtonClicked: {
                        rootSettings.applySettings()
                        modelConfirmUpdateSetting.close()
                        if (AppModel.updateSettingRestartRequired()) {
                            modelRestartApp.open()
                        } else {
                            if(modelConfirmUpdateSetting.requestTo === AppModel.tabIndex){
                                QMLHandle.sendEvent(EVT.EVT_APP_SETTING_BACK_REQUEST)
                            }
                            else{
                                if(NUNCHUCKTYPE.CHAT_TAB === modelConfirmUpdateSetting.requestTo){
                                    QMLHandle.sendEvent(EVT.EVT_STARTING_APPLICATION_ONLINEMODE)
                                } else if (NUNCHUCKTYPE.SERVICE_TAB === modelConfirmUpdateSetting.requestTo) {
                                    QMLHandle.sendEvent(EVT.EVT_GOTO_SERVICE_SETTING_TAB)
                                } else if (NUNCHUCKTYPE.WALLET_TAB === modelConfirmUpdateSetting.requestTo) {
                                    if(0 === AppModel.nunchukMode){
                                        QMLHandle.sendEvent(EVT.EVT_STARTING_APPLICATION_LOCALMODE)
                                    }
                                    else{
                                        QMLHandle.sendEvent(EVT.EVT_STARTING_APPLICATION_ONLINE_HWL)
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        DropShadow {
            anchors.fill: modelConfirmUpdateSettingmask
            horizontalOffset: 3
            verticalOffset: 3
            radius: 8.0
            samples: 17
            color: "#80000000"
            source: modelConfirmUpdateSettingmask
        }
    }
    // ModelConfirm restart
    Popup {
        id: modelRestartApp
        width: parent.width
        height: parent.height
        modal: true
        focus: true
        background: Item{}
        Rectangle {
            id: modelRestartAppmask
            width: 488
            height: 250
            radius: 8
            color: "#FFFFFF"
            anchors.centerIn: parent
            layer.enabled: true
            layer.effect: OpacityMask {
                maskSource: Rectangle {
                    width: 488
                    height: 250
                    radius: 8
                }
            }
            QText {
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: 51
                horizontalAlignment: Text.AlignHCenter
                text: STR.STR_QML_195
                color: "#031F2B"
                font.family: "Montserrat"
                font.weight: Font.DemiBold
                font.pixelSize: 24
            }
            QText {
                width: 355
                height: 63
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: 91
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignHCenter
                text: STR.STR_QML_196
                font.family: "Lato"
                font.pixelSize: 14
            }
            QTextButton {
                width: 100
                height: 32
                label.text: STR.STR_QML_197
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 50
                label.font.pixelSize: 10
                type: eTypeB
                onButtonClicked: {
                    modelRestartApp.close()
                    QMLHandle.sendEvent(EVT.EVT_APP_SETTING_REQUEST_RESTART)
                }
            }
        }
        DropShadow {
            anchors.fill: modelRestartAppmask
            horizontalOffset: 3
            verticalOffset: 3
            radius: 8.0
            samples: 17
            color: "#80000000"
            source: modelRestartAppmask
        }
    }

}
