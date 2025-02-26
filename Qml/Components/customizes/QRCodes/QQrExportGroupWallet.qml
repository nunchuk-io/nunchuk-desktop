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
        width: 391
        height: 487
        radius: 24
        color: "#FFFFFF"
        anchors.centerIn: parent
        layer.enabled: true
        layer.effect: OpacityMask {
            maskSource: Rectangle {
                width: 391
                height: 487
                radius: 24
            }
        }
        Column {
            width: 344
            spacing: 16
            anchors {
                top: parent.top
                topMargin: 24
                horizontalCenter: parent.horizontalCenter
            }
            QQrIcon {
                id: qrCode
                iconSize: 344
                offset: 20
                address: qrcodeExportResult.address
                anchors.horizontalCenter: parent.horizontalCenter
            }
            QLato {
                width: 343
                height: 16
                text: STR.STR_QML_1656
                lineHeightMode: Text.FixedHeight
                font.pixelSize: 12
                lineHeight: 16
                wrapMode: Text.WrapAnywhere
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                anchors.horizontalCenter: parent.horizontalCenter
            }
            QTextButton {
                width: 343
                height: 48
                label.text: STR.STR_QML_245
                label.font.pixelSize: 16
                label.font.weight: Font.Bold
                type: eTypeF
                anchors.horizontalCenter: parent.horizontalCenter
                onButtonClicked: {
                    qrcodeExportResult.close()
                }
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
