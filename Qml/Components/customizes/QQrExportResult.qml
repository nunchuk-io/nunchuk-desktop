import QtQuick 2.4
import QtGraphicalEffects 1.12
import QRCodeItem 1.0
import "../origins"

Rectangle {
    id: qrcodeExportResult
    property alias model: listQr.model

    MouseArea {anchors.fill: parent; onClicked: {}}
    color: Qt.rgba(255, 255, 255, 0.7)
    radius: 8
    Rectangle {
        id: qrcodeExportResultMask
        width: 300
        height: 300
        radius: 4
        visible: false
    }

    Rectangle {
        id: qrcodeExportResultContent
        width: 300
        height: 300
        color: "#F1FAFE"
        anchors.centerIn: parent
        radius: 4
        visible: false
        Rectangle {
            width: parent.width
            height: 4
            color: "#F6D65D"
        }
    }

    OpacityMask {
        id: opaQrmask
        anchors.fill: qrcodeExportResultContent
        source: qrcodeExportResultContent
        maskSource: qrcodeExportResultMask

        QCloseButton {
            anchors {
                right: parent.right
                rightMargin: 16
                top: parent.top
                topMargin: 16
            }
            onClicked: qrcodeExportResult.visible = false
        }

        QListView {
            id: listQr
            width: 200
            height: 200
            currentIndex: 0
            anchors.centerIn: parent
            clip: true
            delegate: QRCodeItem {
                id: qrCode
                width: 200
                height: 200
                borderWitdh: 9
                textInput: modelData
            }
        }
    }

    Timer {
        interval: 250
        running: true
        repeat: true
        onTriggered: {
            if(listQr.currentIndex < listQr.count) {listQr.currentIndex++}
            else{listQr.currentIndex = 0}
            listQr.positionViewAtIndex(listQr.currentIndex, ListView.Beginning)
        }
    }

    DropShadow {
        anchors.fill: opaQrmask
        horizontalOffset: 3
        verticalOffset: 3
        radius: 8.0
        samples: 17
        color: "#80000000"
        source: opaQrmask
    }
}
