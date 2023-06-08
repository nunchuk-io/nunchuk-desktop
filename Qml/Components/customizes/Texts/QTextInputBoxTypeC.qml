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
import "../../origins"

Column {
    id: textipboxType
    property string label: "This is label"
    property alias  textInputted: input.text
    property var   textweight: Font.Bold
    property alias echoMode: input.echoMode
    property alias maxLength: input.maximumLength
    property alias length: input.length
    property bool enableLengthLimit: false
    property int boxWidth: 338
    property int boxHeight: 48
    property bool isDimmed: false
    signal typingFinished(var currentText)
    property alias textBoxFocus: input.focus
    readonly property int eREADONLY_MODE: 0
    readonly property int ePREVIEW_MODE: 1
    readonly property int eEDIT_MODE: 2
    property int mode: eEDIT_MODE
    spacing: 4

    signal downKeyRequest()
    signal upKeyRequest()
    signal enterKeyRequest()
    signal copyCompleted()

    Row {
        width: parent.width
        QText {
            width: textipboxType.width/2
            font.family: "Lato"
            font.pixelSize: 16
            font.weight: textweight
            color: "#031F2B"
            text: label
            anchors.bottom: parent.bottom
        }
        QText {
            width: textipboxType.width/2
            font.family: "Lato"
            font.pixelSize: 12
            color: "#031F2B"
            text: textipboxType.length + "/" + textipboxType.maxLength
            visible: textipboxType.enableLengthLimit
            horizontalAlignment: Text.AlignRight
            anchors.bottom: parent.bottom
        }
    }

    Item {
        width: myRectangle.width+2
        height: myRectangle.height+2
        Item {
            id: background
            width: myRectangle.width+2
            height: myRectangle.height+2
            Rectangle {
                id: myRectangle
                anchors.centerIn: parent
                width: boxWidth
                height: boxHeight
                radius: 8
                color: isDimmed ? "#EAEAEA" : "#FFFFFF"
                border.color:  isDimmed ? "#DEDEDE" : "#DEDEDE"
            }
        }

        InnerShadow {
            anchors.fill: background
            cached: true
            horizontalOffset: 0
            verticalOffset: 0
            radius: 8
            samples: 16
            color: Qt.rgba(3, 31, 43, 0.14)
            smooth: true
            source: background

            QTextEdit {
                id: input
                anchors {
                    fill: parent
                    leftMargin: 5
                    rightMargin: 5 + 24 + 5
                }
                readOnly: (mode == eREADONLY_MODE)
                activeFocusOnPress: (mode == eEDIT_MODE)
                color: "#031F2B"
                font.pixelSize: 16
                clip: true
                onTypingFinished: textipboxType.typingFinished(currentText)

                Keys.onDownPressed: {downKeyRequest()}
                Keys.onUpPressed: {upKeyRequest()}
                Keys.onReturnPressed: { enterKeyRequest()}
                Keys.onEnterPressed:  { enterKeyRequest()}
                wrapMode:Text.WrapAnywhere

            }
        }

        QImage {
            id: copy_
            width: 24
            height: 24
            property bool visiblity: false
            source: "qrc:/Images/Images/copy-dark.svg"
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 12
            scale: _mouse.containsMouse ? 1.1 : 1.0
            MouseArea {
                id:_mouse
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                onClicked: {
                    ClientController.copyMessage(input.text)
                    copyCompleted()
                }
            }
        }
    }
}
