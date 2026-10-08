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
import Qt5Compat.GraphicalEffects
import "../../origins"
import "../../customizes/Texts"

// BUGFIX: root changed from Row to Item. Row forbids anchoring its direct children (only plain
// x/width bindings are allowed), so a single click-through MouseArea covering the whole control
// could not be anchors.fill'd directly under a Row root. The label/icon are now laid out by an
// inner Row (still fine to anchors.fill *that*, since the restriction only applies to a Row's own
// children, not to the Row item itself), with the MouseArea as its sibling. `layoutDirection` is
// re-exposed via alias so every existing caller (QRadioButtonTypeA etc.) keeps working unchanged.
Item {
    id: radioRoot
    property alias layoutDirection: row.layoutDirection
    // Row exposed `spacing`/implicit auto-size natively; Item doesn't, so re-expose both to stay
    // compatible with callers that set `spacing:` directly (QRadioButtonTypeB, QEditTimelockSelectRadio)
    // or rely on auto-sizing with no explicit width/height (QSingleSignerExistDelegate, QCoinCollectionRadioDelegate, QRadioSelectPolicy).
    property alias spacing: row.spacing
    implicitWidth: row.implicitWidth
    implicitHeight: row.implicitHeight
    property bool  selected: false
    property alias content: loader.sourceComponent
    readonly property Item contentItem: loader.item
    property bool isOverlay: false
    // Opt-in only: lets content (e.g. QRadioButtonTypeB's editable field) win
    // clicks over hitArea below. Default false keeps every other caller as-is.
    property bool contentOnTop: false
    signal buttonClicked()
    Row {
        id: row
        anchors.fill: parent
        spacing: 8
        Loader {
            id: loader
            width: row.width - icon.width - row.spacing
            anchors.verticalCenter: parent.verticalCenter
            sourceComponent: contentItem
        }
        Loader {
            id: icon
            anchors.verticalCenter: parent.verticalCenter
            width: 24
            height: 24
            sourceComponent: isOverlay ? radioOverlay : radioIcon
        }
    }
    // Previously 2 separate small MouseAreas nested inside the label/icon Loaders - their hit
    // region depended on the loaded item's own bounds (e.g. the ColorOverlay-wrapped icon), which
    // could end up misaligned with the rendered graphic, making part of the row unclickable. One
    // MouseArea spanning the whole row guarantees the entire visible label+icon area is clickable.
    // BUGFIX: rows can sit flush against a scrollable container's edges (see QPopupHardwareAddKey.qml),
    // where a ScrollBar's real hit-region or the container's own clip boundary can steal clicks right
    // at the row's edges. Extend the hit area a bit past the row's left/right edges (not top/bottom,
    // rows are packed with spacing:0) as a general safety margin for any such flush-edge usage.
    MouseArea {
        id: hitArea
        z: contentOnTop ? -1 : 0
        anchors.fill: parent
        anchors.leftMargin: -12
        anchors.rightMargin: -12
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        onClicked: { buttonClicked() }
    }
    function iconSource() {
        return radioRoot.selected ? "qrc:/Images/Images/radio-selected-dark.svg" : "qrc:/Images/Images/radio-dark.svg"
    }
    Component {
        id: radioOverlay
        ColorOverlay {
            source: QIcon {
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
