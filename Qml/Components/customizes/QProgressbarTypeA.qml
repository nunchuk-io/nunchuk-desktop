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
import "../customizes/Texts"
import "../customizes/Buttons"

Item {
    id: progresBar
    width: 400
    height: 12
    property int percentage: 50

    Rectangle {
        id: barbg
        width: parent.width
        height: progresBar.height
        radius: progresBar.height
        color: "#C4C4C4"
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 3

        Rectangle {
            id: progressbg
            width: percentage*parent.width/100
            height: progresBar.height
            radius: progresBar.height
            anchors.left: parent.left
            color: "#031F2B"
        }
    }
}
