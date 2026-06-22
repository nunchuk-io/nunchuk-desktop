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
import QtQuick.Controls 2.0
import "../../../Components/customizes/Texts"

Item {
    id: root

    // ── Public API ───────────────────────────────────────────────────────
    property var timelineStages: [
        {label: "Stage 1", date: "05/29/2028", color: "#4CAF50", percentage: 20},
        {label: "Stage 2", date: "05/29/2032", color: "#7B3FF2", percentage: 60},
        {label: "Stage 3", date: "05/29/2046", color: "#FFD700", percentage: 20}
    ]
    property bool displayPercentage: false

    // ── Internal state ───────────────────────────────────────────────────
    property var   _segWidths:  []      // computed width for each segment
    property real  _totalW:     0       // sum of all segment widths

    // Height = 3 rows + 2 gaps + scrollbar slot (scrollbar only visible when needed)
    // labelRow(16) + gap(2) + barRow(7) + gap(2) + dateRow(16) + scrollbar(10) = 53
    width:  parent.width
    height: 53

    // ── Hidden text-measure layer ────────────────────────────────────────
    // Measures the real pixel width of each label so we can size segments.
    Item {
        id: measureLayer
        visible: false
        width:  0
        height: 0

        Repeater {
            id: labelMeasure
            model: root.timelineStages
            Text {
                font.pixelSize: 10
                text: modelData.label
                      + (root.displayPercentage
                         ? " (" + modelData.percentage + "%)"
                         : "")
            }
        }

        Repeater {
            id: dateMeasure
            model: root.timelineStages
            Text {
                font.pixelSize: 10
                text: modelData.date
            }
        }
    }

    // ── Recalculate segment widths ────────────────────────────────────────
    // Logic:
    //   1. For each segment i:  minPx[i] = max(labelWidth[i], dateWidth[i]) + HPAD*2
    //   2. Each segment must cover its percentage share → ratio[i] = minPx[i] / pct[i]
    //   3. Use the LARGEST ratio so even the smallest-percentage segment fits its text.
    //   4. Segment width = pct[i] * maxRatio  (proportional, text always fits)
    //   5. If ΣwidthS > container → Flickable scrolls horizontally.

    Component.onCompleted:      Qt.callLater(_recalc)
    onTimelineStagesChanged:    Qt.callLater(_recalc)
    onDisplayPercentageChanged: Qt.callLater(_recalc)

    function _recalc() {
        var n = timelineStages.length
        if (n === 0) return

        var HPAD   = 12   // horizontal padding each side
        var MIN_W  = 32   // absolute floor per segment

        // Step 1: measure text widths
        var maxRatio = 0
        for (var i = 0; i < n; i++) {
            var lw = labelMeasure.itemAt(i) ? labelMeasure.itemAt(i).implicitWidth : 60
            var dw = dateMeasure.itemAt(i)  ? dateMeasure.itemAt(i).implicitWidth  : 60
            var needed = Math.max(lw, dw) + HPAD * 2
            needed = Math.max(needed, MIN_W)

            var pct = parseFloat(timelineStages[i].percentage) || 1
            var ratio = needed / pct   // px per percentage-point required
            if (ratio > maxRatio) maxRatio = ratio
        }

        // Step 2: assign widths proportionally using maxRatio
        var widths = []
        var total  = 0
        for (var j = 0; j < n; j++) {
            var p = parseFloat(timelineStages[j].percentage) || 0
            var w = Math.round(p * maxRatio)
            widths.push(w)
            total += w
        }

        _segWidths = widths
        _totalW    = total
    }

    // ── Flickable (horizontal scroll when content > container) ──────────
    Flickable {
        id: flick
        anchors.fill: parent
        contentWidth:  Math.max(root.width, root._totalW)
        contentHeight: root.height
        clip:          true
        flickableDirection: Flickable.HorizontalFlick

        ScrollBar.horizontal: ScrollBar {
            id: hbar
            anchors.bottom: parent.bottom
            height: 10
            policy: root._totalW > root.width
                    ? ScrollBar.AlwaysOn
                    : ScrollBar.AlwaysOff
        }

        // ── Content column ────────────────────────────────────────────
        Column {
            id: contentCol
            width:   flick.contentWidth
            spacing: 2

            // Row 1 – Labels
            Row {
                id: labelRow
                width:  parent.width
                height: 16
                spacing: 0

                Repeater {
                    model: root.timelineStages
                    Item {
                        width:  root._segWidths.length > index ? root._segWidths[index] : 0
                        height: 16

                        Text {
                            width: parent.width
                            anchors { left: parent.left; top: parent.top; leftMargin: 0 }
                            text:           modelData.label
                                            + (root.displayPercentage
                                               ? " (" + modelData.percentage + "%" + ")"
                                               : "")
                            font.pixelSize: 10
                            font.family:    "Lato"
                            color:          "#031F2B"
                            // No elide needed — segment is wide enough by construction
                        }
                    }
                }
            }

            // Row 2 – Color bars
            Row {
                id: barRow
                width:  parent.width
                height: 7
                spacing: 0

                Repeater {
                    model: root.timelineStages
                    Rectangle {
                        // Last bar fills remainder to avoid rounding gaps
                        width: {
                            if (root._segWidths.length <= index) return 0
                            if (index < root.timelineStages.length - 1)
                                return root._segWidths[index]
                            // last: fill to contentWidth
                            var used = 0
                            for (var k = 0; k < index; k++) used += root._segWidths[k]
                            return flick.contentWidth - used
                        }
                        height: parent.height
                        color:  modelData.color
                        radius: index === 0 ? 2
                              : index === root.timelineStages.length - 1 ? 2
                              : 0
                    }
                }
            }

            // Row 3 – Dates
            Row {
                id: dateRow
                width:  parent.width
                height: 16
                spacing: 0

                Repeater {
                    model: root.timelineStages
                    Item {
                        width:  root._segWidths.length > index ? root._segWidths[index] : 0
                        height: 16

                        Text {
                            width: parent.width
                            anchors { left: parent.left; top: parent.top }
                            text:           modelData.date
                            font.pixelSize: 10
                            font.family:    "Lato"
                            color:          "#757575"
                        }
                    }
                }
            }
        } // Column
    } // Flickable
} // Item

