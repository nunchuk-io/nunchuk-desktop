import QtQuick 2.4
import QtGraphicalEffects 1.12
import "../../origins"
import "../../customizes/Texts"
import "../../customizes/Buttons"

Item {
    property rect areaRect
    property bool ready: false
    onAreaRectChanged: {
        ready = true;
    }
    Item {
        id: captureArea
        anchors.centerIn: parent
        width: areaRect.width
        height: areaRect.height
        visible: ready
        onHeightChanged: {
            toTopAnimation.to = 5
            toBottomAnimation.to = captureArea.height - 5
            scanIndicatorAnimation.start()
        }
        readonly property int  iconsize: 20
        QImage {source: "qrc:/Images/Images/C1.png"; anchors.right: parent.right; width: captureArea.iconsize; height: captureArea.iconsize}
        QImage {source: "qrc:/Images/Images/C2.png"; anchors.left: parent.left; width: captureArea.iconsize; height: captureArea.iconsize}
        QImage {source: "qrc:/Images/Images/C3.png"; anchors.left: parent.left; anchors.bottom: parent.bottom; width: captureArea.iconsize; height: captureArea.iconsize}
        QImage {source: "qrc:/Images/Images/C4.png"; anchors.right: parent.right; anchors.bottom: parent.bottom; width: captureArea.iconsize; height: captureArea.iconsize}
        Rectangle {
            id: scanIndicator
            width: parent.width
            height: 2
            anchors.horizontalCenter: parent.horizontalCenter
            color: "red" // "#218165"
            SequentialAnimation {
                id: scanIndicatorAnimation
                loops: Animation.Infinite
                PropertyAnimation {
                    id: toTopAnimation
                    target: scanIndicator
                    property: "y"
                    duration: 2000
                }
                PropertyAnimation {
                    id: toBottomAnimation
                    target: scanIndicator
                    property: "y"
                    duration: 2000
                }
            }
        }

        RectangularGlow {
            id: effect
            width: scanIndicator.width*2 / 3
            height: scanIndicator.height
            anchors.centerIn: scanIndicator
            glowRadius: 50
            spread: 0.2
            color: "red" // "#218165"
            cornerRadius: glowRadius
        }
    }
}
