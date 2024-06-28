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
    property bool isRecieveTx: AppModel.transactionInfo.isReceiveTx
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
                            + AppModel.transactionInfo.txid
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
        Rectangle {
            width: 728
            height: 496
            anchors.centerIn: parent
            anchors.verticalCenterOffset: -2
            radius: 12
            color: "#F5F5F5"
            Flickable {
                id: invoiceFlick
                anchors.fill: parent
                anchors.margins: 16
                anchors.horizontalCenter: parent.horizontalCenter
                clip: true
                flickableDirection: Flickable.VerticalFlick
                interactive: true
                contentHeight: contentDisplay.height
                ScrollBar.vertical: ScrollBar { active: true }
                Column {
                    id: contentDisplay
                    width: parent.width
                    spacing: 16
                    Column {
                        width: parent.width
                        spacing: 4
                        QText {
                            font.family: "Lato"
                            color: "#031F2B"
                            font.pixelSize: 16
                            text: isRecieveTx ? "Amount receive" : "Amount sent"
                        }
                        QText {
                            font.family: "Lato"
                            color: "#031F2B"
                            font.pixelSize: 24
                            font.weight: Font.DemiBold
                            text: {
                                return qsTr("%1 %2").arg(isRecieveTx ? AppModel.transactionInfo.subtotal : AppModel.transactionInfo.total)
                                                    .arg(RoomWalletData.unitValue)
                            }
                        }
                        QText {
                            font.family: "Lato"
                            color: "#031F2B"
                            font.pixelSize: 16
                            text: AppModel.transactionInfo.blocktime
                        }
                    }
                    Column {
                        width: parent.width
                        Rectangle {
                            width: parent.width
                            height: 48
                            color: "#EAEAEA"
                            QText {
                                anchors.fill: parent
                                anchors.margins: 12
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignLeft
                                font.family: "Lato"
                                color: "#031F2B"
                                font.pixelSize: 12
                                font.weight: Font.DemiBold
                                text: "Transaction ID"
                            }
                        }
                        Rectangle {
                            width: parent.width
                            height: 48
                            color: "#FFFFFF"
                            QText {
                                font.family: "Lato"
                                color: "#031F2B"
                                font.pixelSize: 16
                                font.weight: Font.DemiBold
                                anchors.fill: parent
                                anchors.margins: 12
                                text: AppModel.transactionInfo.txid
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignLeft
                            }
                        }
                        Rectangle {
                            width: parent.width
                            height: 48
                            color: "#EAEAEA"
                            QText {
                                font.family: "Lato"
                                color: "#031F2B"
                                font.pixelSize: 12
                                font.weight: Font.DemiBold
                                anchors.fill: parent
                                anchors.margins: 12
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignLeft
                                text: isRecieveTx ? "Receive at" : "Send to address"
                            }
                        }
                        Rectangle {
                            width: parent.width
                            height: childrenRect.height
                            color: "#FFFFFF"
                            Column {
                                width: parent.width
                                QListView {
                                    id: destlist
                                    width: parent.width
                                    height: contentHeight
                                    model: AppModel.transactionInfo.destinationList
                                    interactive: false
                                    delegate: Item {
                                        width: destlist.width
                                        height: 48
                                        Row {
                                            anchors.fill: parent
                                            anchors.margins: 12
                                            QText {
                                                width: parent.width * 0.7
                                                font.family: "Lato"
                                                color: "#031F2B"
                                                font.pixelSize: 16
                                                font.weight: Font.DemiBold
                                                text: destination_address
                                                verticalAlignment: Text.AlignVCenter
                                                horizontalAlignment: Text.AlignLeft
                                                anchors.verticalCenter: parent.verticalCenter
                                                wrapMode: Text.WrapAnywhere
                                            }
                                            QText {
                                                width: parent.width * 0.3
                                                font.family: "Lato"
                                                color: "#031F2B"
                                                font.pixelSize: 16
                                                font.weight: Font.DemiBold
                                                text: qsTr("%1 %2").arg(destination_amount) //"21,000,000,000,000,000"
                                                                   .arg(RoomWalletData.unitValue)
                                                verticalAlignment: Text.AlignVCenter
                                                horizontalAlignment: Text.AlignRight
                                                anchors.verticalCenter: parent.verticalCenter
                                                wrapMode: Text.WrapAnywhere
                                            }
                                        }
                                    }
                                }
                                Rectangle {
                                    width: parent.width
                                    height: 1
                                    color: "#EAEAEA"
                                    visible: !isRecieveTx
                                }
                                Item {
                                    width: parent.width
                                    height: 48
                                    visible: !isRecieveTx
                                    Row {
                                        anchors.fill: parent
                                        anchors.margins: 12
                                        QText {
                                            width: parent.width * 0.7
                                            font.family: "Lato"
                                            color: "#031F2B"
                                            font.pixelSize: 16
                                            text: "Transaction fee"
                                            verticalAlignment: Text.AlignVCenter
                                            horizontalAlignment: Text.AlignLeft
                                            anchors.verticalCenter: parent.verticalCenter
                                            wrapMode: Text.WrapAnywhere
                                        }
                                        QText {
                                            width: parent.width * 0.3
                                            font.family: "Lato"
                                            color: "#031F2B"
                                            font.pixelSize: 16
                                            font.weight: Font.DemiBold
                                            text: qsTr("%1 %2").arg(AppModel.transactionInfo.fee) //"21,000,000,000,000,000"
                                                               .arg(RoomWalletData.unitValue)
                                            verticalAlignment: Text.AlignVCenter
                                            horizontalAlignment: Text.AlignRight
                                            anchors.verticalCenter: parent.verticalCenter
                                            wrapMode: Text.WrapAnywhere
                                        }
                                    }
                                }
                                Item {
                                    width: parent.width
                                    height: 48
                                    visible: !isRecieveTx
                                    Row {
                                        anchors.fill: parent
                                        anchors.margins: 12
                                        QText {
                                            width: parent.width * 0.7
                                            font.family: "Lato"
                                            color: "#031F2B"
                                            font.pixelSize: 16
                                            text: "Total amount"
                                            verticalAlignment: Text.AlignVCenter
                                            horizontalAlignment: Text.AlignLeft
                                            anchors.verticalCenter: parent.verticalCenter
                                            wrapMode: Text.WrapAnywhere
                                        }
                                        QText {
                                            width: parent.width * 0.3
                                            font.family: "Lato"
                                            color: "#031F2B"
                                            font.pixelSize: 16
                                            font.weight: Font.DemiBold
                                            text: qsTr("%1 %2").arg(AppModel.transactionInfo.total) //"21,000,000,000,000,000"
                                                               .arg(RoomWalletData.unitValue)
                                            verticalAlignment: Text.AlignVCenter
                                            horizontalAlignment: Text.AlignRight
                                            anchors.verticalCenter: parent.verticalCenter
                                            wrapMode: Text.WrapAnywhere
                                        }
                                    }
                                }
                            }
                        }
                        Rectangle {
                            width: parent.width
                            height: 48
                            color: "#EAEAEA"
                            visible: !isRecieveTx && AppModel.transactionInfo.hasChange
                            QText {
                                font.family: "Lato"
                                color: "#031F2B"
                                font.pixelSize: 12
                                font.weight: Font.DemiBold
                                anchors.fill: parent
                                anchors.margins: 12
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignLeft
                                text: "Change address"
                            }
                        }
                        Rectangle {
                            width: parent.width
                            height: 48
                            color: "#FFFFFF"
                            visible: !isRecieveTx && AppModel.transactionInfo.hasChange
                            Row {
                                anchors.fill: parent
                                anchors.margins: 12
                                QText {
                                    width: parent.width * 0.7
                                    font.family: "Lato"
                                    color: "#031F2B"
                                    font.pixelSize: 16
                                    font.weight: Font.DemiBold
                                    text: AppModel.transactionInfo.change.address
                                    verticalAlignment: Text.AlignVCenter
                                    horizontalAlignment: Text.AlignLeft
                                    anchors.verticalCenter: parent.verticalCenter
                                    wrapMode: Text.WrapAnywhere
                                }
                                QText {
                                    width: parent.width * 0.3
                                    font.family: "Lato"
                                    color: "#031F2B"
                                    font.pixelSize: 16
                                    font.weight: Font.DemiBold
                                    text: qsTr("%1 %2").arg(AppModel.transactionInfo.change.amount) //"21,000,000,000,000,000"
                                                       .arg(RoomWalletData.unitValue)
                                    verticalAlignment: Text.AlignVCenter
                                    horizontalAlignment: Text.AlignRight
                                    anchors.verticalCenter: parent.verticalCenter
                                    wrapMode: Text.WrapAnywhere
                                }
                            }
                        }
                        Rectangle {
                            width: parent.width
                            height: 48
                            color: "#EAEAEA"
                            QText {
                                font.family: "Lato"
                                color: "#031F2B"
                                font.pixelSize: 12
                                font.weight: Font.DemiBold
                                anchors.fill: parent
                                anchors.margins: 12
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignLeft
                                text: "Transaction note"
                            }
                        }
                        Rectangle {
                            width: parent.width
                            height: 48
                            color: "#FFFFFF"
                            QText {
                                font.family: "Lato"
                                color: "#031F2B"
                                font.pixelSize: 12
                                anchors.fill: parent
                                anchors.margins: 12
                                text: AppModel.transactionInfo.memo
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignLeft
                            }
                        }
                    }
                }
            }
        }
    }

    FileDialog {
        id: exportPDF
        fileMode: FileDialog.SaveFile
        onAccepted: {
            PDFPrinter.printInvoiceToPdf(exportPDF.currentFile, contentDisplay)
        }
    }
}
