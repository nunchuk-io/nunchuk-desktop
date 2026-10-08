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
import "../../../../Components/origins"
import "../../../../Components/customizes"
import "../../../../Components/customizes/Chats"
import "../../../../Components/customizes/Texts"
import "../../../../Components/customizes/Buttons"
import "../../../../Components/customizes/Members"
import "../../../../../localization/STR_QML.js" as STR

Column {
    property var serverKeyInfo
    property var policies: serverKeyInfo.policies
    width: 539
    height: 20 + 16 + list.height
    spacing: 16
    QSwitchTypeLabel {
        id: _all_same
        label: STR.STR_QML_1032
        value: policies.apply_same_spending_limit
    }
    QListView {
        id: list
        visible: !_all_same.getValue()
        width: parent.width
        height: visible ? list.contentHeight : 0
        model: serverKeyInfo.spendingLimitCurrent
        interactive: false
        spacing: 16
        delegate: QPoliciesPendingLimitMemberDelegate {
            width: ListView.view.width
            member: modelData
            title: modelData.role === "MASTER" ? STR.STR_QML_949 : STR.STR_QML_1031.arg(1 + index)
        }
    }
    function apply_same_spending_limit() {
        return _all_same.getValue()
    }

    function list_spending_limit() {
        var children = list.contentItem.children
        var rets = []
        for(var i = 0; i < children.length; i++) {
            var spending_limit = children[i];
            if (typeof(spending_limit.member_spending_limit) === 'function') {
                rets.push(spending_limit.member_spending_limit())
            }
        }
        return rets
    }
}
