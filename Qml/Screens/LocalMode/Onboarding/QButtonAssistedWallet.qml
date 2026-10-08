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
    width: 550
    height: 112
    color: "#FFFFFF"
    property string title: "Assisted wallet"
    property string optional: ""
    property string description: ""
    property string desImg: ""
    property string note: ""
    radius: 12
    Row {
        anchors {
            left: parent.left
            leftMargin: 16
            verticalCenter: parent.verticalCenter
        }
        spacing: 16
        QIcon {
            iconSize: 44
            source: desImg
            anchors.verticalCenter: parent.verticalCenter
        }
        Item {
            width: 458
            height: _col.childrenRect.height
            anchors.verticalCenter: parent.verticalCenter
            Column {
                id: _col
                width: parent.width
                spacing: 4
                Row {
                    width: parent.width
                    height: 20
                    spacing: 4
                    QLato {
                        width: paintedWidth
                        height: 20
                        text: title
                        font.pixelSize: 16
                        font.weight: Font.Bold
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignLeft
                    }
                    QLato {
                        width: paintedWidth
                        height: 20
                        text: optional
                        font.pixelSize: 16
                        font.weight: Font.Normal
                        color: "#757575"
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignLeft
                    }
                }
                QLato {
                    width: parent.width
                    height: paintedHeight
                    text: description
                    font.pixelSize: 16
                    wrapMode: Text.WordWrap
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    lineHeightMode: Text.FixedHeight
                    lineHeight: 20
                }
                Row {
                    width: parent.width
                    height: 20
                    spacing: 4
                    ColorOverlay {
                        anchors.verticalCenter: parent.verticalCenter
                        width: 20
                        height: 20
                        source: QIcon {
                            id: icon
                            iconSize: 20
                            source: "qrc:/Images/Images/info-60px.svg"
                        }
                        color: "#757575"
                    }
                    QLato {
                        width: parent.width
                        height: 20
                        text: note
                        font.pixelSize: 16
                        font.weight: Font.Normal
                        color: "#757575"
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignLeft
                    }
                }
            }
        }
    }
}
