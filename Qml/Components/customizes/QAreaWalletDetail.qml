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
import "../../Components/origins"
import "../customizes/Texts"
import "../customizes/Buttons"

Item {
    property bool isAssisted: false
    property real ratio: 0.55
    Item {
        id: area_wldetail
        anchors.fill: parent
        visible: false
        Row{
            anchors.fill: parent
            LinearGradient {
                height: parent.height
                width: parent.width * ratio
                start: Qt.point(0, 0)
                end: Qt.point(parent.width * ratio, 0)
                gradient: Gradient {
                    GradientStop { position: 0.0; color: isAssisted ? "#2F766D" : "#2F466C" }
                    GradientStop { position: 1.0; color: isAssisted ? "#1C4A21" : "#031F2B" }
                }
            }
            Rectangle {
                width: parent.width * (1.0 - ratio)
                height: parent.height
                color: "#F5F5F5"
            }
        }
    }
    Rectangle {
        id: mask
        anchors.fill: parent
        visible: false
        radius: 24
    }
    OpacityMask {
        id: realBalanceCard
        anchors.fill: area_wldetail
        source: area_wldetail
        maskSource: mask
    }
    DropShadow {
        anchors.fill: realBalanceCard
        horizontalOffset: 3
        verticalOffset: 3
        radius: 8.0
        samples: 17
        color: "#80000000"
        source: realBalanceCard
    }
//    InnerShadow {
//        anchors.fill: realBalanceCard
//        radius: 8.0
//        samples: 16
//        horizontalOffset: -3
//        verticalOffset: 3
//        color: "#b0000000"
//        source: realBalanceCard
//    }
}
