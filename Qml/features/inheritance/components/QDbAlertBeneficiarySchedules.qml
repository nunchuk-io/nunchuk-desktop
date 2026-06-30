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
import QtQuick.Controls 2.3
import QtGraphicalEffects 1.12
import "../../../Components/customizes/Texts"
import "../../../Components/origins"

Column {
    width: parent.width
    spacing: 12
            
    property var beneficiaries: []

    QLato {
        width: parent.width
        height: paintedHeight
        text: STR.STR_QML_2154
        font.weight: Font.Bold
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
    }
    Rectangle {
        id: root
        width: parent.width
        height: contentColumn.childrenRect.height + 24
        color: "#F5F5F5"
        radius: 12
        
        Column {
            id: contentColumn
            anchors {
                fill: parent
                leftMargin: 12
                topMargin: 12
                rightMargin: 0
                bottomMargin: 12
            }
            spacing: 8
                        
            Repeater {
                model: beneficiaries
                Column {
                    width: parent.width - 24
                    spacing: 8
                    
                    // Email
                    QLato {
                        height: 20
                        text: modelData.email
                        font.pixelSize: 16
                        font.weight: Font.Bold
                    }
                    
                    // First withdrawal date
                    QFirstWithdrawalDateRow {
                        height: 16
                        firstWithdrawalDate: modelData.firstWithdrawalDate
                        firstWithdrawalDateChanged: modelData.firstWithdrawalDateChanged
                    }

                    // Buffer period (if exists)
                    QBufferPeriodRow {
                        height: 16
                        buffer_period: modelData.buffer_period
                        buffer_period_changed: modelData.buffer_periodChanged
                        buffer_apply_on: modelData.buffer_apply_on
                    }
                    
                    // Timeline visualization
                    QTimelineVisualization {
                        timelineStages: modelData.stages
                    }
                }
            }
        }
    }
}
