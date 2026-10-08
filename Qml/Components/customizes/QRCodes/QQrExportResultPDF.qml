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
import Qt.labs.platform 1.1
import QRCodeItem 1.0
import DataPool 1.0
import "../../origins"
import "../../customizes/Texts"
import "../../customizes/Buttons"

Popup {
    id: qrcodeExportResult
    width: parent.width
    height: parent.height
    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape
    background: Item{}
    property alias  model: listQr.model
    property string label: "Export via QR code"
    property string filename: ""

    Rectangle {
        id: qrmask
        width: 600
        height: 548
        radius: 24
        color: "#FFFFFF"
        anchors.centerIn: parent
        layer.enabled: true
        layer.effect: OpacityMask {
            maskSource: Rectangle {
                width: 600
                height: 548
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
        QText {
            anchors.top: parent.top
            anchors.topMargin: 50
            anchors.horizontalCenter: parent.horizontalCenter
            font.family: "Montserrat"
            font.pixelSize: 32
            text: label
        }
        Repeater {
            id: listQr
            width: 340
            height: 340
            anchors.centerIn: parent
            property int currentIndex: 0
            delegate: QRCodeItem {
                id: qrCode
                width: 340
                height: 340
                borderWitdh: 9
                textInput: modelData
                anchors.centerIn: parent
                z: index == listQr.currentIndex ? 1 : 0
            }
        }
        QTextButton {
            width: 86
            height: 34
            label.text: "Save PDF"
            label.font.pixelSize: 12
            type: eTypeB
            anchors.horizontalCenter: listQr.horizontalCenter
            anchors.top: listQr.bottom
            anchors.topMargin: 16
            onButtonClicked: {
                exportPDF.currentFile = StandardPaths.writableLocation(StandardPaths.DocumentsLocation) + "/"
                        + filename
                        + ".pdf"
                exportPDF.open()
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
    Timer {
        interval: 250
        running: true
        repeat: true
        onTriggered: {
            if(listQr.currentIndex < listQr.count) {listQr.currentIndex++}
            else{listQr.currentIndex = 0}
        }
    }

    FileDialog {
        id: exportPDF
        property int exportFormat: -1
        fileMode: FileDialog.SaveFile
        onAccepted: {
            var objects = [];
            for(var i = 0; i < listQr.count; i++) {
                objects.push(listQr.itemAt(i))
            }
            PDFPrinter.printQRCodeToPdf(exportPDF.currentFile, objects)
        }
    }
}
