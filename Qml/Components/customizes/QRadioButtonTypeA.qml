import QtQuick 2.4
import "../origins"

Row {
    id: radioRoot
    spacing: 8
    property bool selected: false
    property string label: ""
    property string fontFamily: "Lato"
    property int fontPixelSize: 14
    property int fontWeight: Font.Normal
    signal buttonClicked()
    QImage {
        id: icon
        width: 24
        height: 24
        source: radioRoot.selected ? "qrc:/Images/Images/RadioEnabled.png" : "qrc:/Images/Images/RadioDeselected.png"
        anchors.verticalCenter: parent.verticalCenter
        MouseArea {
            id: mouse
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor
            onClicked: { buttonClicked() }
        }
    }

    QText {
        id: text
        text: label
        font.family: fontFamily
        font.pixelSize: fontPixelSize
        font.weight: fontWeight
        color: "#031F2B"
        anchors.verticalCenter: parent.verticalCenter
    }
}
