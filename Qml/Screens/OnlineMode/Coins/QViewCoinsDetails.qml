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

QOnScreenContentTypeB {
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_1412
    onCloseClicked: closeTo(NUNCHUCKTYPE.WALLET_TAB)
    property var utxoInfo: walletInfo.utxoInfo
    property var coinTags: utxoInfo.coinTags
    property var coinCollections: utxoInfo.coinCollections
    property var tx: utxoInfo.transaction
    extraHeader: Item {
        width: 318
        height: 24
        Row {
            spacing: 4
            QBadge {
                text: STR.STR_QML_1413
                border.width: 1
                border.color: "#EAEAEA"
                color: "#FFFFFF"
                font.weight: Font.Medium
                fontSize: 12
                anchors.verticalCenter: parent.verticalCenter
                visible: utxoInfo.isChange
            }
            QBadge {
                text: STR.STR_QML_1414
                color: "#EAEAEA"
                icon: "qrc:/Images/Images/secured-dark.svg"
                font.weight: Font.Medium
                fontSize: 12
                anchors.verticalCenter: parent.verticalCenter
                visible: utxoInfo.isLocked
            }
            QBadge {
                text: STR.STR_QML_1415
                color: "#EAEAEA"
                icon: "qrc:/Images/Images/scheduling-dark.svg"
                font.weight: Font.Medium
                fontSize: 12
                anchors.verticalCenter: parent.verticalCenter
                visible: utxoInfo.scheduleTimeDisplay != "--/--/----"
            }
        }
    }
    content: Item {
        Row {
            anchors {
                top: parent.top
                topMargin: 8
            }
            spacing: 28

            Rectangle {
                width: 350
                height: 479
                radius: 12
                color: "#D0E2FF"
                Flickable {
                    anchors.fill: parent
                    flickableDirection: Flickable.VerticalFlick
                    clip: true
                    contentHeight: contentDispLeft.height + 16
                    ScrollBar.vertical: ScrollBar { active: true }
                    Column {
                        id: contentDispLeft
                        width: parent.width - 32
                        anchors {
                            top: parent.top
                            topMargin: 16
                            horizontalCenter: parent.horizontalCenter
                        }
                        spacing: 24
                        Column {
                            width: parent.width
                            spacing: 12
                            QBadge {
                                text: STR.STR_QML_1413
                                border.width: 1
                                border.color: "#EAEAEA"
                                color: "#FFFFFF"
                                font.weight: Font.Medium
                                fontSize: 12
                                visible: utxoInfo.isChange
                            }
                            Column {
                                width: parent.width
                                spacing: 4
                                QLato {
                                    text: qsTr("%1 %2").arg(utxoInfo.amount).arg(RoomWalletData.unitValue)
                                    font.pixelSize: 24
                                    font.weight: Font.Medium
                                    horizontalAlignment: Text.AlignLeft
                                    verticalAlignment: Text.AlignVCenter
                                }
                                QLato {
                                    text: qsTr("%1 %2").arg(AppSetting.currency).arg(utxoInfo.amountCurrency)
                                    color: "#595959"
                                    font.pixelSize: 12
                                    font.weight: Font.Medium
                                    horizontalAlignment: Text.AlignLeft
                                    verticalAlignment: Text.AlignVCenter
                                }
                                Row {
                                    height: 16
                                    spacing: 4
                                    QLato {
                                        text: qsTr("%1").arg(utxoInfo.blocktimeDisplay)
                                        color: "#595959"
                                        font.pixelSize: 12
                                        font.weight: Font.Medium
                                        horizontalAlignment: Text.AlignLeft
                                        verticalAlignment: Text.AlignVCenter
                                        anchors.verticalCenter: parent.verticalCenter
                                    }
                                    QBadge {
                                        color: utxoInfo.outgoing.color
                                        text: utxoInfo.outgoing.label
                                        font.weight: Font.Bold
                                        font.pixelSize: 10
                                        anchors.verticalCenter: parent.verticalCenter
                                    }
                                }
                            }
                        }
                        Column {
                            spacing: 12
                            width: parent.width
                            QLato {
                                text: STR.STR_QML_1420
                                font.pixelSize: 16
                                font.weight: Font.Bold
                                horizontalAlignment: Text.AlignLeft
                                verticalAlignment: Text.AlignVCenter
                            }

                            Rectangle {
                                width: parent.width
                                height: _tran.childrenRect.height + 16*2
                                border.width: 1
                                border.color: "#DEDEDE"
                                color: "#FFFFFF"
                                radius: 12
                                Column {
                                    id: _tran
                                    anchors {
                                        fill: parent
                                        margins: 16
                                    }
                                    spacing: 4
                                    Row {
                                        spacing: 4
                                        width: parent.width
                                        height: 12
                                        QLato {
                                            text: STR.STR_QML_626
                                            color: "#595959"
                                            font.pixelSize: 12
                                            font.weight: Font.Normal
                                            horizontalAlignment: Text.AlignLeft
                                            verticalAlignment: Text.AlignVCenter
                                        }
                                        QLato {
                                            width: 80
                                            text: tx.destination
                                            color: "#595959"
                                            font.pixelSize: 12
                                            font.weight: Font.Normal
                                            horizontalAlignment: Text.AlignLeft
                                            verticalAlignment: Text.AlignVCenter
                                            elide: Text.ElideMiddle
                                        }
                                    }
                                    QLato {
                                        text: qsTr("%1 %2").arg((tx.isReceiveTx ? tx.subtotal : tx.total)).arg(RoomWalletData.unitValue)
                                        font.pixelSize: 16
                                        font.weight: Font.Normal
                                        horizontalAlignment: Text.AlignLeft
                                        verticalAlignment: Text.AlignVCenter
                                    }
                                    QCoinNoteBoder {
                                        id: _note
                                        memo: tx.memo
                                        width: parent.width
                                        visible: memo !== ""
                                    }
                                }
                            }
                        }
                    }
                }
            }
            Item {
                width: 350
                height: 479
                Column {
                    anchors.fill: parent
                    spacing: 24
                    Item {
                        width: parent.width
                        height: 48
                        Row {
                            spacing: 8
                            QIcon {
                                iconSize: 24
                                source: "qrc:/Images/Images/lock-dark.svg"
                                anchors.verticalCenter: parent.verticalCenter
                            }
                            QLato {
                                text: STR.STR_QML_1416
                                font.pixelSize: 16
                                font.weight: Font.Bold
                                anchors.verticalCenter: parent.verticalCenter
                            }
                            anchors.verticalCenter: parent.verticalCenter
                        }
                        QSwitchTypeB {
                            id: _lock
                            width: 84
                            height: 48
                            switchOn: utxoInfo.isLocked
                            anchors {
                                verticalCenter: parent.verticalCenter
                                right: parent.right
                            }
                            onButtonClicked: {
                                var input = {
                                    type: "coin-lock-change"
                                }
                                QMLHandle.sendEvent(EVT.EVT_COIN_DETAILS_CONTROL_ENTER, input)
                            }
                        }
                    }
                    Column {
                        width: parent.width
                        spacing: 12
                        Item {
                            width: parent.width
                            height: 24
                            Row {
                                spacing: 8
                                QIcon {
                                    iconSize: 24
                                    source: "qrc:/Images/Images/coin-tag-dark.svg"
                                    anchors.verticalCenter: parent.verticalCenter
                                }
                                QLato {
                                    text: STR.STR_QML_1417
                                    font.pixelSize: 16
                                    font.weight: Font.Bold
                                    anchors.verticalCenter: parent.verticalCenter
                                }
                                QLato {
                                    visible: coinTags.count > 0
                                    text: qsTr("(%1)").arg(coinTags.count)
                                    color: "#595959"
                                    font.pixelSize: 12
                                    font.weight: Font.Normal
                                    anchors.verticalCenter: parent.verticalCenter
                                }
                            }
                            QTextLink {
                                id: _edit
                                width: _edit.paintedWidth
                                height: 20
                                text: STR.STR_QML_849
                                anchors {
                                    verticalCenter: parent.verticalCenter
                                    right: parent.right
                                }
                                onTextClicked: {
                                    var input = {
                                        type: "edit-view-coin-tags"
                                    }
                                    QMLHandle.sendEvent(EVT.EVT_COIN_DETAILS_CONTROL_ENTER, input)
                                }
                            }
                        }
                        Rectangle {
                            width: parent.width
                            height: 155
                            border.width: 1
                            border.color: "#DEDEDE"
                            radius: 12
                            QLato {
                                anchors {
                                    left: parent.left
                                    leftMargin: 12
                                    top: parent.top
                                    topMargin: 12
                                    right: parent.right
                                    rightMargin: 12
                                }
                                text: STR.STR_QML_1444
                                font.pixelSize: 16
                                font.weight: Font.Normal
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                                visible: coinTags.count === 0
                            }
                            Flickable {
                                visible: coinTags.count > 0
                                anchors.fill: parent
                                anchors.margins: 12
                                clip: true
                                contentHeight: _flow.implicitHeight
                                interactive: contentHeight > height
                                flickableDirection: Flickable.VerticalFlick
                                contentY : contentHeight > height ? contentHeight - height : 0
                                ScrollBar.vertical: ScrollBar { active: true }
                                Item {
                                    width: parent.width
                                    height: _flow.childrenRect.height
                                    Flow {
                                        id: _flow
                                        spacing: 8
                                        width: parent.width
                                        Repeater {
                                            model: coinTags
                                            QCoinType {
                                                height: 36
                                                fontSize: 16
                                                coinColor: coin_tag_color
                                                coinText: coin_tag_name
                                                MouseArea {
                                                    anchors.fill: parent
                                                    onClicked: {
                                                        var input = {
                                                            type: "coin-filter-tag",
                                                            coin_id: coin_tag_id
                                                        }
                                                        QMLHandle.sendEvent(EVT.EVT_COIN_DETAILS_CONTROL_ENTER, input)
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    Column {
                        width: parent.width
                        spacing: 12
                        Item {
                            width: parent.width
                            height: 24
                            Row {
                                spacing: 8
                                anchors.verticalCenter: parent.verticalCenter
                                QIcon {
                                    iconSize: 24
                                    source: "qrc:/Images/Images/add-to-collection-dark.svg"
                                    anchors.verticalCenter: parent.verticalCenter
                                }
                                QLato {
                                    text: STR.STR_QML_1418
                                    font.pixelSize: 16
                                    font.weight: Font.Bold
                                    anchors.verticalCenter: parent.verticalCenter
                                }
                                QLato {
                                    visible: coinCollections.count > 0
                                    text: qsTr("(%1)").arg(coinCollections.count)
                                    color: "#595959"
                                    font.pixelSize: 12
                                    font.weight: Font.Normal
                                    anchors.verticalCenter: parent.verticalCenter
                                }
                            }
                            QTextLink {
                                width: paintedWidth
                                height: 20
                                text: STR.STR_QML_849
                                anchors {
                                    verticalCenter: parent.verticalCenter
                                    right: parent.right
                                }
                                onTextClicked: {
                                    var input = {
                                        type: "edit-view-coin-collections"
                                    }
                                    QMLHandle.sendEvent(EVT.EVT_COIN_DETAILS_CONTROL_ENTER, input)
                                }
                            }

                        }
                        Rectangle {
                            width: parent.width
                            height: 155
                            border.width: 1
                            border.color: "#DEDEDE"
                            radius: 12
                            QLato {
                                anchors {
                                    left: parent.left
                                    leftMargin: 12
                                    top: parent.top
                                    topMargin: 12
                                    right: parent.right
                                    rightMargin: 12
                                }
                                text: STR.STR_QML_1445
                                font.pixelSize: 16
                                font.weight: Font.Normal
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                                visible: coinCollections.count === 0
                            }
                            Flickable {
                                visible: coinCollections.count > 0
                                anchors.fill: parent
                                anchors.margins: 12
                                clip: true
                                contentHeight: _flowCollect.implicitHeight
                                interactive: contentHeight > height
                                flickableDirection: Flickable.VerticalFlick
                                contentY : contentHeight > height ? contentHeight - height : 0
                                ScrollBar.vertical: ScrollBar { active: true }
                                Item {
                                    width: parent.width
                                    height: _flowCollect.childrenRect.height
                                    Flow {
                                        id: _flowCollect
                                        spacing: 8
                                        width: parent.width
                                        Repeater {
                                            model: coinCollections
                                            QCoinText {
                                                label: coin_collection_name
                                                onCoinClicked: {
                                                    var input = {
                                                        type: "coin-filter-collection",
                                                        coin_id: coin_collection_id
                                                    }
                                                    QMLHandle.sendEvent(EVT.EVT_COIN_DETAILS_CONTROL_ENTER, input)
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    onPrevClicked: closeTo(NUNCHUCKTYPE.WALLET_TAB)
    bottomRight: Row {
        spacing: 12
        QBtnOptions {
            id: optionsBtn
            width: 102
            height: 48
            label: STR.STR_QML_1094
            labels: [STR.STR_QML_1474]
            icons: [""]
            colors:   [ "#031F2B"]
            enables:  [ true ]
            visibles: [ true ]
            funcs: [
                function(){
                    _info1.open()
                }
            ]
        }
        QTextButton {
            width: 184
            height: 48
            label.text: STR.STR_QML_1422
            label.font.pixelSize: 16
            type: eTypeB
            onButtonClicked: {
                var input = {
                    type: "coin-ancestry"
                }
                QMLHandle.sendEvent(EVT.EVT_COIN_DETAILS_CONTROL_ENTER, input)
            }
        }
    }

    QPopupInfo{
        id:_info1
        title: STR.STR_QML_1479
        contentText: utxoInfo.outpoint
        btnLabel: STR.STR_QML_1478
        contentWrapMode: Text.WrapAtWordBoundaryOrAnywhere
        onGotItClicked: {
            ClientController.copyMessage(utxoInfo.outpoint)
            AppModel.showToast(0, STR.STR_QML_1480, EWARNING.SUCCESS_MSG);
        }
    }
}
