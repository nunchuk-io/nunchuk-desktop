import QtQuick 2.0
import "../../../Components/origins"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../Components/customizes/QRCodes"
import "../../../../localization/STR_QML.js" as STR
Item {
    id: _item
    property string description: "input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input "
    property string option: "MONTHLY"
    height: _col.childrenRect.height
    readonly property var repeats: [
        {id: "DAILY",             frequency: STR.STR_QML_1097 },
        {id: "WEEKLY",            frequency: STR.STR_QML_1098 },
        {id: "MONTHLY",           frequency: STR.STR_QML_1099 },
        {id: "THREE_MONTHLY",     frequency: STR.STR_QML_1100 },
        {id: "SIX_MONTHLY",       frequency: STR.STR_QML_1101 },
        {id: "YEARLY",            frequency: STR.STR_QML_1102 },
    ]
    Column {
        id: _col
        Item {
            width: parent.width
            height: 24
            QLato {
                font.weight: Font.Bold
                font.pixelSize: 16
                text: STR.STR_QML_1096
                anchors.verticalCenter: parent.verticalCenter
            }
        }
        Repeater {
            model: repeats
            QRadioButtonTypeA {
                width: 346
                height: 48
                label: modelData.frequency
                selected: option === modelData.id
                onButtonClicked: { option = modelData.id; }
            }
        }
    }
}
