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
import QtQuick 2.0
import "../../origins"

Rectangle {
    id: _badge
    property alias  text: _txt.text
    property alias  fontWeight: _txt.font.weight
    property alias  fontColor: _txt.color
    property bool   iconInRight: false
    property int    iconSize: 16
    property int    fontSize: 16
    property alias  font: _txt.font
    property string icon: ""
    property bool   useMouse: false
    property int    topMargin: 4
    property int    leftMargin: 10

    implicitWidth: _row.width + leftMargin*2
    implicitHeight: _row.height + topMargin*2

    radius: 20
    signal badgeClicked()

    Row {
        id: _row
        anchors.centerIn: parent
        spacing: 4
        layoutDirection: iconInRight ?  Qt.RightToLeft : Qt.LeftToRight
        QIcon {
            iconSize: _badge.iconSize
            source: icon
            visible: icon != ""
            anchors.verticalCenter: parent.verticalCenter
        }
        QLato {
            id: _txt
            font.weight: Font.Bold
            font.pixelSize: fontSize
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    MouseArea {
        id: mouse
        visible: useMouse
        anchors.fill: _badge
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        onClicked: {
            badgeClicked()
        }
    }
}
