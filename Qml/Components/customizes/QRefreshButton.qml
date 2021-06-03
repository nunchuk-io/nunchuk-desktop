import QtQuick 2.4
import QtGraphicalEffects 1.0
import "../origins"

Rectangle {
    id: idRootStyle1

    // State enum
    readonly property int eDEFAULT  : 0
    readonly property int eDISABLE  : 1
    readonly property int eHOVER    : 2

    property var backgroundColor: ["transparent", "transparent", "#031F2B"]
    property var borderColor: ["#031F2B", "#9CAEB8", "#031F2B"]
    property var textColor: ["#031F2B", "#9CAEB8", "#F1FAFE"]

    property string label: "Value"
    property int fontPixelSize: 10
    property alias icon : ico.source
    property int iconSize: 24
    property bool iconInRight: false
    border.width: 1
    radius: 20

    state: !enabled ? "disable" : btnMouse.pressed || btnMouse.containsMouse ? "clicked" : "default"

    states: [
        State {
            name: "default"
            PropertyChanges {
                target: idRootStyle1
                color: backgroundColor[eDEFAULT]
                border.color: borderColor[eDEFAULT]
            }
            PropertyChanges {
                target: text
                color: textColor[eDEFAULT]
            }
        },
        State {
            name: "hover"
            PropertyChanges {
                target: idRootStyle1
                color: backgroundColor[eHOVER]
                border.color: borderColor[eHOVER]
            }
            PropertyChanges {
                target: text
                color: textColor[eHOVER]
            }
        },
        State {
            name: "disable"
            PropertyChanges {
                target: idRootStyle1
                color: backgroundColor[eDISABLE]
                border.color: borderColor[eDISABLE]
            }
            PropertyChanges {
                target: text
                color: textColor[eDISABLE]
            }
        }
    ]

    Row {
        anchors.centerIn: parent
        spacing: 8
        QImage {
            id: ico
            width: iconSize
            height: iconSize
            transformOrigin: Item.Center
            source: "qrc:/Images/Images/cached_24px.png"
            anchors.verticalCenter: parent.verticalCenter
        }
        QText {
            id: text
            anchors.verticalCenter: parent.verticalCenter
            text: label
            font.pixelSize: fontPixelSize
            font.weight: Font.Bold
            font.family: "Lato"
        }
    }

    function startRefresh() {animate.start()}
    function stopRefresh()  {animate.stop() }

    NumberAnimation {
        id: animate
        target: ico
        property: "rotation"
        from: 0
        to: -360
        duration: 500
        loops: Animation.Infinite
        alwaysRunToEnd: true
    }

    signal buttonClicked()
    MouseArea {
        id: btnMouse
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        anchors.fill: parent
        onClicked: {
            idRootStyle1.buttonClicked()
            startRefresh()
        }
    }

    Connections {
        target: AppModel
        onFinishedScanDevices : stopRefresh()
    }
}
