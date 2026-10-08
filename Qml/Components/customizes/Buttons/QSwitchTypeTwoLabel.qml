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
import "../../customizes"

Item {
    property var label: ""
    property bool value: false
    property var description: ""
    width: 627
    height: 64
    Column {
        anchors.fill: parent
        spacing: 4
        QLato {
            width: parent.width
            height: 28
            text: label
        }
        QLato {
            width: 531
            height: 32
            font.pixelSize: 12
            font.weight: Font.Medium
            color: "#595959"
            text: description
            lineHeight: 16
            lineHeightMode: Text.FixedHeight
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
        }
    }
    QSwitchTypeB {
        id: _switch
        width: 84
        height: 48
        anchors {
            verticalCenter: parent.verticalCenter
            right: parent.right
        }
        switchOn: value
    }
    function getValue() {
        return _switch.switchOn
    }
    property bool dataChanged: value !== _switch.switchOn
}
