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
import QtQuick

Item {
    id: _marquee
    property alias text: _label.text
    property alias font: _label.font
    property alias color: _label.color
    property int pixelsPerSecond: 40
    property int pauseMs: 1200
    readonly property bool overflow: _label.paintedWidth > width
    height: 20
    clip: true

    QLato {
        id: _label
        width: paintedWidth
        height: parent.height
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        x: 0
    }

    SequentialAnimation {
        id: _anim
        running: _marquee.overflow && _marquee.visible
        loops: Animation.Infinite
        PauseAnimation { duration: _marquee.pauseMs }
        NumberAnimation {
            target: _label
            property: "x"
            from: 0
            to: _marquee.width - _label.paintedWidth
            duration: Math.max(600, (_label.paintedWidth - _marquee.width) * 1000 / _marquee.pixelsPerSecond)
        }
        PauseAnimation { duration: _marquee.pauseMs }
        NumberAnimation {
            target: _label
            property: "x"
            to: 0
            duration: 300
        }
    }
    onOverflowChanged: if (!overflow) _label.x = 0
}
