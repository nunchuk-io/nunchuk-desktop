/*
 * This file is part of the Nunchuk software (https://nunchuk.io/)
 * Copyright (C) 2020-2024 Enigmo
 * Copyright (C) 2024 Nunchuk
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
import QtQuick 2.4
import QtQuick.Controls 2.3
import QtGraphicalEffects 1.12
import Qt.labs.platform 1.1
import Features.Draftwallets.Sandbox.ViewModels 1.0
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Buttons"
import "../../Components/customizes/Lists"
import "../../Components/customizes/Texts"

Rectangle {
    signal addClicked()
    property bool plusEnabled: true
    property bool useBorder: false
    enabled: plusEnabled                
    width: 36
    height: 36
    radius: 36
    anchors.verticalCenter: parent.verticalCenter
    color: "transparent"
    border.width: 2
    border.color: !useBorder ?  "transparent" : getColor(plusEnabled, plusbtn.pressed)
    Rectangle {
        width: 12
        height: 2
        color: getColor(plusEnabled, plusbtn.pressed)
        anchors.centerIn: parent
    }
    Rectangle {
        width: 2
        height: 12
        color: getColor(plusEnabled, plusbtn.pressed)
        anchors.centerIn: parent
    }
    MouseArea {
        id: plusbtn
        anchors.fill: parent
        onClicked: {
            addClicked()
        }
    }
}