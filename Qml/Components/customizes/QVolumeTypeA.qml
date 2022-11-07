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
import "../origins"

Row {
    spacing: 16
    property string value: "0"
    signal plusClicked()
    signal minusClicked()


    MouseArea {
        id: minusMouse
        width: childrenRect.width
        height: childrenRect.height
        anchors.verticalCenter: parent.verticalCenter
        Rectangle {
            width: 36
            height: 36
            border.color: "#031F2B"
            border.width: 2
            radius: width
            color: minusMouse.pressed ? "#031F2B" : "transparent"
            Rectangle {
                width: 10
                height: 2
                anchors.centerIn: parent
                color: !minusMouse.pressed ? "#031F2B" : "#FFFFFF"
            }
        }
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        onClicked: { minusClicked() }
    }

    Item {
        width: 50
        height: 50
        anchors.verticalCenter: parent.verticalCenter
        Rectangle {
            id: background
            anchors.centerIn: parent
            width: parent.width - 2
            height: parent.height - 2
            radius: 8
            color: "#FFFFFF"
            border.color: "#DEDEDE"
        }

        InnerShadow {
            anchors.fill: background
            cached: true
            horizontalOffset: 0
            verticalOffset: 0
            radius: 8
            samples: 16
            color: Qt.rgba(3, 31, 43, 0.14)
            smooth: true
            source: background

            QText {
                anchors.centerIn: parent
                color: "#031F2B"
                font.pixelSize: 16
                text: value
            }
        }
    }

    MouseArea {
        id: plusMouse
        width: childrenRect.width
        height: childrenRect.height
        anchors.verticalCenter: parent.verticalCenter
        Rectangle {
            width: 36
            height: 36
            border.color: "#031F2B"
            border.width: 2
            radius: width
            color: plusMouse.pressed ? "#031F2B" : "transparent"
            Rectangle {
                width: 10
                height: 2
                anchors.centerIn: parent
                color: !plusMouse.pressed ? "#031F2B" : "#FFFFFF"
            }
            Rectangle {
                width: 2
                height: 10
                anchors.centerIn: parent
                color: !plusMouse.pressed ? "#031F2B" : "#FFFFFF"
            }
        }
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        onClicked: { plusClicked() }
    }
}
