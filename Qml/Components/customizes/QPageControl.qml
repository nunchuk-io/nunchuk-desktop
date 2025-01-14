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
import "../origins"
import "../customizes"

Row {
    width: 144
    height: 24
    spacing: 16
    readonly property var chevronLeft: ["qrc:/Images/Images/chevron-left-dark.svg", "qrc:/Images/Images/chevron-left-dark-disabled.svg"]
    readonly property var chevronRight: ["qrc:/Images/Images/chevron-right-dark.svg", "qrc:/Images/Images/chevron-right-dark-disabled.svg"]
    readonly property var firstPage: ["qrc:/Images/Images/first_page_24px_n.png", "qrc:/Images/Images/first_page_24px_d.png"]
    readonly property var lastPage: ["qrc:/Images/Images/last_page_24px_n.png", "qrc:/Images/Images/last_page_24px_d.png"]

    property int currentPage: 1
    property int totalPage: 5

    signal pageRequest(var pageNumber)

    QIcon {
        iconSize: 24
        enabled: currentPage != 1
        source: enabled ? firstPage[0] : firstPage[1]
        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor
            onClicked: {
                currentPage = 1
                pageRequest(currentPage)
            }
        }
    }
    QIcon {
        iconSize: 24
        enabled: (currentPage > 1)
        source: enabled ? chevronLeft[0] : chevronLeft[1]
        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor
            onClicked: {
                currentPage--;
                pageRequest(currentPage)
            }
        }
    }

    QIcon {
        iconSize: 24
        enabled: (currentPage < totalPage)
        source: enabled ? chevronRight[0] : chevronRight[1]
        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor
            onClicked: {
                currentPage++;
                pageRequest(currentPage)
            }
        }
    }
    QIcon {
        iconSize: 24
        enabled: (currentPage != totalPage)
        source: enabled ? lastPage[0] : lastPage[1]
        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor
            onClicked: {
                currentPage = totalPage
                pageRequest(currentPage)
            }
        }
    }
}
