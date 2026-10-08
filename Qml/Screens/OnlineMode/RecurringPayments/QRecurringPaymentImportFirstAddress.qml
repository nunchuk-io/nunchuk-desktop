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
import "../../../../localization/STR_QML.js" as STR

QOnScreenContentTypeA {
    id:_content
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_1068
    extraHeader: Item {}
    onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
    property string payment_bsms: payment.destination_payload.bsms
    property string payment_first_address: payment.destination_payload.first_address
    content: Item {
        Column {
            anchors {
                top: parent.top
            }
            spacing: 24
            QImportedFirstAddressOfWallet
            {
                width: 573
                address: payment_first_address
            }
            QImportedConfigurationDetails
            {
                width: 573
                description: payment_bsms
            }
            QIconTextButton {
                width: 266
                height: 48
                label: STR.STR_QML_1093
                icons: ["Delete.png", "Delete.png", "Delete.png","Delete.png"]
                fontPixelSize: 16
                iconSize: 16
                type: eTypeP
                onButtonClicked: requestBack()
            }
        }
    }
    onPrevClicked: requestBack()
    onNextClicked: {
        var _input = {
            type: "imported-first-address",
        }
        QMLHandle.sendEvent(EVT.EVT_RECURRING_PAYMENTS_ENTER, _input)
    }
}
