import QtQuick 2.0
import "../../origins"

QLato {
    font.weight: Font.Bold
    font.pixelSize: 16
    scale: btnMouse.containsMouse ? 1.1 : 1
    font.underline: true
    signal textClicked()
    MouseArea {
        id: btnMouse
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        onClicked: {
            textClicked()
        }
    }
}
