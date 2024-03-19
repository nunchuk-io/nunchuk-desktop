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
    property alias  textInputted: _input.text
    property alias validator: _input.validator
    property alias acceptableInput : _input.acceptableInput
    property var   textweight: Font.Bold
    property alias length: _input.length
    property alias readOnly: _input.readOnly
    property int boxWidth: 338
    property int boxHeight: 48
    signal typingFinished(var currentText)
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
    }
    Item {
        width: boxWidth
        height: boxHeight
        QTextField {
            id: _input
            anchors.fill: parent
            background: Item {}
            color: "#031F2B"
            font.pixelSize: 16
            clip: true
            leftPadding: 0
            echoMode: TextInput.Normal
            onTypingFinished: textipboxType.typingFinished(currentText)
            Keys.onDownPressed: { downKeyRequest() }
            Keys.onUpPressed: { upKeyRequest() }
            Keys.onReturnPressed: { enterKeyRequest() }
            Keys.onEnterPressed:  { enterKeyRequest() }
            Keys.onPressed: function (keyEvent) {
                if (keyEvent.matches(StandardKey.Paste)) { pasteKeyRequest() }
                if (keyEvent.key === Qt.Key_Tab) { keyEvent.accepted = false; tabKeyRequest() }
            }
        }
        QIcon {
            iconSize: 24
            property bool visiblity: false
            source: "qrc:/Images/Images/edit.png"
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 12
            visible: !_input.activeFocus && !readOnly
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                onClicked: {
                    _input.focus = true
                    _input.forceActiveFocus()
                }
            }
        }
    }
}
