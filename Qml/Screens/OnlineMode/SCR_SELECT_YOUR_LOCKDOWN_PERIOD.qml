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
import Qt.labs.platform 1.1
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../../localization/STR_QML.js" as STR

QScreen {
    id: _period
    property int questionSelected: 0
    property var qSelected : ServiceSetting.servicesTag.periods[questionSelected]
    QOnScreenContentTypeB {
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        label.text: STR.STR_QML_711
        onCloseClicked: {
            QMLHandle.sendEvent(EVT.EVT_CLOSE_TO_SERVICE_SETTINGS_REQUEST, EVT.STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD)
        }

        Column {
            anchors{
                left: parent.left
                leftMargin: 36
                top: parent.top
                topMargin: 84
            }
            spacing: 24
            QText {
                width: 646
                height: 28
                color: "#031F2B"
                font.pixelSize: 16
                font.weight: Font.Normal
                text: STR.STR_QML_712
            }
            QListView {
                id: questions
                width: 539
                height: childrenRect.height
                spacing: 8
                clip: true
                model: ServiceSetting.servicesTag.periods
                interactive: false
                delegate: Item {
                    id: questionsdlg
                    width: 539
                    height: 60
                    Rectangle {
                        id: rect
                        anchors.fill: parent
                        color: "transparent"
                        border.color: "#DEDEDE"
                        border.width: 2
                        radius: 12
                    }
                    DropShadow {
                        anchors.fill: rect
                        verticalOffset: 2
                        cached: true
                        radius: 8
                        samples: 16
                        color: Qt.rgba(0, 0, 0, 0.15)
                        source: rect
                    }
                    QRadioButtonTypeA {
                        id: btn
                        width: 539
                        height: 60
                        label: modelData.display_name
                        layoutDirection: Qt.RightToLeft
                        fontFamily: "Lato"
                        fontPixelSize: 16
                        anchors {
                            left: parent.left
                            leftMargin: 20
                            verticalCenter: parent.verticalCenter
                        }
                        fontWeight: btn.selected ? Font.ExtraBold : Font.DemiBold
                        selected: questionSelected === index
                        onButtonClicked: {
                            questionSelected = index
                        }
                    }
                }
            }
        }

        onNextClicked: {
            QMLHandle.sendEvent(EVT.EVT_INPUT_DAYS_VALUE_REQUEST, qSelected)
        }

        onPrevClicked: {
            QMLHandle.sendEvent(EVT.EVT_CLOSE_TO_SERVICE_SETTINGS_REQUEST, EVT.STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD)
        }
    }
}
