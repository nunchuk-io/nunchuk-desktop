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
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Chats"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

Rectangle {
    width: 500
    height: 196
    color: "#D0E2FF"
    property string title: "Assisted wallet"
    property string description: "For those who are new to Bitcoin, need expert guidance, or want advanced features such as inheritance planning, spending limit and multi-user wallets."
    property string desImg: ""
    radius: 12
    signal onBoardingclicked
    MouseArea {
        anchors.fill: parent
        onClicked: {
            onBoardingclicked()
        }
    }
    Row {   
        anchors {
            fill: parent
            leftMargin: 28
            topMargin: 24
            bottomMargin: 24
            rightMargin: 28
        }
        spacing: 16
        Item {
            anchors {
                left: parent.left
                bottom: parent.bottom
            }
            width: 308
            height: _col.childrenRect.height
        
            Column {
                id: _col
                spacing: 8
                QLato {
                    width: 308
                    height: 28
                    text: title
                    font.bold: true
                    font.pixelSize: 20
                    font.weight: Font.Bold
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                }
                QLato {
                    width: 308
                    height: paintedHeight
                    text: description
                    font.pixelSize: 16
                    wrapMode: Text.WordWrap
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    lineHeightMode: Text.FixedHeight
                    lineHeight: 28
                }
            }
        }
        
        QImage {
            anchors {
                right: parent.right
                bottom: parent.bottom
            }
            source: desImg
            width: 120
            height: 120
        }
    }
}
