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
import "../../customizes/Texts"
import "../../origins"

QBaseButton {
    id: rootTextButton
    width: 137
    height: 48
    property alias  label: text
    property bool   processing: false
    property bool   iconVisible: false
    property var    iconSource: ["", ""]

    QBackgroundState {
        anchors.fill: parent
    }
    QLatoState {
        id: text
        anchors.fill: parent
        anchors.centerIn: rootTextButton
        text: qsTr("TEXT")
        font.weight: Font.DemiBold
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    Rectangle {
        width: 2
        height: 24
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: 60
        color: "#595959"
        visible: iconVisible
    }
    QImage {
        width: 24
        height: 24
        source: btnMouse.containsMouse ? iconSource[0] : iconSource[1]
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: 20
        visible: iconVisible
    }
}
