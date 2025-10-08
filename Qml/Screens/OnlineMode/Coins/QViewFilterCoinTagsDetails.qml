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
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: ""
    onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
    property var utxoInfo: walletInfo.utxoInfo
    property bool isEditing: false
    extraTop: Item {
        width: popupWidth
        height: 112
        Rectangle {
            width: popupWidth
            height: 112 - 36
            color: "#D0E2FF"
            radius: 24
        }
        Rectangle {
            width: popupWidth
            height: 112 - 36
            color: "#D0E2FF"
            anchors.bottom: parent.bottom
        }
        Row {
            anchors {
                fill: parent
                leftMargin: 36
                topMargin: 8
            }
            width: 613
            height: coinname.isValid ? 52 : 72
            spacing: 16
            anchors.verticalCenter: parent.verticalCenter
            QBadge {
                width: 48
                height: 48
                radius: 48
                color: tag.coin_tag_color
                icon: "qrc:/Images/Images/select-color-dark.svg"
                font.weight: Font.Medium
                anchors.verticalCenter: parent.verticalCenter
            }
            Column {
                width: 500
                anchors.verticalCenter: parent.verticalCenter
                spacing: 2
                Item {
                    width: parent.width
                    height: isEditing ? (coinname.isValid ? 36 : 56) : 32
                    Row {
                        anchors.fill: parent
                        spacing: isEditing ? 12 : 4
                        QLato {
                            text: tag.coin_tag_name
                            height: 32
                            anchors.verticalCenter: parent.verticalCenter
                            font.weight: Font.Medium
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            font.pixelSize: 26
                            visible: !isEditing
                        }
                        QIconButton {
                            width: 24
                            height: 24
                            anchors.verticalCenter: parent.verticalCenter
                            icon: "qrc:/Images/Images/edit-dark.svg"
                            bgColor: "#D0E2FF"
                            visible: !isEditing
                            onClicked: {
                                isEditing = true
                            }
                        }
                        QTextInputBoxTypeB {
                            id: coinname
                            label: ""
                            boxWidth: 533
                            boxHeight: 36
                            isValid: true
                            textInputted: tag.coin_tag_name
                            anchors.verticalCenter: parent.verticalCenter
                            enabled: true
                            maxLength: 40
                            spacing: 1
                            validator: RegExpValidator {
                                regExp: /^[^\s]*$/  // Allows letters, numbers, and symbols, no spaces
                            }
                            visible: isEditing
                        }
                        QTextButton {
                            width: 59
                            height: 36
                            label.text: STR.STR_QML_835
                            label.font.pixelSize: 16
                            anchors.top: coinname.top
                            type: eTypeE
                            visible: isEditing
                            onButtonClicked: {
                                var input = {
                                    type: "coin-tag-change-name",
                                    coinTagId: tag.coin_tag_id,
                                    coinTagColor: tag.coin_tag_color,
                                    coinName: coinname.textInputted
                                }
                                QMLHandle.sendEvent(EVT.EVT_COIN_DETAILS_CONTROL_ENTER, input)
                            }
                        }
                    }
                }
                QLato {
                    height: 16
                    text: qsTr("%1 %2 (%3 %4)").arg(walletInfo.utxoFilterTag.count)
                    .arg(walletInfo.utxoFilterTag.count > 1 ? "coins" : "coin")
                    .arg(walletInfo.utxoFilterTag.totalDisplay)
                    .arg(RoomWalletData.unitValue)
                    font.weight: Font.Normal
                    font.pixelSize: 12
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    color: "#595959"
                }
            }
        }
        Connections {
            target: walletInfo
            onUpdateTagNameChanged: {
                if (isError) {
                    coinname.isValid = false
                    coinname.showError = true
                    coinname.errorText = STR.STR_QML_1513
                } else {
                    isEditing = false
                    coinname.isValid = true
                    coinname.showError = false
                    coinname.errorText = ""
                }
            }
        }
    }
    property var tag: walletInfo.coinTags.tag
    extraHeader: Item{}
    content: Item {
        id: _item
        QLato {
            text: STR.STR_QML_1446
            font.pixelSize: 16
            font.weight: Font.Normal
            anchors {
                top: parent.top
                topMargin: 112 - 36 + 228
                horizontalCenter: parent.horizontalCenter
            }
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            visible: _listView.count === 0
        }
        Column {
            visible: _listView.count > 0
            width: _item.width
            anchors {
                top: parent.top
                topMargin: 112 - 36
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
                        checked: walletInfo.utxoFilterTag.isChecked
                        partiallyChecked: walletInfo.utxoFilterTag.isPartiallyChecked
                        anchors.verticalCenter: parent.verticalCenter
                        onCheckboxClicked: {
                            walletInfo.utxoFilterTag.selectAll(!selectAllBox.checked)
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
                        rightMargin: 0
                        verticalCenter: parent.verticalCenter
                    }
                    layoutDirection: Qt.RightToLeft
                    spacing: 12
                    visible: walletInfo.utxoFilterTag.isPartiallyChecked
                    QLato {
                        text:   STR.STR_QML_1410.arg(walletInfo.utxoFilterTag.selectedCount)
                                                .arg((walletInfo.utxoFilterTag.selectedCount > 1) || (walletInfo.utxoFilterTag.selectedCount === 0) ? "s" : "")
                                                .arg(walletInfo.utxoFilterTag.amountDisplay)
                                                .arg(RoomWalletData.unitValue)
                        font.pixelSize: 16
                        font.weight: Font.Normal
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
                height: 456 - 24
                clip: true
                model: walletInfo.utxoFilterTag
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
                    outgoing_label: utxo_outgoing_label
                    outgoing_color: utxo_outgoing_color
                    coin_timelocklist: utxo_coin_timelocklist
                    coin_timelockbase: utxo_coin_timelockbase
                    coin_timelockCount: utxo_coin_timelockCount
                    coin_timelocked: utxo_coin_timeLocked
                    onCoinViewCheck : {
                        utxo_selected = !utxo_selected
                    }
                    onRemoveClicked: {
                        remote_tag.isAll = false
                        remote_tag.tx_id = utxo_txid
                        remote_tag.open()
                    }
                }
            }
        }
    }
    isShowLine: true
    bottomLeft: Row {
        spacing: 8
        QButtonTextLink {
            width: 97
            height: 48
            label: STR.STR_QML_059
            visible: walletInfo.utxoFilterTag.isUnChecked
            onButtonClicked: {
                closeTo(NUNCHUCKTYPE.CURRENT_TAB)
            }
        }
        QTextButton {
            width: 154
            height: 48
            label.text: STR.STR_QML_1451
            label.font.pixelSize: 16
            type: eTypeF
            visible: !walletInfo.utxoFilterTag.isUnChecked
            onButtonClicked: {
                if (walletInfo.utxoFilterTag.isChecked) {
                    remote_tag.isAll = true
                    remote_tag.open()
                }
                else {
                    remote_tag.isAll = false
                    remote_tag.open()
                }
            }
        }
    }
    bottomRight: Row {
        spacing: 8
        QBtnOptions {
            id: optionsBtn
            width: 102
            height: 48
            label: STR.STR_QML_1094
            labels: [STR.STR_QML_1423]
            icons: ["qrc:/Images/Images/Delete.svg"]
            colors:   [ "#CF4018"]
            enables:  [ true ]
            visibles: [ true ]
            visible: walletInfo.utxoFilterTag.isUnChecked
            funcs: [
                function(){
                    if (walletInfo.utxoFilterTag.count > 0) {
                        remote_tag.isAll = true
                        remote_tag.open()
                    }
                    else {
                        var input = {
                            type: "coin-delete-all-from-a-tag",
                            coin_id: tag.coin_tag_id
                        }
                        QMLHandle.sendEvent(EVT.EVT_COIN_DETAILS_CONTROL_ENTER, input)
                    }
                }
            ]
        }
        QTextButton {
            id: _createTxBtn
            width: 166
            height: 48
            label.text: STR.STR_QML_257
            label.font.pixelSize: 16
            type: eTypeB
            visible: !walletInfo.utxoFilterTag.isUnChecked
            onButtonClicked: {
                if (walletInfo.utxoFilterTag.selectedLockedCount > 0) {
                    _confirm.action = function(){
                        walletInfo.requestSyncSelectCoinForMakeTransaction({type:"request-make-transaction-tag"})
                        walletInfo.requestSyncSelectCoinForMakeTransaction({type:"request-unlock-coins-selected"})
                        QMLHandle.sendEvent(EVT.EVT_HOME_SEND_REQUEST, "selected")
                    }
                    _confirm.open()
                } else {
                    walletInfo.requestSyncSelectCoinForMakeTransaction({type:"request-make-transaction-tag"})
                    QMLHandle.sendEvent(EVT.EVT_HOME_SEND_REQUEST, "selected")
                }
            }
        }
    }
    QConfirmYesNoPopup {
        id: remote_tag
        title: STR.STR_QML_334
        property bool isAll: true
        property string tx_id: ""
        contentText: remote_tag.isAll ? STR.STR_QML_1424.arg(walletInfo.utxoFilterTag.count) : STR.STR_QML_1452.arg(tag.coin_tag_name)
        onConfirmNo: close()
        onConfirmYes: {
            close()
            var input = {}
            if (remote_tag.isAll) {
                input = {
                    type: "coin-delete-all-from-a-tag",
                    coin_id: tag.coin_tag_id
                }
                QMLHandle.sendEvent(EVT.EVT_COIN_DETAILS_CONTROL_ENTER, input)
            } else {
                if (remote_tag.tx_id != "") {
                    input = {
                        type: "coin-delete-a-tag",
                        tx_id: remote_tag.tx_id
                    }
                    remote_tag.tx_id = ""
                    QMLHandle.sendEvent(EVT.EVT_COIN_DETAILS_CONTROL_ENTER, input)
                } else {
                    input = {
                        type: "coin-delete-selected-from-a-tag",
                        coin_id: tag.coin_tag_id
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
