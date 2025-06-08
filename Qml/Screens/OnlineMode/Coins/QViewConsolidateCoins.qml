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
import "../../../Components/customizes/Transactions"
import "../../../../localization/STR_QML.js" as STR

QOnScreenContentTypeB {
    id: _context
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_1425
    onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
    extraHeader: Item{}
    content: Item {
        id: _item
        Column {
            width: _item.width
            anchors {
                top: parent.top
                topMargin: -8
            }
            spacing: 24
            QLato {
                width: _item.width
                text: STR.STR_QML_1472
                font.pixelSize: 16
                font.weight: Font.Normal
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
            }
            QWalletAddress
            {
                address: walletInfo.unUseAddress
                width: 539
                address_width: 447
                qrCanClick: false
            }

            QTextAreaBoxTypeA  {
                id: _note
                boxWidth: 539
                boxHeight: 144
                label: STR.STR_QML_218
                enableLengthLimit:true
                isValid: true
                maxLength: 280
                input.verticalAlignment: Text.AlignTop
                input.wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            }
        }

        // Signing policy
        QTransactionSigningPolicy {
            id: signingPolicy
            width: parent.width
            height: parent.height
            // visible: true
            onCloseClicked : {

            }
            onContinueClicked : {
                var saved = [];
                var savedObj = {
                    "toAddress": walletInfo.unUseAddress,
                    "toAmount" : walletInfo.utxoList.amountDisplay
                };
                saved[0] = savedObj
                var destinationObj = {
                    "destinationList"    : saved,
                    "destinationMemo"    : _note.textInputted,
                    "use_script_path"    : use_script_path
                };
                walletInfo.requestConsolidateMakeTransaction(destinationObj)
            }
            onFeeSettingClicked: {
                // QMLHandle.sendEvent(EVT.EVT_SEND_FEE_SETTING_REQUEST)
                console.log("Fee Setting Clicked")
            }
        }

        function createTransaction() {
            var saved = [];
            var savedObj = {
                "toAddress": walletInfo.unUseAddress,
                "toAmount" : walletInfo.utxoList.amountDisplay
            };
            saved[0] = savedObj
            var destinationObj = {
                "destinationList"    : saved,
                "destinationMemo"    : _note.textInputted
            };
            walletInfo.requestConsolidateMakeTransaction(destinationObj)
        }

        Connections {
            target: transactionInfo
            function onRequestFeeSelection() {
                signingPolicy.open()
            }
        }

    }
    isShowLine: true
    onPrevClicked: {
        closeTo(NUNCHUCKTYPE.CURRENT_TAB)
    }

    onNextClicked: {
        _context.contentItem.createTransaction()
    }
}
