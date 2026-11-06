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
import QtQuick
import QtQuick.Controls
import Qt5Compat.GraphicalEffects
import "../../../../Components/origins"
import "../../../../Components/customizes/Texts"

Menu {
    id: _menu
    property int menuWidth: 300
    property var items: []
    signal itemClicked(int index)
    implicitWidth: menuWidth
    padding: 8
    background: Rectangle {
        implicitWidth: _menu.menuWidth
        radius: 8
        color: "#FFFFFF"
        layer.enabled: true
        layer.effect: DropShadow {
            horizontalOffset: 3
            verticalOffset: 3
            radius: 8.0
            samples: 17
            color: "#80000000"
        }
    }
    Repeater {
        model: _menu.items
        MenuItem {
            id: _item
            readonly property bool hasSub: modelData.subtitle !== undefined && modelData.subtitle !== ""
            visible: modelData.visible !== false
            height: visible ? (hasSub ? _col.implicitHeight + 24 : 48) : 0
            width: _menu.menuWidth - 16
            onTriggered: _menu.itemClicked(index)
            background: Rectangle {
                radius: 8
                color: _item.hovered ? "#F5F5F5" : "#FFFFFF"
            }
            contentItem: Item {
                Column {
                    id: _col
                    anchors {
                        left: parent.left
                        right: parent.right
                        leftMargin: 12
                        rightMargin: 12
                        verticalCenter: parent.verticalCenter
                    }
                    spacing: 4
                    QLato {
                        width: parent.width
                        text: modelData.title
                        color: modelData.color !== undefined ? modelData.color : "#031F2B"
                        font.pixelSize: 16
                        horizontalAlignment: Text.AlignLeft
                        wrapMode: Text.WordWrap
                    }
                    QLato {
                        width: parent.width
                        visible: _item.hasSub
                        text: _item.hasSub ? modelData.subtitle : ""
                        color: "#757575"
                        font.pixelSize: 12
                        horizontalAlignment: Text.AlignLeft
                        wrapMode: Text.WordWrap
                    }
                }
            }
        }
    }
}
