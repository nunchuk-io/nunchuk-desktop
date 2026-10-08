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
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.12
import "../../origins"
import "../../customizes/Texts"
import "../../customizes/Buttons"
import "../../../Components/customizes/Chats"
import "../../../../localization/STR_QML.js" as STR

MenuItem {
    id: delegateMenu
    height: visible ? menuHeight : 0
    visible: itemMenu.visible
    signal itemClicked()
    property var itemMenu: []
    property var storeMenu: null
    text: itemMenu.label
    icon.source: itemMenu.icon
    enabled: itemMenu.enable
    onTriggered: {
        var cacheEnable = delegateMenu.enabled
        delegateMenu.enabled = false
        itemClicked()
        delegateMenu.enabled = cacheEnable
    }
    background: Rectangle {
        implicitWidth: menuWidth
        implicitHeight: menuHeight
        radius: 8
        opacity: delegateMenu.enabled ? 1 : 0.7
        color: {
            if(delegateMenu.enabled){ return delegateMenu.hovered ? "#F5F5F5": "#FFFFFF" }
            else{ return "#EAEAEA" }
        }
    }
    contentItem: Item {
        width: menuWidth
        height: menuHeight
        QIcon {
            id:_icon
            iconSize: _icon.source == "" ? 0 : 24
            anchors {
                left: parent.left
                leftMargin: 12
                verticalCenter: parent.verticalCenter
            }
            source: itemMenu.icon
            opacity: delegateMenu.enabled ? 1.0 : 0.7
        }
        QText {
            text: itemMenu.label
            color: {
                if(!delegateMenu.enabled){ return "#595959" }
                return itemMenu.color
            }
            width: paintedWidth
            anchors.left: _icon.right
            anchors.leftMargin: _icon.source == "" ? 0 : 11
            font.family: "Lato"
            font.weight: Font.Normal
            font.pixelSize: 16
            anchors.verticalCenter: parent.verticalCenter
        }
        QIcon {
            id:_arrow
            iconSize: _icon.source == "" ? 0 : 24
            anchors {
                right: parent.right
                rightMargin: 12
                verticalCenter: parent.verticalCenter
            }
            source: itemMenu.iconRight
            opacity: delegateMenu.enabled ? 1.0 : 0.7
        }
    }
}
