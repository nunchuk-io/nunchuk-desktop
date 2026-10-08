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
    property string payment_currency: payment.payment_payload.currency
    property var payment_amount: payment.payment_payload.value
    function isNumeric(str) {
      if (typeof str != "string") return false // we only process strings!
      return !isNaN(str) && // use type coercion to parse the _entirety_ of the string (`parseFloat` alone does not do this)...
             !isNaN(parseFloat(str)) &&  // ...and ensure strings of whitespace fail
              parseFloat(str) >= 0
    }
    RegExpValidator { id: satValidator;      regExp: /^[1-9][0-9]*$/ }
    RegExpValidator { id: usdValidator;   regExp: /^(?:0|[1-9][0-9]*)(\.\d{1,2})?$/ }
    RegExpValidator { id: btcValidator;   regExp: /^(?:0|[1-9][0-9]*)(\.\d{1,8})?$/ }
    content: Item {
        Column {
            anchors.fill: parent
            spacing: 24
            QLato {
                width: 539
                height: paintedHeight
                text: STR.STR_QML_1071
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                wrapMode: Text.WordWrap
                lineHeight: 28
                lineHeightMode: Text.FixedHeight
            }
            Item {
                width: 539
                height: 72
                Row {
                    spacing: 16
                    QTextInputBoxTypeB {
                        id: _amount
                        label: STR.STR_QML_1073
                        boxWidth: 413
                        boxHeight: 48
                        input.placeholderText: payment_currency === "sat" ? "0" : "0.00"
                        input.validator: payment_currency === "sat" ? satValidator :
                                         payment_currency === "USD" ? usdValidator : btcValidator
                        onTypingFinished: {
                            if(!_amount.isValid){
                                _amount.isValid = true
                                _amount.errorText = ""
                            }
                            _amount.showError = false;
                            if (isNumeric(_amount.textInputted)) {
                                payment_amount = _amount.textInputted
                            }
                            else {
                                _amount.errorText = "Value must >= 0.00"
                                _amount.showError = true
                                _amount.isValid = false
                            }
                        }
                    }
                    QComboBox {
                        id: _currency
                        anchors.bottom: parent.bottom
                        width: 99
                        height: 48
                        current_id: {
                            return { id: qsTr("%1").arg(payment_currency),  displayName: qsTr("%1").arg(payment_currency) }
                        }
                        defaultValue: {
                            return { id: qsTr("USD"),  displayName: qsTr("USD") }
                        }
                        displayText: getDisplay().displayName
                        model: [
                            { id: qsTr("USD"),  displayName: qsTr("USD") },
                            { id: qsTr("BTC"),  displayName: qsTr("BTC") },
                            { id: qsTr("sat"),  displayName: qsTr("sat") },
                        ]
                        textRole: "displayName"
                        onActivated: {
                            if (currentIndex >=0) {
                                payment_currency = _currency.getDisplay().id
                            }
                        }
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
            label.text: STR.STR_QML_1070
            label.font.pixelSize: 16
            type: eTypeB
            onButtonClicked: {
                var _input = {
                    type: "use-percentage"
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
            enabled: payment_currency !== payment.payment_payload.currency || payment_amount !== payment.payment_payload.value
            onButtonClicked: {
                var _input = {
                    type: "payment-amount",
                    payment_type: "FIXED_AMOUNT",
                    currency: payment_currency === "" ? "USD" : payment_currency,
                    amount: payment_amount
                }
                QMLHandle.sendEvent(EVT.EVT_RECURRING_PAYMENTS_ENTER, _input)
            }
        }
    }
}
