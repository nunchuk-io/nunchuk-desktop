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
import "../origins"

Rectangle {
    radius: height

    property bool switchOn         : false
    property var borderColors      : ["#031F2B","#FFFFFF"]
    property var backgroundColors  : ["#FFFFFF","#031F2B"]
    property var leftIcons : ["qrc:/Images/Images/OnlineMode/switch_indicator_off.png","qrc:/Images/Images/OnlineMode/switch_on.png"]
    property var rightIcons: ["qrc:/Images/Images/OnlineMode/switch_off.png","qrc:/Images/Images/OnlineMode/switch_indicator_on.png"]

    color: switchOn ? backgroundColors[1] : backgroundColors[0]
    border.color: switchOn ? borderColors[1] : borderColors[0]
    border.width: 2

    Item {
        id: control
        anchors.fill: parent
        anchors.margins: 6
        anchors.centerIn: parent
        Item {
            width: parent.width/2
            height: parent.height
            anchors.left: parent.left

            QImage {
                height: switchOn ? parent.height*0.42 : parent.height
                width: switchOn ? parent.height*0.56 : height
                anchors.centerIn: parent
                source: switchOn ? leftIcons[1] : leftIcons[0]
            }
        }
        Item {
            width: parent.width/2
            height: parent.height
            anchors.right: parent.right
            QImage {
                height: switchOn ? parent.height : parent.height*0.42
                width: switchOn ? height : parent.height*0.42
                anchors.centerIn: parent
                source: switchOn ? rightIcons[1] : rightIcons[0]
            }
        }
    }
    signal buttonClicked()
    MouseArea {
        id: btnMouse
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        anchors.fill: parent
        onClicked: {
            switchOn = !switchOn
            buttonClicked()
        }
    }
}
