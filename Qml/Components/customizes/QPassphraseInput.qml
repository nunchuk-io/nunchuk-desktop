import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.12
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import "../customizes"
import "../origins"
Rectangle {
    color: "Transparent"
    radius: 8
    property alias textInputted : confirmpassphrases.textInputted
    property alias valid        : confirmpassphrases.isValid
    property alias errorText    : confirmpassphrases.errorText
    signal sendPassphraseClicked(var passphrase)
    signal closeClicked()
    MouseArea {anchors.fill: parent; onClicked: {}}
    Rectangle {
        id: mask
        width: 300
        height: 250
        radius: 8
        visible: false
    }

    Rectangle {
        id: content
        width: 300
        height: 250
        color: "#F1FAFE"
        radius: 8
        anchors.centerIn: parent
        visible: false
    }

    OpacityMask {
        id: opacityMaskPassphrase
        anchors.fill: content
        source: content
        maskSource: mask
        QText {
            anchors {
                horizontalCenter: parent.horizontalCenter
                top: parent.top
                topMargin: 24
            }
            text: "Enter your passphrase"
            color: "#031F2B"
            font.family: "Montserrat"
            font.weight: Font.Bold
            font.pixelSize: 16
        }

        QCloseButton {
            anchors {
                right: parent.right
                rightMargin: 16
                top: parent.top
                topMargin: 16
            }
            onClicked: {
                confirmpassphrases.textInputted = ""
                confirmpassphrases.isValid = true
                confirmpassphrases.errorText = ""
                closeClicked()
            }
        }

        QTextInputBoxTypeB {
            id: confirmpassphrases
            label: ""
            boxWidth: 252
            boxHeight: 48
            echoMode: TextInput.Password
            anchors {
                horizontalCenter: parent.horizontalCenter
                bottom: parent.bottom
                bottomMargin: 100
            }
            onTypingFinished: {
                confirmpassphrases.isValid = true
                confirmpassphrases.errorText = ""
            }
        }

        QTextButton {
            width: 200
            height: 48
            anchors {
                horizontalCenter: parent.horizontalCenter
                bottom: parent.bottom
                bottomMargin: 24
            }
            label.text: "Send"
            label.font.pixelSize: 16
            type: eTypeA
            enabled: confirmpassphrases.textInputted !== ""
            onButtonClicked: sendPassphraseClicked(confirmpassphrases.textInputted)
        }
    }

    DropShadow {
        anchors.fill: opacityMaskPassphrase
        horizontalOffset: 3
        verticalOffset: 3
        radius: 8.0
        samples: 17
        color: "#80000000"
        source: opacityMaskPassphrase
    }
}
