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
import "../../customizes/Buttons"

QPopupInfoVertical {
    id: _infoPopup
    property var labels: []
    property var funcs: []
    property bool isVertical: true
    readonly property QBaseButton _btnBase: QBaseButton{}
    property var types: isVertical ? [_btnBase.eTypeE, _btnBase.eTypeF] : [_btnBase.eTypeB, _btnBase.eTypeE]
    buttons: isVertical ? _vertical : _horizontal
    Component {
        id: _vertical
        Column {
            spacing: 12
            QTextButton {
                anchors.horizontalCenter: parent.horizontalCenter
                width: 252
                height: 48
                label.text: labels[0]
                label.font.pixelSize: 16
                type: types[0]
                onButtonClicked: {
                    _infoPopup.close()
                    funcs[0]()
                }
            }
            QTextButton {
                anchors.horizontalCenter: parent.horizontalCenter
                width: 252
                height: 48
                label.text: labels[1]
                label.font.pixelSize: 16
                type: types[1]
                onButtonClicked: {
                    _infoPopup.close()
                    funcs[1]()
                }
            }
        }
    }
    Component {
        id: _horizontal
        Row {
            height: 48
            spacing: 12
            QTextButton {
                width: 118
                height: 48
                label.text: labels[0]
                label.font.pixelSize: 16
                type: types[0]
                onButtonClicked: {
                    _infoPopup.close()
                    funcs[0]()
                }
            }
            QTextButton {
                width: 118
                height: 48
                label.text: labels[1]
                label.font.pixelSize: 16
                type: types[1]
                onButtonClicked: {
                    _infoPopup.close()
                    funcs[1]()
                }
            }
        }
    }
}
