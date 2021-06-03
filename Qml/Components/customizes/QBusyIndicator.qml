import QtQuick 2.4
import "../origins"

Item {

    QImage {
        id: ico
        width: 28
        height: 28
        anchors.centerIn: parent
        rotation: 45
        source: "qrc:/Images/Images/Logo_Palcehoder.png"
        transformOrigin: Item.Center
    }

    QImage {
        id: circular
        width: 64
        height: 64
        anchors.centerIn: parent
        source: "qrc:/Images/Images/Circular.png"
        transformOrigin: Item.Center
    }
    NumberAnimation {
        target: circular
        property: "rotation"
        from: 0
        to: 360
        loops: Animation.Infinite
        running: true
        duration: 600
    }
}
