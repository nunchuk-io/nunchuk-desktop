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
import QtGraphicalEffects 1.0
import "../origins"

Rectangle {
    id: idRootStyle1

    // type enum
    readonly property int eFILLED           : 0
    readonly property int eOUTLINE_NORMAL   : 1
    readonly property int eOUTLINE_DARK     : 2

    // State enum
    readonly property int eDEFAULT  : 0
    readonly property int eDISABLE  : 1
    readonly property int eHOVER    : 2

    // Filled set
    property var backgroundColorFilled: ["#C9DEF1", "#9CAEB8", "#031F2B"]
    property var borderColorFilled: ["transparent", "transparent", "transparent"]
    property var textColorFilled: ["#031F2B", "#C9DEF1", "#C9DEF1"]

    // OutlineNormal set
    property var backgroundColorOutlineNormal: ["transparent", "transparent", "#031F2B"]
    property var borderColorOutlineNormal: ["#031F2B", "#9CAEB8", "#031F2B"]
    property var textColorOutlineNormal: ["#031F2B", "#9CAEB8", "#F1FAFE"]

    // OutlineDark set
    property var backgroundColorOutlineDark: ["transparent", "transparent", "#F1FAFE"]
    property var borderColorOutlineDark: ["#F1FAFE", "#839096", "transparent"]
    property var textColorOutlineDark: ["#F1FAFE", "#839096", "#031F2B"]

    property var backgroundColor: [backgroundColorFilled, backgroundColorOutlineNormal, backgroundColorOutlineDark]
    property var borderColor: [borderColorFilled, borderColorOutlineNormal, borderColorOutlineDark]
    property var textColor: [textColorFilled, textColorOutlineNormal, textColorOutlineDark]

    property int type: eFILLED
    property string label: "Value"
    property int fontPixelSize: 14
    property var fontWeight: Font.Bold
    property string fontFamily: "Lato"

    border.width: 1
    radius: 20
    state: !enabled ? "disable" : (btnMouse.pressed ? "clicked" : (btnMouse.containsMouse ? "hover" : "default"))

    states: [
        State {
            name: "default"
            PropertyChanges {
                target: idRootStyle1
                color: backgroundColor[type][eDEFAULT]
                border.color: borderColor[type][eDEFAULT]
            }
            PropertyChanges {
                target: text
                color: textColor[type][eDEFAULT]
            }
        },
        State {
            name: "disable"
            PropertyChanges {
                target: idRootStyle1
                color: backgroundColor[type][eDISABLE]
                border.color: borderColor[type][eDISABLE]
            }
            PropertyChanges {
                target: text
                color: textColor[type][eDISABLE]
            }
        },
        State {
            name: "hover"
            PropertyChanges {
                target: idRootStyle1
                color: backgroundColor[type][eHOVER]
                border.color: borderColor[type][eHOVER]
            }
            PropertyChanges {
                target: text
                color: textColor[type][eHOVER]
            }
        },
        State {
            name: "clicked"
            PropertyChanges {
                target: idRootStyle1
                color: backgroundColor[type][eDEFAULT]
                border.color: borderColor[type][eDEFAULT]
            }
            PropertyChanges {
                target: text
                color: textColor[type][eDEFAULT]
            }
        }
    ]

    Row {
        anchors.centerIn: idRootStyle1
        spacing: 9
        Item {
            id: plus
            width: 16
            height: 16
            anchors.verticalCenter: parent.verticalCenter
            Rectangle {
                width: 10
                height: 2
                anchors.centerIn: parent
                color: text.color
            }
            Rectangle {
                width: 2
                height: 10
                color: text.color
                anchors.centerIn: parent
            }
        }
        QText {
            id: text
            height: idRootStyle1.height
            anchors.verticalCenter: plus.verticalCenter
            verticalAlignment: Text.AlignVCenter
            font.family: "Lato"
            font.weight: fontWeight
            font.pixelSize: fontPixelSize
            text: label
        }
    }

    Rectangle {
        id: clickbg
        anchors.fill: parent
        radius: parent.radius
        color: type == eOUTLINE_DARK ? Qt.rgba(255, 255, 255, 0.1) : Qt.rgba(0, 0, 0, 0.1)
        visible: btnMouse.pressed
    }

    signal buttonClicked()
    MouseArea {
        id: btnMouse
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        anchors.fill: parent
        onClicked: idRootStyle1.buttonClicked()
    }
}
