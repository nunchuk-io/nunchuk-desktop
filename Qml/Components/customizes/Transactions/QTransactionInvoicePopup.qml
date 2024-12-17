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
import QtQuick.Controls 2.3
import QtGraphicalEffects 1.12
import Qt.labs.platform 1.1
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Chats"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

Popup {
    id: invoicePopup
    width: parent.width
    height: parent.height
    modal: true
    focus: true
    background: Item{}

    property alias txid         :invoiceElement.txid
    property alias total        :invoiceElement.total
    property alias subtotal     :invoiceElement.subtotal
    property alias blocktime    :invoiceElement.blocktime
    property alias fee          :invoiceElement.fee
    property alias memo         :invoiceElement.memo
    property alias hasChange    :invoiceElement.hasChange
    property alias changeAmount :invoiceElement.changeAmount
    property alias changeAddress:invoiceElement.changeAddress
    property alias isRecieveTx  :invoiceElement.isRecieveTx
    property alias destinations :invoiceElement.destinations

    QOnScreenContent {
        id: contenCenter
        width: 800
        height: 700
        anchors.centerIn: parent
        label.text: "Invoice"
        isShowLine: true
        extraHeader:Item {}
        bottomLeft:  Item {}
        bottomRight: Row {
            spacing: 12
            QTextButton {
                width: 100
                height: 48
                label.text: "Save PDF"
                label.font.pixelSize: 16
                type: eTypeB
                onButtonClicked: {
                    exportPDF.currentFile = StandardPaths.writableLocation(StandardPaths.DocumentsLocation) + "/Transaction_"
                            + invoicePopup.txid
                            + ".pdf"
                    exportPDF.open()
                }
            }
        }
        onCloseClicked: {
            invoicePopup.close()
        }
        layer.enabled: true
        layer.effect: OpacityMask {
            maskSource: Rectangle {
                width: 764
                height: 700
                radius: 24
            }
        }
        QTransactionInvoice {
            id: invoiceElement
            anchors.centerIn: parent
            anchors.verticalCenterOffset: -2
        }
    }
    FileDialog {
        id: exportPDF
        fileMode: FileDialog.SaveFile
        onAccepted: {
            PDFPrinter.printInvoiceToPdf(exportPDF.currentFile, invoiceElement.invoiceContent)
        }
    }
}
