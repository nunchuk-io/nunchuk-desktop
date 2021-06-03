import QtQuick 2.4
import QtGraphicalEffects 1.12
import "../origins"

Item {
    width: 100
    height: 100
    transformOrigin: Item.Center
    scale: qrMouse.pressed ? 0.95 : 1

    signal buttonClicked()

    MouseArea {
        id: qrMouse
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        anchors.fill: parent
        onClicked: buttonClicked()
    }
    Rectangle {
        id: qrmaskIco
        width: 100
        height: 100
        radius: 8
        visible: false
    }

    QImage {
        id: qricoimport
        width: 100
        height: 100
        source: "qrc:/Images/Images/QrIco.png"
        visible: false
    }

    OpacityMask {
        id: qrOpamask
        anchors.fill: qricoimport
        source: qricoimport
        maskSource: qrmaskIco
    }

    DropShadow {
        anchors.fill: qrOpamask
        horizontalOffset: 3
        verticalOffset: 3
        radius: 8.0
        samples: 17
        color: "#80000000"
        source: qrOpamask
    }
}
