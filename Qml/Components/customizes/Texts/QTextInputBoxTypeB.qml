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

import "../../../../localization/STR_QML.js" as STR

Column {
    id: textipboxType
    property string label: "This is label"
    property string optional: ""
    property alias  errorText: textErrorItem.text
    property alias  errorTextColor: textErrorItem.color
    property alias  textInputted: _input.text
    property alias validator: _input.validator
    property alias acceptableInput : _input.acceptableInput
    property var   textweight: Font.Bold
    property alias echoMode: _input.echoMode
    property alias maxLength: _input.maximumLength
    property alias length: _input.length
    property bool enableLengthLimit: false
    property int boxWidth: 338
    property int boxHeight: 48
    property bool isValid: true
    property bool showError: false
    property bool showWarning: false
    property alias textBoxFocus: _input.focus
    property alias input: _input
    property bool  isPassword: false
    property bool  passwordValidate: false
    property alias passwordValidateResult: validatePassword.isValid
    property int titleFontSize: 16
    property var disabledColor: "#EAEAEA"

    signal typingFinished(var currentText)
    signal downKeyRequest()
    signal upKeyRequest()
    signal enterKeyRequest()
    signal pasteKeyRequest()
    signal tabKeyRequest()

    spacing: 4

    Row {
        width: parent.width
        QText {
            id:txt1
            width: paintedWidth
            font.family: "Lato"
            font.pixelSize: titleFontSize
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
        QText {
            width: textipboxType.width - txt1.width - txt2.width
            font.family: "Lato"
            font.pixelSize: 12
            color: "#031F2B"
            text: textipboxType.length + "/" + textipboxType.maxLength
            visible: textipboxType.enableLengthLimit
            horizontalAlignment: Text.AlignRight
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
            echoMode: isPassword ? (showpass.visiblity ? TextInput.Normal : TextInput.Password) : TextInput.Normal
            onTypingFinished: {
                textipboxType.typingFinished(currentText)
            }
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
            id: showpass
            property bool visiblity: false
            source: showpass.visiblity ? "qrc:/Images/Images/visibility.png":"qrc:/Images/Images/visibility-off.png"
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 12
            visible: (_input.text !== "") && isPassword
            MouseArea {
                anchors.fill: parent
                onClicked: showpass.visiblity =! showpass.visiblity
            }
        }
    }
    Item{
        width: myRectangle.width+2
        height: 28
        QImage {
            id: alert
            width: 20
            height: 20
            source: "qrc:/Images/Images/error_outline_24px.png"
            anchors {
                left: parent.left
                leftMargin: 0
                verticalCenter: parent.verticalCenter
            }
            visible: !isValid && showError
        }
        QText {
            id: textErrorItem
            width: myRectangle.width+2
            font.family: "Lato"
            font.pixelSize: 12
            color: "#CF4018"
            anchors {
                left: alert.right
                leftMargin: 6
                verticalCenter: parent.verticalCenter
            }
        }
        visible: !isValid && showError
    }
    Item {
        width: myRectangle.width+2
        height: 28
        Row {
            spacing: 4
            QImage {
                width: 20
                height: 20
                source: "qrc:/Images/Images/info-60px.png"
                anchors.verticalCenter: parent.verticalCenter
            }
            QText {
                width: myRectangle.width+2
                font.family: "Lato"
                font.pixelSize: 12
                color: "#595959"
                text: textipboxType.length + "/" + textipboxType.maxLength
                anchors.verticalCenter: parent.verticalCenter
            }
        }
        visible: showWarning && textipboxType.length === textipboxType.maxLength
    }
    Column {
        id: validatePassword
        width: myRectangle.width+2

        property bool ableDisplay: textInputted.length !== 0
        property bool lengthValid: textInputted.length >= 8
        property bool upperCaseValid: textInputted.match(/[A-Z]/) !== null
        property bool numberValid: textInputted.match(/\d/) !== null
        property bool specialCharValid: textInputted.match(/[!@#$%^&*()_+{}:"<>?|[\]\\;',./~`-]/) !== null
        property bool isValid: (textInputted.length === 0) || (lengthValid && upperCaseValid && upperCaseValid && specialCharValid && numberValid)

        visible: textipboxType.isPassword && textipboxType.passwordValidate && validatePassword.ableDisplay
        spacing: 4
        Row {
            spacing: 4
            QImage {
                width: 24
                height: 24
                source: "qrc:/Images/Images/info-60px.png"
                anchors.verticalCenter: parent.verticalCenter
            }
            QText {
                font.family: "Lato"
                font.pixelSize: 12
                text: STR.STR_QML_1504
                color: validatePassword.lengthValid ? "Green" : "red"
                anchors.verticalCenter: parent.verticalCenter
            }
        }
        Row {
            spacing: 4
            QImage {
                width: 24
                height: 24
                source: "qrc:/Images/Images/info-60px.png"
                anchors.verticalCenter: parent.verticalCenter
            }
            QText {
                font.family: "Lato"
                font.pixelSize: 12
                text: STR.STR_QML_1505
                color: validatePassword.numberValid ? "Green" : "red"
                anchors.verticalCenter: parent.verticalCenter
            }
        }
        Row {
            spacing: 4
            QImage {
                width: 24
                height: 24
                source: "qrc:/Images/Images/info-60px.png"
                anchors.verticalCenter: parent.verticalCenter
            }
            QText {
                font.family: "Lato"
                font.pixelSize: 12
                text: STR.STR_QML_1506
                color: validatePassword.upperCaseValid ? "Green" : "red"
                anchors.verticalCenter: parent.verticalCenter
            }
        }
        Row {
            spacing: 4
            QImage {
                width: 24
                height: 24
                source: "qrc:/Images/Images/info-60px.png"
                anchors.verticalCenter: parent.verticalCenter
            }
            QText {
                font.family: "Lato"
                font.pixelSize: 12
                text: STR.STR_QML_1507
                color: validatePassword.specialCharValid ? "Green" : "red"
                anchors.verticalCenter: parent.verticalCenter
            }
        }
    }
}
