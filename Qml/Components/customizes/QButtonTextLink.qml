import QtQuick 2.4
import QtGraphicalEffects 1.0
import "../origins"

Item {
    id: idRootStyle1

    // type enum
    readonly property int eLEFT  : 0
    readonly property int eRIGHT : 1

    // State enum
    readonly property int eDEFAULT  : 0
    readonly property int eDISABLE  : 1
    readonly property int eHOVER    : 2

    // Property set
    property var icon: ["qrc:/Images/Images/arrow_outline_n.png", "qrc:/Images/Images/arrow_outline_d.png", "qrc:/Images/Images/arrow_outline_p.png"]
    property var textColor: ["#031F2B", "#839096", "#35ABEE"]

    property int direction: eLEFT
    property string label: "Value"
    property int fontPixelSize: 12

    state: !enabled ? "disable" : btnMouse.containsMouse ? "hover" : "default"

    states: [
        State {
            name: "default"
            PropertyChanges {
                target: ico
                source: icon[eDEFAULT]
            }
            PropertyChanges {
                target: text
                color: textColor[eDEFAULT]
            }
        },
        State {
            name: "hover"
            PropertyChanges {
                target: ico
                source: icon[eHOVER]
            }
            PropertyChanges {
                target: text
                color: textColor[eHOVER]
            }
        },
        State {
            name: "disable"
            PropertyChanges {
                target: ico
                source: icon[eDISABLE]
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
        layoutDirection: (direction == eRIGHT) ?  Qt.RightToLeft : Qt.LeftToRight
        QImage {
            id: ico
            mirror: (direction == eRIGHT)
            anchors.verticalCenter: parent.verticalCenter
        }
        QText {
            id: text
            anchors.verticalCenter: ico.verticalCenter
            text: label
            font.pixelSize: fontPixelSize
            font.weight: Font.DemiBold
            font.family: "Montserrat"
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
