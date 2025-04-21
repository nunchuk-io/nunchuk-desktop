import QtQuick 2.0
import "../../../Components/origins"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../Components/customizes/QRCodes"
import "../../../../localization/STR_QML.js" as STR
Item {
    id: _item
    property bool isView: false
    property string label: STR.STR_QML_1092
    property string description: "input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input input "
    height: _col.childrenRect.height
    property bool   expanded: false
    Column {
        id: _col
        spacing: 12
        Item {
            width: parent.width
            height: 24
            QLato {
                font.weight: Font.Bold
                font.pixelSize: isView ? 12: 16
                text: label
                anchors.verticalCenter: parent.verticalCenter
            }
        }
        Rectangle {
            width: _item.width
            height: _txt.lineCount * _txt.lineHeight + 10 + 24 + 12 + 10
            color: "#F5F5F5"
            radius: 12
            QLato {
                id: _txt
                width: _item.width - 2 * 12
                text: description
                lineHeightMode: Text.FixedHeight
                lineHeight: 20
                maximumLineCount: 6
                elide: Text.ElideRight
                wrapMode: Text.WrapAnywhere
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
                anchors {
                    top: parent.top
                    topMargin: 12
                    left: parent.left
                    leftMargin: 12
                }
            }
            Rectangle {
                anchors{
                    left: parent.left
                    leftMargin: 12
                    bottom: parent.bottom
                    bottomMargin: 12
                }

                border.width: 1
                border.color: "#DEDEDE"
                color: "#F5F5F5"
                width: _more.paintedWidth + 8*2
                height: 24
                radius: 24
                QLato{
                    id: _more
                    anchors.centerIn: parent
                    text: expanded ? STR.STR_QML_1421 : STR.STR_QML_1094
                    font.pixelSize: 12
                    scale: _mouse.pressed ? 1.1 : 1.0
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }
                MouseArea {
                    id: _mouse
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    anchors.fill: parent
                    onClicked: {
                        expanded = !expanded
                        if(expanded) {
                            _txt.maximumLineCount = 100
                        }
                        else {
                            _txt.maximumLineCount = 6
                        }
                    }
                }
            }
        }
    }
}
