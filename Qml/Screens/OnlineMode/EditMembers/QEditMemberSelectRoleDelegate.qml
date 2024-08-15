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
import QtQuick 2.0
import "../../../Components/customizes"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Members"
import "../../../Components/customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

Rectangle {
    border.width: 1
    border.color: "#DEDEDE"
    height: _col.childrenRect.height + 18
    radius: 12
    Item {
        id: _col
        anchors {
            top: parent.top
            left: parent.left
            leftMargin: 18
        }
        QRadioButtonTypeA {
            id: _radio
            width: 538
            height: 48
            label: modelData.name
            fontPixelSize: 16
            fontWeight: Font.Bold
            selected: option === modelData.id
            layoutDirection: Qt.RightToLeft
            onButtonClicked: {
                option = modelData.id
            }
        }

        QLato {
            anchors {
                top: _radio.bottom
                topMargin: -8
                left: parent.left
                leftMargin: 54 - 18
            }
            height: modelData.des1 !== "" ? 20 : 0
            text: modelData.des1
            font.pixelSize: 12
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            visible: modelData.des1 !== ""
            color: "#595959"
        }
        QLato {
            anchors {
                top: _radio.bottom
                topMargin: modelData.des1 !== "" ? 24 : -4
                left: parent.left
                leftMargin: 54 - 18
            }
            width: parent.width
            height: modelData.des1 !== "" ? (paintedHeight - 12) : (paintedHeight - 8)
            text: modelData.des2
            lineHeightMode: Text.FixedHeight
            lineHeight: 28
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
        }
    }
}
