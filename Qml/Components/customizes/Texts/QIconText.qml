import QtQuick 2.0
import "../../origins"

Row {
    id:_row
    property int iconSize: 16
    property string icon: ""
    property alias text: _txt
    spacing: 4
    QImage {
        width: iconSize
        height: iconSize
        source: icon
        visible: icon != ""
        anchors.verticalCenter: parent.verticalCenter
    }
    QLato {
        id: _txt
        font.weight: Font.Normal
        font.pixelSize: 12
        anchors.verticalCenter: parent.verticalCenter
    }
}
