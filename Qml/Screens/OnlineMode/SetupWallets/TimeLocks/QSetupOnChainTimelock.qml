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
import QtQuick
import NUNCHUCKTYPE 1.0
import "../../../../Components/origins"
import "../../../../Components/customizes"
import "../../../../Components/customizes/Texts"
import "../../../../Components/customizes/Buttons"
import "../../../../Components/customizes/Popups"

QOnScreenContentTypeA {
    id: _setup
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    property string titleText: vm.valueDate.length > 0 ? QSTR.STR_QML_2076 : QSTR.STR_QML_1992
    property var descriptions: [QSTR.STR_QML_1993, QSTR.STR_QML_1994, QSTR.STR_QML_1995]
    property var references: []
    property string infoText: QSTR.STR_QML_1996
    property var validateSelection: null
    property bool infoBelowInputs: false
    label.text: titleText
    onCloseClicked: vm.close()

    content: Item {
        Column {
            width: parent.width
            spacing: 24
            Repeater {
                model: _setup.descriptions
                QLato {
                    width: 728
                    text: modelData
                    lineHeightMode: Text.FixedHeight
                    lineHeight: 28
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
            }
            Rectangle {
                width: 728
                height: _refColumn.implicitHeight + 24
                radius: 12
                color: "#F5F5F5"
                visible: _setup.references.length > 0
                Column {
                    id: _refColumn
                    anchors {
                        left: parent.left
                        right: parent.right
                        verticalCenter: parent.verticalCenter
                        leftMargin: 12
                        rightMargin: 12
                    }
                    spacing: 8
                    Repeater {
                        model: _setup.references
                        Column {
                            spacing: 2
                            QLato {
                                text: modelData.label
                                font.pixelSize: 12
                                color: "#757575"
                                horizontalAlignment: Text.AlignLeft
                            }
                            QLato {
                                text: modelData.value
                                font.weight: Font.Bold
                                horizontalAlignment: Text.AlignLeft
                            }
                        }
                    }
                }
            }
            Column {
                width: parent.width
                spacing: 12                
                Row {
                    spacing: 12
                    Item {
                        width: vm.isShowBlockHeight ? 150 : 173
                        height: _input_date.height
                        QTextInputBoxTypeB {
                            id: _input_date
                            label: QSTR.STR_QML_1463
                            labelComponent.textFormat: Text.RichText
                            labelComponent.font.pixelSize: 12
                            boxWidth: parent.width
                            boxHeight: 48
                            textInputted: _calendarInput.dateString
                            input.placeholderText: vm.valueDate
                            onTextInputtedChanged: {
                                vm.valueDate = _input_date.textInputted
                            }
                        }
                        QIcon {
                            iconSize: 24
                            source: "qrc:/Images/Images/calendar-light.svg"
                            anchors {
                                bottom: parent.bottom
                                bottomMargin: 14
                                right: parent.right
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
                    }

                    Item {
                        width: vm.isShowBlockHeight ? 150 : 173
                        height: _input_time.height
                        QTextInputBoxTypeB {
                            id: _input_time
                            label: QSTR.STR_QML_1989
                            labelComponent.textFormat: Text.RichText
                            labelComponent.font.pixelSize: 12
                            boxWidth: parent.width
                            boxHeight: 48
                            textInputted: _analogClockInput.timeString
                            input.placeholderText: vm.valueTime
                            onTextInputtedChanged: {
                                if(!_input_time.isValid){
                                    _input_time.isValid = true
                                    _input_time.errorText = ""
                                }
                                _input_time.showError = false;
                                vm.valueTime = _input_time.textInputted
                            }
                        }
                        QIcon {
                            iconSize: 24
                            source: "qrc:/Images/Images/clock-dark.svg"
                            anchors {
                                bottom: parent.bottom
                                bottomMargin: 14
                                right: parent.right
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

                    Item {
                        width: vm.isShowBlockHeight ? 240 : 358
                        height: _input_timezone.height
                        QTextInputBoxTypeB {
                            id: _input_timezone
                            label: QSTR.STR_QML_1990
                            labelComponent.textFormat: Text.RichText
                            labelComponent.font.pixelSize: 12
                            boxWidth: parent.width
                            boxHeight: 48
                            textInputted: vm.isShowBlockHeight ? textSlice(_timezoneInput.selectedTimezone) : _timezoneInput.selectedTimezone
                            onTextInputtedChanged: {
                                if(!_input_date.isValid){
                                    _input_date.isValid = true
                                    _input_date.errorText = ""
                                }
                                _input_date.showError = false;
                                vm.valueTimezone = _input_timezone.textInputted
                            }
                        }
                        QIcon {
                            iconSize: 24
                            source: "qrc:/Images/Images/expand-more-dark.svg"
                            anchors {
                                bottom: parent.bottom
                                bottomMargin: 14
                                right: parent.right
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
                        }

                        Connections {
                            target: vm
                            function onValueTimezoneChanged() {
                                if (_timezoneInput.selectedTimezone !== vm.valueTimezone) {
                                    _timezoneInput.timelockVM.timezones.setSelectedTimezone(vm.valueTimezone)
                                }
                            }
                            function onIsShowBlockHeightChanged() {
                                if(vm.isShowBlockHeight && !vm.isInit){
                                    blockBaseInfo.blockBasedTimelock();
                                }
                            }
                            function onInvalidDateAlert(message) {
                                _invalidDateAlert.contentText = (message !== undefined && message !== "" && message !== "Invalid timelock data") ? message : QSTR.STR_QML_2075
                                _invalidDateAlert.open()
                            }
                        }
                    }

                    Item {
                        width: 150
                        height: _blockHeight.height
                        visible: vm.isShowBlockHeight
                        QTextInputBoxTypeB {
                            id: _blockHeight
                            label: QSTR.STR_QML_188
                            labelComponent.textFormat: Text.RichText
                            labelComponent.font.pixelSize: 12
                            boxWidth: parent.width
                            boxHeight: 48
                            textInputted: utils.formatAmount(qsTr("%1").arg(vm.blockHeight))
                            enabled: false
                        }                        
                    }
                }

                QWarningBgMulti {
                    width: 728
                    icon: "qrc:/Images/Images/warning-dark.svg"
                    txt.text: QSTR.STR_QML_2069.arg(utils.formatAmount(qsTr("%1").arg(vm.blockHeight)))
                    color: "#FDEBD2"
                    visible: vm.isShowBlockHeight
                }
                QWarningBgMulti {
                    width: 728
                    icon: "qrc:/Images/Images/info-60px.svg"
                    txt.text: _setup.infoText
                    visible: _setup.infoBelowInputs
                }
            }
        }

        QWarningBgMulti {
            width: 728
            icon: "qrc:/Images/Images/info-60px.svg"
            txt.text: _setup.infoText
            anchors.bottom: parent.bottom
            visible: !_setup.infoBelowInputs
        }
    }
    bottomRight: QTextButton {
        width: label.paintedWidth + 16*2
        height: 48
        label.text: QSTR.STR_QML_835
        label.font.pixelSize: 16
        type: eTypeE
        onButtonClicked: {
            nextClicked()
        }
    }
    onNextClicked: {
        if (typeof _setup.validateSelection === "function") {
            var error = _setup.validateSelection(vm.selectedTimestamp())
            if (error !== "") {
                _invalidDateAlert.contentText = error
                _invalidDateAlert.open()
                return
            }
        }
        if (vm.isShowBlockHeight && (vm.xYearsLock > ServiceSetting.servicesTag.setupConfig.max_timelock_years)) {
            blockBaseInfo.confirmTimelockDuration();
            return;
        }
        vm.setupTimeLock()
    }
    onPrevClicked: {
        vm.close()
    }
    function textSlice(value) {
        return value.length > 25 ? value.slice(0, 22) + "..." : value
    }
    QmlUtils {
        id: utils
    }
    QPopupAnalogClock {
        id: _analogClockInput
        timeString: vm.valueTime
    }
    QPopupCalendar {
        id: _calendarInput        
        minimumDate: null
        selectedDate: {
            var parts = vm.valueDate.split("/")
            return new Date(parts[2], parts[0] - 1, parts[1])
        }
        dateString: vm.valueDate
    }

    QPopupInfoTwoButtons {
        id: blockBaseInfo
        title: QSTR.STR_QML_2070
        contentText: QSTR.STR_QML_2071
        labels: [QSTR.STR_QML_1114, QSTR.STR_QML_427]
        isVertical: false
        funcs: [
            function() {
                _calendarInput.open()
            },
            function() {
                blockBaseInfo.close()
            }
        ]
        function blockBasedTimelock() {
            blockBaseInfo.title = QSTR.STR_QML_2070
            blockBaseInfo.contentText = QSTR.STR_QML_2071
            blockBaseInfo.labels = [QSTR.STR_QML_1114, QSTR.STR_QML_427]
            blockBaseInfo.isVertical = false
            blockBaseInfo.funcs = [
                function() {
                    _calendarInput.open()
                },
                function() {
                    blockBaseInfo.close()
                }
            ]
            blockBaseInfo.open();
        }
        function confirmTimelockDuration() {
            blockBaseInfo.title = QSTR.STR_QML_2072
            blockBaseInfo.contentText = QSTR.STR_QML_2073.arg(ServiceSetting.servicesTag.setupConfig.max_timelock_years)  //vm.xYearsLock)
            blockBaseInfo.labels = [QSTR.STR_QML_1114, QSTR.STR_QML_427]
            blockBaseInfo.isVertical = false
            blockBaseInfo.funcs = [
                function() {
                    _calendarInput.open()
                },
                function() {
                    vm.setupTimeLock()
                }
            ]
            blockBaseInfo.open();
        }
    }

    QPopupInfo{
        id: _invalidDateAlert
        title: QSTR.STR_QML_2074
        contentText: QSTR.STR_QML_2075
        btnLabel: QSTR.STR_QML_341
        action: function() {
                _invalidDateAlert.close()
            }
    }
}
