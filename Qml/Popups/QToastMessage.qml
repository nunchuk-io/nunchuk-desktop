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

Item {
    id: rootToast
    property ListModel model
    property bool isShowing: (model.count > 0)
    property bool isScreenBase: false
    readonly property int bottomMargin: isScreenBase ? 105 : 225
    readonly property int horizontalOffset: isScreenBase ? (width/2 - (width - 386)/2) : 0
    signal toastVisibleChanged()

    Loader {
        anchors.fill: parent
        sourceComponent: isShowing ? mousePrevent : null
    }

    Component {
        id: mousePrevent
        Item {
            anchors.fill: parent
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    rootToast.model.clear()
                    toastVisibleChanged()
                }
            }
        }
    }

    Item {
        id: clickable
        width: 720
        height: parent.height - bottomMargin
        anchors.bottom: parent.bottom
        anchors.bottomMargin: bottomMargin
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.horizontalCenterOffset: horizontalOffset
        clip: true
        visible: isShowing
        ListView {
            id: mylist
            width: clickable.width - 8
            height: contentHeight
            model: rootToast.model
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 4
            spacing: 16
            delegate: QToastItem {
                width: mylist.width
                onToastExpired: {
                    if (index >= 0 && index < rootToast.model.count) {
                        rootToast.model.remove(index, 1)
                    }
                    toastVisibleChanged()
                }
            }
        }
    }
}
