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

QBaseButton {
    id: rootTextButton
    width: 137
    height: 48
    radius: 24
    border.width: 2
    property alias label: text
    property bool processing: false
    state: !enabled ? "disable" : (btnMouse.pressed ? "clicked" : (btnMouse.containsMouse ? "hover" : "default"))
    states: [
        State {
            name: "default"
            PropertyChanges {
                target: rootTextButton
                color: backgroundColor[eNORMAL]
                border.color: borderColor[eNORMAL]
            }
            PropertyChanges {
                target: text
                color: textColor[eNORMAL]
            }
        },
        State {
            name: "hover"
            PropertyChanges {
                target: rootTextButton
                color: backgroundColor[eHOVER]
                border.color: borderColor[eHOVER]
            }
            PropertyChanges {
                target: text
                color: textColor[eHOVER]
            }
        },
        State {
            name: "disable"
            PropertyChanges {
                target: rootTextButton
                color: backgroundColor[eDISABLE]
                border.color: borderColor[eDISABLE]
            }
            PropertyChanges {
                target: text
                color: textColor[eDISABLE]
            }
        },
        State {
            name: "clicked"
            PropertyChanges {
                target: rootTextButton
                color: backgroundColor[eCLICKED]
                border.color: borderColor[eCLICKED]
            }
            PropertyChanges {
                target: text
                color: textColor[eCLICKED]
            }
        }
    ]

    QText {
        id: text
        anchors.fill: parent
        anchors.centerIn: rootTextButton
        text: qsTr("TEXT")
        font.weight: Font.DemiBold
        font.pixelSize: 16
        font.family: "Lato"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    signal buttonClicked()
    MouseArea {
        id: btnMouse
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        anchors.fill: rootTextButton
        onClicked: rootTextButton.buttonClicked()
    }
}
