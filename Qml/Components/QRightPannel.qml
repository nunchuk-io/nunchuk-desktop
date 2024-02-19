import QtQuick 2.0

Rectangle {
    height: parent.height
    color: "#FFFFFF"
    property Component right_pannel
    readonly property var rightContent: _right.item
    Loader {
        id: _right
        anchors.fill: parent
        sourceComponent: right_pannel
    }
}
