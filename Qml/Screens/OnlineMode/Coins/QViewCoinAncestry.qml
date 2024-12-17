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
    label.text: STR.STR_QML_1475
    onCloseClicked: closeTo(NUNCHUCKTYPE.WALLET_TAB)
    property var utxoInfo: walletInfo.utxoInfo
    property var coinTags: utxoInfo.coinTags
    property var coinCollections: utxoInfo.coinCollections
    property var tx: utxoInfo.transaction
    property int maxCount: walletInfo.ancestryList.length
    extraHeader: Item {}
    content: Item {
        Column {
            anchors {
                top: parent.top
                topMargin: 8
            }
            spacing: 0
            Repeater {
                model: walletInfo.ancestryList
                QCoinAncestryListItem {
                    width: 728
                    listModel: modelData
                    itemIndex: index
                    selected: index == 0
                }
            }
        }
    }
    bottomLeft: Item {}
    bottomRight: Item {}
}
