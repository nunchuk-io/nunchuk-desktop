import QtQuick 2.4
import QtQuick.Controls 2.0
import HMIEVENTS 1.0
import "../Components/customizes"
import "../Components/origins"
QScreen {
    MouseArea {anchors.fill: parent}

    Column {
        anchors.centerIn: parent
        spacing: 20
        QText {
            id: name
            text: qsTr("Please wait")
        }
        BusyIndicator {
            width: 100
            height: 100
            anchors.horizontalCenter: name.horizontalCenter
            running: true
        }
    }

}
