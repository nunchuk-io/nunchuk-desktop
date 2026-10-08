import QtQuick 2.0
import "../../../Components/origins"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/QRCodes"
import "../../../../localization/STR_QML.js" as STR
Item {
    id: _item
    property bool isView: false
    property string label: STR.STR_QML_1091
    property string address: "bc1qft5swsj3nnm48qcpwvsqmht899gf6zk28tvvq6pdkjekaq3rltaqkztttq"
    property int address_width: 482
    property bool showLabel: true
    property bool qrCanClick: false
    signal qrClicked(var address)
    height: _col.childrenRect.height
    Column {
        id: _col
        spacing: 12
        Item {
            visible: showLabel
            width: parent.width
            height: 24
            QLato {
                font.weight: Font.Bold
                font.pixelSize: isView ? 12 : 16
                text: label
                anchors.verticalCenter: parent.verticalCenter
            }
        }
        Rectangle {
            width: _item.width
            height: 80
            color: "#F5F5F5"
            radius: 12
            Row {
                anchors.fill: parent
                anchors.margins: 16
                spacing: 12
                QQrIcon {
                    iconSize: 48
                    offset: 8
                    address: _item.address
                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: qrCanClick ? Qt.PointingHandCursor: Qt.CursorShape
                        onClicked: {
                            if (qrCanClick)
                                qrClicked(_item.address)
                        }
                    }
                }
                QLato {
                    width: address_width
                    height: 40
                    text: address
                    lineHeightMode: Text.FixedHeight
                    lineHeight: 20
                    wrapMode: Text.WrapAnywhere
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }
    }
}
