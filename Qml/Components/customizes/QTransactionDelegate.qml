import QtQuick 2.4
import NUNCHUCKTYPE 1.0
import "../origins"

Rectangle {
    property int transactionstatus: 0
    property string transactionMemo: ""
    property string transactionAmount: "0"
    property string transactionDate: "--/--/---- ##:## ##"
    property int confirmation: 1

    property int timeWidth: 123
    property int statusWidth: 166
    property int memoWidth: 178
    property int amountWidth: 178

    color: btnMouse.containsMouse ?"#C9DEF1" : "transparent"

    Rectangle {
        width: parent.width
        height: 1
        color: "#C9DEF1"
        anchors.bottom: parent.bottom
    }

    Row {
        height: parent.height
        Item {
            id: idstate
            width: statusWidth
            height: parent.height
            Rectangle {
                id: indicator
                width: 4
                height: 24
                anchors.left: idstate.left
                anchors.leftMargin: 16
                anchors.verticalCenter: parent.verticalCenter
                radius: 1
                color: {
                    if(transactionstatus === NUNCHUCKTYPE.PENDING_SIGNATURES){ return "#E02247" }
                    else if(transactionstatus === NUNCHUCKTYPE.READY_TO_BROADCAST){ return "#FF7A00" }
                    else if(transactionstatus === NUNCHUCKTYPE.NETWORK_REJECTED){ return "#031F2B" }
                    else if(transactionstatus === NUNCHUCKTYPE.PENDING_CONFIRMATION){ return "#FF7A00" }
                    else if(transactionstatus === NUNCHUCKTYPE.CONFIRMED){ return "#35ABEE" }
                    else { return "#031F2B" }
                }
            }

            QText {
                font.pixelSize: 12
                font.family: "Lato"
                color: "#323E4A"
                anchors.left: indicator.right
                anchors.leftMargin: 8
                anchors.verticalCenter: indicator.verticalCenter
                text: {
                    if(transactionstatus === NUNCHUCKTYPE.PENDING_SIGNATURES){ return "Pending Signatures" }
                    else if(transactionstatus === NUNCHUCKTYPE.READY_TO_BROADCAST){ return "Ready to Broadcast" }
                    else if(transactionstatus === NUNCHUCKTYPE.NETWORK_REJECTED){ return "Network Rejected" }
                    else if(transactionstatus === NUNCHUCKTYPE.PENDING_CONFIRMATION){ return "Pending Confirmations" }
                    else if(transactionstatus === NUNCHUCKTYPE.CONFIRMED){ return (confirmation + " Confirmations") }
                    else { return "Replaced" }
                }
            }

            QImage {
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                source: "qrc:/Images/Images/Signed.png"
                visible: (transactionstatus === NUNCHUCKTYPE.PENDING_SIGNATURES)
            }
        }

        Item {
            id: blocktime
            width: timeWidth
            height: parent.height
            QText {
                anchors {
                    fill: parent
                    leftMargin: 16
                    rightMargin: 16
                    topMargin: 3
                    bottomMargin: 3
                }
                font.family: "Lato"
                font.pixelSize: 12
                color: (transactionstatus === NUNCHUCKTYPE.REPLACED) || (transactionstatus === NUNCHUCKTYPE.NETWORK_REJECTED) ? "#9CAEB8" : "#323E4A"
                text: transactionDate
                wrapMode: Text.WordWrap
                verticalAlignment: Text.AlignVCenter
            }
        }

        Item {
            width: memoWidth
            height: parent.height
            QText {
                id: memo
                anchors {
                    fill: parent
                    leftMargin: 16
                    rightMargin: 16
                    topMargin: 9
                    bottomMargin: 9
                }
                font.family: "Lato"
                font.pixelSize: 14
                color:  (transactionstatus === NUNCHUCKTYPE.REPLACED) || (transactionstatus === NUNCHUCKTYPE.NETWORK_REJECTED)  ? "#9CAEB8" : "#031F2B"
                text: transactionMemo
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }
        }

        Item {
            width: amountWidth
            height: parent.height
            QText {
                id: amount
                width: amountWidth
                anchors {
                    fill: parent
                    leftMargin: 16
                    rightMargin: 16
                    topMargin: 9
                    bottomMargin: 9
                }
                font.family: "Lato"
                font.pixelSize: 14
                color:  (transactionstatus === NUNCHUCKTYPE.REPLACED) || (transactionstatus === NUNCHUCKTYPE.NETWORK_REJECTED) ? "#9CAEB8" : "#031F2B"
                text: transactionAmount + ((AppSetting.unit === 1) ? " sat" : " BTC")
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignRight
                elide: Text.ElideRight
            }
        }
    }

    signal buttonClicked()
    MouseArea {
        id: btnMouse
        hoverEnabled: true
        anchors.fill: parent
        onClicked: buttonClicked()
    }
}
