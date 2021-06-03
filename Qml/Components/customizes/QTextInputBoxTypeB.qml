import QtQuick 2.4
import QtGraphicalEffects 1.0
import "../origins"

Column {
    id: textipboxType
    property string label: "This is label"
    property string errorText: "Error contents"
    property alias  textInputted: input.text
    property var   textweight: Font.Bold
    property alias echoMode: input.echoMode
    property alias maxLength: input.maximumLength
    property alias length: input.length
    property bool enableLengthLimit: false
    property int boxWidth: 338
    property int boxHeight: 48
    property bool isValid: true
    signal typingFinished(var currentText)
    property alias textBoxFocus: input.focus
    spacing: 4
    Row {
        width: parent.width
        QText {
            width: textipboxType.width/2
            font.family: "Lato"
            font.pixelSize: 16
            font.weight: textweight
            color: "#031F2B"
            text: label
            anchors.bottom: parent.bottom
        }

        QText {
            width: textipboxType.width/2
            font.family: "Lato"
            font.pixelSize: 12
            color: "#031F2B"
            text: textipboxType.length + "/" + textipboxType.maxLength
            visible: textipboxType.enableLengthLimit
            horizontalAlignment: Text.AlignRight
            anchors.bottom: parent.bottom
        }
    }

    Item {
        width: myRectangle.width+2
        height: myRectangle.height+2
        Item {
            id: background
            width: myRectangle.width+2
            height: myRectangle.height+2
            Rectangle {
                id: myRectangle
                anchors.centerIn: parent
                width: boxWidth
                height: boxHeight
                radius: 8
                color: isValid ? "#FFFFFF" : "#FFD7D9"
                border.color:  isValid ? "#DEDEDE" : "#CF4018"
            }
        }

        InnerShadow {
            anchors.fill: background
            cached: true
            horizontalOffset: 0
            verticalOffset: 0
            radius: 8
            samples: 16
            color: Qt.rgba(3, 31, 43, 0.14)
            smooth: true
            source: background

            QTextEdit {
                id: input
                anchors {
                    fill: parent
                    leftMargin: 5
                    rightMargin: 5
                }
                color: "#031F2B"
                font.pixelSize: 16
                clip: true
                onTypingFinished: textipboxType.typingFinished(currentText)
            }
        }
    }
    QText {
        width: myRectangle.width+2
        font.family: "Lato"
        font.pixelSize: 16
        color: "#CF4018"
        text: errorText
        visible: !isValid
    }
}
