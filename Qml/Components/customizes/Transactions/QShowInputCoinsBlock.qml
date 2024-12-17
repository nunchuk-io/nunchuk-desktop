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
import QtQuick 2.12
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0
import Qt.labs.platform 1.1
import HMIEVENTS 1.0
import DataPool 1.0
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Texts"
import "../../../Screens/OnlineMode/Coins"
import "../../../../localization/STR_QML.js" as STR

Column {
    width: parent.width
    spacing: 12
    Rectangle {
        width: parent.width
        height: 48
        color: "#F5F5F5"
        QText {
            text: STR.STR_QML_1489
            font.family: "Lato"
            font.weight: Font.Bold
            font.pixelSize: 12
            color: "#323E4A"
            anchors {
                left: parent.left
                leftMargin: 12
                verticalCenter: parent.verticalCenter
            }
        }
        QSwitchTypeB {
            id: showinputs
            width: 62
            height: 34
            switchOn: false
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 13
        }
    }
    QListView {
        id: _listView
        width: parent.width - 24
        height: _listView.contentHeight
        model: transactionInfo.inputCoins
        clip: true
        interactive: true
        visible: showinputs.switchOn
        delegate: QInputCoinDelegate {
            width: _listView.width
            amount: utxo_amount
            currency: qsTr("%1").arg(RoomWalletData.unitValue)
            blocktime: utxo_blocktime
            coin_tags: utxo_coin_tags
            coin_note: utxo_memo
            checked: utxo_selected
            visible: utxo_coin_visible
        }
    }
}
