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
import QtGraphicalEffects 1.0
import Qt.labs.platform 1.1
import HMIEVENTS 1.0
import NUNCHUCKTYPE 1.0
import QRCodeItem 1.0
import DataPool 1.0
import DRACO_CODE 1.0
import EWARNING 1.0
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Chats"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../Components/customizes/Popups"
import "../../../../localization/STR_QML.js" as STR

Rectangle {
    id: coinItem
    height: visible ? body.height : 0
    property string amount: ""
    property string currency: "sat"
    property string blocktime: ""
    property var    coin_tags
    property string coin_note: ""
    property bool   checked: false
    property string backgroundColor: checked ? "#F5F5F5" : "#FFFFFF"

    signal coinViewClicked()
    color: backgroundColor

    Item {
        id: body
        width: parent.width - 24*2
        height: contents.height + 12*2
        anchors.centerIn: parent

        Row {
            id: contents
            width: parent.width
            spacing: 12
            anchors.verticalCenter: parent.verticalCenter
            Item {
                width: parent.width - 36
                height: tagContents.height
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    acceptedButtons: Qt.LeftButton | Qt.RightButton
                    onClicked: {
                        coinViewClicked()
                    }
                }
                Column {
                    id: tagContents
                    width: parent.width
                    spacing: 4
                    Item {
                        width: parent.width
                        height: 20
                        Row {
                            anchors.verticalCenter: parent.verticalCenter
                            spacing: 4
                            QLato {
                                text: qsTr("%1 %2").arg(amount).arg(currency)
                                font.pixelSize: 16
                                font.weight: Font.Bold
                                verticalAlignment: Text.AlignVCenter
                                anchors.verticalCenter: parent.verticalCenter
                            }
                        }
                    }
                    Item {
                        width: parent.width
                        height: 12
                        QLato {
                            text: qsTr("%1").arg(blocktime)
                            color: "#595959"
                            font.pixelSize: 12
                            font.weight: Font.Normal
                            verticalAlignment: Text.AlignVCenter
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }
                    Rectangle {
                        radius: 12
                        width: parent.width
                        height: tagAndMemoContent.height + 8*2
                        border.color: "#DEDEDE"
                        border.width: 1
                        color: backgroundColor
                        visible: coin_tags.count > 0 || coin_note !== ""
                        Column {
                            id: tagAndMemoContent
                            width: parent.width - 16
                            spacing: 8
                            anchors.centerIn: parent
                            Flow {
                                id: tagsflow
                                spacing: 8
                                width: parent.width
                                Repeater {
                                    model: coin_tags
                                    QCoinType {
                                        coinColor: coin_tag_color
                                        coinText: coin_tag_name
                                    }
                                }
                            }
                            QCoinNote {
                                visible: memo !== ""
                                memo: coin_note
                                width: parent.width
                            }
                        }
                    }
                }
            }
        }
    }
}
