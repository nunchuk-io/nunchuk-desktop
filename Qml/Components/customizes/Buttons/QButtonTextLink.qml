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
import "../../origins"
import "../../customizes/Texts"

Item {
    id: idRootStyle1
    width: contentRow.width
    property bool _mirror: true
    // type enum
    readonly property int eLEFT  : 0
    readonly property int eRIGHT : 1

    // State enum
    readonly property int eDEFAULT  : 0
    readonly property int eDISABLE  : 1
    readonly property int eHOVER    : 2

    // Property set
    property var icon: ["qrc:/Images/Images/left-arrow-dark.svg", "qrc:/Images/Images/left-arrow-dark-disabled.svg", "qrc:/Images/Images/left-arrow-dark.svg"]
    property var textColor: ["#031F2B", "#839096", "#35ABEE"]

    property int direction: eLEFT
    property string label: ""
    property int fontPixelSize: 16

    property alias btnIcon: ico
    property alias btnText: text

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
        QIcon {
            id: ico
            iconSize: 24
            visible: displayIcon
            mirror: (direction == eRIGHT) && _mirror
            anchors.verticalCenter: parent.verticalCenter
        }
        QLato {
            id: text
            anchors.verticalCenter: ico.verticalCenter
            text: label
            font.pixelSize: fontPixelSize
            font.weight: Font.Bold
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
