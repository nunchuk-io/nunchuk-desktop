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

Item {
    id: idRootStyle1
    width: contentRow.width + 32
    property bool _mirror: true
    // type enum
    readonly property int eLEFT  : 0
    readonly property int eRIGHT : 1

    // State enum
    readonly property int eDEFAULT  : 0
    readonly property int eDISABLE  : 1
    readonly property int eHOVER    : 2

    // Property set
    property var icon: ["qrc:/Images/Images/left-arrow-dark.svg", "qrc:/Images/Images/arrow_outline_d.png", "qrc:/Images/Images/arrow_outline_p.png"]
    property var textColor: ["#031F2B", "#839096", "#35ABEE"]

    property int direction: eLEFT
    property string label: ""
    property string labelFont: "Lato"
    property int fontPixelSize: 16

    property bool displayIcon: true

    state: !enabled ? "disable" : btnMouse.containsMouse ? "hover" : "default"

    states: [
        State {
            name: "default"
            PropertyChanges {
                target: ico
                source: icon[eDEFAULT]
            }
            PropertyChanges {
                target: text
                color: textColor[eDEFAULT]
            }
        },
        State {
            name: "hover"
            PropertyChanges {
                target: ico
                source: icon[eHOVER]
            }
            PropertyChanges {
                target: text
                color: textColor[eHOVER]
            }
        },
        State {
            name: "disable"
            PropertyChanges {
                target: ico
                source: icon[eDISABLE]
            }
            PropertyChanges {
                target: text
                color: textColor[eDISABLE]
            }
        }
    ]

    Row {
        id: contentRow
        spacing: 8
        anchors.centerIn: parent
        layoutDirection: (direction == eRIGHT) ?  Qt.RightToLeft : Qt.LeftToRight
        QImage {
            id: ico
            width: 24
            height: 24
            visible: displayIcon
            mirror: (direction == eRIGHT) && _mirror
            anchors.verticalCenter: parent.verticalCenter
        }
        QText {
            id: text
            anchors.verticalCenter: ico.verticalCenter
            text: label
            font.pixelSize: fontPixelSize
            font.weight: Font.DemiBold
            font.family: labelFont
        }
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
