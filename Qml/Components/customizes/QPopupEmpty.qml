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
import "../origins"
import "../customizes/Texts"
import "../customizes/Buttons"
import "../../../localization/STR_QML.js" as STR

Popup {
    id: _infoPopup
    width: parent.width
    height: parent.height
    modal: true
    focus: true
    property alias content: contentInfo.sourceComponent
    property string contentText: ""
    property var coverColor: Qt.rgba(255, 255, 255, 0.7)
    signal gotItClicked()
    closePolicy: Popup.CloseOnReleaseOutside | Popup.CloseOnEscape
    background: Item{}
    Rectangle {
        id: bgcontentlimit
        width: Math.max(300,contentInfo.implicitWidth)
        height: Math.max(254,contentInfo.implicitHeight)
        anchors.centerIn: parent
        color: "#FFFFFF"
        radius: 24
        Loader {
            id: contentInfo
            anchors.centerIn: parent
        }
    }
    DropShadow {
        anchors.fill: bgcontentlimit
        horizontalOffset: 3
        verticalOffset: 5
        spread: 0
        radius: 8
        samples: 30
        color: "#aa000000"
        source: bgcontentlimit
    }
}
