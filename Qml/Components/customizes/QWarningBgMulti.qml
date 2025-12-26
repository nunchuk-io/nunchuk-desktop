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
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.0
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import "../../Components/origins"
import "../../Components/customizes/Texts"
import "../../../localization/STR_QML.js" as STR

Rectangle {
    id: _root
    width: 528
    height: 60
    color: "#EAEAEA"
    radius: 12
    property string icon: ""
    property alias txt: _txt
    property int iSize: 24

    Row {
        anchors.fill: parent
        anchors.margins: 12
        spacing: 12
        QIcon {
            iconSize: iSize
            source: icon
            anchors.verticalCenter: parent.verticalCenter
            visible: icon !== ""
        }
        QLato {
            id: _txt
            width: _root.width - 2*12 - iSize - 12
            height: 60 - 2*12
            wrapMode: Text.WordWrap
            lineHeight: 28
            lineHeightMode: Text.FixedHeight
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            anchors.verticalCenter: parent.verticalCenter
            onLinkActivated: Qt.openUrlExternally(link)
            MouseArea {
                anchors.fill: parent
                cursorShape: _txt.hoveredLink ? Qt.PointingHandCursor : Qt.ArrowCursor
                acceptedButtons: Qt.NoButton
            }
        }
    }
}
