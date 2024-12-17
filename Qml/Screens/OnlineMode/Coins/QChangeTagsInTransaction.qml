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
import Qt.labs.platform 1.1
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
import "../../../Components/customizes/Popups"
import "../../../Components/customizes/services"
import "../../../../localization/STR_QML.js" as STR


QPopupEmpty {
    id: _change_tags
    closePolicy:Popup.NoAutoClose
    property bool isCreate: false
    property var walletInfo: AppModel.walletInfo
    property var coinTags: transactionInfo.parentCoinsTag
    content: QOnScreenContentTypeB {
        id: _content
        width: 600
        height: 600
        anchors.centerIn: parent
        label.text: ""
        extraHeader: Item {}
        extraTop: Item {
            width: 600
            height: 112
            QLato {
                width: 420
                text: STR.STR_QML_1509
                anchors {
                    verticalCenter: parent.verticalCenter
                    left: parent.left
                    leftMargin: 36
                }
                font.weight: Font.Normal
                lineHeight: 20
                wrapMode: Text.WordWrap
                lineHeightMode: Text.FixedHeight
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }
        }
        onCloseClicked: { _change_tags.close() }
        content: Item {
            anchors {
                top: parent.top
                topMargin: 74
            }
            Column {
                id: _column
                width: parent.width
                spacing: 0
                Item {
                    width: parent.width
                    height: 20
                    QLato {
                        width: 420
                        text: STR.STR_QML_1510
                        font.weight: Font.Bold
                        lineHeightMode: Text.FixedHeight
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }

                    QTextLink {
                        width: paintedWidth
                        height: 20
                        text: coinTags.isChecked ? STR.STR_QML_1433: STR.STR_QML_1409
                        anchors {
                            verticalCenter: parent.verticalCenter
                            right: parent.right
                        }
                        onTextClicked: {
                            coinTags.selectAll(!coinTags.isChecked)
                        }
                    }
                }

                QListView {
                    id: _listView
                    width: parent.width
                    height: 5 * 72 + 2
                    model: coinTags
                    clip: true
                    interactive: true
                    delegate: QCoinColorTagDelegate {
                        id: _check
                        width: parent.width
                        height: 80
                        coinColor: coin_tag_color
                        coinName: coin_tag_name
                        coinCount: coin_tag_count
                        isEdit: true
                        isChecked: coin_tag_checked
                        onCheckClicked: {
                            var input = {
                                type: "coin-tag-checked-unchecked",
                                coin_id: coin_tag_id,
                                checked: !_check.isChecked
                            }
                            walletInfo.requestSyncSelectCoinForMakeTransaction(input)
                        }
                    }
                }
            }
        }
        isShowLine: true
        onPrevClicked: { _change_tags.close() }
        bottomRight: Row {
            spacing: 12
            QTextButton {
                width: 182
                height: 48
                label.text: STR.STR_QML_1512
                label.font.pixelSize: 16
                type: eTypeB
                onButtonClicked: {
                    _change_tags.close()
                }
            }
            QTextButton {
                width: 113
                height: 48
                label.text: STR.STR_QML_1511
                label.font.pixelSize: 16
                type: eTypeE
                onButtonClicked: {
                    var input = {
                        type: "coin-tags-assign-tags-to-transaction"
                    }
                    walletInfo.requestSyncSelectCoinForMakeTransaction(input)
                    _change_tags.close()
                }
            }
        }
    }
}
