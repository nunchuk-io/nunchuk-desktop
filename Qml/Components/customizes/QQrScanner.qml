import QtQuick 2.4
import QtMultimedia 5.4
import QZXing 2.3

import "../origins"

Item {
    id: qrscannerRoot
    property var tagData:[]
    signal scanFinished(var result, var count)

    Camera
    {
        id:camera
        focus {
            focusMode: CameraFocus.FocusContinuous
            focusPointMode: CameraFocus.FocusPointAuto
        }
        position: Camera.FrontFace
    }

    VideoOutput
    {
        id: videoOutput
        source: camera
        anchors.fill: parent
        autoOrientation: true
        filters: [ zxingFilter ]
        MouseArea {
            anchors.fill: parent
            onClicked: {
                camera.focus.customFocusPoint = Qt.point(mouse.x / width,  mouse.y / height);
                camera.focus.focusMode = CameraFocus.FocusMacro;
                camera.focus.focusPointMode = CameraFocus.FocusPointCustom;
            }
        }
    }

    QZXingFilter
    {
        id: zxingFilter
        decoder {
            enabledDecoders: QZXing.DecoderFormat_QR_CODE
            onTagFound: {
                if(tagData.indexOf(tag) < 0){
                    tagData.push(tag)
                    timeoutDecode.restart()
                }
            }
            tryHarder: false
        }
    }

    QImage {source: "qrc:/Images/Images/C1.png"; anchors.right: parent.right}
    QImage {source: "qrc:/Images/Images/C2.png"; anchors.left: parent.left}
    QImage {source: "qrc:/Images/Images/C3.png"; anchors.left: parent.left; anchors.bottom: parent.bottom}
    QImage {source: "qrc:/Images/Images/C4.png"; anchors.right: parent.right; anchors.bottom: parent.bottom}
    QImage {id: linescan; source: "qrc:/Images/Images/qrline.png"; width: parent.width; height: 5}

    SequentialAnimation {
        running: true
        loops: Animation.Infinite
        NumberAnimation {
            target: linescan
            property: "y"
            from: 0
            to: qrscannerRoot.height - linescan.height
            duration: 1000
        }
        NumberAnimation {
            target: linescan
            property: "y"
            duration: 1000
            from: qrscannerRoot.height - linescan.height
            to: 0
        }
    }

    Timer {
        id: timeoutDecode
        interval: 2000
        onTriggered: {
            qrscannerRoot.scanFinished(tagData, tagData.length)
        }
    }
}
