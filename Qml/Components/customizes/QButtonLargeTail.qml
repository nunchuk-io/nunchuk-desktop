import QtQuick 2.4
import QtGraphicalEffects 1.0
import "../origins"

Rectangle {
    id: rootTextButton
    border.width: 2
    // type enum
    readonly property int ePRIMARY  : 0
    readonly property int eSECONDARY: 1

    // State enum
    readonly property int eDEFAULT  : 0
    readonly property int eDISABLE  : 1
    readonly property int eHOVER    : 2
    readonly property int eCLICKED  : 3

    // Primary set
    property var backgroundColorPrimary: ["#F6D65D", "#9CAEB8", "#96833B", "#B8A048"]
    property var borderColorPrimary: ["transparent", "transparent", "transparent", "#F6D65D"]
    property var textColorPrimary: ["#031F2B", "#C9DEF1", "#031F2B", "#031F2B"]

    // Secondary set
    property var backgroundColorSecondary: ["transparent", "transparent", "#031F2B", "#1A333D"]
    property var borderColorSecondary: ["#031F2B", "#9CAEB8", "#031F2B", "#031F2B"]
    property var textColorSecondary: ["#031F2B", "#9CAEB8", "#F1FAFE", "#F1FAFE"]

    property var backgroundColor: [backgroundColorPrimary, backgroundColorSecondary]
    property var borderColor: [borderColorPrimary, borderColorSecondary]
    property var textColor: [textColorPrimary, textColorSecondary]

    property int type: ePRIMARY
    property string label: "text"
    property bool optionVisible: false

    state: !enabled ? "disable" : optionVisible ? "onOption" : (btnMouse.pressed ? "clicked" : (btnMouse.containsMouse ? "hover" : "default"))

    width: 137
    height: 48
    radius: 24

    states: [
        State {
            name: "default"
            PropertyChanges {
                target: rootTextButton
                color: backgroundColor[type][eDEFAULT]
                border.color: borderColor[type][eDEFAULT]
            }
            PropertyChanges {
                target: text
                color: textColor[type][eDEFAULT]
            }
            PropertyChanges {
                target: indicator
                rotation: 0
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
            PropertyChanges {
                target: indicator
                rotation: 0
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
            PropertyChanges {
                target: indicator
                rotation: 0
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
            PropertyChanges {
                target: indicator
                rotation: 0
            }
        },

        State {
            name: "onOption"
            PropertyChanges {
                target: rootTextButton
                color: backgroundColor[type][eHOVER]
                border.color: borderColor[type][eHOVER]
            }
            PropertyChanges {
                target: text
                color: textColor[type][eHOVER]
            }
            PropertyChanges {
                target: indicator
                rotation: 90
            }
        }
    ]

    Row {
        anchors.centerIn: parent
        spacing: 4

        QText {
            id: text
            text: label
            font.pixelSize: 16
            font.family:  "Lato"
            font.weight: Font.Bold
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        Item {
            id: indicator
            width: 16
            height: 16
            anchors.verticalCenter: text.verticalCenter
            Behavior on rotation {
                NumberAnimation {duration: 200}
            }
            Row {
                anchors.centerIn: parent
                Repeater {
                    model: 3
                    Rectangle { width: 4; height: 4; radius: 4; color: text.color}
                }
            }
        }
    }

    onOptionVisibleChanged: {
        if(optionVisible) { state = "onOption"}
        else{ state = "default"}
    }

    signal buttonClicked()

    MouseArea {
        id: btnMouse
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        anchors.fill: rootTextButton
        onPressed: rootTextButton.state = "clicked"
        onContainsMouseChanged: {
            rootTextButton.state = !enabled ? "disable" : optionVisible ? "onOption" : (btnMouse.pressed ? "clicked" : (btnMouse.containsMouse ? "hover" : "default"))
        }

        onReleased: {
            rootTextButton.buttonClicked()
            rootTextButton.state = "onOption"
        }
    }
}
