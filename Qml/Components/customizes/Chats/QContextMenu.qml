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
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.12
import "../../origins"

Menu {
    id: optionMenu
    implicitWidth: menuWidth
    implicitHeight: labels.length*menuHeight
    property int menuWidth: 250
    property int menuHeight: 48
    property var functions:[]
    property var colors:[]
    property var labels: [""]
    property var icons: ["", "", "", ""]
    property var enables: [true, true, true, true, true, true, true, true]
    signal itemClicked(var index)
    background: Rectangle {
        implicitWidth: menuWidth
        implicitHeight: labels.length*menuHeight
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
        model: labels
        MenuItem {
            id: delegateMenu
            height: menuHeight
            text: labels[index]
            icon.source: icons[index]
            enabled: enables[index]
            onTriggered: { itemClicked(index) }
            background: Rectangle {
                implicitWidth: menuWidth
                implicitHeight: menuHeight
                radius: 8
                opacity: delegateMenu.enabled ? 1 : 0.7
                color: {
                    if(delegateMenu.enabled){
                        return delegateMenu.hovered ? "#F5F5F5": "#FFFFFF"
                    }else{
                        return "#EAEAEA"
                    }
                }
            }
            contentItem: Item {
                width: menuWidth
                height: menuHeight
                QImage {
                    id:_icon
                    width: _icon.source == "" ? 0 : 24
                    height: 24
                    anchors.left: parent.left
                    anchors.leftMargin: 12
                    anchors.verticalCenter: parent.verticalCenter
                    source: icons[index]
                    opacity: delegateMenu.enabled ? 1.0 : 0.7
                }
                QText {
                    text: labels[index]
                    color: {
                        if(!delegateMenu.enabled) return "#595959"
                        if(colors.length == 0){
                            "#031F2B"
                        }
                        else{
                            colors[index]
                        }
                    }
                    width: paintedWidth
                    anchors.left: _icon.right
                    anchors.leftMargin: _icon.source == "" ? 0 : 11
                    font.family: "Lato"
                    font.weight: Font.Normal
                    font.pixelSize: 16
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }
    }
}
