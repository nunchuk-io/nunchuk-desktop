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
import QBarcodeFilter 1.0
import "../../origins"
import "../../customizes/Texts"
import "../../customizes/Buttons"

Item {
    id: qrscannerRoot

    property int    percent: 0
    property bool   complete: false

    signal tagFound(var tag)
    layer.enabled: true
    layer.effect: OpacityMask {
        maskSource: Rectangle {
            width: qrscannerRoot.width
            height: qrscannerRoot.height
        }
    }
    Camera {
        id: camera
        focus {
            focusMode: CameraFocus.FocusContinuous
            focusPointMode: CameraFocus.FocusPointCenter
        }
    }

    VideoOutput {
        id: videoOutput
        source: camera
        width: 1280//camera.viewfinder.resolution.width
        height: 720//camera.viewfinder.resolution.height
        anchors.centerIn: parent
        filters: [ barcodeFilter ]
        transform:  Rotation {
            origin.x: videoOutput.width / 2
            axis { x: 0; y: 1; z: 0 }
            angle: 180
        }
        onSourceRectChanged: {
            var res = camera.viewfinder.resolution
            var expectedWidth = Math.max(1280, res.width)
            var expectedHeight = Math.max(720, res.height)
            videoOutput.width = expectedWidth
            videoOutput.height = expectedHeight
            var width = expectedWidth
            var height = expectedHeight
            var capturedWidth = 380
            var capturedHeight = 380
            var captureRectX = ((width - capturedWidth) / 2)/width
            var captureRectY = ((height - capturedHeight) / 2)/height
            var captureRectWidth = capturedWidth / width
            var captureRectHeight = capturedHeight / height
            barcodeFilter.captureRect = videoOutput.mapRectToSource(videoOutput.mapNormalizedRectToItem(Qt.rect(captureRectX,
                                                                                                                captureRectY,
                                                                                                                captureRectWidth,
                                                                                                                captureRectHeight)))
        }
        // QQrScannerOverlay {
        //     id: scanOverlay
        //     anchors.fill: parent
        //     areaRect: videoOutput.mapRectToItem(barcodeFilter.captureRect)
        // }
    }

    QBarcodeFilter {
        id: barcodeFilter
        // you can adjust capture rect (scan area) ne changing these Qt.rect() parameters
        // captureRect: videoOutput.mapRectToSource(videoOutput.mapNormalizedRectToItem(Qt.rect(0.125, 0.125, 0.75, 0.75)))
        onTagFound: {
            // console.log("tag found: " + tag)
            qrscannerRoot.tagFound(tag)
        }
        onScanPercentChanged: {
            // console.log("Percent", barcodeFilter.scanPercent)
            qrscannerRoot.percent = barcodeFilter.scanPercent
        }
        onScanCompleteChanged: {
            // console.log("Completed", barcodeFilter.scanComplete)
            qrscannerRoot.complete = barcodeFilter.scanComplete
        }
    }
}
