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
import QtQuick.Controls 1.4
import QtQuick.Controls 2.3
import QtGraphicalEffects 1.12
import Qt.labs.platform 1.1
import QtQuick.Controls.Styles 1.4
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Chats"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

Item {
    id: _item
    height: _col.childrenRect.height + 24
    property bool isSelected: false
    property var address_list: ["",""]
    function addAddress() {
        var address = ""
        address_list.push(address)
        model_list.model = address_list
    }
    function removeAddress(index) {
        var list = []
        for (var i = 0; i < address_list.length; i++)
        {
            if (i === index) continue;
            list.push(address_list[i])
        }
        address_list = list
        model_list.model = address_list
    }
    function setAddress(index, address) {
        address_list[index] = address
        model_list.model = address_list
        hasAddress = false
        for (var i = 0; i < address_list.length; i++)
        {
            if (address_list[i] !== "") {
                hasAddress = true
            }
        }
    }
    Connections {
        target: _content
        onSignalAddAddress: {
            _item.addAddress()
        }
        onSignalSetAddress: {
            _item.setAddress(index, address)
        }
        onSignalFinalAddress: {
            if (isSelected) {
                var _input = {
                    type: "import-address-list",
                    import_address_list: address_list
                }
                QMLHandle.sendEvent(EVT.EVT_RECURRING_PAYMENTS_ENTER, _input)
            }
        }
    }
    Column {
        id: _col
        anchors {
            top: _item.top
            topMargin: 24
        }
        spacing: 24
        Repeater {
            id: model_list
            model: address_list
            QEnterAddressDelegate {
                width: _item.width
                oneAddress: modelData
                onSignalRemoveAddress: _item.removeAddress(index)
                onSignalInputAddress: _item.setAddress(index, address)
            }
        }
    }
}
