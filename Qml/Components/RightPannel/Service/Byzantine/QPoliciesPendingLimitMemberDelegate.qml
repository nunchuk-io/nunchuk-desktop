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

import QtQuick 2.0
import "./../Common"
import "../../../customizes/Texts"
import "../../../RightPannel/Wallet/Dashboard"
import "../../../../../localization/STR_QML.js" as STR

Item {
    id: del
    width: 627
    height: 208
    property var member
    property var title: ""
    Column {
        anchors.fill: parent
        spacing: 12
        QLato {
            width: parent.width
            height: 20
            text: title
            font.weight: Font.DemiBold
            verticalAlignment: Text.AlignVCenter
        }
        Rectangle {
            width: 627
            height: 176
            radius: 12
            color: "#F5F5F5"
            Column {
                anchors.fill: parent
                anchors.margins: 16
                spacing: 16
                QMemberDelegate {
                    id: _member
                    width: 595
                    height: 60
                    roleHeight: 16
                    member: del.member
                }
                QPoliciesCoSigningSpendingLimit {
                    id: _limit
                    width: 595
                    height: 68
                    titleFontSize: 12
                    enabled: member.role !== "KEYHOLDER_LIMITED"
                    limit: del.member.spending_limit.limit
                    currency: del.member.spending_limit.currency
                    interval: del.member.spending_limit.interval
                }
            }
        }
    }
    function member_spending_limit() {
        var ret = {
            "membership_id" : del.member.membership_id,
            "spending_limit" : _limit.spending_limit(),
        }
        return ret
    }
}
