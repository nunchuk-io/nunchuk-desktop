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
import DataPool 1.0
import "../../../Components/customizes/Texts"
import "../../../../localization/STR_QML.js" as STR

Column {
    width: parent.width
    spacing: 12
    property bool isEditing: false
    signal memoNotify(var newMemo)
    property string transactionNote: transactionInfo.memo
    Rectangle {
        width: parent.width
        height: 48
        color: "#F5F5F5"
        QText {
            text: STR.STR_QML_218
            font.family: "Lato"
            font.weight: Font.Bold
            font.pixelSize: 12
            color: "#323E4A"
            anchors {
                left: parent.left
                leftMargin: 12
                verticalCenter: parent.verticalCenter
            }
        }
        QTextLink {
            id: _edit
            width: _edit.paintedWidth
            height: 20
            font.pixelSize: 12
            text: isEditing ? STR.STR_QML_835: STR.STR_QML_849
            anchors {
                verticalCenter: parent.verticalCenter
                right: parent.right
                rightMargin: 12
            }
            onTextClicked: {
                if (isEditing) {
                    isEditing = false
                    transactionNote = editNote.text
                    memoNotify(editNote.text)
                    editNote.focus = false
                } else {
                    isEditing = true
                    editNote.focus = true
                    editNote.forceActiveFocus()
                }
            }
        }
    }
    QText {
        id: txMemo
        width: parent.width - 24
        text: txMemo.convertToClickableLinks(transactionNote)
        lineHeightMode: Text.FixedHeight
        lineHeight: 20
        font.pixelSize: 16
        color: "#323E4A"
        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
        anchors.horizontalCenter: parent.horizontalCenter
        visible: !isEditing
        onLinkActivated: {
            Qt.openUrlExternally(link)
        }
        function convertToClickableLinks(text) {
            // Regular expression to match URLs (simplified)
            var urlRegex = /(https?:\/\/[^\s]+)/g;
            // Replace URLs in the text with clickable <a> tags
            return text.replace(urlRegex, "<a href='$1'>$1</a>");
        }
    }
    QTextInputBoxTypeA {
        id: editNote
        width: parent.width - 24
        visible: isEditing
        showEdit: false
        placeholderText: ""
        text: transactionNote
        backgroundColor: "Transparent"
        borderColor: "Transparent"
        color: "#000000"
        font.family: "Montserrat"
        font.pixelSize: 16
        anchors.horizontalCenter: parent.horizontalCenter
    }
}
