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

Item {
    implicitWidth:  width
    implicitHeight: rowDisplay.height + 16

    property int    gapWidth: 24
    property int    memoWidth: width - 48 - gapWidth
    property string memo: ""
    property bool   expanded: false

    QLato {
        id: hiddenNote
        text: convertToClickableLinks(memo)
        width: memoWidth
        font.pixelSize: 12
        font.weight: Font.Normal
        verticalAlignment: Text.AlignVCenter
        lineHeightMode: Text.FixedHeight
        lineHeight: 24
        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
        visible: false
    }

    Row {
        id: rowDisplay
        width: parent.width - 16
        anchors.centerIn: parent
        spacing: 4
        Rectangle {
            width: 24
            height: 24
            radius: 24
            color: "#F5F5F5"
            border.width: 1
            border.color: "#DEDEDE"
            QIcon {
                iconSize: 16
                anchors.centerIn: parent
                source: "qrc:/Images/Images/transaction-note-dark.svg"
            }
        }
        QLato {
            id: note
            text: convertToClickableLinks(memo)
            width: memoWidth
            font.pixelSize: 12
            font.weight: Font.Normal
            verticalAlignment: Text.AlignVCenter
            lineHeightMode: Text.FixedHeight
            lineHeight: 24
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            elide: Text.ElideRight
            maximumLineCount: 1
            onLinkActivated: {
                Qt.openUrlExternally(link)
            }
        }
    }

    QBadge {
        id: showMoreButton
        anchors{
            right: parent.right
            rightMargin: 8
            bottom: parent.bottom
            bottomMargin: 8
        }
        border.width: 1
        border.color: "#DEDEDE"
        color: "#F5F5F5"
        radius: 24
        text: expanded ? STR.STR_QML_1421 : STR.STR_QML_1094
        useMouse: true
        fontSize: 12
        fontColor: "#031F2B"
        fontWeight: Font.DemiBold
        visible: hiddenNote.lineCount > 1
        onBadgeClicked: {
            expanded = !expanded
            if(expanded) {
                gapWidth = 0
                note.maximumLineCount = hiddenNote.lineCount + 1
            }
            else {
                gapWidth = 24
                note.maximumLineCount = 1
            }
        }
    }

    function convertToClickableLinks(text) {
        // Regular expression to match URLs (simplified)
        var urlRegex = /(https?:\/\/[^\s]+)/g;
        // Replace URLs in the text with clickable <a> tags
        return text.replace(urlRegex, "<a href='$1'>$1</a>");
    }
}
