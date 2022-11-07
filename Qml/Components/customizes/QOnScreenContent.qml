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
import QtQuick.Controls 2.3
import QtGraphicalEffects 1.12
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import "../../Components/customizes"
import "../../Components/origins"

Item {
    id: root
    property alias label: screenname
    property alias closebutton: closeButton
    property alias extraHeader: extraHeaderInfo.sourceComponent
    property bool enableHeader: true
    signal closeClicked()
    Rectangle {
        id: mask
        anchors.fill: parent
        color: "#FFFFFF"
        radius: 24
        layer.enabled: true
        layer.effect: OpacityMask {
            maskSource: Rectangle {
                width: root.width
                height: root.height
                radius: 24
            }
        }
    }
    DropShadow {
        anchors.fill: mask
        horizontalOffset: 3
        verticalOffset: 3
        radius: 8.0
        samples: 17
        color: "#80000000"
        source: mask
    }
    QText {
        id: screenname
        visible: enableHeader
        anchors {
            left: parent.left
            leftMargin: 36
            top: parent.top
            topMargin: 36
        }
        text: "Screen name"
        color: "#031F2B"
        font.family: "Montserrat"
        font.weight: Font.Bold
        font.pixelSize: 24
    }
    Loader {
        id: extraHeaderInfo
        anchors {
            verticalCenter: screenname.verticalCenter
            left: screenname.right
            leftMargin: 8
        }
    }
    QCloseButton {
        id: closeButton
        visible: enableHeader
        enabled: visible
        anchors {
            verticalCenter: screenname.verticalCenter
            right: parent.right
            rightMargin: 36
        }
        onClicked: { closeClicked()}
    }
}
