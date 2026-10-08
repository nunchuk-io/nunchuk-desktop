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
import Features.Claiming.ViewModels 1.0
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Chats"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../Components/customizes/QRCodes"

QOnScreenContentTypeA {
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    onCloseClicked: vm.close()
    label.text: STR.STR_QML_2085
    content: Item {
        Row {
            spacing: 36
            Rectangle {
                width: 346
                height: 512
                radius: 24
                color: "#D0E2FF"
                QPicture {
                    anchors.centerIn: parent
                    anchors.verticalCenter: parent.verticalCenter
                    source: "qrc:/Images/Images/verify-inheritance-key.svg"
                }
            }
            Item {
                width: 346
                height: 512
                Column {
                    width: parent.width
                    spacing: 0
                    Item {
                        width: 343
                        height: _signMessage.height
                        QTextInputBoxTypeB {
                            id: _signMessage
                            label: STR.STR_QML_2083
                            labelComponent.textFormat: Text.RichText
                            labelComponent.font.pixelSize: 12
                            boxWidth: parent.width
                            boxHeight: 120
                            textInputted: vm.message
                            enabled: false
                        }                        
                    }
                    QListView {
                        id: signerlist
                        width: 343
                        height: signerlist.count * 60
                        model: vm.signerList
                        delegate: QSingleSignerVerifyDelegate {
                            signerData {
                                single_name: singleSigner_name
                                single_type: single_signer_type
                                single_tag: single_signer_tag
                                single_devicetype: single_signer_devicetype
                                single_masterFingerPrint: singleSigner_masterFingerPrint
                                single_account_index: single_signer_account_index
                                single_checked: single_signer_checked
                                single_is_local: single_signer_is_local
                                single_device_cardid: single_signer_device_cardid
                                single_need_Topup_Xpub: single_signer_need_Topup_Xpub
                                single_signed_status: single_signer_signed_status
                            }
                            width: 343
                            onSignRequest: {
                                if (!vm.isMoreShow) {
                                    vm.onSignClicked()
                                } else {
                                    exportContextMenu.x = 20
                                    exportContextMenu.y = 20 - exportContextMenu.height
                                    exportContextMenu.open()
                                }
                            }

                            QMultiContextMenu {
                                id: exportContextMenu
                                menuWidth: 300
                                subMenuWidth: 300
                                property var exportMessage: [
                                    {
                                        visible: true,
                                        label: STR.STR_QML_114,
                                        icon: "",
                                        iconRight: "",
                                        color: "#031F2B",
                                        enable: true,
                                        subMenu: null,
                                        action: function() {
                                            exportContextMenu.close()
                                            qrcodeExportResult.open()
                                            vm.exportQRSignMessage()
                                        }
                                    },
                                    {
                                        visible: true,
                                        label: STR.STR_QML_2097,
                                        icon: "",
                                        iconRight: "",
                                        color: "#031F2B",
                                        enable: true,
                                        subMenu: null,
                                        action: function() {
                                            exportContextMenu.close()
                                            savefileDialog.action = function(){
                                                vm.exportFileSignMessage(savefileDialog.currentFile)
                                            }
                                            savefileDialog.currentFile = StandardPaths.writableLocation(StandardPaths.DocumentsLocation) + "/"
                                                    + vm.currentXfp
                                                    + ".txt"
                                            savefileDialog.open()

                                        }
                                    },
                                ]
                                property var importMessage: [
                                    {
                                        visible: true,
                                        label: STR.STR_QML_302,
                                        icon: "",
                                        iconRight: "",
                                        color: "#031F2B",
                                        enable: true,
                                        subMenu: null,
                                        action: function() {
                                            exportContextMenu.close()
                                            qrscaner.open()
                                        }
                                    },
                                    {
                                        visible: true,
                                        label: STR.STR_QML_677,
                                        icon: "",
                                        iconRight: "",
                                        color: "#031F2B",
                                        enable: true,
                                        subMenu: null,
                                        action: function(){
                                            exportContextMenu.close()
                                            openfileDialog.open()
                                        }
                                    },
                                ]
                                mapMenu: [
                                    {
                                        visible: true,
                                        label: STR.STR_QML_2095,
                                        icon: "qrc:/Images/Images/save-backup-dark.svg",
                                        iconRight: "qrc:/Images/Images/right-arrow-dark.svg",
                                        color: "#031F2B",
                                        enable: true,
                                        subMenu: exportMessage,
                                        action: function(){

                                        }
                                    },
                                    {
                                        visible: true,
                                        label: STR.STR_QML_2096,
                                        icon: "qrc:/Images/Images/ExportFile.svg",
                                        iconRight: "qrc:/Images/Images/right-arrow-dark.svg",
                                        color: "#031F2B",
                                        enable: true,
                                        subMenu: importMessage,
                                        action: function(){

                                        }
                                    }
                                ]
                            }
                        }
                    }
                }
            }
        }

    }
    onPrevClicked: vm.back()
    bottomRight: Row {
        spacing: 12
        QTextButton {
            width: label.paintedWidth + 32
            height: 48
            label.text: STR.STR_QML_097
            label.font.pixelSize: 16
            type: eTypeE
            enabled: vm.isVerified
            onButtonClicked: vm.next()
        }
    }

    FileDialog {
        id: savefileDialog
        property var action
        fileMode: FileDialog.SaveFile
        onAccepted: {
            if (action) {
                action()
            }
        }
    }
    FileDialog {
        id: openfileDialog
        fileMode: FileDialog.OpenFile
        onAccepted: {
            vm.importFileSignedMessage(openfileDialog.currentFile)
        }
    }

    QQrExportResult {
        id: qrcodeExportResult
        model: vm.qrExported
    }

    QQrImportScanner {
        id: qrscaner
        onTagFound: {
            if(qrscaner.complete) {
                vm.importQRSignedMessage(qrscaner.tags)
                qrscaner.close()
            }
        }
    }

    VerifyInheritanceKeyViewModel {
        id: vm
    }
}
