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
import HMIEVENTS 1.0
import "../../origins"
import "../../customizes/Texts"
import "../../customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

Popup {
    id: qrcodeExportResult
    width: parent.width
    height: parent.height
    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape
    background: Item{}
    property string address:""
    property string label: "Export via QR code"
    Rectangle {
        id: qrmask
        width: 600
        height: 624
        radius: 24
        color: "#FFFFFF"
        anchors.centerIn: parent
        layer.enabled: true
        layer.effect: OpacityMask {
            maskSource: Rectangle {
                width: 600
                height: 624
                radius: 24
            }
        }
        QCloseButton {
            anchors {
                right: parent.right
                rightMargin: 16
                top: parent.top
                topMargin: 16
            }
            onClicked: qrcodeExportResult.close()
        }
        QMontserrat {
            anchors {
                top: parent.top
                topMargin: 36
                left: parent.left
                leftMargin: 36
            }
            font.weight: Font.Medium
            font.pixelSize: 32
            text: label
        }
        Column {
            anchors {
                top: parent.top
                topMargin: 100
                left: parent.left
                leftMargin: 126
            }
            spacing: 24
            QQrIcon {
                id: qrCode
                iconSize: 352
                offset: 12
                address: qrcodeExportResult.address
            }
            QLato {
                width: 352
                height: paintedHeight
                text: address
                lineHeightMode: Text.FixedHeight
                lineHeight: 20
                wrapMode: Text.WrapAnywhere
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }
        QTextButton {
            anchors {
                bottom: parent.bottom
                bottomMargin: 36
                right: parent.right
                rightMargin: 36
            }
            width: 127
            height: 48
            label.text: STR.STR_QML_205
            label.font.pixelSize: 16
            type: eTypeB
            onButtonClicked: {
                ClientController.copyMessage(qrCode.address)
            }
        }
    }
    DropShadow {
        anchors.fill: qrmask
        horizontalOffset: 3
        verticalOffset: 3
        radius: 8.0
        samples: 17
        color: "#80000000"
        source: qrmask
    }
}
