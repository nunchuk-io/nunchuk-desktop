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
    id: progresBar
    width: 420
    height: 62
    property int percentage: 50

    Rectangle {
        id: barbg
        width: parent.width
        height: 8
        radius: 8
        color: "#323E4A"
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 3

        Rectangle {
            id: progressbg
            width: percentage*parent.width/100
            height: 8
            radius: 8
            anchors.left: parent.left
            color: "#F6D65D"
        }
    }

    QImage {
        id: name
        source: "qrc:/Images/Images/snipbar.png"
        width: 32
        height: 62
        x: progressbg.width - width/2
        QText {
            width: 32
            height: 10
            anchors.top: parent.top
            anchors.topMargin: 11
            font.weight: Font.Bold
            font.family: "Lato"
            font.pixelSize: 10
            text: percentage + " %"
            color: "#031F2B"
            horizontalAlignment: Text.AlignHCenter
        }
    }
}
