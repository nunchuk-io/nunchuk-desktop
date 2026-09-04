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

// Qt6: Rewritten as Popup (was Menu+MenuItem+Repeater which breaks in Qt6).
// Popup gives full layout control; items are plain Item delegates in a Column.
Popup {
    id: optionMenu
    padding: 0
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

    implicitWidth: menuWidth
    implicitHeight: {
        var cnt = 0
        for (var i = 0; i < mapMenu.length; i++) {
            if (mapMenu[i].visible) cnt++
        }
        return cnt * menuHeight
    }

    property int menuWidth: 250
    property int menuHeight: 48
    property int subMenuWidth: 250
    property int subMenuHeight: 48
    property var internalMenu: null
    property int _origin: Item.Bottom   // updated in onAboutToShow based on open direction

    property var mapMenu: [
        {
            visible: true,
            label: qsTr("menu1"),
            icon: "",
            iconRight: "",
            color: "#031F2B",
            enable: true,
            subMenu: null,
            action: function() { console.log("menu1") }
        }
    ]

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

    contentItem: Item {
        id: _contentWrapper
        implicitWidth: optionMenu.menuWidth
        implicitHeight: optionMenu.implicitHeight
        transformOrigin: optionMenu._origin   // corner closest to the trigger button
        layer.enabled: true
        layer.effect: OpacityMask {
            maskSource: Rectangle {
                width: _contentWrapper.width
                height: _contentWrapper.height
                radius: 8
            }
        }
        Column {
        Repeater {
            model: optionMenu.mapMenu
            delegate: Item {
                id: _delegate
                required property var modelData
                required property int index
                width: optionMenu.menuWidth
                height: modelData.visible ? optionMenu.menuHeight : 0
                visible: modelData.visible

                Rectangle {
                    anchors.fill: parent
                    color: _area.containsMouse && modelData.enable ? "#F5F5F5" : "#FFFFFF"
                }

                QIcon {
                    id: _icon
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
                    anchors.left: _icon.right
                    anchors.leftMargin: modelData.icon !== "" ? 11 : 12
                    anchors.verticalCenter: parent.verticalCenter
                    font.family: "Lato"
                    font.weight: Font.Normal
                    font.pixelSize: 16
                    opacity: modelData.enable ? 1.0 : 0.7
                }

                QIcon {
                    id: _arrow
                    iconSize: modelData.iconRight !== "" ? 24 : 0
                    anchors {
                        right: parent.right
                        rightMargin: 12
                        verticalCenter: parent.verticalCenter
                    }
                    source: modelData.iconRight
                    opacity: modelData.enable ? 1.0 : 0.7
                }

                MouseArea {
                    id: _area
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: modelData.enable ? Qt.PointingHandCursor : Qt.ArrowCursor

                    onEntered: {
                        if (optionMenu.internalMenu) {
                            optionMenu.internalMenu.close()
                            optionMenu.internalMenu = null
                        }
                        if (modelData.subMenu !== null && modelData.subMenu.length > 0) {
                            var subW = optionMenu.subMenuWidth
                            var subH = modelData.subMenu.length * optionMenu.subMenuHeight
                            var defX = optionMenu.x + optionMenu.menuWidth
                            var defY = optionMenu.y + (_delegate.index * optionMenu.menuHeight)
                            var subX = defX
                            var subY = defY

                            // Pre-calculate correct position before createObject.
                            // Done here (QMultiContextMenu context) where Overlay.overlay
                            // is already proven to work (same as onAboutToShow).
                            var overlay = Overlay.overlay
                            var p = optionMenu.parent
                            if (overlay && p) {
                                var orig = overlay.mapToGlobal(0, 0)
                                var ow = overlay.width
                                var oh = overlay.height

                                // ── Horizontal ──
                                var relRight = p.mapToGlobal(defX + subW, 0).x - orig.x
                                if (relRight > ow - 4) {
                                    subX = optionMenu.x - subW   // flip left of main menu
                                }
                                var relLeft = p.mapToGlobal(subX, 0).x - orig.x
                                if (relLeft < 4) {
                                    subX += (4 - relLeft)        // clamp
                                }

                                // ── Vertical ──
                                var relBottom = p.mapToGlobal(0, defY + subH).y - orig.y
                                if (relBottom > oh - 4) {
                                    subY = defY - (relBottom - oh + 4)
                                }
                                var relTop = p.mapToGlobal(0, subY).y - orig.y
                                if (relTop < 4) {
                                    subY += (4 - relTop)
                                }
                            }

                            var sub = subContextMenu.createObject(p, {
                                mapMenu: modelData.subMenu,
                                menuWidth: optionMenu.subMenuWidth,
                                menuHeight: optionMenu.subMenuHeight,
                                _openRight: (subX === defX),  // false = flipped left
                                x: subX,
                                y: subY
                            })
                            sub.open()
                            optionMenu.internalMenu = sub
                        }
                    }

                    onClicked: {
                        if (modelData.enable && !modelData.subMenu) {
                            modelData.action()
                        }
                    }
                }
            }
        }
        } // Column
    } // Rectangle

    onAboutToShow: {
        // Strategy: use mapToGlobal (bypasses all nested-Popup coordinate chains)
        // + Overlay.overlay.mapToGlobal(0,0) as the window origin reference.
        // Runs BEFORE popup is visible → zero flicker.
        var overlay = Overlay.overlay
        if (!overlay || !parent) return
        var orig = overlay.mapToGlobal(0, 0)   // window top-left in screen coords
        var ow   = overlay.width                // window content width
        var oh   = overlay.height               // window content height

        // ── Horizontal ──
        var relRight = parent.mapToGlobal(x + implicitWidth, 0).x - orig.x
        if (relRight > ow - 4) {
            x -= implicitWidth          // flip: open to the LEFT
        }
        var relLeft = parent.mapToGlobal(x, 0).x - orig.x
        if (relLeft < 4) {
            x += (4 - relLeft)          // clamp: don't bleed past left edge
        }

        // ── Vertical ──
        var relTop = parent.mapToGlobal(0, y).y - orig.y
        if (relTop < 4) {
            y = parent.height + 4       // flip: open below button
        }
        var relBottom = parent.mapToGlobal(0, y + implicitHeight).y - orig.y
        if (relBottom > oh - 4) {
            y -= (relBottom - oh + 4)   // clamp: don't bleed past bottom
        }

        // Set scale-animation origin to the corner of the menu closest to the trigger.
        // y < 0 → menu opens ABOVE the button → button is near the BOTTOM of the menu.
        // y ≥ 0 → menu opens BELOW the button → button is near the TOP of the menu.
        _origin = (y < 0) ? Item.Bottom : Item.Top
    }

    // ── Enter: scale+fade from the corner closest to the trigger ──
    enter: Transition {
        ParallelAnimation {
            NumberAnimation { property: "opacity"; from: 0.0; to: 1.0; duration: 200; easing.type: Easing.OutCubic }
            NumberAnimation { target: _contentWrapper; property: "scale"; from: 0.82; to: 1.0; duration: 200; easing.type: Easing.OutCubic }
        }
    }
    // ── Exit: reverse ──
    exit: Transition {
        ParallelAnimation {
            NumberAnimation { property: "opacity"; from: 1.0; to: 0.0; duration: 140; easing.type: Easing.InCubic }
            NumberAnimation { target: _contentWrapper; property: "scale"; from: 1.0; to: 0.82; duration: 140; easing.type: Easing.InCubic }
        }
    }

    onClosed: {
        if (internalMenu) {
            internalMenu.close()
            internalMenu = null
        }
    }

    Component {
        id: subContextMenu
        QSubContextMenu { }
    }
}
