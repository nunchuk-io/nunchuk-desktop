import QtQuick 2.4
import "../origins"

Item {
    id: root
    width: QAPP_DEVICE_WIDTH
    height: QAPP_DEVICE_HEIGHT
    readonly property int popupWidth: 800
    readonly property int popupHeight: 700


    transformOrigin: Item.TopLeft
    property bool isOnTop: false
    enabled: isOnTop
    signal eventReceiver(int event, int data)
    Component.onCompleted: {QMLHandle.doRegisterQML(root)}
    Component.onDestruction: {QMLHandle.unRegisterQML(root)}
    Rectangle {
        width: QAPP_DEVICE_WIDTH
        height: QAPP_DEVICE_HEIGHT
        color: Qt.rgba(0, 0, 0, 0.7)
        scale: 5
    }
    MouseArea {
        anchors.fill: parent
        onClicked: {root.focus = true}
    }
}
