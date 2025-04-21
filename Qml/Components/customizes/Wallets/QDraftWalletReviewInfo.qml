import QtQuick 2.12
import HMIEVENTS 1.0
import NUNCHUCKTYPE 1.0
import "../../../Components/origins"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

Rectangle {
    width: 352
    height: 396
    radius: 16
    color: "#F5F5F5"
    readonly property var types: [
        {type: NUNCHUCKTYPE.NATIVE_SEGWIT,   displayName: STR.STR_QML_062 },
        {type: NUNCHUCKTYPE.TAPROOT,         displayName: STR.STR_QML_553 },
        {type: NUNCHUCKTYPE.NESTED_SEGWIT,   displayName: STR.STR_QML_063 },
        {type: NUNCHUCKTYPE.LEGACY,          displayName: STR.STR_QML_064 },
    ]
    property var infos: [
        {label: STR.STR_QML_025,  labelValue: newWalletInfo.walletName },
        {label: STR.STR_QML_1639, labelValue: qsTr("%1/%2").arg(newWalletInfo.walletM).arg(newWalletInfo.walletN) },
        {label: STR.STR_QML_066,  labelValue: types.find(function(e) {return e.type === newWalletInfo.walletAddressType }).displayName },
    ]
    Column {
        width: 304
        spacing: 12
        anchors {
            left: parent.left
            leftMargin: 24
            top: parent.top
            topMargin: 16
        }
        Repeater {
            model: infos
            Item {
                width: parent.width
                height: 28
                QLato {
                    anchors {
                        left: parent.left
                        verticalCenter: parent.verticalCenter
                    }
                    text: modelData.label
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }

                QLato {
                    anchors {
                        right: parent.right
                        verticalCenter: parent.verticalCenter
                    }
                    text: modelData.labelValue
                    font.weight: Font.Bold
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }
    }
}
