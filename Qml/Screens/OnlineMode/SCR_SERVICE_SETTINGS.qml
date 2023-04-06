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
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Chats"
import "../../../localization/STR_QML.js" as STR

QScreen {
    id: rootSettings
    width: QAPP_DEVICE_WIDTH
    height: QAPP_DEVICE_HEIGHT
    Row {
        anchors.fill: parent
        Item {
            id: pannel_left
            width: 386
            height: parent.height
            transformOrigin: Item.Center
            Rectangle {
                id: bgColor
                anchors.fill: parent
                color: "#031F2B"
                visible: false
            }
            DropShadow {
                id: dropshodow1
                anchors.fill: bgColor
                radius: 24
                samples: 16
                color: Qt.rgba(0, 0, 0, 0.14)
                source: bgColor
            }
            Row {
                QMenuBar {
                    width: 72
                    height: pannel_left.height
                    anchors.verticalCenter: parent.verticalCenter
                    enables: [
                        ClientController.isNunchukLoggedIn,
                        true,
                        true
                    ]
                    onLocalModeRequest: {
                        if(0 === AppModel.nunchukMode){
                            QMLHandle.sendEvent(EVT.EVT_STARTING_APPLICATION_LOCALMODE)
                        }
                        else{
                            QMLHandle.sendEvent(EVT.EVT_STARTING_APPLICATION_ONLINE_HWL)
                        }
                    }
                    onOnlineModeRequest: {
                        QMLHandle.sendEvent(EVT.EVT_GOTO_HOME_CHAT_TAB)
                    }
                    onOpenSettingRequest: {
                        QMLHandle.sendEvent(EVT.EVT_GOTO_APP_SETTINGS_TAB)
                    }
                    onSignoutRequest: {
                        ClientController.requestSignout()
                    }
                    onUpdateProfileRequest: {
                        QMLHandle.sendEvent(EVT.EVT_ROOT_UPDATE_PROFILE)
                    }
                    onSigninRequest: {
                        QMLHandle.sendEvent(EVT.EVT_STARTING_APPLICATION_ONLINEMODE)
                    }
                }
                Item {
                    id: leftpanel
                    width: 314
                    height: pannel_left.height
                    anchors.verticalCenter: parent.verticalCenter
                    Column {
                        anchors.fill: parent
                        anchors.topMargin: 24
                        spacing: 12
                        Column {
                            spacing: 0
                            Repeater {
                                id: itemsSetting
                                readonly property int _EMERGENCY: 0
                                readonly property int _EMERGENCY_LOCKDOWN: 1
                                readonly property int _KEY_RECOVERY: 2
                                readonly property int _INHERITANCE_PLANING: 3
                                readonly property int _CLAIM_AN_INHERITANCE: 4
                                readonly property int _YOUR_SUBSCRIPTION: 5
                                readonly property int _PLATFORM_KEY_CO_SIGNING_POLICIES: 6
                                readonly property int _GET_ADDITIONAL_WALLETS: 7
                                readonly property int _ORDER_NEW_HARDWARE: 8
                                readonly property int _MANAGE_SUBSCRIPTION: 9
                                readonly property var setting_map: [
                                    {screen:_EMERGENCY,             visible: true, enable:false,  title:STR.STR_QML_702, icon: "qrc:/Images/Images/emergency-dark.svg"   ,action: function(){} },
                                    {screen:_EMERGENCY_LOCKDOWN,    visible: true, enable:true,  title:STR.STR_QML_697, icon: ""   ,action: function(){} },
                                    {screen:_KEY_RECOVERY,          visible: true, enable:true,  title:STR.STR_QML_698, icon: ""   ,action: function(){} },
                                    {screen:_INHERITANCE_PLANING,   visible: false, enable:false,  title:STR.STR_QML_736, icon: "qrc:/Images/Images/inheritance-dark.svg"   ,action: function(){} },
                                    {screen:_CLAIM_AN_INHERITANCE,  visible: false, enable:false,  title:STR.STR_QML_737, icon: ""   ,action: function(){} },
                                    {screen:_YOUR_SUBSCRIPTION,     visible: true, enable:false,  title:STR.STR_QML_699, icon: "qrc:/Images/Images/subscription-light.svg" ,action: function(){} },
                                    {screen:_PLATFORM_KEY_CO_SIGNING_POLICIES,  visible: false, enable:false,  title:STR.STR_QML_738, icon: ""   ,action: function(){} },
                                    {screen:_GET_ADDITIONAL_WALLETS,visible: false, enable:false, title:STR.STR_QML_707, icon: ""   ,action: function(){} },
                                    {screen:_ORDER_NEW_HARDWARE,    visible: true, enable:true,  title:STR.STR_QML_700, icon: ""   ,action: function(){_ManagerSub.link = "https://nunchuk.io/hardware-replacement";_ManagerSub.contentText = STR.STR_QML_735;_ManagerSub.open();} },
                                    {screen:_MANAGE_SUBSCRIPTION,   visible: true, enable:true,  title:STR.STR_QML_682, icon: ""   ,action: function(){_ManagerSub.link = "https://nunchuk.io/my-plan";_ManagerSub.contentText = STR.STR_QML_684;_ManagerSub.open();}}
                                ]

                                model: setting_map.length
                                Rectangle {
                                    id: _item
                                    width: leftpanel.width
                                    height: 48
                                    visible: itemsSetting.setting_map[index].visible
                                    enabled: itemsSetting.setting_map[index].enable
                                    color: GlobalData.serviceIndex === index ? Qt.rgba(255, 255, 255, 0.3) : itemSetingMouse.containsMouse ?  Qt.rgba(255, 255, 255, 0.1) : "transparent"
                                    Row {
                                        spacing: 8
                                        anchors.fill: parent
                                        Item {
                                            width: 8;
                                            height: parent.height
                                            Rectangle {
                                                width: 4
                                                height: parent.height
                                                color: GlobalData.serviceIndex === index ? "#F6D65D" : "transparent"
                                                anchors.verticalCenter: parent.verticalCenter
                                            }
                                        }
                                        QImage {
                                            id:_icon
                                            width: 24
                                            height: 24
                                            anchors.verticalCenter: parent.verticalCenter
                                            source: itemsSetting.setting_map[index].icon
                                            visible: source != ""
                                        }
                                        QText {
                                            anchors.verticalCenter: parent.verticalCenter
                                            text: itemsSetting.setting_map[index].title
                                            color: "#FFFFFF"
                                            font.family: "Lato"
                                            font.pixelSize: 16
                                            font.weight: _icon.visible ? Font.DemiBold : Font.Normal
                                        }
                                    }
                                    MouseArea {
                                        id: itemSetingMouse
                                        anchors.fill: parent
                                        hoverEnabled: true
                                        onClicked: {
                                            var _item = itemsSetting.setting_map[index]
                                            switch(_item.screen) {
                                            case itemsSetting._EMERGENCY_LOCKDOWN:
                                            case itemsSetting._KEY_RECOVERY:
                                                GlobalData.serviceIndex = _item.screen
                                                break
                                            default:
                                                _item.action()
                                                break

                                            }


                                        }
                                    }
                                    Component.onCompleted: {
                                        GlobalData.serviceIndex = itemsSetting._EMERGENCY_LOCKDOWN
                                    }
                                }
                            }
                        }
                        QButtonIcon {
                            width: 289
                            height: 48
                            type: eTypeG
                            label: STR.STR_QML_701
                            anchors.horizontalCenter: parent.horizontalCenter
                            icons: ["contact-support-dark.svg", "contact-support-light.svg", "contact-support-dark.svg", "contact-support-light.svg"]
                            fontPixelSize: 16
                            enabled: ClientController.isMatrixLoggedIn && ClientController.readySupport
                            onButtonClicked: {
                                QMLHandle.sendEvent(EVT.EVT_SERVICE_SUPPORT_REQUEST)
                            }
                        }
                    }
                }
            }
        }
        Rectangle {
            width: parent.width - pannel_left.width
            height: parent.height
            color: "#FFFFFF"
            Item {
                anchors.fill: parent
                visible: GlobalData.serviceIndex === itemsSetting._EMERGENCY_LOCKDOWN
                enabled: visible
                Column {
                    anchors.fill: parent
                    spacing: 24
                    anchors{
                        left: parent.left
                        leftMargin: 24
                        top: parent.top
                        topMargin: 24
                    }
                    Rectangle {
                        width: parent.width - 24
                        height: parent.height * 0.42
                        radius: 24
                        color: "#D0E2FF"
                        QImage {
                            anchors.centerIn: parent
                            width: sourceSize.width
                            height: sourceSize.height
                            source: "qrc:/Images/Images/emergency-lockdown-illustrations.svg"
                        }
                    }
                    Item {
                        width: parent.width - 24
                        height: parent.height * 0.45
                        Column {
                            width: parent.width
                            spacing: 16
                            QText {
                                text: STR.STR_QML_697
                                color: "#031F2B"
                                font.family: "Montserrat"
                                font.pixelSize: 32
                                font.weight: Font.DemiBold
                            }
                            Repeater {
                                id: _description
                                width: parent.width
                                readonly property var content_map: [
                                    {height: 84, title:STR.STR_QML_703, icon: ""   },
                                    {height: 56, title:STR.STR_QML_704, icon: "qrc:/Images/Images/1.Active.svg"   },
                                    {height: 28, title:STR.STR_QML_705, icon: "qrc:/Images/Images/2.Active.svg"   },
                                    {height: 28, title:STR.STR_QML_706, icon: "qrc:/Images/Images/3.Active.svg"    },
                                ]
                                model: content_map.length
                                Rectangle {
                                    width: parent.width
                                    height: _description.content_map[index].height
                                    Row {
                                        spacing: 8
                                        QImage {
                                            id:_numicon
                                            width: 24
                                            height: 24
                                            source: _description.content_map[index].icon
                                            visible: source != ""
                                        }
                                        QText {
                                            id:_numText
                                            width: 591
                                            height: _description.content_map[index].height
                                            anchors.verticalCenter: parent.verticalCenter
                                            text: _description.content_map[index].title
                                            color: "#031F2B"
                                            font.family: "Lato"
                                            font.pixelSize: 16
                                            lineHeightMode: Text.FixedHeight
                                            lineHeight: 28
                                            horizontalAlignment: Text.AlignLeft
                                            verticalAlignment: Text.AlignVCenter
                                        }
                                    }
                                }
                            }
                            QTextButton {
                                width: 98
                                height: 48
                                label.text: STR.STR_QML_097
                                label.font.pixelSize: 16
                                type: eTypeE
                                onButtonClicked: {
                                    var screenObj = { "state_id" : EVT.STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD }
                                    QMLHandle.sendEvent(EVT.EVT_REENTER_YOUR_PASSWORD_REQUEST,screenObj)
                                }
                            }
                        }
                    }
                }
            }
            Item {
                anchors.fill: parent
                visible: GlobalData.serviceIndex === itemsSetting._KEY_RECOVERY
                enabled: visible
                Column {
                    anchors.fill: parent
                    anchors.margins: 24
                    spacing: 10
                    QText {
                        font.family: "Lato"
                        font.pixelSize: 28
                        color: "#031F2B"
                        font.weight: Font.Bold
                        text: STR.STR_QML_698
                    }
                    Row {
                        spacing: 0
                        QText {
                            width: 627-24
                            text: STR.STR_QML_708
                            color: "#031F2B"
                            font.pixelSize: 16
                            font.family: "Lato"
                            anchors.verticalCenter: parent.verticalCenter
                        }
                        QImage {
                            width: 24
                            height: 24
                            source: "qrc:/Images/Images/right-arrow-dark.svg"
                            scale: arrow.containsMouse ? 1.1 : 1
                            MouseArea {
                                id: arrow
                                anchors.fill: parent
                                hoverEnabled: true
                                cursorShape: Qt.PointingHandCursor
                                onClicked: {
                                    var screenObj = { "state_id" : EVT.STATE_ID_SCR_KEY_RECOVERY }
                                    QMLHandle.sendEvent(EVT.EVT_REENTER_YOUR_PASSWORD_REQUEST,screenObj)
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    QPopupInfoVertical {
        id: _ManagerSub
        property string link: "https://nunchuk.io/my-plan"
        lwidths:[252,252]
        title: STR.STR_QML_339
        contentText: STR.STR_QML_684
        labels: [STR.STR_QML_341,STR.STR_QML_683]
        onConfirmNo: {
            close()
            Qt.openUrlExternally(link)
        }
        onConfirmYes: {
            close()
        }
    }
}
