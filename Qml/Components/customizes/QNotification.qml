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
import "../origins"
import "../customizes/Texts"

Rectangle {
    id: toast
    width: visible ? 718 : 0
    height: visible ? 48 : 0
    enabled: visible
    radius: 4

    property int currentStatus: 0
    property alias label: contentMsg.text
    property alias wrapMode: contentMsg.wrapMode
    property alias labelWidth: contentMsg.width

    Rectangle {
        anchors.fill: parent
        radius: 4
        color: backgroundColor[currentStatus]
        opacity: 0.1
    }

    readonly property var backgroundColor: [
        "#35abee",
        "#35abee",
        "#f6d65d",
        "#e02247",
        "#e02247"
    ]
    readonly property var borderColor: [
        "#35ABEE",
        "#35ABEE",
        "#F6D65D",
        "#E02247",
        "#E02247"
    ]
    readonly property var icon: [
        "qrc:/Images/Images/notification_success.png",
        "qrc:/Images/Images/notification_success.png",
        "qrc:/Images/Images/notification_warning.png",
        "qrc:/Images/Images/notification_error.png",
        "qrc:/Images/Images/notification_error.png"
    ]

    border.color: borderColor[currentStatus]
    color: "transparent"

    signal closeRequest()

    Row {
        anchors.centerIn: parent
        spacing: 10
        Image {
            id: ico
            width: 32
            height: 32
            anchors.verticalCenter: parent.verticalCenter
            source: icon[currentStatus]
        }
        Flickable {
            width: 620
            height: toast.height - 5
            anchors.verticalCenter: parent.verticalCenter
            flickableDirection: Flickable.HorizontalFlick
            clip: true
            interactive: (width < contentWidth)
            contentWidth: contentMsg.width
            ScrollBar.horizontal: ScrollBar { active: true }
            QText {
                id: contentMsg
                font.pixelSize: 16
                font.family: "Lato"
                color: "#031F2B"
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        QIcon {
            iconSize: 24
            anchors.verticalCenter: parent.verticalCenter
            source: "qrc:/Images/Images/close_toast_24px.png"
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                onClicked: { closeRequest() }
            }
        }
    }


    property bool autoClose: false
    property int timeout: 3000

    Timer {
        interval: timeout
        running: true
        onTriggered: { if(autoClose) closeRequest() }
    }
}
