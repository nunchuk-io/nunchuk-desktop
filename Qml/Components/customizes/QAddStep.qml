import QtQuick 2.4
import "../origins"

Row {
    width: 178
    height: 31
    property int step: 0
    property string stepName: "stepName"
    property int currentStep: 0

    property bool stepAlready: step < currentStep
    spacing: 8

    Loader {
        width: 24
        height: 24
        anchors.verticalCenter: parent.verticalCenter
        sourceComponent: stepAlready ? stepCompleted : stepOnGoing
    }

    Component {
        id: stepCompleted
        Image { source: "qrc:/Images/Images/StepComplete.png" }
    }
    Component {
        id: stepOnGoing
        Rectangle {
            visible: !stepAlready
            width: 24
            height: 24
            radius: 24
            color: currentStep === step ?"#031F2B":"transparent"
            border.color: currentStep === step ?"transparent" :"#839096"
            border.width: 2
            QText {
                anchors.centerIn: parent
                color: currentStep === step ? "#F6D65D" :"#839096"
                font.pixelSize: 14
                font.weight: Font.DemiBold
                text: step
            }
        }
    }
    Column {
        anchors.verticalCenter: parent.verticalCenter
        QText {
            color: stepAlready ? "#35ABEE" : currentStep === step ? "#323E4A"  : "#839096"
            font.pixelSize: 10
            text: "STEP " + step
            height: 10
        }
        QText {
            color: stepAlready ? "#35ABEE" : currentStep === step ? Qt.rgba(0, 0, 0, 0.87) : "#839096"
            font.pixelSize: 14
            text: stepName
            font.weight: Font.Bold
            height: 21
        }
    }
}
