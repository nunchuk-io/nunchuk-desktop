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
import "../../../../Components/origins"
import "../../../../Components/customizes"
import "../../../../Components/customizes/Chats"
import "../../../../Components/customizes/Texts"
import "../../../../Components/customizes/Buttons"
import "../../../../../localization/STR_QML.js" as STR

Item {
    property bool isTransaction : false
    Row {
        anchors.fill: parent
        spacing: 4
        QIcon {
            iconSize: 20
            source: "qrc:/Images/Images/Timer.svg"
            anchors {
                top: parent.top
                topMargin: 8
            }
        }
        Column {
            id: column
            anchors{
                top: parent.top
                topMargin: 8
            }
            width: parent.width
            spacing: 2
            Row {
                width: parent.width
                height: isTransaction ? 40 : 20
                spacing: 4
                QLato {
                    id: firstLine
                    width: paintedWidth
                    height: 20
                    text: miniscript.firstLine
                    font.weight: Font.Normal
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
                QLato {
                    width: parent.width - firstLine.paintedWidth - 4
                    height: isTransaction ? 40 : 20
                    text: getTitle()
                    font.weight: Font.Normal
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    lineHeight: 20
                    lineHeightMode: Text.FixedHeight
                    wrapMode: Text.WrapAnywhere
                }
            }
            QLato {
                width: parent.width
                height: paintedHeight
                text: getDescription()
                font.weight: Font.Normal
                font.pixelSize: 12
                color: "#757575"
                lineHeight: 16
                lineHeightMode: Text.FixedHeight
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                visible: text !== ""
            }
        }
    }
}
