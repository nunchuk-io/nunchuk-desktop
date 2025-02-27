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
    id: _item
    width: 539
    height: 24
    property string label: ""
    property bool checked: false
    property int fontWeight: Font.Bold
    QLato {
        id: text
        text: label
        font.weight: fontWeight
        anchors.verticalCenter: parent.verticalCenter
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
    }
    QCheckBox {
        anchors {
            verticalCenter: text.verticalCenter
            right: parent.right
        }
        checked: _item.checked
        partiallyChecked: false
        onCheckboxClicked: {
            _item.checked = !_item.checked
            buttonClicked()
        }
    }

    signal buttonClicked()
}
