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
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Chats"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../Components/customizes/Lists"
import "../../../Components/customizes/Transactions"
import "../../../Components/customizes/Wallets"
import "../../../Components/customizes/Popups"
import "../../../../localization/STR_QML.js" as STR

Popup {
    id: pdfExportItem
    width: parent.width
    height: parent.height
    modal: true
    focus: true
    background: Item{}

    property int currentIndex   : 0
    property int totalCount     : 100
    property int hCenterOffset  : -386

    function startPrintInvoices(filepath) {
        contentInvoice.item.startPrintInvoices(filepath)
    }
    function progressChanged(index, total) {
        pdfExportItem.currentIndex = index
        pdfExportItem.totalCount = total
    }

    Loader {
        id: contentInvoice
        sourceComponent: null
        visible: false
    }

    Rectangle {
        width: 300
        height: 222
        radius: 24
        color: "#FFFFFF"
        anchors.centerIn: parent
        anchors.horizontalCenterOffset: pdfExportItem.hCenterOffset/2
        Column {
            spacing: 8
            anchors.centerIn: parent
            QText {
                width: 252
                height: 28
                text: "Processing"
                font.weight: Font.Bold
                font.family: "Lato"
                font.pixelSize: 20
                color: "#031F2B"
                anchors.horizontalCenter: parent.horizontalCenter
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }
            Item {
                width: 252
                height: 16
            }
            QProgressbarTypeA {
                width: 252
                anchors.horizontalCenter: parent.horizontalCenter
                percentage: pdfExportItem.currentIndex*100/pdfExportItem.totalCount
            }
            Row {
                anchors.horizontalCenter: parent.horizontalCenter
                QText {
                    width: 252/2
                    height: 28
                    text: pdfExportItem.currentIndex + "/" + pdfExportItem.totalCount
                    font.family: "Lato"
                    font.pixelSize: 16
                    color: "#031F2B"
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    anchors.verticalCenter: parent.verticalCenter
                }
                QText {
                    width: 252/2
                    height: 28
                    text: Math.round(pdfExportItem.currentIndex*100/pdfExportItem.totalCount) + "%"
                    font.family: "Lato"
                    font.pixelSize: 16
                    color: "#031F2B"
                    anchors.verticalCenter: parent.verticalCenter
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight
                }
            }
            Item {
                width: 252
                height: 16
            }
            QTextButton {
                width: 252
                height: 48
                label.text: "Cancel"
                label.font.pixelSize: 16
                type: eTypeB
                anchors.horizontalCenter: parent.horizontalCenter
                onButtonClicked: {
                    pdfExportItem.close()
                }
            }
        }
    }
    onOpened : {
        pdfExportItem.currentIndex = 0
        pdfExportItem.totalCount = 100
        contentInvoice.sourceComponent = invoiceComponent
    }
    onClosed: {
        pdfExportItem.currentIndex = 0
        pdfExportItem.totalCount = 100
        contentInvoice.sourceComponent = null
    }
    Connections {
        target: PDFPrinter
        function onFinished() {
            console.log("PDFPrinter finished")
            pdfExportItem.close()
        }
        function onProgressChanged(index, total) {
            console.log("PDFPrinter progress changed", index, total)
            pdfExportItem.progressChanged(index, total)
        }
    }

    Component {
        id: invoiceComponent
        Repeater {
            id: listInvoices
            width: 728
            height: 496
            clip: true
            model: AppModel.walletInfo.transactionHistory
            delegate: QTransactionInvoice {
                txid            : transaction_txid
                total           : transaction_total
                subtotal        : transaction_subtotal
                blocktime       : transaction_blocktime
                fee             : transaction_fee
                memo            : transaction_memo
                hasChange       : transaction_hasChange
                changeAmount    : transaction_change_role.amount
                changeAddress   : transaction_change_role.address
                isRecieveTx     : transaction_isReceiveTx
                destinations    : transaction_destinationList
                interactive     : false
            }
            function startPrintInvoices(filepath) {
                var objects = [];
                for(var i = 0; i < listInvoices.count; i++) {
                    objects.push(listInvoices.itemAt(i))
                }
                // PDFPrinter.printInvoicesToPdf(filepath, objects)
                var exportObj = {
                    "export_type"    : "pdf",
                    "export_path"    : filepath,
                    "export_data"    : objects
                };
                QMLHandle.sendEvent(EVT.EVT_HOME_EXPORT_BSMS, exportObj)
            }
        }
    }
}
