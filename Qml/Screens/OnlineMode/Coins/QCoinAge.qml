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
import Qt.labs.platform 1.1
import HMIEVENTS 1.0
import NUNCHUCKTYPE 1.0
import QRCodeItem 1.0
import DataPool 1.0
import DRACO_CODE 1.0
import EWARNING 1.0
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

Rectangle {
    width: 810
    height: childrenRect.height
    radius: 12
    border.width: 1
    border.color: "#FFFFFF"
    color: "#F5F5F5"

    property var    coin_timelocklist
    property var    coin_timelockbase
    property bool   coin_timelocked: false

    Column {
        id: timelineItem
        width: parent.width - 16
        spacing: 4
        anchors.horizontalCenter: parent.horizontalCenter
        Item {width: parent.width; height: 1}
        Row {
            spacing: 4
            QIcon {
                iconSize: 16
                source: "qrc:/Images/Images/Timer.svg"
            }
            QLato {
                text: STR.STR_QML_1879
                font.pixelSize: 12
                verticalAlignment: Text.AlignVCenter
                wrapMode: Text.WordWrap
                anchors.verticalCenter: parent.verticalCenter
                font.weight: Font.Bold
            }
        }

        Item {
            width: parent.width
            height: 46
            Flickable {
                id: flickerWalletList
                anchors.fill: parent
                clip: true
                flickableDirection: Flickable.HorizontalFlick
                interactive: contentWidth > width // TBD, need display button next and previous (not implemented yet)
                contentWidth: contentDisplay.width
                ScrollBar.horizontal : ScrollBar { active: true }
                Row {
                    id: contentDisplay
                    height: 44
                    Item {
                        id: nodeStart
                        width: 8
                        height: 30
                        anchors.bottom: parent.bottom
                        Rectangle {
                            width: parent.width
                            height: width
                            radius: width
                            color: "#FFFFFF"
                            border.width: 1
                            border.color: "#DEDEDE"
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }

                    Repeater {
                        id: repeaterNode
                        model: coin_timelocklist
                        Row {
                            height: 44
                            spacing: 0
                            Item {
                                id: partLine
                                width: (timelineItem.width - nodeStart.width - nodeEnd.width - nodeLock.width*repeaterNode.count )/2
                                height: 30
                                anchors.bottom: parent.bottom
                                Rectangle {
                                    width: parent.width
                                    height: 4
                                    color: (modelData.valueLocked) ? "#031F2B" : "#DEDEDE"
                                    anchors.verticalCenter: parent.verticalCenter
                                }
                                Rectangle {
                                    width: partText.implicitWidth + 16
                                    height: 16
                                    border.color: "#031F2B"
                                    color: "#FFFFFF"
                                    radius: 20
                                    anchors.centerIn: parent
                                    QLato {
                                        id: partText
                                        text: modelData.valueRemaining
                                        font.pixelSize: 10
                                        color: "#031F2B"
                                        font.weight: Font.Bold
                                        anchors.centerIn: parent
                                    }
                                    visible: (modelData.valueRemaining !== "")
                                }
                            }
                            Item {
                                id: nodeLock
                                width: 20
                                height: 30
                                anchors.bottom: parent.bottom
                                Loader {
                                    width: parent.width
                                    anchors.verticalCenter: parent.verticalCenter
                                    sourceComponent: (modelData.valueNode === "" ? nodeEmpty : nodeReal)
                                }
                                QLato {
                                    text: modelData.valueNode
                                    font.pixelSize: 12
                                    color: "#031F2B"
                                    font.weight: Font.Bold
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    anchors.bottom: parent.top
                                    visible: (modelData.valueNode !== "")
                                }
                            }
                        }
                    }

                    Item {
                        id: nodeEnd
                        width: 24
                        height: 30
                        anchors.bottom: parent.bottom
                        Rectangle {
                            width: parent.width/2
                            height: 4
                            color: walletInfo.timelockType === ScriptNodeHelper.TimelockType.LOCKTYPE_ABSOLUTE ? "#DEDEDE" : (coin_timelocked && repeaterNode.count > 2) ? "#031F2B" : "#DEDEDE"
                            anchors.verticalCenter: parent.verticalCenter
                        }
                        QIcon {
                            iconSize: 24
                            source: walletInfo.timelockType === ScriptNodeHelper.TimelockType.LOCKTYPE_ABSOLUTE ? "qrc:/Images/Images/CaretRightGray.svg" :
                                                                                                                  (coin_timelocked  && repeaterNode.count > 2) ? "qrc:/Images/Images/CaretRight.svg" : "qrc:/Images/Images/CaretRightGray.svg"
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.right: parent.right
                        }
                    }
                }
            }
        }
    }

    Component {
        id: nodeReal
        Rectangle {
            width: parent.width
            height: width
            radius: width
            color: "#FFFFFF"
            border.width: 1
            border.color: "#DEDEDE"
            anchors.verticalCenter: parent.verticalCenter
            QIcon {
                iconSize: 12
                source: "qrc:/Images/Images/lock-dark.svg"
                anchors.centerIn: parent
            }
        }
    }

    Component {
        id: nodeEmpty
        Rectangle {
            width: parent.width
            height: 4
            color: walletInfo.timelockType === ScriptNodeHelper.TimelockType.LOCKTYPE_ABSOLUTE ? "#DEDEDE" : (coin_timelocked  && repeaterNode.count > 2) ? "#031F2B" : "#DEDEDE"
            anchors.verticalCenter: parent.verticalCenter
        }
    }
}
