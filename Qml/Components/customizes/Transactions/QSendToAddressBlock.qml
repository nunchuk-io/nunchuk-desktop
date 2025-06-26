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
    property bool useMouseArea: false
    width: parent.width
    spacing: 12
    Rectangle {
        width: parent.width
        height: 48
        color: "#F5F5F5"
        QText {
            text: STR.STR_QML_213
            font.family: "Lato"
            font.weight: Font.Bold
            font.pixelSize: 12
            color: "#323E4A"
            anchors {
                left: parent.left
                leftMargin: 12
                verticalCenter: parent.verticalCenter
            }
        }
    }
    // Destination infomation
    signal addressClicked(var addr)
    Repeater {
        id: lstDestination
        model: transactionInfo.destinationList
        width: parent.width
        QAddressBalanceDelegate {
            anchors {
                left: parent.left
                leftMargin: 12
            }
            width: parent.width
            destination_label: model.destination_label
            destination_address: model.destination_address
            destination_amount: model.destination_amount
            destination_amount_currency: model.destination_amount_currency
            onAddrClicked: {
                addressClicked(addr)
            }
        }
    }
    QEstimatedFee {
        anchors.horizontalCenter: parent.horizontalCenter
    }
    QTotalAmount {
        anchors.horizontalCenter: parent.horizontalCenter
    }
}

