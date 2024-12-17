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
    onCloseClicked: closeTo(NUNCHUCKTYPE.WALLET_TAB)
    property var utxoInfo: walletInfo.utxoInfo
    property int maxChar: 1
    function getFistCharterName(label) {
        if(label !== ""){
            if (maxChar == 1) {
                var ret = label.charAt(0)
                if(label.split(' ').length > 1){ ret += label.split(' ')[(label.split(' ').length - 1)].charAt(0)}
                return ret;
            } else {
                return label.substr(0,maxChar)
            }
        }
        return ""
    }
    property bool isEditing: false
    extraTop: Item {
        width: popupWidth
        height: 112
        Rectangle {
            width: popupWidth
            height: 112 - 36
            color: "#FDEBD2"
            radius: 24
        }
        Rectangle {
            width: popupWidth
            height: 112 - 36
            color: "#FDEBD2"
            anchors.bottom: parent.bottom
        }

        Row {
            anchors {
                fill: parent
                leftMargin: 36
                topMargin: 8
            }
            width: 613
            height: 48
            spacing: 16
            Item {
                width: 60
                height: 48
                anchors.verticalCenter: parent.verticalCenter
                Rectangle {
                    width: 48
                    height: 48
                    radius: 24
                    color: "#FDD95C"
                    QLato {
                        text: getFistCharterName(collection.coin_collection_name)
                        anchors.centerIn: parent
                        font.weight: Font.Bold
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    anchors.verticalCenter: parent.verticalCenter
                }
                Rectangle {
                    width: 24
                    height: 24
                    radius: 24
                    color: "#FFFFFF"
                    QImage {
                        width: 12
                        height: 12
                        anchors.centerIn: parent
                        source: "qrc:/Images/Images/camera-dark.svg"
                    }
                    anchors {
                        bottom: parent.bottom
                        right: parent.right
                    }
                }
            }
            Column {
                height: 48
                width: 500
                anchors.verticalCenter: parent.verticalCenter
                spacing: 0
                Item {
                    height: 32
                    width: parent.width
                    Row {
                        anchors.fill: parent
                        spacing: 4
                        QLato {
                            text: collection.coin_collection_name
                            anchors.verticalCenter: parent.verticalCenter
                            font.weight: Font.Medium
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                        QIconButton{
                            width: 24
                            height: 24
                            anchors.verticalCenter: parent.verticalCenter
                            icon: "qrc:/Images/Images/edit-dark.svg"
                            bgColor: "#FDEBD2"
                            onClicked: {

                            }
                        }
                    }
                    Row {
                        anchors.fill: parent
                        spacing: isEditing ? 12 : 4
                        QLato {
                            text: collection.coin_collection_name
                            anchors.verticalCenter: parent.verticalCenter
                            font.weight: Font.Medium
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            visible: !isEditing
                        }
                        QIconButton {
                            width: 24
                            height: 24
                            anchors.verticalCenter: parent.verticalCenter
                            icon: "qrc:/Images/Images/edit-dark.svg"
                            bgColor: "#FDEBD2"
                            visible: !isEditing
                            onClicked: {
                                isEditing = true
                            }
                        }
                        QTextInputBoxTypeA {
                            id: coinname
                            width: 533
                            height: 36
                            placeholderText: ""
                            borderColor: "#DEDEDE"
                            backgroundColor: "#FFFFFF"
                            text: collection.coin_collection_name
                            leftPadding: 12
                            activeFocusOnPress: true
                            anchors.verticalCenter: parent.verticalCenter
                            showEdit: false
                            enabled: true
                            visible: isEditing
                        }
                        QTextButton {
                            width: 59
                            height: 36
                            label.text: STR.STR_QML_835
                            label.font.pixelSize: 16
                            anchors.verticalCenter: parent.verticalCenter
                            type: eTypeE
                            visible: isEditing
                            onButtonClicked: {
                                isEditing = false
                                var input = {
                                    type: "coin-update-collection",
                                    coin_id: collection.coin_collection_id,
                                    coinName: coinname.text,
                                }
                                QMLHandle.sendEvent(EVT.EVT_COIN_DETAILS_CONTROL_ENTER, input)
                            }
                        }
                    }
                }
                QLato {
                    text: qsTr("%1 %2 (%3 %4)").arg(walletInfo.utxoFilterCollection.count)
                    .arg(walletInfo.utxoFilterCollection.count > 1 ? "coins" : "coin")
                    .arg(walletInfo.utxoFilterCollection.totalDisplay)
                    .arg(RoomWalletData.unitValue)
                    font.weight: Font.Normal
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    color: "#595959"
                }
            }
        }
    }
    property var collection: walletInfo.coinCollections.collection
    extraHeader: Item{}
    content: Item {
        id: _item
        QLato {
            text: STR.STR_QML_1447
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
                        checked: walletInfo.utxoFilterCollection.isChecked
                        partiallyChecked: walletInfo.utxoFilterCollection.isPartiallyChecked
                        anchors.verticalCenter: parent.verticalCenter
                        onCheckboxClicked: {
                            walletInfo.utxoFilterCollection.selectAll(!selectAllBox.checked)
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
                    visible: walletInfo.utxoFilterCollection.isPartiallyChecked
                    QLato {
                        text:   STR.STR_QML_1410.arg(walletInfo.utxoFilterCollection.selectedCount)
                                                .arg((walletInfo.utxoFilterCollection.selectedCount > 1) || (walletInfo.utxoFilterCollection.selectedCount === 0) ? "s" : "")
                                                .arg(walletInfo.utxoFilterCollection.amountDisplay)
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
                anchors.left: parent.left
                anchors.leftMargin: -36
            }
            QListView {
                id: _listView
                anchors {
                    left: parent.left
                    leftMargin: -24
                }
                width: popupWidth - 48
                height: 456 - 24
                clip: true
                model: walletInfo.utxoFilterCollection
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
                    allowRightClick: true
                    removeLabel: STR.STR_QML_1453
                    onCoinViewCheck : {
                        utxo_selected = !utxo_selected
                    }
                    onRemoveClicked: {
                        remote_collection.isAll = false
                        remote_collection.tx_id = utxo_txid
                        remote_collection.open()
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
            visible: walletInfo.utxoFilterCollection.isUnChecked
            onButtonClicked: {
                closeTo(NUNCHUCKTYPE.WALLET_TAB)
            }
        }
        QTextButton {
            width: 201
            height: 48
            label.text: STR.STR_QML_1453
            label.font.pixelSize: 16
            type: eTypeF
            visible: !walletInfo.utxoFilterCollection.isUnChecked
            onButtonClicked: {
                if (walletInfo.utxoFilterCollection.isChecked) {
                    remote_collection.isAll = true
                    remote_collection.open()
                }
                else {
                    remote_collection.isAll = false
                    remote_collection.open()
                }
            }
        }
        QTextButton {
            width: 225
            height: 48
            label.text: STR.STR_QML_1454
            label.font.pixelSize: 16
            type: eTypeF
            visible: !walletInfo.utxoFilterCollection.isUnChecked
            onButtonClicked: {
                var input = {
                    type: "coin-move-to-another-collection"
                }
                QMLHandle.sendEvent(EVT.EVT_COIN_DETAILS_CONTROL_ENTER, input)
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
            labels: [STR.STR_QML_1430, STR.STR_QML_1431]
            icons: ["qrc:/Images/Images/settings-dark.svg", "qrc:/Images/Images/Delete.svg"]
            colors:   [ "#031F2B", "#CF4018"]
            enables:  [ true , true]
            visibles: [ true , true]
            visible: walletInfo.utxoFilterCollection.isUnChecked
            funcs: [
                function(){
                    collection_rename.open()
                },
                function(){
                    remote_collection.isAll = true
                    remote_collection.open()
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
            visible: !walletInfo.utxoFilterCollection.isUnChecked
            onButtonClicked: {
                if (walletInfo.utxoFilterCollection.selectedLockedCount > 0) {
                    _confirm.action = function(){
                        walletInfo.requestSyncSelectCoinForMakeTransaction({type:"request-make-transaction-collection"})
                        walletInfo.requestSyncSelectCoinForMakeTransaction({type:"request-unlock-coins-selected"})
                        QMLHandle.sendEvent(EVT.EVT_HOME_SEND_REQUEST, "selected")
                    }
                    _confirm.open()
                } else {
                    walletInfo.requestSyncSelectCoinForMakeTransaction({type:"request-make-transaction-collection"})
                    QMLHandle.sendEvent(EVT.EVT_HOME_SEND_REQUEST, "selected")
                }
            }
        }
    }
    QConfirmYesNoPopup {
        id: remote_collection
        title: STR.STR_QML_334
        property bool isAll: true
        property string tx_id: ""
        contentText: remote_collection.isAll ? STR.STR_QML_1432.arg(walletInfo.utxoFilterCollection.count) : STR.STR_QML_1452.arg(collection.coin_collection_name)
        onConfirmNo: close()
        onConfirmYes: {
            close()
            var input = {}
            if (remote_collection.isAll) {
                input = {
                    type: "coin-delete-all-collection",
                    coin_id: collection.coin_collection_id
                }
                remote_collection.isAll = false
                QMLHandle.sendEvent(EVT.EVT_COIN_DETAILS_CONTROL_ENTER, input)
            } else {
                if (remote_collection.tx_id !== "") {
                    input = {
                        type: "coin-delete-a-collection",
                        tx_id: remote_collection.tx_id
                    }
                    remote_collection.tx_id = ""
                    QMLHandle.sendEvent(EVT.EVT_COIN_DETAILS_CONTROL_ENTER, input)
                } else {
                    input = {
                        type: "coin-delete-selected-collection",
                        coin_id: collection.coin_collection_id
                    }
                    QMLHandle.sendEvent(EVT.EVT_COIN_DETAILS_CONTROL_ENTER, input)
                }
            }
        }
    }
    QCollectionChangeName {
        id: collection_rename
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
