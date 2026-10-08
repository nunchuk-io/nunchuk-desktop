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
import "../../Components/customizes"
import "../../Components/customizes/Texts"
import "../../Components/origins"
import "../../../localization/STR_QML.js" as STR

Item {
    id: settingpanel
    readonly property var option_map: [
        {screen:_DISPLAY_UNIT,          title:STR.STR_QML_513,  icon: "qrc:/Images/Images/display-unit-light.svg"        },
        {screen:_LOCAL_CURRENCY,        title:STR.STR_QML_741,  icon: "qrc:/Images/Images/local-currency-light.svg"      },
        {screen:_ACCOUNT_SETTINGS,      title:STR.STR_QML_537,  icon: "qrc:/Images/Images/accounts-settings-light.svg"   },
        {screen:_FEE_SETTINGS,          title:STR.STR_QML_1715, icon: "qrc:/Images/Images/fee-setting.svg"},
        {screen:_NETWORK_SETTINGS,      title:STR.STR_QML_514,  icon: "qrc:/Images/Images/network-settings-light.svg"    },
        {screen:_HARDWARE_DRIVER,       title:STR.STR_QML_515,  icon: "qrc:/Images/Images/hardware-driver-light.svg"     },
        {screen:_DATABASE_ENCRYTION,    title:STR.STR_QML_530,  icon: "qrc:/Images/Images/change-password-light.svg"     },
        {screen:_DEVELOPER_SETTINGS,    title:STR.STR_QML_588,  icon: "qrc:/Images/Images/developer-settings-light.svg"  },
        {screen:_ABOUT,                 title:STR.STR_QML_518,  icon: "qrc:/Images/Images/info-filled-light.svg"         }
    ]
    property var itemOption: option_map.find(function(e) {if (e.screen === ProfileSetting.optionIndex) return true; else return false})
    Column {
        anchors.fill: parent
        anchors.topMargin: 24
        spacing: 12
        Item{
            width: 304
            height: ClientController.isNunchukLoggedIn ? userPanel.childrenRect.height : guestPanel.childrenRect.height
            QGuestSettingPanel{
                id:guestPanel
                visible: !ClientController.isNunchukLoggedIn
            }
            QUserSettingPanel{
                id:userPanel
                visible: ClientController.isNunchukLoggedIn
            }
            anchors.horizontalCenter: parent.horizontalCenter
        }
        Column {
            spacing: 0
            Repeater {
                model: option_map.length
                Rectangle {
                    width: settingpanel.width
                    height: 48
                    color: itemOption.screen === option_map[index].screen ? Qt.rgba(255, 255, 255, 0.3) : itemSetingMouse.containsMouse ?  Qt.rgba(255, 255, 255, 0.1) : "transparent"
                    Row {
                        spacing: 8
                        anchors.fill: parent
                        Rectangle {
                            width: 8
                            height: parent.height
                            color: itemOption.screen === option_map[index].screen ? "#F6D65D" : "transparent"
                            anchors.verticalCenter: parent.verticalCenter
                        }
                        Item {width: 4; height: parent.height}
                        QIcon {
                            iconSize: 24
                            anchors.verticalCenter: parent.verticalCenter
                            source: option_map[index].icon
                        }
                        QText {
                            anchors.verticalCenter: parent.verticalCenter
                            text: {
                                var scr = option_map[index].screen
                                var prefix = AppSetting.currency;
                                switch(scr) {
                                case _LOCAL_CURRENCY:
                                    return option_map[index].title.arg(prefix)
                                default:
                                    return option_map[index].title
                                }
                            }

                            color: "#FFFFFF"
                            font.family: "Lato"
                            font.pixelSize: 16
                        }
                    }
                    MouseArea {
                        id: itemSetingMouse
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked: {
                            ProfileSetting.optionIndex = option_map[index].screen
                        }
                    }
                }
            }
        }
    }
}
