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

Column {
    width: parent.width
    spacing: 12
    property int inactivity_interval_count: 0
    property bool inactivity_interval_countChanged: false
    
    QLato {
        width: parent.width
        height: paintedHeight
        text: STR.STR_QML_2169
        font.weight: Font.Bold
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
    }
    Rectangle {
        id: root
        width: parent.width
        height: 52
        color: "#F5F5F5"
        radius: 12

        Column {
            anchors {
                fill: parent
                margins: 12
            }
            spacing: 12

            // Description text
            QLato {
                width: parent.width
                text: STR.STR_QML_2170.arg(inactivity_interval_count)
                color: inactivity_interval_countChanged ? "#CF4018" : "#031F2B"
                font.pixelSize: 16
                lineHeight: 20
                wrapMode: Text.WrapAnywhere
                lineHeightMode: Text.FixedHeight
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
            }
        }
    }
}