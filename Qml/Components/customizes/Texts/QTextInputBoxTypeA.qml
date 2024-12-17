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
import QtQuick.Controls 2.1
import "../../origins"

TextField {
    id: editbox
    property string borderColor: "#FFFFFF"
    property string backgroundColor: "#DEDEDE"
    property int borderRadius: 8
    property bool showEdit: true
    clip: true
    color: "#031F2B"
    font.family: "Lato"
    font.pixelSize: 16
    selectByMouse: true
    wrapMode: Text.WrapAnywhere
    background: Rectangle {
        anchors.fill: parent
        radius: borderRadius
        border.color: borderColor
        color: backgroundColor
    }
    leftPadding: 0
    rightPadding: activeFocus ? 0 : 40
    activeFocusOnPress: false
    QIcon {
        iconSize: 24
        anchors {
            verticalCenter: parent.verticalCenter
            right: parent.right
            rightMargin: 12
        }
        source: "qrc:/Images/Images/edit.png"
        visible: !editbox.activeFocus && showEdit
        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor
            onClicked: {
                editbox.focus = true
                editbox.forceActiveFocus()
            }
        }
    }
    onActiveFocusChanged: { typingFinished(editbox.text) }
    Keys.onReturnPressed: {
        typingFinish()
    }
    Keys.onEnterPressed:  {
        typingFinish()
    }
    function typingFinish() {
        focus = false
        typingFinished(editbox.text)
    }
    signal typingFinished(var currentText)
}
