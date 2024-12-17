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
    label.text: STR.STR_QML_1449
    onCloseClicked: closeTo(NUNCHUCKTYPE.WALLET_TAB)
    extraHeader: Item {}
    content: Item {
        Column {
            anchors {
                top: parent.top
                topMargin: 8
            }
            spacing: 24
            QImage {
                width: sourceSize.width
                height: sourceSize.height
                anchors.horizontalCenter: parent.horizontalCenter
                source: "qrc:/Images/Images/coin-collection.svg"
            }
            Column {
                width: parent.width
                spacing: 16
                QLato {
                    width: parent.width
                    text: STR.STR_QML_1516
                    font.pixelSize: 32
                    font.weight: Font.Medium
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
                QLato {
                    width: parent.width
                    text: STR.STR_QML_1517
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
                Repeater {
                    id: _guide
                    width: parent.width
                    readonly property var content_map: [
                        {height: 92,  headline:STR.STR_QML_1518, content: STR.STR_QML_1519 , icon: "qrc:/Images/Images/1.Active.svg" },
                        {height: 64,  headline:STR.STR_QML_1520, content: STR.STR_QML_1521 , icon: "qrc:/Images/Images/2.Active.svg" },
                    ]
                    model: content_map.length
                    Rectangle {
                        property var _item: _guide.content_map[index]
                        width: 728
                        height: _item.height
                        Row {
                            spacing: 12
                            QIcon {
                                iconSize: 24
                                id: _ico
                                source: _item.icon
                            }
                            Column {
                                width: 690
                                height: _item.height
                                spacing: 4
                                QLato {
                                    width: parent.width
                                    text: _item.headline
                                    font.weight: Font.Bold
                                    horizontalAlignment: Text.AlignLeft
                                    verticalAlignment: Text.AlignVCenter
                                }
                                QLato {
                                    id: _term
                                    width: parent.width
                                    text: _item.content
                                    lineHeightMode: Text.FixedHeight
                                    lineHeight: 28
                                    wrapMode: Text.WordWrap
                                    horizontalAlignment: Text.AlignLeft
                                    verticalAlignment: Text.AlignVCenter
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
            type: "edit-view-coin-collections"
        }
        QMLHandle.sendEvent(EVT.EVT_COIN_DETAILS_CONTROL_ENTER, input)
    }
    onNextClicked: {
        var input = {
            type: "create-a-new-collection"
        }
        QMLHandle.sendEvent(EVT.EVT_COIN_DETAILS_CONTROL_ENTER, input)
    }
}
