import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {
    LinearGradient {
        id: _area
        visible: false
        height: parent.height
        width: parent.width
        start: Qt.point(0, 0)
        end: Qt.point(parent.width, 0)
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#2F766D" }
            GradientStop { position: 1.0; color: "#1C4A21" }
        }
    }
    Rectangle {
        id: mask
        anchors.fill: parent
        visible: false
        radius: 12
    }
    OpacityMask {
        anchors.fill: parent
        source: _area
        maskSource: mask
    }
}
