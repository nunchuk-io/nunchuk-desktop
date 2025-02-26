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
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.12
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import "../../origins"
import "../../customizes"
import "../../customizes/Chats"
import "../../customizes/Texts"
import "../../customizes/Buttons"

Rectangle {
    id: conversationText

    implicitWidth: textContent.width + 24
    implicitHeight: textContent.height + 24
    radius: 24
    color: backgroundColor
    border.color: borderColor

    property int    maxWidth: 260
    property bool   isReceive: false
    property string backgroundColor: isReceive ? "#FFFFFF" : "#D0E2FF"
    property string borderColor: isReceive ? "#DEDEDE" : "#D0E2FF"
    property alias  messageContent: textContent.text

    signal itemRightClicked()
    signal itemLeftClicked();

    Rectangle {
        color: parent.color
        width: parent.width/2
        height: parent.height/2
        anchors.left: parent.left
        anchors.leftMargin: isReceive ? 0 : width
        Rectangle {
            width: parent.width
            color: borderColor
            height: 1
        }
        Rectangle {
            width: 1
            color: borderColor
            height: parent.height
        }
    }

    QTextEdit {
        id: textContent
        anchors.centerIn: parent
        width: Math.min(implicitWidth, maxWidth)
        readOnly: true
        font.pixelSize: 16
        font.family: "Lato"
        horizontalAlignment: Text.AlignJustify
        wrapMode: TextEdit.WrapAnywhere
    }
    // QText {
    //     id: textContent
    //     anchors.centerIn: parent
    //     width: Math.min(implicitWidth, maxWidth)
    //     color: "#031F2B"
    //     wrapMode: Text.WrapAnywhere
    //     font.family: "Lato"
    //     textFormat: Text.RichText
    //     lineHeight: 1.5
    //     font.pixelSize: 16
    //     horizontalAlignment: Text.AlignJustify
    //     onLinkActivated: Qt.openUrlExternally( link )
    //     MouseArea {
    //         anchors.fill: parent
    //         cursorShape: textContent.hoveredLink ? Qt.PointingHandCursor : Qt.ArrowCursor
    //         acceptedButtons: Qt.NoButton
    //     }
    // }
}
