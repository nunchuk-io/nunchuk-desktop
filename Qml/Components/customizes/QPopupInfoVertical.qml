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
import QRCodeItem 1.0
import DataPool 1.0
import "../origins"
import "../customizes"
import "../../../localization/STR_QML.js" as STR

Popup {
    id: confirmYesNo
    width: parent.width
    height: parent.height
    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape
    background: Item{}
    property string title: STR.STR_QML_024
    property string contentText: STR.STR_QML_466
    property var labels: [STR.STR_QML_432,STR.STR_QML_433]
    property var types: [4,1]
    signal confirmYes()
    signal confirmNo()
    Rectangle {
        id: boxmask
        width: 300
        height: _column.implicitHeight + 26 *2
        radius: 24
        color: "#FFFFFF"
        anchors.centerIn: parent
        layer.enabled: true
        layer.effect: OpacityMask {
            maskSource: Rectangle {
                width: boxmask.width
                height: boxmask.height
                radius: boxmask.radius
            }
        }
        Column {
            id:_column
            spacing: 12
            anchors.centerIn: parent
            QText {
                anchors.horizontalCenter: parent.horizontalCenter
                font.family: "Lato"
                font.pixelSize: 16
                font.weight: Font.Bold
                text: title
                lineHeightMode: Text.FixedHeight
                lineHeight: 20
            }
            QText {
                width: 252
                anchors.horizontalCenter: parent.horizontalCenter
                font.family: "Lato"
                font.pixelSize: 16
                lineHeightMode: Text.FixedHeight
                lineHeight: 28
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: contentText
            }
            Column {
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 12
                QTextButton {
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: label.paintedWidth + 20*2
                    height: 36
                    label.text: labels[0]
                    label.font.pixelSize: 12
                    type: types[0]
                    onButtonClicked: {confirmYes()}
                }
                QTextButton {
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: label.paintedWidth + 10*2
                    height: 36
                    label.text: labels[1]
                    label.font.pixelSize: 12
                    type: eTypeF
                    onButtonClicked: {confirmNo()}
                }
            }
        }
    }
    DropShadow {
        anchors.fill: boxmask
        horizontalOffset: 3
        verticalOffset: 5
        spread: 0
        radius: 8
        samples: 30
        color: "#aa000000"
        source: boxmask
    }
}
