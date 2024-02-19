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
import "../Texts"
import "../Buttons"
import "../../../../localization/STR_QML.js" as STR

Row {
    id: _row
    property string addText: ""
    property string addTitle: ""
    signal add();
    signal cancel();
    Item{
        width: 16
        height: _row.height
    }
    Rectangle {
        width: 282
        height: _row.height
        radius: 8
        color: "#D0E2FF"
        Item {
            anchors.fill: parent
            anchors.margins: 16
            QLato {
                anchors {
                    top: parent.top
                    topMargin: 0
                }
                width: 257
                height: paintedHeight
                text: addTitle
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                wrapMode: Text.WordWrap
                lineHeight: 28
                lineHeightMode: Text.FixedHeight
            }
            Item {
                anchors {
                    bottom: parent.bottom
                    bottomMargin: 0
                }
                width: 257
                height: 48
                QTextButton {
                    width: label.paintedWidth + 12*2
                    height: 48
                    anchors.left: parent.left
                    label.text: STR.STR_QML_245
                    label.font.pixelSize: 16
                    type: eTypeM
                    onButtonClicked: {
                        cancel()
                    }
                }
                QTextButton {
                    width: label.paintedWidth + 12*2
                    height: 48
                    anchors.right: parent.right
                    label.text: addText
                    label.font.pixelSize: 16
                    type: eTypeN
                    onButtonClicked: {
                        add()
                    }
                }
            }

        }
    }

}

