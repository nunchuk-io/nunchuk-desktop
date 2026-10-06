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
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import "../origins"
import "../customizes/Texts"
import "../customizes/Buttons"

Item {
    id: root
    property alias label: screenname
    property alias closebutton: closeButton
    property alias extraHeader: extraHeaderInfo.sourceComponent
    property alias extraTop: extraTopInfo.sourceComponent
    property alias content: contentInfo.sourceComponent
    property alias bottomLeft: botLeft.sourceComponent
    property alias bottomRight: botRight.sourceComponent
    readonly property Item contentItem: contentInfo.item
    readonly property Item rightItem: botRight.item
    property bool enableHeader: true
    property bool hasClose: true
    property int  offset: 36
    property int  offsetTop: 36
    property int  offsetBottom: 36
    property int  offsetLeft: 36
    property int  offsetRight: 36
    property bool sameOffset: true
    property bool isShowLine: false
    property int  minWidth: -1
    signal closeClicked()
    MouseArea {
        anchors.fill: parent
        onClicked: {}
    }
    Rectangle {
        id: mask
        anchors.fill: parent
        color: "#FFFFFF"
        radius: 24
        layer.enabled: true
        layer.effect: OpacityMask {
            maskSource: Rectangle {
                width: root.width
                height: root.height
                radius: 24
            }
        }
        Loader {
            id: extraTopInfo
            height: extraTopInfo.sourceComponent ? extraTopInfo.item.height : 0
            anchors.top: parent.top
        }
    }
    DropShadow {
        anchors.fill: mask
        horizontalOffset: 3
        verticalOffset: 3
        radius: 8.0
        samples: 17
        color: "#80000000"
        source: mask
    }
    Item {
        anchors {
            fill: parent
            leftMargin: sameOffset ? offset : offsetLeft
            rightMargin: sameOffset ? offset : offsetRight
            topMargin: sameOffset ? offset : offsetTop
            bottomMargin: sameOffset ? offset : offsetBottom
        }
        Column {
            spacing: 16
            Item {
                id: headerContainer
                width: root.width - (sameOffset ? offset : offsetLeft) - (sameOffset ? offset : offsetRight) + 12
                height: childrenRect.height
                // Right-most x the title is allowed to reach: 12px clear of the close (X) button when
                // it's visible, otherwise just the dialog's own content edge.
                readonly property real maxTitleWidth: (closeButton.visible ? (closeButton.x - 12) : root.width) - (sameOffset ? offset : offsetLeft)
                Row {
                    spacing: 8
                    QHeadLine {
                        id: screenname
                        // BUGFIX: paintedWidth created a circular binding with wrapMode: WordWrap
                        // (width <- paintedWidth <- current wrapped layout), collapsing to ~1 word per
                        // line (e.g. "Add Blockstream Jade" wrapped 3 lines). implicitWidth is the
                        // natural unwrapped text width, independent of the assigned width. Capped by
                        // maxTitleWidth so a long title wraps/shrinks instead of overflowing past the
                        // dialog edge or overlapping the close button (keeps a 12px gap from it).
                        width: Math.min(minWidth === -1 ? screenname.implicitWidth : minWidth, headerContainer.maxTitleWidth)
                        anchors.verticalCenter: parent.verticalCenter
                        visible: enableHeader
                        text: "Screen name"
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                    Loader {
                        id: extraHeaderInfo
                        height: extraHeaderInfo.sourceComponent ? extraHeaderInfo.item.height : 0
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
            }
            Loader {
                id: contentInfo
                width: root.width - (sameOffset ? offset : offsetLeft) - (sameOffset ? offset : offsetRight)
                height: root.height - (sameOffset ? offset : offsetTop) - (sameOffset ? offset : offsetBottom) - 40 - 16 - 24 - 48
            }
        }
        Item {
            width: root.width - (sameOffset ? offset : offsetLeft) - (sameOffset ? offset : offsetRight)
            height: 48
            anchors.bottom: parent.bottom
            Loader{
                id: botLeft
                anchors.left: parent.left
            }
            Loader{
                id: botRight
                anchors.right: parent.right
            }
        }
    }
    QCloseButton {
        id: closeButton
        visible: enableHeader && hasClose
        bgColor: "transparent"
        anchors {
            right: parent.right
            rightMargin: 24
            top: parent.top
            topMargin: 24
        }
        onClicked: { closeClicked()}
    }
    QLine {
        visible: isShowLine
        width: parent.width
        anchors {
            bottom: parent.bottom
            bottomMargin: 102
            horizontalCenter: parent.horizontalCenter
        }
    }
}
