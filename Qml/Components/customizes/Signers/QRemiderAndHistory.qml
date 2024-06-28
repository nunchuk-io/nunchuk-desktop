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
import QtQuick.Controls 2.5
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import "../../../Components/origins"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../Components/customizes/Signers"
import "../../../../localization/STR_QML.js" as STR

Item {
    id: _item
    width: 350
    height: 452
    property var walletInfo: AppModel.walletInfo
    property var keyHealth: walletInfo.dashboardInfo.health.aKeyStatus
    signal reminderClicked()
    readonly property var repeats: [
        // {id: "FIVE_MINUTES",      frequency: STR.STR_QML_1326 },
        {id: "MONTHLY",           frequency: STR.STR_QML_1322 },
        {id: "THREE_MONTHLY",     frequency: STR.STR_QML_1323 },
        {id: "SIX_MONTHLY",       frequency: STR.STR_QML_1324 },
        {id: "YEARLY",            frequency: STR.STR_QML_1325 },
    ]
    Column {
        anchors.fill: parent
        spacing: 16
        Item {
            width: _item.width
            height: 24
            visible: keyHealth ? true : false
            QBadge {
                text: GlobalData.getHealthStatusLabel(keyHealth.lastState)
                color: GlobalData.getHealthStatusColor(keyHealth.lastState)
                icon: "qrc:/Images/Images/health-check-dark.svg"
                anchors.verticalCenter: parent.verticalCenter
                fontSize: 12
            }
            QTextLink {
                anchors.right: parent.right
                width: 71
                height: 20
                text: STR.STR_QML_1204
                anchors.verticalCenter: parent.verticalCenter
                onTextClicked: {
                    reminderClicked()
                }
            }
        }

        Rectangle {
            id: _rect
            visible: keyHealth ? (keyHealth.reminder ? true : false) : false
            color: "#F5F5F5"
            radius: 12
            height: 48
            width: _item.width
            property var freq: repeats.find(function(e) { if (e.id === keyHealth.reminder.frequency) return true; else return false })
            Row {
                width: parent.width - 12*2
                height: 24
                spacing: 8
                anchors {
                    left: parent.left
                    leftMargin: 12
                    verticalCenter: parent.verticalCenter
                }
                QImage {
                    width: 24
                    height: 24
                    anchors.verticalCenter: parent.verticalCenter
                    source: "qrc:/Images/Images/scheduling-dark.svg"
                }
                QLato {
                    text: _rect.freq.frequency
                    font.pixelSize: 12
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
            QTextLink {
                id: _edit
                width: _edit.paintedWidth
                height: 20
                text: STR.STR_QML_849
                anchors {
                    verticalCenter: parent.verticalCenter
                    right: parent.right
                    rightMargin: 12
                }
                onTextClicked: {
                    reminderClicked()
                }
            }
        }

        QHealthCheckHistory {
            hasReminder: keyHealth ? (keyHealth.reminder ? true : false) : false
        }
    }
}


