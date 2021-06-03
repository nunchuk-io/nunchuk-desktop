import QtQuick 2.4
import QtGraphicalEffects 1.0
import "../origins"

Rectangle {
    id: rootTextButton
    width: 137
    height: 48
    radius: 24
    border.width: 2
    property alias label: text

    state: !enabled ? "disable" : (btnMouse.pressed ? "clicked" : (btnMouse.containsMouse ? "hover" : "default"))

    property var backgroundColor: [backgroundColorTypeA, backgroundColorTypeB, backgroundColorTypeC, backgroundColorTypeD]
    property var borderColor: [borderColorTypeA, borderColorTypeB, borderColorTypeC, borderColorTypeD]
    property var textColor: [textColorTypeA, textColorTypeB, textColorTypeC, textColorTypeD]

    property int type: eTypeA
    readonly property int eTypeA: 0
    readonly property int eTypeB: 1
    readonly property int eTypeC: 2
    readonly property int eTypeD: 3

    // Click state
    readonly property int eNORMAL: 0
    readonly property int eHOVER : 1
    readonly property int eDISABLE: 2
    readonly property int eCLICKED: 3

    property var backgroundColorTypeA: ["#F6D65D", "#96833B", "#9CAEB8", "#B8A048"]
    property var borderColorTypeA: ["transparent", "transparent", "transparent", "#F6D65D"]
    property var textColorTypeA: ["#031F2B", "#C9DEF1", "#C9DEF1", "#031F2B"]

    property var backgroundColorTypeB: ["transparent", "#031F2B", "transparent", "#1A333D"]
    property var borderColorTypeB: ["#031F2B", "#031F2B", "#9CAEB8", "#031F2B"]
    property var textColorTypeB: ["#031F2B", "#F1FAFE", "#9CAEB8", "#F1FAFE"]

    property var backgroundColorTypeC: ["transparent", Qt.rgba(218, 222, 224, 0.1), "transparent", Qt.rgba(218, 222, 224, 0.4)]
    property var borderColorTypeC: ["#F1FAFE", Qt.rgba(241, 250, 254, 0.4), "#F1FAFE", "#F1FAFE"]
    property var textColorTypeC: ["#F1FAFE", "#F1FAFE", "#F1FAFE", "#F1FAFE"]

    property var backgroundColorTypeD: ["transparent", "#9FA0A1","transparent", "#8B8C8C"]
    property var borderColorTypeD: ["#FF7A00", "#FF7A00", "#FF7A00", "#FF7A00"]
    property var textColorTypeD: ["#FF7A00", "#FFFFFF", "#FF7A00", "#FFFFFF"]

    states: [
        State {
            name: "default"
            PropertyChanges {
                target: rootTextButton
                color: backgroundColor[type][eNORMAL]
                border.color: borderColor[type][eNORMAL]
            }
            PropertyChanges {
                target: text
                color: textColor[type][eNORMAL]
            }
        },
        State {
            name: "hover"
            PropertyChanges {
                target: rootTextButton
                color: backgroundColor[type][eHOVER]
                border.color: borderColor[type][eHOVER]
            }
            PropertyChanges {
                target: text
                color: textColor[type][eHOVER]
            }
        },
        State {
            name: "disable"
            PropertyChanges {
                target: rootTextButton
                color: backgroundColor[type][eDISABLE]
                border.color: borderColor[type][eDISABLE]
            }
            PropertyChanges {
                target: text
                color: textColor[type][eDISABLE]
            }
        },
        State {
            name: "clicked"
            PropertyChanges {
                target: rootTextButton
                color: backgroundColor[type][eCLICKED]
                border.color: borderColor[type][eCLICKED]
            }
            PropertyChanges {
                target: text
                color: textColor[type][eCLICKED]
            }
        }
    ]

    QText {
        id: text
        width: 102
        height: 14
        anchors.centerIn: rootTextButton
        text: qsTr("TEXT")
        font.weight: Font.DemiBold
        font.pixelSize: 16
        lineHeight: 16
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    signal buttonClicked()
    MouseArea {
        id: btnMouse
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        anchors.fill: rootTextButton
        onClicked: rootTextButton.buttonClicked()
    }
}
