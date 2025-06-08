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
    label.text: STR.STR_QML_948
    extraHeader: Item {}
    onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
    property var dashboardInfo: GroupWallet.dashboardInfo
    content: Item {
        Column {
            anchors.fill: parent
            spacing: 24
            QLato {
                width: parent.width
                height: 20
                text: dashboardInfo.hasWallet ? AppModel.walletInfo.walletName : STR.STR_QML_972
                font.weight: Font.Bold
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
                delegate: QSavingAccountDelegate {
                    width: ListView.view.width
                    height: visible ? 122 : 0
                    visible: modelData.keyinfo.type !== "SERVER"
                    onHealthCheck: {
                        if (modelData.keyinfo.type === "NFC") {
                            _info.open()
                            return
                        }
                        var check = {
                            type: "health-check",
                            xfp: modelData.xfp
                        }
                        QMLHandle.sendEvent(EVT.EVT_KEY_HEALTH_CHECK_STATUS_ENTER, check)
                    }
                    onRequestHealthCheck: {
                        if (modelData.keyinfo.type === "NFC") {
                            _info.open()
                            return
                        }
                        var check = {
                            type: "request-health-check",
                            xfp: modelData.xfp
                        }
                        QMLHandle.sendEvent(EVT.EVT_KEY_HEALTH_CHECK_STATUS_ENTER, check)
                    }
                    onViewHistoryClicked: {
                        dashboardInfo.health.viewHistoryClicked(modelData.xfp)
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

    onPrevClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
    bottomRight: QTextButton {
        width: 108
        height: 48
        label.text: STR.STR_QML_971
        label.font.pixelSize: 16
        type: eTypeB
        enabled: !AppModel.walletInfo.isLocked
        onButtonClicked: {
            dashboardInfo.health.editReminder = ""
            var check = {
                type: "health-check-reminders"
            }
            QMLHandle.sendEvent(EVT.EVT_KEY_HEALTH_CHECK_STATUS_ENTER, check)
        }
    }
    Connections {
        target: dashboardInfo.health
        onRequestHealthCheckSuccess: {
            var _continue = {
                type: "health-check-request-sent"
            }
            QMLHandle.sendEvent(EVT.EVT_KEY_HEALTH_CHECK_STATUS_ENTER, _continue)
        }
    }
    QPopupInfo{
        id:_info
        contentText: STR.STR_QML_998
        usingMin: true
    }
}
