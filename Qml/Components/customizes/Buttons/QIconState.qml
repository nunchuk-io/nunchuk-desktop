import QtQuick 2.0
import "../../origins"

QImage {
    id: _icon
    property int iconSize: 24
    property var icons : ["", "", "", ""]
    readonly property string path: icons[0].startsWith("qrc:") ? "" : "qrc:/Images/Images/"
    width: iconSize
    height: iconSize
    state: !enabled ? "disable" : (btnMouse.pressed ? "clicked" : (btnMouse.containsMouse ? "hover" : "default"))
    states: [
        State {
            name: "default"
            PropertyChanges {
                target: _icon
                source: path + icons[eNORMAL]
            }
        },
        State {
            name: "hover"
            PropertyChanges {
                target: _icon
                source: path + icons[eHOVER]
            }
        },
        State {
            name: "disable"
            PropertyChanges {
                target: _icon
                source: path + icons[eDISABLE]
            }
        },
        State {
            name: "clicked"
            PropertyChanges {
                target: _icon
                source: path + icons[eCLICKED]
            }
        }
    ]
}
