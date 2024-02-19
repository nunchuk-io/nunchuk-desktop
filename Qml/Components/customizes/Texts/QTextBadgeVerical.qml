import QtQuick 2.0
import "../../../Components/customizes/Members"

Column {
    id: _id
    property string role: ""
    property string label: ""
    spacing: 4
    QLato {
        height: 20
        text: _id.label
        font.pixelSize: 16
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
    QMemberRole {
        height: 16
        role: _id.role
        font.weight: Font.Bold
    }
}
