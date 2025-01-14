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
import "../origins"

Item {
    id: rootBusy
    QIcon {
        iconSize: 64
        id: circular
        anchors.centerIn: parent
        source: "qrc:/Images/Images/Circular.svg"
    }
    RotationAnimator {
        target: circular;
        from: 0;
        to: 360;
        duration: 600
        running: rootBusy.visible
        loops: Animation.Infinite
    }
    function loading() {
        rootBusy.visible = true
    }
    function stop() {
        rootBusy.visible = false
    }
}
