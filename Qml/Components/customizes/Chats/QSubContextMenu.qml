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
import "../../origins"
import "../../customizes/Texts"
import "../../customizes/Buttons"
import "../../../Components/customizes/Chats"
import "../../../../localization/STR_QML.js" as STR

// Qt6: Rewritten as Popup (was Menu+Repeater which breaks in Qt6).
Popup {
    id: subOptionsMenu
    padding: 0
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

    implicitWidth: menuWidth
    implicitHeight: {
        if (mapMenu === null) return 0
        var cnt = 0
        for (var i = 0; i < mapMenu.length; i++) {
            if (mapMenu[i].visible) cnt++
        }
        return cnt * menuHeight
    }

    property int menuWidth: 250
    property int menuHeight: 48
    property var mapMenu: null
    property bool _openRight: true  // set by QMultiContextMenu.onEntered; drives transformOrigin
    // Position is pre-calculated by QMultiContextMenu.onEntered before createObject,
    // so no onAboutToShow repositioning is needed here.

    background: Rectangle {
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

    // ── Enter: scale from the edge attached to the parent menu item ──
    enter: Transition {
        ParallelAnimation {
            NumberAnimation { property: "opacity"; from: 0.0; to: 1.0; duration: 180; easing.type: Easing.OutCubic }
            NumberAnimation { target: _subContentWrapper; property: "scale"; from: 0.82; to: 1.0; duration: 180; easing.type: Easing.OutCubic }
        }
    }
    exit: Transition {
        ParallelAnimation {
            NumberAnimation { property: "opacity"; from: 1.0; to: 0.0; duration: 120; easing.type: Easing.InCubic }
            NumberAnimation { target: _subContentWrapper; property: "scale"; from: 1.0; to: 0.82; duration: 120; easing.type: Easing.InCubic }
        }
    }

    contentItem: Item {
        id: _subContentWrapper
        implicitWidth: subOptionsMenu.menuWidth
        implicitHeight: subOptionsMenu.implicitHeight
        // Scale origin: Left when submenu opens right of parent, Right when flipped
        transformOrigin: subOptionsMenu._openRight ? Item.Left : Item.Right
        layer.enabled: true
        layer.effect: OpacityMask {
            maskSource: Rectangle {
                width: _subContentWrapper.width
                height: _subContentWrapper.height
                radius: 8
            }
        }
        Column {
        Repeater {
            model: subOptionsMenu.mapMenu
            delegate: Item {
                required property var modelData
                required property int index
                width: subOptionsMenu.menuWidth
                height: modelData.visible ? subOptionsMenu.menuHeight : 0
                visible: modelData.visible

                Rectangle {
                    anchors.fill: parent
                    color: _sarea.containsMouse && modelData.enable ? "#F5F5F5" : "#FFFFFF"
                }

                QIcon {
                    id: _sicon
                    iconSize: modelData.icon !== "" ? 24 : 0
                    anchors {
                        left: parent.left
                        leftMargin: 12
                        verticalCenter: parent.verticalCenter
                    }
                    source: modelData.icon
                    opacity: modelData.enable ? 1.0 : 0.7
                }

                QText {
                    text: modelData.label
                    color: modelData.enable ? modelData.color : "#595959"
                    anchors.left: _sicon.right
                    anchors.leftMargin: modelData.icon !== "" ? 11 : 12
                    anchors.verticalCenter: parent.verticalCenter
                    font.family: "Lato"
                    font.weight: Font.Normal
                    font.pixelSize: 16
                    opacity: modelData.enable ? 1.0 : 0.7
                }

                MouseArea {
                    id: _sarea
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: modelData.enable ? Qt.PointingHandCursor : Qt.ArrowCursor
                    onClicked: {
                        if (modelData.enable) {
                            modelData.action()
                            subOptionsMenu.close()
                        }
                    }
                }
            }
        }
        } // Column
    } // Rectangle
}
