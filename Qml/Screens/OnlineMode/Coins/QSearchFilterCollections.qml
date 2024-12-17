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

QPopupEmpty {
    id: _popup
    closePolicy:Popup.NoAutoClose
    content: QOnScreenContentTypeB {
        id: _context
        width: 600
        height: 600
        anchors.centerIn: parent
        label.text: STR.STR_QML_1418
        onCloseClicked: {
            coinCollections.cancelSelected()
            _popup.close()
        }
        property bool isEdit: true
        property var coinCollections: walletInfo.coinCollections
        extraHeader: Item{}
        content: Flickable {
            id: _item
            clip: true
            contentWidth: _item.width
            contentHeight: _column.childrenRect.height
            interactive: true
            Column {
                id: _column
                width: _item.width
                spacing: 0
                Row {
                    spacing: 12
                    anchors.right: parent.right
                    QLato {
                        text: selectAllBox.checked ? STR.STR_QML_1433 : STR.STR_QML_1409
                        font.pixelSize: 16
                        font.weight: Font.Bold
                        verticalAlignment: Text.AlignVCenter
                        anchors.verticalCenter: parent.verticalCenter
                        wrapMode: Text.WrapAnywhere
                    }
                    QCheckBox {
                        id: selectAllBox
                        checked: coinCollections.isChecked
                        partiallyChecked: coinCollections.isPartiallyChecked
                        onCheckboxClicked: {
                            coinCollections.selectAll(!coinCollections.isChecked)
                        }
                    }
                }
                QListView {
                    id: _listView
                    width: parent.width
                    height: _listView.count * 72
                    model: coinCollections
                    interactive: false
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
                    }
                }
                Item {
                    width: parent.width
                    height: 72
                }
            }
        }
        isShowLine: true
        onPrevClicked: {
            coinCollections.cancelSelected()
            _popup.close()
        }
        bottomRight: QTextButton {
            width: 74
            height: 48
            label.text: STR.STR_QML_194
            type: eTypeE
            onButtonClicked: {
                _popup.close()
            }
        }
    }
}

