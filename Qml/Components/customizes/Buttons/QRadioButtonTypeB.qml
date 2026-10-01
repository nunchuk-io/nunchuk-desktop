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
import QtQuick
import "../../origins"
import "../../customizes/Texts"
import "../../customizes/Buttons"

QRadioSelect {
    id: radioRoot
    width: 313
    spacing: 8
    property string placeholderText: ""
    property string textOutput: ""
    signal typingFinished(var textOutput)
    property bool showArrow: false
    signal buttonArrowClicked()
    layoutDirection: Qt.RightToLeft
    contentOnTop: true
    // Built directly on QText/QTextField (not QTextInputBox) so the title and
    // the input text share the exact same 16px left anchor with nothing else
    // (QTextInputBox's embedded Controls TextField has its own style padding
    // on top of textLeftMargin, which kept the two misaligned).
    content: Component {
        Item {
            id: box
            height: 56
            property bool isEditing: (fieldInput.text !== "") || fieldInput.activeFocus
            Rectangle {
                anchors.fill: parent
                radius: 4
                border.color: "#C9DEF1"
                color: radioRoot.selected && radioRoot.enabled ? Qt.rgba(255, 255, 255, 0.3) : Qt.rgba(0, 0, 0, 0.1)
            }
            Rectangle {
                width: parent.width - 2
                height: 2
                color: fieldInput.activeFocus ? "#F6D65D" : "#C9DEF1"
                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                visible: box.isEditing
            }
            QText {
                anchors {
                    left: parent.left
                    leftMargin: 16
                    top: parent.top
                    topMargin: box.isEditing ? 8 : 16
                }
                font.family: "Lato"
                font.weight: box.isEditing ? Font.Bold : Font.Normal
                color: "#031F2B"
                font.pixelSize: box.isEditing ? 10 : 16
                font.capitalization: box.isEditing ? Font.AllUppercase : Font.MixedCase
                text: radioRoot.placeholderText
            }
            QTextField {
                id: fieldInput
                anchors {
                    fill: parent
                    leftMargin: 16
                    rightMargin: 78
                    topMargin: 24
                    bottomMargin: 0
                }
                leftPadding: 0
                rightPadding: 0
                background: Rectangle { anchors.fill: parent; color: "transparent" }
                font.family: "Lato"
                font.pixelSize: 14
                color: "#031F2B"
                wrapMode: Text.WrapAnywhere
                clip: true
                text: radioRoot.textOutput
                enabled: radioRoot.selected && radioRoot.enabled
                onTypingFinished: radioRoot.typingFinished(currentText)
                onTextChanged: {
                    if (radioRoot.textOutput !== text) {
                        radioRoot.textOutput = text
                    }
                }
            }
            QIconButton {
                iconSize: 24
                anchors {
                    right: parent.right
                    rightMargin: 6
                    verticalCenter: parent.verticalCenter
                }
                visible: showArrow
                icon: "qrc:/Images/Images/right-arrow-dark.svg"
                onButtonClicked: { buttonArrowClicked() }
                bgColor: "transparent"
            }
        }
    }
}
