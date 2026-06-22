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
    height: isExpanded ? (row.height + rectangle.height + spacing) : row.height
    spacing: 8

    // Properties
    property string releaseInfo: "5% annually (4 installments)"
    property bool isExpanded: false
    property var installmentDetails: [
        { number: "1st", percentage: 20, date: "05/29/2028" },
        { number: "2nd", percentage: 40, date: "05/29/2029" },
        { number: "3rd", percentage: 60, date: "05/29/2030" },
        { number: "4th", percentage: 80, date: "05/29/2031" },
        { number: "5th", percentage: 100, date: "05/29/2032" }
    ]
    property int current_installment_index: -1

    // Signals
    signal expandClicked()

    // Release info with expand button
    Item {
        width: parent.width
        height: 16
        Row {
            id: row
            height: 16
            spacing: 12

            QIcon {
                anchors.verticalCenter: parent.verticalCenter
                source: "qrc:/Images/Images/installments-dark.svg"
                width: 16
                height: 16
            }        

            Row {
                height: 16
                spacing: 2

                QLato {
                    text: STR.STR_QML_2174
                    font.pixelSize: 12
                    anchors.verticalCenter: parent.verticalCenter
                }

                QLato {
                    text: root.releaseInfo
                    font.pixelSize: 12
                    anchors.verticalCenter: parent.verticalCenter
                }
            }

            
        }
        // Expand button
        MouseArea {
            id: expandButton
            width: 80
            height: 30
            anchors {
                verticalCenter: parent.verticalCenter
                right: parent.right
                rightMargin: -8
            }
            cursorShape: Qt.PointingHandCursor
            Row {
                anchors.centerIn: parent
                height: parent.height
                spacing: 4
                QLato {
                    anchors.verticalCenter: parent.verticalCenter
                    text: root.isExpanded ? STR.STR_QML_2176 : STR.STR_QML_2175
                    font.pixelSize: 12
                    font.underline: true
                    font.weight: Font.Bold
                }
                QLato {
                    anchors.bottom: expandButton.bottom
                    anchors.bottomMargin: -8
                    text: root.isExpanded ? "˄" : "˅"
                    font.pixelSize: 32
                    verticalAlignment: Text.AlignBottom
                }
            }
            onClicked: {
                root.expandClicked()
            }
        }
    }
    property int minLength: Math.min(installmentDetails.length, 10)
    // Expanded content placeholder
    Rectangle {
        id: rectangle
        width: parent.width
        height: root.isExpanded ? (minLength * 20 + 16): 0
        color: "#FFFFFF"
        radius: 4
        visible: root.isExpanded
        Rectangle {
            anchors.left: parent.left
            anchors.leftMargin: 22
            width: 1
            height: parent.height - 16
            color: "#DEDEDE"
        }
        Flickable {
            anchors.left: parent.left
            anchors.leftMargin: 22 + 12
            width: parent.width
            height: parent.height - 16
            contentWidth: parent.width
            contentHeight: root.installmentDetails.length * 20 + 16
            clip: true
            flickableDirection: Flickable.VerticalFlick
            ScrollBar.vertical: ScrollBar { active: true }
            Column {
                anchors.fill: parent
                spacing: 8
                Repeater {
                    model: root.installmentDetails                    
                    QLato {
                        width: parent.width
                        enabled: index <= root.current_installment_index || root.current_installment_index === -1
                        opacity: enabled ? 1.0 : 0.4
                        text: modelData.number + ": " + modelData.percentage + "% available by " + modelData.date
                        font.pixelSize: 12
                    }
                }
            }
        }

        Behavior on height {
            NumberAnimation { duration: 200; easing.type: Easing.InOutQuad }
        }
    }
}
