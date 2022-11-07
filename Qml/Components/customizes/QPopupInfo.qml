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
    id: _infoPopup
    width: parent.width
    height: parent.height
    modal: true
    focus: true
    property string contentText: ""
    property string title: STR.STR_QML_339
    property string btnLabel: STR.STR_QML_341
    property var coverColor: Qt.rgba(255, 255, 255, 0.7)
    signal gotItClicked()
    closePolicy: Popup.CloseOnReleaseOutside | Popup.CloseOnEscape
    background: Item{}
    Rectangle {
        width: popupWidth
        height: popupHeight
        radius: 8
        color: coverColor
        anchors.centerIn: parent
        layer.enabled: true
        layer.effect: OpacityMask {
            maskSource: Rectangle {
                width: popupWidth
                height: popupHeight
                radius: 24
            }
        }
        Rectangle {
            id: bgcontentlimit
            width: 300
            height: Math.max(254,_colum.implicitHeight + 24*2)
            anchors.centerIn: parent
            color: "#FFFFFF"
            radius: 24
            Column {
                id:_colum
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
                QTextButton {
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: 252
                    height: 36
                    label.text: btnLabel
                    label.font.pixelSize: 12
                    type: eTypeE
                    onButtonClicked: {
                        _infoPopup.close()
                        gotItClicked()
                    }
                }
            }
        }
        DropShadow {
            anchors.fill: bgcontentlimit
            horizontalOffset: 3
            verticalOffset: 5
            spread: 0
            radius: 8
            samples: 30
            color: "#aa000000"
            source: bgcontentlimit
        }
    }
}
