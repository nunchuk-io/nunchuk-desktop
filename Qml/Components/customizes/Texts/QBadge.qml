import QtQuick 2.0
import "../../origins"

Rectangle {
    property alias text: _txt.text
    property bool iconInRight: false
    property int iconSize: 16
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
            font.weight: Font.Medium
            font.pixelSize: 12
            anchors.verticalCenter: parent.verticalCenter

        }
    }


}
