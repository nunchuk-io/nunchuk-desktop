import QtQuick 2.0
import "../../origins"

Rectangle {
    property alias text: _txt.text
    property bool iconInRight: false
    property int iconSize: 16
    property int fontSize: 16
    property alias font: _txt.font
    property string icon: ""
    width: _row.implicitWidth + 20
    height: 24
    radius: 20
    Row {
        id:_row
        anchors.centerIn: parent
        spacing: 4
        layoutDirection: iconInRight ?  Qt.RightToLeft : Qt.LeftToRight
        QImage {
            width: iconSize
            height: iconSize
            source: icon
            visible: icon != ""
        }
        QLato {
            id: _txt
            font.weight: Font.Bold
            font.pixelSize: fontSize
            anchors.verticalCenter: parent.verticalCenter
        }
    }
}
