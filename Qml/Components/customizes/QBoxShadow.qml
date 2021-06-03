import QtQuick 2.4
import QtGraphicalEffects 1.0
Item {
    id: bxshd
    property alias shadow: drshd
    property int radius: 4
    property color color: "#FBFCFF"

    Rectangle {
        id: bg
        width: bxshd.width
        height: bxshd.height
        color: bxshd.color
        radius: bxshd.radius
    }

    DropShadow {
        id: drshd
        anchors.fill: bg
        source: bg
    }
}
