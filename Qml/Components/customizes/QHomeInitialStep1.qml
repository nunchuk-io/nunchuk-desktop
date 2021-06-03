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
        source: "qrc:/Images/Images/Step1.png"
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

    Column {
        spacing: 24
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: parent.top
            topMargin: 190
        }

        QText {
            width: 530
            text: "Start managing your Bitcoin wallets all in one place by adding a new signer."
            color: "#000000"
            font.family: "Lato"
            font.pixelSize: 16
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
        }

//        QText {
//            width: 530
//            text: "NOTE: Air-gapped signers are not managed by Signer Manager. If you use air-gapped signers, please start by adding a new wallet. You can add air-gapped signers as \"remote signers\" as you create a wallet."
//            color: "#000000"
//            font.family: "Lato"
//            font.pixelSize: 16
//            wrapMode: Text.WordWrap
//            horizontalAlignment: Text.AlignHCenter
//        }
    }

    Row {
        width: 336
        height: 32
        spacing: 16
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: parent.top
            topMargin: 300
        }

        QButtonMedium {
            width: 160
            height: 32
            label: "Add New Signer"
            type: eOUTLINE_NORMAL
            onButtonClicked: {
                QMLHandle.sendEvent(EVT.EVT_HOME_ADD_NEW_SIGNER_REQUEST)
            }
        }

        QButtonMedium {
            width: 160
            height: 32
            label: "Add New Wallet"
            type: eOUTLINE_NORMAL
            onButtonClicked : {
                QMLHandle.sendEvent(EVT.EVT_HOME_ADD_WALLET_REQUEST)
            }
        }
    }
}
