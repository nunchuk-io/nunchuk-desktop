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
import QtQuick.Layouts
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Chats"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

Popup {
    id: _calendar

    property string dateString: ""
    property var minimumDate
    property alias selectedDate: inputCalendar.selectedDate

    modal: true
    focus: true
    closePolicy: Popup.CloseOnReleaseOutside | Popup.CloseOnEscape
    background: Item {}

    x: parent ? Math.round((parent.width - width) / 2) : 0
    y: parent ? Math.round((parent.height - height) / 2) : 0

    width: inputCalendar.implicitWidth
    height: inputCalendar.implicitHeight

    contentItem: Rectangle {
        id: inputCalendar

        property date selectedDate: new Date()
        property date displayedDate: selectedDate

        readonly property int cellSize: 40
        readonly property int navBarHeight: 48
        readonly property int weekHeaderHeight: 40

        color: "#F9F9F9"
        implicitWidth: 7 * cellSize
        implicitHeight: navBarHeight + weekHeaderHeight + (6 * cellSize)

        onSelectedDateChanged: displayedDate = selectedDate

        function isSameDay(a, b) {
            return a
                    && b
                    && a.getFullYear() === b.getFullYear()
                    && a.getMonth() === b.getMonth()
                    && a.getDate() === b.getDate()
        }

        function hasValidMinimumDate() {
            return minimumDate && !isNaN(minimumDate.getTime())
        }

        function isDateEnabled(d) {
            return !hasValidMinimumDate() || d >= minimumDate
        }

        function showPreviousMonth() {
            var d = new Date(displayedDate)
            d.setMonth(d.getMonth() - 1)
            displayedDate = d
        }

        function showNextMonth() {
            var d = new Date(displayedDate)
            d.setMonth(d.getMonth() + 1)
            displayedDate = d
        }

        ColumnLayout {
            anchors.fill: parent
            spacing: 0

            Rectangle {
                Layout.fillWidth: true
                height: inputCalendar.navBarHeight
                color: "#F9F9F9"

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 1
                    spacing: 0

                    Button {
                        Layout.fillHeight: true
                        Layout.preferredWidth: 48
                        text: ""
                        icon.source: "qrc:/Images/Images/chevron-left-dark.svg"
                        background: Item {}
                        onClicked: inputCalendar.showPreviousMonth()
                    }

                    Text {
                        Layout.fillWidth: true
                        height: 40
                        text: monthGrid.title
                        font.pixelSize: 20
                        font.weight: Font.Normal
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }

                    Button {
                        Layout.fillHeight: true
                        Layout.preferredWidth: 48
                        text: ""
                        icon.source: "qrc:/Images/Images/chevron-right-dark.svg"
                        background: Item {}
                        onClicked: inputCalendar.showNextMonth()
                    }
                }
            }

            DayOfWeekRow {
                Layout.fillWidth: true
                height: inputCalendar.weekHeaderHeight
                locale: Qt.locale()

                delegate: Text {
                    required property string shortName
                    text: shortName
                    height: 40
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 14
                }
            }

            MonthGrid {
                id: monthGrid
                Layout.fillWidth: true
                Layout.fillHeight: true

                month: inputCalendar.displayedDate.getMonth()
                year: inputCalendar.displayedDate.getFullYear()
                locale: Qt.locale()

                onClicked: function(date) {
                    if (!inputCalendar.isDateEnabled(date)) {
                        return
                    }

                    inputCalendar.selectedDate = date
                    dateString = Qt.formatDateTime(date, "MM/dd/yyyy")
                    _calendar.close()
                }

                delegate: Rectangle {
                    required property var model

                    implicitWidth: inputCalendar.cellSize
                    implicitHeight: inputCalendar.cellSize

                    property bool visibleMonth: model.month === monthGrid.month
                    property bool valid: inputCalendar.isDateEnabled(model.date)
                    property bool selected: inputCalendar.isSameDay(model.date, inputCalendar.selectedDate)

                    gradient: Gradient {
                        GradientStop {
                            position: 0.00
                            color: selected ? "#111" : (visibleMonth && valid ? "#444" : "#666")
                        }
                        GradientStop {
                            position: 1.00
                            color: selected ? "#444" : (visibleMonth && valid ? "#111" : "#666")
                        }
                        GradientStop {
                            position: 1.00
                            color: selected ? "#777" : (visibleMonth && valid ? "#111" : "#666")
                        }
                    }

                    Text {
                        text: model.day
                        anchors.centerIn: parent
                        color: valid ? "white" : "grey"
                    }

                    Rectangle {
                        width: parent.width
                        height: 1
                        color: "#555"
                        anchors.bottom: parent.bottom
                    }

                    Rectangle {
                        width: 1
                        height: parent.height
                        color: "#555"
                        anchors.right: parent.right
                    }
                }
            }
        }

        Component.onCompleted: displayedDate = selectedDate
    }

    onVisibleChanged: if (visible) inputCalendar.displayedDate = inputCalendar.selectedDate
}
