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
import QtQuick.Layouts 1.3
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Chats"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../Components/customizes/Popups"
import "../../../../localization/STR_QML.js" as STR

QOnScreenContentTypeB {
    id:_content
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_1312
    extraHeader: Item {}
    onCloseClicked: closeTo(NUNCHUCKTYPE.WALLET_TAB)
    property var dashboardInfo: GroupWallet.dashboardInfo
    property var listChecked: {
        var length = dashboardInfo.health.healthStatuses.length
        var ls = []
        for (var i = 0; i < length; i++) {
            ls.push(false)
        }
        return ls
    }
    property var listXfp: {
        var length = dashboardInfo.health.healthStatuses.length
        var ls = []
        for (var i = 0; i < length; i++) {
            ls.push("")
        }
        return ls
    }
    property bool change: false
    content: Item {
        Column {
            anchors.fill: parent
            spacing: 24
            QLato {
                width: parent.width
                height: 20
                text: STR.STR_QML_1313
                font.weight: Font.Normal
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }
            QListView {
                id: masterList
                width: parent.width
                height: parent.height - 24 - 20
                model: dashboardInfo.health.healthStatuses
                interactive: masterList.count > 3
                clip: true
                ScrollBar.vertical: ScrollBar { active: true }
                spacing: 16
                delegate: QKeyReminderDelegate {
                    width: ListView.view.width
                    height: visible ? 122 : 0
                    visible: modelData.keyinfo.type !== "SERVER" && (modelData.reminder ? false : true)
                    isChecked: listChecked[index]
                    onNotifyChecked: {
                        listChecked[index] = isChecked
                        listXfp[index] = isChecked ? modelData.keyinfo.xfp : ""
                        change = !change
                    }
                }
            }
        }
        QLine {
            width: parent.width + 36 *2
            anchors {
                bottom: parent.bottom
                horizontalCenter: parent.horizontalCenter
            }
        }
    }
    onPrevClicked: closeTo(NUNCHUCKTYPE.WALLET_TAB)
    nextEnable: {
        var isChange = change
        for(var i = 0; i < listChecked.length; i++) {
            if (listChecked[i]) {
                return true
            }
        }
        return false
    }

    onNextClicked: {
        var check = {
            type: "health-check-add-reminders-continue",
            checked: listChecked,
            xfps: listXfp
        }
        QMLHandle.sendEvent(EVT.EVT_KEY_HEALTH_CHECK_STATUS_ENTER, check)
    }
}
