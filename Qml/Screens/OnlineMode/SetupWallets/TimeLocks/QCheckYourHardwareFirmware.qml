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
import Qt.labs.platform 1.1
import NUNCHUCKTYPE 1.0
import "../../../../Components/origins"
import "../../../../Components/customizes"
import "../../../../Components/customizes/Chats"
import "../../../../Components/customizes/Texts"
import "../../../../Components/customizes/Buttons"

QOnScreenContentTypeA {
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent    
    onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
    property string linkWeb: "https://www.nunchuk.io/privacy.html"
    property string guideText: ""
    property string guideWeb: ""
    property int offsetWidth: 50
    content: Item {
        Row {
            id: contentRow
            height: 512
            spacing: 36
            Rectangle {
                width: 346
                height: contentRow.height
                radius: 24
                color: "#D0E2FF"
                QPicture {
                    anchors.centerIn: parent
                    source: "qrc:/Images/Images/hardware-illustration.svg"
                }
            }
            Item {
                width: 346
                height: contentRow.height
                Column {
                    width: parent.width
                    spacing: 24
                    QLato {
                        width: parent.width
                        text: guideText
                        lineHeightMode: Text.FixedHeight
                        lineHeight: 28
                        wrapMode: Text.WordWrap
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                    QLato {
                        id: webGuide
                        width: parent.width - offsetWidth
                        text: guideWeb.arg(linkWeb)
                        lineHeightMode: Text.FixedHeight
                        lineHeight: 28
                        textFormat: Text.RichText
                        wrapMode: Text.WordWrap
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        onLinkActivated: Qt.openUrlExternally(linkWeb)
                        MouseArea {
                            anchors.fill: parent
                            cursorShape: webGuide.hoveredLink ? Qt.PointingHandCursor : Qt.ArrowCursor
                            acceptedButtons: Qt.NoButton
                        }
                    }
                }
            }
        }
    }
}
