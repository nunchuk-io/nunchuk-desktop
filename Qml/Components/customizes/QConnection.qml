import QtQuick 2.4
import QtGraphicalEffects 1.0
import NUNCHUCKTYPE 1.0
import "../origins"

Rectangle {
    property alias label: text
    property alias indicator: ico
    border.width: 1
    border.color: "#C9DEF1"
    color: Qt.rgba(255, 255, 255, 0.1)
    radius: 25

    Component {
        id: colorindicator
        Item {
            width: 12
            height: 12
            Rectangle {
                width: 8
                height: 8
                radius: 8
                anchors.centerIn: parent
                color: AppSetting.connectionState === NUNCHUCKTYPE.ONLINE ? ((AppSetting.primaryServer === 0) ? "#35ABEE" : "#EEFF41")
                                                                          : "#E02247"
            }
        }
    }

    Component {
        id: syncindicator
        Item {
            width: 12
            height: 12
            QImage {
                id: syncingIco
                width: 12
                height: 12
                transformOrigin: Item.Center
                anchors.centerIn: parent
                source: "qrc:/Images/Images/syncing.png"
            }

            NumberAnimation {
                target: syncingIco
                property: "rotation"
                duration: 500
                from: 0
                to: 360
                loops:Animation.Infinite
                running: true
            }
        }
    }

    Row {
        height: parent.height
        spacing: 8
        anchors.centerIn: parent

        Loader {
            id: ico
            anchors.verticalCenter: parent.verticalCenter
            sourceComponent: AppSetting.connectionState === NUNCHUCKTYPE.SYNCING ? syncindicator : colorindicator
        }

        QText {
            id: text
            height: parent.height
            color: "#C9DEF1"
            font.pixelSize: 10
            font.weight: Font.ExtraBold
            font.family: "Lato"
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    QText {
        height: 10
        color: "#F1FAFE"
        font.pixelSize: 8
        font.family: "Lato"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.bottom
        text: "Syncing"  + (AppSetting.syncPercent > 0 ? (" " + AppSetting.syncPercent + "%") : " ...")
        visible: AppSetting.connectionState === NUNCHUCKTYPE.SYNCING
    }
}
