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
import "../../../Components/customizes/Texts"
import "../../../Components/origins"

Row {
    id: root
    width: parent.width
    height: 16
    spacing: 6

    // Properties
    property var buffer_period
    property bool buffer_period_changed: false
    property string buffer_apply_on: "FIRST_WITHDRAWAL"

    function bufferPeriod()
    {
        var ret = buffer_period.id === "" ? STR.STR_QML_921 : buffer_period.display_name
        if (buffer_apply_on == "FIRST_WITHDRAWAL") {
            ret += " (first withdrawal only)"
        } else {
            ret += " (every withdrawal)"
        }
        return ret
    }

    QIcon {
        anchors.verticalCenter: parent.verticalCenter
        source: "qrc:/Images/Images/period.svg"
        width: 16
        height: 16
    }

    Row {
        spacing: 2

        QLato {
            text: STR.STR_QML_2173
            font.pixelSize: 12
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignLeft
        }

        QLato {
            text: bufferPeriod()
            color: buffer_period_changed ? "#CF4018" : "#031F2B"
            font.pixelSize: 12
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignLeft
        }
    }
}
