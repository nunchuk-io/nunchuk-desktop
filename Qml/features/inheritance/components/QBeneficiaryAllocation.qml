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
    width: parent.width
    height: 220

    // Properties
    property int beneficiaryIndex: 1
    property string beneficiaryEmail: "wife@gmail.com"
    property real allocationPercentage: 50
    property real minPercentage: 0
    property real maxPercentage: 100

    // Signals
    signal deleteClicked()
    signal percentageChanged(real newPercentage)

    Column {
        width: parent.width
        height: parent.height
        spacing: 12

        // Header with title and delete button
        Item {
            width: parent.width
            height: 32

            QLato {
                anchors {
                    left: parent.left
                    verticalCenter: parent.verticalCenter
                }
                text: STR.STR_QML_2177 + " " + beneficiaryIndex
                font.pixelSize: 16
                font.weight: Font.Bold
                color: "#031F2B"
            }

            MouseArea {
                id: deleteButton
                anchors {
                    right: parent.right
                    verticalCenter: parent.verticalCenter
                }
                width: 40
                height: 40
                cursorShape: Qt.PointingHandCursor

                Rectangle {
                    anchors.fill: parent
                    radius: 4
                    color: "#E8E8E8"

                    Text {
                        anchors.centerIn: parent
                        text: "🗑️"
                        font.pixelSize: 20
                    }
                }

                onClicked: root.deleteClicked()
            }
        }

        // Email display box
        Rectangle {
            width: parent.width
            height: 50
            color: "#F9F9F9"
            border.color: "#E0E0E0"
            border.width: 1
            radius: 6

            QLato {
                anchors {
                    fill: parent
                    margins: 12
                }
                text: beneficiaryEmail
                font.pixelSize: 14
                color: "#031F2B"
                verticalAlignment: Text.AlignVCenter
            }
        }

        // Slider with percentage display
        Row {
            width: parent.width
            height: 60
            spacing: 12
            verticalAlignment: Qt.AlignVCenter

            // Slider
            Slider {
                id: percentageSlider
                width: parent.width - 60
                height: parent.height
                from: minPercentage
                to: maxPercentage
                value: allocationPercentage
                stepSize: 1
                orientation: Qt.Horizontal

                background: Rectangle {
                    x: percentageSlider.leftPadding
                    y: percentageSlider.topPadding + percentageSlider.availableHeight / 2 - height / 2
                    implicitWidth: 200
                    implicitHeight: 8
                    width: percentageSlider.availableWidth
                    height: implicitHeight
                    radius: 4
                    color: "#D0D0D0"

                    Rectangle {
                        width: percentageSlider.visualPosition * parent.width
                        height: parent.height
                        color: "#4A4A4A"
                        radius: 4
                    }
                }

                handle: Rectangle {
                    x: percentageSlider.leftPadding + percentageSlider.visualPosition * (percentageSlider.availableWidth - width)
                    y: percentageSlider.topPadding + percentageSlider.availableHeight / 2 - height / 2
                    implicitWidth: 24
                    implicitHeight: 24
                    radius: 12
                    color: "#031F2B"
                    border.color: "#031F2B"
                    border.width: 1
                }

                onMoved: {
                    allocationPercentage = value
                    root.percentageChanged(value)
                }
            }

            // Percentage display
            Rectangle {
                width: 50
                height: 40
                color: "#F9F9F9"
                border.color: "#E0E0E0"
                border.width: 1
                radius: 6

                QLato {
                    anchors.centerIn: parent
                    text: Math.round(allocationPercentage) + "%"
                    font.pixelSize: 14
                    font.bold: true
                    color: "#031F2B"
                }
            }
        }
    }
}
