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
import NUNCHUCKTYPE 1.0
import "../origins"
import "../customizes"
import "../customizes/Chats"
import "../customizes/Texts"
import "../customizes/Buttons"
import "../../../localization/STR_QML.js" as STR

Item {
    property string btnTextLink: "btnTextLink"
    property string titleSuggest: "titleSuggest"
    property string content: "content"
    property string icon: "icon"
    signal btnClicked()
    width: 600
    height: 180
    Rectangle {
        id: rectEmptyState
        anchors.fill: parent
        anchors.margins: 2
        color: "#FFFFFF"
        radius: 24
        layer.enabled: true
        layer.effect: OpacityMask {
            maskSource: Rectangle {
                width: rectEmptyState.width
                height: rectEmptyState.height
                radius: rectEmptyState.radius
            }
        }
        Row {
            anchors.fill: parent
            anchors.margins: 28
            spacing: 24
            QIcon {
                iconSize: 96
                source: icon
            }
            Column {
                spacing: 4
                QLato {
                    font.pixelSize: 20
                    font.weight: Font.Bold
                    text: titleSuggest
                    color: "#000000"
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                }
                QLato {
                    width: 424
                    font.pixelSize: 16
                    text: content
                    color: "#000000"
                    lineHeightMode: Text.FixedHeight
                    lineHeight: 28
                    wrapMode: Text.WordWrap
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                }
                QButtonTextLink {
                    height: 24
                    label: btnTextLink
                    direction: eRIGHT
                    fontPixelSize: 16
                    onButtonClicked: {
                        btnClicked()
                    }
                }
            }
        }
    }
    DropShadow {
        anchors.fill: rectEmptyState
        horizontalOffset: 0
        verticalOffset: 0
        spread: 0
        radius: 8
        samples: 30
        color: "#aa000000"
        source: rectEmptyState
    }
}
