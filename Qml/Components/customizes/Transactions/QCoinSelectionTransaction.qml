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
import QtQuick.Controls 2.0
import DataPool 1.0
import HMIEVENTS 1.0
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../Screens/OnlineMode/Coins"
import "../../../../localization/STR_QML.js" as STR

Column {
    id: _content
    width: parent.width
    spacing: 16
    Item {
        height: 24
        width: parent.width
        Row {
            height: 24
            spacing: 8
            QIcon {
                iconSize: 24
                source: "qrc:/Images/Images/bitcoin-dark.svg"
                anchors.verticalCenter: parent.verticalCenter
            }
            QLato {
                width: 142
                text: STR.STR_QML_1497
                font.pixelSize: 16
                font.weight: Font.Bold
                anchors.verticalCenter: parent.verticalCenter
            }

        }
        QTextLink {
            id: _customize
            width: 80
            height: 20
            text: STR.STR_QML_1498
            anchors {
                right: parent.right
                verticalCenter: parent.verticalCenter
            }
            onTextClicked: {
                var input = {
                    type: "select-coin-for-create-transaction"
                }
                QMLHandle.sendEvent(EVT.EVT_COIN_DETAILS_CONTROL_REQUEST, input)
            }
        }
    }

    Rectangle {
        id: _rect
        width: parent.width
        height: _content.height - 24 - 16
        border.width: 1
        border.color: "#EAEAEA"
        radius: 12
        Column {
            id: _col
            anchors {
                fill: parent
                margins: 16
            }
            width: parent.width
            spacing: 16
            QLato {
                width: parent.width
                height: 16
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
                text: STR.STR_QML_1496
                font.pixelSize: 12
                font.weight: Font.Bold
            }

            QListView {
                id: _listView
                width: parent.width
                height: _rect.height - 16 - 16 - 16
                clip: true
                model: transactionInfo.inputCoins
                interactive: true
                ScrollBar.vertical: ScrollBar { active: true }
                delegate: QSingleCoinDetailDelegate {
                    width: _listView.width
                    amount: utxo_amount
                    currency: qsTr("%1").arg(RoomWalletData.unitValue)
                    amount_currency: utxo_amount_currency
                    blocktime: utxo_blocktime
                    coin_tags: utxo_coin_tags
                    coin_note: utxo_memo
                    checked: utxo_selected
                    isLocked: utxo_coin_is_locked
                    isScheduled: utxo_coin_is_scheduled
                    isChange: utxo_coin_is_change
                    visible: utxo_coin_visible
                    outgoing_label: utxo_outgoing_label
                    outgoing_color: utxo_outgoing_color
                    allowRightClick: false
                    showCheckBox: false
                    backgroundColor: "#F5F5F5"
                    isLarge: false
                }
            }
        }
    }
}
