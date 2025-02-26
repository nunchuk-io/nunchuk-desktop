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
import "../../../Components/customizes/QRCodes"
import "../../../../localization/STR_QML.js" as STR

QOnScreenContentTypeB {
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: ""
    extraHeader: Item {}
    onCloseClicked: closeTo(NUNCHUCKTYPE.WALLET_TAB)
    property var newWalletInfo: AppModel.newWalletInfo
    property var sandbox: newWalletInfo.sandbox
    Item {
        anchors {
            left: parent.left
            leftMargin: 36
            top: parent.top
            topMargin: 36
        }

        width: 728
        height: 551
        Column {
            width: parent.width
            spacing: 24
            Rectangle {
                width: 96;height: 96;
                radius: 48
                color: "#F5F5F5"
                QIcon {
                    iconSize: 60
                    anchors.centerIn: parent
                    source: "qrc:/Images/Images/Wallet-registration.svg"
                }
            }
            QLato {
                width: 550
                height: 40
                text: STR.STR_QML_1647
                font.pixelSize: 32
                font.weight: Font.Medium
                verticalAlignment: Text.AlignVCenter
            }
            QLato {
                width: 550
                text: STR.STR_QML_1648
                verticalAlignment: Text.AlignVCenter
                lineHeightMode: Text.FixedHeight
                lineHeight: 28
                wrapMode: Text.WordWrap
            }
        }
    }
    onPrevClicked: {
        closeTo(NUNCHUCKTYPE.WALLET_TAB)
    }
    bottomRight: Row {
        spacing: 12
        QTextButton {
            width: 138
            height: 48
            label.text: STR.STR_QML_079
            label.font.pixelSize: 16
            type: eTypeB
            onButtonClicked: {
                closeTo(NUNCHUCKTYPE.WALLET_TAB)
            }
        }
        QButtonLargeTail {
            width: 211
            height: 48
            type: eTypeE
            label: STR.STR_QML_1649
            layoutDirection: Qt.RightToLeft
            onButtonClicked: {
                othersContextMenu.x = 20
                othersContextMenu.y = 20 - othersContextMenu.height
                othersContextMenu.open()
            }
            QMultiContextMenu {
                id: othersContextMenu
                menuWidth: 336
                property var exportMenu: [
                    {
                        visible: true,
                        label: STR.STR_QML_1659,
                        icon: "qrc:/Images/Images/QR-dark.svg",
                        iconRight: "",
                        color: "#031F2B",
                        enable: true,
                        subMenu: null,
                        action: function() {
                            qrcodeExportResult.filename = RoomWalletData.getValidFilename(sandbox.groupName) + "_BCUR2_Legacy"
                            qrcodeExportResult.open()
                            var _input = {
                                type: "export-config-as-QrCodeBCUR2_Legacy"
                            }
                            QMLHandle.sendEvent(EVT.EVT_SETUP_GROUP_WALLET_ENTER, _input)
                            othersContextMenu.close()
                        }
                    },
                    {
                        visible: true,
                        label: STR.STR_QML_1660,
                        icon: "qrc:/Images/Images/QR-dark.svg",
                        iconRight: "",
                        color: "#031F2B",
                        enable: true,
                        subMenu: null,
                        action: function(){
                            qrcodeExportResult.filename = RoomWalletData.getValidFilename(sandbox.groupName) + "_BCUR2"
                            qrcodeExportResult.open()
                            var _input = {
                                type: "export-config-as-QrCode-BCUR2"
                            }
                            QMLHandle.sendEvent(EVT.EVT_SETUP_GROUP_WALLET_ENTER, _input)
                            othersContextMenu.close()
                        }
                    },
                ]
                mapMenu: [
                    {
                        visible: true,
                        label: STR.STR_QML_1657,
                        icon: "qrc:/Images/Images/ios_share-24px.svg",
                        iconRight: "",
                        color: "#031F2B",
                        enable: true,
                        subMenu: null,
                        action: function(){
                            savefileDialog.currentFile = StandardPaths.writableLocation(StandardPaths.DocumentsLocation) + "/"
                                    + RoomWalletData.getValidFilename(sandbox.groupName)
                                    + ".bsms"
                            savefileDialog.open()
                        }
                    },
                    {
                        visible: true,
                        label: STR.STR_QML_1658,
                        icon: "qrc:/Images/Images/QR-dark.svg",
                        iconRight: "qrc:/Images/Images/right-arrow-dark.svg",
                        color: "#031F2B",
                        enable: true,
                        subMenu: exportMenu,
                        action: function(){ console.warn("export") }
                    },
                ]
            }
            QContextMenu {
                id: imExContextMenu
                menuWidth: 300
                labels: [
                    STR.STR_QML_1657,
                    STR.STR_QML_1658,
                ]
                icons: [
                    "qrc:/Images/Images/ios_share-24px.svg",
                    "qrc:/Images/Images/QR-dark.svg",
                ]
                functions: [
                    function() {
                        savefileDialog.currentFile = StandardPaths.writableLocation(StandardPaths.DocumentsLocation) + "/"
                                + RoomWalletData.getValidFilename(sandbox.groupName)
                                + ".bsms"
                        savefileDialog.open()
                    },
                    function() {
                        // "Export as QR"
                        qrcodeExportResult.filename = RoomWalletData.getValidFilename(sandbox.groupName) + "_BCUR2_Legacy"
                        qrcodeExportResult.open()
                        var _input = {
                            type: "export-config-as-QrCode"
                        }
                        QMLHandle.sendEvent(EVT.EVT_SETUP_GROUP_WALLET_ENTER, _input)
                    },
                ]
                onItemClicked: {
                    functions[index]()
                }
            }
        }
    }
    FileDialog {
        id: savefileDialog
        fileMode: FileDialog.SaveFile
        onAccepted: {
            var _input = {
                type: "export-config-as-a-file",
                file: savefileDialog.currentFile
            }
            QMLHandle.sendEvent(EVT.EVT_SETUP_GROUP_WALLET_ENTER, _input)
        }
    }
    QQrExportResultPDF {
        id: qrcodeExportResult
        model: AppModel.qrExported
    }
}
