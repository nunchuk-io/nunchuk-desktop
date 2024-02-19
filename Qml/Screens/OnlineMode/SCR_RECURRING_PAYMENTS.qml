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
import "../../Components/customizes"
import "../OnlineMode/RecurringPayments"
import "../../Components/customizes/services"
import "../../../localization/STR_QML.js" as STR

QScreen {
    property var payment: AppModel.walletInfo.recurringPayment.payment
    property int flow: AppModel.walletInfo.recurringPayment.flow
    readonly property var map_flow: [
        {flow_action: PaymentType.EMPTY_STATE,                          screen_component: _empty_state},
        {flow_action: PaymentType.ENTER_NAME_PAYMENT,                   screen_component: _enter_name},
        {flow_action: PaymentType.ENTER_FIXED_AMOUNT,                   screen_component: _fixed_amount},
        {flow_action: PaymentType.SELECT_DESTINATION_PAYMENT,           screen_component: _destination_payment},
        {flow_action: PaymentType.SELECT_IMPORT_FORMAT,                 screen_component: _import_format},
        {flow_action: PaymentType.ENTER_PERCENTAGE,                     screen_component: _percentage},
        {flow_action: PaymentType.SELECT_PERCENTAGE_CALCULATED,         screen_component: _percentage_calculated},
        {flow_action: PaymentType.ENTER_WHITELISTED_ADDRESS,            screen_component: _whitelisted_address},
        {flow_action: PaymentType.IMPORT_FIRST_ADDRESS_OF_WALLET,       screen_component: _import_first_address},
        {flow_action: PaymentType.ENTER_FREQUENCY_OF_PAYMENT,           screen_component: _frequency},
        {flow_action: PaymentType.SELECT_PLATFORM_KEY_CO_SIGN,          screen_component: _platform_key_co_sign},
        {flow_action: PaymentType.ENTER_TRANSACTION_NOTE,               screen_component: _tx_note},
        {flow_action: PaymentType.RECURRING_PAYMENTS_REVIEW,            screen_component: _review_create_payment},
        {flow_action: PaymentType.VIEWING_RECURRING_PAYMENTS,           screen_component: _viewing_payment},
        {flow_action: PaymentType.RECURRING_PAYMENTS_POPULATED_SATE,    screen_component: _populated_state},
        {flow_action: PaymentType.SELECT_FEE_RATE,                      screen_component: _fee_rate},
        {flow_action: PaymentType.VIEWING_RECURRING_PAYMENTS_ALERT,     screen_component: _review_create_payment_alert},
    ]
    function requestBack() {
        var _input = {
            type: "request-back",
        }
        QMLHandle.sendEvent(EVT.EVT_RECURRING_PAYMENTS_ENTER, _input)

    }

    Loader {
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        sourceComponent: map_flow.find(function(e) {if (e.flow_action === flow) return true; else return false}).screen_component
    }

    Component{
        id: _empty_state
        QRecurringPaymentEmptyState{}
    }
    Component{
        id: _enter_name
        QRecurringPaymentEnterName{}
    }
    Component{
        id: _fixed_amount
        QRecurringPaymentFixedAmount{}
    }
    Component{
        id: _destination_payment
        QRecurringPaymentDestinationPayment{}
    }
    Component{
        id: _frequency
        QRecurringPaymentFrequency{}
    }
    Component{
        id: _import_first_address
        QRecurringPaymentImportFirstAddress{}
    }
    Component{
        id: _import_format
        QRecurringPaymentImportFormat{}
    }
    Component{
        id: _percentage
        QRecurringPaymentPercentage{}
    }
    Component{
        id: _percentage_calculated
        QRecurringPaymentPercentageCalculated{}
    }
    Component{
        id: _platform_key_co_sign
        QRecurringPaymentPlatformKeyCoSign{}
    }
    Component{
        id: _populated_state
        QRecurringPaymentPopulatedState{}
    }
    Component{
        id: _review_create_payment
        QRecurringPaymentReview {
            isCancel: false
        }
    }
    Component{
        id: _review_create_payment_alert
        QRecurringPaymentReview {
            isAlert: true
        }
    }
    Component{
        id: _tx_note
        QRecurringPaymentTransactionNote{}
    }
    Component{
        id: _viewing_payment
        QRecurringPaymentReview {
            isCancel: true
        }
    }
    Component{
        id: _whitelisted_address
        QRecurringPaymentWhiteListedAddress{}
    }
    Component{
        id: _fee_rate
        QRecurringPaymentFeeRate{}
    }
}
