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
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.12
import HMIEVENTS 1.0
import EWARNING 1.0
import QRCodeItem 1.0
import DataPool 1.0
import "../../origins"
import "../../customizes/Texts"
import "../../customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

QPopup {
    id: _infoPopup
    width: 350
    height: 500
    anchors.centerIn: parent
    property string timeString: ""
    property int customHours: getHours(timeString)
    property int customMinutes: getMinutes(timeString)
    signal timeChanged(int newHours, int newMinutes)
    content: analog_clock
    Component {
        id: analog_clock
        QAnalogClock {
            id: analogClock
            anchors.centerIn: parent
            hours: customHours
            minutes: customMinutes
            
            onTimeChanged: {
                customHours = newHours
                customMinutes = newMinutes
            }
            
            onCancelClicked: {
                // Reset to current time
                customHours = getHours(timeString)
                customMinutes = getMinutes(timeString)
                _infoPopup.close()
            }
            
            onOkClicked: {
                timeString = hours.toString().padStart(2,'0') + ":" + minutes.toString().padStart(2,'0')
                _infoPopup.close()
            }
        }
    }

    // Parse hours from a string in "hh:mm" format (returns 0-23). Fallback to 0 on invalid input.
    function getHours(str) {
        if (typeof str !== "string")
            return 0;
        var s = str.trim();
        var parts = s.split(":");
        if (parts.length < 1)
            return 0;
        var h = parseInt(parts[0], 10);
        if (isNaN(h))
            return 0;
        if (h < 0) h = 0;
        if (h > 23) h = 23;
        return h;
    }

    // Parse minutes from a string in "hh:mm" format (returns 0-59). Fallback to 0 on invalid input.
    function getMinutes(str) {
        if (typeof str !== "string")
            return 0;
        var s = str.trim();
        var parts = s.split(":");
        if (parts.length < 2)
            return 0;
        var m = parseInt(parts[1], 10);
        if (isNaN(m))
            return 0;
        if (m < 0) m = 0;
        if (m > 59) m = 59;
        return m;
    }
}
