import QtQuick 2.0
import "../../origins"

Rectangle {
    id: _badge
    property alias  text: _txt.text
    property alias  fontWeight: _txt.font.weight
    property alias  fontColor: _txt.color
    property bool   iconInRight: false
    property int    iconSize: 16
    property int    fontSize: 16
    property alias  font: _txt.font
    property string icon: ""
    property bool   useMouse: false
    property int    topMargin: 4
    property int    leftMargin: 10

    implicitWidth: _row.width + leftMargin*2
    implicitHeight: _row.height + topMargin*2

    radius: 20
    signal badgeClicked()

    Row {
        id: _row
        anchors.centerIn: parent
        spacing: 4
        layoutDirection: iconInRight ?  Qt.RightToLeft : Qt.LeftToRight
        QIcon {
            iconSize: _badge.iconSize
            source: icon
            visible: icon != ""
            anchors.verticalCenter: parent.verticalCenter
        }
        QLato {
            id: _txt
            font.weight: Font.Bold
            font.pixelSize: fontSize
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    MouseArea {
        id: mouse
        visible: useMouse
        anchors.fill: _badge
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        onClicked: {
            badgeClicked()
        }
    }
}
