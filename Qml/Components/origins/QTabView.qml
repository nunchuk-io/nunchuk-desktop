import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

TabView {
    id:rootTabView
    style: TabViewStyle {
        frameOverlap: 1
        tab: Rectangle {
            id: linh
            color: styleData.selected ? "steelblue" :"lightsteelblue"
            border.color:  "black"
            border.width: 5
            implicitWidth: 300
            implicitHeight: 100
//            implicitWidth: Math.max(text.width + 4, 80)
//            implicitHeight: 20
            radius: 2
            Text {
                id: text
                anchors.centerIn: parent
                text: styleData.title
                color: styleData.selected ? "white" : "Red"
            }
        }
//        frame: Rectangle { color: "steelblue" }
    }
}
