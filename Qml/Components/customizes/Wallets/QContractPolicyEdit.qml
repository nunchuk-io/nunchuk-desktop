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
import DataPool 1.0
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Chats"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

Rectangle {
    id: _background
    implicitWidth: _row.childrenRect.width + 8*2
    height: 28
    property string label: ""
    property var backgroundColor: ({normal: "#FDEBD2", hover: "#FDEBD2", selected: "#FFFFFF"})
    property var borderColor: ({normal: "transparent", hover: "#031F2B", selected: "#031F2B"})
    property bool isEditting: false
    signal editingUpdate(bool isEditting)
    border.width: 1
    radius: 4
    state: isEditting ? "selected" : (btnMouse.containsMouse ? "hover" : "default")

    Row {
        id: _row
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 4

        QLato {
            width: paintedWidth
            height: 20
            text: label
            font.weight: isEditting ? Font.Bold : Font.Normal
            font.underline: true
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            anchors {
                bottom: parent.bottom
                bottomMargin: -4
            }
        }

        QIcon {
            visible: !isEditting
            iconSize: 16
            source: "qrc:/Images/Images/edit-dark.svg"
            anchors {
                bottom: parent.bottom
                bottomMargin: -2
            }
        }
    }

    MouseArea {
        id: btnMouse
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        anchors.fill: parent
        onClicked: {
            editingUpdate(true)
        }
    }

    states: [
        State {
            name: "default"
            PropertyChanges {
                target: _background
                color: backgroundColor.normal
                border.color: borderColor.normal
            }
        },
        State {
            name: "hover"
            PropertyChanges {
                target: _background
                color: backgroundColor.hover
                border.color: borderColor.hover
            }
        },
        State {
            name: "disable"
            PropertyChanges {
                target: _background
                color: backgroundColor.disable
                border.color: borderColor.disable
            }
        },
        State {
            name: "selected"
            PropertyChanges {
                target: _background
                color: backgroundColor.selected
                border.color: borderColor.selected
            }
        }
    ]
}