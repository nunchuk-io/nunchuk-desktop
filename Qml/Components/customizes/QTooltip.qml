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
import "../origins"

QImage {
    width: 24
    height: 24
    property string toolTip
    property bool showToolTip: false
    property bool rightOfParent: false

    source: "qrc:/Images/Images/tooltip.png"
    Rectangle {
        id: toolTipRectangle
        anchors.left: parent.left
        anchors.leftMargin: rightOfParent ? (-(width - parent.width/2)) : parent.width/2
        anchors.bottom: parent.bottom
        anchors.bottomMargin: parent.height/2
        width: 241
        height: toolTipText.height + 16
        z: 200
        opacity: toolTip != "" && showToolTip ? 1 : 0
        color: Qt.rgba(0, 0, 0, 0.7)
        radius: 4
        QText {
            id: toolTipText
            width: parent.width - 16
            text: toolTip
            color: "#F1FAFE"
            font.pixelSize: 12
            font.family: "Lato"
            wrapMode: Text.WordWrap
            anchors.centerIn: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignJustify
        }
        Behavior on opacity {
            PropertyAnimation {
                easing.type: Easing.InOutQuad
                duration: 250
            }
        }
    }
    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onEntered: showTimer.start()
        onExited: { showToolTip = false; showTimer.stop(); }
        hoverEnabled: true
        cursorShape: Qt.WhatsThisCursor
    }
    Timer {
        id: showTimer
        interval: 250
        onTriggered: showToolTip = true;
    }
}
