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
import QtGraphicalEffects 1.12
import QtQuick.Controls 2.5
import QRCodeItem 1.0
import "../origins"
import "../customizes/Texts"
import "../customizes/Buttons"
import "../../../localization/STR_QML.js" as STR

Popup {
    id: deletingModelRoot
    width: parent.width
    height: parent.height
    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape
    background: Item{}
    property string label: ""
    property string content: ""
    property string leftBtnLabel: STR.STR_QML_245
    property string rightBtnLabel: STR.STR_QML_246
    property string placeholderText: STR.STR_QML_244
    property int modalWidth: 488
    property int modalHeight: 317
    property int modelRadius: 24
    signal cancelRequest()
    signal deleteRequest()
    Rectangle {
        id: sourceBg
        width: modalWidth
        height: modalHeight
        radius: modelRadius
        color: "#FFFFFF"
        anchors.centerIn: parent
        Rectangle {
            width: parent.width
            height: 2
            color: "Red"
            radius: 8
        }
        visible: false
    }
    Rectangle {
        id: mask
        width: modalWidth
        height: modalHeight
        radius: modelRadius
        visible: false
    }
    OpacityMask {
        id: opacitymask
        anchors.fill: sourceBg
        source: sourceBg
        maskSource: mask
    }
    DropShadow {
        anchors.fill: opacitymask
        horizontalOffset: 3
        verticalOffset: 3
        radius: 8.0
        samples: 17
        color: "#80000000"
        source: opacitymask
    }

    Item{
        width: modalWidth
        height: modalHeight
        anchors.centerIn: parent
        Column{
            height: parent.height - 26
            width: parent.width
            anchors.centerIn: parent
            spacing: 12
            QText {
                width: parent.width - 58*2
                height: paintedHeight
                anchors.horizontalCenter: parent.horizontalCenter
                horizontalAlignment: Text.AlignHCenter
                text: deletingModelRoot.label
                color: "#031F2B"
                font.family: "Montserrat"
                font.weight: Font.DemiBold
                font.pixelSize: 24
            }
            QText {
                width: parent.width - 24*2
                height: Math.max(paintedHeight,84)
                wrapMode: Text.WordWrap
                anchors.horizontalCenter: parent.horizontalCenter
                horizontalAlignment: Text.AlignHCenter
                text: deletingModelRoot.content
                font.family: "Lato"
                font.pixelSize: 14
            }
            QTextInputBox {
                id: inputDelete
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width - 24*2
                heightMin: 56
                mode: eEDIT_MODE
                maximumLength: 10
                placeholder.text: deletingModelRoot.placeholderText
                border.color: "#80000000"
            }
            Item{
                width: parent.width
                height: 24 + 36
                Row {
                    anchors{
                        horizontalCenter: parent.horizontalCenter
                        verticalCenter: parent.verticalCenter
                    }
                    spacing: parent.width - 120*2 - 24*4
                    QTextButton {
                        width: 120
                        height: 36
                        label.text: leftBtnLabel
                        label.font.pixelSize: 16
                        type: eTypeB
                        onButtonClicked: { cancelRequest(); deletingModelRoot.close() }
                    }
                    QTextButton {
                        width: 120
                        height: 36
                        label.text: rightBtnLabel
                        label.font.pixelSize: 16
                        type: eTypeA
                        enabled: (inputDelete.textOutput === "DELETE")
                        onButtonClicked: { deleteRequest(); deletingModelRoot.close() }
                    }
                }
            }
        }
    }
}
