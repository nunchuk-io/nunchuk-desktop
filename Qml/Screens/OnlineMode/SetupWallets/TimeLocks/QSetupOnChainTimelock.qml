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
import QtQuick 2.15
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
    label.text: vm.valueDate.length > 0 ? STR.STR_QML_2076 : STR.STR_QML_1992
    onCloseClicked: vm.close()
    
    content: Item {
        Column {
            width: parent.width
            spacing: 24
            QLato {
                width: 728
                text: STR.STR_QML_1993
                lineHeightMode: Text.FixedHeight
                lineHeight: 28
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }
            QLato {
                width: 728
                text: STR.STR_QML_1994
                lineHeightMode: Text.FixedHeight
                lineHeight: 28
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }
            QLato {
                width: 728
                text: STR.STR_QML_1995
                lineHeightMode: Text.FixedHeight
                lineHeight: 28
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
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
                            label: STR.STR_QML_1463
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
                            label: STR.STR_QML_1989
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
                            label: STR.STR_QML_1990
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
                            onValueTimezoneChanged: {
                                _timezoneInput.timelockVM.timezones.setSelectedTimezone(vm.valueTimezone)
                            }
                            onIsShowBlockHeightChanged: {
                                if(vm.isShowBlockHeight && !vm.isInit){
                                    blockBaseInfo.blockBasedTimelock();
                                }
                            }
                            onInvalidDateAlert: {
                                console.log("hadv3 invalid timelock data signal received");
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
                            label: STR.STR_QML_188
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
                    height: 80
                    icon: "qrc:/Images/Images/warning-dark.svg"
                    txt.text: STR.STR_QML_2069.arg(utils.formatAmount(qsTr("%1").arg(vm.blockHeight)))
                    color: "#FDEBD2"
                    visible: vm.isShowBlockHeight
                }
            }
        }

        QWarningBgMulti {
            width: 728
            height: 60
            icon: "qrc:/Images/Images/info-60px.svg"
            txt.text: STR.STR_QML_1996
            anchors.bottom: parent.bottom
        }
    }
    bottomRight: QTextButton {
        width: label.paintedWidth + 16*2
        height: 48
        label.text: STR.STR_QML_835
        label.font.pixelSize: 16
        type: eTypeE
        onButtonClicked: {
            nextClicked()
        }
    }
    onNextClicked: {
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
        title: STR.STR_QML_2070
        contentText: STR.STR_QML_2071
        labels: [STR.STR_QML_1114, STR.STR_QML_427]
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
            blockBaseInfo.title = STR.STR_QML_2070
            blockBaseInfo.contentText = STR.STR_QML_2071
            blockBaseInfo.labels = [STR.STR_QML_1114, STR.STR_QML_427]
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
            blockBaseInfo.title = STR.STR_QML_2072
            blockBaseInfo.contentText = STR.STR_QML_2073.arg(ServiceSetting.servicesTag.setupConfig.max_timelock_years)  //vm.xYearsLock)
            blockBaseInfo.labels = [STR.STR_QML_1114, STR.STR_QML_427]
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
        title: STR.STR_QML_2074
        contentText: STR.STR_QML_2075
        btnLabel: STR.STR_QML_341
        action: function() {
                _invalidDateAlert.close()
            }
    }
}
