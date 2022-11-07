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
import "../customizes"
import "../../../localization/STR_QML.js" as STR
Popup {
    id: toast
    width: 728
    height: 60
    modal: false
    focus: true
    closePolicy: Popup.CloseOnPressOutside
    signal popupClosed()
    property bool showClose: false
    property int    warningType: EWARNING.SUCCESS_MSG  // EWARNING.NONE_MSG,
                                            // EWARNING.SUCCESS_MSG,
                                            // EWARNING.WARNING_MSG,
                                            // EWARNING.ERROR_MSG,
                                            // EWARNING.EXCEPTION_MSG
    property int    warningCode: 0
    property string warningWhat: ""
    property string warningContent: ""
    property string warningExplain: ""

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

    background:Item {
        width: toast.width
        height: toast.height
        Rectangle {
            id:background
            implicitWidth: parent.width
            implicitHeight: parent.height
            radius: 8
            color: warningColor[warningType]
            visible: false
        }

        DropShadow {
            width: toast.width
            height: toast.height
            horizontalOffset: 3
            verticalOffset: 3
            radius: 8.0
            samples: 17
            color: "#80000000"
            source: background
        }
    }

    MouseArea { anchors.fill: parent ; onClicked: {toast.close()}}
    Row {
        id: contentRow
        anchors.centerIn: parent
        width: toast.width - 30
        spacing: 8
        QImage {
            width: 36
            height: 36
            source: warningIcon[warningType]
            anchors.verticalCenter: parent.verticalCenter
        }
        QText {
            width: 600
            anchors.verticalCenter: parent.verticalCenter
            lineHeightMode: Text.FixedHeight
            lineHeight: 28
            wrapMode: Text.WrapAnywhere
            color: warningTextColor[warningType]
            text: warningType === EWARNING.SUCCESS_MSG || warningType === EWARNING.WARNING_MSG ? warningExplain : warningContent
        }
        Rectangle {
            width: 48
            height: 48
            color: !closemouse.containsMouse ? "transparent" : Qt.rgba(0, 0, 0, 0.1)
            anchors.verticalCenter: parent.verticalCenter
            radius: 48
            visible: showClose
            QImage {
                width: 24
                height: 24
                source: closeIcon[warningType]
                anchors.centerIn: parent
            }
            MouseArea {
                id: closemouse
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                onClicked: { toast.close() }
            }
        }
    }

    Timer {
        id: timer4s
        interval: 4000
        repeat: false
        running: false
        onTriggered: toast.close()
    }
    onOpened: {
        timer4s.restart()
    }
    onClosed: {
        timer4s.stop()
        popupClosed()
    }
}
