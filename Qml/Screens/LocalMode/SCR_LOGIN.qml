import QtQuick 2.12
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0
import HMIEVENTS 1.0
import EWARNING 1.0
import QRCodeItem 1.0
import NUNCHUCKTYPE 1.0
import "../../Components/customizes"
import "../../Components/origins"
QScreen {
    Image {
        id: bug
        source: "qrc:/Images/Images/Default.png"
        sourceSize: Qt.size(parent.width, parent.height)
        smooth: true
        visible: false
    }

    GaussianBlur {
        anchors.fill: bug
        source: bug
        radius: 12
        samples: 12
    }

    Rectangle {
        width: 1164
        height: 800
        anchors.centerIn: parent
        color: "#F1FAFE"
        radius: 4
        Image {
            width: 522
            height: 522
            anchors.centerIn: parent
            source: "qrc:/Images/Images/Logo_bgr.png"
        }

        Item {
            width: 448
            height: 262
            anchors.centerIn: parent

            QText {
                id: title
                width: 312
                height: 72
                text: "Please Enter Your Passphrase"
                wrapMode: Text.WordWrap
                font.family: "Montserrat"
                font.pixelSize: 24
                font.weight: Font.DemiBold
                color: "#031F2B"
                horizontalAlignment: Text.AlignHCenter
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: 40
            }

            QTextInputBox {
                id: passInputted
                width: 248
                heightMin: 56
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: title.bottom
                anchors.topMargin: 24
                border.color: "#C9DEF1"
                placeholder.text: "Enter Passphrase"
                echoMode: TextInput.Password
                errorText.text: "The Passphrase you entered is incorrect."
                onTypingFinished : passInputted.validInput = true
                Keys.onEnterPressed: {
                    if(passInputted.textOutput !== ""){
                        QMLHandle.sendEvent(EVT.EVT_LOGIN_PERFORM_PASSWORD_REQUEST, passInputted.textOutput)
                    }
                }
            }

            QTextButton {
                id: changepassbtn
                width: 248
                height: 48
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: passInputted.bottom
                anchors.topMargin: 25
                label.text: "Enter The Dragon"
                label.font.pixelSize: 14
                type: eTypeA
                enabled: (passInputted.textOutput !== "") && (passInputted.length >= 8)
                onButtonClicked: {
                    QMLHandle.sendEvent(EVT.EVT_LOGIN_PERFORM_PASSWORD_REQUEST, passInputted.textOutput)
                }
            }
        }

        Connections {
            target: AppModel
            onLoginResult : {
                if(!succeed){
                    passInputted.validInput = false
                }
            }
        }
    }
}
