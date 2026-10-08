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
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Chats"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

Column {
    id: _row
    property real limit: 0.0
    property var currency: ""
    property var interval: ""
    signal limitInputChanged(var value)
    signal currencyInputChanged(string value)
    signal intervalInputChanged(string value)
    width: 343
    spacing: 8
    function getLimit() {
        return co_signing.textInputted
    }
    function getCurrency() {
        return _currency.getDisplay().id
    }
    function getInterval() {
        return _interal.getDisplay().id
    }
    Row {
        spacing: 8
        QTextInputBoxTypeB {
            id: co_signing
            label: ""
            boxWidth: 240
            boxHeight: 48
            titleFontSize: 16
            isValid: true
            textInputted: limit
            onTextInputtedChanged: {
                if(!co_signing.isValid){
                    co_signing.isValid = true
                    co_signing.errorText = ""
                }
                co_signing.showError = false;
                _row.limitInputChanged(co_signing.textInputted)
            }
        }
        QComboBox {
            id: _currency
            anchors.bottom: co_signing.bottom
            width: 95
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
            onSignalActivated: {
                _row.currencyInputChanged(_currency.getDisplay().id)
            }
        }
    }
    
    QComboBox {
        id: _interal
        anchors.bottom: co_signing.bottom
        width: _row.width
        height: 48
        current_id: {
            for (var i = 0; i < _interal.model.length; i++) {
                if (_interal.model[i].id === _row.interval) {
                    return _interal.model[i]
                }
            }
            return _interal.model[0]
        }
        defaultValue: {
            return { id: qsTr("%1").arg(_interal.model[0].interval),  displayName: qsTr("%1").arg(_interal.model[0].displayName) }
        }
        displayText: getDisplay().displayName
        model:  [
            { id: qsTr("DAILY"),   displayName: qsTr("Daily")  },
            { id: qsTr("WEEKLY"),  displayName: qsTr("Weekly") },
            { id: qsTr("MONTHLY"), displayName: qsTr("Monthly")},
            { id: qsTr("YEARLY"),  displayName: qsTr("Yearly") }
        ]
        textRole: "displayName"
        onSignalActivated: {
            _row.intervalInputChanged(_interal.getDisplay().id)
        }
    }
}
