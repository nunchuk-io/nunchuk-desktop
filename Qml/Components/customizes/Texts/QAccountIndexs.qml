import QtQuick 2.0
import "../../origins"

Row {
    spacing: 4
    property var accountIndexs: []
    property string walletType: ""
    Repeater {
        model: accountIndexs
        Rectangle {
            width: accttext.width + 10
            height: 16
            color: "#EAEAEA"
            visible: {
                if (walletType === "MULTI_SIG") {
                    return (modelData > 0)
                } else {
                    return true
                }
            }
            radius: 8
            QLato {
                id: accttext
                font.pixelSize: 10
                anchors.centerIn: parent
                font.weight: Font.Bold
                text: qsTr("Acct %1").arg(modelData)
            }
        }
    }
}