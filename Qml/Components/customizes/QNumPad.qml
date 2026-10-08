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
import QtQuick.Controls 2.3
import QtGraphicalEffects 1.0
import "../origins"
import "../customizes/Texts"
import "../customizes/Buttons"
import "../../../localization/STR_QML.js" as STR

Item {
    id: rootPin
    width: 352
    height: 540
    property alias pinInputted: pin.textOutput
    property string warning: ""
    property alias validInput: pin.validInput
    property bool  processing: false
    enabled: !processing
    signal senPINClicked()
    signal closeClicked()
    MouseArea {anchors.fill: parent; onClicked: {}}
    Rectangle {
        id: mask
        anchors.fill: parent
        color: "#FFFFFF"
        radius: 24
        layer.enabled: true
        layer.effect: OpacityMask {
            maskSource: Rectangle {
                width: rootPin.width
                height: rootPin.height
                radius: 24
            }
        }
    }
    DropShadow {
        anchors.fill: mask
        horizontalOffset: 3
        verticalOffset: 3
        radius: 8.0
        samples: 17
        color: "#80000000"
        source: mask
    }
    QText {
        id: title
        anchors.top: parent.top
        anchors.topMargin: 48
        anchors.horizontalCenter: parent.horizontalCenter
        font.family: "Montserrat"
        font.pixelSize: 24
        font.weight: Font.Bold
        text: STR.STR_QML_458
        color: "#031F2B"
    }
    QCloseButton {
        anchors {
            right: parent.right
            rightMargin: 16
            top: parent.top
            topMargin: 16
        }
        onClicked: { closeClicked() }
    }
    QTextInputBox {
        id: pin
        width: 264
        height: 56
        anchors.top: title.bottom
        anchors.topMargin: 24
        anchors.horizontalCenter: parent.horizontalCenter
        border.color: "#C9DEF1"
        placeholder.text: "PIN"
        echoMode: TextInput.Password
        mode: eREADONLY_MODE
        errorText.text: warning
        MouseArea {
            id: deletePinBtn
            width: 24
            height: 24
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 8
            enabled: pin.textOutput !== ""
            QIcon {
                iconSize: 24
                anchors.centerIn: parent
                source: parent.pressed ? "qrc:/Images/Images/backspace-24px-2.png" :
                                         "qrc:/Images/Images/backspace-24px-1.png"
            }
            onClicked: {
                pin.textOutput = pin.textOutput.slice(0, -1)
                pin.validInput = true
                rootPin.warning = ""
            }
        }
    }
    GridView {
        id: gridPin
        width: 264
        height: 208
        cellWidth: width/3
        cellHeight: height/3
        anchors.top: pin.bottom
        anchors.topMargin: 40
        anchors.horizontalCenter: parent.horizontalCenter
        interactive: false
        layoutDirection: Qt.RightToLeft
        model: 9
        delegate: Item {
            width: gridPin.cellWidth
            height: gridPin.cellHeight
            Rectangle {
                width: 56
                height: 56
                radius: 56
                anchors.centerIn: parent
                color: pinMouse.pressed ?"#031F2B" : "transparent"
                border.color: pinMouse.pressed ? "#F1FAFE" : "#031F2B"
                Rectangle {
                    width: 8
                    height: 8
                    radius: 8
                    color: pinMouse.pressed ? "#F1FAFE" : "#031F2B"
                    anchors.centerIn: parent
                }
            }
            MouseArea {
                id: pinMouse
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                onClicked: {
                    pin.textOutput += (9 - index)
                    pin.validInput = true
                    rootPin.warning = ""
                }
            }
        }
    }
    QTextButton {
        id: sendPinBtn
        width: gridPin.width - 30
        label.text: processing ? STR.STR_QML_536 : STR.STR_QML_459
        type: eTypeA
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 56
        anchors.horizontalCenter: gridPin.horizontalCenter
        onButtonClicked: { rootPin.senPINClicked() }
    }
}
