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
        end: Qt.point(parent.width, 0)
        gradient: Gradient {
            GradientStop { position: 0.1; color: _item.iEnabled ? "#2F766D" : "#595959" }
            GradientStop { position: 1.0; color: _item.iEnabled ? "#1C4A21" : "#595959" }
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
