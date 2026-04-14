/**************************************************************************
 * This file is part of the Nunchuk software (https://nunchuk.io/)        *
 * Copyright (C) 2020-2022 Enigmo                                         *
 * Copyright (C) 2022 Nunchuk                                             *
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
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Buttons"
import "../../../Components/customizes/Texts"

Rectangle {
    id: root
    radius: 8
    color: "#FFFFFF"
    border.width: 1
    border.color: "#DEDEDE"

    property string label: ""
    property string subLabel: ""
    property bool   subLabelChanged: false
    property string iconUrl: ""
    property string signer_tag: ""
    property string signer_type: ""
    property bool enableCoSigningDelay: false
    property bool coSigningDelayChanged: false
    property bool autoBroadcast: false
    property bool autoBroadcastChanged: false

    signal editClicked()

    function formatInterval(interval) {
        switch ((interval ?? "").toString().toUpperCase()) {
        case "DAILY": return "Day"
        case "WEEKLY": return "Week"
        case "MONTHLY": return "Month"
        case "YEARLY": return "Year"
        default: return interval ?? ""
        }
    }

    Column {
        id: contentColumn
        width: parent.width - 24
        anchors {
            top: parent.top
            topMargin: 12
            left: parent.left
            leftMargin: 12
        }
        spacing: 12

        Item {
            width: parent.width
            height: 48
            Row {
                spacing: 12
                QCircleIcon {
                    bgSize: 48
                    icon.iconSize: 24
                    icon.type: root.signer_type
                    icon.tag: root.signer_tag
                    icon.iconUrl: root.iconUrl
                    anchors.verticalCenter: parent.verticalCenter
                    color: "#F5F5F5"
                }
                Column {
                    width: 100
                    height: 40
                    spacing: 4
                    anchors.verticalCenter: parent.verticalCenter
                    QLato {
                        width: parent.width
                        text: root.label
                        font.pixelSize: 12
                        font.weight: Font.Normal
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignLeft
                    }
                    QLato {
                        width: parent.width
                        text: root.subLabel
                        font.pixelSize: 16
                        font.weight: Font.Bold
                        color: root.subLabelChanged ? "#CF4018" : "#031F2B"
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignLeft
                    }
                }
            }
           
            QTextLink {
                id: _edit
                width: _edit.paintedWidth
                height: 20
                text: STR.STR_QML_849
                font.weight: Font.Normal
                anchors {
                    right: parent.right
                    verticalCenter: parent.verticalCenter
                }
                onTextClicked: root.editClicked()
            }
        }

        Rectangle {
            width: parent.width
            height: 1
            color: "#EAEAEA"
        }
        Column {
            width: parent.width
            height: 56
            spacing: 8
            Row {
                width: parent.width
                height: 20

                QLato {
                    width: parent.width - coSigningDelaySwitch.width
                    text: STR.STR_QML_795
                    font.pixelSize: 16
                    font.weight: Font.Normal
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    anchors {
                        left: parent.left
                        verticalCenter: parent.verticalCenter
                    }
                }

                QLato {
                    id: coSigningDelaySwitch
                    width: 84
                    height: 48
                    text: root.enableCoSigningDelay ? STR.STR_QML_2134 : STR.STR_QML_2135
                    font.pixelSize: 16
                    font.weight: Font.Bold
                    color: root.coSigningDelayChanged ? "#CF4018" : "#031F2B"
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight
                    anchors {
                        right: parent.right
                        verticalCenter: parent.verticalCenter
                    }
                }
            }
            Row {
                width: parent.width
                height: 20
                
                QLato {
                    width: parent.width
                    anchors.left: parent.left
                    text: STR.STR_QML_2125
                    font.pixelSize: 16
                    font.weight: Font.Normal
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    anchors {
                        left: parent.left
                        verticalCenter: parent.verticalCenter
                    }
                }

                QLato {
                    id: autoBroadcastSwitch
                    width: 84
                    height: 48
                    text: root.autoBroadcast ? STR.STR_QML_2134 : STR.STR_QML_2135
                    color: root.autoBroadcastChanged ? "#CF4018" : "#031F2B"
                    font.pixelSize: 16
                    font.weight: Font.Bold
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight
                    anchors {
                        right: parent.right
                        verticalCenter: parent.verticalCenter
                    }
                }
            }
        }
    }
}
