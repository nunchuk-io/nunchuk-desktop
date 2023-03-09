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
pragma Singleton
import QtQuick 2.4
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0

QtObject {
    readonly property string unitValue: (AppSetting.unit === NUNCHUCKTYPE.SATOSHI) ? " sat" : " BTC"
    readonly property QtObject currentRoom: ClientController.rooms ? ClientController.rooms.currentRoom : null
    readonly property QtObject roomWallet : currentRoom ? currentRoom.roomWallet : null
    readonly property bool roomWalletReady: currentRoom && roomWallet
    readonly property bool roomWalletInitialized: walletInitId && roomWallet.walletInitId !== ""
    readonly property bool roomWalletCreated: roomWalletReady ? roomWallet.walletReady : false
    readonly property int typeSeleted: roomWalletReady ? parseInt(roomWallet.walletAddressType) : 3
    readonly property var typeSeletedStr: ["Any", "Legacy", "Nested Segwit", "Native Segwit"]
    readonly property string walletType: roomWalletReady ? roomWallet.walletEscrow ? "Escrow wallet" : "Standard wallet" : "Escrow wallet"
    readonly property string addressType: typeSeletedStr[typeSeleted]
    readonly property string multisigConfiguration: roomWalletReady ? (roomWallet.walletM + "/" + roomWallet.walletN) : "0/0"
    readonly property string walletName: roomWalletReady ? roomWallet.walletName : "No name"
    readonly property bool   fullSigners: roomWalletReady ? roomWallet.walletN === roomWallet.walletSignersCount : false
    readonly property int    pendingSigners: roomWalletReady ? roomWallet.walletN - roomWallet.walletSignersCount : 0
    readonly property bool   isCreator: roomWalletReady ? roomWallet.isCreator : true
    readonly property string walletInitId: roomWalletReady ? roomWallet.walletInitId : ""
    readonly property string walletId: roomWalletReady ? roomWallet.walletId : ""
    readonly property bool   isEscrow: roomWalletReady ? roomWallet.walletEscrow : false
    readonly property string amount: (roomWalletReady && currentRoom.roomWallet.info ? currentRoom.roomWallet.info.walletBalance : "0" ) + unitValue
    readonly property string amountBTC: (roomWalletReady && currentRoom.roomWallet.info ? currentRoom.roomWallet.info.walletBalanceBTC : "0" )
    readonly property string amountUSD: (roomWalletReady && currentRoom.roomWallet.info ? currentRoom.roomWallet.info.walletBalanceUSD : "0" )
    readonly property string amountCurrency: qsTr("$%1 USD").arg(amountUSD)

    function getValidFilename( name ){ return name.replace(/(\W+)/gi, '').trim()}
}
