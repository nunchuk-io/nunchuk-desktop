import QtQuick 2.4
import QtQuick.Controls 2.3
import "../origins"

Item {
    width: 85
    height: 20
    property alias position: control.position
    property alias checked: control.checked

    Row {
        anchors.fill: parent
        spacing: 4
        QText {
            text: "OFF"
            font.pixelSize: 12
            font.family: "Lato"
            color: (0 === control.checked) ? "#031F2B" : "#9CAEB8"
            anchors.verticalCenter: parent.verticalCenter
        }

        Switch {
            id: control
            width: 34
            height: 20
            indicator: Rectangle {
                id: body
                implicitWidth: control.width
                implicitHeight: control.height * (14/20)
                y: parent.height / 2 - height / 2
                radius: 7
                color: control.checked ? "#323E4A" : "#C9DEF1"

                Rectangle {
                    x: control.checked ? parent.width - width : 0
                    y: parent.height / 2 - height / 2
                    width: control.height
                    height: control.height
                    radius: control.height
                    color: control.checked ? "#F6D65D" : "#9CAEB8"
                }
            }
        }

        QText {
            text: "ON"
            font.pixelSize: 12
            font.family: "Lato"
            color: (1 === control.checked) ? "#031F2B" : "#9CAEB8"
            anchors.verticalCenter: parent.verticalCenter
        }
    }
}


