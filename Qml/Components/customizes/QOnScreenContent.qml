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
import QtQuick 2.4
import QtQuick.Controls 2.3
import QtGraphicalEffects 1.12
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
    property alias content: contentInfo.sourceComponent
    property alias bottomLeft: botLeft.sourceComponent
    property alias bottomRight: botRight.sourceComponent
    readonly property Item contentItem: contentInfo.item
    property bool enableHeader: true
    property int offset: 36
    signal closeClicked()
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
        anchors.fill: parent
        anchors.margins: offset
        Column {
            spacing: 24
            Item {
                width: root.width - offset*2 + 12
                height: 40
                Row {
                    anchors.fill: parent
                    spacing: 8
                    QHeadLine {
                        id: screenname
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
                Row {
                    anchors.fill: parent
                    spacing: 12
                    layoutDirection: Qt.RightToLeft
                    QCloseButton {
                        id: closeButton
                        visible: enableHeader
                        anchors {
                            verticalCenter: parent.verticalCenter
                            verticalCenterOffset: -12
                        }
                        enabled: visible
                        onClicked: { closeClicked()}
                    }
                }
            }
            Loader {
                id: contentInfo
                width: root.width - offset*2
                height: root.height - offset*2 - 40 - 48 - 48
            }
            Item {
                width: root.width - offset*2
                height: 48
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
    }
}
