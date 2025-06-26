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
import QRCodeItem 1.0
import DataPool 1.0
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Chats"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR


Column {
    property string destination_label: ""
    property string destination_address: ""
    property string destination_amount: ""
    property string destination_amount_currency: ""
    signal addrClicked(var addr)

    width: parent.width - 24
    spacing: 4
    Row {
        width: parent.width
        height: 16
        spacing: 4
        visible: destination_label !== ""
        QIcon {
            iconSize: 16
            source: "qrc:/Images/Images/fav_bookmark_24px.svg"
        }
        QText {
            width: parent.width - 20
            lineHeightMode: Text.FixedHeight
            lineHeight: 16
            wrapMode: Text.WrapAnywhere
            font.pixelSize: 12
            color: "#031F2B"
            font.family: "Lato"
            text: destination_label
            verticalAlignment: Text.AlignVCenter
        }
    }
    Row {
        width: parent.width
        spacing: 12
        QText {
            width: 192
            lineHeightMode: Text.FixedHeight
            lineHeight: 20
            wrapMode: Text.WrapAnywhere
            font.pixelSize: 16
            font.weight: Font.Bold
            color: "#031F2B"
            font.family: "Lato"
            text: destination_address
            verticalAlignment: Text.AlignVCenter
            MouseArea {
                id: btnMouseVerify
                visible: useMouseArea
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                anchors.fill: parent
                onClicked: {
                    addrClicked(destination_address)
                }
            }
        }
        Column {
            width: 122
            spacing: 4
            anchors.bottom: parent.bottom
            QText {
                width: parent.width
                lineHeightMode: Text.FixedHeight
                lineHeight: 20
                wrapMode: Text.WrapAnywhere
                font.pixelSize: 16
                font.weight: Font.Bold
                color: "#031F2B"
                font.family: "Lato"
                text: destination_amount + RoomWalletData.unitValue
                horizontalAlignment: Text.AlignRight
            }
            QText {
                width: parent.width
                lineHeightMode: Text.FixedHeight
                lineHeight: 16
                wrapMode: Text.WrapAnywhere
                font.pixelSize: 12
                color: "#031F2B"
                font.family: "Lato"
                text: qsTr("%1%2 %3")
                .arg(AppSetting.currencySymbol)
                .arg(destination_amount_currency)
                .arg(AppSetting.currency)
                horizontalAlignment: Text.AlignRight
                visible: !AppModel.walletInfo.isByzantineGuardian
            }
        }
    }
    Item {
        width: parent.width - 24
        height: 8
        Rectangle {
            width: parent.width
            height: 1
            color: "#DEDEDE"
            anchors.bottom: parent.bottom
        }
    }
}
