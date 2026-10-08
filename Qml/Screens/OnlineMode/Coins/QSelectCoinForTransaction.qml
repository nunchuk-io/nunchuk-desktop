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
import "../../../Components/customizes/Popups"
import "../../../../localization/STR_QML.js" as STR

QOnScreenContentTypeA {
    id: _content
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: ""
    onCloseClicked: QMLHandle.sendEvent(EVT.EVT_CONSOLIDATE_COINS_MERGE_MAKE_TRANSACTION_REQUEST, {})
    property var transactionInfo: AppModel.transactionInfo
    extraTop: Item {
        id: header
        width: popupWidth
        height: 112
        Row {
            height: 48
            spacing: 10
            anchors {
                left: parent.left
                leftMargin: 36
                verticalCenter: parent.verticalCenter
            }
            QSearchingBoxTwo {
                id: _search
                width: 622
                onFilterTextChanged: {
                    var input = {
                        type: "filtering-text",
                        searchText: text
                    }
                    QMLHandle.sendEvent(EVT.EVT_COIN_DETAILS_CONTROL_ENTER, input)
                }
            }
            QIconButton {
                id: _filter
                anchors.verticalCenter: parent.verticalCenter
                width: 24
                height: 24
                iconSize: 24
                bgColor: "transparent"
                icon: "qrc:/Images/Images/filter-dark.svg"
                onClicked: {
                    var input = {
                        type: "filtering-search-coins-for-create-transaction"
                    }
                    QMLHandle.sendEvent(EVT.EVT_COIN_DETAILS_CONTROL_ENTER, input)
                }
                Rectangle {
                    width: 16
                    height: 16
                    color: "#CF4018"
                    radius: 16
                    visible: transactionInfo.manualCoins.isFiltered
                    anchors {
                        top: parent.top
                        topMargin: -8
                        right: parent.right
                        rightMargin: -8
                    }
                }
            }
        }
    }
    property bool isFilter: false
    property bool isEnabled: false
    extraHeader: Item{}
    content: Item {
        id: _item
        Column {
            width: _item.width
            anchors {
                top: parent.top
                topMargin: 80 - 36
            }
            spacing: 0
            Item {
                width: _item.width
                height: 48
                Row {
                    spacing: 12
                    anchors.verticalCenter: parent.verticalCenter
                    QCheckBox {
                        id: selectAllBox
                        checked: transactionInfo.manualCoins.isChecked
                        partiallyChecked: transactionInfo.manualCoins.isPartiallyChecked
                        anchors.verticalCenter: parent.verticalCenter
                        onCheckboxClicked: {
                            isEnabled = true
                            transactionInfo.manualCoins.selectAll(!selectAllBox.checked)
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
                Row {
                    anchors{
                        right: parent.right
                        rightMargin: -24
                        verticalCenter: parent.verticalCenter
                    }
                    layoutDirection: Qt.RightToLeft
                    spacing: 4
                    QLato {
                        id: _txt
                        width: 600
                        height: 16
                        text: STR.STR_QML_1508.arg(transactionInfo.manualCoins.amountDisplay)
                                                .arg(RoomWalletData.unitValue)
                                                .arg(transactionInfo.total)
                                                .arg(RoomWalletData.unitValue)
                        font.pixelSize: 16
                        font.weight: Font.Normal
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignRight
                        anchors.verticalCenter: parent.verticalCenter
                        wrapMode: Text.WrapAnywhere
                        textFormat: Text.RichText
                        onLinkActivated: {
                            isFilter = !isFilter
                        }
                        MouseArea {
                            anchors.fill: parent
                            cursorShape: _txt.hoveredLink ? Qt.PointingHandCursor : Qt.ArrowCursor
                            acceptedButtons: Qt.NoButton
                        }
                    }
                    Rectangle {
                        width: 16
                        height: 16
                        color: "#CF4018"
                        radius: 16
                        visible: isFilter
                        anchors.verticalCenter: parent.verticalCenter
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
                height: 456 - 24 + 32
                clip: true
                model: transactionInfo.manualCoins
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
                    visible: isFilter ? utxo_selected : utxo_coin_visible
                    outgoing_label: utxo_outgoing_label
                    outgoing_color: utxo_outgoing_color
                    allowRightClick: false
                    coin_timelocklist: utxo_coin_timelocklist
                    coin_timelockbase: utxo_coin_timelockbase
                    coin_timelockCount: utxo_coin_timelockCount
                    coin_timelocked: utxo_coin_timeLocked
                    onCoinViewCheck : {
                        isEnabled = true
                        utxo_selected = !utxo_selected
                    }
                }
            }
        }
    }
    isShowLine: true
    onPrevClicked: _content.closeClicked()
    bottomRight: Row {
        spacing: 8
        QTextButton {
            id: _createTxBtn
            width: 166
            height: 48
            label.text: STR.STR_QML_1499
            label.font.pixelSize: 16
            type: eTypeB
            enabled: isEnabled
            onButtonClicked: {
                if (transactionInfo.manualCoins.selectedLockedCount > 0) {
                    _confirm.action = function(){
                        walletInfo.requestSyncSelectCoinForMakeTransaction({type:"request-unlock-coins-selected-for-transaction"})
                        var input = {
                            type: "use-selected-coins-create-transaction"
                        }
                        QMLHandle.sendEvent(EVT.EVT_COIN_DETAILS_CONTROL_ENTER, input)
                    }
                    _confirm.open()
                } else {
                    var input = {
                        type: "use-selected-coins-create-transaction"
                    }
                    QMLHandle.sendEvent(EVT.EVT_COIN_DETAILS_CONTROL_ENTER, input)
                }
            }
        }
    }

    QConfirmYesNoPopup {
        id: _confirm
        title: STR.STR_QML_334
        property var action
        contentText: STR.STR_QML_1486
        onConfirmNo: close()
        onConfirmYes: {
            close()
            if(action) {
                action()
            }
        }
    }
}
