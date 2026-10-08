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
    property var   _segWidths:  []      // text-minimum widths (determines _totalW / scroll threshold)
    property real  _totalW:     0       // sum of _segWidths → used as contentWidth floor
    property var   _barWidths:  []      // actual pixel width of each segment (used by ALL 3 rows)
    property var   _normalizedPercentages: []  // normalized percentages if total > 100
    property real  _totalPercentage: 0  // sum of all percentages

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
                         ? " (" + (root._normalizedPercentages.length > index
                                    ? Math.round(root._normalizedPercentages[index])
                                    : modelData.percentage) + "%)"
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
    onWidthChanged:             Qt.callLater(_recalc)   // recompute when container resizes
    onTimelineStagesChanged:    Qt.callLater(_recalc)
    onDisplayPercentageChanged: Qt.callLater(_recalc)

    function _recalc() {
        var n = timelineStages.length
        if (n === 0) return

        var HPAD   = 12   // horizontal padding each side
        var MIN_W  = 32   // absolute floor per segment

        // Step 0: Calculate total percentage and normalize if needed
        var totalPct = 0
        var rawPercentages = []
        for (var pi = 0; pi < n; pi++) {
            var rawPct = parseFloat(timelineStages[pi].percentage) || 0
            rawPercentages.push(rawPct)
            totalPct += rawPct
        }
        
        _totalPercentage = totalPct
        var normalizedPercentages = []
        
        // Normalize percentages if total > 100
        if (totalPct > 100) {
            for (var ni = 0; ni < n; ni++) {
                normalizedPercentages.push((rawPercentages[ni] / totalPct) * 100)
            }
            _normalizedPercentages = normalizedPercentages
        } else {
            _normalizedPercentages = rawPercentages
            normalizedPercentages = rawPercentages
        }

        // Step 1: measure text widths for each segment
        var textMins = []
        for (var i = 0; i < n; i++) {
            var lw = labelMeasure.itemAt(i) ? labelMeasure.itemAt(i).implicitWidth : 60
            var dw = dateMeasure.itemAt(i)  ? dateMeasure.itemAt(i).implicitWidth  : 60
            var needed = Math.max(lw, dw) + HPAD * 2
            textMins.push(Math.max(needed, MIN_W))
        }

        // Step 2: per-segment width = max(proportional share of container, 20%-equivalent width).
        //
        // Minimum = pixel width of 20% of the container. Any segment whose proportional
        // share is smaller than this minimum is clamped to the minimum so it remains
        // clearly visible and has room for its label. Large segments keep their
        // proportional share. If ΣwidthS > root.width → Flickable scrolls (bounded
        // excess, not the 100× inflation the old maxRatio approach caused).
        var minSegW = Math.round(0.20 * root.width)
        var widths = []
        var total  = 0
        for (var j = 0; j < n; j++) {
            var p = normalizedPercentages[j]
            var propW = p > 0 ? Math.round(p / 100.0 * root.width) : 0
            var w = Math.max(propW, minSegW)
            widths.push(w)
            total += w
        }

        _segWidths = widths
        _totalW    = total

        // Step 3: compute _barWidths — the actual pixel width of each segment.
        // contentWidth = max(container width, _totalW).  All three rows (label,
        // bar, date) must use _barWidths so labels stay aligned with their bars.
        //
        // Proof that text always fits:
        //   cw >= _totalW = Σ widths[j] >= Σ (pct[j] * maxRatio)
        //   → barWidth[i] = pct[i]/100 * cw >= pct[i]/100 * maxRatio * 100
        //                  = pct[i] * maxRatio >= textMins[i]  ✓
        // Step 3: _barWidths — scale from widths[] (which already enforces minSegW),
        // NOT from raw percentages. Scaling from percentages would bypass the minimum
        // and cause small segments to be squished again.
        var cw = Math.max(root.width, total)
        var bws  = []
        var used = 0
        for (var bi = 0; bi < n; bi++) {
            var bw
            if (bi < n - 1) {
                bw = Math.round(widths[bi] / total * cw)
                used += bw
            } else {
                // Last segment fills remainder to eliminate rounding gaps
                bw = Math.max(0, cw - used)
            }
            bws.push(bw)
        }
        _barWidths = bws
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
                        // Same width as the corresponding bar segment → labels stay aligned
                        width:  root._barWidths.length > index ? root._barWidths[index] : 0
                        height: 16
                        clip:   true   // guard against extremely narrow edge-case bars

                        Text {
                            width: parent.width
                            anchors { left: parent.left; top: parent.top; leftMargin: 0 }
                            text:           modelData.label
                                            + (root.displayPercentage
                                               ? " (" + (root._normalizedPercentages.length > index
                                                          ? Math.round(root._normalizedPercentages[index])
                                                          : modelData.percentage) + "%" + ")"
                                               : "")
                            font.pixelSize: 10
                            font.family:    "Lato"
                            color:          "#031F2B"
                        }
                    }
                }
            }

            // Row 2 – Color bars
            // Bar widths are purely percentage-proportional to contentWidth,
            // using normalized percentages if total > 100.
            Row {
                id: barRow
                width:  parent.width
                height: 7
                spacing: 0

                Repeater {
                    model: root.timelineStages
                    Rectangle {
                        // Use _barWidths — same source as label/date rows — to guarantee alignment
                        width:  root._barWidths.length > index ? root._barWidths[index] : 0
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
                        width:  root._barWidths.length > index ? root._barWidths[index] : 0
                        height: 16
                        clip:   true

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

