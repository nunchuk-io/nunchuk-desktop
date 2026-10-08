import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {
    id: _item
    property int radius: 12
    property bool iEnabled: true
    LinearGradient {
        id: _area
        visible: false
        height: parent.height
        width: parent.width
        start: Qt.point(0, 0)
        end: Qt.point(0, parent.height)
        gradient: Gradient {
            GradientStop { position: 0.1; color: "#084B7B" }
            GradientStop { position: 1.0; color: "#2B74A9" }
        }
    }
    Rectangle {
        id: mask
        anchors.fill: parent
        visible: false
        radius: _item.radius
    }
    OpacityMask {
        anchors.fill: parent
        source: _area
        maskSource: mask
    }
}
