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

Rectangle {
    id: root
    width: parent.width
    height: innerColumn.implicitHeight + 24
    color: "#FFFFFF"
    radius: 12

    // Property: list of {label: "Name", email: "email@example.com", color: "#RRGGBB", percent: 0-100}
    property var allocationData: [
        {email: "wife@gmail.com", color: "#2C3E50", asset_percentage: 50},
        {email: "son@gmail.com", color: "#F5A17D", asset_percentage: 25},
        {email: "daughter@gmail.com", color: "#F4D03F", asset_percentage: 25}
    ]
    property string releaseMethod: "SHARED" // or "INDIVIDUAL"

    // Signals
    signal assetAllocationEditClicked()
    signal releaseMehodEditClicked()

    Column {
        id: innerColumn
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            margins: 12
        }
        spacing: 12

        // Title and Edit button
        Item {
            width: parent.width
            height: 20

            QLato {
                id: titleText
                anchors.verticalCenter: parent.verticalCenter
                text: STR.STR_QML_2166
                font.pixelSize: 16
                font.weight: Font.Bold
            }

            QTextLink {
                width: 29
                height: 20
                anchors.right: parent.right
                text: STR.STR_QML_849
                color: "#031F2B"
                anchors.verticalCenter: parent.verticalCenter
                onTextClicked: {
                    root.assetAllocationEditClicked()
                }
            }
        }

        // Content Row: Chart + Legend
        Row {
            width: parent.width
            height: Math.max(60, legendColumn.implicitHeight)
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
                id: legendColumn
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
                                font.pixelSize: 12
                                color: "#757575"
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignLeft
                            }

                            QLato {
                                text: modelData.asset_percentage + "%"
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
        Item {
            width: parent.width
            height: 42
            Column {
                anchors.fill: parent
                spacing: 2
                QLato {
                    id: releaseTitle
                    text: STR.STR_QML_2153
                    font.pixelSize: 16
                    font.weight: Font.Bold
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                }

                QLato {
                    text: releaseMethod === "INDIVIDUAL" ? STR.STR_QML_2167 : STR.STR_QML_2182
                    font.pixelSize: 16
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                }
            }            

            QTextLink {
                width: 29
                height: 20
                anchors.right: parent.right
                text: STR.STR_QML_849
                color: "#031F2B"
                onTextClicked: {
                    root.releaseMehodEditClicked()
                }
            }
        }
    }
}
