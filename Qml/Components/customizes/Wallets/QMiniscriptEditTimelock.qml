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
import "../../../Components/customizes/Popups"
import "../../../../localization/STR_QML.js" as STR

Column {
    width: 350
    spacing: 8
    QLato {
        width: parent.width
        text: STR.STR_QML_1817
        font.weight: Font.Bold
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
    }
    Column {
        width: parent.width
        spacing: 16
        Column {
            width: parent.width
            QLato {
                width: parent.width
                text: STR.STR_QML_1830
                font.pixelSize: 12
                font.weight: Font.Bold
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }
            QEditTimelockSelectRadio {
                width: parent.width
                height: 62
                headline: STR.STR_QML_1831
                description: STR.STR_QML_1832
                selected: newWalletInfo.timelockType === ScriptNodeHelper.TimelockType.LOCKTYPE_ABSOLUTE
                onButtonClicked: {
                    newWalletInfo.timelockType = ScriptNodeHelper.TimelockType.LOCKTYPE_ABSOLUTE
                }

            }
            QEditTimelockSelectRadio {
                width: parent.width
                height: 94
                headline: STR.STR_QML_1833
                description: STR.STR_QML_1834
                selected: newWalletInfo.timelockType === ScriptNodeHelper.TimelockType.LOCKTYPE_RELATIVE
                onButtonClicked: {
                    newWalletInfo.timelockType = ScriptNodeHelper.TimelockType.LOCKTYPE_RELATIVE
                }
            }
        }
        QLine {
            width: parent.width
            height: 1
            color: "#DEDEDE"
        }
        Column {
            width: parent.width
            QLato {
                width: parent.width
                text: STR.STR_QML_1835
                font.pixelSize: 12
                font.weight: Font.Bold
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }
            QEditTimelockSelectRadio {
                width: parent.width
                height: 62
                headline: STR.STR_QML_1836
                description: STR.STR_QML_1837
                selected: newWalletInfo.timeUnit === ScriptNodeHelper.TimelockBased.TIME_LOCK
                onButtonClicked: {
                    newWalletInfo.timeUnit = ScriptNodeHelper.TimelockBased.TIME_LOCK
                }
            }
            QEditTimelockSelectRadio {
                width: parent.width
                height: 62
                headline: STR.STR_QML_1838
                description: STR.STR_QML_1839
                selected: newWalletInfo.timeUnit === ScriptNodeHelper.TimelockBased.HEIGHT_LOCK
                onButtonClicked: {
                    newWalletInfo.timeUnit = ScriptNodeHelper.TimelockBased.HEIGHT_LOCK
                }
            }
        }
        QLine {
            width: parent.width
            height: 1
            color: "#DEDEDE"
        }
        Loader {
            id: timelockInputLoader
            width: parent.width
            sourceComponent: {
                if(newWalletInfo.timelockType === ScriptNodeHelper.TimelockType.LOCKTYPE_ABSOLUTE && newWalletInfo.timeUnit === ScriptNodeHelper.TimelockBased.TIME_LOCK) {
                    return unlockAfterAFixedDate
                } else if(newWalletInfo.timelockType === ScriptNodeHelper.TimelockType.LOCKTYPE_ABSOLUTE && newWalletInfo.timeUnit === ScriptNodeHelper.TimelockBased.HEIGHT_LOCK) {
                    return unlockAfterBlockNumber
                } else if(newWalletInfo.timelockType === ScriptNodeHelper.TimelockType.LOCKTYPE_RELATIVE && newWalletInfo.timeUnit === ScriptNodeHelper.TimelockBased.TIME_LOCK) {
                    return unlockAfterTimePeriodDays
                } else if(newWalletInfo.timelockType === ScriptNodeHelper.TimelockType.LOCKTYPE_RELATIVE && newWalletInfo.timeUnit === ScriptNodeHelper.TimelockBased.HEIGHT_LOCK) {
                    return unlockAfterTimePeriodBlocks
                }
            }
        }
    }
    Component {
        id: unlockAfterAFixedDate
        Column {
            width: parent.width
            spacing: 12
            QLato {
                width: parent.width
                text: STR.STR_QML_1840
                font.pixelSize: 12
                textFormat: Text.RichText
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }
            Item {
                width: 350
                height: _input_timezone.height
                QTextInputBoxTypeB {
                    id: _input_timezone
                    label: "Time zone"
                    labelComponent.textFormat: Text.RichText
                    labelComponent.font.pixelSize: 12
                    boxWidth: parent.width
                    boxHeight: 48
                    textInputted: _timezoneInput.selectedTimezone
                }
                QIcon {
                    iconSize: 24
                    source: "qrc:/Images/Images/expand-more-dark.svg"
                    anchors {
                        bottom: _input_timezone.bottom
                        bottomMargin: 14
                        right: _input_timezone.right
                        rightMargin: 12
                    }
                }
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    propagateComposedEvents: true
                    onClicked: {
                        _timezoneInput.open()
                    }
                }
                QPopupTimezone {
                    id: _timezoneInput
                    selectedWalletInfo: AppModel.newWalletInfo
                }
            }

            Row {
                spacing: 12
                Item {
                    width: 169
                    height: _input_date.height
                    QTextInputBoxTypeB {
                        id: _input_date
                        label: STR.STR_QML_1463
                        labelComponent.textFormat: Text.RichText
                        labelComponent.font.pixelSize: 12
                        boxWidth: parent.width
                        boxHeight: 48
                        textInputted: _calendarInput.dateString
                        input.placeholderText: newWalletInfo.timeMini.absoluteTimestamp.valueDate
                        onTextInputtedChanged: {
                            if(!_input_date.isValid){
                                _input_date.isValid = true
                                _input_date.errorText = ""
                            }
                            _input_date.showError = false;
                            var absolute_time = {
                                "valueDate"      : _input_date.textInputted,
                                "valueTime"      : newWalletInfo.timeMini.absoluteTimestamp.valueTime
                            }
                            newWalletInfo.updateTimeMiniscript("absoluteTimestamp", absolute_time);
                        }
                    }
                    QIcon {
                        iconSize: 24
                        source: "qrc:/Images/Images/calendar-light.svg"
                        anchors {
                            bottom: _input_date.bottom
                            bottomMargin: 14
                            right: _input_date.right
                            rightMargin: 12
                        }
                    }
                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        propagateComposedEvents: true
                        onClicked: {
                            _calendarInput.open()
                        }
                    }
                    QPopupCalendar {
                        id: _calendarInput
                        minimumDate: new Date()
                        dateString: newWalletInfo.timeMini.absoluteTimestamp.valueDate
                    }
                }

                Item {
                    width: 169
                    height: _input_time.height
                    QTextInputBoxTypeB {
                        id: _input_time
                        label: STR.STR_QML_1989
                        labelComponent.textFormat: Text.RichText
                        labelComponent.font.pixelSize: 12
                        boxWidth: parent.width
                        boxHeight: 48
                        textInputted: _analogClockInput.timeString
                        input.placeholderText: newWalletInfo.timeMini.absoluteTimestamp.valueTime
                        onTextInputtedChanged: {
                            if(!_input_time.isValid){
                                _input_time.isValid = true
                                _input_time.errorText = ""
                            }
                            _input_time.showError = false;
                            var absolute_time = {
                                "valueDate"      : newWalletInfo.timeMini.absoluteTimestamp.valueDate,
                                "valueTime"      : _input_time.textInputted
                            }
                            newWalletInfo.updateTimeMiniscript("absoluteTimestamp", absolute_time);
                        }
                    }
                    QIcon {
                        iconSize: 24
                        source: "qrc:/Images/Images/clock-dark.svg"
                        anchors {
                            bottom: _input_time.bottom
                            bottomMargin: 14
                            right: _input_time.right
                            rightMargin: 12
                        }
                    }
                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        propagateComposedEvents: true
                        onClicked: {
                            _analogClockInput.open()
                        }
                    }
                }
            }
        }
    }
    Component {
        id: unlockAfterBlockNumber
        Item {
            width: parent.width
            height: 88
            Column {
                width: parent.width
                spacing: 4
                QTextInputBoxTypeB {
                    id: _inputblockAbs
                    label: STR.STR_QML_1841
                    labelComponent.textFormat: Text.RichText
                    labelComponent.font.pixelSize: 12
                    boxWidth: parent.width
                    boxHeight: 48
                    textInputted: newWalletInfo.timeMini.absoluteBlockheight
                    input.placeholderText: AppModel.blockHeight + 4320
                    isValid: (textInputted !== "") && (!Number.isNaN(parseInt(_inputblockAbs.textInputted))) && (parseInt(_inputblockAbs.textInputted) <= 499999999)
                    errorText: STR.STR_QML_1887
                    validator: RegExpValidator {
                        regExp: /^$|^[0-9]+$/
                    }
                    onTypingFinished: {
                        if(isValid && !Number.isNaN(parseInt(_inputblockAbs.textInputted)) && (parseInt(_inputblockAbs.textInputted) <= 499999999)){
                            newWalletInfo.updateTimeMiniscript("absoluteBlockheight", parseInt(_inputblockAbs.textInputted));
                        }
                    }
                    onIsValidChanged: {
                        if(!isValid){
                            AppModel.showToast(0, STR.STR_QML_1887, EWARNING.ERROR_MSG);
                        }
                    }
                }
                QLato {
                    width: parent.width
                    text: STR.STR_QML_1843.arg(AppModel.blockHeight)
                    font.pixelSize: 12
                    font.weight: Font.Normal
                    color: "#757575"
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }
    }
    Component {
        id: unlockAfterTimePeriodDays
        Item {
            width: parent.width
            height: 72
            Column {
                spacing: 1
                QLato {
                    width: parent.width
                    text: STR.STR_QML_1840
                    font.pixelSize: 12
                    font.weight: Font.Bold
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    textFormat: Text.RichText
                }
                Row {
                    spacing: 12
                    QTextInputBoxTypeB {
                        id: input_days
                        label: "Days"
                        titleFontSize: 12
                        boxWidth: 108
                        boxHeight: 48
                        textInputted: newWalletInfo.timeMini.relativeTimestamp.valueDay
                        input.placeholderText: "90"
                        isValid: textInputted !== ""
                        validator: RegExpValidator {
                            // Regex cho phép:
                            // - 1–9
                            // - 10–99
                            // - 100–299
                            // - 300–388
                            regExp: /^([1-9]|[1-9][0-9]|[1-2][0-9][0-9]|3[0-8][0-8]?|38[0-8])$/
                        }
                        onTypingFinished: {
                            var relative_time = {
                                "valueDay"      : input_days.textInputted,
                                "valueHour"     : input_hours.textInputted,
                                "valueMinute"   : input_minutes.textInputted
                            }
                            newWalletInfo.updateTimeMiniscript("relativeTimestamp", relative_time);
                        }
                    }
                    QTextInputBoxTypeB {
                        id: input_hours
                        label: "Hours"
                        titleFontSize: 12
                        boxWidth: 108
                        boxHeight: 48
                        textInputted: newWalletInfo.timeMini.relativeTimestamp.valueHour
                        input.placeholderText: "0"
                        isValid: textInputted !== ""
                        validator: RegExpValidator {
                            regExp: /^(0?\d|1\d|2[0-3])$/
                        }
                        onTypingFinished: {
                            var relative_time = {
                                "valueDay"      : input_days.textInputted,
                                "valueHour"     : input_hours.textInputted,
                                "valueMinute"   : input_minutes.textInputted
                            }
                            newWalletInfo.updateTimeMiniscript("relativeTimestamp", relative_time);
                        }
                    }
                    QTextInputBoxTypeB {
                        id: input_minutes
                        label: "Minutes"
                        titleFontSize: 12
                        boxWidth: 108
                        boxHeight: 48
                        textInputted: newWalletInfo.timeMini.relativeTimestamp.valueMinute
                        input.placeholderText: "0"
                        isValid: textInputted !== ""
                        validator: RegExpValidator {
                            regExp: /^(0?\d|[1-5]\d)$/
                        }
                        onTypingFinished: {
                            var relative_time = {
                                "valueDay"      : input_days.textInputted,
                                "valueHour"     : input_hours.textInputted,
                                "valueMinute"   : input_minutes.textInputted
                            }
                            newWalletInfo.updateTimeMiniscript("relativeTimestamp", relative_time);
                        }
                    }
                }
            }
        }
    }
    Component {
        id: unlockAfterTimePeriodBlocks
        Item {
            width: parent.width
            height: 72
            QTextInputBoxTypeB {
                id: _inputblocks
                label: STR.STR_QML_1841
                labelComponent.textFormat: Text.RichText
                labelComponent.font.pixelSize: 12
                boxWidth: parent.width
                boxHeight: 48
                textInputted: newWalletInfo.timeMini.relativeBlockheight
                input.placeholderText: "4320"
                isValid: (textInputted !== "") && (!Number.isNaN(parseInt(_inputblocks.textInputted))) && (parseInt(_inputblocks.textInputted) < 65535)
                errorText: STR.STR_QML_1888
                validator: RegExpValidator {
                    regExp: /^$|^[0-9]+$/
                }
                onTypingFinished: {
                    if(isValid && !Number.isNaN(parseInt(_inputblocks.textInputted)) && (parseInt(_inputblocks.textInputted) < 65535)){
                        newWalletInfo.updateTimeMiniscript("relativeBlockheight", parseInt(_inputblocks.textInputted));
                    }
                }
                onIsValidChanged: {
                    if(!isValid){
                        AppModel.showToast(0, STR.STR_QML_1888, EWARNING.ERROR_MSG);
                    }
                }
            }
        }
    }
}
