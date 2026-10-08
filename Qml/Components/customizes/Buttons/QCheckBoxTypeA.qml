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
    id: _item
    width: 539
    height: 88
    border.width: 2
    border.color: "#000000"
    radius: 12
    property string label: ""
    property string description: ""
    property bool checked: false
    signal buttonClicked()
    MouseArea {
        anchors.fill: parent
        onClicked: {
            buttonClicked()
        }
    }
    Row {
        anchors {
            verticalCenter: parent.verticalCenter
            left: parent.left
            leftMargin: 18
        }
        height: 52
        spacing: 12
        QCheckBox {
            anchors.top: parent.top
            checked: _item.checked
            partiallyChecked: false
            onCheckboxClicked: {
                _item.checked = !_item.checked
                buttonClicked()
            }
        }
        Column {
            width: _item.width - 12 -24 - 2*18
            spacing: 4
            QLato {
                text: label
                font.weight: Font.Bold
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }
            QLato {
                text: description
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }
        }
    }
}
