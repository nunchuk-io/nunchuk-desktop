/**************************************************************************
 * This file is part of the Nunchuk software (https://nunchuk.io/)        *
 * Copyright (C) 2026 Nunchuk                                            *
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
 **************************************************************************/
import QtQuick
import "../../origins"
import "../Texts"

// Persistent, non-dismissible warning banner sourced from the generic
// home reminder API (HomeReminderViewModel). Unlike QWalletWarningInfo,
// this component:
//  - has no close ("X") button — visibility is entirely server-driven,
//  - is entirely clickable (not just a hyperlink inside the text),
//  - collapses to an arrow-only compact form when the available width
//    is too small to show the title/description comfortably.
//
// NOTE: icon + background color below are placeholders reusing the
// existing desktop "warning" style (#FDEBD2 / warning_amber icon). They
// have not yet been confirmed pixel-for-pixel against the Mobile app's
// generic-warning banner; swap once that asset/style parity is confirmed.
Rectangle {
    id: root

    property string title: ""
    property string description: ""
    property bool compact: width < 360

    // Emitted when the user clicks anywhere on the banner. Wiring the
    // actual navigation (HomeReminderViewModel.triggerAction(...)) is the
    // caller's responsibility so this component stays presentation-only.
    signal bannerClicked()

    // Deliberately no default `width` binding here: every caller sizes
    // this either via explicit anchors (left+right) or an explicit width
    // (e.g. inside a Column), and setting both an internal width binding
    // and external anchors on the same item is an anchor-vs-binding
    // conflict in QtQuick.
    height: 60
    radius: 8
    color: "#FDEBD2"

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        onClicked: root.bannerClicked()
    }

    Row {
        anchors {
            verticalCenter: parent.verticalCenter
            left: parent.left
            leftMargin: 12
            right: trailingArrow.left
            rightMargin: 8
        }
        spacing: 8

        QIcon {
            iconSize: 36
            anchors.verticalCenter: parent.verticalCenter
            source: "qrc:/Images/Images/warning_amber-60px.png"
        }

        Column {
            visible: !root.compact
            width: parent.width - 36 - parent.spacing
            anchors.verticalCenter: parent.verticalCenter
            spacing: 2

            QLato {
                width: parent.width
                visible: text.length > 0
                text: root.title
                textFormat: Text.PlainText
                font.pixelSize: 16
                font.weight: Font.Bold
                elide: Text.ElideRight
                maximumLineCount: 1
                wrapMode: Text.NoWrap
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
            }

            QLato {
                width: parent.width
                visible: text.length > 0
                text: root.description
                textFormat: Text.PlainText
                font.pixelSize: 13
                elide: Text.ElideRight
                maximumLineCount: 1
                wrapMode: Text.NoWrap
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
            }
        }
    }

    // Trailing arrow -- always shown (both compact and full form) as a
    // visual affordance that the whole banner is clickable, matching the
    // arrow-only compact form used elsewhere in the codebase (e.g. the
    // group-replace "accepted" banner in QHomeInitialStep3.qml).
    QIcon {
        id: trailingArrow
        iconSize: 24
        anchors {
            verticalCenter: parent.verticalCenter
            right: parent.right
            rightMargin: 12
        }
        source: "qrc:/Images/Images/right-arrow-dark.svg"
    }
}
