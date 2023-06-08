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
        Column {
            spacing: 16
            QLato {
                width: 257
                height: 28
                text: addTitle
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }
            Row {
                spacing: 16
                QTextButton {
                    width: 120
                    height: 48
                    label.text: STR.STR_QML_245
                    label.font.pixelSize: 16
                    type: eTypeM
                    onButtonClicked: {
                        cancel()
                    }
                }
                QTextButton {
                    width: 120
                    height: 48
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
}
