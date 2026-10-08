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
import "../../../Components/origins"

Column {
    id: root
    width: parent.width
    spacing: 12

    // Properties
    property string firstWithdrawalDate: "05/29/2028"
    property bool firstWithdrawalDateChanged: false
    property var buffer_period: ""
    property bool buffer_period_changed: false

    property string buffer_apply_on: "FIRST_WITHDRAWAL"
    property bool buffer_apply_on_changed: false
    property bool isSingle: true
    
    // Timeline stages: list of {label: "Stage X", date: "MM/DD/YYYY", color: "#RRGGBB"}
    property var timelineStages: [
        {label: "Stage 1", date: "05/29/2028", color: "#4CAF50"},
        {label: "Stage 2", date: "05/29/2032", color: "#7B3FF2"},
        {label: "Stage 3", date: "05/29/2046", color: "#FFD700"}
    ]

    QLato {
        width: parent.width
        height: paintedHeight
        text: STR.STR_QML_2151
        font.weight: Font.Bold
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
    }

    Rectangle {        
        width: parent.width
        height: 99
        color: "#F5F5F5"
        radius: 12

        Column {
            anchors {
                fill: parent
                margins: 12
            }
            spacing: 12
            Item {
                width: parent.width
                height: 67
                Column {
                    anchors.fill: parent
                    spacing: 8
                    QFirstWithdrawalDateRow {
                        visible: !root.isSingle
                        firstWithdrawalDate: root.firstWithdrawalDate
                        firstWithdrawalDateChanged: root.firstWithdrawalDateChanged
                    }

                    // Buffer period
                    QBufferPeriodRow {
                        buffer_period: root.buffer_period
                        buffer_period_changed: root.buffer_period_changed
                        buffer_apply_on: root.buffer_apply_on
                    }

                    // Timeline visualization
                    QTimelineVisualization {
                        timelineStages: root.timelineStages
                    }
                }
            }
        }
    }
}