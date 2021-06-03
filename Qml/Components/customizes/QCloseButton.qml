import QtQuick 2.4
import QtGraphicalEffects 1.0
import "../origins"
MouseArea {
    id: closemouse
    hoverEnabled: true
    cursorShape: Qt.PointingHandCursor
    width: ico.width
    height: ico.height
    property alias icon: ico.source
    QImage {
        id: ico
        width: 32
        height: 32
        scale: closemouse.containsMouse ? 1.1 : 1
        transformOrigin: Item.Center
        source: "qrc:/Images/Images/Close.png"
    }
}
