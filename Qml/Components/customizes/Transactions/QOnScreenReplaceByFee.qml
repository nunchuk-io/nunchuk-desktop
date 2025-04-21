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
import QRCodeItem 1.0
import DataPool 1.0
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Chats"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../Components/customizes/Transactions"
import "../../../Components/customizes/Popups"
import "../../../Components/customizes/QRCodes"
import "../../../../localization/STR_QML.js" as STR

QOnScreenContent {
    id: contenCenter
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_209
    label.font.pixelSize: 32
    label.font.weight: Font.Medium
    extraHeader: Row {}
    onCloseClicked: closeTo(NUNCHUCKTYPE.WALLET_TAB)
    content: QContentReplaceByFee {
        transactionInfo: AppModel.transactionInfo
        myRole: AppModel.walletInfo.myRole
        onAddrToVerify: {
            displayAddressBusybox.addrToVerify = addr
            QMLHandle.sendEvent(EVT.EVT_TRANSACTION_VERIFY_ADDRESS, addr)
        }
        onNewMemoNotify: {
            QMLHandle.sendEvent(EVT.EVT_TRANSACTION_SET_MEMO_REQUEST, newMemo)
        }
        onTypingFinished: {
            var feeObj = { "feeRate" : currentText }
            QMLHandle.sendEvent(EVT.EVT_CREATE_TRANSACTION_MAKE_DRAFT_TX, feeObj)
        }
    }
    bottomLeft: QButtonTextLink {
        height: 24
        label: STR.STR_QML_059
        onButtonClicked: closeTo(NUNCHUCKTYPE.WALLET_TAB)
    }
    bottomRight: QTextButton {
        width: 293
        height: 48
        label.text: STR.STR_QML_1177
        label.font.pixelSize: 16
        type: eTypeE
        onButtonClicked: {
            requestCreateTransaction()
        }
    }
    Timer {
        id: timerCreateTx
        property var destinationData
        interval: 500
        onTriggered: {
            QMLHandle.sendEvent(EVT.EVT_CREATE_TRANSACTION_SIGN_REQUEST, timerCreateTx.destinationData)
        }
    }
    function requestCreateTransaction() {
        var msg = { "feeRate"                : contenCenter.contentItem.new_fee,
                    "manualFee"              : true,
                    "manualOutput"           : true};
        timerCreateTx.destinationData = msg
        timerCreateTx.restart()
    }
}
