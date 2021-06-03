import QtQuick 2.0

Item {
    id: root
    readonly property int viewerIndex: QAPP_VIEWER_INDEX
    property bool isOnTop: false
    signal eventReceiver(int event, int data)
    Component.onCompleted: {QMLHandle.doRegisterQML(root)}
    Component.onDestruction: {QMLHandle.unRegisterQML(root)}
}
