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
    property string label: ""
    property string fontFamily: "Lato"
    property string textBadge: ""
    property int fontPixelSize: 14
    property int fontWeight: Font.Bold
    signal buttonClicked()
    border.width: 2
    border.color: selected ? "#000000" : "#DEDEDE"
    radius: 12
    Row {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 8
        QImage {
            id: icon
            width: 24
            height: 24
            source: radioRoot.selected ? "qrc:/Images/Images/radio-selected-dark.svg" : "qrc:/Images/Images/radio-dark.svg"
            anchors.verticalCenter: parent.verticalCenter
            MouseArea {
                id: mouse
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                onClicked: { buttonClicked() }
            }
        }
        QText {
            id: text
            text: label
            font.family: fontFamily
            font.pixelSize: fontPixelSize
            font.weight: fontWeight
            color: "#031F2B"
            anchors.verticalCenter: parent.verticalCenter
            width: paintedWidth
        }
        QBadge {
            text: textBadge
            color: "#EAEAEA"
            visible: textBadge !== ""
        }
    }
}


