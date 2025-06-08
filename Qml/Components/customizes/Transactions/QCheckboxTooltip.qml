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
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

Item {
    id: _item
    width: parent.width
    height: 28
    property alias text: textContent.text
    property alias tooltip: tooltipContent.toolTip
    property bool  checked: false
    property alias rightOfParent: tooltipContent.rightOfParent
    property int   pointerPositionIndex: 9
    property int   pointerPositionRatio: 10

    QLato {
        z:1
        id: textContent
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        QTooltip {
            id: tooltipContent
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.right
            anchors.leftMargin: 8
            source: "qrc:/Images/Images/help_outline_24px.png"
            pointerPositionIndex: _item.pointerPositionIndex
            pointerPositionRatio: _item.pointerPositionRatio
        }
    }
    QCheckBox {
        anchors {
            verticalCenter: parent.verticalCenter
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
