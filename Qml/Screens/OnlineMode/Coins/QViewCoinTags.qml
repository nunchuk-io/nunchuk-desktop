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
    label.text: STR.STR_QML_1442
    onCloseClicked: prevClicked()
    property bool isEdit: false
    property bool isCreateTag: false
    property var coinTags: walletInfo.coinTags
    sameOffset: false
    offsetLeft: 36
    offsetRight: 8
    extraHeader: Item{}
    content: Flickable {
        id: _item
        clip: true
        contentWidth: _item.width
        contentHeight: _listView.contentHeight + (!isCreateTag ? 72 : 0)
        interactive: true
        ScrollBar.vertical: ScrollBar { active: true; width: 16 }
        Column {
            id: _column
            width: 728 + 20
            spacing: 0
            QCoinColorTagNewTag {
                id: _newTag
                width: 728
                height: 80
                visible: isCreateTag
                onDoneClicked: {
                    isCreateTag = false
                }
            }
            QListView {
                id: _listView
                width: 728
                height: _listView.contentHeight
                model: coinTags
                interactive: false
                delegate: QCoinColorTagDelegate {
                    id: _check
                    width: _listView.width
                    height: 80
                    coinColor: coin_tag_color
                    coinName: coin_tag_name
                    coinCount: coin_tag_count
                    isEdit: _context.isEdit
                    isChecked: coin_tag_checked
                    onCheckClicked: {
                        var input = {
                            type: "coin-tag-checked-unchecked",
                            coin_id: coin_tag_id,
                            checked: !_check.isChecked
                        }
                        QMLHandle.sendEvent(EVT.EVT_COIN_DETAILS_CONTROL_ENTER, input)
                    }
                    onArrowClicked: {
                        var input = {
                            type: "coin-filter-tag",
                            coin_id: coin_tag_id
                        }
                        QMLHandle.sendEvent(EVT.EVT_COIN_DETAILS_CONTROL_ENTER, input)
                    }
                }
            }
        }
        function updateY() {
            _item.contentY = 0
        }
        function refreshColor() {
            _newTag.refreshColor()
        }
    }
    isShowLine: true
    bottomRight: Row {
        spacing: 12
        QIconTextButton {
            width: 229
            height: 48
            label: STR.STR_QML_1443
            icons: ["add-dark.svg", "add-dark.svg", "add-dark.svg","add-dark.svg"]
            fontPixelSize: 16
            iconSize: 16
            type: eTypeB
            onButtonClicked: {
                isCreateTag = true
                _context.contentItem.refreshColor()
                _context.contentItem.updateY()
            }
        }
        QTextButton {
            width: 61
            height: 48
            label.text: STR.STR_QML_835
            label.font.pixelSize: 16
            type: eTypeB
            visible: isEdit
            enabled: totalCount !== coinTags.selectedCount
            onButtonClicked: {
                var input = {
                    type: inputType
                }
                QMLHandle.sendEvent(EVT.EVT_COIN_DETAILS_CONTROL_ENTER, input)
                saveClicked()
            }
        }
    }
    signal saveClicked()
    property string inputType: "add-coin-tags-to-wallet"
    property int totalCount: 0
    Component.onCompleted: {
        totalCount = coinTags.selectedCount
    }
}
