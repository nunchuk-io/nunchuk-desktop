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
import QtCore
import QtMultimedia
import QBarcodeFilter 1.0
import "../../origins"
import "../../customizes/Texts"
import "../../customizes/Buttons"

Item {
    id: qrscannerRoot

    property int  percent:  0
    property bool complete: false

    signal tagFound(var tag)

    // ── Permission ────────────────────────────────────────────────────────────
    // Use Qt's QML-native CameraPermission type.
    // On macOS this shows the system dialog; on Windows/Linux it resolves
    // to Granted immediately (no dialog needed).
    CameraPermission {
        id: camPerm
        onStatusChanged: {
            if (status === Qt.PermissionStatus.Granted)
                camera.start()
            else if (status === Qt.PermissionStatus.Denied)
                camera.stop()
        }
    }

    // ── Camera pipeline ───────────────────────────────────────────────────────
    CaptureSession {
        camera: Camera {
            id: camera

            // DO NOT set focusMode — macOS built-in cameras (FaceTime HD,
            // Continuity Camera) do not support programmatic focus and will
            // fail silently if an unsupported mode is set.

            onErrorOccurred: (errorCode, errorString) => {
                console.warn("[QQrScanner] Camera error", errorCode, ":", errorString)
            }

            onCameraFormatChanged: {
                var res = (cameraFormat && cameraFormat.resolution.width > 0)
                          ? cameraFormat.resolution : Qt.size(0, 0)
                if (res.width > 0 && res.height > 0) {
                    var sz = Math.min(res.width, res.height) * 0.8
                    barcodeFilter.captureRect = Qt.rect(
                        (res.width  - sz) / 2,
                        (res.height - sz) / 2,
                        sz, sz)
                }
            }
        }
        videoOutput: videoPresent
    }

    // ── Camera preview ────────────────────────────────────────────────────────
    // Mirror horizontally so the feed behaves like a mirror for the user
    // (move right → image moves right). This is visual-only: raw frames
    // delivered to QBarcodeFilter via videoSink are NOT mirrored, so QR
    // scanning accuracy is unaffected.
    VideoOutput {
        id: videoPresent
        anchors.fill: parent
        fillMode: VideoOutput.PreserveAspectCrop
        transform: Scale {
            xScale: -1
            origin.x: videoPresent.width / 2
        }
    }

    // ── "Permission denied" overlay ───────────────────────────────────────────
    // NOTE: once the OS denies camera permission, camPerm.request() will NOT
    // show the dialog again. The only way for the user to re-enable is via
    // system settings. We surface a direct "Open Settings" link for each OS.
    Rectangle {
        anchors.fill: parent
        color: "#F0F0F0"
        visible: camPerm.status === Qt.PermissionStatus.Denied

        Column {
            anchors.centerIn: parent
            spacing: 12

            QIcon {
                anchors.horizontalCenter: parent.horizontalCenter
                iconSize: 48
                source: "qrc:/Images/Images/camera-dark.svg"
            }
            QLato {
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: 14
                font.weight: Font.Bold
                color: "#031F2B"
                text: "Camera access denied"
            }
            QLato {
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: 12
                color: "#595959"
                horizontalAlignment: Text.AlignHCenter
                text: Qt.platform.os === "osx"
                      ? "Go to System Settings → Privacy & Security\n→ Camera and enable Nunchuk"
                      : Qt.platform.os === "windows"
                        ? "Go to Settings → Privacy & Security → Camera\nand allow Nunchuk access"
                        : "Allow camera access for Nunchuk\nin your system privacy settings"
            }
            // "Open Settings" button — takes user directly to the camera privacy page.
            // After granting access there, user must press Retry (macOS does not
            // notify running processes of TCC changes via onStatusChanged).
            QTextButton {
                anchors.horizontalCenter: parent.horizontalCenter
                width: 160
                height: 48
                label.text: "Open Settings"
                type: eTypeE
                onButtonClicked: {
                    var url = ""
                    if      (Qt.platform.os === "osx")     url = "x-apple.systempreferences:com.apple.preference.security?Privacy_Camera"
                    else if (Qt.platform.os === "windows") url = "ms-settings:privacy-webcam"
                    if (url !== "") Qt.openUrlExternally(url)
                }
            }
            QTextButton {
                anchors.horizontalCenter: parent.horizontalCenter
                width: 160
                height: 48
                label.text: "Retry"
                type: eTypeA
                onButtonClicked: {
                    if (camPerm.status === Qt.PermissionStatus.Granted)
                        camera.start()
                    else
                        camPerm.request()
                }
            }
        }
    }

    // ── Barcode decoder ───────────────────────────────────────────────────────
    QBarcodeFilter {
        id: barcodeFilter
        videoOutput: videoPresent
        onTagFound:           (tag) => { qrscannerRoot.tagFound(tag)                      }
        onScanPercentChanged:       { qrscannerRoot.percent  = barcodeFilter.scanPercent  }
        onScanCompleteChanged:      { qrscannerRoot.complete = barcodeFilter.scanComplete }
    }

    Component.onCompleted: {
        if (camPerm.status === Qt.PermissionStatus.Granted)
            camera.start()
        else
            camPerm.request()
    }
}
