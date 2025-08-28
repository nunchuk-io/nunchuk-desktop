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
    height: visible ? itemMenu.height : 0
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
        implicitHeight: itemMenu.height
        radius: 8
        opacity: delegateMenu.enabled ? 1 : 0.7
        color: {
            if(delegateMenu.enabled){ return delegateMenu.hovered ? "#F5F5F5": "#FFFFFF" }
            else{ return "#EAEAEA" }
        }
    }
    contentItem: Loader {
        anchors.fill: parent
        sourceComponent: itemMenu.isHeadLine ? itemMiniScriptHeadLine :
                         itemMenu.isSeparator ? itemMiniScriptSeparator : itemMiniScriptDelegate
    }
    Component {
        id: itemMiniScriptHeadLine
        Item {
            QLato {
                anchors.left: parent.left
                width: menuWidth
                height: itemMenu.height
                text: itemMenu.label
                font.weight: Font.Bold
                font.pixelSize: 12
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
            }
        }
        
    }
    Component {
        id: itemMiniScriptSeparator
        Item {
            QLine {
                anchors {
                    left: parent.left
                    verticalCenter: parent.verticalCenter
                }
                width: menuWidth - 12*2
            }
        }        
    }
    Component {
        id: itemMiniScriptDelegate
        Item {
            QIcon {
                id:_icon
                iconSize: _icon.source == "" ? 0 : 24
                anchors {
                    left: parent.left
                    top: _col.top
                }
                source: itemMenu.icon
                opacity: delegateMenu.enabled ? 1.0 : 0.7
            }
            Column {
                id: _col
                width: menuWidth - 12*2 - 32
                height: itemMenu.height - 10*2
                spacing: 2
                anchors.left: _icon.right
                anchors.leftMargin: _icon.source == "" ? 0 : 8
                anchors.verticalCenter: parent.verticalCenter
                QLato {
                    id: _label
                    text: itemMenu.label
                    color: itemMenu.color
                    width: _col.width
                    height: 24
                    font.weight: Font.Normal
                    font.pixelSize: 16
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                }
                QLato {
                    text: itemMenu.description
                    color: itemMenu.colorDescription
                    width: _col.width
                    height: paintedHeight
                    font.weight: Font.Normal
                    font.pixelSize: 14
                    lineHeight: 16
                    lineHeightMode: Text.FixedHeight
                    wrapMode: Text.WordWrap
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                }
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
}
