import QtQuick 2.0
import "../../../customizes/Texts"
import "../../../../../localization/STR_QML.js" as STR

Column {
    property string description: ""
    width: 627
    spacing: 4
    QLato {
        width: parent.width
        height: 20
        text: STR.STR_QML_1009
        font.weight: Font.DemiBold
        verticalAlignment: Text.AlignVCenter
    }
    QLato {
        width: parent.width
        height: paintedHeight
        text: description
        lineHeightMode: Text.FixedHeight
        lineHeight: 28
        wrapMode: Text.WordWrap
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
    }
}
