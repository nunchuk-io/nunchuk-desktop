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
import DataPool 1.0
import "../origins"

QBaseButton {
    id: idBtn
    property string label: "Value"
    property int fontPixelSize: 10
    property int iconSize: 24
    property bool iconInRight: false
    property var icons : ["", "", "", ""]
    border.width: 1
    radius: 20
    state: !enabled ? "disable" : (btnMouse.pressed ? "clicked" : (btnMouse.containsMouse ? "hover" : "default"))
    states: [
        State {
            name: "default"
            PropertyChanges {
                target: idBtn
                color: backgroundColor[eNORMAL]
                border.color: borderColor[eNORMAL]
            }
            PropertyChanges {
                target: idText
                color: textColor[eNORMAL]
            }
            PropertyChanges {
                target: idIcon
                source: "qrc:/Images/Images/" + icons[eNORMAL]
            }
        },
        State {
            name: "hover"
            PropertyChanges {
                target: idBtn
                color: backgroundColor[eHOVER]
                border.color: borderColor[eHOVER]
            }
            PropertyChanges {
                target: idText
                color: textColor[eHOVER]
            }
            PropertyChanges {
                target: idIcon
                source: "qrc:/Images/Images/" + icons[eHOVER]
            }
        },
        State {
            name: "disable"
            PropertyChanges {
                target: idBtn
                color: backgroundColor[eDISABLE]
                border.color: borderColor[eDISABLE]
            }
            PropertyChanges {
                target: idText
                color: textColor[eDISABLE]
            }
            PropertyChanges {
                target: idIcon
                source: "qrc:/Images/Images/" + icons[eDISABLE]
            }
        },
        State {
            name: "clicked"
            PropertyChanges {
                target: idBtn
                color: backgroundColor[eCLICKED]
                border.color: borderColor[eCLICKED]
            }
            PropertyChanges {
                target: idText
                color: textColor[eCLICKED]
            }
            PropertyChanges {
                target: idIcon
                source: "qrc:/Images/Images/" + icons[eCLICKED]
            }
        }
    ]

    signal buttonClicked()
    MouseArea {
        id: btnMouse
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        anchors.fill: parent
        onClicked: idBtn.buttonClicked()
    }
    Row {
        anchors.centerIn: parent
        spacing: 8
        layoutDirection: iconInRight ?  Qt.RightToLeft : Qt.LeftToRight
        QImage {
            id: idIcon
            width: iconSize
            height: iconSize
            anchors.verticalCenter: parent.verticalCenter
        }
        QText {
            id: idText
            anchors.verticalCenter: parent.verticalCenter
            text: label
            font.pixelSize: fontPixelSize
            font.weight: Font.Bold
            font.family: "Lato"
        }
    }
}
