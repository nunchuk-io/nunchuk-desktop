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
import NUNCHUCKTYPE 1.0
import Features.Inheritance.OffChain.ViewModels 1.0
import "../../../../Components/origins"
import "../../../../Components/customizes"
import "../../../../Components/customizes/Texts"
import "../../../../Components/customizes/Buttons"
import "../../../../Components/customizes/Popups"
import "../../../../../localization/STR_QML.js" as STR

QOnScreenContentTypeA {
    id: _setup
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_2055
    onCloseClicked: vm.close()
    onPrevClicked: vm.close()
    onNextClicked: {
        vm.timeLockContinueClicked()
        vm.close()
    }

    content: Item {
        Column {
            width: parent.width
            spacing: 24
            QLato {
                width: 732
                text: STR.STR_QML_2056
                lineHeightMode: Text.FixedHeight
                lineHeight: 20
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }
            QLato {
                width: 732
                text: STR.STR_QML_2057
                lineHeightMode: Text.FixedHeight
                lineHeight: 20
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }
            QLato {
                width: 732
                text: STR.STR_QML_2058
                lineHeightMode: Text.FixedHeight
                lineHeight: 20
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }
            Row {
                spacing: 12
                Item {
                    width: 360
                    height: _input_timezone.height
                    QTextInputBoxTypeB {
                        id: _input_timezone
                        label: STR.STR_QML_1990
                        labelComponent.textFormat: Text.RichText
                        labelComponent.font.pixelSize: 16
                        boxWidth: parent.width
                        boxHeight: 48
                        textInputted: _timezoneInput.selectedTimezone
                        onTextInputtedChanged: {
                            vm.valueTimezone = _input_timezone.textInputted
                        }
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
                    }

                    Connections {
                        target: vm
                        onValueTimezoneChanged: {
                            _timezoneInput.timelockVM.timezones.setSelectedTimezone(vm.valueTimezone)
                        }
                    }
                }
                Item {
                    width: 360
                    height: _input_date.height
                    QTextInputBoxTypeB {
                        id: _input_date
                        label: STR.STR_QML_1463
                        labelComponent.textFormat: Text.RichText
                        labelComponent.font.pixelSize: 16
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
                    
                }
            }
        }

        QWarningBgMulti {
            width: 732
            height: 80
            icon: "qrc:/Images/Images/info-60px.svg"
            txt.text: STR.STR_QML_2059
            anchors.bottom: parent.bottom
        }
    }
    function textSlice(value) {
        return value.length > 25 ? value.slice(0, 22) + "..." : value
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
    SetupAnOffChainTimelockViewModel {
        id: vm
        Component.onCompleted: {
            FlowManager.currentFlow.bind(vm)
            vm.attachContext(vmContext)
        }
    }
}
