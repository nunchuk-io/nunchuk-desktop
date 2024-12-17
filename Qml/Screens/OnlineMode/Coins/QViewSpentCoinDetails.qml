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

QOnScreenContentTypeA {
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_1412
    onCloseClicked: closeTo(NUNCHUCKTYPE.WALLET_TAB)
    property var utxoInfo: walletInfo.utxoInfo
    property var tx: utxoInfo.transaction
    extraHeader: Item {
        width: 318
        height: 24
        Row {
            spacing: 4
            QBadge {
                text: STR.STR_QML_1477
                border.width: 1
                border.color: "#EAEAEA"
                color: "#FFFFFF"
                font.weight: Font.Medium
                anchors.verticalCenter: parent.verticalCenter
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
                color: "#EAEAEA"
                Column {
                    anchors {
                        fill: parent
                        margins: 16
                    }
                    spacing: 24
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
                                color: "#E8DAFF"
                                text: STR.STR_QML_1419
                                font.weight: Font.Black
                                font.pixelSize: 10
                                anchors.verticalCenter: parent.verticalCenter
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
                                QCoinNote {
                                    id: _note
                                    memo: tx.memo
                                    width: parent.width
                                    visible: memo !== ""
                                }
                            }
                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    QMLHandle.signalNotifySendEvent(EVT.EVT_HOME_TRANSACTION_INFO_REQUEST, tx.txid)
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    onPrevClicked: {
        var input = {
            type: "spent-coin-details-back"
        }
        QMLHandle.sendEvent(EVT.EVT_COIN_DETAILS_CONTROL_ENTER, input)
    }

    bottomRight: Item { }
}
