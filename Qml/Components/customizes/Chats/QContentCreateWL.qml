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
import QtQuick 2.12
import QtQuick.Controls 2.1
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.0

Item {
    id: contentCreatingWl
    property alias topArea: _topArea
    property alias bottomArea: _bottomArea
    Rectangle {
        id: mask
        width: parent.width - 4
        height: parent.height - 4
        radius: 24
        visible: false
        anchors.centerIn: parent
    }
    Column {
        id: content
        width: parent.width - 4
        height: parent.height - 4
        anchors.centerIn: parent
        visible: false
        Rectangle {
            id:_topArea
            width: parent.width
            height: parent.height*0.7
            color: "#F5F5F5"
        }
        Rectangle {
            id:_bottomArea
            width: parent.width
            height: parent.height*0.3
            color: "#FFFFFF"
        }
    }
    OpacityMask {
        id: contentMasked
        anchors.fill: content
        source: content
        maskSource: mask
    }
    DropShadow {
        anchors.fill: contentMasked
        verticalOffset: 2
        cached: true
        radius: 8
        samples: 16
        color: Qt.rgba(0, 0, 0, 0.15)
        source: contentMasked
    }
}
