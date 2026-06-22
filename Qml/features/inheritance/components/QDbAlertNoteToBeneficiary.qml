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
import QtQuick.Controls 2.0
import "../../../Components/origins"
import "../../../Components/customizes/Texts"

Column {
    width: parent.width
    spacing: 12
    property string note: ""
    property bool noteChanged: false
    property var beneficiariesData: []
    property var emailNotes: {
        var result = [];
        for (var i = 0; i < beneficiariesData.length; i++) {
            var b = beneficiariesData[i];
            result.push({
                email: b.email,
                note: (b.note && b.note.length > 0) ? b.note : "",
                noteChanged: b.noteChanged
            });
        }
        return result;
    }
    QLato {
        width: parent.width
        height: paintedHeight
        text: STR.STR_QML_1047
        font.weight: Font.Bold
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
    }
    Rectangle {
        width: parent.width
        height: 52
        color: "#F5F5F5"
        radius: 12
        visible: emailNotes.length === 0
        QLato {
            width: parent.width
            anchors {
                left: parent.left
                leftMargin: 16
                verticalCenter: parent.verticalCenter
            }
            height: paintedHeight
            text: note
            color: noteChanged ? "#CF4018" : "#031F2B"
            font.weight: Font.Normal
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
        }
    }
    Rectangle {
        width: parent.width
        height: columnContent.childrenRect.height + 24
        color: "#F5F5F5"
        radius: 12
        visible: emailNotes.length > 0
        Column {
            id: columnContent
            anchors {
                fill: parent
                margins: 12
            }
            spacing: 12
            Repeater {
                model: emailNotes
                Column {
                    width: parent.width
                    anchors {
                        left: parent.left
                        leftMargin: 16
                    }
                    spacing: 12
                    QLato {
                        width: parent.width
                        height: paintedHeight
                        text: modelData.email
                        font.weight: Font.Bold
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                    QLato {
                        width: parent.width
                        height: paintedHeight
                        text: modelData.note
                        color: modelData.noteChanged ? "#CF4018" : "#031F2B"
                        font.weight: Font.Normal
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        visible: modelData.note.length > 0
                    }
                    QLine {
                        width: parent.width - 32
                        visible: index < emailNotes.length - 1
                    }
                }
            }
        }
    }
}