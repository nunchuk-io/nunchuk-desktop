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
import QtQuick.Controls 2.1
import QtQuick.Controls 1.4
import QtGraphicalEffects 1.0
import QtQuick.Controls.Styles 1.4
import Qt.labs.platform 1.1
import HMIEVENTS 1.0
import NUNCHUCKTYPE 1.0
import QRCodeItem 1.0
import DataPool 1.0
import DRACO_CODE 1.0
import EWARNING 1.0
import "../../../origins"
import "../../../customizes"
import "../../../customizes/Chats"
import "../../../customizes/Texts"
import "../../../customizes/Buttons"
import "../../../customizes/services"
import "../Common"
import "../../../../../localization/STR_QML.js" as STR

Item {
    property var inheritancePlanInfo: ServiceSetting.walletInfo.inheritancePlanInfo
    property var planInfo: inheritancePlanInfo.planInfo
    property string walletName: ServiceSetting.walletInfo.walletName
    QContextMenu {
        id: optionMenu
        menuWidth: 300
        icons: [
            "qrc:/Images/Images/close-24px.svg"
        ]
        labels: [
            STR.STR_QML_844
        ]
        colors: [
            "#CF4018"
        ]
        onItemClicked: {
            switch(index){
            case 0:
                var _input = {
                    option : "cancel-inheritance-plan"
                }
                QMLHandle.sendEvent(EVT.EVT_INHERITANCE_PLAN_FINALIZE_REQUEST, _input)
                break;
            default:
                break;
            }
        }
    }
    Rectangle {
        id: _img_bg
        width: parent.width
        height: parent.height * 0.5
        color: "#D0E2FF"
        QIconButton{
            id:_more
            anchors{
                top: parent.top
                topMargin: 24
                right: parent.right
                rightMargin: 24
            }
            width: 48
            height: 48
            bgColor: parent.color
            visible: inheritancePlanInfo.isActived
            icon: "qrc:/Images/Images/more-horizontal-dark.svg"
            onClicked: {
                optionMenu.popup()
            }
        }
    }
    Column {
        anchors.fill: parent
        anchors.margins: 24
        spacing: 28
        QText {
            font.family: "Lato"
            font.pixelSize: 28
            color: "#031F2B"
            font.weight: Font.Bold
            text: STR.STR_QML_843
        }
        Rectangle {
            width: parent.width
            height: _img_bg.height * 0.75
            radius: 24
            color: "#2F466C"
            Column {
                anchors.fill: parent
                anchors.margins: 24
                spacing: 24
                Item {
                    width: parent.width
                    height: 60
                    Row {
                        anchors.fill: parent
                        spacing: 12
                        QImage {
                            width: 60
                            height: 60
                            anchors.verticalCenter: parent.verticalCenter
                            source: "qrc:/Images/Images/wallet-brand-icon.svg"
                        }
                        Column {
                            width: 304
                            height: 44
                            anchors.verticalCenter: parent.verticalCenter
                            spacing: 4
                            QLato {
                                font.weight: Font.Bold
                                font.pixelSize: 16
                                color: "#FFFFFF"
                                text: STR.STR_QML_845
                            }
                            QLato {
                                color: "#FFFFFF"
                                text: walletName
                            }
                        }
                    }
                    Row {
                        anchors.fill: parent
                        layoutDirection: Qt.RightToLeft
                        spacing: 12
                        QTextButton {
                            width: 162
                            height: 48
                            label.text: STR.STR_QML_847
                            label.font.pixelSize: 16
                            anchors.verticalCenter: parent.verticalCenter
                            type: eTypeC
                            onButtonClicked: {
                                QMLHandle.sendEvent(EVT.EVT_SHARE_YOUR_SECRET_REQUEST)
                            }
                        }
                        QTextLink {
                            width: 220
                            height: 48
                            font.weight: Font.Bold
                            font.pixelSize: 16
                            color: "#FFFFFF"
                            text: STR.STR_QML_846
                            anchors.verticalCenter: parent.verticalCenter
                            font.underline: false
                            onTextClicked: {
                                Qt.openUrlExternally("https://nunchuk.io/howtoclaim")
                            }
                        }
                    }
                }
                QTextInputBoxTypeE {
                    width: parent.width
                    icon: "qrc:/Images/Images/calendar-dark.png"
                    input.text: planInfo.activation_date
                    input.verticalAlignment: Text.AlignVCenter
                    onTextEditClicked: {
                        QMLHandle.sendEvent(EVT.EVT_EDIT_YOUR_INHERITANCE_PLAN_REQUEST, ServiceType.IE_ACTIVATION_DATE)
                    }
                }
                Item {
                    width: parent.width
                    height: 104
                    Row {
                        spacing: 24
                        QTextInputBoxTypeE {
                            width: 387
                            height: 104
                            icon: "qrc:/Images/Images/star-dark.png"
                            edit.visible: false
                            label.text: STR.STR_QML_749
                            input.text: planInfo.magic
                            input.verticalAlignment: Text.AlignTop
                            input.height: 68
                        }
                        QTextInputBoxTypeE {
                            width: 387
                            height: 104
                            icon: "qrc:/Images/Images/change-password-dark.svg"
                            edit.visible: false
                            label.text: STR.STR_QML_727
                            input.text: STR.STR_QML_917
                            input.verticalAlignment: Text.AlignTop
                            input.height: 68
                        }
                    }
                }
            }
        }
        Item {
            width: parent.width - 24
            height: parent.height * 0.58 - 24
            Flickable {
                anchors.top: parent.top
                anchors.topMargin: 12
                clip: true
                width: 661
                height: 372
                contentWidth: width
                contentHeight: _colum.childrenRect.height + 100
                ScrollBar.vertical: ScrollBar { active: true }
                Column {
                    id: _colum
                    anchors.top: parent.top
                    anchors.topMargin: 24
                    width: parent.width
                    spacing: 24
                    QTextAreaBoxTypeB {
                        width: 651
                        height: 128
                        label.text: STR.STR_QML_850
                        input.text: planInfo.note
                        input.backgroundColor: "#F5F5F5"
                        input.verticalAlignment: Text.AlignTop
                        input.height: 96
                        input.readOnly: true
                        textColor: "#031F2B"
                        onTextEditClicked: {
                            QMLHandle.sendEvent(EVT.EVT_EDIT_YOUR_INHERITANCE_PLAN_REQUEST, ServiceType.IE_LEAVE_MESSAGE)
                        }
                    }
                    Rectangle {
                        height: 1
                        width: 651
                        color: "#EAEAEA"
                    }
                    QTextInputBoxTypeE {
                        width: 651
                        height: 84
                        label.text: STR.STR_QML_851
                        input.text: planInfo.buffer_period.id === "" ? STR.STR_QML_921 : planInfo.buffer_period.display_name
                        input.backgroundColor: "#F5F5F5"
                        input.height: 52
                        input.readOnly: true
                        textColor: "#031F2B"
                        onTextEditClicked: {
                            QMLHandle.sendEvent(EVT.EVT_EDIT_YOUR_INHERITANCE_PLAN_REQUEST, ServiceType.IE_BUFFER_PERIOD)
                        }
                    }
                    Rectangle {
                        height: 1
                        width: 651
                        color: "#EAEAEA"
                    }
                    QServiceInheritanceNotificationPreferences {
                        width: 651
                        inheritance: planInfo
                        onTextEditClicked: {
                            QMLHandle.sendEvent(EVT.EVT_EDIT_YOUR_INHERITANCE_PLAN_REQUEST, ServiceType.IE_NOTIFICATION)
                        }
                    }
                    Item {
                        width: 651
                        height: 80
                    }
                }
            }
        }
    }
    Rectangle {
        id: normalRect
        height: 80
        anchors{
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
        layer.enabled: true
        layer.effect: DropShadow {
            radius: 4
            samples: radius * 2
            source: normalRect
            color: Qt.rgba(0, 0, 0, 0.5)
        }
        Row {
            anchors{
                right: parent.right
                rightMargin: 24
                bottom: parent.bottom
                bottomMargin: 16
            }
            spacing: 12
            layoutDirection: Qt.RightToLeft
            QTextButton {
                id: _save
                width: 214
                height: 48
                label.text: STR.STR_QML_804
                label.font.pixelSize: 16
                type: eTypeE
                enabled: planInfo.edit_isChanged
                onButtonClicked: {
                    var _input = {
                        option : "finalize-change"
                    }
                    QMLHandle.sendEvent(EVT.EVT_INHERITANCE_PLAN_FINALIZE_REQUEST, _input)
                }
            }
            QTextButton {
                width: 148
                height: 48
                label.text: STR.STR_QML_805
                label.font.pixelSize: 16
                type: eTypeF
                enabled: planInfo.edit_isChanged
                onButtonClicked: {
                    var _input = {
                        option : "discard-change"
                    }
                    QMLHandle.sendEvent(EVT.EVT_INHERITANCE_PLAN_FINALIZE_REQUEST, _input)
                }
            }
        }
    }

    Connections {
        target: inheritancePlanInfo
        onSecurityQuestionClosed: {
            if (ServiceSetting.optionIndex === _VIEW_INHERITANCE_PLANING) {
                _Security.close()
            }
        }
        onInheritanceDummyTransactionAlert: {
            if (ServiceSetting.optionIndex === _VIEW_INHERITANCE_PLANING) {
                QMLHandle.sendEvent(EVT.EVT_HEALTH_CHECK_STARTING_REQUEST)
            }
        }
    }
}
