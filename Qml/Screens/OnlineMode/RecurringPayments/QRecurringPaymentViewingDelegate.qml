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
import QRCodeItem 1.0
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../Components/customizes/QRCodes"
import "../../../../localization/STR_QML.js" as STR

Item {
    id: _delegate
    height: 88
    property bool enableFixedPrecision: AppSetting.enableFixedPrecision
    property var paymentDel
    property var payment_payload: paymentDel.payment_payload
    signal signalSelectedPayment(var payment_id)
    readonly property var repeats: [
        {id: "DAILY",             frequency: "day" },
        {id: "WEEKLY",            frequency: "week" },
        {id: "MONTHLY",           frequency: "month" },
        {id: "THREE_MONTHLY",     frequency: "three months" },
        {id: "SIX_MONTHLY",       frequency: "six months" },
        {id: "YEARLY",            frequency: "year" },
    ]
    property var freq: repeats.find(function(e) {if (e.id === paymentDel.frequency) return true; else return false}).frequency
    function payment_value() {
        if (paymentDel.payment_type === "FIXED_AMOUNT") {
            var str = "";
            var cur = paymentDel.payment_payload.currency
            var val = paymentDel.payment_payload.value
            if (cur === "sat") {
                str = qsTr("%1").arg(val.toFixed())
            }
            else if (cur === "USD") {
                str = qsTr("%1").arg(val.toFixed(2))
            }
            else {
                if (enableFixedPrecision) {
                    str = qsTr("%1").arg(val.toFixed(8))
                }
                else {
                    str = qsTr("%1").arg(val.toPrecision())
                }
            }
            return qsTr("%1 %2").arg(str).arg(cur)
        }
        else {
            var per = paymentDel.payment_payload.valuePercent
            return qsTr("%1% of wallet balance (*)").arg(per.toFixed(4))
        }
    }

    Item {
        height: _delegate.height - 12 * 2
        width: _delegate.width
        Column {
            spacing: 4
            QLato {
                width: _delegate.width - 42
                height: 20
                font.pixelSize: 16
                font.weight: Font.Normal
                text: paymentDel.name
                anchors.left: parent.left
                color: "#595959"
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }
            Item {
                width: _delegate.width
                height: 20
                QLato {
                    width: _delegate.width - 42
                    height: 20
                    text: qsTr("%1 %2 / %3").arg(paymentDel.payment_type === "FIXED_AMOUNT" ? payment_payload.currency : "").arg(payment_value()).arg(freq)
                    anchors.left: parent.left
                    font.weight: Font.DemiBold
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
                QIconButton {
                    iconSize: 24
                    anchors {
                        right: parent.right
                        rightMargin: 0
                        verticalCenter: parent.verticalCenter
                    }
                    icon: "qrc:/Images/Images/right-arrow-dark.svg"
                    onButtonClicked: {
                        signalSelectedPayment(paymentDel.id)
                    }
                    bgColor: "transparent"
                }
            }
            QLato {
                width: _delegate.width - 42
                height: 20
                font.pixelSize: 12
                font.weight: Font.Medium
                text: qsTr("%1: %2").arg(STR.STR_QML_1104).arg(paymentDel.start_date)
                anchors.left: parent.left
                color: "#595959"
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }
        }
    }
}
