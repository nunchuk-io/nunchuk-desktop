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

Item {
    id: _item
    enabled: true
    property int type: eTypeA
    readonly property int eTypeA: 0
    readonly property int eTypeB: 1
    readonly property int eTypeC: 2
    readonly property int eTypeD: 3
    readonly property int eTypeE: 4
    readonly property int eTypeF: 5
    readonly property int eTypeG: 6

    readonly property int eTypeH: 7
    readonly property int eTypeI: 8

    readonly property int eTypeJ: 9
    readonly property int eTypeK: 10
    readonly property int eTypeL: 11
    readonly property int eTypeM: 12
    readonly property int eTypeN: 13
    readonly property int eTypeO: 14

    readonly property int eTypeP: 15

    // Click state
    readonly property int eNORMAL: 0
    readonly property int eHOVER : 1
    readonly property int eDISABLE: 2
    readonly property int eCLICKED: 3
    property var backgroundColor: GlobalData.backgroundColor[type]
    property var borderColor: GlobalData.borderColor[type]
    property alias btnMouse: _mouse
    signal buttonClicked(var mouse)
    signal pressed(var mouse)
    signal released(var mouse)
    signal containsMouseChanged()
    signal clicked(var mouse)
    MouseArea {
        id: _mouse
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        anchors.fill: parent
        onPressed: _item.pressed(mouse)
        onClicked: {
            if (!timer1s.running) {
                timer1s.restart()
                _item.buttonClicked(mouse)
                _item.clicked(mouse)
            }
        }
        onReleased: _item.released(mouse)
        onContainsMouseChanged: _item.containsMouseChanged()
    }
    Timer {
        id: timer1s
        interval: 1000
        repeat: false
        running: false
        onTriggered: timer1s.stop()
    }
}
