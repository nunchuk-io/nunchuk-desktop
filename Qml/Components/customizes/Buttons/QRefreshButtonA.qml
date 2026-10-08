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

Rectangle {
    id: idRootStyle1

    // State enum
    readonly property int eDEFAULT  : 0
    readonly property int eDISABLE  : 1
    readonly property int eHOVER    : 2

    property var backgroundColor: ["transparent", "transparent", "transparent"]
    property var borderColor: ["transparent", "transparent", "transparent"]
    property var textColor: ["#031F2B", "#9CAEB8", "#F1FAFE"]

    property string label: "Value"
    property int fontPixelSize: 10
    property alias icon : ico.source
    property int iconSize: 24
    property bool iconInRight: false
    property int  iconSpacing: 8
    border.width: 1
    radius: 20

    state: !enabled ? "disable" : btnMouse.pressed || btnMouse.containsMouse ? "clicked" : "default"

    states: [
        State {
            name: "default"
            PropertyChanges {
                target: idRootStyle1
                color: backgroundColor[eDEFAULT]
                border.color: borderColor[eDEFAULT]
            }
            PropertyChanges {
                target: text
                color: textColor[eDEFAULT]
            }
        },
        State {
            name: "hover"
            PropertyChanges {
                target: idRootStyle1
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
                target: idRootStyle1
                color: backgroundColor[eDISABLE]
                border.color: borderColor[eDISABLE]
            }
            PropertyChanges {
                target: text
                color: textColor[eDISABLE]
            }
        }
    ]

    Row {
        anchors.centerIn: parent
        spacing: iconSpacing
        QIcon {
            id: ico
            iconSize: idRootStyle1.iconSize
            source: "qrc:/Images/Images/cached_24px.png"
            anchors.verticalCenter: parent.verticalCenter
        }
        QText {
            id: text
            anchors.verticalCenter: parent.verticalCenter
            text: label
            font.pixelSize: fontPixelSize
            font.weight: Font.Bold
            font.family: "Lato"
        }
    }

    function startRefresh() {animate.start()}
    function stopRefresh()  {animate.stop() }

    NumberAnimation {
        id: animate
        target: ico
        property: "rotation"
        from: 0
        to: -360
        duration: 500
        loops: Animation.Infinite
        alwaysRunToEnd: true
    }

    signal buttonClicked()
    MouseArea {
        id: btnMouse
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        anchors.fill: parent
        onClicked: {
            startRefresh()
            idRootStyle1.buttonClicked()
        }
    }

    Connections {
        target: AppModel
        onFinishedScanDevices : stopRefresh()
    }
}
