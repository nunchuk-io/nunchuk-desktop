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
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Chats"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../Components/customizes/QRCodes"
import "../../Components/customizes/Popups"
import "../../Components/customizes/Wallets"
import "../../../localization/STR_QML.js" as STR

QScreen {
    id:_screen
    property bool firstEnable: true
    readonly property int walletOptType: AppModel.newWalletInfo.walletOptType
    property var sandbox        : AppModel.newWalletInfo.sandbox
    QOnScreenContentTypeB {
        id:_content
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        label.text: STR.STR_QML_073
        extraHeader: Item {}
        onCloseClicked: closeTo(NUNCHUCKTYPE.WALLET_TAB)
        content: Item {
            clip: true
            Column {
                id: _cols
                width: 550
                spacing: 24
                QTextInputBoxTypeB {
                    id: _walletName
                    label: STR.STR_QML_025
                    boxWidth: _cols.width
                    boxHeight: 48
                    isValid: true
                    enableLengthLimit: true
                    maxLength: 20
                    textInputted: AppModel.newWalletInfo.walletName
                    onTextInputtedChanged: {
                        if(!_walletName.isValid){
                            _walletName.isValid = true
                            _walletName.errorText = ""
                        }
                        _walletName.showError = false;
                    }
                }
                QLine {
                    width: _cols.width
                }
                Flickable {
                    width: _cols.width
                    height: 400
                    contentHeight: _area.childrenRect.height
                    clip: true
                    Column {
                        id: _area
                        spacing: 24
                        QAddAddressType {
                            id: addressTypeSelection
                            width: 360
                            onSelectTypeOption: {
                                addressTypeSelection.typeOption = type
                            }
                        }
                        QLine {
                            width: 360
                            visible: walletOptType === NUNCHUCKTYPE.E_GROUP_WALLET
                        }
                        QAddWalletConfig {
                            id: walletConfig
                            width: 360
                            addressType: addressTypeSelection.typeOption
                            visible: walletOptType === NUNCHUCKTYPE.E_GROUP_WALLET
                        }
                    }
                }
            }
            function isEnable() {
                return _walletName.textInputted !== ""
            }

            function createWallet() {
                if(_walletName.textInputted !== ""){
                    var newObj = { "walletNameInputted"    : _walletName.textInputted,
                                         "addressType"     : addressTypeSelection.typeOption };
                    QMLHandle.sendEvent(EVT.EVT_ADD_WALLET_SIGNER_CONFIGURATION_REQUEST, newObj)
                }
                else{
                    _warning.open()
                }
            }
            function createGroupWallet() {
                if(_walletName.textInputted !== ""){
                    var config = walletConfig.findOpt()
                    var newObj = {
                        "type": "create-group-wallet",
                        "walletNameInputted": _walletName.textInputted,
                        "walletM"         : config.walletM,
                        "walletN"         : config.walletN,
                        "addressType"     : addressTypeSelection.typeOption };
                    QMLHandle.sendEvent(EVT.EVT_SETUP_GROUP_WALLET_REQUEST, newObj)
                }
                else{
                    _warning.open()
                }
            }
            function importQRScan() {
                if(AppModel.parseQRWallet(_walletName.textOutput, "", qrscaner.tags)){
                    qrscaner.close()
                    var importData = {  "qrTags"        : qrscaner.tags,
                                        "importType"    : fileDialog.iType,
                                        "walletname"    : _walletName.textOutput,
                                        "walletdescription"    : ""};
                    QMLHandle.sendEvent(EVT.EVT_ADD_WALLET_IMPORT, importData)
                }
            }
            function importFile() {
                var importData = {  "filePath"      : fileDialog.file,
                                    "importType"    : fileDialog.iType,
                                    "walletname"    : _walletName.textOutput,
                                    "walletdescription"    : ""};
                QMLHandle.sendEvent(EVT.EVT_ADD_WALLET_IMPORT, importData)
            }
        }
        onPrevClicked: closeTo(NUNCHUCKTYPE.WALLET_TAB)
        bottomRight: Row {
            spacing: 8
            QButtonLargeTail {
                id: importwallet
                width: 159
                height: 48
                label: STR.STR_QML_036
                type: eSECONDARY
                optionVisible: optionMenu.visible
                layoutDirection: Qt.RightToLeft
                visible: walletOptType == NUNCHUCKTYPE.E_PERSONAL_WALLET
                onButtonClicked: {
                    optionMenu.x = 20
                    optionMenu.y = 20 - optionMenu.height
                    optionMenu.open()
                }
                QContextMenu {
                    id: optionMenu
                    menuWidth: 350
                    labels: [
                        STR.STR_QML_037,
                        STR.STR_QML_038,
                        STR.STR_QML_040,
                        STR.STR_QML_041
                    ]
                    icons: [
                        "qrc:/Images/Images/import.png",
                        "qrc:/Images/Images/QRCodeScan.png",
                        "qrc:/Images/Images/import.png",
                        "qrc:/Images/Images/download.png"
                    ]
                    functions: [
                        function(){
                            fileDialog.iType = Popup_t.IMPORT_WALLET_DESCRIPTOR
                            fileDialog.open()
                        },
                        function(){
                            fileDialog.iType = Popup_t.IMPORT_WALLET_QRCODE
                            qrscaner.open()
                        },
                        function(){
                            fileDialog.iType = Popup_t.IMPORT_WALLET_CONFIGFILE
                            fileDialog.open()
                        },
                        function(){
                            fileDialog.iType = Popup_t.IMPORT_WALLET_DB
                            fileDialog.open()
                        },
                    ]
                    onItemClicked: {
                        functions[index]()
                    }
                }
            }
            QTextButton {
                width: 99
                height: 48
                label.text: STR.STR_QML_265
                label.font.pixelSize: 16
                type: eTypeE
                enabled: _content.contentItem.isEnable() || firstEnable
                onButtonClicked: {
                    firstEnable = false
                    if (walletOptType === NUNCHUCKTYPE.E_GROUP_WALLET) {
                        _content.contentItem.createGroupWallet()
                    } else {
                        _content.contentItem.createWallet()
                    }
                }
            }

        }
    }
    QPopupToast{
        id:_warning
        x:_content.x + 36
        y:_content.y + 520
        warningType:EWARNING.WARNING_MSG
        warningExplain:STR.STR_QML_587
    }
    QQrImportScanner {
        id: qrscaner
        onTagFound: {
            _content.contentItem.importQRScan()
        }
    }
    FileDialog {
        id: fileDialog
        property int iType: Popup_t.IMPORT_WALLET_DB
        fileMode: FileDialog.OpenFile
        onAccepted: {
            _content.contentItem.importFile()
        }
    }
}
