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
import QtQuick 2.0
import QtQuick 2.0
import QtQml 2.2

Rectangle{
    property int percentage: 50
    id: root
    width: 300
    height: 300
    radius: width / 2

    color: "white"
    border.color: "#385d8a"
    border.width: 5

    Item{
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        height: parent.height * parent.percentage / 100
        clip: true

        Rectangle{
            width: root.width - root.border.width * 2
            height: root.height - root.border.width * 2
            radius: width / 2
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.margins: root.border.width
            color: "red"
        }
    }

    Text{
        anchors.centerIn: parent
        font.pixelSize: 20
        text: root.percentage.toString() + "%"
    }
}

//Rectangle {
//    id: root
//    width: size
//    height: size
//    border.color: "Red"
//    property int size: 200               // The size of the circle in pixel
//    property real arcBegin: 0            // start arc angle in degree
//    property real arcEnd: 270            // end arc angle in degree
//    property real arcOffset: 0           // rotation
//    property bool isPie: false           // paint a pie instead of an arc
//    property bool showBackground: false  // a full circle as a background of the arc
//    property real lineWidth: 20          // width of the line
//    property string colorCircle: "#CC3333"
//    property string colorBackground: "#779933"
//    property alias beginAnimation: animationArcBegin.enabled
//    property alias endAnimation: animationArcEnd.enabled
//    property int animationDuration: 200
//    onArcBeginChanged: canvas.requestPaint()
//    onArcEndChanged: canvas.requestPaint()
//    Behavior on arcBegin {
//       id: animationArcBegin
//       enabled: true
//       NumberAnimation {
//           duration: root.animationDuration
//           easing.type: Easing.InOutCubic
//       }
//    }
//    Behavior on arcEnd {
//       id: animationArcEnd
//       enabled: true
//       NumberAnimation {
//           duration: root.animationDuration
//           easing.type: Easing.InOutCubic
//       }
//    }
//    Canvas {
//        id: canvas
//        anchors.fill: parent
//        rotation: -90 + root.arcOffset
//        onPaint: {
//            var ctx = getContext("2d")
//            var x = (size / 2)
//            var y = (size / 2)
//            var start = Math.PI * (root.arcBegin / 180)
//            var end = Math.PI * (root.arcEnd / 180)
//            ctx.reset()
//            if (root.isPie) {
//                if (root.showBackground) {
//                    ctx.beginPath()
//                    ctx.fillStyle = root.colorBackground
//                    ctx.moveTo(x, y)
//                    ctx.arc(x, y, size / 2, 0, Math.PI * 2, false)
//                    ctx.lineTo(x, y)
//                    ctx.fill()
//                }
//                ctx.beginPath()
//                ctx.fillStyle = root.colorCircle
//                ctx.moveTo(x, y)
//                ctx.arc(x, y, size / 2, start, end, false)
//                ctx.lineTo(x, y)
//                ctx.fill()
//            } else {
//                if (root.showBackground) {
//                    ctx.beginPath();
//                    ctx.arc(x, y, (width / 2) - root.lineWidth / 2, 0, Math.PI * 2, false)
//                    ctx.lineWidth = root.lineWidth
//                    ctx.strokeStyle = root.colorBackground
//                    ctx.stroke()
//                }
//                ctx.beginPath();
//                ctx.arc(x, y, (size / 2) - root.lineWidth / 2, start, end, false)
//                ctx.lineWidth = root.lineWidth
//                ctx.strokeStyle = root.colorCircle
//                ctx.stroke()
//            }
//        }
//    }
//}

