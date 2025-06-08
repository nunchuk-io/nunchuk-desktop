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
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_1470
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
            spacing: 0
            Item {
                width: popupWidth - 36 * 2
                height: 48
                Row {
                    spacing: 12
                    anchors.verticalCenter: parent.verticalCenter
                    QCheckBox {
                        id: selectAllBox
                        checked: walletInfo.utxoListLocked.isChecked
                        partiallyChecked: walletInfo.utxoListLocked.isPartiallyChecked
                        anchors.verticalCenter: parent.verticalCenter
                        onCheckboxClicked: {
                            walletInfo.utxoListLocked.selectAll(!selectAllBox.checked)
                        }
                    }
                    QLato {
                        text: selectAllBox.checked ? STR.STR_QML_1433 : STR.STR_QML_1409
                        font.pixelSize: 16
                        font.weight: Font.Bold
                        verticalAlignment: Text.AlignVCenter
                        anchors.verticalCenter: parent.verticalCenter
                        wrapMode: Text.WrapAnywhere
                    }
                }
            }
            QLine {
                width: popupWidth
                anchors {
                    left: parent.left
                    leftMargin: -36
                }
            }
            QListView {
                id: _listView
                anchors {
                    left: parent.left
                    leftMargin: -24
                }
                width: popupWidth - 24
                height: Math.min(_listView.contentHeight, 463)
                clip: true
                model: walletInfo.utxoListLocked
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
                    visible: utxo_coin_is_locked
                    outgoing_label: utxo_outgoing_label
                    outgoing_color: utxo_outgoing_color
                    allowRightClick: false
                    onCoinViewCheck : {
                        utxo_selected = !utxo_selected
                    }
                }
            }
        }
    }
    isShowLine: true
    bottomLeft: QTextButton {
        width: 80
        height: 48
        label.text: STR.STR_QML_245
        label.font.pixelSize: 16
        type: eTypeF
        visible: walletInfo.utxoListLocked.isUnChecked
        onButtonClicked: {
            closeTo(NUNCHUCKTYPE.CURRENT_TAB)
        }
    }
    bottomRight: Row {
        spacing: 8
        QBtnOptions {
            id: optionsBtn
            width: 102
            height: 48
            label: STR.STR_QML_1094
            labels: [
                STR.STR_QML_1426,
                STR.STR_QML_1427,
                STR.STR_QML_1428,
                STR.STR_QML_1429
            ]
            icons: [
                "",
                "",
                "",
                ""
            ]
            colors:   [
                "#031F2B",
                "#031F2B",
                "#031F2B",
                "#031F2B"
            ]
            enables:  [
                true,
                true,
                true,
                true
            ]
            visibles: [
                true,
                true,
                true,
                true
            ]
            funcs: [
                function(){
                    var input = {
                        type: "lock-coins"
                    }
                    walletInfo.requestForLockedAllCoins(input)
                },
                function(){
                    var input = {
                        type: "unlock-coins"
                    }
                    walletInfo.requestForLockedAllCoins(input)
                },
                function(){
                    var input = {
                        type: "edit-locked-multi-coin-collections"
                    }
                    QMLHandle.sendEvent(EVT.EVT_COIN_DETAILS_CONTROL_ENTER, input)
                },
                function(){
                    var input = {
                        type: "edit-locked-multi-coin-tags"
                    }
                    QMLHandle.sendEvent(EVT.EVT_COIN_DETAILS_CONTROL_ENTER, input)
                }
            ]
            visible: walletInfo.utxoListLocked.isPartiallyChecked
        }
        QTextButton {
            width: 154
            height: 48
            label.text: STR.STR_QML_257
            label.font.pixelSize: 16
            type: eTypeB
            visible: walletInfo.utxoListLocked.isPartiallyChecked
            onButtonClicked: {

            }
        }
        QTextButton {
            width: 154
            height: 48
            label.text: STR.STR_QML_1409
            label.font.pixelSize: 16
            type: eTypeB
            visible: walletInfo.utxoListLocked.isUnChecked
            onButtonClicked: {
                walletInfo.utxoListLocked.selectAll(true)
            }
        }
    }
}
