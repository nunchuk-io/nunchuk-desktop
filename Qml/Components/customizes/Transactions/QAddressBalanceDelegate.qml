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


Row {
    property string destination_address: ""
    property string destination_amount: ""
    property string destination_amount_currency: ""
    width: parent.width - 24
    spacing: 12
    signal addrClicked(var addr)
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
        anchors.verticalCenter: parent.verticalCenter
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
        }
    }
}
