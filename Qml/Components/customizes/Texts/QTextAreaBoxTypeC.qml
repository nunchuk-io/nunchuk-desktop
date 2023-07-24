import QtQuick 2.0
import "../../origins"
import "../../../../localization/STR_QML.js" as STR
Item {
    property string icon: ""
    property alias label: _label
    property alias edit: _edit
    property string textColor: "#FFFFFF"
    property string leftText1: ""
    property string rightText1: ""
    property string leftText2: ""
    property string rightText2: ""
    property var rightList: rightText1.split(",")

    signal textEditClicked()
    height: 64 + Math.max(1, rightList.length) * 20
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
                    width: sourceSize.width
                    height: sourceSize.height
                    anchors.verticalCenter: parent.verticalCenter
                    source: icon
                    visible: icon !== ""
                }
                QLato {
                    id: _label
                    font.weight: Font.Bold
                    font.pixelSize: 16
                    color: textColor
                    text: STR.STR_QML_852
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
        Rectangle {
            width: 651
            height: 85 + Math.max(1, rightList.length) * 20
            radius: 12
            color: "#F5F5F5"
            Column {
                anchors.centerIn: parent
                spacing: 16
                Item {
                    width: 619
                    height: Math.max(1, rightList.length) * 20
                    QLato {
                        width: 302
                        height: 20
                        anchors.left: parent.left
                        text: leftText1
                    }
                    Column {
                        anchors.right: parent.right
                        spacing: 1
                        Repeater {
                            model: rightList
                            QLato {
                                width: 302
                                height: 20
                                font.weight: Font.Bold
                                text: rightList[index]
                                color: rightList[index] === "None" ? "#CF4018" : "#031F2B"
                                horizontalAlignment: Text.AlignRight
                            }
                        }
                    }
                }
                Rectangle {
                    width: 311
                    height: 1
                    color: "#EAEAEA"
                }
                Item {
                    width: 619
                    height: 20
                    QLato {
                        width: 302
                        height: 20
                        anchors.left: parent.left
                        text: STR.STR_QML_868
                    }
                    QLato {
                        width: 302
                        height: 20
                        font.weight: Font.Bold
                        anchors.right: parent.right
                        text: rightText2
                        horizontalAlignment: Text.AlignRight
                    }
                }
            }
        }
    }
}
