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

Item {
    id: _item
    property string title: ""
    property var inheritance
    property bool isCancel: false
    property bool isOnChain: false

    Column {
        spacing: 16
        QLato {
            width: 656
            height: paintedHeight
            text: title
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            wrapMode: Text.WordWrap
            lineHeight: 28
            lineHeightMode: Text.FixedHeight
            visible: title.length > 0
        }
        Flickable {
            width: 539 + 12
            height: 452
            contentHeight: _col.height
            ScrollBar.vertical: ScrollBar { active: true }
            clip: true
            visible: !isCancel
            Column {
                id: _col
                spacing: 16
                Column {
                    spacing: 12
                    QLato {
                        width: 539
                        height: paintedHeight
                        text: isOnChain ? STR.STR_QML_1958 : STR.STR_QML_1954
                        font.weight: Font.Bold
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                    Rectangle {
                        width: 539
                        height: (inheritance.activation_timezone.length > 0 && inheritance.activation_date.length > 0) ? 70 : 52
                        color: "#F5F5F5"
                        Column {
                            anchors.fill: parent
                            anchors.margins: 16
                            spacing: 2
                            QLato {
                                width: 539
                                height: paintedHeight
                                text: inheritance.activation_date
                                font.weight: Font.Normal
                                horizontalAlignment: Text.AlignLeft
                                verticalAlignment: Text.AlignVCenter
                            }
                            QLato {
                                width: 539
                                height: paintedHeight
                                font.pixelSize: 12
                                text: inheritance.activation_timezone
                                font.weight: Font.Normal
                                horizontalAlignment: Text.AlignLeft
                                verticalAlignment: Text.AlignVCenter
                                visible: inheritance.activation_timezone.length > 0
                            }
                        }                        
                    }
                }
                Column {
                    spacing: 12
                    QLato {
                        width: 539
                        height: paintedHeight
                        text: STR.STR_QML_1047
                        font.weight: Font.Bold
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                    Rectangle {
                        width: 539
                        height: 52
                        color: "#F5F5F5"                        
                        QLato {
                            width: 539
                            anchors {
                                left: parent.left
                                leftMargin: 16
                                verticalCenter: parent.verticalCenter
                            }
                            height: paintedHeight
                            text: inheritance.note
                            font.weight: Font.Normal
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                }
                QInheritanceNotificationPreferencesOnChain {
                    id: pre
                    width: 539
                    inheritance: _item.inheritance
                }
            }
        }
    }
}
