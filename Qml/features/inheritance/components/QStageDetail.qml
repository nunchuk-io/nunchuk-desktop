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
import "../../../Components/customizes/Buttons"
import "../../../Components/customizes/Texts"
import "../../../Components/origins"

Item {
    id: root
    width: parent.width
    height: isExpanded ? (128 + minLength * 20) : 98 + 12
    property bool hasEdit: true
    property int minLength: Math.min(installments.length, 15)
    // Properties
    property string stageName: "Stage 1"
    property string stagePercentage: "20"
    property string stageColor: "#4CAF50"
    property string firstWithdrawalDate: "05/29/2028"
    property string releaseInfo: "5% annually (4 installments)"
    property bool isExpanded: false
    property bool isFirst: false
    property bool isLast: false
    property var installments: [] // List of installment objects with number, percentage, date
    property int current_installment_index: -1
    property bool isLastEnabled: true

    // Signals
    signal editClicked()
    signal expandClicked()
    // Left colored border
    Item {
        width: 6
        height: parent.height
        anchors {
            left: parent.left
            leftMargin: 12
        }
    
        Rectangle {
            width: 2
            height: 12 + 10
            color: "#DEDEDE"
            radius: 8
            opacity: root.isFirst ? 0.0 : 1.0
        }
        Rectangle {
            width: 8
            height: 8
            color: "#DEDEDE"
            radius: 8
            anchors {
                top: parent.top
                topMargin: 12 + 10 - 3
                left: parent.left
                leftMargin: -3
            }
        }
        Rectangle {
            width: 2
            height: root.height - (12 + 10)
            color: "#DEDEDE"
            radius: 2
            anchors {
                bottom: parent.bottom
                bottomMargin: 0
            }
            opacity: root.isLast ? 0.0 : 1.0
        }
    }
    
    Column {
        width: parent.width
        anchors {
            fill: parent
            leftMargin: 24
            rightMargin: 12
            topMargin: 12
            bottomMargin: 12
        }
        spacing: 16

        // Header with stage info and edit button
        Item {
            id: header
            width: parent.width
            height: 20
            // Stage name and percentage
            QLato {
                anchors {
                    left: parent.left
                    leftMargin: 0
                    verticalCenter: parent.verticalCenter
                }
                text: stageName + " • " + stagePercentage + "%"
                font.pixelSize: 16
                font.weight: Font.Bold
            }
            QIconButton {
                width: 16
                height: 16
                anchors {
                    verticalCenter: parent.verticalCenter
                    right: parent.right
                    rightMargin: 0
                }
                visible: root.hasEdit
                icon: "qrc:/Images/Images/edit-dark.svg"
                bgColor: "transparent"
                onClicked: {
                    root.editClicked()
                }
            }
        }

        // Content area
        Column {
            width: parent.width
            spacing: 12
            clip: true

            // First withdrawal date
            QFirstWithdrawalDateRow {
                width: parent.width
                height: 16
                firstWithdrawalDate: root.firstWithdrawalDate
            }
            // Release info with expand button
            QReleaseInfoRow {
                id: releaseRow
                width: parent.width
                releaseInfo: root.releaseInfo
                isExpanded: root.isExpanded
                installmentDetails: root.installments
                current_installment_index: isLastEnabled ? root.current_installment_index : -1
                onExpandClicked: {
                    root.isExpanded = !root.isExpanded
                    root.expandClicked()
                }
            }
        }
    }
}
