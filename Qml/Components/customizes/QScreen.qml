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

Item {
    id: root
    width: QAPP_DEVICE_WIDTH
    height: QAPP_DEVICE_HEIGHT
    property int popupWidth: 800
    property int popupHeight: 700
    transformOrigin: Item.TopLeft
    property bool isOnTop: false
    enabled: isOnTop
    signal eventReceiver(int event, int data)
    property alias background: background
    Component.onCompleted: {QMLHandle.doRegisterQML(root)}
    Component.onDestruction: {QMLHandle.unRegisterQML(root)}
    Rectangle {
        id: background
        anchors.fill: parent
        color: Qt.rgba(0, 0, 0, 0.7)
        scale: 5
    }
    MouseArea {
        anchors.fill: parent
        onClicked: {root.focus = true}
    }
}
