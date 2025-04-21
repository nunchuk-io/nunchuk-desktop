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
import NUNCHUCKTYPE 1.0
import "../../origins"
import "../../customizes/Texts"
import "../../../../localization/STR_QML.js" as STR

Rectangle {
    width: 360
    height: 192
    radius: 8
    border.color: "#DEDEDE"
    border.width: 1
    property bool selected: false
    property var keysSettings: [
        {title: STR.STR_QML_015 , description: STR.STR_QML_1651, tmValue: customizeN, decrease: function() { customizeN--; if(customizeN < customizeM) customizeM = customizeN; }, increase: function() { customizeN++; }, plusEnable: function() { return customizeN < (addressType === NUNCHUCKTYPE.TAPROOT ? 5 : 15) }, minusEnable: function() { return customizeN > 0 }},
        {title: STR.STR_QML_1652 , description: STR.STR_QML_1653, tmValue: customizeM, decrease: function() { customizeM --; }, increase: function() { customizeM++; }, plusEnable: function() { return customizeM < customizeN }, minusEnable: function() { return customizeM > 0 } },
    ]

    Connections {
        target: _walletConfig
        onAddressTypeChanged: {
            if (addressType == NUNCHUCKTYPE.TAPROOT) {
                if (customizeN > 5) customizeN = 5
            }
        }
    }

    Column {
        width: parent.width
        spacing: 12
        Rectangle {
            width: parent.width
            height: 40
            radius: 8
            color: selected ? "#D0E2FF" : "#FFFFFF"
            Rectangle {
                width: parent.width
                height: 20
                color: selected ? "#D0E2FF" : "#FFFFFF"
                anchors.bottom: parent.bottom
            }
            QIcon {
                id: icon
                iconSize: 24
                source: selected ? "qrc:/Images/Images/radio-selected-dark.svg" : "qrc:/Images/Images/radio-dark.svg"
                anchors {
                    verticalCenter: parent.verticalCenter
                    right: parent.right
                    rightMargin: 12
                }
            }
        }

        Column {
            width: parent.width
            spacing: 16
            Repeater {
                model: keysSettings
                Item {
                    width: parent.width
                    height: 56
                    Column {
                        anchors {
                            left: parent.left
                            leftMargin: 12
                            verticalCenter: parent.verticalCenter
                        }
                        width: 178
                        spacing: 4
                        QLato {
                            width: 178
                            text: modelData.title
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter
                        }
                        QLato {
                            width: 178
                            text: modelData.description
                            font.pixelSize: 12
                            font.weight: Font.Normal
                            lineHeight: 16
                            color: "#757575"
                            lineHeightMode: Text.FixedHeight
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter
                            wrapMode: Text.Wrap
                        }
                    }
                    Row {
                        height: parent.height
                        anchors {
                            right: parent.right
                            rightMargin: 12
                        }
                        spacing: 12
                        Rectangle {
                            width: 36
                            height: 36
                            border.color: "#031F2B"
                            border.width: 2
                            radius: 44
                            property var isEnable: keysSettings[index].minusEnable()
                            enabled: isEnable
                            opacity: isEnable ? 1.0 : 0.4
                            Rectangle {
                                width: 10.5
                                height: 1.5
                                color: "#031F2B"
                                anchors.centerIn: parent
                            }
                            anchors.verticalCenter: parent.verticalCenter
                            MouseArea {
                                anchors.fill: parent
                                cursorShape: Qt.PointingHandCursor
                                onClicked: {
                                    keysSettings[index].decrease()
                                }
                            }
                        }

                        Rectangle {
                            width: 50
                            height: 48
                            border.color: "#DEDEDE"
                            border.width: 1
                            radius: 8
                            QLato {
                                width: 44
                                height: 20
                                text: modelData.tmValue
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                                anchors.centerIn: parent
                            }
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        Rectangle {
                            width: 36
                            height: 36
                            border.color: "#031F2B"
                            border.width: 2
                            radius: 44
                            property var isEnable: keysSettings[index].plusEnable()
                            enabled: isEnable
                            opacity: isEnable ? 1.0 : 0.4
                            Rectangle {
                                width: 10.5
                                height: 1.5
                                color: "#031F2B"
                                anchors.centerIn: parent
                            }
                            Rectangle {
                                width: 1.5
                                height: 10.5
                                color: "#031F2B"
                                anchors.centerIn: parent
                            }
                            anchors.verticalCenter: parent.verticalCenter
                            MouseArea {
                                anchors.fill: parent
                                cursorShape: Qt.PointingHandCursor
                                onClicked: {
                                    keysSettings[index].increase()
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
