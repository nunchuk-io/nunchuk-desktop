import QtQuick 2.4
import QtGraphicalEffects 1.0
import "../origins"

Rectangle {
    id: idRootStyle1

    // type enum
    readonly property int eFIRST  : 0
    readonly property int eSECOND : 1
    readonly property int eTHIRD  : 2
    readonly property int eFOURD  : 3

    // State enum
    readonly property int eDEFAULT  : 0
    readonly property int eDISABLE  : 1
    readonly property int eHOVER    : 2
    readonly property int eCLICKED  : 3

    // First set
    property var backgroundColorFirst: ["#F6D65D", "#9CAEB8", "#96833B", "#B8A048"]
    property var borderColorFirst: ["transparent", "transparent", "transparent", "#F6D65D"]
    property var textColorFirst: ["#031F2B", "#C9DEF1", "#C9DEF1", "#031F2B"]

    // Second set
    property var backgroundColorSecond: ["transparent", "transparent", "#031F2B", "#1A333D"]
    property var borderColorSecond: ["#031F2B", "#9CAEB8", "#031F2B", "#031F2B"]
    property var textColorSecond: ["#031F2B", "#9CAEB8", "#F1FAFE", "#F1FAFE"]

    // Third set
    property var backgroundColorThird: ["transparent", "#F1FAFE", "#F1FAFE", "#c5cdd1"]
    property var borderColorThird: ["#F1FAFE", "#9CAEB8", "transparent", "#F1FAFE"]
    property var textColorThird: ["#F1FAFE", "#9CAEB8", "#031F2B", "#031F2B"]

    // Fourd set
    property var backgroundColorFourd: ["transparent", "transparent", "#96833B", "#B8A048"]
    property var borderColorFourd: ["#F6D65D", "#839096", "transparent", "#F6D65D"]
    property var textColorFourd: ["#F6D65D", "#839096", "#C9DEF1", "#031F2B"]

    property var backgroundColor: [backgroundColorFirst, backgroundColorSecond, backgroundColorThird, backgroundColorFourd]
    property var borderColor: [borderColorFirst, borderColorSecond, borderColorThird, borderColorFourd]
    property var textColor: [textColorFirst, textColorSecond, textColorThird, textColorFourd]

    property int type: eFIRST

    property string label: "Value"
    property int fontPixelSize: 10
    property var icons : ["", "", "", ""]
    property int iconSize: 24
    property bool iconInRight: false
    border.width: 1
    radius: 20
    state: !enabled ? "disable" : (btnMouse.pressed ? "clicked" : (btnMouse.containsMouse ? "hover" : "default"))
    states: [
        State {
            name: "default"
            PropertyChanges {
                target: idRootStyle1
                color: backgroundColor[type][eDEFAULT]
                border.color: borderColor[type][eDEFAULT]
            }
            PropertyChanges {
                target: text
                color: textColor[type][eDEFAULT]
            }
            PropertyChanges {
                target: ico
                source: "qrc:/Images/Images/" + icons[eDEFAULT]
            }
        },
        State {
            name: "hover"
            PropertyChanges {
                target: idRootStyle1
                color: backgroundColor[type][eHOVER]
                border.color: borderColor[type][eHOVER]
            }
            PropertyChanges {
                target: text
                color: textColor[type][eHOVER]
            }
            PropertyChanges {
                target: ico
                source: "qrc:/Images/Images/" + icons[eHOVER]
            }
        },
        State {
            name: "disable"
            PropertyChanges {
                target: idRootStyle1
                color: backgroundColor[type][eDISABLE]
                border.color: borderColor[type][eDISABLE]
            }
            PropertyChanges {
                target: text
                color: textColor[type][eDISABLE]
            }
            PropertyChanges {
                target: ico
                source: "qrc:/Images/Images/" + icons[eDISABLE]
            }
        },
        State {
            name: "clicked"
            PropertyChanges {
                target: idRootStyle1
                color: backgroundColor[type][eCLICKED]
                border.color: borderColor[type][eCLICKED]
            }
            PropertyChanges {
                target: text
                color: textColor[type][eCLICKED]
            }
            PropertyChanges {
                target: ico
                source: "qrc:/Images/Images/" + icons[eCLICKED]
            }
        }
    ]

    Row {
        anchors.centerIn: parent
        spacing: 8
        layoutDirection: iconInRight ?  Qt.RightToLeft : Qt.LeftToRight
        QImage {
            id: ico
            width: iconSize
            height: iconSize
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

    signal buttonClicked()
    MouseArea {
        id: btnMouse
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        anchors.fill: parent
        onClicked: idRootStyle1.buttonClicked()
    }
}
