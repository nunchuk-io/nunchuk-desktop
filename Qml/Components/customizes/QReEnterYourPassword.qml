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
import "../../../localization/STR_QML.js" as STR

Item {
    id: root
    width: 320
    height: confirmpassword.isValid ? 280 : 300
    property alias textInputted : confirmpassword.textInputted
    property alias valid        : confirmpassword.isValid
    property alias errorText    : confirmpassword.errorText
    property bool  processing: false
    enabled: !processing
    signal sendPasswordClicked(var password)
    signal cancelClicked()
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
    Column {
        anchors.fill: parent
        anchors.margins: 24
        spacing: 24
        Column {
            spacing: 12
            QText {
                width: root.width - 2*24
                height: 20
                text: STR.STR_QML_709
                color: "#031F2B"
                font.family: "Lato"
                font.weight: Font.Bold
                font.pixelSize: 16
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            QText {
                width: root.width - 2*24
                height: 56
                text: STR.STR_QML_710
                color: "#031F2B"
                font.family: "Lato"
                font.weight: Font.Normal
                font.pixelSize: 16
                lineHeightMode: Text.FixedHeight
                lineHeight: 28
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }
        QTextInputBoxTypeB {
            id: confirmpassword
            label: ""
            boxWidth: root.width - 2*24
            boxHeight: 48
            isPassword: true
            onTextInputtedChanged: {
                if(!confirmpassword.isValid){
                    confirmpassword.isValid = true
                    confirmpassword.errorText = ""
                }
                confirmpassword.showError = false;
            }
        }
        Row {
            height: 48
            spacing: 24
            QTextButton {
                width: 118
                height: 48
                label.text: STR.STR_QML_035
                label.font.pixelSize: 16
                type: eTypeF
                onButtonClicked: cancelClicked()
            }
            QTextButton {
                width: 118
                height: 48
                label.text: STR.STR_QML_097
                label.font.pixelSize: 16
                type: eTypeE
                onButtonClicked: sendPasswordClicked(confirmpassword.textInputted)
            }
        }
    }
    function setErrorText(errorText)
    {
        confirmpassword.errorText = errorText
        confirmpassword.isValid = false
        confirmpassword.showError = true
    }
}
