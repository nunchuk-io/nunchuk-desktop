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
import "../origins"
import "../customizes/Texts"
import "../customizes/Buttons"

Item {
    id: tabroot
    property string label: "value"
    property alias fontPixelSize: tablabel.font.pixelSize
    property alias horizontalAlignment: tablabel.horizontalAlignment

    enabled: false
    property bool isCurrentTab: false
    property bool sortDescending: true
    property int iconPading: 5

    Rectangle {
        anchors.fill: parent
        color: "#C9DEF1"
        opacity: tabmouse.containsMouse ? 1 : 0.5
    }

    Rectangle {
        width: parent.width
        height: 1
        color: "#839096"
        anchors.bottom: parent.bottom
    }

    Row {
        anchors {
            fill: parent
            topMargin: 10
            bottomMargin: 10
            leftMargin: 16
            rightMargin: 16
        }
        spacing: tabroot.enabled ? iconPading : 0
        layoutDirection: tabroot.horizontalAlignment === Text.AlignLeft ? Qt.LeftToRight : Qt.RightToLeft
        QText {
            id: tablabel
            height: parent.height
            text: label
            font.family: "Lato"
            font.weight: Font.Bold
            color: "#323E4A"
            font.capitalization: Font.AllUppercase
        }
        Image {
            visible: tabroot.enabled && tabroot.isCurrentTab
            width: 20
            height: 20
            source: !sortDescending ? "qrc:/Images/Images/SortAscen.png":
                                      "qrc:/Images/Images/SortDescen.png"
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    signal tabClicked(var sortOrder)
    MouseArea {
        id: tabmouse
        enabled: tabroot.enabled
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        anchors.fill: parent
        onClicked: {
            sortDescending = !sortDescending
            tabClicked(sortDescending)
        }
    }
}
