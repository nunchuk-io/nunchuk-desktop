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
import Qt5Compat.GraphicalEffects
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
    id: coinAgeRoot
    width: 810
    height: childrenRect.height
    radius: 12
    border.width: 1
    border.color: "#FFFFFF"
    color: "#F5F5F5"

    property var    coin_timelocklist
    property var    coin_timelockbase
    property bool   coin_timelocked: false

    readonly property string colorActive: "#031F2B"
    readonly property string colorInactive: "#DEDEDE"
    readonly property int    itemCount: coin_timelocklist ? coin_timelocklist.length : 0
    readonly property var    lastItem: itemCount > 0 ? coin_timelocklist[itemCount - 1] : null
    readonly property bool   allUnlocked: lastItem !== null && lastItem.valuePassed === true
    readonly property real   segmentWidth: Math.max(0, (timelineItem.width - nodeStart.width - nodeEnd.width - nodeWidth * 2) / 2)
    readonly property int    nodeWidth: 20
    readonly property int    stepWidth: segmentWidth + nodeWidth

    function activeIndex() {
        for (var i = 0; i < itemCount; i++) {
            if (coin_timelocklist[i].valueActive === true) {
                return i
            }
        }
        return -1
    }

    function maxContentX() {
        return Math.max(0, flickerTimeline.contentWidth - flickerTimeline.width)
    }

    function scrollTo(x) {
        flickerTimeline.contentX = Math.max(0, Math.min(x, maxContentX()))
    }

    function ensureActiveVisible() {
        var idx = activeIndex()
        if (idx < 0) {
            scrollTo(allUnlocked ? maxContentX() : 0)
            return
        }
        scrollTo((idx - 1) * stepWidth)
    }

    onCoin_timelocklistChanged: Qt.callLater(ensureActiveVisible)

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
                id: flickerTimeline
                anchors.fill: parent
                clip: true
                flickableDirection: Flickable.HorizontalFlick
                boundsBehavior: Flickable.StopAtBounds
                interactive: contentWidth > width
                contentWidth: contentDisplay.width
                onContentWidthChanged: Qt.callLater(coinAgeRoot.ensureActiveVisible)
                onWidthChanged: Qt.callLater(coinAgeRoot.ensureActiveVisible)
                Behavior on contentX {
                    enabled: !flickerTimeline.moving
                    NumberAnimation { duration: 200; easing.type: Easing.OutCubic }
                }
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
                            border.color: coinAgeRoot.colorInactive
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }

                    Repeater {
                        id: repeaterNode
                        model: coin_timelocklist
                        Row {
                            height: 44
                            spacing: 0
                            readonly property bool itemActive: modelData.valueActive === true
                            readonly property bool itemSynthetic: modelData.valueSynthetic === true
                            readonly property bool itemDark: itemActive || modelData.valuePassed === true
                            Item {
                                width: coinAgeRoot.segmentWidth
                                height: 30
                                anchors.bottom: parent.bottom
                                Rectangle {
                                    width: parent.width
                                    height: 4
                                    color: itemDark ? coinAgeRoot.colorActive : coinAgeRoot.colorInactive
                                    anchors.verticalCenter: parent.verticalCenter
                                }
                                Rectangle {
                                    width: partText.implicitWidth + 16
                                    height: 16
                                    border.color: coinAgeRoot.colorActive
                                    color: "#FFFFFF"
                                    radius: 20
                                    anchors.centerIn: parent
                                    visible: modelData.valueRemaining !== undefined && modelData.valueRemaining !== ""
                                    QLato {
                                        id: partText
                                        text: modelData.valueRemaining !== undefined ? modelData.valueRemaining : ""
                                        font.pixelSize: 10
                                        color: coinAgeRoot.colorActive
                                        font.weight: Font.Bold
                                        anchors.centerIn: parent
                                    }
                                }
                            }
                            Item {
                                width: coinAgeRoot.nodeWidth
                                height: 30
                                anchors.bottom: parent.bottom
                                Loader {
                                    width: parent.width
                                    anchors.verticalCenter: parent.verticalCenter
                                    sourceComponent: itemSynthetic ? nodeEmpty : nodeReal
                                    onLoaded: {
                                        if (item && itemSynthetic) {
                                            item.color = Qt.binding(function() { return itemDark ? coinAgeRoot.colorActive : coinAgeRoot.colorInactive })
                                        }
                                    }
                                }
                                QLato {
                                    text: modelData.valueNode
                                    font.pixelSize: 12
                                    color: coinAgeRoot.colorActive
                                    font.weight: itemActive ? Font.Bold : Font.Normal
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    anchors.bottom: parent.top
                                    visible: !itemSynthetic && modelData.valueNode !== ""
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
                            color: coinAgeRoot.allUnlocked ? coinAgeRoot.colorActive : coinAgeRoot.colorInactive
                            anchors.verticalCenter: parent.verticalCenter
                        }
                        QIcon {
                            iconSize: 24
                            source: coinAgeRoot.allUnlocked ? "qrc:/Images/Images/CaretRight.svg" : "qrc:/Images/Images/CaretRightGray.svg"
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.right: parent.right
                        }
                    }
                }
            }

            Rectangle {
                width: 40
                height: parent.height
                anchors.left: parent.left
                visible: backButton.visible
                gradient: Gradient {
                    orientation: Gradient.Horizontal
                    GradientStop { position: 0.0; color: coinAgeRoot.color }
                    GradientStop { position: 1.0; color: "transparent" }
                }
            }
            Rectangle {
                width: 40
                height: parent.height
                anchors.right: parent.right
                visible: nextButton.visible
                gradient: Gradient {
                    orientation: Gradient.Horizontal
                    GradientStop { position: 0.0; color: "transparent" }
                    GradientStop { position: 1.0; color: coinAgeRoot.color }
                }
            }

            Rectangle {
                id: backButton
                width: 24
                height: 24
                radius: 12
                color: "#FFFFFF"
                anchors.left: parent.left
                anchors.top: parent.top
                visible: flickerTimeline.contentX > 1
                QIcon {
                    iconSize: 16
                    source: "qrc:/Images/Images/left-arrow-dark.svg"
                    anchors.centerIn: parent
                }
                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: coinAgeRoot.scrollTo(flickerTimeline.contentX - coinAgeRoot.stepWidth)
                }
            }
            Rectangle {
                id: nextButton
                width: 24
                height: 24
                radius: 12
                color: "#FFFFFF"
                anchors.right: parent.right
                anchors.top: parent.top
                visible: flickerTimeline.contentX < coinAgeRoot.maxContentX() - 1
                QIcon {
                    iconSize: 16
                    source: "qrc:/Images/Images/right-arrow-dark.svg"
                    anchors.centerIn: parent
                }
                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: coinAgeRoot.scrollTo(flickerTimeline.contentX + coinAgeRoot.stepWidth)
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
            border.color: coinAgeRoot.colorInactive
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
            color: coinAgeRoot.colorInactive
            anchors.verticalCenter: parent.verticalCenter
        }
    }
}
