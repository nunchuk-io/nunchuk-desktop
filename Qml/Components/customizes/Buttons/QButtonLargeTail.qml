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
import "../../customizes/Texts"

QBaseButton {
    id: rootTextButton
    // type enum
    readonly property int ePRIMARY  : eTypeH
    readonly property int eSECONDARY: eTypeI
    readonly property int eTHIRT: eTypeE
    type: ePRIMARY
    property string label: "text"
    property alias optionVisible: indicator.optionVisible
    width: 137
    height: 48
    QBackgroundState {
        anchors.fill: parent
    }
    Row {
        anchors.centerIn: parent
        spacing: 4
        QLatoState {
            id: text
            text: label
            font.weight: Font.Bold
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
        QIconRotate {
            id: indicator
            width: 16
            height: 16
            anchors.verticalCenter: text.verticalCenter
        }
    }

    onPressed: {
        indicator.pressed()
    }
    onButtonClicked: {
        indicator.released()
    }
    onContainsMouseChanged: {
        indicator.containsMouseChanged()
    }
}
