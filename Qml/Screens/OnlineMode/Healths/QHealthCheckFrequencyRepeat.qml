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

QOnScreenContentTypeA {
    id:_content
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_1312
    extraHeader: Item {}
    property var dashboardInfo: GroupWallet.dashboardInfo
    property string start_date: _calendarStart.dateString
    property var reminderKeys: dashboardInfo.health.reminderKeys
    property string option: dashboardInfo.health.editReminder
    readonly property var repeats: [
        {id: "",                  frequency: STR.STR_QML_1315 },
        // {id: "FIVE_MINUTES",      frequency: STR.STR_QML_1327 },
        {id: "MONTHLY",           frequency: STR.STR_QML_1316 },
        {id: "THREE_MONTHLY",     frequency: STR.STR_QML_1317 },
        {id: "SIX_MONTHLY",       frequency: STR.STR_QML_1318 },
        {id: "YEARLY",            frequency: STR.STR_QML_1319 },
    ]
    onCloseClicked: {
        dashboardInfo.health.resetScreen()
        closeTo(NUNCHUCKTYPE.CURRENT_TAB)
    }
    signal healthCheckBack(var obj)
    signal healthCheckSave(var obj)
    content: Item {
        Column {
            id: _col
            spacing: 12
            Item {
                width: parent.width
                height: 24
                QLato {
                    font.weight: Font.Bold
                    font.pixelSize: 16
                    text: {
                        var keyCount = 0;
                        for(var i = 0; i < reminderKeys.length; i++){
                            if(reminderKeys[i]){
                                keyCount ++
                            }
                        }
                        return keyCount === 1 ? STR.STR_QML_1320 : STR.STR_QML_1314
                    }
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
            Column {
                Repeater {
                    model: repeats
                    QRadioButtonTypeA {
                        width: 538
                        height: 48
                        label: modelData.frequency
                        selected: option === modelData.id
                        onButtonClicked: {
                            option = modelData.id;
                            if (option === "") {
                                _calendarStart.dateString = ""
                            }
                            else {
                                _calendarStart.dateString = dashboardInfo.health.dateToday()
                            }
                        }
                    }
                }
            }
            QLine {
                width: 538
            }
            Item {
                width: 538
                height: 72
                QTextInputBoxTypeB {
                    id: _start_date
                    label: STR.STR_QML_1104
                    boxWidth: 538
                    boxHeight: 48
                    textInputted: _calendarStart.dateString
                    input.placeholderText: dashboardInfo.health.dateToday()
                    enabled: option != ""
                    onTextInputtedChanged: {
                        if(!_start_date.isValid){
                            _start_date.isValid = true
                            _start_date.errorText = ""
                        }
                        _start_date.showError = false;
                    }
                }
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    enabled: option != ""
                    cursorShape: Qt.PointingHandCursor
                    propagateComposedEvents: true
                    onClicked: {
                        _calendarStart.open()
                    }
                }
            }
        }
    }
    onPrevClicked: {
        var check = {
            type: "health-check-add-reminders"
        }
        healthCheckBack(check)
    }
    bottomRight: QTextButton {
        width: 66
        height: 48
        label.text: STR.STR_QML_835
        label.font.pixelSize: 16
        type: eTypeE
        onButtonClicked: {
            if (option === "") {
                var noReminder = {
                    type: "health-check-add-reminders-no-reminder"
                }
                healthCheckSave(noReminder)
            }
            else {
                var check = {
                    type: "health-check-add-reminders-frequency-save",
                    frequency: option,
                    start_date: _calendarStart.dateString
                }
                healthCheckSave(check)
            }
        }
    }
    QPopupCalendar {
        id: _calendarStart
        minimumDate: new Date()
    }
}
