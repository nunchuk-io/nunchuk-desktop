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
import NUNCHUCKTYPE 1.0
import Features.Claiming.ViewModels 1.0
import "./../../../origins"
import "./../../../customizes"
import "./../../../customizes/Chats"
import "./../../../customizes/Texts"
import "./../../../customizes/Buttons"

Item {
    // Properties
    property var timelineStages: [
        {label: "Stage 1", percentage: 20},
        {label: "Stage 2", percentage: 60},
        {label: "Stage 3", percentage: 20}
    ]
    property bool displayPercentage: false
    Column {
        anchors.fill: parent
        spacing: 24
        anchors{
            left: parent.left
            leftMargin: 24
            top: parent.top
            topMargin: 24
        }
        Rectangle {
            width: parent.width - 24
            height: parent.height * 0.42
            radius: 24
            color: "#D0E2FF"
            Column {
                anchors.centerIn: parent
                width: 211
                height: 128
                spacing: 4
                // Stage labels
                Row {
                    width: parent.width
                    height: 16
                    spacing: 0

                    Repeater {
                        model: timelineStages

                        Item {
                            width: parent.width / timelineStages.length
                            height: 16

                            QLato {
                                anchors {
                                    left: parent.left
                                    top: parent.top
                                }
                                text: modelData.label + (displayPercentage ? " (" + modelData.percentage + "%" + ")" : "")
                                font.pixelSize: 10
                                font.weight: Font.Black
                                color: "#031F2B"
                            }
                        }
                    }
                }

                QImage {
                    source: "qrc:/Images/Images/StageTimeline.svg"
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                QImage {
                    source: "qrc:/Images/Images/BitCoinStageFlow.svg"
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }
        }
        Item {
            width: parent.width - 24
            height: parent.height * 0.58 - 24
            Column {
                width: parent.width
                spacing: 16
                QLato {
                    width: 700
                    text: STR.STR_QML_2237
                    font.pixelSize: 32
                    font.weight: Font.Medium
                    wrapMode: Text.WordWrap
                }
                QLato {
                    width: 700
                    text: STR.STR_QML_2238
                    font.pixelSize: 16
                    font.weight: Font.Normal
                    wrapMode: Text.WordWrap
                }
                QLato {
                    width: 700
                    text: STR.STR_QML_2239
                    font.pixelSize: 16
                    font.weight: Font.Normal
                    wrapMode: Text.WordWrap
                    lineHeight: 28
                    lineHeightMode: Text.FixedHeight
                }
                QLato {
                    width: 700
                    text: STR.STR_QML_2240
                    font.pixelSize: 16
                    font.weight: Font.Normal
                    wrapMode: Text.WordWrap
                }
            }
            Row {
                spacing: 12
                anchors {
                    right: parent.right
                    bottom: parent.bottom
                    bottomMargin: 16
                }
                QIconTextButton {
                    width: 98
                    height: 48
                    label: STR.STR_QML_059
                    icons: ["left-arrow-dark.svg", "left-arrow-dark.svg", "left-arrow-dark.svg","left-arrow-dark.svg"]
                    fontPixelSize: 16
                    iconSize: 24
                    type: eTypeJ
                    onButtonClicked: {
                        vm.back()
                    }
                }
                QTextButton {
                    width: label.paintedWidth + 32
                    height: 48
                    label.text: STR.STR_QML_097
                    label.font.pixelSize: 16
                    type: eTypeE
                    onButtonClicked: {
                        vm.onContinueClicked()
                    }
                }
            }
        }
    }
    PhaseRolloutGuidelineViewModel {
        id: vm
    }
}
