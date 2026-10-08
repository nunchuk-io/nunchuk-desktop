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
import QtQuick.Controls 2.1
import QtGraphicalEffects 1.0
import HMIEVENTS 1.0
import EWARNING 1.0
import QRCodeItem 1.0
import DataPool 1.0
import NUNCHUCKTYPE 1.0
import "../origins"
import "../customizes/Texts"
import "../../../localization/STR_QML.js" as STR

Item {
    height: 48
    property string verSystemName: ""
    property string verDeviceName: ""
    property string rightText: ""
    property bool thisDevice: false
    signal signOutClicked()
    signal markClicked()
    QText {
        id:_name
        font.pixelSize: 16
        font.family: "Lato"
        text: verSystemName
        height: 16
        horizontalAlignment: Text.AlignLeft
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: _des.visible ? -9: 0
        anchors.left: parent.left
    }
    QText {
        id:_des
        font.pixelSize: 14
        color: "#595959";
        font.family: "Lato"
        text: verDeviceName + (thisDevice ? " (This device)":"")
        visible: _des.text != ""
        anchors.left: parent.left
        anchors.top: _name.bottom
        anchors.topMargin: 2
    }

    QText {
        id:_signOut
        anchors{
            right: textmark.left
            rightMargin: 12
            verticalCenter: parent.verticalCenter
        }
        width: paintedWidth
        font.pixelSize: 16
        font.weight: Font.Bold
        color: "#031F2B"
        font.family: "Lato"
        text: STR.STR_QML_548
        scale: btnSign.containsMouse ? 1.1 : 1.0
        visible: !thisDevice
        MouseArea {
            id: btnSign
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor
            anchors.fill: _signOut
            onClicked: signOutClicked()
        }
    }

    QText {
        id: textmark
        width: paintedWidth
        anchors{
            right: parent.right
            rightMargin: 12
            verticalCenter: parent.verticalCenter
        }
        visible: !thisDevice
        font.pixelSize: 16
        color: "#CF4018";
        font.family: "Lato"
        font.weight: Font.Bold
        text: rightText
        scale: btnMark.containsMouse ? 1.1 : 1.0
        MouseArea {
            id: btnMark
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor
            anchors.fill: textmark
            onClicked: markClicked()
        }
    }
}
