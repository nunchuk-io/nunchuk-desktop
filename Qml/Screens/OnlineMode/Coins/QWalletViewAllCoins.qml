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
import NUNCHUCKTYPE 1.0
import QRCodeItem 1.0
import DataPool 1.0
import DRACO_CODE 1.0
import EWARNING 1.0
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Chats"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../Components/customizes/Popups"
import "../../../Components/customizes/Transactions"
import "../../../../localization/STR_QML.js" as STR

Rectangle {
    color: "#FFFFFF"
    id: _coinsroot
    property var walletInfo: AppModel.walletInfo
    QContextMenu {
        id: optionMenu
        menuWidth: 300
        icons: ["", "", ""]
        labels: {
            var ls = []
            ls.push(STR.STR_QML_1439)
            ls.push(STR.STR_QML_1440)
            ls.push(STR.STR_QML_1441)
            return ls
        }
        colors: {
            var ls = []
            ls.push("#031F2B")
            ls.push("#031F2B")
            ls.push("#031F2B")
            return ls
        }
        onItemClicked: {
            var _input = {}
            switch(index){
            case 0:
                _input = {
                    type: "view-coin-tags"
                }
                QMLHandle.sendEvent(EVT.EVT_COIN_DETAILS_CONTROL_REQUEST, _input)
                break;
            case 1:
                _input = {
                    type: "view-coin-collections"
                }
                QMLHandle.sendEvent(EVT.EVT_COIN_DETAILS_CONTROL_REQUEST, _input)
                break;
            case 2:
                if (walletInfo.utxoList.lockedCount === 0) {
                    _locked_coins.open()
                } else {
                    _input = {
                        type: "view-locked-coins"
                    }
                    QMLHandle.sendEvent(EVT.EVT_COIN_DETAILS_CONTROL_REQUEST, _input)
                }
                break;
            default:
                break;
            }
        }
    }
    property bool isSearching: false
    MouseArea {
        anchors.fill: parent
        onClicked: {
            if(isSearching) {
                isSearching = false
                searchbox.text = ""
            }
        }
    }

    Column {
        anchors.fill: parent

        // Header
        Item {
            id: header
            width: parent.width
            height: 120
            Column {
                width: parent.width

                Item {
                    width: parent.width
                    height: 24
                }
                Item {
                    width: parent.width
                    height: 48
                    Row {
                        anchors {
                            left: parent.left
                            leftMargin: 24
                            verticalCenter: parent.verticalCenter
                        }
                        spacing: 16
                        QImage {
                            source: "qrc:/Images/Images/left-arrow-dark.svg"
                            width: 24
                            height: 24
                            anchors.verticalCenter: parent.verticalCenter
                            scale: mouseedit.pressed ? 0.95 : 1
                            transformOrigin: Item.Center
                            MouseArea {
                                id: mouseedit
                                hoverEnabled: true
                                cursorShape: Qt.PointingHandCursor
                                anchors.fill: parent
                                onClicked: {
                                    walletInfo.isViewCoinShow = false
                                }
                            }
                        }
                        Row {
                            spacing: 8
                            anchors.verticalCenter: parent.verticalCenter
                            QLato {
                                text: STR.STR_QML_1408
                                font.pixelSize: 32
                                font.weight: Font.Medium
                                verticalAlignment: Text.AlignVCenter
                                anchors.verticalCenter: parent.verticalCenter
                                wrapMode: Text.WrapAnywhere
                            }
                            QLato {
                                text: qsTr("(%1)").arg(_listView.count)
                                color: "#595959"
                                font.pixelSize: 12
                                verticalAlignment: Text.AlignVCenter
                                anchors.top: parent.top
                                wrapMode: Text.WrapAnywhere
                                anchors.verticalCenter: parent.verticalCenter
                            }
                        }
                    }

                    Row {
                        anchors{
                            right: parent.right
                            rightMargin: 24
                            verticalCenter: parent.verticalCenter
                        }
                        layoutDirection: Qt.RightToLeft
                        spacing: 12
                        QIconButton {
                            width: 48
                            height: 48
                            bgColor: "#FFFFFF"
                            icon: "qrc:/Images/Images/more-horizontal-dark.svg"
                            onClicked: {
                                optionMenu.popup()
                            }
                        }
                        QIconButton {
                            width: 48
                            height: 48
                            bgColor: "#FFFFFF"
                            icon: "qrc:/Images/Images/search_24px.svg"
                            visible: !isSearching
                            onClicked: {
                                isSearching = true
                            }
                        }

                        QSearchingBox {
                            id: searchbox
                            visible: isSearching
                            anchors.verticalCenter: parent.verticalCenter
                            onFilterClicked: {
                                var input = {
                                    type: "filtering-search-coins"
                                }
                                QMLHandle.sendEvent(EVT.EVT_COIN_DETAILS_CONTROL_REQUEST, input)
                            }
                        }
                    }
                }
                Item {
                    width: parent.width
                    height: 48
                    Row {
                        anchors {
                            left: parent.left
                            leftMargin: 24
                            verticalCenter: parent.verticalCenter
                        }
                        spacing: 12
                        QCheckBox {
                            id: selectAllBox
                            checked: walletInfo.utxoList.isChecked
                            partiallyChecked: walletInfo.utxoList.isPartiallyChecked
                            onCheckboxClicked: {
                                walletInfo.utxoList.selectAll(!selectAllBox.checked)
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
                            rightMargin: 24
                            verticalCenter: parent.verticalCenter
                        }
                        layoutDirection: Qt.RightToLeft
                        spacing: 12
                        visible: !(isSearching && walletInfo.utxoList.countVisible === 0)
                        QLato {
                            text:   STR.STR_QML_1410.arg(walletInfo.utxoList.selectedCount)
                                                    .arg((walletInfo.utxoList.selectedCount > 1) || (walletInfo.utxoList.selectedCount === 0) ? "s" : "")
                                                    .arg(walletInfo.utxoList.amountDisplay)
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
                    width: parent.width
                }
            }
        }

        //Body
        Item {
            width: parent.width
            height: parent.height - header.height - footer.height
            Item {
                width: parent.width
                height: 132
                visible: isSearching && walletInfo.utxoList.countVisible === 0
                anchors {
                    horizontalCenter: parent.horizontalCenter
                    top: parent.top
                    topMargin: 24
                }
                Column {
                    anchors.fill: parent
                    spacing: 16
                    Rectangle {
                        width: 96;height: 96;
                        radius: 48
                        color: "#F5F5F5"
                        anchors.horizontalCenter: parent.horizontalCenter
                        QImage {
                            anchors.centerIn: parent
                            width: 60; height: 60;
                            source: "qrc:/Images/Images/not-found-dark.svg"
                        }
                    }
                    QLato {
                        width: parent.width
                        height: 20
                        text: STR.STR_QML_1455
                        font.pixelSize: 16
                        font.weight: Font.Normal
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                    }
                }
            }
            Column {
                anchors.fill: parent
                Item {
                    width: parent.width
                    height: 24
                    visible: isSearching && walletInfo.utxoList.countVisible > 0
                }
                QLato {
                    id: foundCoin
                    anchors {
                        left: parent.left
                        leftMargin: 24
                    }
                    visible: isSearching && walletInfo.utxoList.countVisible > 0
                    width: parent.width
                    height: 20
                    text: STR.STR_QML_1411.arg(walletInfo.utxoList.countVisible).arg(walletInfo.utxoList.countVisible > 1 ? "s" : "")
                    font.pixelSize: 16
                    font.weight: Font.Normal
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                }
                QListView {
                    id: _listView
                    width: parent.width
                    height: parent.height - ((isSearching && walletInfo.utxoList.countVisible > 0) ? (foundCoin.height + 24) : 0)
                    clip: true
                    model: walletInfo.utxoList
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
                        onCoinViewClicked: {
                            var input = {
                                type: "coin-details",
                                coin_index: index
                            }
                            QMLHandle.sendEvent(EVT.EVT_COIN_DETAILS_CONTROL_REQUEST, input)
                        }
                        onCoinViewCheck : {
                            utxo_selected = !utxo_selected
                        }
                    }
                }
            }
        }

        //Footer
        Rectangle {
            id: footer
            width: parent.width
            height: visible ? 80 : 24
            visible: walletInfo.utxoList.selectedCount > 0
            color: "#FFFFFF"
            QLine {
                width: parent.width
            }
            Row {
                anchors{
                    right: parent.right
                    rightMargin: 24
                    bottom: parent.bottom
                    bottomMargin: 16
                }
                spacing: 12
                layoutDirection: Qt.RightToLeft
                QTextButton {
                    id: _createTxBtn
                    width: 166
                    height: 48
                    label.text: STR.STR_QML_257
                    label.font.pixelSize: 16
                    type: eTypeB
                    onButtonClicked: {
                        if (walletInfo.utxoList.selectedLockedCount > 0) {
                            _confirm.action = function(){
                                walletInfo.requestSyncSelectCoinForMakeTransaction({type:"request-unlock-coins-selected"})
                                QMLHandle.sendEvent(EVT.EVT_HOME_SEND_REQUEST, "selected")
                            }
                            _confirm.open()
                        } else {
                            QMLHandle.sendEvent(EVT.EVT_HOME_SEND_REQUEST, "selected")
                        }
                    }
                }
                QBtnOptions {
                    id: optionsBtn
                    width: 102
                    height: 48
                    label: STR.STR_QML_1094
                    labels: [
                        STR.STR_QML_1425,
                        STR.STR_QML_1426,
                        STR.STR_QML_1427,
                        STR.STR_QML_1428,
                        STR.STR_QML_1429
                    ]
                    icons: ["","","","",""]
                    colors: [
                        "#031F2B",
                        "#031F2B",
                        "#031F2B",
                        "#031F2B",
                        "#031F2B"
                    ]
                    enables: [true, true, true, true, true]
                    visibles: [true, true, true, true, true]
                    funcs: [
                        function(){
                            if (walletInfo.utxoList.selectedLockedCount > 0) {
                                _confirm.action = function(){
                                    walletInfo.requestSyncSelectCoinForMakeTransaction({type:"request-unlock-coins-selected"})
                                    var input = {
                                        type: "view-consolidate-coins"
                                    }
                                    QMLHandle.sendEvent(EVT.EVT_COIN_DETAILS_CONTROL_REQUEST, input)
                                }
                                _confirm.open()
                            } else {
                                var input = {
                                    type: "view-consolidate-coins"
                                }
                                QMLHandle.sendEvent(EVT.EVT_COIN_DETAILS_CONTROL_REQUEST, input)
                            }
                        },
                        function(){
                            var input = {
                                type: "lock-coins"
                            }
                            walletInfo.requestForAllCoins(input)
                        },
                        function(){
                            var input = {
                                type: "unlock-coins"
                            }
                            walletInfo.requestForAllCoins(input)
                        },
                        function(){
                            var input = {
                                type: "edit-multi-coin-collections"
                            }
                            QMLHandle.sendEvent(EVT.EVT_COIN_DETAILS_CONTROL_REQUEST, input)
                        },
                        function(){
                            var input = {
                                type: "edit-multi-coin-tags"
                            }
                            QMLHandle.sendEvent(EVT.EVT_COIN_DETAILS_CONTROL_REQUEST, input)
                        }
                    ]
                }
            }
        }
    }
    QPopupInfo {
        id:_locked_coins
        title: STR.STR_QML_339
        contentText: STR.STR_QML_1471
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
