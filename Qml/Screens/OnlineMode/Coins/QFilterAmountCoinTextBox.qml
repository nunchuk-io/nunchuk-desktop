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
import DataPool 1.0
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Texts"
import "../../../../localization/STR_QML.js" as STR

Column {
    id: textipboxType
    property string label: "This is label"
    property alias  textInputted: _input.text
    property alias validator: _input.validator
    property alias acceptableInput : _input.acceptableInput
    property alias echoMode: _input.echoMode
    property alias maxLength: _input.maximumLength
    property alias length: _input.length
    property bool enableLengthLimit: false
    property int boxWidth: 338
    property int boxHeight: 48
    property bool isValid: true
    property bool showError: false
    property bool showWarning: false
    signal typingFinished(var currentText)
    property alias textBoxFocus: _input.focus
    property alias input: _input
    property int titleFontSize: 16
    property string disabledColor: "#EAEAEA"
    property bool showArrow: false
    spacing: 4

    signal downKeyRequest()
    signal upKeyRequest()
    signal enterKeyRequest()
    signal pasteKeyRequest()
    signal tabKeyRequest()

    Row {
        width: parent.width
        QLato {
            id:txt1
            width: paintedWidth
            font.family: "Lato"
            font.pixelSize: titleFontSize
            font.weight: Font.Bold
            text: label
            visible: label != ""
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
                color: textipboxType.enabled ? (isValid ? "#FFFFFF" : "#FFD7D9") : disabledColor
                border.color:  isValid ? "#DEDEDE" : "#CF4018"
            }
        }
        QTextField {
            id: _input
            anchors.fill: parent
            background: _background
            color: "#031F2B"
            font.pixelSize: 16
            clip: true
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
        Row {
            anchors {
                verticalCenter: parent.verticalCenter
                right: parent.right
                rightMargin: 6
            }
            spacing: 6
            QIcon {
                iconSize: 24
                source: "qrc:/Images/Images/fav_swapt-24px.png"
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        isCurrency = !isCurrency
                    }
                }
            }
            QTextLink {
                width: 84
                font.pixelSize: 12
                height: 16
                font.weight: Font.Bold
                text: STR.STR_QML_1460.arg(isCurrency ? RoomWalletData.unitValue : AppSetting.currency)
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                anchors.verticalCenter: parent.verticalCenter
                onTextClicked: {
                    isCurrency = !isCurrency
                }
            }
        }
    }
}
