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
import "../../origins"
import "../../customizes/Texts"

Rectangle {
    id: radioRoot
    property bool selected: false
    property string labelTop: ""
    property string labelBottom: ""
    property int labelMaxWidth: 467
    signal buttonClicked()
    border.width: 2
    border.color: selected ? "#000000" : "#DEDEDE"
    radius: 12

    Row {
        anchors.fill: parent
        anchors.margins: 18
        spacing: 12
        QImage {
            id: icon
            width: 24
            height: 24
            source: selected ? "qrc:/Images/Images/radio-selected-dark.svg" : "qrc:/Images/Images/radio-dark.svg"
            anchors.verticalCenter: _top.verticalCenter
            MouseArea {
                id: mouse
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                onClicked: { buttonClicked() }
            }
        }
        Column {
            id: info
            spacing: 4
            QLato {
                id: _top
                text: labelTop
                font.weight: Font.DemiBold
                width: Math.min(labelMaxWidth, implicitWidth)
                wrapMode: Text.WordWrap
            }
            QLato {
                text: labelBottom
                width: Math.min(labelMaxWidth, implicitWidth)
                visible: labelBottom != ""
                wrapMode: Text.WordWrap
                lineHeight: 28
                lineHeightMode: Text.FixedHeight
            }
        }
    }
}
