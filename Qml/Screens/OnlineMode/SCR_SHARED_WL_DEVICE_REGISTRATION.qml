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
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.12
import Qt.labs.platform 1.1
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Chats"
import "../../../localization/STR_QML.js" as STR
QScreen {
    id: roots
    popupWidth: 800
    popupHeight: 700

    QOnScreenContent {
        id: contenCenter
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        label.text: "Device registration"
        onCloseClicked: {
            QMLHandle.sendEvent(EVT.EVT_ONLINE_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION)
        }

        Column {
            width: 500
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 144
            spacing: 24
            Rectangle {
                width: 96
                height: 96
                color: "#F5F5F5"
                anchors.horizontalCenter: parent.horizontalCenter
                QImage {
                    width: 60
                    height: 60
                    anchors.centerIn: parent
                    source: "qrc:/Images/Images/OnlineMode/Device Registration.png"
                }
            }
            QText {
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Upload wallet configuration"
                font.pixelSize: 20
                font.weight: Font.ExtraBold
                font.family: "Lato"
                color: "#031F2B"
                horizontalAlignment: Text.AlignHCenter
            }
            QText {
                width: parent.width
                anchors.horizontalCenter: parent.horizontalCenter
                text: "If you have hardware keys in your multisig wallet, you might be required to upload the wallet configuration file onto the hardware before the key can be used."
                font.pixelSize: 16
                font.family: "Lato"
                color: "#031F2B"
                lineHeight: 28
                lineHeightMode: Text.FixedHeight
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignHCenter
            }
            QText {
                width: parent.width
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Choose the export option according to your hardware type. Please check the vendor’s manual."
                font.pixelSize: 16
                font.family: "Lato"
                color: "#031F2B"
                lineHeight: 28
                lineHeightMode: Text.FixedHeight
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignHCenter
            }
        }

        QButtonTextLink {
            height: 48
            label: STR.STR_QML_059
            fontPixelSize: 16
            anchors {
                left: parent.left
                leftMargin: 40
                bottom: parent.bottom
                bottomMargin: 40
            }
            onButtonClicked: {
                QMLHandle.sendEvent(EVT.EVT_SHARED_WL_DEVICE_REGISTRAION_BACK)
            }
        }

        QTextButton {
            id: buttonBottomR
            width: 80
            height: 48
            label.text: STR.STR_QML_323
            label.font.pixelSize: 16
            type: eTypeE
            anchors {
                right: parent.right
                rightMargin: 36
                bottom: parent.bottom
                bottomMargin: 36
            }
            onButtonClicked: {
                exportMenu.popup()
            }

            QContextMenu {
                id: exportMenu
                labels: ["Export wallet to Coldcard", "Export wallet as QR code"]
                icons: ["qrc:/Images/Images/OnlineMode/Export.png", "qrc:/Images/Images/OnlineMode/QRCodeScan.png"]
                onItemClicked: {
                    switch(index){
                    case 0:
                        exportwalletDialog.exportFormat = NUNCHUCKTYPE.COLDCARD
                        exportwalletDialog.currentFile = StandardPaths.writableLocation(StandardPaths.DocumentsLocation) + "/"
                                + RoomWalletData.getValidFilename(RoomWalletData.walletName)
                                + "-Coldcard-Config.txt"
                        exportwalletDialog.open()
                        break;
                    case 1:
                    default:
                        qrcodeExportResult.open()
                        QMLHandle.sendEvent(EVT.EVT_SHARED_WALLET_EXPORT_QRCODE, RoomWalletData.walletId)
                        break;
                    }
                }
            }
        }

        QQrExportResult {
            id: qrcodeExportResult
            model: AppModel.qrExported
        }
    }

    FileDialog {
        id: exportwalletDialog
        property int exportFormat: -1
        fileMode: FileDialog.SaveFile
        onAccepted: {
            if(exportFormat === NUNCHUCKTYPE.COLDCARD){
                QMLHandle.sendEvent(EVT.EVT_SHARED_WALLET_EXPORT_COLDCARD, exportwalletDialog.currentFile)
            }
        }
    }
}
