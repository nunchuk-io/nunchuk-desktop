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
    property bool isHotWallet: false
    property bool isLocked: false
    property bool isReplaced: false
    property bool isSandboxWallet: false
    property real ratio: 0.55
    property string myRole: AppModel.walletInfo.myRole
    readonly property string gradientFrom: {
        if (isReplaced || isLocked)
            return "#595959"
        else if (isHotWallet)
            return "#A66800";
        else if (isSandboxWallet)
            return "#2B74A9";
        else if (isAssisted)
            return "#2F766D";
        else
            return "#2F466C";
    }

    readonly property string gradientTo : {
        if (isReplaced || isLocked)
            return "#595959"
        else if (isHotWallet)
            return "#A66800";
        else if (isSandboxWallet)
            return "#084B7B";
        else if (isAssisted)
            return "#1C4A21";
        else
            return "#031F2B";
    }
    Item {
        id: area_wldetail
        anchors.fill: parent
        visible: false

        LinearGradient {
            height: parent.height
            width: parent.width
            start: Qt.point(0, 0)
            end: Qt.point(parent.width, 0)
            gradient: Gradient {
                GradientStop { position: 0.0; color: gradientFrom }
                GradientStop { position: 1.0; color: gradientTo }
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
        samples: 32
        color: "#80000000"
        source: realBalanceCard
    }
    Rectangle {
        width: parent.width * (1.0 - ratio)
        height: parent.height
        anchors.right: parent.right
        color: "#F5F5F5"
        visible: (myRole !== "FACILITATOR_ADMIN")
        radius: 24
        Rectangle {
            width: 24
            height: parent.height
            anchors.left: parent.left
            color: "#F5F5F5"
        }
    }
}
