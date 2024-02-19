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
import QtQuick 2.12
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0
import Qt.labs.platform 1.1
import HMIEVENTS 1.0
import NUNCHUCKTYPE 1.0
import QRCodeItem 1.0
import DataPool 1.0
import DRACO_CODE 1.0
import EWARNING 1.0
import "./../../../origins"
import "./../../../customizes"
import "./../../../customizes/Chats"
import "./../../../customizes/Texts"
import "./../../../customizes/Buttons"
import "../../../RightPannel/Service/Common"
import "../../../../../localization/STR_QML.js" as STR

Column {
    property var serverKeyInfo
    property var policies: serverKeyInfo.policies
    property bool automatically: policies.auto_broadcast_transaction
    property bool enableCoSigning: policies.signing_delay_seconds > 0
    property var time: serverKeyInfo.secondsToTime(policies.signing_delay_seconds)
    property var hours: time.hours
    property var minutes: time.minutes

    spacing: 16
    Item {
        width: 627
        height: 52
        Column {
            anchors.fill: parent
            spacing: 4
            QLato {
                width: parent.width
                height: 20
                font.weight: Font.Bold
                text: STR.STR_QML_792
            }
            QLato {
                width: parent.width
                height: 28
                text: STR.STR_QML_793
            }
        }
    }
    function isChanged() {
        return _auto.getValue() || enable() || _hours.dataChanged || _minutes.dataChanged
    }

    QSwitchTypeLabel {
        id: _auto
        label: STR.STR_QML_794
        value: automatically
    }

    QSwitchTypeTwoLabel {
        id: _enable
        label: STR.STR_QML_795
        description: STR.STR_QML_796
        value: enableCoSigning
    }
    function enable() {
        return _enable.getValue()
    }

    function getHours() {
        return _hours.textInputted
    }

    function getMinutes() {
        return _minutes.textInputted
    }

    Item {
        width: 627
        height: 72
        visible: _enable.getValue()
        Row {
            anchors.fill: parent
            spacing: 15
            QTextInputBoxTypeB {
                id: _hours
                label: STR.STR_QML_797
                boxWidth: 306
                boxHeight: 48
                isValid: true
                validator: IntValidator {bottom: 0; top: 167}
                textInputted: hours ? hours : ""
                onTextInputtedChanged: {
                    if(!_hours.isValid){
                        _hours.isValid = true
                        _hours.errorText = ""
                    }
                    _hours.showError = false;
                    if (parseInt(textInputted) > 167) {
                        AppModel.showToast(-1, STR.STR_QML_808, EWARNING.ERROR_MSG);
                    }
                }
                property bool dataChanged: hours !== _hours.textInputted
            }
            QTextInputBoxTypeB {
                id: _minutes
                label: STR.STR_QML_798
                boxWidth: 306
                boxHeight: 48
                isValid: true
                validator: RegExpValidator { regExp: /[0-5]?[0-9]/; }
                textInputted: minutes ? minutes : ""
                onTextInputtedChanged: {
                    if(!_minutes.isValid){
                        _minutes.isValid = true
                        _minutes.errorText = ""
                    }
                    _minutes.showError = false;
                }
                property bool dataChanged: minutes !== _minutes.textInputted
            }
        }
    }
    function policies_broadcast() {
        var time = {
            "hours" : parseInt(getHours()),
            "minutes" : parseInt(getMinutes()),
        }
        var ret = {
            "auto_broadcast_transaction": _auto.getValue(),
            "signing_delay_seconds": _enable.getValue() ? serverKeyInfo.timeToSeconds(time) : 0,
        }
        return ret
    }
}
