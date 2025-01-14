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
import DataPool 1.0
import "../../../Components/customizes"
import "../../../Components/customizes/Texts"
import "../../../../localization/STR_QML.js" as STR

Row {
    width: parent.width - 24
    spacing: 12
    QLato {
        width: 192
        font.weight: Font.Normal
        text: STR.STR_QML_216
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignLeft
        anchors.verticalCenter: parent.verticalCenter
    }
    Column {
        width: 122
        spacing: 4
        QLato {
            width: parent.width
            lineHeightMode: Text.FixedHeight
            lineHeight: 20
            wrapMode: Text.WrapAnywhere
            font.pixelSize: 16
            font.weight: Font.Bold
            color: "#031F2B"
            font.family: "Lato"
            text: transactionInfo.total + RoomWalletData.unitValue
            horizontalAlignment: Text.AlignRight
        }
        QLato {
            width: parent.width
            lineHeightMode: Text.FixedHeight
            lineHeight: 16
            wrapMode: Text.WrapAnywhere
            font.pixelSize: 12
            color: "#031F2B"
            font.family: "Lato"
            text: qsTr("%1%2 %3")
            .arg(AppSetting.currencySymbol)
            .arg(transactionInfo.totalCurrency)
            .arg(AppSetting.currency)
            horizontalAlignment: Text.AlignRight
        }
    }
}
