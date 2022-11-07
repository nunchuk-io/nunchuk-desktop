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
import QtGraphicalEffects 1.0
import DataPool 1.0
import "../origins"

QBaseButton {
    id: idRootStyle1
    property string label: "Value"
    property int fontPixelSize: 10
    property int iconSize: 24
    property bool iconInRight: false

    Row {
        anchors.centerIn: parent
        spacing: 8
        layoutDirection: iconInRight ?  Qt.RightToLeft : Qt.LeftToRight
        QImage {
            id: idIcon
            width: iconSize
            height: iconSize
            anchors.verticalCenter: parent.verticalCenter
        }
        QText {
            id: idText
            anchors.verticalCenter: parent.verticalCenter
            text: label
            font.pixelSize: fontPixelSize
            font.weight: Font.Bold
            font.family: "Lato"
        }
    }
}
