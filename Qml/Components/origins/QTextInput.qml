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
import QtQuick.Controls 2.4


TextInput {
    id: textEdit
    color: "Black"
    activeFocusOnTab : true
    activeFocusOnPress: true
    verticalAlignment: Text.AlignVCenter
    cursorVisible: !readOnly && textEdit.activeFocus
    selectByMouse: true
    renderType: Text.QtRendering

    signal typingFinished(var currentText)
    onTextChanged: if(initialized === true) inputIdentify.restart()

    property bool initialized: false
    Timer {
        id: inputIdentify
        interval: 250
        onTriggered: { if(textEdit.text !== "") typingFinished(textEdit.text) }
    }
    Component.onCompleted: initialized = true
}
