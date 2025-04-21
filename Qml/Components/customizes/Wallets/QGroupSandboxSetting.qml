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
import "../../../Components/customizes/Popups"
import "../../../../localization/STR_QML.js" as STR

QOnScreenContentTypeB {
    id:_content
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_157
    extraHeader: Item {}
    onCloseClicked: closeTo()
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
                textInputted: sandbox.groupName
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
                        typeOption: sandbox.addressType
                        isEnabled: sandbox.isCreate
                        onSelectTypeOption: {
                            addressTypeSelection.typeOption = type
                            if (type !== sandbox.addressType) {
                                _warning.open()
                            }
                        }
                        onTypeOptionChanged: {
                            if (typeOption !== sandbox.addressType) {
                                _warning.open()
                            }
                        }
                    }
                    QLine {
                        width: 360
                    }
                    QAddWalletConfig {
                        id: walletConfig
                        width: 360
                        isSetting: true
                        isEnabled: sandbox.isCreate
                        addressType: addressTypeSelection.typeOption
                    }
                }
            }
        }
        function typeOptionNo() {
            addressTypeSelection.typeOption = sandbox.addressType
        }

        function isEnable() {
            return _walletName.textInputted !== ""
        }

        function updateGroupWallet() {
            if(_walletName.textInputted !== ""){
                var config = walletConfig.findOpt()
                var newObj = {
                    "type": "update-group-wallet",
                    "walletNameInputted": _walletName.textInputted,
                    "walletM"         : config.walletM,
                    "walletN"         : config.walletN,
                    "addressType"     : addressTypeSelection.typeOption };
                QMLHandle.sendEvent(EVT.EVT_SETUP_GROUP_WALLET_ENTER, newObj)
            }
        }
    }
    onPrevClicked: closeTo()

    function closeTo() {
        var _input = {
            type: "setting-sandbox-close"
        }
        QMLHandle.sendEvent(EVT.EVT_SETUP_GROUP_WALLET_ENTER, _input)
    }

    bottomRight: Row {
        spacing: 8
        QTextButton {
            width: 99
            height: 48
            label.text: STR.STR_QML_265
            label.font.pixelSize: 16
            type: eTypeE
            enabled: _content.contentItem.isEnable()
            onButtonClicked: {
                _content.contentItem.updateGroupWallet()
            }
        }
    }
    QPopupInfoTwoButtons {
        id: _warning
        title: STR.STR_QML_661
        contentText: STR.STR_QML_1650
        labels: [STR.STR_QML_432,STR.STR_QML_097]
        types: [_btnBase.eTypeP, _btnBase.eTypeE]
        isVertical: false
        funcs: [
            function() {
                _content.contentItem.typeOptionNo()
            },
            function() {
                _warning.close()
            }
        ]
    }
}
