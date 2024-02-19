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
import "../../../localization/STR_QML.js" as STR

QScreen {
    QOnScreenContentTypeA {
        id:_content
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        label.text: STR.STR_QML_785
        extraHeader: QBadge {
            color: "#E8DAFF"
            text: STR.STR_QML_286
        }
        onCloseClicked: closeTo(NUNCHUCKTYPE.SERVICE_TAB)
        content: Item {
            QSendAddressArea {
                transactionInfo: AppModel.transactionInfo
                onAddrToVerify: {
                    displayAddressBusybox.addrToVerify = addr
                    QMLHandle.sendEvent(EVT.EVT_TRANSACTION_VERIFY_ADDRESS, addr)
                }
                onNewMemoNotify: {
                    QMLHandle.sendEvent(EVT.EVT_TRANSACTION_SET_MEMO_REQUEST, newMemo)
                }
            }
        }

        onPrevClicked: {
            QMLHandle.sendEvent(EVT.EVT_INHERITANCE_TRANSACTION_DETAILS_BACK)
        }
        bottomRight: QTextButton {
            width: 249
            height: 48
            label.text: STR.STR_QML_783
            label.font.pixelSize: 16
            type: eTypeB
            onButtonClicked: {
                Qt.openUrlExternally(urlActiveLink())
            }
        }
    }

    QPopupInfo{
        id:_congra
        title: STR.STR_QML_788
        contentText: STR.STR_QML_789
    }

    Component.onCompleted: {
        _congra.open()
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
            activeLink = AppSetting.signetStream + AppModel.transactionInfo.txid
            break
        default: break
        }
        return activeLink
    }
}
