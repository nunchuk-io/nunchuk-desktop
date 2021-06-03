import QtQuick 2.4
import HMIEVENTS 1.0
import "../origins"

Rectangle {
    id: homestep1
    width: 976
    height: 910
    color: "#F1FAFE"

    QImage {
        width: 640
        height: 640
        anchors.centerIn: parent
        source: "qrc:/Images/Images/Logo_bgr.png"
    }

    QImage {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 335
        source: "qrc:/Images/Images/Step2.png"
    }


    QText {
        width: 255
        height: 36
        text: "Welcome to Nunchuk"
        color: "#000000"
        font.weight: Font.DemiBold
        font.family: "Montserrat"
        font.pixelSize: 24
        horizontalAlignment: Text.AlignHCenter
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: parent.top
            topMargin: 143
        }
    }

    QText {
        width: 378
        height: 48
        text: "Congrats on your first signers! Now you can create a new wallet using the signers you have added."
        color: "#000000"
        font.family: "Lato"
        font.pixelSize: 16
        wrapMode: Text.WordWrap
        horizontalAlignment: Text.AlignHCenter
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: parent.top
            topMargin: 187
        }
    }

    QButtonMedium {
        width: 160
        height: 32
        label: "Add New Wallet"
        type: eOUTLINE_NORMAL
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: parent.top
            topMargin: 267
        }
        onButtonClicked : {
            QMLHandle.sendEvent(EVT.EVT_HOME_ADD_WALLET_REQUEST)
        }
    }
}
