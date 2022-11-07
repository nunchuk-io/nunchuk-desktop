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
import QtMultimedia 5.12
import QtGraphicalEffects 1.12
import QZXing 3.3
import "../origins"

Item {
    id: qrscannerRoot
    signal tagFound(var tag)
    layer.enabled: true
    layer.effect: OpacityMask {
        maskSource: Rectangle {
            width: qrscannerRoot.width
            height: qrscannerRoot.height
        }
    }
    function startCamera(){camera.cameraState = Camera.ActiveState }
    function stopCamera(){ camera.cameraState = Camera.UnloadedState }
    Camera {
        id: camera
        cameraState: Camera.UnloadedState
        captureMode: Camera.CaptureStillImage
        focus {
            focusMode: CameraFocus.FocusContinuous
            focusPointMode: CameraFocus.FocusPointAuto
        }
    }
    VideoOutput {
        id: videoOutput
        source: camera
        width: qrscannerRoot.width
        height: qrscannerRoot.height
        anchors.centerIn: parent
        fillMode: VideoOutput.PreserveAspectCrop
        filters: [ zxingFilter ]
    }
    QZXingFilter {
        id: zxingFilter
        decoder {
            onTagFound: { qrscannerRoot.tagFound(tag) }
        }
        captureRect: {
            // setup bindings
            videoOutput.contentRect;
            videoOutput.sourceRect;
            // only scan the central quarter of the area for a barcode
            return videoOutput.mapRectToSource(videoOutput.mapNormalizedRectToItem(Qt.rect(captureRectX,
                                                                                           captureRectY,
                                                                                           captureRectW,
                                                                                           captureRectH)));
        }
    }

    readonly property real areaSize: videoOutput.width*0.75
    readonly property real captureRectX: (videoOutput.width - areaSize)/(videoOutput.width*2)
    readonly property real captureRectY: (videoOutput.height - areaSize)/(videoOutput.height*2)
    readonly property real captureRectW: areaSize/videoOutput.width
    readonly property real captureRectH: areaSize/videoOutput.height
    readonly property int  iconsize: 20

    Item {
        id: captureArea
        width: areaSize
        height: areaSize
        anchors.centerIn: parent
        QImage {source: "qrc:/Images/Images/C1.png"; anchors.right: parent.right; width: iconsize; height: iconsize}
        QImage {source: "qrc:/Images/Images/C2.png"; anchors.left: parent.left; width: iconsize; height: iconsize}
        QImage {source: "qrc:/Images/Images/C3.png"; anchors.left: parent.left; anchors.bottom: parent.bottom; width: iconsize; height: iconsize}
        QImage {source: "qrc:/Images/Images/C4.png"; anchors.right: parent.right; anchors.bottom: parent.bottom; width: iconsize; height: iconsize}
        QImage {id: linescan; source: "qrc:/Images/Images/qrline.png"; width: parent.width; height: 3}
        SequentialAnimation {
            running: true
            loops: Animation.Infinite
            NumberAnimation {
                target: linescan
                property: "y"
                from: linescan.height
                to: captureArea.height - linescan.height
                duration: 1000
                easing.type: Easing.InQuad
            }
            NumberAnimation {
                target: linescan
                property: "y"
                duration: 1000
                from: captureArea.height - linescan.height
                to: linescan.height
                easing.type: Easing.InQuad
            }
        }
    }

    Rectangle {
        anchors.fill: parent
        color: "transparent"
        opacity: 0.5
        border.color: Qt.rgba(0,0,0, 0.5)
        border.width: (width - areaSize)/2
    }
}
