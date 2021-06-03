import QtQuick 2.4
import "../origins"

Item {
    id: progresBar
    width: 420
    height: 62
    property int percentage: 50

    Rectangle {
        id: barbg
        width: parent.width
        height: 8
        radius: 8
        color: "#323E4A"
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 3

        Rectangle {
            id: progressbg
            width: percentage*parent.width/100
            height: 8
            radius: 8
            anchors.left: parent.left
            color: "#F6D65D"
        }
    }

    Image {
        id: name
        source: "qrc:/Images/Images/snipbar.png"
        width: 32
        height: 62
        x: progressbg.width - width/2
        QText {
            width: 32
            height: 10
            anchors.top: parent.top
            anchors.topMargin: 11
            font.weight: Font.Bold
            font.family: "Lato"
            font.pixelSize: 10
            text: percentage + " %"
            color: "#031F2B"
            horizontalAlignment: Text.AlignHCenter
        }
    }
}
