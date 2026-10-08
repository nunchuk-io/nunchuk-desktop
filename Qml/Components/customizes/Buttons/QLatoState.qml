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
import "../../origins"
import "../../customizes/Texts"

QLato {
    id: _text
    // Click state
    property var textColor: GlobalData.textColor.find(function(e) {return e.id === type})
    state: !enabled ? "disable" : (btnMouse.pressed ? "clicked" : (btnMouse.containsMouse ? "hover" : "default"))
    states: [
        State {
            name: "default"
            PropertyChanges {
                target: _text
                color: textColor.normal
            }
        },
        State {
            name: "hover"
            PropertyChanges {
                target: _text
                color: textColor.hover
            }
        },
        State {
            name: "disable"
            PropertyChanges {
                target: _text
                color: textColor.disable
            }
        },
        State {
            name: "clicked"
            PropertyChanges {
                target: _text
                color: textColor.clicked
            }
        }
    ]
}
