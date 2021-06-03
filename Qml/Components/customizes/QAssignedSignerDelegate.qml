import QtQuick 2.4
import "../origins"

Row {
    width: 216
    height: 56
    spacing: 8
    property string signerName: "value"
    property bool isRemote: false
    property string signerXFP: "value"
    property string lastHealthCheck: "value"
    Rectangle {
        width: 4
        height: parent.height - 2
        color: "#C9DEF1"
        radius: 2
    }

    Column {
        width: 216
        height: 56
        spacing: 4
        Item {
            width: 204
            height: 21
            QText {
                id: signernametext
                width: 140
                height: 21
                font.family: "Montserrat"
                font.weight: Font.DemiBold
                font.pixelSize: 14
                color: "#031F2B"
                text: signerName
                elide: Text.ElideRight
                anchors.verticalCenter: parent.verticalCenter
            }

            Rectangle {
                width: 63
                height: 21
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                color: "#C9DEF1"
                visible: isRemote
                radius: 4
                QText {
                    text: "AIR-GAPPED"
                    font.family: "Lato"
                    font.weight: Font.Bold
                    font.pixelSize: 10
                    anchors.centerIn: parent
                    color: "#031F2B"
                }
            }
        }

        QText {
            width: 204
            height: 16
            font.family: "Lato"
            font.pixelSize: 12
            color: "#031F2B"
            text: "XFP: " + signerXFP
            font.capitalization: Font.AllUppercase
        }
        QText {
            width: 204
            height: 10
            font.family: "Lato"
            font.pixelSize: 10
            color: "#839096"
            text: "Health check: " + lastHealthCheck
        }
    }
}
