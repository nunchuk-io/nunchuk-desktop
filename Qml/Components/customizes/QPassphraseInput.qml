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
import QtQuick.Controls 1.4
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.12
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import "../origins"
import "../customizes"
import "../customizes/Texts"
import "../customizes/Buttons"
import "../../../localization/STR_QML.js" as STR

Item {
    id: root
    width: 300
    height: 214
    property alias textInputted : confirmpassphrases.textInputted
    property alias valid        : confirmpassphrases.isValid
    property alias errorText    : confirmpassphrases.errorText
    property bool  processing: false
    enabled: !processing
    signal sendPassphraseClicked(var passphrase)
    signal closeClicked()
    MouseArea {anchors.fill: parent; onClicked: {}}
    Rectangle {
        id: mask
        anchors.fill: parent
        color: "#FFFFFF"
        radius: 24
        layer.enabled: true
        layer.effect: OpacityMask {
            maskSource: Rectangle {
                width: root.width
                height: root.height
                radius: 24
            }
        }
    }
    DropShadow {
        anchors.fill: mask
        horizontalOffset: 3
        verticalOffset: 3
        radius: 8.0
        samples: 17
        color: "#80000000"
        source: mask
    }
    QCloseButton {
        id: _closeBtn
        bgColor: "transparent"
        iconWidth: 0
        anchors {
            right: parent.right
            rightMargin: 24
            top: parent.top
            topMargin: 24
        }
        onClicked: {
            confirmpassphrases.textInputted = ""
            confirmpassphrases.isValid = true
            confirmpassphrases.errorText = ""
            closeClicked()
        }
    }
    Column {
        anchors.fill: parent
        anchors.margins: 24
        spacing: 24
        QLato {
            width: 252
            text: "Enter your passphrase"
            color: "#031F2B"
            font.weight: Font.Bold
            font.pixelSize: 16
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }
        QTextInputBoxTypeB {
            id: confirmpassphrases
            label: ""
            boxWidth: 252
            boxHeight: 48
            isPassword: true
            onTypingFinished: {
                confirmpassphrases.isValid = true
                confirmpassphrases.errorText = ""
            }
        }
        QTextButton {
            width: 252
            height: 48
            label.text: STR.STR_QML_265
            label.font.pixelSize: 16
            type: eTypeE
            onButtonClicked: sendPassphraseClicked(confirmpassphrases.textInputted)
        }
    }
}
