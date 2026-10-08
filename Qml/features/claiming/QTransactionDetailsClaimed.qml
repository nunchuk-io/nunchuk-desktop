/**************************************************************************
 * This file is part of the Nunchuk software (https://nunchuk.io/)        *
 * Copyright (C) 2020-2022 Enigmo                                         *
 * Copyright (C) 2022 Nunchuk                                             *
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
 **************************************************************************/
import QtQuick
import QtQuick.Controls
import Qt5Compat.GraphicalEffects
import DataPool 1.0
import NUNCHUCKTYPE 1.0
import Features.Transactions.ViewModels 1.0
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Chats"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../Components/customizes/QRCodes"
import "../../Components/customizes/Transactions"
import "../../Components/customizes/Popups"

QOnScreenContentTypeA {
    id:_content
    property var transactionInfo: vm.transactionInfo
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: QSTR.STR_QML_785
    readonly property int confirmations: Math.max(0, (AppModel.blockHeight - transactionInfo.height) + 1)
    extraHeader: QBadge {
        height: 24
        color: GlobalData.transactionColor(transactionInfo.status)
        text: GlobalData.transactionStatus(transactionInfo.status, confirmations)
    }
    onCloseClicked: {
        if (vm.isAddressFlow) {
            vm.close()
        } else {
            vm.closeToWalletTab()
        }            
    }
    content: Item {
        QSendAddressAreaClaimInheritance {
            transactionInfo: vm.transactionInfo
            onAddrToVerify: (addr) => {
                displayAddressBusybox.addrToVerify = addr
                vm.verifyAddress(addr)
            }
            onNewMemoNotify: (newMemo) => {
                vm.setMemo(newMemo)
            }
        }
    }
    onPrevClicked: vm.back()
    bottomRight: QTextButton {
        width: 249
        height: 48
        label.text: QSTR.STR_QML_783
        label.font.pixelSize: 16
        type: eTypeB
        onButtonClicked: {
            Qt.openUrlExternally(urlActiveLink())
        }
    }
    function urlActiveLink() {
        var activeLink = "";
        switch (AppSetting.primaryServer) {
        case NUNCHUCKTYPE.MAIN:
            activeLink = EXPLORER_MAINNET + transactionInfo.txid;
            break;
        case NUNCHUCKTYPE.TESTNET:
            activeLink = EXPLORER_TESTNET + transactionInfo.txid;
            break;
        case NUNCHUCKTYPE.SIGNET:
            activeLink = EXPLORER_SIGNNET + transactionInfo.txid;
            break;
        default:
            break;
        }
        return activeLink;
    }
    Connections {
        target: vm
        function onTransactionInfoChanged() {
            if (vm.transactionInfo && !_congra.opened) {
                _congra.open()
            }
        }
    }
    QPopupInfo{
        id:_congra
        title: QSTR.STR_QML_788
        contentText: QSTR.STR_QML_789
    }
    TransactionDetailsClaimedViewModel {
        id: vm
    }
}