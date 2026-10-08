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
    height: _address.height + 24
    property bool isSelected: false
    property var address_list: []
    property var addresses: ""
    Connections {
        target: _content
        onSignalFinalAddress: {
            if (isSelected) {
                address_list = addresses.split('\n')
                var _input = {
                    type: "import-address-list",
                    import_address_list: address_list
                }
                QMLHandle.sendEvent(EVT.EVT_RECURRING_PAYMENTS_ENTER, _input)
            }
        }
    }
    QTextAreaBoxTypeA  {
        id: _address
        anchors {
            top: _item.top
            topMargin: 24
        }
        boxWidth: _item.width
        boxHeight: Math.max(384, input.paintedHeight)
        label: STR.STR_QML_1135
        onTypingFinished: {
            addresses = _address.textInputted
            hasAddress = addresses!== ""
        }
        isValid: true
        input.placeholderText: STR.STR_QML_1136
        input.verticalAlignment: Text.AlignTop
        input.wrapMode: Text.WrapAtWordBoundaryOrAnywhere
    }
}
