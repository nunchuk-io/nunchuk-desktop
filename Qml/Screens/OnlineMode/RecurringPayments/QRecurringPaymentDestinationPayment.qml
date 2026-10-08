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
    property string option: "DESTINATION_WALLET"
    onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
    property var destinations: [
        {id: "DESTINATION_WALLET"},
        {id: "WHITELISTED_ADDRESSES"}
    ]
    content: Item {
        Column {
            anchors {
                top: parent.top
                topMargin: -8
            }
            spacing: 24
            QLato {
                width: 539
                height: 28
                text: STR.STR_QML_1076
                anchors.left: parent.left
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }
            Column {
                id: _colum
                spacing: 16
                QRadioButtonTypeF {
                    width: 573
                    height: 144
                    labelMaxWidth: 500
                    selected: option === destinations[0].id
                    labelTop: STR.STR_QML_1077
                    labelBottom: STR.STR_QML_1078
                    onButtonClicked: { option = destinations[0].id; }
                }
                QRadioButtonTypeF {
                    width: 573
                    height: 116
                    labelMaxWidth: 500
                    selected: option === destinations[1].id
                    labelTop: STR.STR_QML_1079
                    labelBottom: STR.STR_QML_1081
                    onButtonClicked: { option = destinations[1].id; }
                }
            }
        }
    }
    onPrevClicked: requestBack()
    nextEnable: option !== ""
    onNextClicked: {
        var _input = {
            type: "select-destination",
            destination_type: option
        }
        QMLHandle.sendEvent(EVT.EVT_RECURRING_PAYMENTS_ENTER, _input)
    }
}
