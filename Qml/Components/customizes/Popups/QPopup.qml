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
import QtQuick.Controls 1.4
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.12
import HMIEVENTS 1.0
import EWARNING 1.0
import QRCodeItem 1.0
import DataPool 1.0
import "../../origins"
import "../../customizes/Texts"
import "../../customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

Popup {
    width: parent.width
    height: parent.height
    modal: true
    focus: true
    property int offset: 24
    property int offsetRadius: 24
    property Component content
    readonly property Item itemInfo: contentInfo.item
    closePolicy: Popup.CloseOnReleaseOutside | Popup.CloseOnEscape
    background: Item{}
    Rectangle {
        id: boxmask
        width: contentInfo.implicitWidth + offset*2
        height: contentInfo.implicitHeight + offset*2
        anchors.centerIn: parent
        color: "#FFFFFF"
        radius: offsetRadius
        layer.enabled: true
        layer.effect: OpacityMask {
            maskSource: Rectangle {
                width: boxmask.width
                height: boxmask.height
                radius: boxmask.radius
            }
        }
        Loader {
            id: contentInfo
            anchors.centerIn: parent
            sourceComponent: content
        }
    }
    DropShadow {
        anchors.fill: boxmask
        horizontalOffset: 3
        verticalOffset: 5
        spread: 0
        radius: 8
        samples: 30
        color: "#aa000000"
        source: boxmask
    }
}
