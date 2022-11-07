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
import QtGraphicalEffects 1.0
import "../origins"

Rectangle {
    id: rootTextButton
    width: 137
    height: 48
    radius: 24
    border.width: 2
    property alias label: text
    property bool processing: false
    state: !enabled ? "disable" : (btnMouse.pressed ? "clicked" : (btnMouse.containsMouse ? "hover" : "default"))

    property var backgroundColor:   [backgroundColorTypeA,  backgroundColorTypeB,   backgroundColorTypeC,   backgroundColorTypeD,   backgroundColorTypeE,   backgroundColorTypeF]
    property var borderColor:       [borderColorTypeA,      borderColorTypeB,       borderColorTypeC,       borderColorTypeD,       borderColorTypeE,       borderColorTypeF]
    property var textColor:         [textColorTypeA,        textColorTypeB,         textColorTypeC,         textColorTypeD,         textColorTypeE,         textColorTypeF]

    property int type: eTypeA
    readonly property int eTypeA: 0
    readonly property int eTypeB: 1
    readonly property int eTypeC: 2
    readonly property int eTypeD: 3
    readonly property int eTypeE: 4
    readonly property int eTypeF: 5

    // Click state
    readonly property int eNORMAL: 0
    readonly property int eHOVER : 1
    readonly property int eDISABLE: 2
    readonly property int eCLICKED: 3

    property var backgroundColorTypeA: ["#F6D65D", "#96833B", "#9CAEB8", "#B8A048"]
    property var borderColorTypeA: ["transparent", "transparent", "transparent", "#F6D65D"]
    property var textColorTypeA: ["#031F2B", "#C9DEF1", "#C9DEF1", "#031F2B"]

    property var backgroundColorTypeB: ["transparent", "#031F2B", "transparent", "#1A333D"]
    property var borderColorTypeB: ["#031F2B", "#031F2B", "#9CAEB8", "#031F2B"]
    property var textColorTypeB: ["#031F2B", "#F1FAFE", "#9CAEB8", "#F1FAFE"]

    property var backgroundColorTypeC: ["transparent", Qt.rgba(218, 222, 224, 0.1), "transparent", Qt.rgba(218, 222, 224, 0.4)]
    property var borderColorTypeC: ["#F1FAFE", Qt.rgba(241, 250, 254, 0.4), "#F1FAFE", "#F1FAFE"]
    property var textColorTypeC: ["#F1FAFE", "#F1FAFE", "#F1FAFE", "#F1FAFE"]

    property var backgroundColorTypeD: ["transparent", "#9FA0A1","transparent", "#8B8C8C"]
    property var borderColorTypeD: ["#FF7A00", "#FF7A00", "#FF7A00", "#FF7A00"]
    property var textColorTypeD: ["#FF7A00", "#FFFFFF", "#FF7A00", "#FFFFFF"]

    property var backgroundColorTypeE: ["#031F2B", "#1A333D","#EAEAEA", "#031F2B"]
    property var borderColorTypeE: ["#031F2B", "#031F2B", "#EAEAEA", "#031F2B"]
    property var textColorTypeE: ["#FFFFFF", "#FFFFFF", "#595959", "#FFFFFF"]

    property var backgroundColorTypeF: ["#FFFFFF", "#031F2B","#EAEAEA", "#031F2B"]
    property var borderColorTypeF: ["#FFFFFF", "#031F2B", "#EAEAEA", "#595959"]
    property var textColorTypeF: ["#031F2B", "#FFFFFF", "#595959", "#FFFFFF"]

    states: [
        State {
            name: "default"
            PropertyChanges {
                target: rootTextButton
                color: backgroundColor[type][eNORMAL]
                border.color: borderColor[type][eNORMAL]
            }
            PropertyChanges {
                target: text
                color: textColor[type][eNORMAL]
            }
        },
        State {
            name: "hover"
            PropertyChanges {
                target: rootTextButton
                color: backgroundColor[type][eHOVER]
                border.color: borderColor[type][eHOVER]
            }
            PropertyChanges {
                target: text
                color: textColor[type][eHOVER]
            }
        },
        State {
            name: "disable"
            PropertyChanges {
                target: rootTextButton
                color: backgroundColor[type][eDISABLE]
                border.color: borderColor[type][eDISABLE]
            }
            PropertyChanges {
                target: text
                color: textColor[type][eDISABLE]
            }
        },
        State {
            name: "clicked"
            PropertyChanges {
                target: rootTextButton
                color: backgroundColor[type][eCLICKED]
                border.color: borderColor[type][eCLICKED]
            }
            PropertyChanges {
                target: text
                color: textColor[type][eCLICKED]
            }
        }
    ]

    QText {
        id: text
        anchors.fill: parent
        anchors.centerIn: rootTextButton
        text: qsTr("TEXT")
        font.weight: Font.DemiBold
        font.pixelSize: 16
        font.family: "Lato"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    signal buttonClicked()
    MouseArea {
        id: btnMouse
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        anchors.fill: rootTextButton
        onClicked: rootTextButton.buttonClicked()
    }
}
