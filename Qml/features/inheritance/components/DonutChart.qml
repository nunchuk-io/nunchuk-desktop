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
import QtGraphicalEffects 1.0

Item {
    id: root
    width: 200
    height: 200

    // Property: list of {color: "#RRGGBB", asset_percentage: 0-100}
    property var chartData: [
        {color: "#2C3E50", asset_percentage: 40},
        {color: "#F4D03F", asset_percentage: 35},
        {color: "#F5A17D", asset_percentage: 25}
    ]

    // Customizable properties
    property real outerRadius: Math.min(width, height) / 2
    property real innerRadius: outerRadius * 0.4
    property real startAngle: -90

    Canvas {
        id: canvas
        anchors.fill: parent
        onPaint: {
            var ctx = getContext("2d");
            ctx.clearRect(0, 0, width, height);

            var centerX = width / 2;
            var centerY = height / 2;
            var currentAngle = startAngle;

            // Calculate total asset_percentage
            var totalPercent = 0;
            for (var i = 0; i < chartData.length; i++) {
                totalPercent += chartData[i].asset_percentage;
            }

            // Draw each segment
            for (var i = 0; i < chartData.length; i++) {
                var segment = chartData[i];
                var sliceAngle = (segment.asset_percentage / totalPercent) * 360;
                var endAngle = currentAngle + sliceAngle;

                // Convert to radians
                var startRad = currentAngle * Math.PI / 180;
                var endRad = endAngle * Math.PI / 180;

                ctx.fillStyle = segment.color;
                ctx.beginPath();

                // Outer arc
                ctx.arc(centerX, centerY, outerRadius, startRad, endRad, false);

                // Line to inner arc
                ctx.lineTo(
                    centerX + innerRadius * Math.cos(endRad),
                    centerY + innerRadius * Math.sin(endRad)
                );

                // Inner arc (backwards)
                ctx.arc(centerX, centerY, innerRadius, endRad, startRad, true);

                ctx.closePath();
                ctx.fill();

                currentAngle = endAngle;
            }
        }
    }

    // Redraw when data changes
    onChartDataChanged: canvas.requestPaint()
    Component.onCompleted: canvas.requestPaint()
}
