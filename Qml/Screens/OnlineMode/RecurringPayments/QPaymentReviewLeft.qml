import QtQuick 2.0
import "../../../Components/origins"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../Components/customizes/QRCodes"
import "../../../Components/customizes/Popups"
import "../../../../localization/STR_QML.js" as STR
Item {
    id: _item
    height: _col.childrenRect.height
    readonly property var fee_rates: [
        {id: "PRIORITY" , fee_rate: STR.STR_QML_1108},
        {id: "STANDARD" , fee_rate: STR.STR_QML_1110},
        {id: "ECONOMY"  , fee_rate: STR.STR_QML_1112}
    ]
    property string myRole: GroupWallet.dashboardInfo.myRole
    Column {
        id: _col
        spacing: 16
        QTextInputBoxTypeB {
            id: _fee_rate
            label: STR.STR_QML_1127
            boxWidth: left_width
            boxHeight: 48
            textInputted: fee_rates.find(function(e) {if (e.id === payment.fee_rate) return true; else return false}).fee_rate
            enabled: false
        }

        Column {
            spacing: 4
            QLato {
                font.weight: Font.Bold
                font.pixelSize: 16
                text: STR.STR_QML_1125
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }
            QLato {
                width: left_width
                text: STR.STR_QML_1126
                lineHeightMode: Text.FixedHeight
                lineHeight: 20
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }
        }

        QImportedFirstAddressOfWallet
        {
            isView: true
            width: left_width
            address_width: 257
            qrCanClick: true
            address: payment.destination_payload.first_address
            onQrClicked: {
                _exportAddress.address = address
                _exportAddress.open()
            }
        }

        Repeater {
            model: payment.destination_payload.addresses
            QImportedFirstAddressOfWallet
            {
                isView: true
                address: modelData
                label: STR.STR_QML_1128
                showLabel: index == 0
                width: left_width
                address_width: 257
                qrCanClick: true
                onQrClicked: {
                    _exportAddress.address = address
                    _exportAddress.open()
                }
            }
        }

        QImportedConfigurationDetails
        {
            isView: true
            width: left_width
            visible: payment.destination_type === "DESTINATION_WALLET" && myRole !== "KEYHOLDER"
            description: payment.destination_payload.bsms
        }
    }

}
