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
    property string option: payment.payment_payload.calculation_method
    onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
    property var percentages: [
        {id: "RUNNING_AVERAGE"},
        {id: "JUST_IN_TIME"}
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
                text: STR.STR_QML_1075
                anchors.left: parent.left
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }
            Column {
                id: _colum
                spacing: 16
                QRadioButtonTypeF {
                    width: 573
                    height: 116
                    labelMaxWidth: 500
                    selected: option === percentages[0].id
                    labelTop: STR.STR_QML_1082
                    labelBottom: STR.STR_QML_1083
                    onButtonClicked: { option = percentages[0].id; }
                }
                QRadioButtonTypeF {
                    width: 573
                    height: 116
                    labelMaxWidth: 500
                    selected: option === percentages[1].id
                    labelTop: STR.STR_QML_1084
                    labelBottom: STR.STR_QML_1085
                    onButtonClicked: { option = percentages[1].id; }
                }
            }
        }
    }
    onPrevClicked: requestBack()
    nextEnable: option !== payment.payment_payload.calculation_method
    onNextClicked: {
        var _input = {
            type: "select-percentage",
            payment_type: "PERCENTAGE",
            calculation_method: option
        }
        QMLHandle.sendEvent(EVT.EVT_RECURRING_PAYMENTS_ENTER, _input)
    }
}
