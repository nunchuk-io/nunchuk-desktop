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
import QtGraphicalEffects 1.0
import Qt.labs.platform 1.1
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

Rectangle {
    id: cointagRoot
    implicitHeight: _row.height
    implicitWidth: _row.width + 4*2
    radius: 12
    border.width: 1
    border.color: "#DEDEDE"
    property string coinColor: "#FAA077"
    property string coinText: "#NAME NAME NAME"
    property int    fontSize: 12
    property bool   expanded: false
    property int    textWidth: 200 - (showMoreButton.visible ? (showMoreButton.width + 4) : 0)

    Row {
        id: _row
        spacing: 4
        anchors.centerIn: parent
        Item {
            width: 16
            height: 24
            Rectangle {
                width: 16
                height: 16
                radius: 24
                border.width: 1
                border.color: "#DEDEDE"
                color: coinColor
                anchors.verticalCenter: parent.verticalCenter
            }
        }
        QLato {
            id: note
            property int cachedLineCount: 0
            text: coinText
            width: textWidth
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            lineHeightMode: Text.FixedHeight
            lineHeight: 16
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            elide: Text.ElideRight
            font.pixelSize: fontSize
            font.weight: Font.Normal
            anchors.verticalCenter: parent.verticalCenter
            Component.onCompleted: {
                note.cachedLineCount = note.lineCount
                note.maximumLineCount = 1
                if(note.cachedLineCount > 1) {
                    note.width = textWidth
                }
                else {
                    note.width = paintedWidth
                }
            }
        }
        QBadge {
            id: showMoreButton
            anchors{
                bottom: parent.bottom
                bottomMargin: 4
            }
            border.width: 1
            border.color: "#DEDEDE"
            color: "#F5F5F5"
            radius: 24
            text: expanded ? STR.STR_QML_1421 : STR.STR_QML_1094
            useMouse: true
            topMargin: 4
            fontSize: 10
            fontColor: "#031F2B"
            fontWeight: Font.DemiBold
            visible: note.cachedLineCount > 1
            onBadgeClicked: {
                expanded = !expanded
                if(expanded) {
                    note.maximumLineCount = note.cachedLineCount + 1
                }
                else {
                    note.maximumLineCount = 1
                }
            }
        }
    }
}
