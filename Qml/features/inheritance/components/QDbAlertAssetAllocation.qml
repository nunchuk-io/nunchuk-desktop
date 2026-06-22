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
import QtGraphicalEffects 1.0
import "../../../Components/customizes/Texts"

Column {
    width: parent.width
    spacing: 12
    
    // Property: list of {label: "Name", email: "email@example.com", color: "#RRGGBB", percent: 0-100}
    property var allocationData: [
        {email: "wife@gmail.com", color: "#2C3E50", asset_percentage: 50},
        {email: "son@gmail.com", color: "#F5A17D", asset_percentage: 25},
        {email: "daughter@gmail.com", color: "#F4D03F", asset_percentage: 25}
    ]
    property string releaseMethod: "SHARED" // or "INDIVIDUAL"
    property bool isReleaseMothodChanged: false
    QLato {
        width: parent.width
        height: paintedHeight
        text: STR.STR_QML_2224
        font.weight: Font.Bold
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
    }
    Rectangle {
        width: parent.width
        height: innerCol.childrenRect.height + 24
        color: "#F5F5F5"
        radius: 12
        Column {
            id: innerCol
            anchors {
                left: parent.left
                right: parent.right
                top: parent.top
                margins: 12
            }
            spacing: 12
            // Title
            QLato {
                width: parent.width
                height: paintedHeight
                text: STR.STR_QML_2166
                font.pixelSize: 16
                font.weight: Font.Bold
            }
            // Content Row: Chart + Legend
            Row {
                width: parent.width
                height: Math.max(donutChart.height, legendCol.height)
                spacing: 24

                // Donut Chart
                DonutChart {
                    id: donutChart
                    width: 60
                    height: 60
                    chartData: allocationData
                }

                // Legend
                Column {
                    id: legendCol
                    spacing: 4
                    width: parent.width - donutChart.width - 24

                    Repeater {
                        model: allocationData
                        Row {
                            spacing: 4
                            width: parent.width
                            height: 16

                            // Color box
                            Rectangle {
                                width: 12
                                height: 12
                                color: modelData.color
                                radius: 2
                                anchors.verticalCenter: parent.verticalCenter
                            }

                            // Label and percentage
                            Row {
                                spacing: 2

                                QLato {
                                    text: modelData.email
                                    color: modelData.emailChanged ? "#CF4018" : "#031F2B"
                                    font.pixelSize: 12
                                    verticalAlignment: Text.AlignVCenter
                                    horizontalAlignment: Text.AlignLeft
                                }

                                QLato {
                                    text: modelData.asset_percentage + "%"
                                    color: modelData.asset_percentageChanged ? "#CF4018" : "#031F2B"
                                    font.pixelSize: 12
                                    verticalAlignment: Text.AlignVCenter
                                    horizontalAlignment: Text.AlignLeft
                                }
                            }
                        }
                    }
                }
            }

            // Divider
            Rectangle {
                width: parent.width
                height: 1
                color: "#DEDEDE"
            }

            // Release method section
            Column {
                width: parent.width
                spacing: 2
                QLato {
                    text: STR.STR_QML_2153
                    font.pixelSize: 16
                    font.weight: Font.Bold
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                }
                QLato {
                    text: releaseMethod === "INDIVIDUAL" ? STR.STR_QML_2167 : STR.STR_QML_2182
                    color: isReleaseMothodChanged ? "#CF4018" : "#031F2B"
                    font.pixelSize: 16
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                }
            }
        }
    }
}
