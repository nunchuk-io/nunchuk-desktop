import QtQuick 2.4
import QtQuick.Controls 2.3
import QtGraphicalEffects 1.0
import "../origins"

Rectangle {
    id: rootPin
    property alias pinInputted: pin.textOutput
    property string warning: ""
    property alias validInput: pin.validInput

    signal senPINClicked()
    signal closeClicked()

    color: Qt.rgba(255, 255, 255, 0.7)
    radius: 8
    MouseArea {anchors.fill: parent; onClicked: {}}
    Rectangle {
        id: busymask
        width: 352
        height: 540
        radius: 8
        visible: false
    }

    Rectangle {
        id: busyboxbg
        width: 352
        height: 540
        color: "#F1FAFE"
        radius: 8
        anchors.centerIn: parent
        visible: false
        Rectangle {
            height: 4
            width: parent.width
            color: "#F6D65D"
        }
    }

    Rectangle {
        width: 352
        height: 540
        radius: 8
        anchors.centerIn: parent
    }

    OpacityMask {
        id: busyboxmask
        anchors.fill: busyboxbg
        source: busyboxbg
        maskSource: busymask

        QCloseButton {
            anchors {
                right: parent.right
                rightMargin: 16
                top: parent.top
                topMargin: 16
            }
            onClicked: {
                closeClicked()
                rootPin.visible = false
            }
        }

        QText {
            id: title
            anchors.top: parent.top
            anchors.topMargin: 48
            anchors.horizontalCenter: parent.horizontalCenter
            font.family: "Montserrat"
            font.pixelSize: 24
            font.weight: Font.Bold
            text: "Your PIN Number"
            color: "#031F2B"
        }

        QTextInputBox {
            id: pin
            width: 264
            height: 56
            anchors.top: title.bottom
            anchors.topMargin: 24
            anchors.horizontalCenter: parent.horizontalCenter
            border.color: "#C9DEF1"
            placeholder.text: "PIN"
            echoMode: TextInput.Password
            mode: eREADONLY_MODE
            errorText.text: warning

            MouseArea {
                id: deletePinBtn
                width: 24
                height: 24
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 8
                enabled: pin.textOutput !== ""
                QImage {
                    anchors.fill: parent
                    source: parent.pressed ? "qrc:/Images/Images/backspace-24px-2.png" :
                                             "qrc:/Images/Images/backspace-24px-1.png"
                }
                onClicked: {
                    pin.textOutput = pin.textOutput.slice(0, -1)
                    pin.validInput = true
                    rootPin.warning = ""
                }
            }
        }

        GridView {
            id: gridPin
            width: 264
            height: 208
            cellWidth: width/3
            cellHeight: height/3
            anchors.top: pin.bottom
            anchors.topMargin: 40
            anchors.horizontalCenter: parent.horizontalCenter
            interactive: false
            layoutDirection: Qt.RightToLeft
            model: 9
            delegate: Item {
                width: gridPin.cellWidth
                height: gridPin.cellHeight
                Rectangle {
                    width: 56
                    height: 56
                    radius: 56
                    anchors.centerIn: parent
                    color: pinMouse.pressed ?"#031F2B" : "transparent"
                    border.color: pinMouse.pressed ? "#F1FAFE" : "#031F2B"
                    Rectangle {
                        width: 8
                        height: 8
                        radius: 8
                        color: pinMouse.pressed ? "#F1FAFE" : "#031F2B"
                        anchors.centerIn: parent
                    }
                }

                MouseArea {
                    id: pinMouse
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    onClicked: {
                        pin.textOutput += (9 - index)
                        pin.validInput = true
                        rootPin.warning = ""
                    }
                }
            }
        }

        QTextButton {
            id: sendPinBtn
            width: gridPin.width - 30
            label.text: "Send PIN"
            type: eTypeA
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 56
            anchors.horizontalCenter: gridPin.horizontalCenter
            enabled: pin.textOutput !== ""
            onButtonClicked: { rootPin.senPINClicked() }
        }
    }

    DropShadow {
        anchors.fill: busyboxmask
        horizontalOffset: 3
        verticalOffset: 3
        radius: 8.0
        samples: 17
        color: "#80000000"
        source: busyboxmask
    }
}
