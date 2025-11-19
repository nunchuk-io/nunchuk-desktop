import QtQuick 2.0
import "../../origins"
import "../../../../localization/STR_QML.js" as STR
Item {
    id: _item
    property alias icon: _icon
    property alias label: _label
    property alias edit: _edit
    property alias input: _input
    property string textColor: "#FFFFFF"
    property var echoMode: TextInput.Normal
    signal textEditClicked()
    height: 84
    Column {
        anchors.fill: parent
        spacing: 12
        Item {
            width: parent.width
            height: 24
            Row {
                anchors.fill: parent
                spacing: 12
                QImage {
                    id: _icon
                    width: sourceSize.width
                    height: sourceSize.height
                    anchors.verticalCenter: parent.verticalCenter
                    visible: source !== ""
                }
                QLato {
                    id: _label
                    font.weight: Font.Bold
                    font.pixelSize: 16
                    color: textColor
                    text: STR.STR_QML_848
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
            Row {
                anchors.fill: parent
                layoutDirection: Qt.RightToLeft
                spacing: 12
                QTextLink {
                    id: _edit
                    width: 29
                    height: 20
                    text: STR.STR_QML_849
                    color: textColor
                    anchors.verticalCenter: parent.verticalCenter
                    onTextClicked: {
                        textEditClicked()
                    }
                }
            }
        }
        QTextField {
            id: _input
            width: parent.width
            height: 48
            leftPadding: 12
            wrapMode: Text.WordWrap
            echoMode: _item.echoMode
        }
    }
}
