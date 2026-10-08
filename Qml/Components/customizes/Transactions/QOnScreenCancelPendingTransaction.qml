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
    label.text: STR.STR_QML_296
    label.font.pixelSize: 32
    label.font.weight: Font.Medium
    extraHeader: Row {}
    onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
    content: QContentCancelPendingTransaction {
        transactionInfo: AppModel.transactionInfo
        walletInfo: AppModel.walletInfo
    }

    bottomLeft: QTextButton {
        width: 118
        height: 48
        label.text: STR.STR_QML_035
        label.font.pixelSize: 16
        type: eTypeF
        onButtonClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
    }
    bottomRight: Row {
        height: 48
        spacing: 24
        QTextButton {
            width: 193
            height: 48
            label.text: STR.STR_QML_1180
            label.font.pixelSize: 16
            type: eTypeB
            onButtonClicked: {
                isCustomize = true
                new_fee_rate = contenCenter.contentItem.new_fee
            }
        }
        QTextButton {
            width: 254
            height: 48
            label.text: AppModel.walletInfo.tranReplace ? STR.STR_QML_784 : STR.STR_QML_235
            label.font.pixelSize: 16
            type: eTypeE
            enabled: contenCenter.contentItem.new_fee !== 0
            onButtonClicked: {
                requestCreateTransaction()
            }
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
                    "manualOutput"           : true,
                    "unUseAddress"           : AppModel.walletInfo.unUseAddress
        };
        timerCreateTx.destinationData = msg
        timerCreateTx.restart()
    }
}
