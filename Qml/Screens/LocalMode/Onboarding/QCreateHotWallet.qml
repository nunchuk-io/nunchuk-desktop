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
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Chats"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

QBoxShadow {
    id: _item
    width: 500
    height: 196
    color: "#FFFFFF"
    property string title: "Assisted wallet"
    property string description: ""
    property string desImg: ""
    radius: 12
    signal itemclicked
    MouseArea {
        anchors.fill: parent
        onClicked: {
            itemclicked()
        }
    }
    Row {
        anchors {
            left: parent.left
            leftMargin: 28
            verticalCenter: parent.verticalCenter
        }
        spacing: 24
        QImage {
            source: desImg
            width: 48
            height: 48
            anchors.verticalCenter: parent.verticalCenter
        }
        Item {
            width: 363
            height: _col.childrenRect.height
            anchors.verticalCenter: parent.verticalCenter
            Column {
                id: _col
                spacing: 8
                QLato {
                    width: 363
                    height: 28
                    text: title
                    font.bold: true
                    font.pixelSize: 20
                    font.weight: Font.Bold
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                }
                QLato {
                    width: 363
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
            source: "qrc:/Images/Images/right-arrow-dark.svg"
            width: 24
            height: 24
            anchors.verticalCenter: parent.verticalCenter
        }
    }
}
