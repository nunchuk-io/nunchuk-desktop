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
import "../../origins"
import "../../customizes/Texts"

Row {
    id: radioRoot
    spacing: 8
    property bool selected: false
    property alias content: loader.sourceComponent
    readonly property Item contentItem: loader.item
    property bool isOverlay: false
    signal buttonClicked()
    Loader {
        id: loader
        width: radioRoot.width - icon.width - 8
        anchors.verticalCenter: parent.verticalCenter
        sourceComponent: contentItem
        MouseArea {
            anchors.fill: loader
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor
            onClicked: { buttonClicked() }
        }
    }
    Loader {
        id: icon
        anchors.verticalCenter: parent.verticalCenter
        width: 24
        height: 24
        sourceComponent: isOverlay ? radioOverlay : radioIcon
        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor
            onClicked: { buttonClicked() }
        }
    }
    function iconSource() {
        return radioRoot.selected ? "qrc:/Images/Images/radio-selected-dark.svg" : "qrc:/Images/Images/radio-dark.svg"
    }
    Component {
        id: radioOverlay
        ColorOverlay {
            source: QIcon {
                id: icon
                iconSize: 24
                source: iconSource()
            }
            color: radioRoot.enabled ? "#031F2B" : "#666666"
        }
    }
    Component {
        id: radioIcon
        QIcon {
            iconSize: 24
            source: iconSource()
        }
    }
}