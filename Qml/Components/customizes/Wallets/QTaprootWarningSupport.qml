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
import QtQuick.Controls 1.4
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.12
import HMIEVENTS 1.0
import EWARNING 1.0
import QRCodeItem 1.0
import DataPool 1.0
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Chats"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../Components/customizes/QRCodes"
import "../../../Components/customizes/Popups"
import "../../../../localization/STR_QML.js" as STR


QOnScreenContentTypeA {
    id: _contentBip
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_661
    property var sandbox        : AppModel.newWalletInfo.sandbox
    content: Item {
        Column {
            id:_colum
            spacing: 24
            anchors.centerIn: parent
            Rectangle {
                width: 96;height: 96;
                radius: 48
                color: "#F5F5F5"
                anchors.horizontalCenter: parent.horizontalCenter
                QIcon {
                    iconSize: 60
                    anchors.centerIn: parent
                    anchors.horizontalCenter: parent.horizontalCenter
                    source: "qrc:/Images/Images/warning-dark.svg"
                }
            }
            QLato {
                width: 500
                height: 28
                anchors.horizontalCenter: parent.horizontalCenter
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: STR.STR_QML_1688
            }
            Repeater {
                id: _description
                width: parent.width
                readonly property var content_map: [
                    {width: 536, height: 168, headline:STR.STR_QML_1689, content: STR.STR_QML_1690, icon: "qrc:/Images/Images/1.Active.svg"   },
                    {width: 536, height: 84, headline:STR.STR_QML_1691, content: STR.STR_QML_1692, icon: "qrc:/Images/Images/2.Active.svg"   },
                ]
                model: content_map.length
                Rectangle {
                    id: _rect
                    property var _item: _description.content_map[index]
                    width: _item.width
                    height: _item.height
                    Row {
                        spacing: 8
                        QIcon {
                            iconSize: 24
                            id:_numicon
                            source: _item.icon
                            visible: source != ""
                        }
                        Column {
                            width: 500
                            height: _item.height
                            spacing: 8
                            QLato {
                                width: 500
                                text: _item.headline
                                font.weight: Font.DemiBold
                                horizontalAlignment: Text.AlignLeft
                                verticalAlignment: Text.AlignVCenter
                            }
                            QLato {
                                id: _term
                                width: 500
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
