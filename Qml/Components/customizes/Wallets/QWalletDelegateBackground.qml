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
import QtGraphicalEffects 1.0
import Qt.labs.platform 1.1
import "../../origins"
import "../../customizes/Texts"
import "../../customizes/Buttons"

Item {
    id: _item
    property bool isHotWallet: false
    property bool isAssisted: false
    property bool hasOwner: true
    property bool isDashboard: false
    property bool isLocked: false
    property bool isReplaced: false
    property string walletRole: ""
    readonly property string gradientFrom   : isHotWallet ? "#A66800" : (isLocked || (walletRole === "KEYHOLDER_LIMITED") ? "#595959" : (isAssisted || isDashboard ? "#2F766D" : "#2F466C"))
    readonly property string gradientTo     : isHotWallet ? "#A66800" : (isLocked || (walletRole === "KEYHOLDER_LIMITED") ? "#595959" : (isAssisted || isDashboard ? "#1C4A21" : "#1C4A21"))
    Item {
        id: _detail
        anchors.fill: parent
        visible: false
        Column {
            anchors.fill: parent
            LinearGradient {
                height: 78
                width: _item.width
                start: Qt.point(0, 0)
                end: Qt.point(0, 78)
                gradient: Gradient {
                    orientation: Gradient.Horizontal
                    GradientStop { position: 0.0; color: gradientFrom }
                    GradientStop { position: 1.0; color: gradientTo }
                }
            }
            Rectangle {
                width: _item.width
                height: hasOwner ? 32 : 0
                color: "#F5F5F5"
            }
        }
    }
    Rectangle {
        id: mask
        anchors.fill: parent
        visible: false
        radius: 8
    }
    OpacityMask {
        id: _card
        anchors.fill: _detail
        source: _detail
        maskSource: mask
    }
    DropShadow {
        anchors.fill: _card
        horizontalOffset: 3
        verticalOffset: 3
        radius: 8.0
        samples: 32
        color: "#80000000"
        source: _card
    }
}
