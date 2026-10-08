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
import QtQuick
import QtQuick.Controls
import "../../../Components/customizes/Texts"
import "../../../Components/origins"

Row {
    id: root
    width: parent.width
    height: wrap ? Math.max(16, _innerRow.implicitHeight) : 16
    spacing: 6

    // Properties
    property var buffer_period
    property bool buffer_period_changed: false
    property string buffer_apply_on: "FIRST_WITHDRAWAL"
    property bool wrap: false

    function bufferPeriod()
    {
        var ret = ""
        if (!buffer_period || !buffer_period.id || buffer_period.id === "") {
            ret = QSTR.STR_QML_2252
        } else {
            ret = buffer_period.display_name
        }

        if (buffer_apply_on === "FIRST_WITHDRAWAL") {
            ret += " (first withdrawal only)"
        } else if (buffer_apply_on === "EVERY_WITHDRAWAL") {
            ret += " (every withdrawal)"
        }
        return ret
    }

    QIcon {
        id: _icon
        anchors.verticalCenter: parent.verticalCenter
        source: "qrc:/Images/Images/period.svg"
        width: 16
        height: 16
    }

    Row {
        id: _innerRow
        spacing: 2

        QLato {
            id: _lbl
            text: QSTR.STR_QML_2173
            font.pixelSize: 12
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignLeft
        }

        QLato {
            width: root.wrap ? (root.width - _icon.width - root.spacing - _lbl.implicitWidth - _innerRow.spacing) : implicitWidth
            text: bufferPeriod()
            color: buffer_period_changed ? "#CF4018" : "#031F2B"
            font.pixelSize: 12
            wrapMode: root.wrap ? Text.WordWrap : Text.NoWrap
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignLeft
        }
    }
}
