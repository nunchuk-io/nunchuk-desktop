import QtQuick 2.4
import QtGraphicalEffects 1.0
import "../origins"

Rectangle {
    id: wldlg
    width: 304
    height: 64
    color: walletmouse.containsMouse ? Qt.rgba(255, 255, 255, 0.1) : "transparent"

    property bool isCurrentIndex: false
    property bool isEscrow: true
    property string walletName : "Name"
    property string walletBalance: "0.0000000"
    property string walletM: "0"
    property string walletN: "0"

    Rectangle {
        width: parent.width
        height: 1
        color: "#C9DEF1"
        opacity: 0.1
        anchors.bottom: parent.bottom
    }

    Rectangle {
        id: indi
        width: 8
        height: parent.height
        color: "#F6D65D"
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        visible: isCurrentIndex
    }

    Column {
        width: 216
        height: isEscrow ? 53 : 37
        anchors.left: parent.left
        anchors.leftMargin: 24
        anchors.verticalCenter: parent.verticalCenter
        Item {
            width: 64
            height: isEscrow ? 16 : 0
            visible: isEscrow
            QImage {
                id: escIco
                width: 16
                height: 16
                anchors {
                    left: parent.left
                    verticalCenter: parent.verticalCenter
                }
                source: "qrc:/Images/Images/Escrow Wallet.png"
            }

            QText {
                width: 47
                height: 10
                anchors {
                    left: escIco.right
                    leftMargin: 3
                    verticalCenter: escIco.verticalCenter
                }
                text: "ESCROW"
                color: "#F6D65D"
                font.pixelSize: 10
                font.letterSpacing: 0.05
                lineHeight: 10
            }
        }

        QText {
            id: idWalletName
            width: 216
            height: 21
            text: wldlg.walletName
            color: "#F1FAFE"
            font.weight: Font.Bold
            elide: Text.ElideRight
            font.pixelSize: 14
            lineHeight: 21
        }
        QText {
            id: idWalletBalabce
            width: 208
            height: 16
            text: walletBalance + ((AppSetting.unit === 1) ? " sat" : " BTC")
            color: "#F1FAFE"
            font.pixelSize: 12
            lineHeight: 16
        }
    }

    Row {
        height: 21
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: 21.5
        spacing: 8

        QText {
            id: id_walletM
            text: walletM
            color: "#F7F7F9"
            font.pixelSize: 16
            lineHeight: 21
            font.weight: Font.DemiBold
            horizontalAlignment: Text.AlignLeft
            anchors.verticalCenter: parent.verticalCenter
        }
        Rectangle {
            width: 1
            height: 16
            color: "#F7F7F9"
            anchors.verticalCenter: parent.verticalCenter
        }
        QText {
            id: id_walletN
            text: walletN
            color: "#F6D65D"
            font.pixelSize: 16
            lineHeight: 21
            font.weight: Font.DemiBold
            horizontalAlignment: Text.AlignRight
            anchors.verticalCenter: parent.verticalCenter
        }
    }


    Rectangle {
        anchors.fill: parent
        color: Qt.rgba(255, 255, 255, 0.3)
        visible: isCurrentIndex
    }

    signal buttonClicked()
    MouseArea {
        id: walletmouse
        hoverEnabled: true
        anchors.fill: parent
        onClicked: wldlg.buttonClicked()
    }
}
