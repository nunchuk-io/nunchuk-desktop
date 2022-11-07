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
    id: radioRoot
    width: 313
    spacing: 8
    property bool selected: false
    property alias text: text
    signal buttonClicked()
    signal buttonResetClicked()
    signal typingFinished(var textOutput)

    QImage {
        id: icon
        width: 24
        height: 24
        source: radioRoot.selected ? "qrc:/Images/Images/RadioEnabled.png" : "qrc:/Images/Images/RadioDeselected.png"
        anchors.verticalCenter: parent.verticalCenter
        MouseArea {
            id: mouse
            anchors.fill: parent
            onClicked: { buttonClicked()}
        }
    }

    QTextInputBox {
        id: text
        width: radioRoot.width - icon.width - 8
        rightPading: 78
        heightMin: 56
        fontPixelSize: 14
        anchors.verticalCenter: parent.verticalCenter
        border.color: "#C9DEF1"
        color: radioRoot.selected && parent.enabled? Qt.rgba(255, 255, 255, 0.3) : Qt.rgba(0, 0, 0, 0.1)
        enabled: radioRoot.selected && parent.enabled
        onTypingFinished: radioRoot.typingFinished(currentText)

        QTextButton {
            width: 70
            height: 24
            label.text: "RESET"
            label.font.pixelSize: 10
            type: eTypeB
            visible: radioRoot.selected
            anchors {
                right: parent.right
                rightMargin: 6
                verticalCenter: parent.verticalCenter
            }
            onButtonClicked: { buttonResetClicked() }
        }
    }
}
