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
import "../Components/customizes"
import "../Components/customizes/Texts"
import "../Components/origins"

Item {
    id: toast
    width: 720
    height: childrenRect.height
    signal toastExpired()

    readonly property var warningColor: [
        "#EAEAEA",
        "#A7F0BA",
        "#FDEBD2",
        "#CF4018",
        "#CF4018"
    ]
    readonly property var warningTextColor: [
        "#031F2B",
        "#031F2B",
        "#031F2B",
        "#FFFFFF",
        "#FFFFFF"
    ]
    readonly property var warningIcon: [
        "qrc:/Images/Images/error-dark.svg",
        "qrc:/Images/Images/check-outline-dark.svg",
        "qrc:/Images/Images/warning-dark.svg",
        "qrc:/Images/Images/error-light.svg",
        "qrc:/Images/Images/error-light.svg",
    ]
    readonly property var closeIcon: [
        "qrc:/Images/Images/close-dark.svg",
        "qrc:/Images/Images/close-dark.svg",
        "qrc:/Images/Images/close-dark.svg",
        "qrc:/Images/Images/close-light.svg",
        "qrc:/Images/Images/close-light.svg",
    ]
    Rectangle {
        id: content
        width: toast.width - 4
        height: contentRow.height + 24
        anchors.horizontalCenter: parent.horizontalCenter
        radius: 8
        color: warningColor[type]
        Row {
            id: contentRow
            anchors.centerIn: parent
            spacing: 8
            QImage {
                width: 36
                height: 36
                source: warningIcon[type]
                anchors.verticalCenter: parent.verticalCenter
            }
            QText {
                id: _txt
                width: 600
                anchors.verticalCenter: parent.verticalCenter
                lineHeightMode: Text.FixedHeight
                lineHeight: 28
                wrapMode: Text.WordWrap
                color: warningTextColor[type]
                text: contentDisplay
            }
        }
    }
    DropShadow {
        anchors.fill: content
        horizontalOffset: 3
        verticalOffset: 3
        radius: 8.0
        samples: 17
        color: "#80000000"
        source: content
    }

    Timer {
        interval: 3000
        running: true
        repeat: false
        onTriggered: {
            toast.toastExpired()
        }
    }
}
