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
    property string optional: ""
    property alias  errorText: textErrorItem.text
    property alias  errorTextColor: textErrorItem.color
    property alias  textInputted: _input.text
    property var   textweight: Font.Bold
    property alias length: _input.length
    property bool enableLengthLimit: false
    property int boxWidth: 338
    property int boxHeight: 48
    property bool isValid: true
    property bool showError: false
    signal typingFinished(var currentText)
    property alias textBoxFocus: _input.focus
    property alias input: _input
    spacing: 4

    signal downKeyRequest()
    signal upKeyRequest()
    signal enterKeyRequest()
    signal pasteKeyRequest()
    signal tabKeyRequest()

    Row {
        width: parent.width
        QText {
            id:txt1
            width: paintedWidth
            font.family: "Lato"
            font.pixelSize: 16
            font.weight: textweight
            color: "#031F2B"
            text: label
            visible: label != ""
            anchors.bottom: parent.bottom
        }
        QText {
            id:txt2
            width: paintedWidth
            font.family: "Lato"
            font.pixelSize: 12
            font.weight: textweight
            color: "#595959"
            text: optional
            visible: optional != ""
            anchors.verticalCenter: txt1.verticalCenter
        }
        QText {
            width: textipboxType.width - txt1.width - txt2.width
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
            id: _background
            width: myRectangle.width+2
            height: myRectangle.height+2
            Rectangle {
                id: myRectangle
                anchors.centerIn: parent
                width: boxWidth
                height: boxHeight
                radius: 8
                color: textipboxType.enabled ? (isValid ? "#FFFFFF" : "#FFD7D9") : "#EAEAEA"
                border.color:  isValid ? "#DEDEDE" : "#CF4018"
            }
        }
        QTextArea {
            id: _input
            anchors.fill: parent
            background: _background
            color: "#031F2B"
            font.pixelSize: 16
            clip: true
            onTypingFinished: textipboxType.typingFinished(currentText)
            Keys.onDownPressed: { downKeyRequest() }
            Keys.onUpPressed: { upKeyRequest() }
            Keys.onPressed: function (keyEvent) {
                if (keyEvent.matches(StandardKey.Paste)) { pasteKeyRequest() }
                if (keyEvent.key === Qt.Key_Tab) { keyEvent.accepted = false; tabKeyRequest() }
            }
        }
    }
    Item{
        width: myRectangle.width+2
        height: 28
        QImage {
            id: alert
            width: 20
            height: 20
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
