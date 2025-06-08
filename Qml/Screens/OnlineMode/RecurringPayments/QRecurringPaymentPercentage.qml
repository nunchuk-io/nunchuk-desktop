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
    property var payment_percentage: payment.payment_payload.valuePercent
    function isNumeric(str) {
      if (typeof str != "string") return false // we only process strings!
      return !isNaN(str) && // use type coercion to parse the _entirety_ of the string (`parseFloat` alone does not do this)...
             !isNaN(parseFloat(str)) &&  // ...and ensure strings of whitespace fail
              parseFloat(str) >= 0 && parseFloat(str) <= 100
    }
    RegExpValidator { id: percentValidator;   regExp: /^(?:0|[1-9][0-9]*)(\.\d{1,4})?$/ }
    content: Item {
        Column {
            anchors.fill: parent
            spacing: 24
            QLato {
                width: 539
                height: paintedHeight
                text: STR.STR_QML_1074
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                wrapMode: Text.WordWrap
                lineHeight: 28
                lineHeightMode: Text.FixedHeight
            }
            QTextInputBoxTypeB {
                id: _percent
                label: STR.STR_QML_1080
                boxWidth: 539
                boxHeight: 48
                input.placeholderText: "0.00%"
                input.validator: percentValidator
                onTypingFinished: {
                    if(!_percent.isValid){
                        _percent.isValid = true
                        _percent.errorText = ""
                    }
                    _percent.showError = false;
                    if (isNumeric(_percent.textInputted)) {
                        payment_percentage = _percent.textInputted
                    }
                    else {
                        _percent.errorText = "Value must >= 0.00% && <= 100%"
                        _percent.showError = true
                        _percent.isValid = false
                    }
                }
            }
        }
    }
    onPrevClicked: requestBack()
    bottomRight: Row {
        spacing: 12
        QTextButton {
            width: 157
            height: 48
            label.text: STR.STR_QML_1086
            label.font.pixelSize: 16
            type: eTypeB
            onButtonClicked: {
                var _input = {
                    type: "use-fixed-amount"
                }
                QMLHandle.sendEvent(EVT.EVT_RECURRING_PAYMENTS_ENTER, _input)
            }
        }
        QTextButton {
            width: 99
            height: 48
            label.text: STR.STR_QML_097
            label.font.pixelSize: 16
            type: eTypeE
            enabled: payment_percentage !== payment.payment_payload.valuePercent
            onButtonClicked: {
                var _input = {
                    type: "payment-percentage",
                    payment_type: "PERCENTAGE",
                    percent: payment_percentage
                }
                QMLHandle.sendEvent(EVT.EVT_RECURRING_PAYMENTS_ENTER, _input)
            }
        }
    }
}
