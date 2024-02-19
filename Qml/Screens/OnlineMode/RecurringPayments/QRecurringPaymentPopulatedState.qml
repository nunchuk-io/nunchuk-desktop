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

QOnScreenContentTypeB {
    id:_content
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_1065
    extraHeader: Item {}
    onCloseClicked: closeTo(NUNCHUCKTYPE.WALLET_TAB)
    property string myRole:         GroupWallet.dashboardInfo.myRole
    property var recurringPayment: AppModel.walletInfo.recurringPayment
    property string option: recurringPayment.sortOption
    property var paymentList: recurringPayment.paymentList
    property string recurring_payment_id: ""
    content: Item {
        QListView {
            id: _payments
            anchors {
                top: parent.top
                topMargin: 8
            }
            width: 728 + 12
            height: 480
            model: paymentList
            clip: true
            interactive: true
            ScrollBar.vertical: ScrollBar { active: true }
            delegate: QRecurringPaymentViewingDelegate {
                width: _payments.width - 12
                paymentDel: modelData
                onSignalSelectedPayment: {
                    var _input = {
                        type: "select-recurring-payment",
                        recurring_payment_id: payment_id
                    }
                    QMLHandle.sendEvent(EVT.EVT_RECURRING_PAYMENTS_ENTER, _input)
                }
            }
        }
    }
    isShowLine: true
    onPrevClicked: closeTo(NUNCHUCKTYPE.WALLET_TAB)
    bottomRight: Row {
        spacing: 12
        QIconTextButton {
            width: 92
            height: 48
            label: STR.STR_QML_1137
            icons: ["sort-dark.svg", "sort-dark.svg", "sort-dark.svg","sort-dark.svg"]
            fontPixelSize: 16
            iconSize: 16
            type: eTypeD
            onButtonClicked: {
                _sort_view.open()
            }
            Rectangle{
                width: 16
                height: 16
                radius: 8
                visible: option !== "newest"
                color: "#CF4018"
                anchors{
                    left: parent.right
                    leftMargin: -20
                    top: parent.top
                    topMargin: -6
                }

            }
        }
        QTextButton {
            width: 196
            height: 48
            label.text: STR.STR_QML_1066
            label.font.pixelSize: 16
            type: eTypeE
            visible: myRole !== "KEYHOLDER"
            onButtonClicked: {
                var _input = {
                    type: "add-recurring-payment",
                }
                QMLHandle.sendEvent(EVT.EVT_RECURRING_PAYMENTS_ENTER, _input)
            }
        }
    }
    QRecurringPaymentViewingSort {
        id: _sort_view
    }
}
