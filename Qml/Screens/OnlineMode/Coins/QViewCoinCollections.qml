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
    id: _context
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_1418
    property bool isEdit: false
    onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
    property var coinCollections: walletInfo.coinCollections
    extraHeader: Item{}
    content: Item {
        id: _item
        QListView {
            id: _listView
            width: parent.width
            height: parent.height
            model: coinCollections
            clip: true
            delegate: QCoinCollectionDelegate {
                id: _check
                width: _listView.width
                height: 80
                coinName: coin_collection_name
                coinCount: coin_collection_count
                isEdit: _context.isEdit
                isChecked: coin_collection_checked
                onCheckClicked: {
                    var input = {
                        type: "coin-collection-checked-unchecked",
                        coin_id: coin_collection_id,
                        checked: !_check.isChecked
                    }
                    QMLHandle.sendEvent(EVT.EVT_COIN_DETAILS_CONTROL_ENTER, input)
                }
                onArrowClicked: {
                    var input = {
                        type: "coin-filter-collection",
                        coin_id: coin_collection_id
                    }
                    QMLHandle.sendEvent(EVT.EVT_COIN_DETAILS_CONTROL_ENTER, input)
                }
            }
        }
    }
    isShowLine: true
    onPrevClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
    bottomRight: Row {
        spacing: 12
        QIconTextButton {
            width: 229
            height: 48
            label: STR.STR_QML_1449
            icons: ["add-dark.svg", "add-dark.svg", "add-dark.svg","add-dark.svg"]
            fontPixelSize: 16
            iconSize: 16
            type: eTypeB
            onButtonClicked: {
                var input = {
                    type: "create-a-new-collection-description",
                }
                QMLHandle.sendEvent(EVT.EVT_COIN_DETAILS_CONTROL_ENTER, input)
            }
        }
        QTextButton {
            width: 61
            height: 48
            label.text: STR.STR_QML_835
            label.font.pixelSize: 16
            type: eTypeE
            visible: isEdit
            enabled: totalCount !== coinCollections.selectedCount
            onButtonClicked: {
                var input = {
                    type: inputType
                }
                QMLHandle.sendEvent(EVT.EVT_COIN_DETAILS_CONTROL_ENTER, input)
            }
        }
    }
    property string inputType: "add-coin-collections-to-wallet"
    property int totalCount: 0
    Component.onCompleted: {
        totalCount = coinCollections.selectedCount
    }
}
