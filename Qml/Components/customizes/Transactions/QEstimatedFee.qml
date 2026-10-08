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
    z:1
    Item {
        anchors.verticalCenter: parent.verticalCenter
        width: 192
        height: 24
        z: 1
        Row {
            spacing: 6
            QLato {
                anchors.verticalCenter: parent.verticalCenter
                width: 97
                font.weight: Font.Normal
                text: STR.STR_QML_215
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
            }
            // QTooltip {
            //     tipWidth: 300
            //     pointerPositionIndex: 6
            //     pointerPositionRatio: 10
            //     toolTip: STR.STR_QML_807
            //     source: "qrc:/Images/Images/help_outline_24px.svg"
            // }
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
            text: transactionInfo.fee + RoomWalletData.unitValue
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
            .arg(transactionInfo.feeCurrency)
            .arg(AppSetting.currency)
            horizontalAlignment: Text.AlignRight
            visible: !AppModel.walletInfo.isByzantineGuardian
        }
    }
}
