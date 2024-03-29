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

Rectangle {
    width: 48
    height: 48
    radius: width
    property bool isTinyShow: false
    property bool isCurrentItem: false
    property alias icon: iconsource.source
    property var backgroundColor: Qt.rgba(255, 255, 255, 0.3)
    color: isCurrentItem ? backgroundColor : "transparent"
    border.color: mouse.containsMouse ? backgroundColor : "transparent"
    Rectangle{
        width: 8
        height: 8
        radius: 4
        visible: isTinyShow
        color: "#CF4018"
        anchors{
            left:iconsource.right
            top:iconsource.top
            topMargin: -4
        }
    }

    QIcon {
        id: iconsource
        iconSize: 24
        anchors.centerIn: parent
        scale: mouse.pressed && !isCurrentItem ? 0.9 : 1
        transformOrigin: Item.Center
    }

    signal buttonClicked()
    MouseArea {
        id: mouse
        hoverEnabled: true
        cursorShape: isCurrentItem ? Qt.ArrowCursor : Qt.PointingHandCursor
        anchors.fill: parent
        onClicked: buttonClicked()
    }
}
