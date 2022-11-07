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

Rectangle {
    id: idBtn
    // type enum
    readonly property int eFIRST  : 0
    readonly property int eSECOND : 1
    readonly property int eTHIRD  : 2
    readonly property int eFOURD  : 3
    readonly property int eFIVE   : 4
    readonly property int eSIX    : 5
    readonly property int eSEVEN  : 6
    readonly property int eEIGHT  : 7
    readonly property int eNINE   : 8
    readonly property int eTEN    : 9
    readonly property int eELEVEN : 10
    readonly property int eTWELVE : 11
    readonly property int eTHIRTEEN:12
    readonly property int eFOURTEEN:13
    readonly property int eFIFTEEN :14

    // State enum
    readonly property int eDEFAULT  : 0
    readonly property int eDISABLE  : 1
    readonly property int eHOVER    : 2
    readonly property int eCLICKED  : 3

    property var backgroundColor: GlobalData.backgroundColor[type]
    property var borderColor: GlobalData.borderColor[type]
    property var textColor: GlobalData.textColor[type]

    property var icons : ["", "", "", ""]
    property int type: eFIRST
    border.width: 1
    radius: 20
    state: !enabled ? "disable" : (btnMouse.pressed ? "clicked" : (btnMouse.containsMouse ? "hover" : "default"))
    states: [
        State {
            name: "default"
            PropertyChanges {
                target: idBtn
                color: backgroundColor[eDEFAULT]
                border.color: borderColor[eDEFAULT]
            }
            PropertyChanges {
                target: idText
                color: textColor[eDEFAULT]
            }
            PropertyChanges {
                target: idIcon
                source: "qrc:/Images/Images/" + icons[eDEFAULT]
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
}
