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
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Chats"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../Components/customizes/Transactions"
import "../../Components/customizes/Popups"
import "../OnlineMode/Inheritances"
import "../../../localization/STR_QML.js" as STR

QScreen {
    property var transactionInfo: AppModel.transactionInfo
    readonly property int confirmations: Math.max(0, (AppModel.blockHeight - transactionInfo.height) + 1)
    Loader {
        id: screenLoader
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        sourceComponent: {
            if (transactionInfo.status === NUNCHUCKTYPE.PENDING_SIGNATURES ||
                transactionInfo.status === NUNCHUCKTYPE.READY_TO_BROADCAST) {
                return inheritance_on_chain_transaction_details
            } else {                
                return inheritance_transaction_details_claimed
            }
        }
    }

    Component {
        id: inheritance_transaction_details_claimed
        QInheritanceTransactionDetailsClaimed {
           
        }
    }
    Component {
        id: inheritance_on_chain_transaction_details
        QInheritanceOnChainTransactionDetails {

        }
    }
    Connections {
        target: transactionInfo
        onNunchukTransactionChanged: {
            if (transactionInfo.status !== NUNCHUCKTYPE.PENDING_SIGNATURES &&
                transactionInfo.status !== NUNCHUCKTYPE.READY_TO_BROADCAST) {
                if (!_congra.opened) {
                    _congra.open()
                }
            }
        }
    }

    QPopupInfo{
        id:_congra
        title: STR.STR_QML_788
        contentText: STR.STR_QML_789
    }

    function urlActiveLink(){
        var activeLink = ""
        switch(AppSetting.primaryServer){
        case NUNCHUCKTYPE.MAIN:
            activeLink = EXPLORER_MAINNET + AppModel.transactionInfo.txid
            break;
        case NUNCHUCKTYPE.TESTNET:
            activeLink = EXPLORER_TESTNET + AppModel.transactionInfo.txid
            break;
        case NUNCHUCKTYPE.SIGNET:
            activeLink = EXPLORER_SIGNNET + AppModel.transactionInfo.txid
            break
        default: break
        }
        return activeLink
    }
}
