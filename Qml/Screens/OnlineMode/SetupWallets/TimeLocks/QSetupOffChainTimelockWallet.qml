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
    onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)

    property var    inheritancePlanInfo: ServiceSetting.walletInfo.inheritancePlanInfo
    property var    planInfo: inheritancePlanInfo.planInfo
    property string walletName: ServiceSetting.walletInfo.walletName
    property int    walletType: ServiceSetting.walletInfo.walletType

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
                        textInputted: ServiceSetting.walletInfo.timezones.selectedTimezone
                        onTextInputtedChanged: {
                            if(!_input_date.isValid){
                                _input_date.isValid = true
                                _input_date.errorText = ""
                            }
                            _input_date.showError = false;
                            var _time = {
                                "valueTimezone"  : _input_timezone.textInputted,
                                "valueDate"      : planInfo.activation_date,
                            }
                            // dashInfo.walletSetupTimeLock(_time)
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
                        selectedWalletInfo: ServiceSetting.walletInfo
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
                        input.placeholderText: planInfo.activation_date
                        onTextInputtedChanged: {
                            if(!_input_date.isValid){
                                _input_date.isValid = true
                                _input_date.errorText = ""
                            }
                            _input_date.showError = false;
                            var _time = {
                                "valueTimezone"  : planInfo.activation_timezone,
                                "valueDate"      : _input_date.textInputted
                            }
                            // dashInfo.walletSetupTimeLock(_time)
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
                            var parts = planInfo.activation_date.split("/")
                            var dated = new Date(parts[2], parts[0] - 1, parts[1])
                            _calendarInput.selectedDate = dated
                            _calendarInput.open()
                        }
                    }
                    QPopupCalendar {
                        id: _calendarInput
                        minimumDate: new Date()
                        selectedDate: {
                            var parts = planInfo.activation_date.split("/")
                            return new Date(parts[2], parts[0] - 1, parts[1])
                        }
                        dateString: planInfo.activation_date
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
}
