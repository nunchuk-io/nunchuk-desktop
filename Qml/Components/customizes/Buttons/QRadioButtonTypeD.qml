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
import "../../origins"
import "../../customizes/Texts"

Row {
    id: radioRootD
    property bool selected: false
    property string labelTop: ""
    property string labelBottom: ""
    property alias center1: _center1
    property alias center2: _center2
    signal buttonClicked()
    spacing: 12
    Column {
        id: info
        width: radioRootD.width - 24 - 12
        height: radioRootD.height
        spacing: 4
        QLato {
            text: labelTop
            font.weight: Font.Bold
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
        }
        Item {
            width: info.width
            height: 24
            Row {
                anchors.fill: parent
                QBadge {
                    id: _center1
                    color: "#EAEAEA"
                }
                QBadge {
                    id: _center2
                    color: "#EAEAEA"
                    visible: _center2.icon !== ""
                }
            }
        }
        QLato {
            text: labelBottom
            font.weight: Font.Normal
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            color: "#595959"
        }
    }
    QIcon {
        id: icon
        iconSize: 24
        source: selected ? "qrc:/Images/Images/radio-selected-dark.svg" : "qrc:/Images/Images/radio-dark.svg"
        anchors.verticalCenter: parent.verticalCenter
        MouseArea {
            id: mouse
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor
            onClicked: { buttonClicked() }
        }
    }
}
