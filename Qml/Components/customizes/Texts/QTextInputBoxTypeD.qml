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
import "../../customizes"
import "../../customizes/Chats"

Column {
    id: textipboxType
    property string label: "This is label"
    property alias placeHoder: placeholderText.text
    property alias  errorText: textErrorItem.text
    property alias  errorTextColor: textErrorItem.color
    property alias  textInputted: input.text
    property var   textweight: Font.Bold
    property alias echoMode: input.echoMode
    property alias maxLength: input.maximumLength
    property alias length: input.length
    property bool enableLengthLimit: false
    property int boxWidth: 338
    property int boxHeight: 48
    property bool isValid: true
    property bool showError: false
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
                color: isValid ? "#FFFFFF" : "#FFD7D9"
                border.color:  isValid ? "#DEDEDE" : "#CF4018"
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
            QText {
                id: placeholderText
                anchors.fill: input
                visible: textInputted === ""
                color: "#031F2B"
                font.pixelSize: 16
                wrapMode: Text.WordWrap
            }

            QTextEdit {
                id: input
                anchors {
                    fill: parent
                    leftMargin: 5
                    rightMargin: 5
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
                MouseArea {
                    anchors.fill: parent
                    propagateComposedEvents: true
                    acceptedButtons: Qt.LeftButton | Qt.RightButton
                    onPressed: {
                        if (mouse.button === Qt.LeftButton) {
                            mouse.accepted = false
                        }
                    }
                    onClicked: {
                        if (mouse.button === Qt.RightButton) {
                            optionMenu.popup(input,Math.min(185,mouse.x),mouse.y)
                        }
                    }
                }
            }
        }
        QContextMenu {
            id: optionMenu
            menuWidth: 150
            labels: [
                "Copy content"
            ]
            icons: [
                "qrc:/Images/Images/copy-dark.svg"
            ]
            onItemClicked: {
                switch(index){
                case 0:
                default:
                    ClientController.copyMessage(input.text)
                    break;
                }
            }
        }
    }
    Item{
        width: myRectangle.width+2
        height: 28
        QIcon {
            iconSize: 20
            id: alert
            source: "qrc:/Images/Images/error_outline_24px.png"
            anchors {
                left: parent.left
                leftMargin: 0
                verticalCenter: parent.verticalCenter
            }
            visible: !isValid && showError
        }
        QText {
            id: textErrorItem
            width: myRectangle.width+2
            font.family: "Lato"
            font.pixelSize: 16
            color: "#CF4018"
            anchors {
                left: alert.right
                leftMargin: 6
                verticalCenter: parent.verticalCenter
            }
        }
        visible: !isValid && showError
    }
}
