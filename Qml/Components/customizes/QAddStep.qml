/**************************************************************************
 * This file is part of the Nunchuk software (https://nunchuk.io/)        *
 * Copyright (C) 2020-2022 Enigmo								          *
 * Copyright (C) 2022 Nunchuk								              *
 *                                                                        *
 * This program is free software; you can redistribute it and/or          *
 * modify it under the terms of the GNU General Public License            *
 * as published by the Free Software Foundation; either version 3         *
 * of the License, or (at your option) any later version.                 *
 *                                                                        *
 * This program is distributed in the hope that it will be useful,        *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 * GNU General Public License for more details.                           *
 *                                                                        *
 * You should have received a copy of the GNU General Public License      *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
 *                                                                        *
 **************************************************************************/
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
