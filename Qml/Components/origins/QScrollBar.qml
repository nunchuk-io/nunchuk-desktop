import QtQuick
import QtQuick.Controls

ScrollBar {
    id: control
    width: 8
    policy: ScrollBar.AsNeeded
    // topPadding / bottomPadding: set at usage site to match the container's
    // border-radius so the track and thumb don't paint into rounded corners.
    // Defaults to 0 (no inset); override per usage: QScrollBar { topPadding: 12; bottomPadding: 12 }
    //
    // Visibility: both track and thumb bind to control.size explicitly.
    // policy: AsNeeded alone is unreliable — the C++ size binding may lag or
    // return 0 during initialisation.
    //   size > 0   — guard against uninitialised (size=0) state
    //   size < 1.0 — content overflows the viewport → scrolling is needed
    background: Item {
        Rectangle {
            x: 0
            y: control.topPadding
            width: parent.width
            height: parent.height - control.topPadding - control.bottomPadding
            color: "#EAEAEA"
            radius: 4
            visible: control.size > 0 && control.size < 1.0
        }
    }
    contentItem: Item {
        Rectangle {
            anchors.fill: parent
            radius: 4
            color: "#BDBDBD"
            visible: control.size > 0 && control.size < 1.0
        }
    }
}
