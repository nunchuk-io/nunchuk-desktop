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
import QtGraphicalEffects 1.12
import "../origins"
import "../../../localization/STR_QML.js" as STR

QImage {
    width: 24
    height: 24
    property string toolTip
    property string titleStr:STR.STR_QML_339
    property bool showToolTip: false
    property bool rightOfParent: false
    property int tipWidth: 241

    source: "qrc:/Images/Images/tooltip.png"
    Rectangle {
        id: toolTipRectangle
        anchors.left: parent.left
        anchors.leftMargin: rightOfParent ? (-(width - parent.width/2)) : parent.width/2
        anchors.verticalCenter: parent.verticalCenter
//        anchors.bottomMargin: -(parent.height/2)
        width: tipWidth
        height: (_title.height + toolTipText.height) + 20
        z: 200
        opacity: toolTip != "" && showToolTip ? 1 : 0
        color: "#FFFFFF"
        radius: 12
        Column{
            id:_col
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 16
            spacing: 4
            QText {
                id: _title
                width: parent.width - 16
                height: paintedHeight
                text: titleStr
                color: "#222222"
                font.pixelSize: 16
                font.weight: Font.Bold
                font.family: "Lato"
                wrapMode: Text.WordWrap
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
            }
            QText {
                id: toolTipText
                width: tipWidth - 16
                height: paintedHeight
                text: toolTip
                color: "#222222"
                font.pixelSize: 12
                font.family: "Lato"
                wrapMode: Text.WordWrap
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
            }
        }

        Behavior on opacity {
            PropertyAnimation {
                easing.type: Easing.InOutQuad
                duration: 250
            }
        }
    }
    DropShadow {
        visible: showToolTip
        anchors.fill: toolTipRectangle
        horizontalOffset: 3
        verticalOffset: 5
        spread: 0
        radius: 8
        samples: 30
        color: "#aa000000"
        source: toolTipRectangle
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
