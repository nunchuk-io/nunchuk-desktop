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

Rectangle {
    id: _background
    property var backgroundColor: GlobalData.backgroundColor.find(function(e) {return e.id === type})
    property var borderColor: GlobalData.borderColor.find(function(e) {return e.id === type})
    border.width: 2
    radius: _background.width/2
    state: !enabled ? "disable" : (btnMouse.pressed ? "clicked" : (btnMouse.containsMouse ? "hover" : "default"))

    states: [
        State {
            name: "default"
            PropertyChanges {
                target: _background
                color: backgroundColor.normal
                border.color: borderColor.normal
            }
        },
        State {
            name: "hover"
            PropertyChanges {
                target: _background
                color: backgroundColor.hover
                border.color: borderColor.hover
            }
        },
        State {
            name: "disable"
            PropertyChanges {
                target: _background
                color: backgroundColor.disable
                border.color: borderColor.disable
            }
        },
        State {
            name: "clicked"
            PropertyChanges {
                target: _background
                color: backgroundColor.clicked
                border.color: borderColor.clicked
            }
        }
    ]
}
