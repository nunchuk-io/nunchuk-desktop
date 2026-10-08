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

Rectangle {
    id: root
    width: parent.width
    height: 401
    color: "#FFFFFF"
    radius: 12
    
    // Signal
    signal editClicked()
    // Sample beneficiary data
    property var beneficiaries: [
        {
            email: "wife@gmail.com",
            firstWithdrawalDate: "05/29/2028",
            buffer_apply_on: "FIRST_WITHDRAWAL",
            buffer_period: {id: "90_days", display_name: "90 days"},
            stages: [
                {label: "Stage 1", date: "05/29/2028", color: "#4CAF50", percentage: 25},
                {label: "Stage 2", date: "05/29/2032", color: "#2196F3", percentage: 35},
                {label: "Stage 3", date: "05/29/2046", color: "#FF9800", percentage: 40}
            ]
        },
        {
            email: "son@gmail.com",
            firstWithdrawalDate: "05/29/2030",
            buffer_apply_on: "EVERY_WITHDRAWAL",
            buffer_period: {id: "90_days", display_name: "90 days"},
            stages: [
                {label: "Stage 1", date: "05/29/2030", color: "#4CAF50", percentage: 50},
                {label: "Stage 2", date: "05/29/2032", color: "#2196F3", percentage: 50}
            ]
        },
        {
            email: "daughter@gmail.com",
            firstWithdrawalDate: "05/29/2030",
            buffer_apply_on: "",
            buffer_period: {id: "", display_name: ""},
            stages: [
                {label: "Stage 1", date: "05/29/2030", color: "#4CAF50", percentage: 100}
            ]
        }
    ]
    
    Column {
        anchors {
            fill: parent
            leftMargin: 12
            topMargin: 12
            rightMargin: 0
            bottomMargin: 12
        }
        spacing: 8
        
        // Header with title and Edit button
        Item {
            width: parent.width
            height: 20
            
            QLato {
                id: titleText
                anchors {
                    left: parent.left
                    verticalCenter: parent.verticalCenter
                }
                text: STR.STR_QML_2154
                font.pixelSize: 16
                font.weight: Font.Bold
            }
            
            QTextLink {
                width: 29
                height: 20
                anchors {
                    right: parent.right
                    rightMargin: 12
                    verticalCenter: parent.verticalCenter
                }
                text: STR.STR_QML_849
                color: "#031F2B"
                onTextClicked: {
                    root.editClicked()
                }
            }
        }
        
        // Beneficiaries list
        Flickable {
            width: parent.width
            height: 349
            contentHeight: contentDisp.height
            clip: true
            ScrollBar.vertical: ScrollBar { active: true }
            Column {
                id: contentDisp
                width: parent.width
                anchors.horizontalCenter: parent.horizontalCenter
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
                        }
                        
                        // Buffer period (if exists)
                        QBufferPeriodRow {
                            height: 16
                            buffer_period: modelData.buffer_period
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
}
