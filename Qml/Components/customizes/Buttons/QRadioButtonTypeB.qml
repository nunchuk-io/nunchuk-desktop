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
    content: Component {
        QTextInputBox {
            id: text
            rightPadding: 78
            heightMin: 56
            fontPixelSize: 14
            border.color: "#C9DEF1"
            color: radioRoot.selected && radioRoot.enabled? Qt.rgba(255, 255, 255, 0.3) : Qt.rgba(0, 0, 0, 0.1)
            enabled: radioRoot.selected && radioRoot.enabled
            onTypingFinished: radioRoot.typingFinished(currentText)
            placeholder.text: radioRoot.placeholderText
            textOutput: radioRoot.textOutput
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
