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
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Buttons"
import "../../Components/customizes/Lists"
import "../../Components/customizes/Texts"
import Features.Common.ViewModels 1.0

Item {
    id: _settingTimezone
    property alias timelockVM: vm
    signal timezoneSelected()
    Column {
        width: parent.width
        spacing: 4
        QLato {
            font.pixelSize: 12
            font.weight: Font.Bold
            text: STR.STR_QML_1990
        }
        Item {
            width: _settingTimezone.width
            height: 300
            clip: true
            QComboBoxIndex {
                currentIndex: vm.timezones.currentIndex
                width: _settingTimezone.width
                height: 48
                heigthContent: 300
                model: vm.timezones
                textRole: "timezoneName"
                onActivated: {
                    vm.timezones.currentIndex = index
                    _settingTimezone.timezoneSelected()
                }
                function getText(data)
                {
                    return data.timezoneName
                }
            }
        }
    }
    TimezoneViewModel {
        id: vm
    }
}
