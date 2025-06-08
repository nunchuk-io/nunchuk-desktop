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
import "../../../../../localization/STR_QML.js" as STR

Row {
    id: _row
    property real limit: 0.0
    property var currency: ""
    property var interval: ""
    property int titleFontSize: 16
    width: 627
    height: 60
    spacing: 14
    function spending_limit() {
        var ret = {
            "limit" : getLimit(),
            "currency" : getCurrency(),
            "interval" : getInterval(),
        }
        return ret
    }
    QTextInputBoxTypeB {
        id: co_signing
        label: STR.STR_QML_801
        boxWidth: _row.width - 2 * _row.spacing - _currency.width - _interal.width
        boxHeight: 48
        titleFontSize: _row.titleFontSize
        isValid: true
        textInputted: limit
        onTextInputtedChanged: {
            if(!co_signing.isValid){
                co_signing.isValid = true
                co_signing.errorText = ""
            }
            co_signing.showError = false;
        }
        property bool dataChanged: limit !== co_signing.textInputted
    }
    function getLimit() {
        return co_signing.textInputted
    }
    QComboBox {
        id: _currency
        anchors.bottom: co_signing.bottom
        width: 99
        height: 48
        current_id: {
            return { id: qsTr("%1").arg(_row.currency),  displayName: qsTr("%1").arg(_row.currency) }
        }
        defaultValue: {
            return { id: qsTr("%1").arg(AppSetting.currency),  displayName: qsTr("%1").arg(AppSetting.currency) }
        }
        displayText: getDisplay().displayName
        model: [
            { id: qsTr("%1").arg(AppSetting.currency),  displayName: qsTr("%1").arg(AppSetting.currency) },
            { id: qsTr("BTC"),                          displayName: qsTr("BTC") },
            { id: qsTr("sat"),                          displayName: qsTr("sat") },
        ]
        textRole: "displayName"
        property bool dataChanged: _currency.getIndex(_row.currency) !== _currency.currentIndex
    }
    function getCurrency() {
        return _currency.getDisplay().id
    }
    QComboBox {
        id: _interal
        anchors.bottom: co_signing.bottom
        width: 128
        height: 48
        current_id: {
            return { id: qsTr("%1").arg(_row.interval),  displayName: qsTr("%1").arg(_row.interval) }
        }
        defaultValue: {
            return { id: qsTr("%1").arg(_interal.model[0].interval),  displayName: qsTr("%1").arg(_interal.model[0].displayName) }
        }
        displayText: getDisplay().displayName
        model:  [
            { id: qsTr("DAILY"),   displayName: qsTr("DAILY")  },
            { id: qsTr("WEEKLY"),  displayName: qsTr("WEEKLY") },
            { id: qsTr("MONTHLY"), displayName: qsTr("MONTHLY")},
            { id: qsTr("YEARLY"),  displayName: qsTr("YEARLY") }
        ]
        textRole: "displayName"
        property bool dataChanged: _interal.getIndex(_row.interval) !== _interal.currentIndex
    }
    function getInterval() {
        return _interal.getDisplay().id
    }
}
