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
import HMIEVENTS 1.0
import DataPool 1.0
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

Item {
    id: _listItem
    property int itemIndex: 0
    property var listModel
    property bool selected: true

    height: 88
    Row {
        spacing: 12
        anchors.verticalCenter: parent.verticalCenter
        Item {
            width: 24
            height: 88
            QImage {
                source: {
                    var img = ""
                    if (itemIndex == 0) {
                        img = "qrc:/Images/Images/ancestry_unselected_begin.svg"
                    } else if (itemIndex > 0 && itemIndex < (maxCount - 1)) {
                        img = "qrc:/Images/Images/ancestry_unselected.svg"
                    } else {
                        img = "qrc:/Images/Images/ancestry_unselected_end.svg"
                    }
                    return img
                }
                width: sourceSize.width
                height: sourceSize.height
                anchors.verticalCenter: parent.verticalCenter
                visible: maxCount > 1
            }
            QImage {
                visible: selected
                source: "qrc:/Images/Images/ancestry_selected.svg"
                width: sourceSize.width
                height: sourceSize.height
                anchors.verticalCenter: parent.verticalCenter
            }
        }
        Flickable {
            width: _listItem.width - 36
            height: _listItem.height
            contentHeight: _listItem.height
            contentWidth: _row.childrenRect.width
            flickableDirection: Flickable.HorizontalFlick
            clip: true
            anchors.verticalCenter: parent.verticalCenter
            Row {
                id: _row
                spacing: 12
                anchors.verticalCenter: parent.verticalCenter
                Repeater {
                    id: ancestryRepeater
                    model: listModel
                    anchors.verticalCenter: parent.verticalCenter
                    QCoinAncestry {
                        anchors.verticalCenter: parent.verticalCenter
                        height: ancestryRepeater.count === 1 ? 64 : 80
                        isThis: utxo_txid === utxoInfo.txid
                        amount: utxo_amount
                        currency: qsTr("%1").arg(RoomWalletData.unitValue)
                        blocktime: utxo_blocktime
                        onCoinAncestryClicked: {
                            if (!isThis) {
                                var input = {
                                    type: "spent-coin-details",
                                    tx_id: utxo_txid,
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
