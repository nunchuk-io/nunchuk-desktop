import QtQuick 2.0
import "../customizes/Texts"
import "../customizes/Buttons"
import "../../../localization/STR_QML.js" as STR

Rectangle {
    property string addText: ""
    property string addTitle: ""
    width: 289
    height: 124
    radius: 8
    color: "#D0E2FF"
    signal add();
    signal cancel();
    Item {
        anchors.fill: parent
        anchors.margins: 16
        QLato {
            anchors {
                top: parent.top
                topMargin: 0
            }
            width: 257
            height: paintedHeight
            text: addTitle
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            wrapMode: Text.WordWrap
            lineHeight: 28
            lineHeightMode: Text.FixedHeight
        }
        Item {
            anchors {
                bottom: parent.bottom
                bottomMargin: 0
            }
            width: 257
            height: 48
            QTextButton {
                width: label.paintedWidth + 12*2
                height: 48
                anchors.left: parent.left
                label.text: STR.STR_QML_245
                label.font.pixelSize: 16
                type: eTypeM
                onButtonClicked: {
                    cancel()
                }
            }
            QTextButton {
                width: label.paintedWidth + 12*2
                height: 48
                anchors.right: parent.right
                label.text: addText
                label.font.pixelSize: 16
                type: eTypeN
                onButtonClicked: {
                    add()
                }
            }
        }

    }
}
