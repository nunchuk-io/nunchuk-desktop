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
import "../../../../localization/STR_QML.js" as STR

Item {
    id: _item
    property string address: "bc1qft5swsj3nnm48qcpwvsqmht899gf6zk28tvvq6pdkjekaq3rltaqkztttq"
    property int address_width: 482
    property bool qrCanClick: false
    signal qrClicked(var address)
    height: _col.childrenRect.height
    Column {
        id: _col
        spacing: 12
        Item {
            width: parent.width
            height: 24
            QLato {
                font.weight: Font.Bold
                font.pixelSize: 16
                text: STR.STR_QML_1179
                anchors.verticalCenter: parent.verticalCenter
            }
        }
        Rectangle {
            width: _item.width
            height: 80
            color: "#F5F5F5"
            radius: 12
            Row {
                anchors.fill: parent
                anchors.margins: 16
                spacing: 12
                QQrIcon {
                    iconSize: 48
                    offset: 8
                    address: _item.address
                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: qrCanClick ? Qt.PointingHandCursor: Qt.CursorShape
                        onClicked: {
                            if (qrCanClick)
                                qrClicked(_item.address)
                        }
                    }
                }
                QLato {
                    width: address_width
                    height: 40
                    text: address
                    lineHeightMode: Text.FixedHeight
                    lineHeight: 20
                    wrapMode: Text.WrapAnywhere
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }
    }
}
